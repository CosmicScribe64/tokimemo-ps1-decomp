---
type: concept
updated: 2026-10-09
sources: ["tools/funcdiff.py", "tools/progress.py", "tools/cc.py", "configure.py", "tools/rodata_island.py", "tools/rodata_pieces.py"]
---

# How to decompile a function

All commands run through `tools/docker.sh`. Standards: `CODING_STANDARDS.md`. Known matching limits: [[matching-notes]]. Create a ticket first ([[kanban]]).

1. Pick a function (small leaf first). Size and sources: `head -1 asm/nonmatchings/main/<file>/func_XXXXXXXX.s` (`<file>` = the `src/main/<file>.c` that holds it; `grep -l func_XXXXXXXX src/main/*.c`) (`nonmatching name, size`). Progress: `tools/docker.sh ninja progress`.
2. First time only: make the reference object from a matching build: `tools/docker.sh ninja && mkdir -p expected/build && cp -r build/src expected/build/` (gitignored; must come from an all-`INCLUDE_ASM` or otherwise sha1-OK build).
3. Draft C with m2c: `tools/docker.sh m2c --target mipsel-gcc-c asm/nonmatchings/main/<file>/func_XXXXXXXX.s` (add `--context` for types). Clean it to C89 and `u8/s16/s32` types; put `extern` globals in `include/game.h`.
4. Replace the function's `INCLUDE_ASM(...)` line in its `src/main/<file>.c` with the C body, same position.
5. Build: `tools/docker.sh ninja`. It ends with `build/SLPM_86.053.bin: OK` only if everything still matches.
6. Diff the function: `tools/docker.sh python3 tools/funcdiff.py func_XXXXXXXX` (finds the object itself) prints `MATCH` or a short diff (`-` expected, `+` built). Scratch-compile with other compilers/flags by calling `python3 tools/cc.py <file.c> <out.o> ido 5.3` (or `ido 7.1`, or `gcc <gcc_version> <aspsx>`) on a copy of the function and `funcdiff.py --built <out.o>`; keep scratch files in `build/scratch/` (gitignored).
7. If it does not match after a reasonable number of tries, restore `INCLUDE_ASM`, add the function to [[matching-notes]], move on. Guard readable non-matching C with `#ifdef NON_MATCHING` and a ticket id (CODING_STANDARDS section 1).
8. Commit one function (or one small batch) per commit: `T-NNNN: match func_XXXXXXXX`, ending with the Co-Authored-By line; never commit `asm/`, `build/`, `expected/`, `disc/`.
9. Frames: IDO frames are 16 bytes smaller than the original's; `tools/frame_pass.py` adds them in every compile ([[toolchain]]), so write the C as if for a normal frame and compare. Overlay functions: copy the all-`INCLUDE_ASM` object (`build/ovl/<NAME>/src/ovl/<NAME>.o`) to `expected/ovl/<NAME>.o` once and use `funcdiff.py --expected expected/ovl/<NAME>.o --built <obj>`.

## How to decompile a switch (jump table, T-1340)
A function is a jump-table function when its asm has `%lo(jtbl_XXXXXXXX)` and a `jr`. Background and limits: [[build-system]] (section "Jump tables: rodata islands"); `grep -l '%lo(jtbl_' asm/nonmatchings/main/*/*.s asm/ovl/*/nonmatchings/*/*.s` lists them.

1. Draft with m2c and give it the rodata asm so it sees the table: `tools/docker.sh m2c --target mipsel-gcc-c <func>.s asm/data/rodata.rodata.s` (main; overlays: `asm/ovl/<NAME>/data/<NAME>_rodata.rodata.s`). m2c merges case labels that point at the default and can fold equal case bodies; the original usually has explicit `case` labels sharing the default (`case 1: case 2: default:`, ETC `func_8014A2C4`) and one separate block per case (main `func_80053DDC`: IDO does not merge equal bodies).
2. Make the C file's rodata an island, once per C file: `tools/docker.sh python3 tools/rodata_island.py config/overlays/<NAME>.yaml <NAME> <func...>` or `config/SLPM_86.053.yaml main/<addr> <func...>` (run after a split of the current config; it edits the yaml, prints the range, and refuses functions whose tables sit in different original objects). A file that already has an island can only take functions whose tables are in that chunk; other chunks need a per-object split of the C file first.
3. Replace the `INCLUDE_ASM` line by the C, then `tools/docker.sh sh -c 'python3 configure.py && ninja'` (the yaml change needs the reconfigure). The split step prints `N symbols into functions, M unowned (...)`; unowned symbols (strings the C code reaches through an extern, data in the island that no function mentions) must be written as literals in the C or placed with `INCLUDE_RODATA("<asm_path>/data/<name>.rodata", <symbol>);` in the position of the original.
4. The sha1 line decides. `funcdiff.py` is still useful for the code, but it reports the table relocations (`jtbl_X` against `.rodata`) and `.L` labels as differences; compare the instruction words only. A wrong case layout shows as a sha1 failure in the rodata range (the table words point to other blocks); fix the `case` structure until the text matches, the table then follows.
5. Declarations: new externs go to `include/game.h` / the overlay header per CODING_STANDARDS 8a (`tools/check_headers.py` runs in ninja); an overlay header that includes `game.h` must not redeclare what it has.
