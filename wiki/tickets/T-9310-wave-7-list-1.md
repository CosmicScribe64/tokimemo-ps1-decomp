---
id: T-9310
title: Wave 7 list 1
status: Done
assignee: w7-1
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-7 list 1 (34 files, 153 functions, 71,436 bytes in the list).

## Acceptance criteria
- [x] Matches committed, clean rebuild `rm -rf asm build; configure; ninja` all OK, `build/headers.ok`, `build/globals.ok`
- [x] `sync_protos.py --check-branch` OK, `migrate_globals.py --check` OK
- [x] Inline code review against CODING_STANDARDS.md (below)

## Notes
Result: 30 functions, 10,492 bytes, progress 4558 -> 4588 of 6958. 24 rows appended to [[data/t0018-cases]]. Idioms and findings: [[matching-notes]] "Wave 7, list 1 (T-9310)" and [[decompile-workflow]] "Wave-7 codegen shapes (T-9310)".

Matched: main `get_weekly_bg_sector`, `menu_check`, `func_8005352C`, `func_80074F24`; EVENT `func_800F7AA8`, `func_800F7C94`, `func_800F8590`, `func_800F8878`, `func_800F9558`; DATE `func_80151F84`, `func_80152684`; DATE2 `func_80136BA4`; GEKO `func_801339AC`, `func_8013CD18`; OLH `func_8013306C`, `func_8013500C`; OPTION `func_8013A604`, `func_8013AD94`, `func_8013B410`, `func_8013BECC`, `func_8013C404`, `func_8013C52C`; RPG_BAT `func_801426B0`, `func_80142A30`; SHUGAKU `func_80134F18`; TACO `func_801570A4`, `func_80158F48`; TAIIKU `func_8013C694`, `func_801404DC`; TT `func_8013ED70`.

## Code review (inline, against CODING_STANDARDS.md)
- Section 1 (match rule): every function verified with `funcdiff.py --resolve` and the unit sha1 in a clean full rebuild; no `NON_MATCHING` blocks added.
- Section 2 (C89): declarations at block tops, `/* */` comments only, no new dialect features; K&R definitions only where the original needs them (`menu_check`).
- Section 7 (fakematches): twelve bodies carry a `FAKE` comment with a reason and `T-9310` (unused pad words for frame/array offsets in OPTION x5, OLH x2, DATE `func_80152684`, EVENT `func_800F7C94`; one-object array views in RPG_BAT `func_80142A30` and SHUGAKU `func_80134F18`; same-line call in TACO `func_80158F48`). No unmarked trick.
- Section 4/8a (names, declarations): no renames in C; main-exe globals added with `sync_protos.py --add-global`; the `menu_check` prototypes (`s8`, plus `menu_check_1/2`) were edited by hand in `include/main_api.h` because the function now returns a value, and the stale alias line `func_8004F984` was deleted (the tool reported the alias conflict); overlay-local declarations were appended to `include/ovl/{EVENT,OLH,OPTION,TT,TAIIKU,RPG_BAT}.h`; EVENT got `MAIN_API_OVERRIDE_` lines for `D_801207B0/B4/8012121C` (written by `sync_protos.py --write`).
- No findings left open.

## Comments
