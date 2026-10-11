---
id: T-9180
title: Wave 6: list 8
status: Done
assignee: w6-8
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match the functions of wave-6 work list 8 (150 functions, 36 files).

## Acceptance criteria
- [x] Matched functions build byte-identical, 27/27 OK, headers OK
- [x] Inline review against CODING_STANDARDS.md

## Notes
Result: 38 functions matched, 13,328 bytes (dec_bg_reset, func_8005751C, func_8013EE3C, func_80139E08, func_8013EAC0, func_800FFF50, func_801004F0, func_80100888, func_8013C73C, func_8013F0BC, func_8013FF64, func_8014016C, func_801410A4, func_80133374, func_8013B20C, OLH func_80135CB4/80135DD4/80135EC0/80136000/80136124/801362C4/801363CC, func_8013F3C0, func_80140BD8, func_80133980, func_80154F74, func_80155230, func_8015595C, func_80155F98, func_80156178, func_80156924, func_80159678, func_80159F60, func_8015A164, func_8015A378, func_8015A680, TEL func_80139E90, func_8013A2F0). Patterns: [[matching-notes]], "Wave 6, list 8 (T-9180)". 22 functions that were tried and not reproduced have rows in [[data/t0018-cases]].

FAKE markers (to revisit): DATE `func_8013EE3C` and TEL `func_80139E90` (spare `s32 pad` for the frame), TACO `func_80154F74`, `func_8015595C`, `func_8015A164` (unused leading `TcPos`), TACO `func_80156178` (stores on one line), OLH `func_80135CB4` (`lbu` view of the s8 `unk_1093`).

Own data: main `80057390` now defines its `.data` (`s32 D_800B5920[4]` and the neighbours); `config/symbol_addrs_types.txt` has `D_800B5920` for overlay C.

## Comments

### Review (inline, CODING_STANDARDS.md)
- Section 1/6: every replaced `INCLUDE_ASM` keeps its symbol and position; the 38 objects match (unit sha1 OK for all 27 units after a clean rebuild).
- Section 2/8: fixed-width types, K&R-friendly C89 (declarations at block start); new structs carry offsets and sizes (`TkPl` was dropped with its function; `Tbl21`, `Tbl12` not kept; `MasterSlot` documents bit 30 as the flag).
- Section 7: all tricks that are not plausible source carry a `FAKE` comment (see Notes); no dummy statements. `func_8013EAC0` has no self-assignment (the m2c `D = D` was dropped, it still matches).
- Section 8a: new main-exe declarations only through `include/main_api.h` (`D_800B5920[4]` replaces four scalars, `D_80122CE8`-style names were not needed); overlay symbols appended to `include/ovl/*.h` (OLH, DATE, EVENT, TACO, MASTER, SHOUGATU); `sync_protos.py --check-branch` OK, `migrate_globals.py --check` OK, `headers OK`.
- Section 10: no game data or asm committed; strings are the game's own text in the source as in the sibling files.
No open findings.
