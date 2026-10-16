# T-18800: DATE2 redraft with the recovered types

Fresh drafts of the 10 DATE2 functions that were unmatched at `w2-good`, made after the types-first pass of
T-18800 (wiki page `scenes-family-results`, section "Types-first pass (T-18800)"). `func_80135E50` was matched
during the pass (`status: matched` in its result.json, source in `src/ovl/DATE2/80132DE0.c`).

- `redraft.py`: the driver, a copy of `drafts/T-18500/redraft.py` adapted to DATE2 (run it in Docker from the
  repo root; its docstring lists the rewrites). Changes against T-18500: the fold also covers the `Rec24` table
  `D_801217D0`, and three new rewrites: `rect-view` (the labels inside `D_800B5920` as fields of two `RECT`s),
  `array-views` (m2c's and the census's views of a scalar header array such as `s16 D_8013A5E4[3]`: `(&D_X)[i]`,
  a byte offset on `&D_X`, `D_X->unk0`, the bare name read as a scalar), and `stack-elements` for the rows of a
  copied two-dimensional byte table (`&sp65` inside `Date2Labels4x21 sp50` is `sp50.v[1]`).
- `hand.py`: scores every `DATE2/<func>/hand.c` through the same full-unit path and keeps the better of hand
  draft and redraft (`hand_score`).
- `inventory.json`, `environment.json`: the functions and the header hashes the run used.
- `_runs/plain/DATE2/<func>/`, `_runs/typed/DATE2/<func>/`, `_runs/census/DATE2/<func>/`: the three runs (m2c
  with the new headers; m2c on folded asm plus the rewrites; the census body plus the rewrites). The full-unit
  `context.c` files are pruned.
- `DATE2/<func>/`: the best starting point (`body.c`, the full unit `context.c`, `declarations.h`, and
  `result.json` with `scope`, `score`, `census_score`, `census_path`, `plain_score`, `typed_score`,
  `vs_census`, provenance). `catalog.jsonl` has one line per function.
- `DATE2/func_80135E50/hand.c`: the port of the DATE twin `func_801489D0`, 25 rows before the FAKE empty test
  that matched it.
- `DATE2/func_80136794/hand.c`: the port of the matched DATE2 sibling `func_801366E8`, 38 rows: the original
  loads both call arguments through `a1` (`lui a1; lbu a0,(a1)`) and keeps `unk_71E` in `v0`.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. `func_801395C8` has no compiling fresh draft (IDO stops without an error
message; `diagnostics.txt` shows only warnings about m2c's `&sp60` arithmetic over the 81 x 31 byte buffer);
its best starting point is the repaired census body.
