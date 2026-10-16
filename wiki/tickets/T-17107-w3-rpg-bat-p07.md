---
id: T-17107
title: "Wave 3 package w3-rpg-bat-p07"
status: In Progress
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the RPG_BAT functions of package w3-rpg-bat-p07 (`src/ovl/RPG_BAT/8013E4D0.c`, `8013B5B0.c`): `func_8013B5B0` (928 B, best draft score 109), `func_8013EA60` (1,244 B, 144), `func_8013BBF8` (688 B, 221) and `func_8013E54C` (628 B, 1095).

## Acceptance criteria

- [ ] each function matched, or its draft stays guarded with a current differing-line count (or restored `INCLUDE_ASM` over 30 lines)
- [ ] unit sha1 OK, clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK
- [ ] wiki: matching notes section, t0018 rows where needed, log, kanban, index

## Notes

Worktree `w3-rpg-bat-p07` (branch `w3-rpg-bat-p07`, from `w2-good`). Started 2026-10-16 19:29 UTC.

## Comments
