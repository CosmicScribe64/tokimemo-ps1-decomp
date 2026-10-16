---
id: T-17113
title: Wave 3 package w3-rpg-bat-p13
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the four listed RPG_BAT functions of `src/ovl/RPG_BAT/801447E0.c`, `src/ovl/RPG_BAT/8013C890.c` and `src/ovl/RPG_BAT/801484E0.c`: `func_801447E0` (924 bytes), `func_80148714` (1,012), `func_8013C890` (1,072), `func_80144C84` (1,276). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best drafts `drafts/T-14500/RPG_BAT/<func>` and `drafts/T-17100/RPG_BAT/<func>` (scores 216, 182, 212, 360).

## Acceptance criteria

- [ ] Matched functions are plain C (FAKE statements commented), unit sha1 OK, committed
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17113 comment, others restored to `INCLUDE_ASM`
- [ ] Notes in [[matching-notes]] (section T-17113); compiler-gap rows in [[data/t0018-cases]] if any
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p13`, branch `w3-rpg-bat-p13`, base `w2-good`. Owns `src/ovl/RPG_BAT/801447E0.c`, `src/ovl/RPG_BAT/8013C890.c` and `src/ovl/RPG_BAT/801484E0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 20:31 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|

## Comments
