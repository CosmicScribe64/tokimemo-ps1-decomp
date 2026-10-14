---
id: T-11540
title: TT: order_search pass on the scheduling and temp-numbering near misses
status: Done
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

- [x] `func_8013E850` and `func_8013DE10` searched; matches committed, results recorded
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
- `func_8013DE10`: **matched**. Marked the u/v/clut sums and the two `flags[r[0]]` address additions as (nested) `ORDER_SWAP`s
  inside an ORDER block: 192 variants, score 0 after 48 (about 4 s of compiles). The match needs both outer sums
  reversed (shift term first) and the flags address written `*(r[0] + flags)`; T-11520 had tried only the outer sums.
  `funcdiff --resolve`: MATCH (984 bytes); 27/27.
- `func_8013E850`: no gain. 96 variants (the `||` operands, the two `|=` lines, the last tail stores with the `e` load),
  then a 198-compile hill-climb over all 15 tail statements: the written order stays best (6 rows). The remaining
  delay-slot difference comes from the shape of the condition, not from an order the tool expresses.
- Tool fixes made during this pass (pushed to PR #11): ties keep the written order; nested `ORDER_SWAP`.
- Sonnet agent pass over the other guarded drafts (about 25 minutes, 173k tokens; tier 1 complete, tier 2 mostly
  skipped as register allocation): no match; improved `func_8013C1FC` (36 -> 4 diff lines), `func_80140920` (84 -> 50),
  `func_801330D8` (45 -> 34), `func_8013B4A0` (22 -> 20 rows), `func_8013E498` (101 -> 99 rows); no gain on ten others
  (listed in [[matching-notes]] "TT overlay, order_search pass (T-11540)").
- Cleanup: removed the stale T-9380 NON_MATCHING draft of `func_8014CC04`; its typed version has matched since T-9940.
- TT 206/303 (44.7%); one match in this ticket.
- Tool feedback for later: warn when an ORDER block contains a call, and say in the output that the score counts
  resolved diff rows (- and +), which is larger than funcdiff's `-` row count.

## Review
Inline, against CODING_STANDARDS.md: the match is plain C with a comment on the operand order; guard comments of the
improved drafts carry T-11540 and the lines left; no new fakematches; the `func_8014CC04` cleanup changes no bytes
(27/27, funcdiff MATCH). No findings.

## Comments
