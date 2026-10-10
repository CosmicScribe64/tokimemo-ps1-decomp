---
id: T-3110
title: Test IDO 5.2 and 4.1 against the game code
status: Done
assignee: agent (o-ido52)
created: 2026-10-09
updated: 2026-10-09
links: ["[[ido-52-evaluation]]", "[[original-compiler]]", "[[toolchain]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[tickets/T-3100-identify-original-compiler]]", "[[tickets/T-0100-older-mips-compiler-emulation]]", "[[tickets/T-1321-register-promotion-build-step]]"]
---

## Goal

Find out whether IDO 5.2 (release 3.18, the best fit from T-3100) or IDO 4.1 reproduces the game code more closely than IDO 5.3, and recommend which compiler the game code should use.

## Acceptance criteria

- [x] IDO 5.2 and 4.1 obtained from decomp.me's public compiler distribution (user decision 2026-10-09), kept in a gitignored directory, never committed; source URL, version and sha256 in the wiki.
- [x] Both run in Docker (no OS image downloaded).
- [x] Regression check: matched C functions compiled with each compiler, with and without the frame pass; counts recorded.
- [x] Blocked cases (T-0018 `promo`/`regorder` rows, constant reuse) compiled from readable C; counts recorded.
- [x] Frame questions answered: +16 frames natively? the function-pointer-table frame (0x88 vs 0x90)?
- [x] Version stamp written by each compiler recorded.
- [x] [[ido-52-evaluation]] written with tables, provenance and a recommendation (switch, keep 5.3 + frame pass + uopt patch, or mix) and the two CI options.
- [x] Build default compiler unchanged.

## Notes

User decision (2026-10-09): IDO 5.2 and 4.1 may be downloaded from decomp.me's public compiler distribution for private use only; no OS images and no other compilers.

## Comments
- 2026-10-09: sources located, download blocked.
  - decomp.me compiler distribution (decompme/compilers @ fdd6793): `platforms/n64/ido5.2/Dockerfile` fetches `https://github.com/LLONSIT/qemu-irix-helpers/raw/refs/heads/n/qemu/ido5.2.tar.xz`; `platforms/n64/ido4.1/Dockerfile` fetches `https://github.com/decompme/compilers/releases/download/compilers/ido4.1.tar.gz`.
  - How decomp.me runs them (decomp.me `cromper/cromper/compilers.py`): IRIX binaries under qemu-irix, which ships inside each tarball (`usr/bin/qemu-irix` for 5.2; `usr/bin/qemu-irix-4.0` plus `usr/bin/ecoff_tool.py --convert-elf` for 4.1, whose output is ECOFF). The IRIX libraries come in the tarball (`-L ${COMPILER_DIR}`), so no OS image is needed.
  - The download into the gitignored `tools/local-compilers/` was refused by the session's permission system (untrusted code). Waiting for the user to allow it; nothing was downloaded and no file outside the wiki changed.
- 2026-10-09: archives downloaded by the coordinator after the user's approval (sha256 in [[ido-52-evaluation]]). They run in image `tokimemo-decomp-ido52` (local `tools/Dockerfile` additions for qemu-irix's glib/glibc needs, not committed).
- 2026-10-09: results ([[ido-52-evaluation]]), harness `tools/ido_eval.py`, cases `tools/ido_eval_cases/`.
  - Version stamps: 5.3 3.19, 5.2 **3.18** (as O.BIN), 4.1 3.12 (big-endian ECOFF header, f_flags 0x8000, as O.BIN).
  - 2564 matched C functions: 5.2 + frame pass 2564 identical; 5.2 without the pass 504 (same as 5.3, so no native +16). 4.1 uopt mix + pass: 2564. 4.1 ugen mix without the pass: 2484, and its frames equal the pass's in all 2564; the 80 differences are register allocation. Full 4.1: 928, and 10 files do not compile.
  - 46 blocked cases: no configuration beats 5.3 + pass on any case; 5.2 + pass gives identical counts.
  - T-1321 behaviours (o-t0018 `16bbea4` ETC/TACO, 26 pass-only functions): recovered by none of 5.2, 4.1 or 5 mixes.
  - Function-pointer table (`sp+0x2C`/0x90) and the `func_8013BC74` spill offset: not produced by any version; source-level.
  - Recommendation (b): keep 5.3 + frame pass + T-1321 pass. CI options for (a) listed for the user; nothing implemented. Build default unchanged.
- 2026-10-09: inline review against CODING_STANDARDS.md (Standards and Spec).
  - Standards: `tools/ido_eval.py` is Python 3 with a module docstring and runs in Docker; it takes paths as arguments and exits non-zero on a failed compile or bad arguments. It is an evaluation tool, not a build pass, so 7a does not apply and it has no unit tests; it is checked by the runs recorded here (5.3 + pass reproduces 2564 of 2564). The case files are C89 test inputs (m2c drafts) and not part of the build; one string literal was replaced by an extern. No game data, no compiler binaries and no Dockerfile change are staged; `/tools/local-compilers/` is gitignored. Commit message carries the ticket id.
  - Spec: every acceptance criterion is met. The licensing section states facts with links and gives no legal advice; both CI options are listed, and neither is implemented.
  - Findings: none open. Note: the local `tools/Dockerfile` change stays uncommitted on purpose; the lines are on the evaluation page.
