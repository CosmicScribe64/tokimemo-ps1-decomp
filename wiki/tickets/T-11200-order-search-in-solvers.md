---
id: T-11200
title: Statement-order search in the automatic solvers
status: In Review
assignee: agent
created: 2026-10-14
updated: 2026-10-14
links: [[[decompile-workflow]], [[register-cheatsheet]], [[tickets/T-11170-cheat-sheet-auto-tweaker]], [[tickets/T-11190-bulk-m2c-pipeline]], [[tickets/T-11530-order-search-tool]], tools/autotweak.py, tools/bulk_m2c.py, tools/order_search.py]
---

## Goal

Let autotweak (T-11170) and bulk_m2c (T-11190) try statement and operand orders on their own, reusing diman8's
`tools/order_search.py` (T-11530), and fix the climb reporting a match for an unverified score of 0.

## Acceptance criteria

- [x] autotweak edit `order`: swap two adjacent independent simple statements (same indent, conservative token check),
      in single edits and depth-2 pairs.
- [x] Order stage when the climb stalls above 0: detect runs of 2..6 independent statements, mark the best one or two
      (stores first) with `/* ORDER */` blocks plus up to N `ORDER_SWAP` operands, search them in-process with
      `order_search.search` and autotweak's scorer, within order_search's `--max-exhaustive` logic. A 0 is verified.
      Markers never reach a file.
- [x] bulk_m2c uses it through autotweak; `--no-order` turns it off.
- [x] `climb` no longer reports a match for a draft that scores 0 but fails verification.
- [x] Synthetic tests: dependency check, run detection, marker insertion/removal, order_search integration, climb fix.

## Notes

- `tools/autotweak.py`: `Access`/`access`/`independent` (dependency check), `t_order` (edit), `order_runs`,
  `rank_runs`, `pick_order`, `mark_order`, `order_stage`; `climb(order=True, order_opts=...)`; CLI `--no-order`,
  `--max-exhaustive`, `--order-swaps`. `swap_sites` factored out of `t_swap` and shared with the order stage.
- The order stage reuses autotweak's scorer: `order_search.Template` works on the function text and
  `order_search.search` takes any `score(state)`, so no second compile path is needed; jobs=1, because the
  BuildScorer writes one scratch file. Markers only exist in memory.
- Name-level aliasing is accepted (`p->x = 0; v = q->x;` may swap): every candidate still has to match byte for
  byte, so a wrong reorder can only cost a compile.
- Climb fix: an unverified 0 ranks as 1 internally, so only a verified 0 ends the search; `Result.unverified`.
- `tools/bulk_m2c.py --no-order` passes `order=False` to the climb (only then, so custom climbs keep working).
- Cloud environment without game binaries: synthetic tests only. Smoke check over the 162 guarded drafts in src/:
  run detection, marking and identity render work on all of them (127 have a run).


## Comments

- 2026-10-14: implemented on branch `t11200-order-in-solvers`; `test_autotweak.py` (45), `test_bulk_m2c.py` (22),
  `test_order_search.py`, `check_headers.py include src`, `test_check_headers.py` pass. Needs the real-build
  check (commands in the PR) and the code review before Done.
