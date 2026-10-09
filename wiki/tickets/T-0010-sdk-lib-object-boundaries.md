---
id: T-0010
title: Pin SDK version and lib object boundaries
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

Follow-up of [[tickets/T-0006-identify-psyq-libs-sdk-version]]: find the exact start of libpress and the object-level boundaries of libpress/libcd/libsnd/libspu/libgpu/libapi, and pin the PsyQ version.

## Acceptance criteria

- [x] Each lib object in `sdk_libs` becomes its own asm subsegment: done for libpress (3 blocks), libcd (event, rest), libgte (36 objects), libetc, libapi/libcard/libc objects; the other libs are one segment each. The rest is [[tickets/T-0300-sdk-object-split-remaining-libs]]; rodata/data is [[tickets/T-0301-sdk-rodata-data-split]] (re-scoped, not delivered here).
- [x] SDK release narrowed by matching against the public PsyQ signature sets: libgte 3.4, libc/libcard 3.3, libgpu <= 3.61, game era late 1995. A single release could not be pinned, see [[tickets/T-0302-sdk-version-conflict]].
- [x] Real SDK names for 166 functions in `config/symbol_addrs.txt`; game/SDK boundary confirmed at 0x80086810 (no `src/game.c` change needed).
- [x] Findings recorded in [[psyq-sdk]].

## Notes

Method and tools: `tools/psyq_sigmatch.py`, `tools/psyq_sigfuzzy.py` (signature data in the gitignored `tools/psyq_sigs/`). Build: `tools/asm.py` avoids gas section padding, `configure.py` reads the asm file list from the yaml. sha1 verified: `build/SLPM_86.053.bin: OK`.

## Comments

- 2026-10-09: moved to In Review for the code-review gate.
