---
type: concept
updated: 2026-10-09
sources: ["configure.py", "config/SLPM_86.053.yaml", "config/overlays.txt", "tools/gen_overlay_configs.py", "README.md"]
---

# Build system

Status: OK build. `build/SLPM_86.053.bin` is byte-identical to the original (sha1 `e823bd844a8f8fa4d05483b59c66bc54b8393b26`), with the game functions in the 29 files `src/main/<address>.c` (834 functions, 75 in C, the rest `INCLUDE_ASM`; [[source-files]]) and the SDK/lib region (65 asm segments, see [[psyq-sdk]]) and data as plain asm.

## Commands (all through Docker)
1. `tools/docker.sh python3 tools/extract_disc.py "<game>.zip" disc` once, to create `disc/`.
2. `tools/docker.sh python3 configure.py` writes `build.ninja` (ninja regenerates it when `configure.py` changes) and `objdiff.json`.
3. `tools/docker.sh ninja` splits, assembles, compiles, links and checks the sha1 of the main exe and of all 26 overlays (default target). `ninja overlays` builds only the overlays, `ninja build/ovl/TT.ok` one overlay.

## Progress and per-function diff
- `tools/docker.sh ninja progress` (or `tools/docker.sh python3 tools/progress.py`) prints decompiled versus total functions and bytes per splat segment, counting functions still `INCLUDE_ASM` in `src/main/*.c` against sizes from the `nonmatching` headers in `asm/nonmatchings/main/<addr>/`; one row per source file. Only the `src/main` files are C-capable now; the SDK lib region is plain asm and not counted (the totals, 834 functions and 284428 bytes, cover the game code only; the SDK lib region is a separate, later effort).
- `tools/docker.sh python3 tools/funcdiff.py <func...>` finds the `src/main` file that holds each function and diffs it against the same object under `expected/` (`expected/build/src/main/<addr>.o`). See [[decompile-workflow]].
- `tools/docker.sh python3 tools/list_leaves.py [--file ADDR]` lists remaining leaf functions over all files; `tools/docker.sh python3 tools/game_boundaries.py` reprints the file-boundary evidence ([[source-files]]).

