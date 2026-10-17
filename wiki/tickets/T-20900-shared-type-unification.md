---
id: T-20900
title: "Cross-unit shared-type unification"
status: In Review
assignee: claude-opus (ty-shared)
created: 2026-10-17
updated: 2026-10-17
links: ["[[data-types]]", "[[build-step-gaps]]", "[[speedup-research-2026-10-16]]"]
---

## Goal

The per-unit types-first passes (T-16550, T-16560, T-17000 .. T-18800) could only add to `include/game.h`, so the
shared tables have many parallel per-overlay views. Start with the 0x44-byte display-record table `D_8011ECD0`
(and the pools of the same record at `D_800E9620` in EVENT and `D_80129F40` in GYOZI): find the views of the same
table, define one shared record type in `include/game.h` from the union of all users' accesses (widths from the asm
loads and stores), and convert the users where the build stays byte-identical. Where a field's width or signedness
really differs between users, keep a documented view. Then do the same for other tables with three or more
per-overlay duplicate types. Measure the effect by rescoring the drafts of unmatched functions that use the
unified tables (`tools/redraft.py`).

## Acceptance criteria

- [x] One shared 0x44 record type in `include/game.h`; per-overlay duplicates converted or documented as views.
- [x] Other tables with three or more duplicate types handled the same way.
- [x] Every matched function byte-identical (clean build 27/27); `check_headers`, `migrate_globals --check`,
      `sync_protos --check-branch` pass.
- [x] Draft score changes measured and recorded.
- [x] [[data-types]] and the family pages document the types.
- [ ] Code review against `CODING_STANDARDS.md` (before Done).

## Notes

Off limits: `src/ovl/TT/*` (contributor, issue #2) and main `dialogue_render` in `src/main/8007C030.c` (issue #45).

## Comments

### 2026-10-17 claude-opus (ty-shared): work done, ready for review

Start 04:57 UTC, worktree tokimemo-wt/ty-shared from `w2-good` (dcfb746d). Every step built byte-identical
(27 sha1 OK); the full layout and the evidence are in [[data-types]] ("Shared-type unification"), the GameState words
in [[game-state]], one T-20900 line on each family page.

- Display-record pools (`D_8011ECD0`, EVENT `D_800E9620`, GYOZI `D_80129F40`, 0x44 records): `DisplayActor44`
  (game.h) is the union of all accesses (asm scan: bytes only `lbu`/`sb`, halfwords `lh`/`sh`, pointers +0x0C/+0x10/+0x34,
  word +0x38), `DisplayFixActor44` the view with 16.16 words at +0x24/+0x28 (`Fix32`) and words at +0x2C/+0x30. 35
  per-unit record type names and 6 fixed-point unions retired (list in [[data-types]]). Kept: TAIIKU `TaiikuActor44`
  (s8 at +0x01/+0x06; three functions mismatch with u8, measured).
- Byte-offset uses of the pools in matched code became record fields (base symbol kept): 669 constant, 12 record
  copies, 83 indexed; EVENT `func_80108DD4` keeps its byte view (one more register with fields), now marked FAKE.
  RPG_BAT's block views became record indexing. Two FAKE comments removed, one narrowed, one added (850 to 849).
- Other families with three or more views: `D_800E7D4F` (`Flags7D4F`), `D_80122EB8` (`BunkaDispatch8` in game.h),
  matrix/vector copies (`MATRIX`, `VECTOR`), GameState +0x54C (`ScheduleEntryFlags`), +0x66C (`ScheduleClubFlags`),
  +0xF4 (`ScheduleFlags`), +0xF8 (`GsDateBits`; NAME_ENT's swapped day/month names fixed), and the work area
  `D_800E7D10` (`Work800E7D10` in main_api.h for main's FlagsWork24, the album view NameMetricWork and TACO's TcSetAll;
  the OPTION and TACO overrides are gone). Not merged: `D_801217D0` (two views), RENSYU's `u16` club view, TACO's
  `TcSave` union and ETC's `EtcConfig8` casts.
- 68 type names gone in all (702 to 631 typedefs in include/ and src/, TT excluded, four new).
- Tools: `tools/redraft_rewrites.py` reads MASTER's pool as `DisplayFixActor44`, `TYPE_ALIASES` maps the retired names
  (and old field names) in archived bodies, NAME_ENT's object view of `D_800E7D10` is gone (the header declares it);
  `tools/redraft_extras.py` club-bits uses `ScheduleClubFlags`, record-copy the unit's record view. Tests:
  `tools/test_redraft.py` 99 pass (228 with check_headers, sync_protos, migrate_globals, type_recovery).
- Off limits respected: no edit in `src/ovl/TT/*` or `src/main/8007C030.c`; TT builds unchanged with the new headers.

Measurement (`tools/redraft.py`, variants plain/typed/typed-late, no draft_fixes, before = snapshot of dcfb746d,
after = snapshot of the unified headers, each run from its own tree under build/ty/):
- Header types (0x44 views, bit views, byte folds): 166 unmatched pool users, before and after. 153 score on both
  sides with the same best score (34,580 rows), 12 on neither, 1 new: `main:func_80077900` 163 rows (census and best
  archive 211), because the club-bits rewrite, gated on MASTER's private type before, now runs in every unit. Draft
  text: 51 of 111 typed bodies of the 37 that touch the new fields got field names instead of casts (1,057 to 945
  casts); scores identical, as IDO compiles a member like a cast at that address (T-7010). A first run showed the
  record-copy rewrite writing DisplayFixActor44 everywhere (3 typed drafts stopped compiling); fixed, rerun equal.
- `Work800E7D10` as the declared type of `D_800E7D10`: 34 unmatched users, 13 change (3 better: main:func_8005CC40
  92 to 15, NAME_ENT:func_80139F30 1,171 to 992, ETC:func_80144F74 115 to 46; 10 worse, sum 8,201 to 8,297); with the
  archived bodies on the new headers 8 of the 13 beat their best archive, 3 fall behind (ETC func_8013E6AC 216/130,
  func_8013F054 249/238, main func_8005CA94 65/61). This is the label-fold effect (one symbol instead of many).
  Improved drafts in `drafts/T-20900/`. main:func_8005CC40 tried by hand: 15 rows, the switch value's register in
  case 1 (original $v1, draft $v0); not matched, source left as INCLUDE_ASM.

Final checks: clean `rm -rf build && python3 configure.py && ninja` 27/27, `check_headers` OK, `migrate_globals --check`
OK, `sync_protos --check-branch` OK (0 new, 15 known). No draft reached 0, so nothing new to match.
