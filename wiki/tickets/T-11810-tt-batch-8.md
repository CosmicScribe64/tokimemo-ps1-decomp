---
id: T-11810
title: TT batch 8: Opus on untried pipeline drafts, then the closest guarded drafts
status: In Progress
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

- [ ] Pass A done
- [ ] Pass B done
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
