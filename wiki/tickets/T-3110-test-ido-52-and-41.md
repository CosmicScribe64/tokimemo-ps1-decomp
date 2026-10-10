---
id: T-3110
title: Test IDO 5.2 and 4.1 against the game code
status: In Progress
assignee: agent (o-ido52)
created: 2026-10-09
updated: 2026-10-09
links: ["[[ido-52-evaluation]]", "[[original-compiler]]", "[[toolchain]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[tickets/T-3100-identify-original-compiler]]", "[[tickets/T-0100-older-mips-compiler-emulation]]", "[[tickets/T-1321-register-promotion-build-step]]"]
---

## Goal

Find out whether IDO 5.2 (release 3.18, the best fit from T-3100) or IDO 4.1 reproduces the game code more closely than IDO 5.3, and recommend which compiler the game code should use.

## Acceptance criteria

- [ ] IDO 5.2 and 4.1 obtained from decomp.me's public compiler distribution (user decision 2026-10-09), kept in a gitignored directory, never committed; source URL, version and sha256 in the wiki.
- [ ] Both run in Docker (no OS image downloaded).
- [ ] Regression check: matched C functions compiled with each compiler, with and without the frame pass; counts recorded.
- [ ] Blocked cases (T-0018 `promo`/`regorder` rows, constant reuse) compiled from readable C; counts recorded.
- [ ] Frame questions answered: +16 frames natively? the function-pointer-table frame (0x88 vs 0x90)?
- [ ] Version stamp written by each compiler recorded.
- [ ] [[ido-52-evaluation]] written with tables, provenance and a recommendation (switch, keep 5.3 + frame pass + uopt patch, or mix) and the two CI options.
- [ ] Build default compiler unchanged.

## Notes

User decision (2026-10-09): IDO 5.2 and 4.1 may be downloaded from decomp.me's public compiler distribution for private use only; no OS images and no other compilers.

## Comments
- 2026-10-09: sources located, download blocked.
  - decomp.me compiler distribution (decompme/compilers @ fdd6793): `platforms/n64/ido5.2/Dockerfile` fetches `https://github.com/LLONSIT/qemu-irix-helpers/raw/refs/heads/n/qemu/ido5.2.tar.xz`; `platforms/n64/ido4.1/Dockerfile` fetches `https://github.com/decompme/compilers/releases/download/compilers/ido4.1.tar.gz`.
  - How decomp.me runs them (decomp.me `cromper/cromper/compilers.py`): IRIX binaries under qemu-irix, which ships inside each tarball (`usr/bin/qemu-irix` for 5.2; `usr/bin/qemu-irix-4.0` plus `usr/bin/ecoff_tool.py --convert-elf` for 4.1, whose output is ECOFF). The IRIX libraries come in the tarball (`-L ${COMPILER_DIR}`), so no OS image is needed.
  - The download into the gitignored `tools/local-compilers/` was refused by the session's permission system (untrusted code). Waiting for the user to allow it; nothing was downloaded and no file outside the wiki changed.
