---
type: concept
updated: 2026-10-18
sources: ["tools/gcc_fingerprint.py", "tools/aspsx_survey.py", "tools/test_gcc_fingerprint.py", "config/toolchains.txt", "configure.py", "tools/cc.py", "tools/Dockerfile"]
---

# PsyQ-gcc-built objects inside overlays (T-9200)

The game code is IDO output, but some overlay objects are PsyQ SDK code (gcc 2.x + ASPSX, or hand-written libgte assembly). They cannot be matched with IDO. This page records how they are found, which toolchain each needs, and what is known. Tool and build rules: [[toolchain]], [[build-system]].

## Fingerprint (`tools/gcc_fingerprint.py`)
Two instruction habits that IDO never produces, measured on the matched IDO functions of the whole tree (4521 functions, 0 hits each) and present all over the SDK asm of the main exe (libgte, libgpu, libsnd, libspu, libcd, libetc, libgs, libpress, libapi: e.g. 50 of 96 libgpu and 38 of 61 libcd functions):
- `move` spelled `addu rd,rs,$zero` (IDO: `or rd,rs,$zero`);
- `j .Lxxxx` to a label inside the function (IDO: `b`).
A hit needs a local jump, or one move per 32 instructions (three IDO-shaped functions of 230 to 1100 instructions hold one or two `addu` moves: `get_fnt`, ETC `func_801328A4`, NAME_ENT `func_80143580`). Limits: it cannot tell gcc C from hand-written assembly, and it misses gcc code with neither habit. Run `tools/docker.sh python3 tools/gcc_fingerprint.py [--matched]` after a split (`--matched` is the control, 0 hits).

## Hits (all overlays, 2026-10-10)
| function | object | verdict |
|---|---|---|
| TAIIKU `func_801488F0` | `TAIIKU/801488F0` | gcc 2.8.1-psx C, matched (game code, not SDK: it calls SDK functions) |
| TT `func_8014D260` | `TT/8014D260` | gcc C (an `ratan2`-style lookup: abs of both arguments, divide, table). Structure matches gcc 2.7.2/2.6.3-psx up to the divisions; not matched, see below |
| TACO `func_8015CF30`, `func_8015D0D0`, `func_8015D270`; DATE `func_8015A640`, `func_8015A7E0`, `func_8015A980` | six identical-shape rotation helpers (`RotMatrixX/Y/Z` on the sine table `D_800DE500`) | not compiler output: `$t7`-first registers, `multu` for signed products, `mflo; nop; nop` before the next `multu`. gcc 2.6.3 to 2.91.66 give `$v0`-first registers and `mult` for the same C. Hand-written libgte assembly (ASPSX); they stay `INCLUDE_ASM` |

Not gcc: TACO `func_801420DC` and `func_801421AC` (listed as gcc-shaped by wave 6; the fingerprint does not flag them). They are IDO code and match with plain IDO C (`if (D->unk80 == 3 || D->unk80 == 4)` chains; constants in registers, `nop` after `beq`), see [[matching-notes]].

## Which gcc
Chosen by compiling the C and comparing bytes. `func_801488F0`: 2.7.2-psx and 2.6.3-psx end the function with `addu $sp; j $31; nop`, while the original has `jr $ra` with the stack release in its delay slot; gcc 2.8.0-psx and 2.8.1-psx print that epilogue (`.set noreorder`) and match byte for byte (ASPSX version 2.79). 2.8.0 and 2.8.1 are not told apart by this function; 2.8.1 is used.

## `func_8014D260` (open, T-11890: the original is not ASPSX output of gcc 2.7.2)
With gcc 2.7.2-psx / 2.6.3-psx and maspsx `--expand-div`, the C in [[tickets/T-9200-gcc-built-sdk-objects-in-overlays]] is identical to the original up to the first division and has the same block structure, but is 4 bytes shorter. The divisions differ in layout: ASPSX expands `div rd,rs,rt` as `div $zero,rs,rt; mflo rd; <checks>` when `rd` is neither `rs` nor `rt` and as `div; <checks>; mflo rd` otherwise (original: four divisions, two of each kind), and puts the `mflo` hazard nops in other places (`mflo a0; nop; nop; j; sll v0,a0,1` against `mflo a0; nop; sll`). GNU as always puts the `mflo` after the checks. A pass in front of `maspsx` could model it; one function is too little evidence for CODING_STANDARDS 7a, so this waits for a second division site in gcc-built code.

