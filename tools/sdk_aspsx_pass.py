#!/usr/bin/env python3
"""Sony workstation MIPS assembler (`as1` / SDK ASPSX) emulation pass (T-24086).

The precompiled PsyQ SDK library objects linked into `SLPM_86.053` (`asm/lib*.s`)
and the on-demand `LIBGTE.LIB(RATAN.OBJ)` module pulled into the end of `TT.EXN`
(`TT:func_8014D260`, `ratan2` + `ratan_tbl` at `D_80159B20`) were assembled by
Sony's MIPS workstation assembler (`as0` + `as1` derivative on Sony NEWS-OS)
rather than SN Systems' later DOS/Win32 `ASPSX.EXE` (modeled by `maspsx`).

Both GCC-compiled SDK objects (`gcc 2.7.2-psx` / `2.8.1-psx`, e.g. `TT/8014D260`,
`TAIIKU/801488F0`, `libsnd`, `libgte`, `libetc`, `libpress`, and the last 3
functions of `libspu`) and MIPS ucode `cc`-compiled SDK objects (the first 25
functions of `libspu`) share three uniform assembler expansion behaviors that
differ from `maspsx`:

1. Indexed symbol load expansion (`l* rd, sym(rs)` when `rd != rs`):
   When expanding `lb`/`lbu`/`lh`/`lhu`/`lw rd, sym(rs)` with `rd != rs`, MIPS
   `as1` (and GNU `as`) uses the destination register `rd` as the `%hi` base
   register (`lui rd, %hi(sym); addu rd, rd, rs; l* rd, %lo(sym)(rd)`) instead
   of `$at`. Only when `rd == rs` (where loading `%hi(sym)` into `rd` would
   clobber the index register `rs`) or on stores (`sb`/`sh`/`sw`) does it expand
   through `$at`. Measured across `asm/lib*.s`: 103 of 103 loads with `rd != rs`
   expand through `rd`; 53 of 53 loads with `rd == rs` and 99 of 99 stores
   expand through `$at`. Native MIPS `as0` + `as1` reproduces this directly on
   GCC 2.7.2-psx `.s` output.

2. Quotient/remainder move placement in expanded divides (`div`/`divu` + checks):
   When expanding a three-operand `div`/`rem` (`div $zero, rs, rt`) or
   `divu`/`remu` (`divu $zero, rs, rt`) with its divide-by-zero (`break 7`) and
   signed overflow (`break 6`) check sequence, the check sequence reads
   `{rs, rt}` for signed `div` and `{rt}` for unsigned `divu`:
   - When `rd` is NOT read by the check sequence (`rd not in {rs, rt}` for
     signed `div`, or `rd != rt` for unsigned `divu`), `mflo rd` / `mfhi rd` has
     no Write-After-Read (WAR) hazard with the check branches and is emitted
     immediately after `div $zero, rs, rt` (before `bnez rt`).
   - When `rd` IS read by the check sequence (`rd in {rs, rt}` for signed `div`,
     or `rd == rt` for unsigned `divu`), moving `mflo rd` / `mfhi rd` before the
     checks would clobber `rs` or `rt` before the zero/overflow checks test it,
     so `mflo rd` / `mfhi rd` stays at the end of the check sequence.
   - Unlike DOS/Win32 `ASPSX.EXE`, the workstation assembler never inserts a
     `nop # DEBUG: Reuse of ...` between a late `mflo rd` / `mfhi rd` and a
     following instruction that reads `rd` (16 of 16 late moves before an `rd`
     reader in `asm/lib*.s` have no nop before the reader).
   Measured across `asm/lib*.s` and `TT:func_8014D260`: matches 4 of 4 divides
   in `TT:func_8014D260` (2 early with `rd` distinct, 2 late with `rd in {rs, rt}`)
   and 63 of 67 expanded divide sites in `asm/lib*.s` (including 37/37 late
   `div` with `rd == rs`, 1/1 late `div` with `rd == rt`, 15/15 early `divu`,
   and 10/14 early `rem` with `rd` distinct).

3. Two-slot `mflo`/`mfhi` hazard window before labels, `.set noreorder` branches,
   and `mult`/`div`:
   MIPS `as1` ensures the two instruction slots after `mflo`/`mfhi` contain no
   `mult`/`div`/`mthi`/`mtlo` instruction and are not cut short by a basic-block
   label or by a GCC `.set noreorder` branch (whose delay slot was already
   filled by GCC), inserting the missing `nop`s immediately before that
   boundary. A `.set reorder` branch whose delay slot was filled with a `nop` by
   the assembler closes the two-slot window by itself (branch + delay-slot
   `nop` = 2 slots), and an early `mflo`/`mfhi` inside an expanded divide is
   followed by `bnez rt; nop`, which also closes the window. Measured across
   `asm/lib*.s`: 290 of 290 sites follow this exact rule, and native MIPS
   `as0` + `as1` reproduces both the `mflo a0; nop; nop; j $L; sll v0, a0, 1`
   and `mflo a0; sll v0, a0, 1; nop; $L:` patterns of `TT:func_8014D260`.

This pass runs uniformly on `maspsx` output in `tools/cc.py` (`compile_gcc`),
with no per-function switches or source markers (CODING_STANDARDS section 7a).
"""
import re
import sys

