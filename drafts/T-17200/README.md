# T-17200: DATE redraft with the recovered types

Fresh drafts of the 49 DATE functions that were unmatched at `w2-good`, made after the types-first pass of
T-17200 (wiki page `date-family-results`, section "Types-first pass (T-17200)"). The driver adapts
`drafts/T-16560/redraft.py` (plain and typed runs, `tools/draft_fixes.py`, finish) and
`drafts/T-16550/redraft.py` (fold, stack-table copies).

- `redraft.py`: the driver (run it in Docker from the repo root; its docstring lists the rewrites).
- `inventory.json`, `environment.json`: the functions and the header hashes the run used.
- `_runs/plain/DATE/<func>/`: m2c with the new headers through the T-14500 census path. Only the headers
  differ from the census.
- `_runs/typed/DATE/<func>/`: m2c on folded asm plus the type-driven rewrites (`rewrites` in result.json).
  The full-unit `context.c` files of both runs are pruned.
- `DATE/<func>/`: the better of the two (`body.c`, the full unit `context.c`, `declarations.h`, and
  `result.json` with `census_score`, `plain_score`, `typed_score`, `score`, `vs_census`; `status: matched`
  with `matched_in` for the functions now in `src/`). `catalog.jsonl` has one line per function.
- `DATE/func_80155E40/hand.c`: the redraft with two counters above the step table (frame and table offset
  match), register rows left in the compare chain.

Matched in `src/` during the pass: `func_8014AD9C` (port of the TACO clone `func_80147488`), `func_8013B208`
(the EVENT twin `func_80117D80`), `func_80159FAC` and `func_80158338` (typed redrafts written in the
`func_80150ED4` step-table shape), 1,760 bytes.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. The census scored the hand-written guarded body of the 15
guarded functions, so a fresh m2c draft of those scores worse; compare the other 34.

Results (34 unguarded functions): 30 drafts compile; against the census 10 better (9,764 bytes), 16 the same,
4 worse; sum 7,592 to 7,311 diff rows. The typed run beat the plain one in 13 of 35 functions that compiled
both ways and lost in 5; `tools/draft_fixes.py` changed no score. No draft scored 0.
