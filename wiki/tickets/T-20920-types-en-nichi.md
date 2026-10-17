---
id: T-20920
title: "Targeted type pass: EN_NICHI"
status: Done
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

[2026-10-17] Done in one session (07:03 to 09:10 UTC). Every match was checked with the EN_NICHI sha1 in
ninja (all 27 OK) and committed on its own; the clean build `rm -rf build && python3 configure.py && ninja` gives
27/27; `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` pass. No edit in `src/ovl/TT/*`
or `src/main/8007C030.c`. The code review against `CODING_STANDARDS.md` was still open at that point; it is below.

### 2026-10-17 claude-sonnet (review): code review against `CODING_STANDARDS.md`, Done

Reviewed `git diff w2-good...ty-en-nichi` (13 commits, 56 files; 11 are `src`, `include`, `config` and `wiki` files, the rest
`drafts/T-20920/`) against the checklist. Two small findings, both fixed; nothing left open.
- Scope: no edit in `src/ovl/TT/*` and none in `src/main/8007C030.c` (`dialogue_render`); no game data, binaries or
  `disc/` content (the `drafts/T-20920/` files are the usual draft bodies and header contexts, as in the other
  `drafts/T-*` folders). The one edit outside the stated ownership, the `D_800E7D10` link name in
  `config/symbol_addrs_types.txt`, is a single `T-20920` line in the existing format and is needed to link.
- Matches: the eight new matches are plain C with no `#ifdef NON_MATCHING`; the two guards that existed
  (`func_801330D0`, `func_80136A2C`) are gone with their `INCLUDE_ASM` fallbacks, and no guard was added. Clean
  `rm -rf build && python3 configure.py && ninja` in Docker: 27/27 sha1 OK. The splat "Unable to determine a segment"
  lines are the known main-exe symbol messages (none names a symbol of this branch).
- C89: the scan of added `src` and `include` lines found no `//` comment (the `//` in `symbol_addrs_types.txt` is the
  splat symbol-file syntax), no `inline`, `bool` or non-ASCII text; locals are declared at block top (the `Fix32 x, y`,
  `EnHalf64 tbl` and `u8 list[15]` locals included).
- Types: `EnObj` and `EnFixPos` carry offsets and sizes; `Fix32`/`DisplayFixActor44` are the T-20900 game types, no SDK
  struct redefined; the new `func_80136700`/`func_80136904` prototypes and the `s32 arg0` prototypes match their
  definitions (the `BUNKA_SD` `func_80136904(void)` is another overlay and another header closure). `EnResetSlot8` and
  the `s16 [10][4]` view have no remaining user. `check_headers` OK, `migrate_globals --check` OK,
  `sync_protos --check-branch` OK (0 new, 15 known).
- Matched code changed only to use the new types (`.unk_16` to `.unk_14.h[1]`, `.unk_1A` to `.unk_18.h[1]`, the grid and
  centre halves to `.unk_00.h[1]`/`.unk_04.h[1]`); the sha1 of every unit is unchanged.
- FAKE: one new marker, the empty statement in `func_80135ACC` (reason and `T-20920` in the comment). The FAKEs of the
  two former guards are gone. The `s32 func_80132450(void)` without a return is the implicit-int view of its
  prototype-less caller, explained in its comment, not a fakematch.
- Findings fixed: (1) a blank line right after the opening brace of `func_801339D4` removed (clean rebuild still
  27/27); (2) the "Left for later waves" line of [[en-nichi-family-results]] still listed four functions this ticket
  matched, now annotated.
- Wiki: types in [[data-types]] (table row and section "EN_NICHI targeted pass (T-20920)"), results in
  [[en-nichi-family-results]]; status, kanban card (In Review to Done), index line and log agree.