T-11800 (2026-10-15) tried every existing combination: gcc 2.6.3-psx, 2.7.2-cdk, 2.7.2-psx, 2.8.0-psx, 2.8.1-psx and
2.91.66-psx (2.95.2-psx's cpp rejects `-undef`) against maspsx `--aspsx-version` 2.21, 2.34, 2.56, 2.67, 2.77, 2.79, 2.81
and 2.86, each with and without `--expand-div`. None matches; the best is 96 instructions against 97 (2.6.3/2.7.2-psx,
aspsx >= 2.56, `--expand-div`). The C is a standard PsyQ `ratan2(y, x)` (sign flags, `x == 0 && y == 0` return,
`(y & 0x7FE00000) ? y / (x >> 10) : (y << 10) / x`, table `D_80159B20`, `1024 - tbl`, `2048 - c`, negate) and is identical
to the original up to the first division. Three ASPSX behaviours maspsx does not model: (1) `div; mflo; checks` when rd is
neither rs nor rt (maspsx: `div; checks; mflo`); (2) the `mflo` hazard nops (`mflo a0; nop; nop; j; sll` and
`mflo a0; sll v0,a0,1; nop`); (3) `lh $3,sym($2)` expanded through the destination register
(`lui v1,%hi; addu v1,v1,v0; lh v1,%lo(v1)`), where maspsx always uses `$at`. Matching it needs a maspsx change.

T-11890 (2026-10-18): the C is settled. With the quotient in its own variable (`q`, so the table value lands in `$v1`
as in the original), gcc 2.7.2-psx gives the original's registers, and applying the three habits by hand to the maspsx
output (scratch only) makes all 97 instructions identical to the original apart from relocation fields. The draft:

```c
s32 func_8014D260(s32 y, s32 x) {
    s32 neg_x, neg_y, q, v;
    neg_x = 0; neg_y = 0;
    if (x < 0) { neg_x = 1; x = -x; }
    if (y < 0) { neg_y = 1; y = -y; }
    if (x == 0 && y == 0) return 0;
    if (y < x) {
        if (y & 0x7FE00000) q = y / (x >> 10); else q = (y << 10) / x;
        v = D_80159B20[q];
    } else {
        if (x & 0x7FE00000) q = x / (y >> 10); else q = (x << 10) / y;
        v = 1024 - D_80159B20[q];
    }
    if (neg_x) v = 2048 - v;
    if (neg_y) v = -v;
    return v;
}
```

No build pass was written: the real ASPSX does none of the three habits (sections below), so the object stays
`INCLUDE_ASM` (CODING_STANDARDS 7a; [[tickets/T-11890-tt-ratan2-aspsx]]).

## ASPSX habits in the SDK asm (T-11890)
Measured with `tools/docker.sh python3 tools/aspsx_survey.py` over every `asm/lib*.s` (the PsyQ library code of the
main exe, all ASPSX-assembled except `libc_sprintf`, an older non-gcc build, and the hand-written libgte matrix code).
Counts are sites; examples are in the script output.

**1. Quotient/remainder move of an expanded divide (not a uniform rule).** 67 expanded sites with a move (14 more
`div`/`divu $zero` sites have no check sequence, one has no move):

| site | early (move right after `div $zero`) | late (move after the checks) |
|---|---|---|
| `div`, `mflo`, rd = rs | 0 | 37 |
| `div`, `mfhi`, rd = rt | 0 | 1 |
| `div`, `mfhi` (rem), rd distinct | 10 | 4 (all in libspu `func_80098D00`, rt = `$t7` in each) |
| `div`, `mflo` (quotient), rd distinct | 0 | 0 |
| `divu`, `mflo`/`mfhi`, rd = rs | 7 | 0 |
| `divu`, `mflo`/`mfhi`, rd distinct | 8 | 0 |

The candidate rule "the move goes right after the divide unless the checks read rd (rs and rt for `div`, rt for
`divu`), otherwise after them" fits 63 of 67 sites. Against it: the 4 late `rem` sites (one of them, libspu `80098F2C`,
has nothing else inside the expansion); no SDK site at all for the case TT needs (signed quotient, rd distinct); and
the ASPSX fixture in the pinned maspsx (`aspsx/fixtures/div.yaml`, real ASPSX output for `div $2,$4,$6`) puts the
`mflo` after the checks in every version from 1.07 to 2.86. In 44 of the 67 sites ASPSX has moved neighbouring
instructions into the expansion (between `div` and `bnez`, or between `break 6` and the move), so the move position
looked like the work of an ASPSX instruction scheduler; the real ASPSX (next section) shows it is not ASPSX at all.
See "The real ASPSX on synthetic input" below.

