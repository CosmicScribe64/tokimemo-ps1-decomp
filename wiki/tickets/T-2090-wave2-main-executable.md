---
id: T-2090
title: Wave 2: main executable
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match functions in all `src/main/*.c` files (main executable), wave 2. Owned: `src/main/*.c`, `include/game.h` (append-only), `include/main_only.h`.

## Acceptance criteria

- [x] At least 60 functions matched, 27 of 27 sha1 OK after each commit (61 matched; `ninja progress` grand total 901 -> 962 of 6962).
- [x] T-0018 cases recorded in [[data/t0018-cases]] (34 rows); new patterns in [[matching-notes]] (section "Main exe wave 2").
- [x] Inline code review against CODING_STANDARDS.md, no open findings.

## Notes

Matched (61): Default_Disp, Hw_Test, LoadSquare, MoveSquare, Rot_2D, StoreSquare, Sw_Test, addr_init_club_sd, addr_init_else_sd, addr_init_holiday_sd, addr_init_weekly_sd, bg_read_sub2, birth_day_check_days, cal_sprite_disp_switch, dec_init, func_80041840, func_800420D0, func_80042A00, func_80046478, func_80046590, func_80048F64, func_8004E44C, func_80053CC0, func_80054BE4, func_800570B8, func_8005907C, func_80059688, func_80062D0C, func_8006CF38, func_8006CF80, func_8006D038, func_8006D4A0, func_8006D52C, func_80071280, func_80075A64, func_80075C24, func_80077330, func_80078A0C, func_80079E9C, func_8007A924, func_8007AB24, func_8007AD6C, func_8007ADD8, func_8007B568, func_8007B7E0, func_8007B844, func_8007BAAC, func_8007BFB8, func_800847B8, func_80085E30, func_800866A0, get_h_yuukou, holiday_club_exit, last_date_spot_timer_dec, pre_syogatu_init1, pre_syogatu_init2, set_tarao_rect, set_tarao_sprt, strKickCD, syoushin_up, tpage_buf_clear_all.

Blockers (details in [[matching-notes]]): functions that return a value but are declared `void` in other agents' overlay headers (`func_80042940`, `func_80044750`, `load_palette`, ...); jump-table functions need the yaml island (not allowed here); the T-0018 register-choice family; `lui $at` sharing over several stores.

Tooling bugs seen: `tools/funcdiff.py` returns "missing in built" for any function whose name starts with an uppercase `L` (its internal-label regex `\.?L\w+` matches `LoadSquare`); `tools/srcscan.py` (`DEF_RE`) and `tools/permute.py` do not recognise old-style (K&R) definitions, so `ninja progress` aborts on one (none left in the tree); `funcdiff.py <name>` compares the object of the first `src/main` file that mentions the name, which gives a stale MATCH when the build stopped at the header check.

## Comments
- 2026-10-09 code review (inline, CODING_STANDARDS.md section 13): matches verified by the sha1 of a clean rebuild (`rm -rf asm build; configure.py; ninja`, 27 of 27 OK, `build/headers.ok` present, `ninja progress` 901 -> 962 of 6962). `NON_MATCHING` blocks: the three T-0016 blocks of `LoadSquare`/`MoveSquare`/`StoreSquare` were replaced by matching C; the three left (`func_8004111C`, `func_80044700`, `strSync`) keep their guards and ticket ids. C89 only (no `//`, declarations at block top, no K&R definitions left). Types from `game.h`; no SDK struct redefined. Externs: `tools/check_headers.py` clean; symbols that overlay headers declare themselves went to `main_only.h`; every symbol added to `game.h`/`main_only.h` is used by a `src/main` file. Fakematches carry a `FAKE` comment with the reason: indexed symbol views in `cal_sprite_disp_switch`, `func_80062D0C`, `func_8007A924`, `func_80053CC0`, the no-op `& 0xFF` in `func_8004E44C`. The bit-field `CharFlags` and the `u16`/`s32` parameter types are plain C. Two rows added to [[data/t0018-cases]] for `func_8007A924` and `func_80053CC0` were removed before commit because both functions matched afterwards. Findings: none open.
