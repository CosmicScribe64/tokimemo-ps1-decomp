---
id: T-4050
title: "Wave 3: list 5"
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the functions of wave 3 work list 5 (134 functions, 26044 bytes, 27 C files: main 80041000 and the overlays BUNKASAI, BUNKA_SD, DATE, DATE2, ETC, EVENT, GYOZI, MASTER, OPTION, TACO, TEL, TT, VALEN), best first, then continue with `queue.py --by bytes` inside those files.

## Acceptance criteria

- [x] Every match verified by `funcdiff.py --resolve` and a full `ninja` (27 of 27 OK, `build/headers.ok`).
- [x] Failures that are T-0018 shapes have one row each in `wiki/data/t0018-cases.md`.
- [x] New patterns recorded in `wiki/matching-notes.md` (own section).
- [x] Inline review against CODING_STANDARDS.md recorded below.

## Notes

Branch `w3-5`, worktree `tokimemo-wt/w3-5`. Not merged or pushed.

Result: 67 functions matched (63 of the 134 listed, 8676 of 26044 bytes, plus 4 from `queue.py --by bytes`; 9232 bytes in all), `ninja progress` grand total 2805 -> 2872 of 6958, 314276 bytes. 37 rows added to [[data/t0018-cases]]. Patterns in [[matching-notes]], section "Wave 3, list 5 (T-4050)". Shared header edits: `include/main_api.h` (about 45 new declarations; `func_80049B20` now returns `s32`, `func_800AE0B0` is variadic, `func_80051A68` and `func_80051B48` have a return type), `include/ovl/DATE.h` and `include/ovl/EVENT.h` (appended).

## Comments

### 2026-10-10 Inline review against CODING_STANDARDS.md
- Matches (1): each function checked with `funcdiff.py --resolve` against the all-`INCLUDE_ASM` object and, for strings and rodata, the overlay sha1; clean rebuild (`rm -rf asm build; configure.py; ninja`) ends with 27 of 27 `OK` and `build/headers.ok`.
- NON_MATCHING (1): none added.
- C89 (2): declarations at block top, `/* */` comments only, no C99 constructs; checked with `git diff | grep //`.
- Fakematches (7, 7a): 11 `FAKE` comments, each saying what it does: index through the first symbol (DATE `func_80153928`, `func_8015347C`, `func_80153E90`; EVENT `func_80119A60`, `func_8011B09C`; DATE2 `func_801366E8`), `(u32)` address (MASTER `func_80138374`, EVENT `func_8011B09C`), unused `s32 pad` (DATE2 `func_80132F48`), parameters used as locals (TT `func_8013F988`). The bit-field structs are not fakes: they reproduce `sll; bgez/bltz` and are documented as such.
- Types and shared externs (8, 8a): every main-exe symbol is declared once in `include/main_api.h`; `check_headers.py` passes; the two prototype changes (`func_80049B20` returns `s32`, `func_800AE0B0(void *, ...)`) keep all other users matching (sha1 of all objects unchanged).
- Wiki (13): ticket, kanban, log, matching-notes section and 37 t0018 rows updated; no new pages, so `wiki/index.md` is unchanged.
- Findings: none open.
