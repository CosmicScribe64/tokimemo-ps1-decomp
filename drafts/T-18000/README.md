# T-18000: SHUGAKU redraft with the recovered types

Fresh drafts of the 25 SHUGAKU functions that were unmatched at `w2-good` (9,908 bytes, 3 of them guarded),
made after the types-first pass of T-18000 (wiki page `shugaku-family-results`, section "Types-first pass
(T-18000)").

- `redraft.py`: the driver, copied from `drafts/T-17700/redraft.py` (GEKO uses the same main-exe aggregates:
  display records `D_8011ECD0`, Rec24 records `D_801217D0`, GameState). Changes: SHUGAKU paths, `ShFnTblNN`
  callback tables in the `fptab` rewrite, display byte offsets on the bare symbol, and a new `rect-local`
  rewrite (s16 stack locals passed as `(RECT *) &spN` become one `RECT`). Run it in Docker from the repo root;
  its docstring lists the rewrites.
- `hand.py`: scores the `hand.c` drafts through the same full-unit path and keeps the better of hand draft and
  redraft (`hand_score`, `variant: hand`).
- `inventory.json`, `environment.json`: the functions and the header hashes of the run (commit `bc9be3d3`).
- `_runs/plain/SHUGAKU/<func>/`, `_runs/typed/SHUGAKU/<func>/`: the two runs (m2c with the new headers; m2c on
  folded asm plus the rewrites, `rewrites` in result.json). The full-unit `context.c` files are pruned.
- `SHUGAKU/<func>/`: the best starting point: `body.c`, the full unit `context.c`, `declarations.h`, and
  `result.json` with `scope`, `score`, `census_score`, `census_path`, `plain_score`, `typed_score`, `vs_census`
  and provenance. `catalog.jsonl` has one line per function.
- Matched from here (`status: matched` in result.json, 2,292 bytes): `func_8013282C`, `func_801399F4`
  (location-table switches, GYOZI/GEKO sibling template), `func_80135630` (GEKO `func_8013B3C0` port),
  `func_80134B94` (EVENT callback-table idiom), `func_80139054` (guard replaced).
- `hand.c` near misses: `func_80135480` (5 rows: a dead `li v0,-0x81` the original keeps after the two
  masked stores), `func_80133E10` (13 rows: the byte of display record 96 is loaded into `a1` and masked into
  `a0` in the original).

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. Over the 22 unguarded functions: 9 better than the census
(3,368 bytes), 13 the same, none worse; sum 1,905 to 1,734 rows, median 74 to 68. The census scored the
hand-written guard body of the 3 guarded functions.
