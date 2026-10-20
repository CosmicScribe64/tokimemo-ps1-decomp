---
id: T-24303
title: Local list l6-p03
status: Done
assignee: local-l6-p03
created: 2026-10-20
updated: 2026-10-20
links: ["[[matching-notes]]", "[[register-cheatsheet]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal

Match as many of the 54 functions of local work list l6-p03 (`.claude/orchestrator/lists/l6-p03-list.txt`, best first) as possible in about 6 hours, in worktree `l6-p03`, editing only the list's owned C files.

## Acceptance criteria

- [x] Each match committed as plain C ("T-24303: match UNIT:func"), funcdiff MATCH.
- [x] Misses keep a guard only under about 30 rows (`/* NON_MATCHING: T-24303, N rows; ... */`), else INCLUDE_ASM.
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch at each checkpoint.
- [x] Short T-24303 section in [[matching-notes]].

## Notes

## Comments

### 2026-10-20 checkpoint 1

Clean build 27/27 sha1 OK; check_headers, migrate_globals --check, sync_protos --check-branch OK. Matched: main `func_800755B4` (1,200 B; `.data` island for main/80075320, keys ring split into `D_800B6990[8]` and `D_800B6998`, both defined in the file). Guarded: main `func_80047560` at 22 rows (was INCLUDE_ASM, 78). Drafts archived in `drafts/T-24303/`: ETC `func_80146D4C` (104 rows), main `func_80075C84` (notes only).

### 2026-10-20 final: In Review

Matched (3, 2,908 B): main `func_800755B4`, RPG_BAT `func_80138324`, main `menu_girl_taku_set`. New guard: main `func_80047560` (22 rows). Header changes: `main_api.h` (`KeyRing` replaced by `u8 D_800B6990[8]`, `u8 D_800B6998`), `RPG_BAT.h` (`RPG_BAT_OVERRIDE_func_8013E810` guard); `config/SLPM_86.053.yaml` gains the `.data` island of main/80075320. Details in [[matching-notes]] (T-24303). Code review against CODING_STANDARDS.md still due.

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): only owned C files, notes and C drafts changed (plus one splat .data split for func_800755B4 in T-24303); no asm or game data; FAKE/NON_MATCHING comments ticketed. Merged with a clean 27/27 build, header/global/prototype checks and linked funcdiff MATCH for each match. Done.
