---
type: concept
updated: 2026-10-09
sources: ["tools/Dockerfile", "raw/disc-findings.md", "configure.py", "tools/cc.py"]
---

# Toolchain

Everything runs in Docker (`tools/docker.sh <cmd>`, image `tokimemo-decomp`, forced linux/amd64; the old-gcc PsyQ builds are x86_64-only, so Apple Silicon runs it emulated). Never install tools on the host.

## Pinned versions (tools/Dockerfile)
splat64 0.50.0, spimdisasm 1.42.4, rabbitizer 1.16.2, pycparser 3.11, colorama 0.4.6, m2c (git commit 708d2d2), PyYAML 6.0.3, tqdm 4.67.1, crcmod 1.7, ninja_syntax 1.7.2, binutils-mips-linux-gnu 2.38, ninja 1.10.1, make 4.3, git 2.34.1, maspsx e85ecb3, objdiff-cli v3.8.2, mkpsxiso/dumpsxiso 2.30, decompals/old-gcc 0.17 in `/opt/gcc/<ver>/` (2.6.3-psx, 2.7.2-psx, 2.7.2-cdk, 2.8.0-psx, 2.8.1-psx, 2.91.66-psx, 2.95.2-psx), decompals/ido-static-recomp v1.2 in `/opt/ido/5.3/` and `/opt/ido/7.1/`, asm-processor (simonlindholm) f3b2f85 in `/opt/asm-processor`. Env: `MASPSX_DIR=/opt/maspsx`, `GCC_DIR=/opt/gcc`, `IDO_DIR=/opt/ido`, `ASM_PROCESSOR_DIR=/opt/asm-processor`.

## Compiler choice
Game code (`src/game.c`): SGI IDO 5.3 `cc -c -EL -O2 -mips1 -G 0 -non_shared -Xcpluscomm`, run through asm-processor so `INCLUDE_ASM` functions are spliced in and assembled with GNU as ([[tickets/T-0013-identify-original-compiler-pipeline]]). IDO accepts `-EL` and writes little-endian ELF. It reproduces every original signature gcc could not (`$t6`-first temporaries, `$at` store expansion with the store in the `jr` delay slot, `lh` copies, `or` for move, `li` as `addiu`, IDO's save-order prologues), and 22 leaf functions byte-match in the build. It is NOT the exact original compiler: non-leaf frames in the original are 16 bytes larger than IDO's, and one function shows less global-address CSE. Evidence and hypotheses in [[matching-notes]]. IDO 7.1 is installed for comparison; 5.3 matched one function 7.1 did not (`func_8004E750`).

The toolchain is chosen per C file in `configure.py` (`C_FILES`); `tools/cc.py` supports `ido <ver>` and `gcc <ver> <aspsx>`. The gcc path (`gcc 2.7.2-psx` + maspsx `--aspsx-version=2.79`, `-O2 -G0 -mcpu=3000`) is kept for SDK library C from 1995-era PsyQ ([[psyq-sdk]]), which is still all asm. `-G 0` everywhere: no code in the exe uses `$gp`.

## Assembler flags
`mips-linux-gnu-as -EL -march=r3000 -mabi=32 -G0 -Iinclude -I.`. IDO-built C goes through `asm-processor/build.py` (IDO `cc`, then GNU as for the spliced asm, prelude `include/asmproc_prelude.inc`); gcc-built C through `cpp | cc1 | maspsx | as`. `tools/cc.py` checks every stage. The splat `INCLUDE_ASM` macro (`include/include_asm.h`) is used with `INCLUDE_ASM_USE_MACRO_INC` defined in `include/common.h`, so `include/macro.inc` supplies `glabel`/`nonmatching`.
