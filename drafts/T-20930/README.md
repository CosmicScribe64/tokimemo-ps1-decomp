# T-20930: ENDING targeted type pass

Drafts of the 13 ENDING functions that were unmatched at the start of T-20930, after the type changes of the
ticket (wiki page `ending-family-results`, section "Targeted type pass").

- `ENDING/<func>/result.json`: `scope`, `score` (diff rows of the function in the full unit, the census measure
  of `drafts/T-14500`), `census_score`, `variant` (the redraft variant, or `hand`), `hand_score`, provenance.
  Three functions are matched in `src/` (`status: matched`, `score: 0`, the body in `matched.c`):
  `func_80132420`, `func_80132B04`, `func_80139B48`.
- `ENDING/<func>/body.c`, `context.c`, `declarations.h`: the best draft and the full unit that was scored.
- `ENDING/<func>/hand.c`: hand drafts. New in this ticket: `func_801330E4` (114), `func_80134C2C` (40),
  `func_8013759C` (66, the `D_8011ECD0` record view). `func_80137BB4`, `func_80137E40` and `func_801387B0` hold
  the T-18300 hand drafts with the `(u8)` casts of `D_8013C364` removed, rescored (58, 64, 156).
- `catalog.jsonl`, `inventory.json`, `environment.json`: the `tools/redraft.py` run on b035184a (after the type
  changes; `func_80132B04` and `func_80139B48` were already matched).
- `fold_ending.py`: the one-off rewrite of the scalar labels into the new aggregates (run once, kept as a record).
- `hand.py`: scores the `hand.c` files with the census measure and keeps the better draft in `result.json`.

## Reproduce

Run in Docker from the repo root (`tools/docker.sh`):

```
python3 tools/redraft.py --ticket T-20930 --unit ENDING --jobs 2
python3 drafts/T-20930/hand.py
```

The run before the type changes (172ab7ce) was made the same way in a copy of the tree; its scores are in the
wiki table.
