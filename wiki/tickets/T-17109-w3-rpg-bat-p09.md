---
id: T-17109
title: Wave 3 package w3-rpg-bat-p09
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the three listed RPG_BAT functions of `src/ovl/RPG_BAT/8013D780.c` and `src/ovl/RPG_BAT/801477E0.c`: `func_8013D834` (1,160 bytes), `func_80147930` (1,624 bytes), `func_8013DC60` (1,804 bytes). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best drafts `drafts/T-14500/RPG_BAT/<func>` (scores 158, 396, 319).

## Acceptance criteria

- [ ] Matched functions are plain C (FAKE statements commented), unit sha1 OK, committed
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17109 comment, others restored to `INCLUDE_ASM`
- [ ] Notes in [[matching-notes]] (section T-17109); compiler-gap rows in [[data/t0018-cases]] if any
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p09`, branch `w3-rpg-bat-p09`, base `w2-good`. Owns `src/ovl/RPG_BAT/801477E0.c` and `src/ovl/RPG_BAT/8013D780.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 19:46 UTC.

## Comments
