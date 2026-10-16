---
id: T-17115
title: Wave 3 package w3-rpg-bat-p15
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the four listed RPG_BAT functions, best first: `func_801328C4` (464 B, `801321D0.c`, T-17100 draft score 97), `func_8014A3DC` (672 B, `801498C0.c`, T-14500 draft score 66), `func_80132B34` (452 B, `801321D0.c`, draft score 154, a known T-0018 constant-folding family) and `func_80149C5C` (1,296 B, `801498C0.c`, draft score 567). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [ ] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17115 comment, others restored to `INCLUDE_ASM`
- [ ] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17115)
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p15`, branch `w3-rpg-bat-p15`, base `w2-good`. Owns `src/ovl/RPG_BAT/801498C0.c` and `src/ovl/RPG_BAT/801321D0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 21:00 UTC.

## Comments
