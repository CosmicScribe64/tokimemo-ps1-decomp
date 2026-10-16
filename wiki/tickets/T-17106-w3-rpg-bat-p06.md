---
id: T-17106
title: Wave 3 package w3-rpg-bat-p06
status: In Progress
assignee: w3-rpg-bat-p06 agent
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the work list of package w3-rpg-bat-p06 in `src/ovl/RPG_BAT/80154430.c` (wave 3, from [[tickets/T-17100-types-first-rpg_bat]]): RPG_BAT `func_80154DD8` (1,204 B, draft score 113), `func_80155868` (520 B, 213), `func_80154760` (1,656 B, 319).

## Acceptance criteria

- [ ] Each matched function is plain C, verified by the RPG_BAT sha1 in ninja, committed one per commit.
- [ ] Near misses under about 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17106 comment; the rest go back to plain `INCLUDE_ASM`.
- [ ] Clean build 27/27 sha1 OK; check_headers, migrate_globals --check and sync_protos --check-branch pass.
- [ ] Notes in [[matching-notes]] (T-17106); compiler-gap rows in [[data/t0018-cases]].

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p06`, branch `w3-rpg-bat-p06` from `w2-good`. Owned C file: `src/ovl/RPG_BAT/80154430.c` only (plus `include/`, `config/`, `wiki/`). Started 19:23 UTC.

## Comments
