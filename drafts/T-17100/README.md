# T-17100: RPG_BAT redraft with the recovered types

Fresh drafts of the 74 RPG_BAT functions that were unmatched at `w2-good`, made after the types-first pass
of T-17100 (wiki page `rpg-bat-family-results`, section "Types-first pass (T-17100)").

- `redraft.py`: the driver (run it in Docker from the repo root; its docstring lists the rewrites). It is
  `drafts/T-16560/redraft.py` (runner, `tools/draft_fixes.py` step, actor-offset, pointer-step,
  field-symbols) with the asm fold and the display-record view of `drafts/T-16550/redraft.py`, a stack-table
  rewrite for the RPG_BAT tables, a record-offset rewrite for `DisplayActor44` pointers, and a third variant.
- `inventory.json`, `environment.json`: the functions and the header hashes the run used.
- `_runs/<variant>/RPG_BAT/<func>/`: one census-path run per variant:
  - `plain`: m2c with the new headers, nothing else;
  - `typed`: m2c on the folded asm, then the type-driven rewrites (`rewrites` in result.json);
  - `census`: the T-14500 census body (or a better T-16510 one) with the same rewrites, so a census
    draft that needed a manual repair still gets the types.
- `RPG_BAT/<func>/`: the best of the three after `tools/draft_fixes.py` (`body.c`, the full unit
  `context.c`, `declarations.h`, and `result.json` with `census_score`, `plain_score`, `typed_score`,
  `census_typed_score`, `score`, `vs_census`). `catalog.jsonl` has one line per function.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the same measure as
`drafts/T-14500/best-quality-catalog.jsonl`. A score of 0 is a candidate match, not a verified one.

## Reproduce

```
tools/docker.sh python3 drafts/T-17100/redraft.py --jobs 2 --timeout 180
tools/docker.sh python3 drafts/T-17100/redraft.py --finish-only        # rebuild RPG_BAT/ from _runs
tools/docker.sh python3 drafts/T-17100/redraft.py --rewrite BODY.c --scope RPG_BAT:func_X   # one rewrite
```

The scores hold for the headers named in `environment.json`.
