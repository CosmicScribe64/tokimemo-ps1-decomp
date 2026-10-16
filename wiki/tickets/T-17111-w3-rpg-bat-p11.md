---
id: T-17111
title: Wave 3 package w3-rpg-bat-p11
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the two listed RPG_BAT functions, best first: `func_80144010` (1,196 B, `80144010.c`, T-14500 draft score 333) and `func_801505B8` (1,376 B, `801504E0.c`, T-17100 draft score 233). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [ ] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17111 comment, others restored to `INCLUDE_ASM`
- [ ] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17111)
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p11`, branch `w3-rpg-bat-p11`, base `w2-good`. Owns `src/ovl/RPG_BAT/801504E0.c` and `src/ovl/RPG_BAT/80144010.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 20:21 UTC.

## Comments
