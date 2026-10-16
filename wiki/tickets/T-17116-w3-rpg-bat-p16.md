---
id: T-17116
title: Wave 3 package w3-rpg-bat-p16
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the three listed RPG_BAT functions, best first: `func_80156064` (856 B, `80155CC0.c`, T-17100 draft score 228), `func_80134E78` (1,268 B, `80134D30.c`, score 254) and `func_80135358` (1,700 B, `80134D30.c`, score 521). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [ ] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17116 comment, others restored to `INCLUDE_ASM`
- [ ] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17116)
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p16`, branch `w3-rpg-bat-p16`, base `w2-good`. Owns `src/ovl/RPG_BAT/80155CC0.c` and `src/ovl/RPG_BAT/80134D30.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 21:12 UTC.

## Comments
