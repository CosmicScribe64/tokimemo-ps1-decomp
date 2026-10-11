---
id: T-9120
title: Wave 6: list 2
status: Done
assignee: w6-2
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match functions from wave-6 work list 2 (37 files, 148 functions) in branch w6-2.

## Acceptance criteria
- [x] Matches committed, 27/27 OK, headers OK, globals OK
- [x] Inline review against CODING_STANDARDS.md

## Notes
Branch w6-2, list `wave6-list-2.txt` (37 files, 148 functions). Result: 39 functions matched (4267 -> 4306 of 6958; about 13 KB): RPG_BAT 23 (dialogue-script pages and short state machines), OLH 6 (help pages), DATE 2, NAME_ENT 2, GYOZI, main (with the T-9010 data island of `80043510`), ETC, SHOUGATU, TACO, VALEN one each. Patterns in [[matching-notes]] ("Wave 6, list 2"). 24 functions that failed after a real attempt are rows in [[data/t0018-cases]] (selector/local in `$v1`, constants numbered in another order, stack-argument store in the delay slot, a few more). TACO `func_8015CF30` is hand-scheduled libgte assembly and cannot be matched from C.
Tooling notes: `funcdiff.py` cannot judge jump-table functions (table relocations and `.L` labels show as differences; use the sha1 of the unit); `maindecl` flow for a new main-exe global: declare it in `include/game.h`, run `sync_protos.py --fix`, which moves it into `main_api.h`; `sync_protos --fix` ignores an extern that only a `.c` file declares.

## Comments
Inline review against CODING_STANDARDS.md (no reviewer sub-agents), 2026-10-10:
- Matches verified by a clean rebuild: 27/27 sha1 OK, `build/headers.ok`, `build/globals.ok`; `sync_protos.py --check-branch` OK, `migrate_globals.py --check` OK.
- No `NON_MATCHING` blocks; unmatched functions are back on `INCLUDE_ASM` (parked drafts stay in the scratchpad only).
- C89 (declarations first, `/* */` comments), UTF-8 string literals only, fixed-width types; struct offsets documented where new types were added (`GyoziBit3`, `FnTbl42`).
- Two `FAKE` comments, both the T-9020 idiom `f(*(u8 *)&D = k)` (GYOZI `func_8013BB0C`, SHOUGATU `func_8013AB5C`); no other fakematch.
- Declarations: one main-exe symbol added (`func_8009ED74` in `main_api.h` through `sync_protos --fix`); overlay prototypes changed from `void` to `s32` for the functions that need the selector copy (OLH x6, RPG_BAT `func_80137280`, `func_801373A8`). `config/SLPM_86.053.yaml` got the `.data` island line from `tools/data_island.py` (not edited by hand).
- No game data, asm, build output or expected objects staged.
No open findings.
