---
id: T-11540
title: TT: order_search pass on the scheduling and temp-numbering near misses
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11530-order-search-tool]]", "[[tickets/T-11520-tt-batch-4]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Use `tools/order_search.py` (T-11530, PR #11) on the guarded TT near misses whose remaining difference is scheduling
or ugen temp numbering: first `func_8013E850` (2 lines) and `func_8013DE10` (7 lines), then other candidates.
Stacked on T-11520 (PR #8); the tool is used from an untracked copy, its changes go to PR #11.

## Acceptance criteria

- [ ] `func_8013E850` and `func_8013DE10` searched; matches committed, results recorded
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
