---
id: T-17112
title: Wave 3 package w3-rpg-bat-p12
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the four listed RPG_BAT functions in `src/ovl/RPG_BAT/801525B0.c`, best first: `func_80154220` (528 B, draft score 135), `func_80153784` (732 B, draft score 192), `func_80153F3C` (740 B, draft score 249), `func_801529B4` (1,460 B, draft score 183). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [ ] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17112 comment, others restored to `INCLUDE_ASM`
- [ ] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17112)
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p12`, branch `w3-rpg-bat-p12`, base `w2-good`. Owns `src/ovl/RPG_BAT/801525B0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 20:27 UTC.

## Results

## Comments
