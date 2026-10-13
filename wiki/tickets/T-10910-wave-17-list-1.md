---
id: T-10910
title: Wave 17: list 1
status: Done
assignee: wave-17 agent 1
created: 2026-10-13
updated: 2026-10-13
links: []
---

## Goal
Match functions on wave-17 list 1 (37 files, ~55 functions).

## Acceptance criteria
- [x] Every listed function attempted; matches committed
- [x] ninja 27/27 OK, code review recorded

## Notes
Matched (4 functions, 1,476 bytes): RPG_BAT `func_80153A60` (168), TT `func_8014A814` (228), TT `func_80147D08` (524), TAIIKU `func_801339C4` (556), all through long decomp-permuter runs on guarded near misses (FAKE-tagged: dead copies/tests that reserve registers, selector through a pointer). Data islands for DATE 80149310, GEKO 80134D10 and SHOUGATU 80135B90 define the copy-chain variables; the three chain functions match in instruction order but not in three load registers and stay guarded (14-15 lines), as does main `func_800764AC` (24 lines). Details, recipes and dead ends: [[matching-notes]] "Wave 17, list 1" and [[data/t0018-cases]].
Code review (inline, CODING_STANDARDS checklist): matched code is plain C plus four FAKE-tagged tricks with reason and T-10910; no main-exe symbol declared outside `main_api.h` (the D_80120888 attempt was dropped); NON_MATCHING blocks carry T-10910 and the INCLUDE_ASM fallback; data-island definitions keep the original initial words; `sync_protos --check-branch`, `migrate_globals --check`, `check_headers`, `ninja` (27/27 OK, `headers.ok`, `globals.ok`) pass; no game data committed.

## Comments
