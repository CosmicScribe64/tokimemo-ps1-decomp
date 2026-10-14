---
id: T-11530
title: Tooling: order_search.py, exhaustive statement-order and operand-order search
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[decompile-workflow]]", "[[matching-notes]]", "tools/order_search.py", "tools/funcdiff.py", "tools/permute.py"]
---

## Goal
In the TT batches (T-11000..T-11520) agents repeatedly searched by hand for the right order of independent
statements (packet stores, RECT fields, increments) and the right operand order of sums, because IDO's scheduling
and ugen temp numbering depend on them and decomp-permuter samples them poorly. Add a tool that enumerates these
orders systematically on a marked draft and scores each variant with funcdiff's comparison.

## Acceptance criteria

- [ ] `tools/order_search.py`: markers `/* ORDER */ ... /* END ORDER */` (permute the lines between) and
      `ORDER_SWAP(A, op, B)` (both operand orders); exhaustive search when small, hill-climb otherwise; parallel compiles
      through the same preprocess + `tools/cc.py` path as funcdiff's guarded compile; reports the best variants;
      `--apply` writes the best one back without markers
- [ ] Synthetic tests (no compiler, no game data)
- [ ] Tried on a real TT draft
- [ ] Documented in [[decompile-workflow]]; inline review against `CODING_STANDARDS.md`

## Results

## Review

## Comments
