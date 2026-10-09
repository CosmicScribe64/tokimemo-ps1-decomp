---
type: concept
updated: 2026-10-09
sources: ["configure.py", "config/SLPM_86.053.yaml", "README.md"]
---

# Build system

Status: OK build. `build/SLPM_86.053.bin` is byte-identical to the original (sha1 `e823bd844a8f8fa4d05483b59c66bc54b8393b26`), with every game function as `INCLUDE_ASM` in `src/game.c` (831 functions) and the SDK/lib region (65 asm segments, see [[psyq-sdk]]) and data as plain asm.

## Commands (all through Docker)
1. `tools/docker.sh python3 tools/extract_disc.py "<game>.zip" disc` once, to create `disc/`.
2. `tools/docker.sh python3 configure.py` writes `build.ninja` (ninja regenerates it when `configure.py` changes) and `objdiff.json`.
3. `tools/docker.sh ninja` splits, assembles, compiles, links and checks the sha1.

## Progress and per-function diff
- `tools/docker.sh ninja progress` (or `tools/docker.sh python3 tools/progress.py`) prints decompiled versus total functions and bytes per splat segment, counting functions still `INCLUDE_ASM` in `src/*.c` against sizes from the `nonmatching` headers in `asm/nonmatchings/`. Only the `game` segment is C-capable now; the SDK lib region is plain asm and not counted (the totals, 831 functions and 284404 bytes, cover the `game` segment only; the SDK lib region is a separate, later effort).
- `tools/docker.sh python3 tools/funcdiff.py <func...>` diffs functions of `build/src/game.o` against `expected/build/src/game.o`. See [[decompile-workflow]].

## Pipeline
`splat split config/SLPM_86.053.yaml` -> `asm/` (generated, gitignored) and `build/SLPM_86.053.ld`; `src/game.c` is created by splat only if missing (it lists `INCLUDE_ASM` for each function; edit it afterwards, never regenerate) -> asm objects via `mips-linux-gnu-as`; C via `tools/cc.py` with the toolchain `configure.py` assigns per file in `C_FILES` (`src/game.c`: IDO 5.3 through asm-processor, see [[toolchain]]; gcc path `cpp | cc1 | maspsx | as` kept for SDK C; stage failures abort, depfile lists the `INCLUDE_ASM` .s files) -> `ld -T build/SLPM_86.053.ld -T build/undefined_syms_auto.txt` -> `objcopy -O binary` (the header section is the first 0x800 bytes, so the output is the complete PS-X EXE) -> `sha1sum -c` against `config/SLPM_86.053.sha1`.

## Gotchas found
- gas pads the standard `.text` section of every object to 16 bytes, which shifted everything after a lib object whose size is not a multiple of 16. `tools/asm.py` assembles splat's `.section .text` into the custom section `.text.sdk` (no padding) and renames it to `.text`; the linker script's `SUBALIGN(2)` keeps placement exact (T-0010).
- `configure.py` derives `ASM_FILES` from the `asm` / `rodata` / `data` / `bss` subsegments of `config/SLPM_86.053.yaml`; splitting a segment needs no `configure.py` edit.
- splat rewrites `include/include_asm.h` and the `.inc` macro files on every split, which dropped the IDO guard from T-0013; `generate_asm_macros_files: False` in the yaml stops that.
- IDO cannot parse `__asm__`: `include/include_asm.h` skips its macros when `__sgi` is defined, and asm-processor replaces the `INCLUDE_ASM` lines itself, assembling them with GNU as and `include/asmproc_prelude.inc` (label macros without `.ent`/`.end`, plus `gte_macros.inc`). asm-processor runs with `--convert-statics no` because its `.mdebug` static-symbol import crashes on IDO `-EL` objects; C `static` symbols are therefore not visible to `INCLUDE_ASM` code yet.
- `ld_gp_expression` must be a string; `include_macro_inc` is not a 0.50 option.
- Shift-JIS decoding in splat (`string_encoding: SHIFT-JIS`) writes UTF-8 into the `.s` and changes the bytes (+0xC70 total). Use ASCII.
- The link needs `build/undefined_syms_auto.txt` (131 addresses in overlay/heap space, see [[overlays]]).
- Four tiny functions (`func_80066A68`, `func_80066A70`, `func_80066A78`, `func_80066AC0`) were emitted by splat with `jlabel`, which gives harmless `.end without .ent` assembler warnings.
- Generated `asm/` contains game code and must never be committed (gitignored with `build/`, `disc/`).

## objdiff
`objdiff.json` lists `src/game.c` with target `expected/build/src/game.o` (copy the matching build's objects to `expected/` once for per-function diffs, `expected/` is gitignored) and base `build/src/game.o`.

Layout facts: [[executable]]. Toolchain: [[toolchain]].
