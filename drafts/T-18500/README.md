# T-18500: BUNKAKEN redraft with the recovered types

Fresh drafts of the 12 BUNKAKEN functions that were unmatched at `w2-good`, made after the types-first
pass of T-18500 (wiki page `bunka-family-results`, section "Types-first pass (T-18500)").

- `redraft.py`: the driver, copied from `drafts/T-18100/redraft.py` (itself the T-16550 and T-16560
  drivers joined) with three rewrites from `drafts/T-18300/redraft.py` (stack-elements,
  gamestate-records, struct-bytes) and three new ones: header-arrays (census bodies that index the new
  header arrays by hand), array-symbols (a splat label inside `s32 D_801562A0[3][13]` is that element)
  and gs-local (the best body is also scored with every GameState access through
  `GameState *g = &D_800E6280;`, the BUNKA family form of T-12701). Run it in Docker from the repo
  root; its docstring lists every rewrite.
- `hand.py`: scores hand drafts (`BUNKAKEN/<func>/hand.c`) through the same full-unit path and keeps
  the better of hand draft and redraft; `--try FUNC FILE` prints one score.
- `inventory.json`, `environment.json`: the functions and the header hashes of the run.
- `_runs/{plain,typed,census}/BUNKAKEN/<func>/`: the three drafts of each function, as in T-18100.
- `BUNKAKEN/<func>/`: `body.c` (the best draft), `context.c` (the full unit it was scored in) and
  `result.json` (`score`, `census_score`, `census_path`, `vs_census`, `variant`, provenance).
  Functions matched since have `status: matched`; the matched C is in `src/`.
- `catalog.jsonl`: one line per function.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py` (0 is a candidate match),
the measure of `drafts/T-14500/best-quality-catalog.jsonl`.
