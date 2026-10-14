---
id: T-11530
title: Tooling: order_search.py, exhaustive statement-order and operand-order search
status: Done
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

- [x] `tools/order_search.py`: markers `/* ORDER */ ... /* END ORDER */` (permute the lines between) and
      `ORDER_SWAP(A, op, B)` (both operand orders); exhaustive search when small, hill-climb otherwise; parallel compiles
      through the same preprocess + `tools/cc.py` path as funcdiff; reports the best variants; writes the best one back
      without markers
- [x] Synthetic tests (no compiler, no game data)
- [x] Tried on a real TT draft
- [x] Documented in [[decompile-workflow]]; inline review against `CODING_STANDARDS.md`

## Results
- `tools/order_search.py`, tests `tools/test_order_search.py` (10 tests: marker parsing and errors, rendering,
  enumeration, neighbours, exhaustive search stopping at 0, hill-climb reaching a target, budget).
- Validation on real TT code:
  - `func_80147150` (matched in T-11500 after 12 hand-tried orders): its four packet stores, scrambled back to the
    pre-match order x, h, y, w, gave 24 variants compiled in about 2 s (8 jobs); the written order scored 21, exactly one
    variant scored 0, and it is the matched source byte for byte.
  - `func_80139770` (T-11510): the sum `*(s16 *)(c + 0x14) + D_80152D9C[i]` reversed in an `ORDER_SWAP` was flipped back
    (score 4 -> 0).
- Fixes found during validation:
  - `concurrent.futures` fails here because `tools/queue.py` shadows the standard `queue` module on the tools path;
    the search uses plain threads.
  - Compiling every variant with NON_MATCHING turned the other guarded drafts of the file into C and shifted the
    rodata offsets the function refers to; NON_MATCHING is now defined only when the function itself is guarded,
    as in funcdiff.
  - Unresolved comparison has a noise floor (symbol+offset naming; funcdiff without `--resolve` shows the same rows
    on a matched function), so `--resolve` is the default and `--no-resolve` the opt-out.
- A stale `asm/` after a branch switch makes every variant fail to compile (as for funcdiff); the tool still restores
  the file without markers. Documented.

## Review
Inline, against CODING_STANDARDS.md: Python 3, run through `tools/docker.sh`, no host installs; synthetic tests only;
no game data; the tool never leaves markers in the tree (write-back in `finally`). No findings.

## Comments
