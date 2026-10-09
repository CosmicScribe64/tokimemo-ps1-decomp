# Tokimeki Memorial - Forever with You (PS1) decompilation

Work in progress matching decompilation of the Japanese "PlayStation the Best" release. The boot executable `SLPM_86.053` and the 26 overlays (`CDROM/EXEDIR/*.EXN`) rebuild byte-for-byte (every function is still `INCLUDE_ASM`). This repo contains no game data: you need your own copy of the disc zip.

All tools run in Docker (`tools/docker.sh`; the image builds on first use, linux/amd64).

```
# 1. extract the disc into disc/ (gitignored)
tools/docker.sh python3 tools/extract_disc.py "Tokimeki Memorial - Forever with You (Japan) (PlayStation the Best).zip" disc

# 2. configure and build (split, assemble, compile, link, sha1 check of the exe and every overlay)
tools/docker.sh python3 configure.py
tools/docker.sh ninja
```

A successful build prints `OK` for `build/SLPM_86.053.bin` and for each `build/ovl/<NAME>.bin`; `ninja overlays` builds only the overlays. Project knowledge, tickets and layout notes live in `wiki/` (start at `wiki/index.md`); agent rules in `AGENTS.md`, code conventions in `CODING_STANDARDS.md`.
