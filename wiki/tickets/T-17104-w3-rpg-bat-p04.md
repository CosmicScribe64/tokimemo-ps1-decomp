---
id: T-17104
title: Wave 3 package w3-rpg-bat-p04
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the one listed RPG_BAT function of `src/ovl/RPG_BAT/801468B0.c`: `func_80146C30` (2,992 bytes of code, a 36-case switch of the Stampede / hermit battle script). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best draft `drafts/T-14500/RPG_BAT/func_80146C30` (score 329), typed variant `drafts/T-17100/RPG_BAT/func_80146C30` (329).

## Acceptance criteria

- [ ] Matched function is plain C (FAKE statements commented), unit sha1 OK, committed
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17104 comment, others restored to `INCLUDE_ASM`
- [ ] Notes in [[matching-notes]] (section T-17104); compiler-gap rows in [[data/t0018-cases]] if any
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p04`, branch `w3-rpg-bat-p04`, base `w2-good`. Owns `src/ovl/RPG_BAT/801468B0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 19:19 UTC.

## Comments