LUI_AT_RE = re.compile(r"^(\s*)lui\s+\$at,\s*%hi\(([^)]+)\)\s*$")
ADDU_AT_RE = re.compile(r"^(\s*)addu\s+\$at,\s*\$at,\s*(\$\w+)\s*$")
LOAD_AT_RE = re.compile(
    r"^(\s*)(lb|lbu|lh|lhu|lw)\s+(\$\w+),\s*%lo\(([^)]+)\)\(\$at\)\s*$"
)
DIV_ZERO_RE = re.compile(r"^(\s*)(div|divu)\s+\$zero,\s*(\$\w+),\s*(\$\w+)\s*$")
MF_RE = re.compile(r"^\s*(mflo|mfhi)\s+(\$\w+)\s*$")
REUSE_NOP_RE = re.compile(r"^\s*nop\s*#\s*DEBUG:\s*Reuse of\b")
LABEL_RE = re.compile(r"^(\$?\w+|\.L\w+):")
INS_RE = re.compile(r"^\s*([a-zA-Z]\w*)\b")

BRANCH_OPS = {
    "b", "j", "jr", "jal", "jalr",
    "beq", "bne", "beqz", "bnez", "bgez", "bgtz", "blez", "bltz",
}
MULDIV_OPS = {"mult", "multu", "div", "divu", "mthi", "mtlo"}


def _is_label(line):
    return bool(LABEL_RE.match(line.strip()))


def _insn_op(line):
    s = line.strip()
    if not s or s.startswith("#") or s.startswith(".") or _is_label(s):
        return None
    m = INS_RE.match(s)
    return m.group(1) if m else None


