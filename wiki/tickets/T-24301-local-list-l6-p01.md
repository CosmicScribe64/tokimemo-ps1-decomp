---
id: T-24301
title: Local list l6-p01
status: In Progress
assignee: local session l6-p01
created: 2026-10-19
updated: 2026-10-19
links: ["[[matching-notes]]", "[[register-cheatsheet]]", ".claude/orchestrator/lists/l6-p01-list.txt"]
---

## Goal

Match as many of the 53 functions on the local work list l6-p01 as possible (best first), in worktree `l6-p01`. Owned files: the C files named in the list header.

## Acceptance criteria

- [ ] Each match committed as "T-24301: match UNIT:func", plain C, linked funcdiff proof.
- [ ] Non-matches: guard under about 30 rows with a NON_MATCHING note, else INCLUDE_ASM.
- [ ] Clean 27/27 build, check_headers, migrate_globals --check, sync_protos --check-branch at each checkpoint.
- [ ] T-24301 section in [[matching-notes]].

## Notes

## Comments
- 2026-10-19 checkpoint 1 (clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch OK): 4 matches (GYOZI func_80140464, main func_8005CA94, main func_80042CB0, main func_8005A668); guards GYOZI func_80140E0C (11 rows), GYOZI func_80139E4C (15 rows, note), main func_8006C988 (11 rows); drafts in drafts/T-24301/.
- 2026-10-19 checkpoint 2 (clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch OK): 9 matches so far (adds OPTION func_80133010, main func_8005F730, KANGEI func_80132FF8, main func_80065F34, OPTION func_80132BA8 with a MAIN_API_OVERRIDE_func_80053D10 implicit int view); GYOZI func_80140E0C guard now 5 rows; RPG_BAT func_8014BB80 guard 22 rows (RPG_BAT_OVERRIDE_func_8013E7C0 in 8014ADD0.c).
