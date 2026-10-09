---
id: T-0010
title: Pin SDK version and lib object boundaries
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

Follow-up of [[tickets/T-0006-identify-psyq-libs-sdk-version]]: find the exact start of libpress and the object-level boundaries of libpress/libcd/libsnd/libspu/libgpu/libapi, and pin the PsyQ version.

## Acceptance criteria

- [ ] Each lib object in `sdk_libs` becomes its own asm subsegment with its rodata/data. Partly delivered: libpress (3 blocks, low confidence), libcd (2), libgte (36 objects), libetc, libapi/libcard/libc objects; one segment each for libsnd/libspu/libgs/libgpu. -> moved to [[tickets/T-0300-sdk-object-split-remaining-libs]] (remaining objects) and [[tickets/T-0301-sdk-rodata-data-split]] (rodata/data).
- [ ] SDK release pinned by matching an SDK function against a known library object. Not met: narrowed only (libgte matches 3.4 exactly, libc/libcard 3.3, libgpu probably <= 3.61; the rest conflicts). -> moved to [[tickets/T-0302-sdk-version-conflict]].
- [x] Findings recorded in [[psyq-sdk]].
- [x] Extra, delivered: 166 real SDK names in `config/symbol_addrs_sdk.txt`; game/SDK boundary confirmed at 0x80086810 (no `src/game.c` change).

## Notes

Method and tools: `tools/psyq_sigmatch.py`, `tools/psyq_sigfuzzy.py` (signature data in the gitignored `tools/psyq_sigs/`). Build: `tools/asm.py` avoids gas section padding, `configure.py` reads the asm file list from the yaml. sha1 verified: `build/SLPM_86.053.bin: OK`.

## Comments

- 2026-10-09: moved to In Review for the code-review gate.
- 2026-10-09: code-review (Standards + Spec) run. Findings fixed: original acceptance criteria restored with unmet items moved to T-0300/T-0301/T-0302, SDK pins softened with confidence levels, signature tools share `tools/psyqsig.py` and exit non-zero on missing/empty input, asm.py/configure.py fail loudly, T-0300 libpress criterion made explicit. Merged main (overlays); clean rebuild: 27/27 sha1 OK (main exe + 26 overlays), `ninja progress` 63/834. Moved to Done.
