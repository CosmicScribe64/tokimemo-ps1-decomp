---
id: T-9340
title: Wave 7: list 4
status: Done
assignee: w7-4
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Match as many functions as possible from wave-7 work list 4 (36 files, 142 functions).

## Acceptance criteria
- [x] Matched functions committed, full build 27/27 OK
- [x] Inline review against CODING_STANDARDS.md

## Notes
Matched 9 functions (2960 bytes): main `func_8006B5BC`; OPTION `func_80137864`; SHOUGATU `func_80134400`; ENDING `func_80137340`, `func_801380B0`, `func_80139498`; GYOZI `func_8013D294`; TT `func_80144094`, `func_80145E60`. About 30 T-0018 rows added to [[data/t0018-cases]]; patterns in [[matching-notes]] "Wave 7, list 4 (T-9340)". Clean rebuild: 27/27 OK, headers OK, globals OK, `sync_protos --check-branch` OK, grand total 4567/6958 functions.

## Comments
Inline review against CODING_STANDARDS.md (no sub-agents): C89 only (declarations at block tops, `/* */` comments), no per-file flags, all tricks carry a `FAKE` comment with the ticket id (`if (i) { }`, `D &= (m = 0xFF7F)`, three `s32 pad` locals, the byte-typed argument store), new main-exe globals went through `sync_protos.py --add-global`, overlay declarations were appended to `include/ovl/ENDING.h` and `include/ovl/SHOUGATU.h`. No findings open.
