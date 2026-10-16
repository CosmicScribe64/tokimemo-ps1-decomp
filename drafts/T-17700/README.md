# T-17700: GEKO redraft with the recovered types

Fresh drafts of the 35 GEKO functions that were unmatched after the types-first pass of T-17700 (wiki page
`scenes-family-results`, section "Types-first pass (T-17700)"). `func_8013E22C` was matched before the redraft
as a port of its EVENT clone `func_8010564C`, so it is not here.

- `redraft.py`: the driver, a copy of `drafts/T-17400/redraft.py` adapted to GEKO (run it in Docker from the repo
  root; its docstring lists the rewrites). GEKO's display records are `D_8011ECD0` (`u8[]` in `main_api.h`, read
  through a `DisplayActor44` view) and its `Rec24` records `D_801217D0` (already `Rec24[]`).
  Changes against T-17400: the fold runs after `tools/m2c.py`'s `fix_shared_hi` and keeps the offset of a
  `%hi` (folding first made m2c print `D_800E6280.unk0` for every GameState byte that shares a `%hi`, which
  hit 11 typed drafts of the T-17200/T-17400 runs too); new rewrites `stack-elements` (a stack local m2c
  names inside a copied table, a byte step from an indexed element, or the table's name read as a value, are
  elements of the table) and `gamestate-records` (`*(&D_800E6280.unk_1BC[0].F + (i * 0x38))` is
  `D_800E6280.unk_1BC[i].F`).
- `hand.py`: scores the guarded `NON_MATCHING` bodies in `src` through the same full-unit path and keeps the
  better of hand body and redraft (`variant: hand-guarded`, `hand_guarded_score`).
- `inventory.json`, `environment.json`: the functions and the header hashes the run used. The rows of the five
  functions guarded during the pass were refreshed (`guarded: true`) before their rerun.
- `_runs/plain/GEKO/<func>/`, `_runs/typed/GEKO/<func>/`: the two runs (m2c with the new headers; m2c on
  folded asm plus the rewrites, `rewrites` in result.json). The full-unit `context.c` files are pruned.
- `GEKO/<func>/`: the best starting point (`body.c`, the full unit `context.c`, `declarations.h`, and
  `result.json` with `scope`, `score`, `census_score`, `census_path`, `plain_score`, `typed_score`,
  `vs_census`, provenance). `catalog.jsonl` has one line per function.
- `GEKO/func_80133024/hand.c`: a hand edit of the 46-row typed draft, 40 funcdiff rows (frame offset of the
  month table and the loop registers).

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. `func_80142C9C` and `func_80143F48` have no compiling draft (m2c
context errors in both runs); their `score` is the census score (`score_source: census`).

Results (29 unguarded functions): 27 drafts compile; against the census 14 better (8,804 bytes), 11 the same,
4 worse (`func_801358CC`, `func_80135DC0`, `func_80138E10`, `func_80139E1C`, whose census drafts come from the
T-16510 defect rewrites or a manual repair); sum 6,067 to 5,536 diff rows, median 138 to 132. The typed run
beat the plain one in 11 of 25 functions that compiled both ways and lost in 5; it also compiled 2 drafts
whose plain run did not. The gains come from the stack tables: `func_80144270` 491 to 200, `func_801406CC`
283 to 219, `func_80132B24` 211 to 160, `func_80133024` 94 to 46, `func_80144198` 97 to 69.
`tools/draft_fixes.py` changed no score. No draft scored 0.
