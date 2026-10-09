---
type: concept
updated: 2026-10-09
sources: ["tools/funcdiff.py", "tools/progress.py", "tools/cc.py", "configure.py"]
---

# How to decompile a function

All commands run through `tools/docker.sh`. Standards: `CODING_STANDARDS.md`. Known matching limits: [[matching-notes]]. Create a ticket first ([[kanban]]).

1. Pick a function (small leaf first). Size and sources: `head -1 asm/nonmatchings/game/func_XXXXXXXX.s` (`nonmatching name, size`). Progress: `tools/docker.sh ninja progress`.
2. First time only: make the reference object from a matching build: `tools/docker.sh ninja && mkdir -p expected/build/src && cp build/src/game.o expected/build/src/game.o` (gitignored; must come from an all-`INCLUDE_ASM` or otherwise sha1-OK build).
3. Draft C with m2c: `tools/docker.sh m2c --target mipsel-gcc-c asm/nonmatchings/game/func_XXXXXXXX.s` (add `--context` for types). Clean it to C89 and `u8/s16/s32` types; put `extern` globals in `include/game.h`.
4. Replace the function's `INCLUDE_ASM(...)` line in `src/game.c` with the C body, same position.
5. Build: `tools/docker.sh ninja`. It ends with `build/SLPM_86.053.bin: OK` only if everything still matches.
6. Diff the function: `tools/docker.sh python3 tools/funcdiff.py func_XXXXXXXX` prints `MATCH` or a short diff (`-` expected, `+` built). Scratch-compile with other compilers/flags by calling `python3 tools/cc.py <file.c> <out.o> <gcc_version> <aspsx>` on a copy of the function and `funcdiff.py --built <out.o>`; keep scratch files in `build/scratch/` (gitignored).
7. If it does not match after a reasonable number of tries, restore `INCLUDE_ASM`, add the function to [[matching-notes]], move on. Guard readable non-matching C with `#ifdef NON_MATCHING` and a ticket id (CODING_STANDARDS section 1).
8. Commit one function (or one small batch) per commit: `T-NNNN: match func_XXXXXXXX`, ending with the Co-Authored-By line; never commit `asm/`, `build/`, `expected/`, `disc/`.
