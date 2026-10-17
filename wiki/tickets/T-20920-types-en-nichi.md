---
id: T-20920
title: "Targeted type pass: EN_NICHI"
status: In Review
assignee: claude-opus (ty-en-nichi)
created: 2026-10-17
updated: 2026-10-17
links: ["[[data-types]]", "[[build-step-gaps]]", "[[speedup-research-2026-10-16]]", "[[en-nichi-family-results]]"]
---

## Goal

A second, targeted type pass for EN_NICHI after the types-first pass of T-18200. Find the shared structures
that the 14 unmatched functions use (work structs, records, callback tables, stack tables, bit-fields) from the
asm access patterns and the matched siblings, declare them in `include/ovl/EN_NICHI.h` (`include/game.h`
additions only), move matched code onto them byte-identically, and redraft the unmatched functions with
`tools/redraft.py` into `drafts/T-20920/`.

## Acceptance criteria

- [x] New types declared with offsets and sizes (CODING_STANDARDS section 8); matched code uses them; every
      step builds 27/27.
- [x] Unmatched functions redrafted with `tools/redraft.py`, `drafts/T-20920/<UNIT>/<func>/result.json` with
      `scope`, `score`, `census_score`; paired score changes reported.
- [x] Types documented in [[data-types]] and [[en-nichi-family-results]].
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.
- [x] Off limits respected: no edits in `src/ovl/TT/*` or main `dialogue_render` (`src/main/8007C030.c`).

## Notes

Branch `ty-en-nichi` from `w2-good`. Results: [[en-nichi-family-results]], section "Targeted type pass (T-20920)";
types: [[data-types]], section "EN_NICHI targeted pass (T-20920)".

- Types (`include/ovl/EN_NICHI.h`): `EnObj.unk_14`/`unk_18` as `Fix32` (16.16 positions), `unk_20` `s32`;
  `EnFixPos` (two `Fix32`) for `D_801392AC[15][12]` and `D_80139228[10]` (was `EnResetSlot8` and `s16[10][4]`);
  prototypes for `func_80136700`, `func_80136904` and the new definitions. Matched uses moved to `.h[1]`, sha1 unchanged.
- Matched (8 functions, 6,328 bytes, all by hand): `func_80135ACC` (1,160; one FAKE empty statement from the
  permuter), `func_80134800` (1,052), `func_801339D4` (724), `func_80132450` (780), `func_801330D0` (736, was guarded,
  T-10610), `func_80135274` (620), `func_80136A2C` (228, was guarded, T-10850), `func_80138CCC` (1,028).
- Still unmatched (6, 6,636 bytes): `func_80132EE8` (hand draft 6 rows: two `lui` in the other order),
  `func_801362A4` (11), `func_80135FBC` (14; 5 with a FAKE), `func_8013358C` (46, T-18200 hand), `func_80136DEC`,
  `func_8013735C` (redraft only).
- Paired redraft (`tools/redraft.py --unit EN_NICHI`, default variants, snapshots of `f3933305` and `c2dabc21`): the
  6 unmatched keep their best scores (242, 77, 288, 65, 480, 584); the 8 matched scored 1,521 rows in the baseline.
- Config: `config/symbol_addrs_types.txt` gains `D_800E7D10` (link name for the `Work800E7D10` base, so overlay C can
  read `D_800E7D10.flags[n]`); all 27 sha1 unchanged.

## Comments

[2026-10-17] Done in one session (07:03 to 09:20 UTC). Every match was checked with the EN_NICHI sha1 in
ninja (all 27 OK) and committed on its own; the clean build `rm -rf build && python3 configure.py && ninja` gives
27/27; `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` pass. No edit in `src/ovl/TT/*`
or `src/main/8007C030.c`. Code review against `CODING_STANDARDS.md` still open (In Review).