**2. `mflo`/`mfhi` hazard nops (uniform in the SDK asm; not ASPSX, see below).** Rule: the two instruction slots after an `mflo`/`mfhi` hold no
`mult`/`div`-class instruction, and the window is not cut by a label or by a branch from a gcc `.set noreorder`
block (delay slot filled by gcc); the missing nops sit right before that boundary. A branch whose delay slot
ASPSX filled with a nop closes the window itself (branch + nop = two slots). 290 of 290 sites as predicted (153
before mult/div, 15 before a label, 21 before a gcc-filled branch, 30 before an ASPSX-filled branch, 70 with no
boundary in reach; the one site with a nop and no boundary, libspu `800996A4`, is a load-delay nop). Not counted:
3 sites in `libc_sprintf` and the hand-written libgte `mtx` files (no ASPSX padding at all). An early move inside a
divide expansion is followed by the checks (`bnez; nop`), which close the window (all 25 early moves fit). maspsx pads only
before mult/div. It also puts a nop between a late move and a reader of rd (`Reuse of`); the SDK has 16 late moves
followed directly by a reader of rd and none with a nop.

**3. `lb/lbu/lh/lhu/lw rd,sym(rs)` (uniform in the SDK asm; not ASPSX, see below).** 103 of 103 loads with rd != rs expand through rd (`lui rd,%hi(sym);
addu rd,rd,rs; l* rd,%lo(sym)(rd)`); 53 of 53 loads with rd == rs and 99 of 99 stores go through `$at`, as maspsx does
for all of them. The maspsx fixture `expand_lw.yaml` (`lw $2,ctlbuf($2)`, rd == rs) agrees.

## The real ASPSX on synthetic input (T-11890)
The ASPSX binaries of the maspsx author's release (`aspsx-binaries.tar.gz`, 13 versions) were run under wine in a throwaway
container outside the repo (measurement only; no binary or output is committed). Versions 2.56, 2.67, 2.77, 2.79,
2.81 and 2.86 (the Win32 ones; 1.05 to 2.34 need dosemu2 and were not run: maspsx's fixtures show that versions
before 2.30 do not pad before `mult` at all, which the SDK does 153 times) give the same output for everything below.

- **Divide (36 synthetic cases, x 2 versions):** `div`, `divu` and `rem` with rd distinct, rd = rs and rd = rt, each
  followed by a reader of rd, an independent instruction, a label, or a gcc `.set noreorder` branch. In every case
  the move comes after the checks, and a reader of rd gets a nop after the move (`mflo v0; nop; addu v1,v0,a1`).
  The SDK has 16 late moves directly followed by a reader of rd and no nop, and moves before the checks in 25 sites.
- **Hazard nops:** `mflo` then `mult` gets two nops (as maspsx); `mflo` before a label, before a gcc noreorder
  branch, or before a reorder branch gets none.
- **`l* rd,sym(rs)`:** every load (`lh`, `lw`, `lbu`, `lwl`, rd = rs or not) and every store goes through `$at`.
- **The function itself:** the gcc 2.7.2-psx output of the draft C assembles to the same 96 instructions in all six
  versions, equal to the maspsx path apart from relocation fields; the original has 97.

