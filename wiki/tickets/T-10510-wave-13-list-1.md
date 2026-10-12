---
id: T-10510
title: "Wave 13: list 1"
status: Done
assignee: w13-1
created: 2026-10-12
updated: 2026-10-12
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match as many as possible of the 58 functions assigned in scratchpad/wave13-list-1.txt, within the 29 owned C files.

## Acceptance criteria

- [x] First pass (m2c draft, build, diff) over the list; deep passes on near misses.
- [x] Final incremental build passes all 27 unit checks and header/global guards.
- [x] Inline review against CODING_STANDARDS.md.

## Comments

- 2026-10-12: Result: 11 matches, 2,380 bytes: GEKO `func_801397A8`, `func_80141DDC`, `func_80142018`; SHOUGATU `func_80139CCC`; DATE2 `func_80135DA4`, `func_80136A90`; EVENT `func_80100AB8`; NAME_ENT `func_80145EA4`; TT `func_80133C1C`; main `yuukou_down`, `select_girl_main` (return type `s32`, `select_girl_main` now `s32` in `main_api.h`). 15 guarded near misses (4 to 30 differing lines), 24 new rows in [[data/t0018-cases]], notes and new patterns in [[matching-notes]] ("Wave 13, list 1"). DATE `func_8015A980` and TACO `func_8015D0D0` are the hand-written rotation helpers of [[gcc-objects]] and stay `INCLUDE_ASM`.
- Inline review against CODING_STANDARDS.md: C89 declarations at block tops, `/* */` comments only, every `NON_MATCHING` block carries T-10510 and its INCLUDE_ASM fallback, tricks marked FAKE (`yuukou_down` constant local, NAME_ENT `func_80145EA4` shared source line; the copied twin shapes in GEKO/SHOUGATU keep their FAKE comments), main-exe globals added only through `sync_protos --add-global` (`D_800E3680[16]`, `D_800CA330[]`, `D_801217D4`), one overlay prototype appended (`include/ovl/OPTION.h`). `ninja` ends with 27/27 sha1 OK, `build/headers.ok`, `build/globals.ok`; `sync_protos --check-branch` and `migrate_globals --check` pass. No findings open.
