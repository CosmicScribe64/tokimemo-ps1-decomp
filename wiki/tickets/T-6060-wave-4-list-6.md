---
id: T-6060
title: "Wave 4: list 6"
status: Done
assignee: claude
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the functions of wave-4 work list 6 (150 functions, 40664 bytes, 33 C files: main 80041000, 800420D0, 80042540, 80057390, 80058D20, 8006CB30, 80075320 and DATE, ENDING, ETC, EVENT, GEKO, GYOZI, OMIMAI, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TT objects).

## Acceptance criteria

- [x] Work list processed in order, time-boxed per function (about 100 of 150 tried; the rest are packet builders, 400+ byte loops and functions with a recorded blocker).
- [x] T-0018 cases recorded in [[data/t0018-cases]] (17 rows).
- [x] Clean build (`rm -rf asm build; configure.py; ninja`): 27 of 27 OK, `build/headers.ok`, `build/globals.ok`.
- [x] Inline code review against CODING_STANDARDS.md recorded below.

## Notes
Result: 40 functions, 6724 bytes matched (39 of the list, 6564 bytes, plus EVENT `func_80101510` from the byte queue). `ninja progress`: grand total 3579/6958 functions, 451096/2279368 bytes (3539 before). Patterns and blockers: [[matching-notes]], section "Wave 4, list 6 (T-6060)".

Matched: main `holiday_club` `holiday_tel_call` `func_80071038` `func_800410AC` `func_80075468` `func_80076EC0` `func_80076F48` `func_80077BE0` `func_8007894C` `func_8006CD14` `func_8006CDD4` `func_8006CE84`; DATE `func_8014F370` `func_8014FBA0` `func_8015908C` `func_801591BC`; ENDING `func_80133540` `func_80133AD0` `func_801338F8`; EVENT `func_80101510` `func_80101BEC` `func_80116EF0` `func_80119824` `func_8011552C` `func_8011A144` `func_8011A328` `func_8011A508`; GEKO `func_80142A5C`; GYOZI `func_80138784`; OMIMAI `func_801327BC`; RPG_BAT `func_801446F8`; SHOUGATU `func_80133698`; SHUGAKU `func_80135A80`; TT `func_8013ABB0` `func_8013AB50` `func_8013ADC0` `func_801426F0` `func_8013C818` `func_8013CB58` `func_8013D060`.

## Comments
Code review (inline, against CODING_STANDARDS.md, 2026-10-10):
- Match rule: every match is covered by the sha1 checks of a clean build (27 of 27 OK); no NON_MATCHING blocks added. A first round of funcdiff MATCH results was wrong for DATE `func_8014F370` (the `expected/` object had been copied after a build of the edited C); the overlay sha1 caught it and the function now matches with the `u32` override.
- C89: declarations at block top, `/* */` comments, no inline/C99 constructs; ASCII comments (the Japanese string literals are UTF-8 source, converted by `cc.py`).
- Declarations (8a): main-exe symbols added to `include/main_api.h` only (prototypes of `func_80077C50`, `magazine_exit`, `restore_bgm`, `func_8009CC04` family and others; `D_800B0BCF`; `Rec34.unk_27`); overlay-local externs and prototypes appended to `include/ovl/{DATE,EVENT,ENDING,GYOZI,SHUGAKU,SHOUGATU,RPG_BAT}.h`; one override `MAIN_API_OVERRIDE_D_80122CD0` (DATE, `u32` selector in $v1) with its `#ifndef` wrapper and reason; `func_8009B430`-style K&R prototypes were not changed. `check_headers.py` and `sync_protos.py --check-branch` pass (0 new disagreements).
- Fakematches: the dummy `s32 idx;` of the function-table dispatchers (DATE x3, ENDING x2) carries a `FAKE` comment (T-3330 frame offset). `(u32)` casts that unshare constants are plain C and explained in [[matching-notes]]. No other tricks.
- Types: fixed-width types; local structs (`DrawEnv60`, `DispEnv14`, `SprEntB`-style records) carry offsets and sizes.
- Wiki: matching-notes section, 17 T-0018 rows, kanban, log. Tooling issues reported in the notes (permuter score 0 not reproducible by ninja; `expected/` hazard).
- Findings: none open.
