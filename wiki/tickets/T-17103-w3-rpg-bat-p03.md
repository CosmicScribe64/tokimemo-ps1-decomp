---
id: T-17103
title: "Wave 3 package w3-rpg-bat-p03"
status: Done
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
- [x] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Owned C file: `src/ovl/RPG_BAT/80137280.c`. Work list in the scratchpad (`w3-rpg-bat-p03-list.txt`).

Result: 4 of 4 matched (about 55 minutes, start 19:02 UTC), no guard. `func_80139E88` (indexed loop, three scalars between the stack tables, `== 0` / `else` pair), `func_8013A034` (address-taken stores keep the constant stores in order: FAKE; pointer view `RPG_BAT_OVERRIDE_D_8015EE8C`; indexed record loops), `func_801387F0` and `func_80138B9C` (`u32` view of `D_8015EBAC`, implicit-int `func_8013E7C0` override, `D_8015EC74 -= 1` form, two pad locals: FAKE). Notes in [[matching-notes]] ("T-17103"), three rows in [[register-cheatsheet]], section in [[rpg-bat-family-results]].

## Comments

- 2026-10-16: In Review. Code review against `CODING_STANDARDS.md` still to do.
- 2026-10-16: Code review against the `CODING_STANDARDS.md` checklist. Checked: only `src/ovl/RPG_BAT/80137280.c` changed among C files (plus the `RPG_BAT.h` wrapper for `D_8015EE8C`, consistent with the file-level override and its reason); no drafts/ archive, asm, `.word` or game data; all four matches are plain C; nothing guarded, so no `NON_MATCHING` block; the three fakematches (address-taken stores in `func_8013A034`, pad locals plus `u32` view plus implicit-int callee in `func_801387F0` and `func_80138B9C`) carry `FAKE` and the ticket; the `& 0xFFFFFF` in `func_80138B9C` is in the original asm (`lui/ori $at,0xFFFFFF`), not a trick. Findings, both fixed: (1) three register-cheatsheet rows were appended below the "Do not" bullets, outside the table; moved into the table. (2) three triple blank lines in `80137280.c`; collapsed. Rebuild: unit sha1 OK, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` (0 new, 15 known) pass. Done.
