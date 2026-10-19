#!/usr/bin/env python3
"""Block-copy direction state pass for IDO 5.3 ugen (T-24092).

IDO 5.3's ugen selects between a forward (`F`) and backward (`B`) `lwl`/`lwr`/
`swl`/`swr` loop in `f_eval_mov` (`Umov`, struct/block copy with size > 32 and
alignment < 4) by reading an uninitialized byte on its own stack frame at
`sp + 240` (`0xfffe0c0`), which holds the high halfword of whatever 32-bit value
a preceding ugen routine left in that stack slot (`< 0x7ff8` -> forward `F`,
`>= 0x7ff8` -> backward `B`).

In the original compiler that built Tokimeki Memorial, `Umov` uses backward (`B`)
at procedure entry, after calls, after direct stores (`Ustr`), and after ordinary
loops or branches (`Ufjp`/`Utjp`), and switches to forward (`F`) only when the
procedure has executed an indirect store (`Uistr`/`Uistv`) either immediately
before the copy or inside a preceding counted loop whose branch emits a signed
immediate `<` comparison (`zblt` with positive immediate, `for (i = 0; i < N; i++)`).
Details and census: wiki/matching-notes.md, wiki/toolchain.md.

In stock IDO 5.3 `ugen`, `f_jump` (`Ufjp`/`Utjp`) happens to leave `0` (`< 0x7ff8`)
in `0xfffe0c0` on non-`zblt` branches, flipping every post-branch block copy to `F`.

This shim wraps `/opt/ido/5.3/ugen` without modifying the binary:
  1. Before running `ugen`, for each procedure in `u.O` that contains `Umov`, it
     tracks the block-copy direction state (`B` or `F`) and injects a zero-cost
     sentinel store (`Uldc 0; Ustr M -0x7777` for `B`, or `Uldc 0; Uldc 0;
     Uistr -0x7777` for `F`) immediately before the two address operands of each
     `Umov`. Because the stored value and base are hardware `$zero`, `ugen`
     allocates zero registers, leaves CSE and register allocation untouched,
     writes `0x10000` (`B`) or `0x57` (`F`) into `0xfffe0c0`, and emits a single
     identifiable `zsb $zero, offset($sp/$zero)` binasm record right before the
     block copy.
  2. After `ugen` produces the `.G` binasm file, the shim strips the sentinel
     `zsb $zero` records before `frame_pass.py` / `as1` runs.

Procedures without `Umov` are not modified. Anything the pass cannot parse or
delimit exactly aborts with PassError.

Usage as the ugen shim:  ugen_pass.py --ugen /opt/ido/5.3/ugen <ugen args...>
"""
import os
import struct
import subprocess
import sys
import tempfile

# Uopcode enum (ucode.h), in order.
OPS = """abs add adj aent and aos asym bgn bgnb bsub cg1 cg2 chkh chkl chkn chkt
cia clab clbd comm csym ctrl cubd cup cvt cvtl dec def dif div dup end
endb ent eof equ esym fill fjp fsym geq grt gsym hsym icuf idx iequ igeq
igrt ijp ilda ildv ileq iles ilod inc ineq init inn int ior isld isst istr
istv ixa lab lbd lbdy lbgn lca lda ldap ldc ldef ldsp lend leq les lex
lnot loc lod lsym ltrm max min mod mov movv mpmv mpy mst mus neg neq
nop not odd optn par pdef pmov pop regs rem ret rlda rldc rlod rnd rpar
rstr sdef sgs shl shr sign sqr sqrt ssym step stp str stsp sub swp tjp
tpeq tpge tpgt tple tplt tpne typ ubd ujp unal uni vreg xjp xor xpar mtag
alia ildi isti irld irst ldrc msym rcuf ksym osym irlv irsv""".split()
OP = {name: i for i, name in enumerate(OPS)}

