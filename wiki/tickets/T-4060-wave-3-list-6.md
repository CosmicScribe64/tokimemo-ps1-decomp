---
id: T-4060
title: Wave 3: list 6
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the functions of wave-3 work list 6 (108 functions, 24120 bytes, 28 C files: main 800674B0 and 80085E30, and BUNKA_SD, DATE, DATE2, ETC, GYOZI, MASTER, NAME_ENT, OMIMAI, RPG_BAT, SHOUGATU, TACO, TT, VALEN objects).

## Acceptance criteria

- [x] Work list processed in order, time-boxed per function (about 70 of 108 tried; the rest are TACO/NAME_ENT/main functions with a known blocker or a library routine).
- [x] T-0018 cases recorded in [[data/t0018-cases]] (17 rows).
- [x] Clean build (`rm -rf asm build; configure.py; ninja`): 27 of 27 OK, `build/headers.ok`.
- [x] Inline code review against CODING_STANDARDS.md recorded below.

## Notes
Result: 26 functions, 3832 bytes matched (verified by the overlay and main sha1 checks of a clean build). `ninja progress`: grand total 2831/6958 functions, 308876/2279368 bytes. Patterns and blockers: [[matching-notes]], section "Wave 3 list 6 (T-4060)".

Matched: GYOZI `func_8013AF1C` `func_8013B03C` `func_8013B2B8` `func_80140C90`; DATE `func_8014D6B0` `func_8014DB08` `func_8014DF54` `func_8014DFB0` `func_8014E2E8` `func_8015A4A8` `func_8015A58C`; DATE2 `func_80132000` `func_801324A8`; MASTER `func_80139150`; OMIMAI `func_80134030`; RPG_BAT `func_8014EEF4`; SHOUGATU `func_801386CC` `func_80138710` `func_80138E1C` `func_801391F4` `func_80139734`; TT `func_80138D94` `func_8013BD4C` `func_8013BD80`; VALEN `func_80133A4C`; main `func_8008667C`.

## Comments
Code review (inline, against CODING_STANDARDS.md, 2026-10-10):
- Match rule: every match is covered by the sha1 checks of a clean build (27 of 27 OK); no NON_MATCHING blocks added.
- C89: declarations at block top, `/* */` comments, no inline/C99 constructs; ASCII comments.
- Declarations (8a): main-exe symbols added to `include/main_api.h` only (`D_800E65FE`, `D_80122D04`); overlay-local externs and prototypes appended to `include/ovl/{DATE,DATE2,OMIMAI,TT}.h`; `check_headers.py` passes; no `MAIN_API_OVERRIDE` added.
- Fakematches: `func_8008667C` carries a `FAKE` comment (`(u8)` cast, same as GYOZI `func_80140D7C`). Explained non-fake tricks have comments: one-base access of adjacent globals (`func_8014D6B0`, `func_80140C90`, `D_80120650[...]` accessors), init order of the TT loops.
- Types: fixed-width types; function-pointer table typedefs carry their size; the index-declared-first idiom (T-3330) is commented.
- Wiki: matching-notes section, 17 T-0018 rows, kanban, log. Tooling bugs reported in the notes (funcdiff `locate()` picks a caller's file; stale MATCH while `headers.ok` fails).
- Findings: none open.
