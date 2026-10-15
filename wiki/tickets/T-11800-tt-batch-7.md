---
id: T-11800
title: TT batch 7: bulk_m2c rerun, Opus on close drafts, Opus on pipeline drafts and the gcc atan2
status: Done
assignee: claude (diman8 fork)
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-11560-tt-batch-6]]", "[[decompile-workflow]]", "[[data/t0018-cases]]", "[[gcc-objects]]"]
---

## Goal
Seventh TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2). Stays off the TT files of wave 1 (T-11620
`80133970.c`, `80142D40.c`; T-11630 `8013C6D0.c`); asked on #2 which files the rest of the wave will touch. Ticket number
above the wave's reserved range T-11570..T-11740.
1. `tools/bulk_m2c.py` rerun on TT with the current pipeline (T-11210/T-11230 improvements), fresh state.
2. Opus pass 1: the closest guarded TT drafts.
3. Opus pass 2: the best pipeline drafts, and `func_8014D260` (PsyQ atan2, gcc-built: [[gcc-objects]]).

## Acceptance criteria

- [x] bulk_m2c rerun; matches outside the wave files committed
- [x] Opus pass 1 and pass 2 done; matches one per commit, near misses guarded with `T-11800`
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
TT: 225/303 -> 235/303 (52.8% -> 56.7%) on top of the wave-1 matches. 10 functions matched.
- bulk_m2c rerun (fresh state, `--variants 6 --time 150`): 53 targets, no match, 52 drafts ranked by score.
- Opus pass 1, guarded drafts (about 38 minutes, 216k tokens): matched `func_80142350` (544 bytes), `func_80149BB0` (436),
  `func_80145370` (360); closer `func_8013B4A0` (20 -> 4 lines), `func_80148E7C` (11 -> 7); unchanged `func_80148764`,
  `func_8014C5F8`, `func_801392A0`; too far `func_80147FD0`, `func_8013C0A8`, `func_80139B98`, `func_8013A3E0`,
  `func_801374C0`, `func_8013BE9C`.
- Opus pass 2, pipeline drafts and the gcc function (about 53 minutes, 336k tokens): matched `func_8014CFEC` (380),
  `func_80149400` (216), `func_801491F0` (528) and its twin `func_8014ACEC` (532), `func_8013AE3C` (676), `func_80138B90`
  (384), `func_80132198` (372); not kept `func_80146E10` (42 lines), `func_80146924` (162); not reached `func_80137654`.
  `func_8014D260`: no gcc/aspsx combination matches; three ASPSX behaviours maspsx lacks recorded in [[gcc-objects]].
- New TT.h types/externs: `TtAnim14`, `D_80156C90`; `TtActor78.unk_48`.
- Removed the stale `void-result TT:func_8014D260` line from `config/proto_known.txt` (the prototype is `s32` since
  T-11000).
- Process: an agent commit that did not build on its own (a `sed` ampersand broke a guard comment, fixed in the next
  commit) was squashed with its fix; the final tree was unchanged.

## Review
Inline, against CODING_STANDARDS.md: full rebuild 27/27; `funcdiff --resolve` MATCH for all ten; prototype and global
checks clean; every new fakematch (empty-if levers, the dead local `d2`, the extra call argument, the `s32` view, the
pad) carries `FAKE` with T-11800 on its line; the wave-1 TT files are untouched. No open findings.

## Comments
