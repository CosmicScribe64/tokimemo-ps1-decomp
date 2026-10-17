# T-20910: OPTION redraft after the second type pass

Fresh drafts of the 32 OPTION functions that were unmatched at the start of T-20910 (wiki page
`option-family-results`, section "Second type pass (T-20910)"). One of them, `func_80135E48`, became a
match and is in `src/ovl/OPTION/80132000.c`.

- `redraft.py`: the driver. It is `tools/redraft.py` (T-20800) with two additions, set up again in every
  worker: the object view `SpriteSlot24` for main's BG slot 0 at `D_801220D0` (as ETC's `D_801230D0`
  view), and a variant `hand` that scores `OPTION/<func>/hand.c` as it is. Its docstring has the details.
- `OPTION/<func>/`: the best draft of each function, as `tools/redraft.py` writes it: `body.c`, the full
  unit `context.c`, `declarations.h`, and `result.json` with `scope`, `score`, `census_score`, the
  variant scores and provenance. `catalog.jsonl` has one line per function.
- `OPTION/<func>/hand.c`: hand drafts in the shape of the matched siblings (the menu-page template,
  the pair-table loop of `func_80134210`, the BG map of `func_801328C0`). `variant_scores.hand` in
  result.json is their score as written; `gs-local` on top of the best variant is often lower still.
- `baseline.jsonl`: the same driver without the T-20910 changes (tools/redraft.py, default variants,
  tree `26d45f25`): the left side of the paired scores.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. A score of 0 is a candidate match, not a verified one.

## Reproduce

Each run worked in its own copy of the tree under `build/ty/` (the runs compile against `src/`):

```
tools/docker.sh python3 tools/redraft.py --ticket T-20910 --unit OPTION --out OUT --work WORK   # baseline
tools/docker.sh python3 drafts/T-20910/redraft.py --ticket T-20910 --unit OPTION --jobs 2 --timeout 200
```
