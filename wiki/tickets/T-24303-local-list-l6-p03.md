---
id: T-24303
title: Local list l6-p03
status: In Progress
assignee: local-l6-p03
created: 2026-10-20
updated: 2026-10-20
links: ["[[matching-notes]]", "[[register-cheatsheet]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal

Match as many of the 54 functions of local work list l6-p03 (`.claude/orchestrator/lists/l6-p03-list.txt`, best first) as possible in about 6 hours, in worktree `l6-p03`, editing only the list's owned C files.

## Acceptance criteria

- [ ] Each match committed as plain C ("T-24303: match UNIT:func"), funcdiff MATCH.
- [ ] Misses keep a guard only under about 30 rows (`/* NON_MATCHING: T-24303, N rows; ... */`), else INCLUDE_ASM.
- [ ] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch at each checkpoint.
- [ ] Short T-24303 section in [[matching-notes]].

## Notes

## Comments
