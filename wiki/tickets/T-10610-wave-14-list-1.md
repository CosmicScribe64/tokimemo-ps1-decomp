---
id: T-10610
title: "Wave 14: list 1"
status: Done
assignee: w14-1
created: 2026-10-12
updated: 2026-10-12
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match the 54 functions assigned in scratchpad/wave14-list-1.txt within the 33 owned C files. Follow the wave-14 brief.

## Acceptance criteria

- [x] Every function has an m2c draft, build and funcdiff; deep pass on near misses.
- [x] Final incremental build passes all 27 unit checks and header/global guards.
- [x] Inline review against CODING_STANDARDS.md done.

## Comments

### Result (2026-10-12)
- Matches (6 functions, 964 bytes): EVENT `func_80103BF8`, `func_80104B00`, `func_80104D48`; SHOUGATU `func_80135B10`, `func_8013507C`; ETC `func_80140BC4` (ETC.h prototype retyped void to s32, the function returns 0/1/2).
- Guarded near misses (NON_MATCHING with ticket id): TAIIKU `func_801339C4` (4 lines, selector in $v1), EN_NICHI `func_801330D0`, EVENT `func_8010B614`, TT `func_80147FD0`, main `func_8007A868`.
- Attempted and logged in [[data/t0018-cases]] (11 rows) and [[matching-notes]] (section T-10610): InitMouse, func_8004636C, func_8007B64C, func_80134418, func_8013E094, func_80140B20, func_80147120, func_801347F4 / initModelingData_init, func_80144198, func_80135A48, func_80134110, func_80136A2C, func_801398EC (draft).
- m2c draft and review only (earlier waves already documented them as 56-180 line drafts): func_80147210, func_8014666C, func_8013AF78, func_8006D6E0, func_800764AC, func_80065964, func_801472D4, func_80147B24, func_80147D08, func_80148244, func_801407D4, func_8013B448, func_80135600, func_80147B14, func_80052F68, func_8013230C, get_h_tokimeki, func_80136808, menu_set, menu_check_1/2, menu_bar_color (T-0018 own-data case), func_80042CB0, func_80142AF0; DATE/TACO rotation helpers func_8015A7E0/CF30/D270 stay INCLUDE_ASM ([[gcc-objects]]).
- Key lesson: splat's separate D_ names for neighbouring globals can be offsets of one array; using the array form fixed load-order diffs in three functions.
- Build: ninja all units OK, build/headers.ok, build/globals.ok; sync_protos --check-branch and migrate_globals --check clean. `ninja progress` grand total 5136/6958 functions, 920516 bytes (40.4%).
- Inline review against CODING_STANDARDS.md: no findings. NON_MATCHING blocks carry T-10610 and an INCLUDE_ASM fallback; new globals declared only via sync_protos --add-global in main_api.h; overlay header edits are a prototype retype (ETC.h func_80140BC4 s32) and a K&R prototype (TAIIKU.h func_801339C4 `()`), both with evidence.