_LEN4 = """adj bgn cia clab ctrl comm cup cvt def dif int uni ent aent iequ igeq igrt
ileq iles ineq fill ildv ilod inn istv istr lab ldef lca ldc lod mov mus optn
par pdef regs rlda rldc rlod rnd rpar rstr sdef asym hsym sgs str swp csym esym
fsym gsym lsym icuf isld isst pmov mpmv vreg typ ssym rcuf ldrc msym ksym osym
ildi isti irld irst""".split()
LENGTH = {OP[n]: 4 for n in _LEN4}
LENGTH.update({OP["init"]: 6, OP["ilda"]: 6, OP["lda"]: 6, OP["xjp"]: 8})
HASCONST = {OP[n] for n in "cia comm init lca ldc rldc ssym".split()}

DTYPES = "ACFGHIJKLMNPQRSWXZ"
DT_A = DTYPES.index("A")
DT_I = DTYPES.index("I")
DT_J = DTYPES.index("J")
DT_L = DTYPES.index("L")
STRING_DTYPES = {DTYPES.index(c) for c in "MQRSX"}
MT_M = 1
MT_R = 3

# Stack-pushing expression operators: number of expression operands popped
# (each pushes 1 result onto ugen's expression stack).
EXPR_POP = dict(
    abs=1, add=2, adj=1, And=2, bsub=2, cvt=1, cvtl=1, dec=1, dif=2, div=2,
    equ=2, geq=2, grt=2, ilda=1, ildv=1, ilod=1, inc=1, inn=2, int=2, ior=2,
    isld=1, ixa=2, lda=0, ldap=0, ldc=0, ldsp=0, leq=2, les=2, lnot=1, lod=0,
    max=2, min=2, mod=2, mpy=2, mus=2, neg=1, neq=2, Not=1, odd=1, rem=2,
    rnd=1, sgs=1, shl=2, shr=2, sign=2, sqr=1, sqrt=1, sub=2, uni=2, xor=2,
)
EXPR_POP = {OP[k.lower()]: v for k, v in EXPR_POP.items()}

# Non-pushing declarations/preload records that may appear between operands.
SKIP_OPS = {
    OP[n] for n in (
        "vreg loc rldc rlod rlda clbd cubd lbgn lend lbdy ltrm unal nop"
    ).split()
}

SENTINEL_OFF = (-0x7777) & 0xFFFFFFFF

# binasm constants (binasm.h)
I_IOCODE = 23
I_IASCII = 5
I_IASCIIZ = 6
F_FROB = 0
Z_ZSB = 70
SP = 29


class PassError(Exception):
    """The pass met something it cannot transform exactly."""


def s32(x):
    return x - (1 << 32) if x & 0x80000000 else x


class Insn:
    """One ucode record: its 32-bit words and any trailing string bytes."""

    __slots__ = ("words", "extra")

    def __init__(self, words, extra=b""):
        self.words = list(words)
        self.extra = extra

    @property
    def opc(self):
        return self.words[0] >> 24

    @property
    def dtype(self):
        return (self.words[0] >> 16) & 0x1F

    @property
    def mtype(self):
        return (self.words[0] >> 21) & 7


def parse_ucode(data):
    """List of Insn for a binary ucode file; raises PassError if malformed."""
    if len(data) % 4:
        raise PassError("ucode size is not a multiple of 4")
    n = len(data) // 4
    words = struct.unpack(">%dI" % n, data)
    out, p = [], 0
    while p < n:
        opc = words[p] >> 24
        if opc >= len(OPS):
            raise PassError("unknown ucode opcode %d at word %d" % (opc, p))
        length = LENGTH.get(opc, 2)
        if opc in HASCONST:
            length += 2
        if p + length > n:
            raise PassError("truncated ucode record at word %d" % p)
        rec = words[p:p + length]
        p += length
        extra = b""
        if opc in HASCONST:
            dtype = (rec[0] >> 16) & 0x1F
            if dtype in STRING_DTYPES or opc == OP["comm"]:
                nwords = (rec[LENGTH.get(opc, 2)] + 3) // 4
                nwords += nwords & 1
                if p + nwords > n:
                    raise PassError("truncated ucode string at word %d" % p)
                extra = data[p * 4:(p + nwords) * 4]
                p += nwords
        out.append(Insn(rec, extra))
    return out


def serialize_ucode(insns):
    return b"".join(
        struct.pack(">%dI" % len(i.words), *i.words) + i.extra for i in insns
    )


