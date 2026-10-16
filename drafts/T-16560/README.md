# T-16560: TACO redraft with the recovered types

Fresh drafts of the 126 TACO functions that were unmatched at `w2-good`, made after the types-first pass
of T-16560 (wiki page `taco-family-results`, section "Types-first pass"). None scored 0; three became
matches with a small hand edit and are in `src/` (`status: matched` in their result.json):
`func_8013822C`, `func_8013555C`, `func_80135638`.

- `redraft.py`: the driver (run it in Docker from the repo root; its docstring lists the rewrites).
- `inventory.json`, `environment.json`: the functions and the header hashes the run used.
- `_runs/plain/TACO/<func>/`: m2c with the new headers through the T-14500 census path (bulk cleanup,
  full-unit compile with the cheap declaration fixes). Only the headers differ from the census.
- `_runs/typed/TACO/<func>/`: the same draft after the type-driven rewrites (`rewrites` in result.json).
- `TACO/<func>/`: the better of the two after `tools/draft_fixes.py` (`body.c`, the full unit `context.c`,
  `declarations.h`, and `result.json` with `census_score`, `plain_score`, `typed_score`, `score`,
  `vs_census`). `catalog.jsonl` has one line per function.
- `TACO/func_80147488/sibling.c`: a hand copy of the matched clone `func_8015B3B0`, 10 rows from a match.
- `TACO/func_80133870/hand.c`: the redraft with the post-decrement in the call argument, 18 rows.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the same measure as
`drafts/T-14500/best-quality-catalog.jsonl`. The census used the hand-written guarded body for the
10 guarded functions, so a fresh m2c draft of those scores worse; compare the other 116.

Results (116 unguarded functions): 103 drafts compile (census: 116, 13 of them only after manual
repair); against the census score 48 better, 53 the same, 2 worse; median 166 to 150 diff rows over the
103 that compile, sum 21,752 to 19,983. The headers alone (plain drafts) changed 7 of 98 scores (3 better,
4 worse) and left 18 drafts that do not compile; the rewrites compile 5 of those and give the rest of the gain.
