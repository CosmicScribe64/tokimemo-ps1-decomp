---
id: T-8010
title: "Wave 5: list 1"
status: Done
assignee: wave5-agent-1
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match as many as possible of the 141 functions (48516 bytes) of wave 5 list 1, in 35 files (main 80042540, 80049FF0, 8004C3F0, 800563F0, 80058D20, 80079B10 and overlays BUNKAKEN, BUNKASAI, DATE, ENDING, EN_NICHI, ETC, EVENT, GEKO, GYOZI, KANGEI, OLH, OPTION, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TT, VALEN).

## Acceptance criteria

- [x] Every function in the list matched or reverted to INCLUDE_ASM with a note
- [x] `ninja` ends with 27 of 27 OK and build/headers.ok
- [x] Inline review against CODING_STANDARDS.md recorded below

## Notes

Work in worktree w5-1 (branch w5-1). T-0018 cases go to [[data/t0018-cases]].

## Comments

### Result (2026-10-10)
20 functions matched, 4048 bytes (18 of them from the list, 3708 of 48516 bytes; two more table dispatchers of the same files, DATE `func_801571D4` and `func_80156494`). After `rm -rf asm build; configure.py; ninja`: 27 of 27 sha1 OK, `build/headers.ok`, `build/globals.ok`; `sync_protos.py --check-branch` OK (no new finding), `migrate_globals.py --check` OK. `ninja progress` grand total 3987 of 6958 functions, 518260 of 2279368 bytes (22.7%). 19 rows added to [[data/t0018-cases]]. Patterns, near misses and blockers: [[matching-notes]], section "Wave 5, list 1 (T-8010)".

Matched: main `func_8007B358`, `func_80059308`, `func_8007A43C`; TT `func_80133058`, `func_80133288`; TAIIKU `func_80138950`, `func_80140738`, `func_8013AF74`; KANGEI `func_80134DB0`; DATE `func_801563A8`, `func_801571D4`, `func_80156494`; ENDING `func_80135DD0`; EVENT `func_801087F8`, `func_80109EE8`; OPTION `func_8013C780`; GYOZI `func_80135988`; OLH `func_801352A0`; TACO `func_801480B0`, `func_80138450`.

Fakematches, all marked `FAKE` in the source with this ticket or T-3330: `s32 idx`/`s32 pad` before the table of KANGEI `func_80134DB0`, DATE `func_801563A8`, `func_801571D4`, `func_80156494`, ENDING `func_80135DD0`, EVENT `func_801087F8`, `func_80109EE8`; `(D << 2) << 6` in main `func_8007B358`; two stores on one line in TT `func_80133058` and main `func_80059308`; `v ^ 0` in GYOZI `func_80135988`.

Tooling notes: none blocking. `funcdiff.py` reports jump-table relocations as differences (documented); the permuter's "score 0" can fail the real build for jump-table functions, so check with ninja (main `func_8007A43C`).

### Inline review against CODING_STANDARDS.md (13)
- Matches verified: every function by `funcdiff.py --resolve` on its own object and by the sha1 of its binary after a clean rebuild (27/27 OK); jump-table and string functions (main `func_8007A43C`, OLH, EVENT, GYOZI) by the unit sha1.
- No `NON_MATCHING` blocks added; every near miss was reverted to plain `INCLUDE_ASM`.
- C89: declarations at block top, `/* */` comments only, no C99 forms; one `goto` label (TT `func_80133288`, the original's `||` body shared by two conditions).
- Types: fixed-width typedefs; new local table types `FnTblN` (size comments), `TaiikuTbl11`; no SDK struct redefined.
- Declarations: main-exe symbols added only to `include/main_api.h` (`D_80125D50`, K&R `func_8007B5CC`, `func_8007B6E0`, `func_8007B734`, `func_8007B7E0`); overlay symbols to the overlay headers (appended); `check_headers.py` passes.
- FAKE comments carry reason and ticket; the tracked list is above.
- No game data staged; commits are one function each with the trailer.
- Wiki: ticket Done and the card in Done, `wiki/log.md` entries appended, `wiki/index.md` updated.
