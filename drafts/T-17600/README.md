# T-17600: GYOZI redraft with the recovered types

Fresh drafts of the 37 GYOZI functions that were unmatched at `w2-good`, made after the types-first pass of
T-17600 (wiki page `event-family-results`, section "Types-first pass (T-17600)").

- `redraft.py`: the driver, copied from `drafts/T-17300/redraft.py` (T-16550 fold and stack tables, T-16560
  rewrites, T-17300 work-prim and display-base) and adapted to GYOZI: display records `D_80129F40`, Rec24 table
  `D_8012CA40`, `func_80054B60` as the primitive allocator, `GzCallback` tables as stack tables, and only the
  girl records of `GyoziWork D_800F53A0` folded (its scalars keep their `main_api.h` names). Run it in Docker
  from the repo root; its docstring lists the rewrites.
- `inventory.json`, `environment.json`: the functions and the header hashes of the first run. The `plain`
  run used the header of commit `c3fd3056` (before `GYOZI.h` included `game.h`); `typed` and `typed1` were
  rerun with the header and driver of commit `5e91a3e1`.
- `_runs/<variant>/GYOZI/<func>/`: `plain` (m2c with the new headers through the T-14500 census path),
  `typed` (plus the type-driven rewrites), `typed1` (the T-17200 bare-base fold).
- `GYOZI/<func>/`: the best of the three after `tools/draft_fixes.py`: `body.c`, the full unit `context.c`,
  `declarations.h`, `result.json` (`score`, `census_score`, `plain_score`, `typed_score`, `typed1_score`,
  `vs_census`, provenance). `catalog.jsonl` has one line per function.
- Five functions got no compiling redraft (`func_8013506C`, `func_801368B8`, `func_80141944`,
  `func_80143160`, `func_80145384`): their folder holds the census body (or the guarded body in `src`) and
  `result.json` carries the census score with `score_source: census`.
- Matched from here (`status: matched` in result.json): `func_8013E308` (SHOUGATU clone port),
  `func_8013DC2C` (GYOZI sibling port), `func_8013B0C4` (hand edit of the 29-row redraft); 908 bytes.
- `hand.c` near misses: `func_8014206C` (6 rows, `beq` operand order), `func_8013C774` (literal rodata and
  delay-slot scheduling).

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. Results over the 29 unguarded functions with a redraft: 7 better
than the census (5,416 bytes), 19 the same, 3 worse; sum 4,710 to 4,715 rows, median 124 to 123. The headers
alone changed 10 of 28 plain scores (5 better); the typed rewrites beat the plain draft in 4 of 26 and lost in 5.
