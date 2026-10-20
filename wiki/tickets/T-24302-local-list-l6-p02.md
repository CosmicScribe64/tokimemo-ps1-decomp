---
id: T-24302
title: Local list l6-p02
status: Done
assignee: local session l6-p02
created: 2026-10-19
updated: 2026-10-20
links: ["[[matching-notes]]", "[[register-cheatsheet]]", ".claude/orchestrator/lists/l6-p02-list.txt"]
---

## Goal

Match as many as possible of the 53 functions on local work list l6-p02 (GEKO, BUNKA_SD, SHOUGATU, ENDING, main, RPG_BAT, ETC, TACO, OPTION, KANGEI, TAIIKU, TEL, NAME_ENT), working in worktree `../tokimemo-wt/l6-p02`, editing only the owned C files.

## Acceptance criteria

- [x] Each match committed as plain C ("T-24302: match UNIT:func")
- [x] Guards only under about 30 rows, with a `NON_MATCHING: T-24302` note; others back to INCLUDE_ASM
- [x] Clean 27/27 build, check_headers, migrate_globals --check, sync_protos --check-branch
- [x] T-24302 section in [[matching-notes]]

## Notes

## Comments

- 2026-10-20 checkpoint 1: 1 match (GEKO func_801358CC, 416 B); guards improved: GEKO func_80133024 36 -> 5 rows, GEKO func_80134808 new guard at 15 rows. Clean build OK, checks OK.
- 2026-10-20 final: 1 match (GEKO func_801358CC). Guards: GEKO func_80133024 5 rows, func_80134808 15 rows, func_80132B24 about 30 rows. The other 49 functions were not reached or were left as they were; details in [[matching-notes]] (T-24302 section). In Review.

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): only owned C files, notes and C drafts changed (plus one splat .data split for func_800755B4 in T-24303); no asm or game data; FAKE/NON_MATCHING comments ticketed. Merged with a clean 27/27 build, header/global/prototype checks and linked funcdiff MATCH for each match. Done.
