---
id: T-17110
title: "Wave 3 package w3-rpg-bat-p10"
status: In Progress
assignee: claude-opus (w3-rpg-bat-p10)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match as many as possible of four RPG_BAT functions in `src/ovl/RPG_BAT/8014ADD0.c` from the T-17100 types-first drafts: `func_8014C360` (412 B), `func_8014B79C` (876 B), `func_8014D11C` (652 B), `func_8014BFE4` (892 B). Wave 3 package ([[tickets/T-16990-wave-3-conveyor]]).

## Acceptance criteria

- [ ] Each matched function is plain C, unit sha1 OK, committed on its own.
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a ticketed comment; others restored to `INCLUDE_ASM`.
- [ ] T-17110 section in `wiki/matching-notes.md`; T-0018 rows where a compiler gap shows.
- [ ] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Owned C file: `src/ovl/RPG_BAT/8014ADD0.c`. Work list in the scratchpad (`w3-rpg-bat-p10-list.txt`). Start 20:09 UTC.

## Comments
