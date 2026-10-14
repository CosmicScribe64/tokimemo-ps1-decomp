---
id: T-11270
title: Exact rescoring in autotweak and bulk_m2c
status: In Review   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee: tooling agent
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11250-faster-candidate-scoring]]", "[[tickets/T-11260-fast-scoring-order-search-permute]]", "[[decompile-workflow]]"]
---

## Goal

autotweak and bulk_m2c rank candidates with the stripped single-function compile. T-11260 measured that its scores differ from the full translation-unit compile of the real build (0 of 22 states equal: sibling-call relocation names, trailing nops, rodata layout). A candidate that truly matches can score above 0 stripped and never reach the verify step. Rescore the promising candidates with the full-unit compile, as `order_search --fast` does.

## Acceptance criteria

- [x] A full-unit scorer for one draft, reusing order_search's compile and funcdiff score (no copy).
- [x] autotweak's climb: after each step, the best `--exact-top` (3) candidates by stripped score plus every candidate at or under `--exact-threshold` (40) get the full-unit score; an exact 0 goes to the existing verify; the step's accept decision uses exact scores; exact scores are cached by text hash.
- [x] bulk_m2c: the same for the start-text and variant pick, and through autotweak for the climb.
- [x] Flags `--exact-top K`, `--exact-threshold S`, `--no-exact-rescore` on both tools; the per-function line says when the result came from an exact rescore.
- [x] Tests with mocked stripped and exact scorers that disagree: hidden match found, acceptance by exact score, cache, flags. test_autotweak, test_bulk_m2c, test_fastscore, test_order_search, test_typefix, check_headers, test_check_headers pass.
- [ ] Measured on the real build by the orchestrator: one draft before/after, candidates/s with and without `--no-exact-rescore`.
- [ ] Code review against `CODING_STANDARDS.md`.

## Notes

### What changed

- `tools/order_search.py`: `make_text_scorer` (scores one C text of the whole file, full-unit compile or `--fast`), split out of `make_scorer`; order_search's behaviour is unchanged.
- `tools/autotweak.py`: `BuildScorer.exact_rescorer()` builds that scorer for a draft (guard replaced by the candidate, the other drafts stay asm, `NON_MATCHING` not defined, relocations resolved) and wraps it in `ExactRescorer` (cache by sha1 of the text, parallel on the shared fastscore workers). `climb(..., rescore=, exact_top=, exact_threshold=)`: after the single edits, the pairs and the order stage (`order_stage(every=True)` returns all its variants), `pick_rescore` takes the top K by stripped score plus every one at or under the threshold; an exact 0 goes to verify (verify results are cached per climb); the step is the lowest exact score that beats the current best's exact score. A verified stripped 0 is still a match. Without `rescore` the climb is unchanged. `Result` gains `exact_base`, `exact_best`, `via_exact`, `rescored`.
- `tools/bulk_m2c.py`: `pick_exact` over the start texts of all variants; an exact 0 ends the sweep and is verified in `_solve`; the climb gets the candidate's rescorer. `--keep-score` and the recorded score stay stripped; `state.jsonl` gets `exact`.
- Output: autotweak `... N tried  exact A -> B (R rescored)` and `MATCH ... (found by exact rescore)`; bulk_m2c `near UNIT:f 0x40 12 exact 5`, `MATCH UNIT:f 0x40 (found by exact rescore)`.

### Open: cost

Each rescored text is one full-unit compile (about 1 s, T-11260). Near a match many single edits can score at most 40 stripped, so a step may rescore tens of texts; `--exact-threshold` lowers that. To be measured on the real build (PR body has the commands).

## Comments