## Pipeline
`splat split config/SLPM_86.053.yaml` -> `asm/` (generated, gitignored) and `build/SLPM_86.053.ld`; each `c` subsegment of the `main` segment is a `src/main/<addr>.c` that splat creates only if missing (`INCLUDE_ASM` per function; edit afterwards, never regenerate) -> asm objects via `mips-linux-gnu-as`; C via `tools/cc.py` with the toolchain `configure.py` assigns per file (`C_FILES` is read from the yaml's `c` subsegments, every file IDO 5.3 unless listed in `C_TOOLCHAIN_OVERRIDES`; IDO 5.3 through asm-processor with the frame-layout emulation pass `tools/frame_pass.py`, see [[toolchain]]; gcc path `cpp | cc1 | maspsx | as` kept for SDK C; stage failures abort, depfile lists the `INCLUDE_ASM` .s files) -> `ld -T build/SLPM_86.053.ld -T build/undefined_syms_auto.txt` -> `objcopy -O binary` (the header section is the first 0x800 bytes, so the output is the complete PS-X EXE) -> `sha1sum -c` against `config/SLPM_86.053.sha1`.

## Frame pass
Every IDO compile (main exe and overlays) goes through `tools/frame_pass.py` via an `as1` shim under `USR_LIB` (details in [[toolchain]]). `configure.py` lists `tools/frame_pass.py` as an implicit input of the C compile rules, so changing it rebuilds all C. `tools/docker.sh python3 tools/test_frame_pass.py` runs its unit tests.

## Overlays
Each overlay in `config/overlays.txt` is its own target (see [[overlays]]): `splat split config/overlays/<NAME>.yaml` -> `asm/ovl/<NAME>/` (gitignored) and `build/ovl/<NAME>.ld` -> `src/ovl/<NAME>.c` (IDO 5.3 via asm-processor) plus the rodata asm object -> `ld` with the splat script and `build/ovl/<NAME>_undefined_{funcs,syms}_auto.txt` -> `objcopy -O binary` -> sha1 against `config/overlays/<NAME>.sha1`, giving `build/ovl/<NAME>.ok`. The 26 `src/ovl/*.c` files are created by splat once (every function `INCLUDE_ASM`) and committed; like the `src/main` files they are never regenerated. `config/overlays/*.yaml`, the sha1 files and `config/overlays.txt` come from `tools/docker.sh python3 tools/gen_overlay_configs.py` (needs `disc/`). Status: all 26 rebuild byte-identical; O.BIN is not built ([[tickets/T-0201-obin-format-and-symbols]]). Not in `objdiff.json` yet.

## Gotchas found
- gas pads the standard `.text` section of every object to 16 bytes, which shifted everything after a lib object whose size is not a multiple of 16. `tools/asm.py` assembles splat's `.section .text` into the custom section `.text.sdk` (no padding) and renames it to `.text`; the linker script's `SUBALIGN(2)` keeps placement exact (T-0010).
- `configure.py` derives `ASM_FILES` from the `asm` / `rodata` / `data` / `bss` subsegments of `config/SLPM_86.053.yaml`; splitting a segment needs no `configure.py` edit.
- splat rewrites `include/include_asm.h` and the `.inc` macro files on every split, which dropped the IDO guard from T-0013; `generate_asm_macros_files: False` in the yaml stops that.
- splat rewrites `include/include_asm.h`, `macro.inc` and friends on every run, which clobbers the hand-edited IDO guard and breaks the IDO build after the first split. Both the main config and all overlay configs set `generate_asm_macros_files: False`.
- asm-processor rejects a function that is a single 4-byte `nop` ("too short .text block"); see DATE in [[overlays]].
- The linker aligns each section end to 16; the overlay code segment must therefore end on a 16-byte boundary (the generator rounds up) or later symbols shift.
- IDO cannot parse `__asm__`: `include/include_asm.h` skips its macros when `__sgi` is defined, and asm-processor replaces the `INCLUDE_ASM` lines itself, assembling them with GNU as and `include/asmproc_prelude.inc` (label macros without `.ent`/`.end`, plus `gte_macros.inc`). asm-processor runs with `--convert-statics no` because its `.mdebug` static-symbol import crashes on IDO `-EL` objects; C `static` symbols are therefore not visible to `INCLUDE_ASM` code yet.
- `ld_gp_expression` must be a string; `include_macro_inc` is not a 0.50 option.
- Shift-JIS strings (T-0012): `string_encoding`/`data_string_encoding: SHIFT-JIS` makes splat write readable UTF-8 `.asciz "..."` lines (with the raw hex in a comment), and gas would emit UTF-8 bytes (+0xC70 total in the main exe). `tools/asm.py` re-encodes every non-ASCII character of a `.asciz`/`.ascii`/`.string` literal to Shift-JIS octal escapes before assembling, so the bytes match again. The main config and all overlay configs (via `tools/gen_overlay_configs.py`) use SHIFT-JIS now; tests: `tools/docker.sh python3 tools/test_asm.py` (run from `tools/`).
- The link needs `build/undefined_syms_auto.txt` (131 addresses in overlay/heap space, see [[overlays]]).
- Four tiny functions (`func_80066A68`, `func_80066A70`, `func_80066A78`, `func_80066AC0`) were emitted by splat with `jlabel`, which gives harmless `.end without .ent` assembler warnings.
- In a fresh checkout the first `splat split` rewrites the committed `include/include_asm.h` with splat's own version (no `__sgi` guard), and the IDO compile then fails with `cfe: Error: include/include_asm.h, line 27: Syntax Error`. Restore it with `git checkout include/include_asm.h` and rerun `ninja` (T-0014; fix tracked in [[tickets/T-0101-splat-overwrites-include-asm-h]]).
- Generated `asm/` contains game code and must never be committed (gitignored with `build/`, `disc/`).

## objdiff
`objdiff.json` (written by `configure.py`) has one unit per `src/main` file: target `expected/build/src/main/<addr>.o` (copy the matching build's objects to `expected/` once for per-function diffs, `expected/` is gitignored) and base `build/src/main/<addr>.o`.

Layout facts: [[executable]]. Toolchain: [[toolchain]].

SDK names (T-0010) are in `config/symbol_addrs_sdk.txt`, read by the main config only: the shared `config/symbol_addrs.txt` is also loaded by the overlay configs, where splat rejects symbols outside the overlay segment.

## Per-file layout notes (T-0012)
- Adding or moving a file boundary: edit the `c` subsegment list in `config/SLPM_86.053.yaml` (names `main/<vram>`, start = file offset) and move the functions between `src/main/*.c`; `configure.py`, `objdiff.json`, `progress.py` follow automatically. `INCLUDE_ASM` folder names are `asm/nonmatchings/main/<addr>`.
- `tools/cc.py` pads each object's `.text` to a multiple of 16 so the original per-object alignment survives the `SUBALIGN(2)` link ([[source-files]]).
- C headers are listed for ninja by glob (`include/*.h`, `include/*.inc`), so new headers need no `configure.py` edit.
