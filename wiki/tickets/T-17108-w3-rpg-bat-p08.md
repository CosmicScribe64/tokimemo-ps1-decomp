---
id: T-17108
title: Wave 3 package w3-rpg-bat-p08
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the six listed RPG_BAT functions, best first: `func_8014E994` (452 B, guarded draft, score 8), `func_8014AB84` (240 B, guarded draft, score 12), `func_8014AC74` (348 B, guarded draft, score 36), `func_8014F278` (216 B, draft score 67), `func_8014EF8C` (244 B, draft score 69), `func_8014EBEC` (344 B, draft score 61). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [ ] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17108 comment, others restored to `INCLUDE_ASM`
- [ ] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17108)
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p08`, branch `w3-rpg-bat-p08`, base `w2-good`. Owns `src/ovl/RPG_BAT/8014E780.c` and `src/ovl/RPG_BAT/8014A660.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 19:36 UTC.

## Comments
