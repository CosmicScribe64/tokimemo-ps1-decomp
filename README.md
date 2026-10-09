# Tokimeki Memorial - Forever with You (PS1) decompilation

Work in progress matching decompilation of the Japanese "PlayStation the Best" release. The boot executable `SLPM_86.053` and the 26 overlays (`CDROM/EXEDIR/*.EXN`) rebuild byte-for-byte (nearly every function is still `INCLUDE_ASM`; the game code is split into 28 files, `src/main/<address>.c`). This repo contains no game data: you need your own copy of the disc zip.

All tools run in Docker (`tools/docker.sh`; the image builds on first use, linux/amd64).

```
# 1. extract the disc into disc/ (gitignored)
tools/docker.sh python3 tools/extract_disc.py "Tokimeki Memorial - Forever with You (Japan) (PlayStation the Best).zip" disc

# 2. configure and build (split, assemble, compile, link, sha1 check of the exe and every overlay)
tools/docker.sh python3 configure.py
tools/docker.sh ninja
```

A successful build prints `OK` for `build/SLPM_86.053.bin` and for each `build/ovl/<NAME>.bin`; `ninja overlays` builds only the overlays. Project knowledge, tickets and layout notes live in `wiki/` (start at `wiki/index.md`); agent rules in `AGENTS.md`, code conventions in `CODING_STANDARDS.md`.

The IDO compiles run through a documented frame-layout emulation pass (`tools/frame_pass.py`, wiki `toolchain`): the original's stack frames are 16 bytes larger than IDO 5.3's, and the pass models that for every function, so the C stays ordinary. It needs no extra build step; its unit tests run with `tools/docker.sh python3 tools/test_frame_pass.py`.

Progress per source file: `tools/docker.sh ninja progress`; per-function diff: `tools/docker.sh python3 tools/funcdiff.py func_XXXXXXXX` (finds the file itself); remaining leaf functions: `tools/docker.sh python3 tools/list_leaves.py [--file ADDR]`. `objdiff.json` has one unit per `src/main` file. File boundaries and their evidence: `wiki/source-files.md`.
