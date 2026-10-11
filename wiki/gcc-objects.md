---
type: concept
updated: 2026-10-10
sources: ["tools/gcc_fingerprint.py", "tools/test_gcc_fingerprint.py", "config/toolchains.txt", "configure.py", "tools/cc.py", "tools/Dockerfile"]
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

## `func_8014D260` (open)
With gcc 2.7.2-psx / 2.6.3-psx and maspsx `--expand-div`, the C in [[tickets/T-9200-gcc-built-sdk-objects-in-overlays]] is identical to the original up to the first division and has the same block structure, but is 4 bytes shorter. The divisions differ in layout: ASPSX expands `div rd,rs,rt` as `div $zero,rs,rt; mflo rd; <checks>` when `rd` is neither `rs` nor `rt` and as `div; <checks>; mflo rd` otherwise (original: four divisions, two of each kind), and puts the `mflo` hazard nops in other places (`mflo a0; nop; nop; j; sll v0,a0,1` against `mflo a0; nop; sll`). GNU as always puts the `mflo` after the checks. A pass in front of `maspsx` could model it; one function is too little evidence for CODING_STANDARDS 7a, so this waits for a second division site in gcc-built code.

## Build integration
- `config/toolchains.txt` names the toolchain of an object (`<subsegment name> gcc <ver> <aspsx>`); `configure.py` validates it and passes it to `tools/cc.py`. The gcc path has no asm-processor, so only objects whose functions are all C belong there. Test: `tools/test_configure_toolchains.py`.
- `tools/cc.py` runs maspsx with `--expand-div` (ASPSX expands the three-operand divide with its zero and overflow checks).
- gcc in the arm64 image: [[toolchain]], "Old gcc on arm64 (T-9200)".
