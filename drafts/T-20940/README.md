# T-20940: targeted type pass for src/main/80075320.c

Redrafts of the 15 functions of `src/main/80075320.c` that were unmatched at `w2-good` (d9794660), after the
type changes of T-20940 (wiki pages `data-types` and `main-family-results`, sections on T-20940).

- `main/<func>/`: `result.json` (`scope`, `score`, `census_score`, `before_score`, variant, provenance), and for
  the functions the tool redrafted: `body.c`, the full unit `context.c`, `declarations.h`. `hand.c` is the hand
  draft written with the new types; `tools/redraft.py` scored it as its `archived` variant (with `gs-local`).
  Four drafts became matches and are in `src/` (`status: matched`, `score` 0, `draft_score` the tool's last
  score): `func_80077694`, `telephone_class_init`, `holiday_tel_exit`, `func_80077900` (2,568 bytes).
- `catalog.jsonl`: one line per function, `before` and `after` best scores.
- `runs.txt`: the summary lines of the three runs: before (d9794660), after with the header types only, after
  with the hand drafts as archives.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py` (same measure as the census); 0 is a
candidate, not a verified match (the four matches were verified with the full build, 27/27 sha1).

Runs: `tools/redraft.py --ticket T-20940 --jobs 3 <15 functions>` from snapshots of the tree under `build/ty/`
(`base`, `after`). The hand drafts sit in this ticket's folder, which the tool excludes from its own archive, so
the second after-run used `--ticket T-20940h --work build/redraft/T-20940 --out drafts/T-20940 --force` with a
`hand_score` (funcdiff rows of the hand draft) in each `result.json`; the provenance was renamed back to T-20940.
