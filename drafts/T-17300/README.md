# T-17300: ETC redraft with the recovered types

Fresh drafts of the 43 ETC functions that were unmatched at `w2-good`, made after the types-first pass of
T-17300 (wiki page `etc-family-results`, section "Types-first pass (T-17300)"). The driver is a copy of
`drafts/T-17200/redraft.py` (which adapts `drafts/T-16560/redraft.py` and `drafts/T-16550/redraft.py`) with
ETC rewrites added: work-prim, display-base, work-view, the Rec24 fold, and lui pairing in the fold.

- `redraft.py`: the driver (run it in Docker from the repo root; its docstring lists the rewrites).
- `inventory.json`, `environment.json`: the functions and the header hashes at the start of the run.
- `_runs/plain/ETC/<func>/`: m2c with the new headers through the T-14500 census path. Only the headers
  differ from the census (the five pass-2 functions were rerun after the second header change).
- `_runs/typed/ETC/<func>/`: m2c on folded asm (lui pairing) plus the type-driven rewrites (`rewrites` in
  result.json).
- `_runs/typed1/ETC/<func>/`: the same with the T-17200 fold (bare-base `%hi`) and no stack-array typing.
  The full-unit `context.c` files of the three runs are pruned.
- `ETC/<func>/`: the best of the three after `tools/draft_fixes.py` (`body.c`, the full unit `context.c`,
  `declarations.h`, and `result.json` with `census_score`, `plain_score`, `typed_score`, `typed1_score`,
  `score`, `vs_census`). `catalog.jsonl` has one line per function. `hand.c` where a hand edit got closer.
