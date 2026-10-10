---
id: T-6050
title: Wave 4: list 5
status: Done
assignee: wave4 agent 5
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[game-state]]", "[[data/t0018-cases]]"]
---

## Goal
Match as many of the 166 functions (41008 bytes) of wave 4 list 5 as possible: main 80047550, 800490C0, 80049FF0 and the overlays BUNKA_SD, DATE, ETC, EVENT, GEKO, GYOZI, KANGEI, NAME_ENT, OMIMAI, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TT (31 C files, listed in the work list). Worktree w4-5, branch w4-5.

## Acceptance criteria
- [x] Clean build (`rm -rf asm build; configure.py; ninja`): 27 of 27 OK, headers OK, globals OK.
- [x] `sync_protos.py --check-branch` (0 new disagreements) and `migrate_globals.py --check` clean.
- [x] T-0018 cases recorded in `wiki/data/t0018-cases.md` (49 rows).
- [x] Inline review against CODING_STANDARDS.md recorded below.

## Notes
Result: 40 of 166 listed functions matched (6492 of 41008 bytes) plus ETC `func_8014A258` (108 bytes, from the byte queue); progress 3539 -> 3580 of 6958 functions, grand total 450960 of 2279368 bytes (19.8%). Patterns and blockers: [[matching-notes]], section "Wave 4, list 5 (T-6050)". Blocked functions: 49 rows in [[data/t0018-cases]]; two more (DATE `func_80132000`, one-nop lead; ETC `func_80142AE8`, hoisted end pointer) are noted in matching-notes only.
FAKE comments (3): KANGEI `func_80134EA8` (unused `s32 pad`), DATE `func_80138418` (`s32 sp28[2]`), TACO `func_80144D90` (assignment pairs on one source line).
Header changes: declarations of `main_api.h` symbols the matched C needs (appended through `tools/sync_protos.py --fix`), overlay-local externs appended to `include/ovl/{BUNKA_SD,DATE,ETC,NAME_ENT,SHUGAKU,TACO}.h`.

## Comments

### Review (inline, CODING_STANDARDS.md checklist) 2026-10-10
- Matches verified: every function by `funcdiff.py --resolve` MATCH and by the clean build (27/27 sha1 OK); no `NON_MATCHING` blocks added.
- C89: declarations at block top, `/* */` comments only, no C99. Bit-field structs (`KangeiFlagBits`) are local to the .c file like ETC's `EtcSlot`.
- Fakematches: three, each with a `FAKE` comment and listed in Notes; one non-obvious plain-C form (ETC `func_80147414`, pointer local) has an explanatory comment.
- Types and headers: main-exe symbols declared once in `main_api.h` (current names: `k_disp_inc`, `x_taku_string_set`, not the `func_` aliases; `--check-branch` reports 0 new disagreements), overlay-local ones in the overlay headers, no old `GameState` names (`build/globals.ok`).
- No game data staged; commits are small, each starts with `T-6050:`.
- Findings: none open.