def rewrite(maspsx_text):
    """Rewrite `maspsx` assembly text according to the three workstation assembler rules."""
    lines = maspsx_text.splitlines()

    # Rule 1: `lui $at, %hi(sym); addu $at, $at, rs; l* rd, %lo(sym)($at)` -> through `rd` when `rd != rs`.
    i = 0
    while i + 2 < len(lines):
        m1 = LUI_AT_RE.match(lines[i])
        m2 = ADDU_AT_RE.match(lines[i + 1])
        m3 = LOAD_AT_RE.match(lines[i + 2])
        if m1 and m2 and m3 and m1.group(2) == m3.group(4):
            sym, idx_reg, op, rd = m1.group(2), m2.group(2), m3.group(2), m3.group(3)
            if rd != idx_reg:
                lines[i] = "%slui\t%s,%%hi(%s)" % (m1.group(1), rd, sym)
                lines[i + 1] = "%saddu\t%s,%s,%s" % (m2.group(1), rd, rd, idx_reg)
                lines[i + 2] = "%s%s\t%s,%%lo(%s)(%s)" % (m3.group(1), op, rd, sym, rd)
                i += 3
                continue
        i += 1

    # Rule 2: In `maspsx`-expanded `div`/`divu` blocks, hoist `mflo`/`mfhi` before `bnez`
    # when the check sequence does not read `rd`, and drop `maspsx`'s `Reuse of` nop.
    out = []
    i = 0
    while i < len(lines):
        m_div = DIV_ZERO_RE.match(lines[i])
        if m_div:
            _, div_op, rs, rt = m_div.groups()
            j = i + 1
            mf_idx = None
            mf_rd = None
            while j < min(i + 20, len(lines)):
                m_mf = MF_RE.match(lines[j])
                if m_mf:
                    mf_idx = j
                    mf_rd = m_mf.group(2)
                    break
                j += 1
            if mf_idx is not None:
                check_lines = lines[i + 1:mf_idx]
                mf_line = lines[mf_idx]
                next_idx = mf_idx + 1
                while next_idx < len(lines) and (
                    not lines[next_idx].strip()
                    or lines[next_idx].strip().startswith("#")
                    or lines[next_idx].strip().startswith(".set")
                ):
                    next_idx += 1
                if next_idx < len(lines) and REUSE_NOP_RE.match(lines[next_idx]):
                    lines.pop(next_idx)
                check_reads = {rs, rt} if div_op == "div" else {rt}
                if mf_rd not in check_reads:
                    out.append(lines[i])
                    out.append(mf_line)
                    out.extend(check_lines)
                else:
                    out.append(lines[i])
                    out.extend(check_lines)
                    out.append(mf_line)
                i = mf_idx + 1
                continue
        out.append(lines[i])
        i += 1
    lines = out

    # Rule 3: Ensure 2 instruction slots after `mflo`/`mfhi` before a label, a GCC
    # `.set noreorder` branch (non-nop delay slot), or a `mult`/`div` instruction.
    i = 0
    while i < len(lines):
        op = _insn_op(lines[i])
        if op in ("mflo", "mfhi"):
            real_count = 0
            j = i + 1
            boundary_at = None
            while j < len(lines) and real_count < 2:
                s = lines[j].strip()
                if _is_label(s):
                    boundary_at = j
                    break
                iop = _insn_op(lines[j])
                if iop is not None:
                    if iop in MULDIV_OPS:
                        boundary_at = j
                        break
                    if iop in BRANCH_OPS:
                        k = j + 1
                        while k < len(lines) and _insn_op(lines[k]) is None:
                            k += 1
                        ds_op = _insn_op(lines[k]) if k < len(lines) else "nop"
                        if ds_op != "nop":
                            b_pos = j
                            while b_pos > i + 1 and lines[b_pos - 1].strip() in (
                                ".set\tnoreorder", ".set noreorder",
                                ".set\tnomacro", ".set nomacro", "",
                            ):
                                b_pos -= 1
                            boundary_at = b_pos
                        break
                    real_count += 1
                j += 1
            if boundary_at is not None and real_count < 2:
                nops_needed = 2 - real_count
                for _ in range(nops_needed):
                    lines.insert(boundary_at, "\tnop")
                i = boundary_at + nops_needed
                continue
        i += 1

    return "\n".join(lines) + ("\n" if maspsx_text.endswith("\n") or lines else "")


def main(argv):
    if len(argv) > 2:
        sys.exit("Usage: sdk_aspsx_pass.py [input.s]")
    if len(argv) == 2:
        with open(argv[1]) as f:
            text = f.read()
    else:
        text = sys.stdin.read()
    sys.stdout.write(rewrite(text))


if __name__ == "__main__":
    main(sys.argv)
