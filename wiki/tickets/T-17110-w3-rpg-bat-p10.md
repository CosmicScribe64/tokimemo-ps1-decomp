---
id: T-17110
title: "Wave 3 package w3-rpg-bat-p10"
status: Done
assignee: claude-opus (w3-rpg-bat-p10)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match as many as possible of four RPG_BAT functions in `src/ovl/RPG_BAT/8014ADD0.c` from the T-17100 types-first drafts: `func_8014C360` (412 B), `func_8014B79C` (776 B), `func_8014D11C` (600 B), `func_8014BFE4` (892 B). Wave 3 package ([[tickets/T-16990-wave-3-conveyor]]).

## Acceptance criteria

- [x] Each matched function is plain C, unit sha1 OK, committed on its own.
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a ticketed comment; others restored to `INCLUDE_ASM`.
- [x] T-17110 section in `wiki/matching-notes.md`; T-0018 rows where a compiler gap shows.
- [x] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Owned C file: `src/ovl/RPG_BAT/8014ADD0.c`. Work list in the scratchpad (`w3-rpg-bat-p10-list.txt`). Start 20:09 UTC.

Result: 3 of 4 matched (about 60 minutes), nothing guarded. `func_8014C360` (parameters changed in place, local `u = t + 0x40`), `func_8014B79C` (scalar aliases `D_8015EE48`/`D_8015EE4C`, empty `case 3`, FAKE `s32 pad`; `D_8015E740` is `u8 **`), `func_8014D11C` (radix in a variable, loop start in the `for` head). `func_8014BFE4` back to `INCLUDE_ASM` (128 rows at best, hand draft in `drafts/T-17110/RPG_BAT/func_8014BFE4/hand.c`). Notes in [[matching-notes]] ("T-17110"), four rows in [[register-cheatsheet]], section in [[rpg-bat-family-results]], update line in [[data/t0018-cases]].

## Comments

- 2026-10-16: In Review. Code review against `CODING_STANDARDS.md` still to do.
- 2026-10-16: Code review against `CODING_STANDARDS.md` (checklist, section 1 to 13). Checked: only `src/ovl/RPG_BAT/8014ADD0.c` changed among C files (plus `include/ovl/RPG_BAT.h` and the new `drafts/T-17110/` folder; no existing draft touched); no inline asm or `.word`; the three matches are plain C; the one fakematch (`s32 pad;` in `func_8014B79C`) carries FAKE and the ticket; `func_8014BFE4` is `INCLUDE_ASM` with its hand draft archived, no `NON_MATCHING` block needed; header changes (`D_8015E740` to `u8 **`, `func_8014B79C` to `void`, new externs and prototypes for `func_8013E864`, `func_8014EB58`, `D_8015EE5C/68/74/80`) are declared once and agree with the definitions in `8014E780.c`; non-ASCII only in game string literals, as in the other overlays; no game data. Rebuild after the fixes (`tools/docker.sh ninja`): `build/ovl/RPG_BAT.bin` sha1 OK (the clean 27/27 build runs in the merge script), `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` (0 new, 15 known) pass. Findings, all fixed: (1) byte counts wrong in the ticket Goal (`func_8014B79C` 876 B, `func_8014D11C` 652 B) and the notes (`func_8014D11C` 612 B, total 1,800 B): the asm sizes are 0x308 = 776, 0x258 = 600, 0x19C = 412, so the matched total is 1,788 B; corrected in the ticket, [[matching-notes]] and [[rpg-bat-family-results]] (the earlier log entry keeps the old figure). (2) The matching tricks had no comment in the C (scalar views `D_8015EE48`/`D_8015EE4C` of the struct words, empty `case 3`, radix variable `base`, named local `u`): one-line comments added (comment-only change, sha1 unchanged). No open findings. Done.