def _procedures(insns):
    """[(index of ENT, index of END)] of every procedure."""
    procs, start = [], None
    for k, i in enumerate(insns):
        if i.opc == OP["ent"]:
            start = k
        elif i.opc == OP["end"] and start is not None:
            procs.append((start, k))
            start = None
    return procs


def _mov_insert_index(insns, start, mov_idx):
    """Index of the first instruction of `Umov`'s two address operands."""
    need = 2
    k = mov_idx - 1
    while k > start:
        opc = insns[k].opc
        if opc in SKIP_OPS:
            k -= 1
            continue
        pops = EXPR_POP.get(opc)
        if pops is None:
            raise PassError(
                "unexpected opcode %s in Umov operand at record %d"
                % (OPS[opc], k)
            )
        need += pops - 1
        if need == 0:
            while k - 1 > start and insns[k - 1].opc == OP["unal"]:
                k -= 1
            return k
        k -= 1
    raise PassError("cannot delimit operands of Umov at record %d" % mov_idx)


def _is_signed_blt_pos_imm_jump(insns, start, jump_idx):
    """True if the fjp/tjp at `jump_idx` emits `zblt` (`s3 == 0x17`) with imm > 0."""
    fjp_flag = insns[jump_idx].opc == OP["fjp"]
    idx = jump_idx - 1
    while idx > start and insns[idx].opc in SKIP_OPS:
        idx -= 1
    while idx > start and insns[idx].opc == OP["lnot"]:
        fjp_flag = not fjp_flag
        idx -= 1
        while idx > start and insns[idx].opc in SKIP_OPS:
            idx -= 1
    if idx <= start:
        return False
    cmp_ins = insns[idx]
    if cmp_ins.dtype not in (DT_I, DT_J):
        return False
    rhs_idx = idx - 1
    while rhs_idx > start and insns[rhs_idx].opc in SKIP_OPS:
        rhs_idx -= 1
    rhs = insns[rhs_idx]
    if rhs.opc != OP["ldc"]:
        return False
    c = s32(rhs.words[4])
    if not fjp_flag:
        if cmp_ins.opc == OP["les"]:
            return c > 0
        if cmp_ins.opc == OP["leq"]:
            return c + 1 > 0
    else:
        if cmp_ins.opc == OP["geq"]:
            return c > 0
        if cmp_ins.opc == OP["grt"]:
            return c + 1 > 0
    return False


def _gadget_b(proc_block):
    """Zero-register ucode statement that sets 0xfffe0c0 = 0x10000 >= 0x7ff8 (State B)."""
    return [
        Insn([(OP["ldc"] << 24) | (DT_L << 16), 0, 4, 0, 0, 0]),
        Insn([(OP["str"] << 24) | (MT_M << 21) | (DT_L << 16), proc_block, 1, SENTINEL_OFF]),
    ]


def _gadget_f():
    """Zero-register ucode statement that sets 0xfffe0c0 = 0x57 < 0x7ff8 (State F)."""
    return [
        Insn([(OP["ldc"] << 24) | (DT_A << 16), 0, 4, 0, 0, 0]),
        Insn([(OP["ldc"] << 24) | (DT_L << 16), 0, 4, 0, 0, 0]),
        Insn([(OP["istr"] << 24) | (DT_L << 16) | 8, SENTINEL_OFF, 1, 0]),
    ]


