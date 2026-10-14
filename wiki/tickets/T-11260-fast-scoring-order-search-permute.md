---
id: T-11260
title: Fast scoring for order_search, permute and the permuter farm
status: In Review   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee: tooling agent g43
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11250-faster-candidate-scoring]]", "[[tickets/T-11530-order-search-tool]]"]
---

## Goal

Bring the T-11250 speedup (`tools/fastscore.py`: cached strip, thread pool) to the other solvers that score candidates, with identical scores.

## Acceptance criteria

- [x] `tools/order_search.py`, `tools/permute.py`, `tools/permuter_farm.py`, other `strip_functions`/`cc.py` users looked at; what can and cannot use fastscore is listed below.
- [x] Equality measured against the exact path on real drafts (jump table, rodata file). Result: the single-function compile does NOT give the same scores as order_search's full-unit compile, so it is opt-in (`--fast`), the default path is unchanged.
- [x] Tests: `tools/test_order_search.py` (rescore logic); test_permute, test_fastscore, test_autotweak, test_bulk_m2c, test_check_headers, `check_headers.py include src` pass.
- [ ] Code review against `CODING_STANDARDS.md`.

## Notes

### Why order_search cannot be exact and fast

order_search scores a variant by compiling the whole translation unit (gcc -E, cc.py, trailing pad, object trim) and comparing funcdiff rows with relocations resolved against that object. fastscore's stripped single-function unit leaves out the other functions, so calls to sibling functions relocate against a different symbol (`jal <func_800674B0>` vs `<func_80067870>`), the nops after the function differ, and resolved words differ with the rodata/jump-table layout. Measured on 22 states of two drafts: 0 of 22 scores equal.

| draft | exact | stripped | notes |
| --- | --- | --- | --- |
| main:func_80067870 (switch), 3 lines marked, 6 states | 304,306,304,306,310,318 | 326,334,298,298,308,310 | order differs; exact best at stripped rank 4 |
| main:LoadSquare (file with INCLUDE_RODATA), 4 lines marked, 16 states | 0,4,0,0,4,4,8,12,... | 3,5,3,3,5,5,11,13,... (resolved), +5 (plain) | constant offset, same order |

Candidates/s (8 threads, warm cache): func_80067870 4.0 exact, 13.2 stripped; LoadSquare 5.5 exact, 11.2 stripped (about 2-3x, not the 25x of autotweak: the exact path already runs -j compiles in parallel and its compile is only about 0.65 s of 1 s).

### What changed

- `tools/order_search.py --fast`: search with the stripped single-function compile (`compile_variant_fast`, `fastscore.BaseBuilder`), then score the best `--top` (at least 5) variants and the written order with the exact full-unit compile (`rescore_exact`) and choose among those exact scores. The write-back guarantees are unchanged (the written order is always rescored, so the result is never worse). Default stays the exact path. End to end on LoadSquare (24 variants) it is not faster: the first strip costs about 1 s, so `--fast` only pays on searches of a few hundred variants or more.
- `tools/permute.py`, `tools/permuter_farm.py`: `build_base` runs once per job at setup and the decomp-permuter does all candidate compiles itself (its own compile.sh calls), so there is no scoring loop to speed up. Left alone.
- Also left alone: `autotweak.py`, `bulk_m2c.py` (T-11250 done), `typefix.py` (T-11240, other agent), `funcdiff.py`, `funcdiff_verify.py`, `cvt_pass.py`, `frame_pass.py`, `ido_eval.py`, `uopt_eval.py`, `gcc_fingerprint.py` (single compiles or fingerprint runs, no candidate loop).

## Comments
