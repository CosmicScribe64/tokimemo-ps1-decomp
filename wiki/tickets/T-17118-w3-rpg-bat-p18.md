---
id: T-17118
title: Wave 3 package w3-rpg-bat-p18
status: In Progress
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the one listed RPG_BAT function, `func_80133108` (4,192 B, `src/ovl/RPG_BAT/80132FE0.c`; T-14500 draft score 863). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [ ] A matched function is plain C (FAKE statements commented), unit sha1 OK, committed at once
- [ ] An unmatched draft under 30 differing lines stays under `#ifdef NON_MATCHING` with a T-17118 comment, otherwise it is restored to `INCLUDE_ASM`
- [ ] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17118)
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p18`, branch `w3-rpg-bat-p18`, base `w2-good`. Owns `src/ovl/RPG_BAT/80132FE0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 21:29 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80133108` | 4,192 | `INCLUDE_ASM` (T-14500 draft, score 863) | |

## Comments
