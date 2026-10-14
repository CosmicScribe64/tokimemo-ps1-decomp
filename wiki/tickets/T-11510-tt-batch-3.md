---
id: T-11510
title: TT overlay batch 3: Opus near misses, Sonnet larger functions
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11500-tt-batch-2]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]"]
---

## Goal
Third TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11500 (PR #4). In order:
1. Opus on the guarded near misses from T-11500: `func_80144A04` (3 lines), `func_8013B6C8` (13), `func_80147644` (18),
   `func_80136244` (register naming only).
2. Sonnet on a shorter list of 14 untried TT functions (the 4 read-only ones from T-11500 and the 10 smallest untouched).

## Acceptance criteria

- [ ] Opus pass done; each near miss matched or its guard comment/row updated
- [ ] Sonnet list done; matches one per commit, near misses guarded with `T-11510`, T-0018 rows
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
