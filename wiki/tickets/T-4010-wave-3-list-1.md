---
id: T-4010
title: "Wave 3: list 1"
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the functions of wave 3 work list 1 (86 functions, 20848 bytes) in 26 C files: main 80047550 and the overlays DATE, ENDING, EN_NICHI, ETC, EVENT, GEKO, GYOZI, KANGEI, OPTION, RPG_BAT, SHUGAKU, TACO, TT.

## Acceptance criteria

- [x] List worked in order, time-boxed; blocked T-0018 cases reverted to INCLUDE_ASM with a [[data/t0018-cases]] row (13 rows).
- [x] Clean build (`rm -rf asm build; configure; ninja`, no `-k`): 27/27 OK, `build/headers.ok`.
- [x] Inline review against CODING_STANDARDS.md recorded below.

## Notes

Result: 51 of the 86 listed functions matched, 11536 of 20848 bytes (main_api.h gained 95 symbols, 13 headers gained overlay-local externs). Patterns, failures and tooling notes: [[matching-notes]], section "Wave 3, list 1 (T-4010)". Commits `T-4010: match ...` on branch `w3-1`; not merged, not pushed.

## Comments

Inline review against CODING_STANDARDS.md (13 checklist items), 2026-10-10:
- Matches verified: every match is covered by the sha1 of its overlay or exe in the clean build (27 of 27 OK); `funcdiff.py` was used only as a first filter.
- No `NON_MATCHING` blocks. Every reverted function is `INCLUDE_ASM` again, in its original position.
- C89 only: new code has declarations at block tops (checked with a grep over the diff for `//` and mid-block declarations; none), `/* */` comments, no K&R definitions kept.
- Headers: all new main-exe symbols went through `tools/sync_protos.py --write` into `include/main_api.h`; overlay-local externs were appended to `include/ovl/<NAME>.h`; `tools/check_headers.py` passes. Two types were changed in main_api.h with a comment: `func_80082764` (`s32`) and `func_800AE0B0` (unprototyped); both rebuilt all 27 targets identical. `D_801227A0` is not in main_api.h (TAIIKU declares it as `TaiikuBig[]`); ETC reaches it through `D_801227A4 - 4`, explained at the use.
- Fakematches: 17 marked `FAKE ... T-4010` (unused pad locals that reproduce the original table or frame offsets, one base symbol for neighbouring byte or halfword globals). One unmarked trick was found in review and marked (DATE `func_801510E8`, RPG_BAT `func_8013D030` absolute load).
- Types fixed-width; struct layouts documented (FnTbl sizes, EtcMat/EtcSVec/EtcVec, DateTxt).
- No game data, `asm/`, `build/`, `expected/` or `disc/` committed.
- Wiki: matching-notes section, 13 rows in `wiki/data/t0018-cases.md`, kanban and log updated.
No open findings.
