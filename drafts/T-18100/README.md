# T-18100: BUNKA_SD redraft with the recovered types

Fresh drafts of the 15 BUNKA_SD functions still unmatched after the first hand matches of the
types-first pass (T-18100, wiki page `bunka-family-results`, section "Types-first pass (T-18100)"),
plus a folder for each of the 5 functions matched by hand before the run.

- `redraft.py`: the driver, copied from `drafts/T-17800/redraft.py` (the T-16550 and T-16560 drivers
  joined) with two new rewrites: `pointer-index` (`D_8013C730[2]` for a header `s32 *` global that m2c
  prints with byte offsets) and `record-walk` (a `DisplayActor44 *` that walks the display records:
  the step moves to the end of the loop body and the fields print as `p[96].unk_01`). Run it in Docker
  from the repo root; its docstring lists every rewrite.
- `inventory.json`, `environment.json`: the functions and the header hashes of the run.
- `_runs/{plain,typed,census}/BUNKA_SD/<func>/`: the three drafts of each function, as in T-17800.
- `BUNKA_SD/<func>/`: `body.c` (the best draft), `context.c` (the full unit it was scored in) and
  `result.json` (`score`, `census_score`, `census_path`, `vs_census`, `variant`, provenance).
  Where a hand draft beat the driver, `body.c` is the hand draft, the driver's best is
  `driver_body.c`/`driver_context.c`, and `result.json` has `driver_score` and `hand_note`.
  Functions matched since have `status: matched` and the matched C in `matched.c` (or `body.c` for the
  five matched before the run).
- `catalog.jsonl`: one line per function.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py` (0 is a candidate match),
the measure of `drafts/T-14500/best-quality-catalog.jsonl`. Hand drafts were scored the same way
(`draft_fixes.make_scorer` on the current source file with the body in place).
