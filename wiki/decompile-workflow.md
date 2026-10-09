---
type: concept
updated: 2026-10-09
sources: ["tools/funcdiff.py", "tools/progress.py", "tools/cc.py", "tools/queue.py", "configure.py"]
---

# How to decompile a function

All commands run through `tools/docker.sh`. Standards: `CODING_STANDARDS.md`. Known matching limits: [[matching-notes]]. Create a ticket first ([[kanban]]).

0. Get your work list: `tools/docker.sh python3 tools/queue.py --next 20 --files <your files>` (main address stems such as `80061710`, overlay names such as `TEL`, comma separated). It ranks the remaining `INCLUDE_ASM` functions: unblocked first, leaf before non-leaf, smaller first, with size, call count and flags (`L` loop; `J` jump table, `S` string, `P` one-nop pad: cannot be built; `R`, `V`: T-0018 register-promotion gap, see [[matching-notes]]). `--blocked` lists what the flags rule out, `--summary` counts per file; a `=func_X` in the last column marks a duplicate of an already listed function when a dupes list exists, so one match covers both. Do not start with blocked functions; if you try one and it does fail on the T-0018 pattern, it goes into the data file (step 7).
1. Pick a function from that list. Size and sources: `head -1 asm/nonmatchings/main/<file>/func_XXXXXXXX.s` (`<file>` = the `src/main/<file>.c` that holds it; `grep -l func_XXXXXXXX src/main/*.c`) (`nonmatching name, size`). Progress: `tools/docker.sh ninja progress`.
2. First time only: make the reference object from a matching build: `tools/docker.sh ninja && mkdir -p expected/build && cp -r build/src expected/build/` (gitignored; must come from an all-`INCLUDE_ASM` or otherwise sha1-OK build).
3. Draft C with m2c: `tools/docker.sh m2c --target mipsel-gcc-c asm/nonmatchings/main/<file>/func_XXXXXXXX.s` (add `--context` for types). Clean it to C89 and `u8/s16/s32` types; put `extern` globals in `include/game.h`.
4. Replace the function's `INCLUDE_ASM(...)` line in its `src/main/<file>.c` with the C body, same position.
5. Build: `tools/docker.sh ninja`. It ends with `build/SLPM_86.053.bin: OK` only if everything still matches.
6. Diff the function: `tools/docker.sh python3 tools/funcdiff.py func_XXXXXXXX` (finds the object itself) prints `MATCH` or a short diff (`-` expected, `+` built). Scratch-compile with other compilers/flags by calling `python3 tools/cc.py <file.c> <out.o> ido 5.3` (or `ido 7.1`, or `gcc <gcc_version> <aspsx>`) on a copy of the function and `funcdiff.py --built <out.o>`; keep scratch files in `build/scratch/` (gitignored).
7. If it does not match after a reasonable number of tries, restore `INCLUDE_ASM`, add the function to [[matching-notes]], move on. A T-0018 case (the original keeps a global in one register across blocks or calls, IDO uses other registers; or the `$v1`/`$v0` choice of a loaded global) also gets one row appended to [[data/t0018-cases]] (file, function, category, symptom; never edit old rows; rules in that file). It feeds [[tickets/T-1321-register-promotion-build-step]] and the detector calibration (`queue.py --calibrate`). Guard readable non-matching C with `#ifdef NON_MATCHING` and a ticket id (CODING_STANDARDS section 1).
8. Commit one function (or one small batch) per commit: `T-NNNN: match func_XXXXXXXX`, ending with the Co-Authored-By line; never commit `asm/`, `build/`, `expected/`, `disc/`.
9. Frames: IDO frames are 16 bytes smaller than the original's; `tools/frame_pass.py` adds them in every compile ([[toolchain]]), so write the C as if for a normal frame and compare. Overlay functions: copy the all-`INCLUDE_ASM` object (`build/ovl/<NAME>/src/ovl/<NAME>.o`) to `expected/ovl/<NAME>.o` once and use `funcdiff.py --expected expected/ovl/<NAME>.o --built <obj>`.