So maspsx models the real ASPSX correctly here, and none of the three habits of the SDK asm and TT `func_8014D260` is
ASPSX behaviour on gcc's macro output (the accepted options `-a -f -g -u -v -x -D -K -L -R -W -q` change nothing; `-0`
drops the checks). Most likely the original division code, address loads and nops came out of the
compiler already expanded (explicit `div $zero`, checks and `mflo` that the compiler scheduled; explicit
`%hi`/`%lo` through the destination; the compiler's own hazard nops), from a compiler build none of the old-gcc
releases we have reproduces (2.6.3 to 2.95.2 all print the `div rd,rs,rt` macro or `div $0` + `mflo` without checks).
An ASPSX emulation pass would model something ASPSX does not do, so none is built.

## Sony/SN cc1psx (PsyQ 3.3 to 4.6) on the same input (T-11890)
The PsyQ compiler binaries that decomp.me uses (mkst/esa release `psyq-binaries`: `psyq-compilers.tar.gz` with 4.0,
4.1, 4.3, 4.5, 4.6, and `psyq3.3`/`psyq3.5`/`psyq3.6`) were run in throwaway containers outside the repo
(measurement only, nothing committed): the Win32 4.x `CC1PSX.EXE` + `ASPSX.EXE` under wine, the DOS 3.x ones under
dosemu2 with the archive's `dosemurc`. Input: the ratan2 draft preprocessed with old-gcc cpp, and small synthetic C
functions (quotient and remainder with rd distinct and rd = rs, unsigned divide, a `short` table load by index,
`mult` + `mflo` before a branch).

| PsyQ | cc1psx | ratan2 (original: 97 instructions) |
|---|---|---|
| 3.3 | GNU C 2.6.0 (SN) | 98, 41 differing words (ASPSX 2.21: `li` as `ori`, `addiu $at` load form) |
| 3.5, 3.6 | GNU C 2.6.0 (SN) | 96, 41 differing words |
| 4.0 | GNU C 2.7.2.SN32.3.7.0002 | 96, identical to our gcc 2.7.2-psx + maspsx path (relocations aside) |
| 4.1, 4.3, 4.5 | cygnus-2.7.2 SN32 | 95, more different (compiler splits `%hi`/`%lo` and indexes through another register) |
| 4.6 | gcc 2.8 class | 71 |

What the compilers print (`.s`, all versions; `-O1`, `-O3`, `-mno-gas`, `-fschedule-insns`, `-mips2`,
`-membedded-pic`, `-mlong-calls` change nothing for 4.0):
- 3.3 to 4.5 print the `div rd,rs,rt` macro; ASPSX expands it with the move after the checks, as measured above.
  4.6 prints `div $0,rs,rt; mflo rd` and only a zero check (`bne rt,$0,1f; nop; break 7`), without the overflow check
  the original has.
- 3.3 to 4.0 print `lh rd,sym(rs)`, which ASPSX expands through `$at`; 4.1 and later split the address with another
  register (`lui`/`addiu` of the full address, then `addu` with the index and `lh rd,0(...)`). None gives the
  original's `lui rd,%hi; addu rd,rd,rs; l* rd,%lo(sym)(rd)`.
- No version prints its own hazard nops; `mflo` before a branch or label gets none after ASPSX.

No PsyQ compiler from 3.3 to 4.6 produces the original `func_8014D260` or the three SDK habits, so the question of
TAIIKU `func_801488F0` under one of them does not arise. Where the original's code generator comes from stays open
(not tried: other cc1psx builds, such as the `psyq_263_221` set decomp.me lists, and the libraries' own object files).

## Build integration
- `config/toolchains.txt` names the toolchain of an object (`<subsegment name> gcc <ver> <aspsx>`); `configure.py` validates it and passes it to `tools/cc.py`. The gcc path has no asm-processor, so only objects whose functions are all C belong there. Test: `tools/test_configure_toolchains.py`.
- `tools/cc.py` runs maspsx with `--expand-div` (ASPSX expands the three-operand divide with its zero and overflow checks).
- gcc in the arm64 image: [[toolchain]], "Old gcc on arm64 (T-9200)".
