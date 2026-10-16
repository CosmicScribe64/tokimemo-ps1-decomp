---
id: T-17103
title: "Wave 3 package w3-rpg-bat-p03"
status: In Review
assignee: claude-opus (w3-rpg-bat-p03)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match as many as possible of four RPG_BAT functions in `src/ovl/RPG_BAT/80137280.c` from the T-17100 types-first drafts: `func_80139E88` (468 B), `func_8013A034` (728 B), `func_801387F0` (988 B), `func_80138B9C` (1024 B). Wave 3 package ([[tickets/T-16990-wave-3-conveyor]]).

## Acceptance criteria

- [x] Each matched function is plain C, unit sha1 OK, committed on its own.
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a ticketed comment; others restored to `INCLUDE_ASM`.
- [x] T-17103 section in `wiki/matching-notes.md`; T-0018 rows where a compiler gap shows.
- [ ] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Owned C file: `src/ovl/RPG_BAT/80137280.c`. Work list in the scratchpad (`w3-rpg-bat-p03-list.txt`).

Result: 4 of 4 matched (about 55 minutes, start 19:02 UTC), no guard. `func_80139E88` (indexed loop, three scalars between the stack tables, `== 0` / `else` pair), `func_8013A034` (address-taken stores keep the constant stores in order: FAKE; pointer view `RPG_BAT_OVERRIDE_D_8015EE8C`; indexed record loops), `func_801387F0` and `func_80138B9C` (`u32` view of `D_8015EBAC`, implicit-int `func_8013E7C0` override, `D_8015EC74 -= 1` form, two pad locals: FAKE). Notes in [[matching-notes]] ("T-17103"), three rows in [[register-cheatsheet]], section in [[rpg-bat-family-results]].

## Comments

- 2026-10-16: In Review. Code review against `CODING_STANDARDS.md` still to do.
