---
id: T-17114
title: Wave 3 package w3-rpg-bat-p14
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the four listed RPG_BAT functions of `src/ovl/RPG_BAT/8013F4F0.c` and `src/ovl/RPG_BAT/801368B0.c`: `func_80136A9C` (316 bytes), `func_80140E70` (496), `func_8013F82C` (1,600), `func_80140204` (2,180). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best drafts `drafts/T-14500/RPG_BAT/<func>` (scores 114, 77, 248, 508).

## Acceptance criteria

- [ ] Matched functions are plain C (FAKE statements commented), unit sha1 OK, committed
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17114 comment, others restored to `INCLUDE_ASM`
- [ ] Notes in [[matching-notes]] (section T-17114); compiler-gap rows in [[data/t0018-cases]] if any
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p14`, branch `w3-rpg-bat-p14`, base `w2-good`. Owns `src/ovl/RPG_BAT/8013F4F0.c` and `src/ovl/RPG_BAT/801368B0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 20:49 UTC.

## Results

(filled in at the end)

## Comments
