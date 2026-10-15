---
id: T-11820
title: TT batch 9: Opus on never-attempted TT functions, the four skipped drafts and func_801333D4
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-11810-tt-batch-8]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Ninth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11810 (PR #31). Stays off the wave-1 TT files
(`80133970.c`, `80142D40.c`, `8013C6D0.c`). Per T-11810, no more passes on the closest guarded drafts (their remaining
differences are IDO limits); one Opus pass on:
1. TT functions no agent has attempted (bulk_m2c drafts exist): `func_80149704`, `func_80136830`, `func_8013F3B4`,
   `func_80146000`, `func_801410A0`, `func_80138808`.
2. The four T-11810 pass B did not reach: `func_80142AF0`, `func_8014B274`, `func_801487D0`, `func_80146E10`.
3. `func_801333D4` (m2c cannot draft it: case labels inside else-if arms), structure by hand.

## Acceptance criteria

- [ ] Pass done; matches one per commit, near misses guarded with `T-11820`
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
