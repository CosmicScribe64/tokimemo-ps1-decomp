---
id: T-17101
title: Wave 3 package w3-rpg-bat-p01
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the four listed RPG_BAT functions, best first: `func_80152080` (812 B, `80151FC0.c`, draft score 32), `func_80145CC0` (2,372 B, `80145CC0.c`, guarded draft), `func_80152384` (556 B, `80151FC0.c`, draft score 93), `func_80146578` (824 B, `80145CC0.c`, draft score 277). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [ ] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17101 comment, others restored to `INCLUDE_ASM`
- [ ] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17101)
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p01`, branch `w3-rpg-bat-p01`, base `w2-good`. Owns `src/ovl/RPG_BAT/80145CC0.c` and `src/ovl/RPG_BAT/80151FC0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 18:42 UTC.

## Comments
