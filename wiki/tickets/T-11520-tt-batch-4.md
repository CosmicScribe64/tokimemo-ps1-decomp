---
id: T-11520
title: TT overlay batch 4: Opus near misses, Sonnet drafts of the large functions
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11510-tt-batch-3]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]"]
---

## Goal
Fourth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11510 (PR #5). In order:
1. Opus on the closest guarded near misses: `func_80140624` (~14 lines), `func_8013E850` (~14), `func_801392A0` (23),
   `func_8013DE10` (~29), `func_80134BBC` (phantom 0x40).
2. Sonnet over the 13 large untouched TT functions (1,180-2,200 bytes): draft each, match where possible, otherwise
   guard the close ones so a later Opus pass can close them.

## Acceptance criteria

- [ ] Opus pass done; each near miss matched or its guard comment/row updated
- [ ] Sonnet pass done; matches one per commit, near misses guarded with `T-11520`, T-0018 rows
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
