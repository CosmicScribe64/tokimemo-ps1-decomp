---
id: T-11810
title: TT batch 8: Opus on untried pipeline drafts, then the closest guarded drafts
status: Done
assignee: claude (diman8 fork)
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-11800-tt-batch-7]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Eighth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11800 (PR #30). Stays off the wave-1 TT files
(`80133970.c`, `80142D40.c`, `8013C6D0.c`); no answer on #2 yet, upstream unchanged at the start.
- Step 1: Opus pass A on untried bulk_m2c drafts (smallest first): `func_80149B20`, `func_801369F4`, `func_8013EC64`,
  `func_80149EA0`, `func_8013F9E0`, `func_80142110`, `func_8013A0DC`, `func_8013C41C`, `func_80140D84`, `func_8013F040`,
  `func_80135870`.
- Step 2 (later, after the user's usage reset): Opus pass B on the closest guarded drafts and earlier pipeline attempts.
- Step 3: wrap-up and PR.

## Acceptance criteria

- [x] Pass A done
- [x] Pass B done
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
TT: 235/303 -> 242/303 (56.7% -> 59.1%). 7 functions matched (2,864 bytes).
- Pass A (untried bulk_m2c drafts; about 54 minutes, 375k tokens): matched `func_80149B20` (144 bytes), `func_801369F4`
  (228), `func_8013EC64` (268), `func_80149EA0` (352), `func_8013F9E0` (496), `func_80135870` (484), `func_8013C41C`
  (692); guarded `func_8013A0DC` (16 lines); not kept `func_80140D84` (46), `func_8013F040` (218), `func_80142110` (~130).
- Pass B (closest guarded drafts; about 22 minutes, 207k tokens): no match and no commit; causes narrowed for
  `func_8013B4A0`, `func_80148E7C`, `func_80148764`, `func_80147FD0`/`func_8014C5F8`, `func_8013A0DC`, `func_80140D84`;
  the rule for call-argument constant sharing found (see [[matching-notes]] "TT overlay, batch 8"). `func_80146E10`,
  `func_8014B274`, `func_80142AF0`, `func_801487D0` not reached.
- `include/ovl/TT.h`: scene tables and handler prototypes for `func_801369F4`, moved into their own guarded block
  (`OVL_TT_T11810_SCENE_TABLES`; the agent had appended them inside the T-11800 `TtAnim14` block).
- [[matching-notes]]: the DATE2 `func_8013775C` note about `func_80084D3C(0x38U)` marked superseded (the call has no
  argument since T-9210).

## Review
Inline, against CODING_STANDARDS.md: full rebuild 27/27 plus headers; `funcdiff --resolve` MATCH for all seven;
prototype and global checks clean; new fakematches (two empty `if (c) {}` levers, the `s32` view and pad of the
`func_80149400` twin) carry `FAKE` with T-11810; wave-1 TT files untouched. Finding fixed: header block placement.

## Comments
