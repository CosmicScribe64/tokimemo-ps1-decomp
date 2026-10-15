---
id: T-11800
title: TT batch 7: bulk_m2c rerun, Opus on close drafts, Opus on pipeline drafts and the gcc atan2
status: In Progress
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

- [ ] bulk_m2c rerun; matches outside the wave files committed
- [ ] Opus pass 1 and pass 2 done; matches one per commit, near misses guarded with `T-11800`
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
