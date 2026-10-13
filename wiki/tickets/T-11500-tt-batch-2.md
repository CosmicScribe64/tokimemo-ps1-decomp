---
id: T-11500
title: TT overlay batch 2: Opus near misses, Sonnet untried list
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-13
updated: 2026-10-13
links: ["[[tickets/T-11000-tt-overlay]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]"]
---

## Goal
Second TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2; the maintainer confirmed TT is ours). Two parts, in order:
1. One Opus agent on the closest guarded near misses from T-11000: `func_80134470` (1 line), `func_801349B0` (7),
   `func_80134BBC` (7), `func_80147150` (~8), `func_80142350` (11). Tests whether Opus pays off for near misses.
2. One Sonnet list over the unflagged TT functions not on the T-11000 lists.

## Acceptance criteria

- [ ] Opus pass done; each near miss matched or its guard comment/row updated with what was tried
- [ ] Sonnet list done; matches one per commit, near misses guarded with `T-11500`, T-0018 rows
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
