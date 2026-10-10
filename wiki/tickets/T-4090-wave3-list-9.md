---
id: T-4090
title: "Wave 3: list 9"
status: Done
assignee: agent (w3-9)
created: 2026-10-09
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal

Match the INCLUDE_ASM functions of wave-3 work list 9 (136 functions, 26284 bytes) in 27 files of the main exe and the overlays BUNKASAI, DATE, DATE2, ENDING, EN_NICHI, ETC, EVENT, GEKO, GYOZI, MASTER, OMIMAI, SHOUGATU, SHUGAKU, TACO, TAIIKU, TT.

## Acceptance criteria

- [x] Work list processed in order, time-boxed per function: 59 of 136 functions, 8984 of 26284 bytes matched
- [x] Every commit ends with 27 of 27 sha1 OK and `build/headers.ok` (final: `rm -rf asm build; configure; ninja`, no `-k`, 27/27 OK)
- [x] T-0018 cases appended to [[data/t0018-cases]] (18 rows)
- [x] Inline review against CODING_STANDARDS.md recorded below

## Notes

Branch w3-9, worktree tokimemo-wt/w3-9. Patterns and failures: [[matching-notes]], section "Wave 3, list 9 (T-4090)". Progress grand total 2805 -> 2864 of 6958.

Method: m2c drafts for the whole list, a scratch loop (`tools/cc.py` + `funcdiff.py --built`, no ninja) for each file, ninja only for the sha1 check; decomp-permuter on small near misses (about a quarter reached score 0). `main_api.h` additions were made by hand (sorted by address) and checked with `tools/check_headers.py`.

## Comments

- 2026-10-10: inline review against CODING_STANDARDS.md, no open findings. 1 Match: final clean build 27 of 27 sha1 OK, `build/headers.ok`; every matched function was also checked with `funcdiff.py --resolve` (relocation-only differences for strings, jump tables and calls into other objects; sha1 decides). 2 NON_MATCHING: none added; every failed attempt is back to `INCLUDE_ASM`. 3/8a: no SDK header touched; new main-exe symbols are in `include/main_api.h` only (unused additions pruned), overlay symbols in `include/ovl/<NAME>.h`; overrides added: `MAIN_API_OVERRIDE_set_kanji_string` (ENDING) and `MAIN_API_OVERRIDE_func_80046094` (GEKO), both with the reason; `func_80044750` became `s32`, `func_80079B10` `void(u16)`, `func_8007BE94` `void(s16)`, `func_800AE0B0` K&R, `func_80046094` `u8(void)`. 4: renamed callees use the new names. 7: fourteen `FAKE` comments, each with reason and ticket: index trick for a load kept after a store (DATE x4, EN_NICHI x2, GYOZI x1, SHUGAKU x1 with a negative index), unused local for a frame (DATE `func_8014FDA0`, main `func_80079D28`), empty double test (GYOZI `func_80135CA0`), `* 0` (GYOZI `func_80135D64`), `^ 0` (main `func_8007BE94`), `(u32)` cast (main `func_80057640`); the `volatile` of `D_8012B920` is a declaration documented in `GYOZI.h`. T-3330 idiom comments on the table copies. 8: fixed-width types, bit-field struct documented; struct/field offsets in comments. 9: ASCII comments, no commented-out code. 12: one commit per batch, only intended files. 13: ticket, kanban, log updated.
