---
id: T-10720
title: Wave 15: list 2
status: Done
assignee: wave-15 agent 2
created: 2026-10-12
updated: 2026-10-12
links: []
---

## Goal
Match functions on wave-15 list 2 (35 files, ~50 functions).

## Acceptance criteria
- [x] Every listed function attempted; matches committed
- [x] ninja 27/27 OK, code review recorded

## Notes
Matched (6 functions, 4172 bytes): GEKO `func_8013E69C`, `func_8013E760`, SHOUGATU `func_801366A4`, `func_80141AE8` (dupes.py `--lenient`), TAIIKU `func_80137090` (member-indexed slot object, `s16 y` stack slot, load order), RPG_BAT `func_8014D380` (three switches; the original jump tables have 12 and 15 entries, hence `case 11:` and `case 14:`).
Near misses kept under `#ifdef NON_MATCHING` with INCLUDE_ASM fallback: TAIIKU `func_8013CF88`, `func_8013C780`, `func_801406A8`; KANGEI `func_80136970`; SHUGAKU `func_80139054`; TT `func_80134874`, `func_80148E7C`; BUNKA_SD `func_80134418`; main `func_80079C70`; ETC `func_80143F24` (draft replaced by a closer one).
All other functions got an m2c draft, a build and a funcdiff (drafts in the agent scratch directory); T-0018 rows appended to [[data/t0018-cases]]; recipe and dead ends in [[matching-notes]] ("Wave 15, list 2").
Code review (inline, CODING_STANDARDS checklist): matched code is plain C except the documented FAKE in TAIIKU `func_80137090`; headers append-only with guards; externs for main-exe symbols added through `sync_protos.py --add-global` (D_80125D.., D_80120896, D_80120954 stay out of the tree because their drafts were reverted); every NON_MATCHING block carries T-10720 and the INCLUDE_ASM fallback; no game data committed. `sync_protos --check-branch`, `migrate_globals --check`, `ninja` 27/27 OK, `headers.ok`, `globals.ok`.

## Comments
