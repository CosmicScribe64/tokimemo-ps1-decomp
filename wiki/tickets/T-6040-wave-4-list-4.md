---
id: T-6040
title: Wave 4: list 4
status: Done
assignee: agent (worktree w4-4)
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[game-state]]", "[[data/t0018-cases]]"]
---

## Goal

Match the functions of wave-4 work list 4 (185 functions, 41996 bytes in 36 C files: main 80059B40, 8005A0B0, 80062CD0, 80079B10, 8007C030, 80085E30 and overlays BUNKAKEN, BUNKA_SD, DATE, DATE2, EN_NICHI, ETC, EVENT, GEKO, GYOZI, NAME_ENT, RPG_BAT, SHOUGATU, TACO, TT, VALEN), best first, then continue from the byte queue inside the same files.

## Acceptance criteria

- [x] 28 functions matched (3972 bytes), each verified with `funcdiff.py --resolve` and a sha1-OK build
- [x] blocked T-0018 shapes stay INCLUDE_ASM, 13 rows appended to `wiki/data/t0018-cases.md`
- [x] `sync_protos.py --check-branch` and `migrate_globals.py --check` clean
- [x] clean rebuild 27/27 OK, inline code review recorded below

## Notes

Result: 28 of 185 functions (3972 of 41996 bytes), grand total 3539 -> 3567 of 6958 functions, 448356 bytes. Patterns, blocked shapes and the two permuter finds are in [[matching-notes]] ("Wave 4, list 4"). Matched: `func_80059B40`, `func_8005A2A8`, `func_8005A560`, `func_8005D31C`, `join_club`, `func_80060B24`, `pre_xmas`, `func_80060DF8`, `pre_syogatu_init`, `func_80061634`, `func_8007BDE8`, `normal_date_girl_in`, `normal_date_girl_out`, `normal_date_move_place`, `normal_date_three_select`, `func_80083628`, `select_girl`, `select_girl2`, `day_plus`, `vram_bustup_clear`, DATE2 `func_8013856C`, EN_NICHI `func_80133924`, ETC `func_80144A38`, `func_8014ACB4`, `func_8014AE8C`, `func_8014B9CC`, `func_8014BB34`, NAME_ENT `func_80144A34`.

Method: m2c drafts for all 185 functions, a batch applier and a build/funcdiff loop that drops functions with compile or diff errors; hand fixes for the near misses; decomp-permuter runs (21 functions, 75 to 240 s) for the register-order diffs.

## Comments

### Code review (inline, CODING_STANDARDS 13)
- Matches verified: `funcdiff.py --resolve` MATCH for each function, clean rebuild (`rm -rf asm build`, configure, ninja) 27/27 sha1 OK, `headers OK`, `globals OK`.
- No `NON_MATCHING` blocks. C89 only (declarations at block top, `/* */` comments). m2c `/* irregular */` comments removed.
- Fakematches: two unused locals marked `FAKE` (`day_plus` `sp28`, ETC `func_8014B9CC` `sp34`, T-3330 slot rule, tracked here). `u8 n = 10` in `func_80133924` and `u8 var_v1` in `func_80083628` are plain narrow locals. No first-symbol tricks; `Work80125D10` and `GameState` fields are used.
- Headers: main-exe symbols declared once in `include/main_api.h` (return type changes `s32 f()` for the implicit-int dispatchers, `select_girl_init/main(s32)`, new prototypes of callees); no overlay-local duplicates; `sync_protos.py --check-branch` OK, no new overrides.
- Renamed symbols (`xa_wait`, `k_sub_reset`, `gnsx`, `sndisp`, `menu_girl_taku_set`) used instead of `func_` names.
- No game data staged; commits end with the Co-Authored-By line.
- Findings: none open.
