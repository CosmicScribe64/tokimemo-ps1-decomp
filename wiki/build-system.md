---
type: concept
updated: 2026-10-09
sources: ["configure.py", "config/SLPM_86.053.yaml", "README.md"]
---

# Build system

Status: OK build. `build/SLPM_86.053.bin` is byte-identical to the original (sha1 `e823bd844a8f8fa4d05483b59c66bc54b8393b26`), with every game function as `INCLUDE_ASM` in `src/game.c` (831 functions) and the SDK/lib region and data as plain asm.

## Commands (all through Docker)
1. `tools/docker.sh python3 tools/extract_disc.py "<game>.zip" disc` once, to create `disc/`.
2. `tools/docker.sh python3 configure.py` writes `build.ninja` and `objdiff.json`.
3. `tools/docker.sh ninja` splits, assembles, compiles, links and checks the sha1.

## Pipeline
`splat split config/SLPM_86.053.yaml` -> `asm/` (generated, gitignored) and `build/SLPM_86.053.ld`; `src/game.c` is created by splat only if missing (it lists `INCLUDE_ASM` for each function; edit it afterwards, never regenerate) -> asm objects via `mips-linux-gnu-as`; C via `cpp | cc1 | maspsx | as` -> `ld -T build/SLPM_86.053.ld -T build/undefined_syms_auto.txt` -> `objcopy -O binary` (the header section is the first 0x800 bytes, so the output is the complete PS-X EXE) -> `sha1sum -c` against `config/SLPM_86.053.sha1`.

## Gotchas found
- `ld_gp_expression` must be a string; `include_macro_inc` is not a 0.50 option.
- Shift-JIS decoding in splat (`string_encoding: SHIFT-JIS`) writes UTF-8 into the `.s` and changes the bytes (+0xC70 total). Use ASCII.
- The link needs `build/undefined_syms_auto.txt` (131 addresses in overlay/heap space, see [[overlays]]).
- Four tiny functions (`func_80066A68`, `func_80066A70`, `func_80066A78`, `func_80066AC0`) were emitted by splat with `jlabel`, which gives harmless `.end without .ent` assembler warnings.
- Generated `asm/` contains game code and must never be committed (gitignored with `build/`, `disc/`).

## objdiff
`objdiff.json` lists `src/game.c` with target `expected/build/src/game.o` (copy the matching build's objects to `expected/` once for per-function diffs, `expected/` is gitignored) and base `build/src/game.o`.

Layout facts: [[executable]]. Toolchain: [[toolchain]].