def rewrite_ucode(insns):
    """Inject State B/F gadgets before Umov records; returns (new_insns, changed)."""
    if not any(i.opc == OP["mov"] for i in insns):
        return insns, False

    insertions = {}  # record index -> list of gadget Insn
    for start, end in _procedures(insns):
        if not any(insns[k].opc == OP["mov"] for k in range(start, end)):
            continue
        proc_block = insns[start].words[1]
        seen_uist = False
        uist_rill_loop = False
        last_state_op = "ent"

        for k in range(start + 1, end):
            ins = insns[k]
            opc = ins.opc
            if opc in (OP["istr"], OP["istv"]):
                seen_uist = True
                last_state_op = "uist"
            elif opc == OP["str"] and ins.mtype != MT_R:
                last_state_op = "ustr"
            elif opc in (OP["fjp"], OP["tjp"]):
                if seen_uist and _is_signed_blt_pos_imm_jump(insns, start, k):
                    uist_rill_loop = True
                last_state_op = "jump"
            elif opc == OP["mov"]:
                want_f = uist_rill_loop or (last_state_op == "uist")
                ins_idx = _mov_insert_index(insns, start, k)
                gadget = _gadget_f() if want_f else _gadget_b(proc_block)
                insertions.setdefault(ins_idx, []).extend(gadget)

    if not insertions:
        return insns, False

    out = []
    for k, ins in enumerate(insns):
        if k in insertions:
            out.extend(insertions[k])
        out.append(ins)
    return out, True


def _is_sentinel_binasm(w1, w2, w3):
    """True if the 16-byte binasm record is our injected `zsb $zero` sentinel."""
    instr = (w1 >> 16) & 0x3F
    op = (w1 >> 1) & 0x1FF
    form = (w2 >> 14) & 0xF
    reg1 = (w2 >> 25) & 0x7F
    reg2 = (w2 >> 18) & 0x7F
    imm = s32(w3)
    return (
        instr == I_IOCODE
        and op == Z_ZSB
        and form == F_FROB
        and reg1 == 0
        and reg2 in (0, SP)
        and imm <= -0x4000
    )


def strip_sentinel_binasm(data):
    """Remove sentinel `zsb $zero` records from ugen's `.G` binasm output."""
    if len(data) % 16:
        raise PassError("binasm size %d is not a multiple of 16" % len(data))
    out, i, n_stripped = [], 0, 0
    while i < len(data):
        rec_bytes = data[i:i + 16]
        w0, w1, w2, w3 = struct.unpack(">4I", rec_bytes)
        i += 16
        instr = (w1 >> 16) & 0x3F
        payload = b""
        if instr in (I_IASCII, I_IASCIIZ):
            n = -(-w2 // 16) * 16
            payload = data[i:i + n]
            i += n
        if _is_sentinel_binasm(w1, w2, w3):
            n_stripped += 1
            continue
        out.append(rec_bytes)
        if payload:
            out.append(payload)
    if n_stripped == 0:
        raise PassError("expected sentinel binasm record(s) in ugen output, found 0")
    return b"".join(out)


VALUED = {"-G", "-o", "-t", "-temp", "-f", "-l", "-Olimit"}


def run_ugen(real, argv):
    """Rewrite ugen's input ucode if it contains Umov, run real ugen, strip sentinels."""
    inp, outp, i = None, None, 0
    while i < len(argv):
        if argv[i] == "-o" and i + 1 < len(argv):
            outp = i + 1
            i += 2
        elif argv[i] in VALUED:
            i += 2
        elif argv[i].startswith("-"):
            i += 1
        else:
            if inp is None:
                inp = i
            i += 1
    if inp is None or outp is None:
        raise PassError("ugen arguments name no input ucode and -o output")

    with open(argv[inp], "rb") as f:
        insns = parse_ucode(f.read())
    new_insns, changed = rewrite_ucode(insns)
    if not changed:
        return subprocess.call([real] + list(argv))

    fd, tmp_o = tempfile.mkstemp(prefix="ugenpass", suffix=".O")
    with os.fdopen(fd, "wb") as f:
        f.write(serialize_ucode(new_insns))
    args = list(argv)
    args[inp] = tmp_o
    try:
        rc = subprocess.call([real] + args)
    finally:
        os.unlink(tmp_o)
    if rc == 0:
        with open(argv[outp], "rb") as f:
            g_data = f.read()
        with open(argv[outp], "wb") as f:
            f.write(strip_sentinel_binasm(g_data))
    return rc


def main(argv):
    if len(argv) < 3 or argv[1] != "--ugen":
        sys.exit(__doc__)
    try:
        sys.exit(run_ugen(argv[2], argv[3:]))
    except PassError as e:
        sys.exit("ugen_pass.py: %s" % e)


if __name__ == "__main__":
    main(sys.argv)
