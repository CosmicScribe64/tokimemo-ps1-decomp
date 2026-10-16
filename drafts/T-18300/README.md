# T-18300: ENDING redraft with the recovered types

Fresh drafts of the 13 ENDING functions that were unmatched after the first match of T-18300
(`func_80132F18`, a port of main `check_k_scroll`), made after the types-first pass of T-18300 (wiki page
`ending-family-results`, section "Types-first pass (T-18300)"). One of them, `func_80134B18`, is a guarded
draft in `src/`.

- `redraft.py`: the driver, copied from `drafts/T-18000/redraft.py` (ENDING uses the same main-exe
  aggregates as SHUGAKU: display records `D_8011ECD0`, Rec24 records `D_801217D0`, GameState). Changes: ENDING
  paths; the stack-table rewrite also reads an index scaled into its own temporary (`t = i * 2; (sp + t)`);
  a new `struct-bytes` rewrite turns m2c's `&D_800E6280 + i` (a byte offset that C would scale by the
  struct size) into `(u8 *)&D_800E6280 + i`. Run it in Docker from the repo root; its docstring lists the
  rewrites.
- `hand.py`: scores the `hand.c` drafts through the same full-unit path and keeps the better of hand draft
  and redraft (`hand_score`, `variant: hand`).
- `inventory.json`, `environment.json`: the functions and the header hashes of the run.
- `_runs/plain/ENDING/<func>/`, `_runs/typed/ENDING/<func>/`: the two runs (m2c with the new headers; m2c on
  folded asm plus the rewrites, `rewrites` in result.json). The full-unit `context.c` files are pruned.
- `ENDING/<func>/`: the best starting point: `body.c`, the full unit `context.c`, `declarations.h`, and
  `result.json` with `scope`, `score`, `census_score`, `census_path`, `plain_score`, `typed_score`,
  `vs_census` and provenance. `catalog.jsonl` has one line per function.
- `hand.c` drafts, written in the style of the matched scene scripts of `80137340.c`:
  `func_80137BB4` (58 rows: the original keeps the promoted `D_8011F4DE` in `a1` and compares
  `(u8) D_8013C364` with the shared constant `1` in `v0`), `func_80137E40` (64 rows, registers only: the
  original keeps the flag word in `a0` and the shifted word in `v0`), `func_8013759C` (94 rows: the original
  keeps the promoted `D_8011F513` in `s1`), `func_801387B0` (156 rows: constant registers),
  `func_8013A2E8` (the census repair of the string copy, with its declarations; m2c still prints
  `M2C_LWL` for the 27-byte copy, so neither run compiles).

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. Over the 12 unguarded functions: 7 better than the census
(7,268 bytes), 5 the same, none worse; sum 4,250 to 4,113 rows, median 161.5 to 142.5. No draft scored 0.
The census scored the hand-written guard body of `func_80134B18` (24 rows; the new draft has 40).
