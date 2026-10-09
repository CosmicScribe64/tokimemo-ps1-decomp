---
type: concept
updated: 2026-10-09
sources: ["tools/Dockerfile", "raw/disc-findings.md", "configure.py"]
---

# Toolchain

Everything runs in Docker (`tools/docker.sh <cmd>`, image `tokimemo-decomp`, forced linux/amd64; the old-gcc PsyQ builds are x86_64-only, so Apple Silicon runs it emulated). Never install tools on the host.

## Pinned versions (tools/Dockerfile)
splat64 0.50.0, spimdisasm 1.42.4, rabbitizer 1.16.2, pycparser 3.11, colorama 0.4.6, m2c (git commit 708d2d2), PyYAML 6.0.3, tqdm 4.67.1, crcmod 1.7, ninja_syntax 1.7.2, binutils-mips-linux-gnu 2.38, ninja 1.10.1, make 4.3, git 2.34.1, maspsx e85ecb3, objdiff-cli v3.8.2, mkpsxiso/dumpsxiso 2.30, decompals/old-gcc 0.17 in `/opt/gcc/<ver>/` (2.6.3-psx, 2.7.2-psx, 2.7.2-cdk, 2.8.0-psx, 2.8.1-psx, 2.91.66-psx, 2.95.2-psx). Env: `MASPSX_DIR=/opt/maspsx`, `GCC_DIR=/opt/gcc`.

## Compiler choice (provisional)
`gcc 2.7.2-psx` (Sony CCPSX, GCC 2.7.2 based) with maspsx `--aspsx-version=2.79` and flags `-O2 -G0 -mcpu=3000`. Reasons: the embedded libs date to 1995 (libgpu RCS ids 1995-03 to 1995-10), which fits the PsyQ 4.x era whose ccpsx is 2.7.2 based; fallbacks are 2.6.3-psx and 2.8.x. The first ten decompiled functions (trivial getters) match with it, but setters and anything needing `$at` stores, delay-slot filling or `$t6`-first allocation do not match under any gcc in `/opt/gcc`; the compiler is therefore NOT confirmed, see [[matching-notes]] ([[tickets/T-0011-game-code-file-boundaries-and-compiler]]). `-G0` is used because no code in the exe uses `$gp`.

## Assembler flags
`mips-linux-gnu-as -EL -march=r3000 -mabi=32 -G0 -Iinclude -I.`. C goes through `cpp | cc1 | maspsx | as` via `tools/cc.py`, which checks every stage (see `configure.py`). The splat `INCLUDE_ASM` macro (`include/include_asm.h`) is used with `INCLUDE_ASM_USE_MACRO_INC` defined in `include/common.h`, so `include/macro.inc` supplies `glabel`/`nonmatching`.
