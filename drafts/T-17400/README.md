# T-17400: EVENT redraft with the recovered types

Fresh drafts of the 38 EVENT functions that were unmatched after the types-first pass of T-17400 (wiki page
`event-family-results`, section "Types-first pass (T-17400)"). `func_800FF320` and `func_80104C3C` were matched
before the redraft as ports of their DATE clones, so they are not here.

- `redraft.py`: the driver, a copy of `drafts/T-17200/redraft.py` adapted to EVENT (run it in Docker from the repo
  root; its docstring lists the rewrites). EVENT's display records are `D_800E9620` and its `Rec24` records
  `D_800EC190`; both are `u8[]` in `main_api.h`, so m2c sees them typed and the draft reads them through
  `((DisplayActor44 *)D_800E9620)` and `((Rec24 *)D_800EC190)`.
- `inventory.json`, `environment.json`: the functions and the header hashes the run used.
- `_runs/plain/EVENT/<func>/`: m2c with the new headers through the T-14500 census path. Only the headers differ
  from the census.
- `_runs/typed/EVENT/<func>/`: m2c on folded asm plus the type-driven rewrites (`rewrites` in result.json). The
  full-unit `context.c` files of both runs are pruned.
- `EVENT/<func>/`: the better of the two (`body.c`, the full unit `context.c`, `declarations.h`, and `result.json`
  with `census_score`, `plain_score`, `typed_score`, `score`, `vs_census`; `status: matched` with `matched_in`
  for the functions now in `src/`). `catalog.jsonl` has one line per function.
- `hand.c` near misses (rows are `tools/funcdiff.py --resolve` rows; `hand` in result.json says what is left):
  `func_800FD00C` (6), `func_8010F340` (11), `func_801022C0` (14).

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. The census scored the hand-written guarded body of the 4 guarded
functions, so a fresh m2c draft of those scores worse; compare the other 34.

Results (34 unguarded functions): 32 drafts compile (`func_8010F74C` and `func_8011B848` do not); against the
census 9 better (7,560 bytes), 21 the same, 2 worse (`func_800FD00C` and `func_800FEAE8`, whose census drafts
were repaired by hand); sum 11,600 to 11,395 diff rows. The typed run beat the plain one in 9 of 30 functions that
compiled both ways and lost in 7; it also compiled 5 drafts whose plain run did not (the step tables are typed in
the header now, so m2c's word copies no longer compile). `tools/draft_fixes.py` changed no score. No draft scored
0; `func_800FF61C` (2 rows) matched with a hand edit, `func_8010AE7C` as a port of its GEKO sibling.
