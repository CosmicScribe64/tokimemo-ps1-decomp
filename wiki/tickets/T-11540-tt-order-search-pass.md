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
- `func_8013DE10`: **matched**. Marked the u/v/clut sums and the two `flags[r[0]]` address additions as (nested) `ORDER_SWAP`s
  inside an ORDER block: 192 variants, score 0 after 48 (about 4 s of compiles). The match needs both outer sums
  reversed (shift term first) and the flags address written `*(r[0] + flags)`; T-11520 had tried only the outer sums.
  `funcdiff --resolve`: MATCH (984 bytes); 27/27.
- `func_8013E850`: no gain. 96 variants (the `||` operands, the two `|=` lines, the last tail stores with the `e` load),
  then a 198-compile hill-climb over all 15 tail statements: the written order stays best (6 rows). The remaining
  delay-slot difference comes from the shape of the condition, not from an order the tool expresses.
- Tool fixes made during this pass (pushed to PR #11): ties keep the written order; nested `ORDER_SWAP`.

## Review

## Comments
