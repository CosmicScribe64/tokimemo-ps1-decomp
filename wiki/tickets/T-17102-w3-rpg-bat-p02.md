---
id: T-17102
title: Wave 3 package w3-rpg-bat-p02
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the three listed RPG_BAT functions of `src/ovl/RPG_BAT/80151C10.c` and `src/ovl/RPG_BAT/80156CC0.c`: `func_80151C10`, `func_80156D78`, `func_80157158`. Package list from the T-14500 drafts (see [[rpg-bat-family-results]], section "Types-first pass").

## Acceptance criteria

- [ ] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [ ] Unmatched drafts under about 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17102 comment, others restored to `INCLUDE_ASM`
- [ ] Compiler-gap rows in [[data/t0018-cases]]; notes in [[matching-notes]] (section T-17102)
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p02`, branch `w3-rpg-bat-p02`, base `w2-good`. Owns the two C files above (plus `include/`, `config/`, `wiki/` edits). Started 18:58 UTC.

## Comments
