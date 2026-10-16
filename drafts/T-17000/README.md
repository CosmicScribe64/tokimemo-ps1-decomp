# T-17000: main redraft after the types-first pass

Fresh drafts of the unmatched main-exe functions (`src/main/*.c`) at `w2-good`, made after the
types-first pass of T-17000 (wiki page `main-family-results`, section "Types-first pass (T-17000)").

- `redraft.py`: the driver (run it in Docker from the repo root; its docstring lists the rewrites).
  It is `drafts/T-16560/redraft.py` (two variants, typed rewrites, `tools/draft_fixes.py`, finish) with
  the asm fold of `drafts/T-16550/redraft.py`, widened to every typed main aggregate and every sized
  array of `include/main_api.h`. The TACO-only by-value `TcPos` rewrite is dropped.
- `inventory.json`, `environment.json`: the functions and the header hashes at the start of the run.
- `_runs/plain/main/<func>/`: m2c with the current headers through the T-14500 census path (bulk
  cleanup, full-unit compile with the cheap declaration fixes). Only the headers differ from the census.
- `_runs/typed/main/<func>/`: the same with the asm fold, the display-record view and the typed
  rewrites (`rewrites` in result.json). The run folders keep no `context.c` (each is the src file with
  the body in place of its `INCLUDE_ASM`); the chosen one is in `main/<func>/`.
- `main/<func>/`: the better of the two after `tools/draft_fixes.py`: `body.c`, the full unit
  `context.c`, `declarations.h`, and `result.json` with `scope`, `score`, `census_score`, `plain_score`,
  `typed_score`, `vs_census` and provenance. `catalog.jsonl` has one line per function.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the same measure as
`drafts/T-14500/best-quality-catalog.jsonl`; 0 is a candidate, not a verified match. Guarded functions
are regenerated from m2c (the census used the hand-written guarded body for those), so compare the
unguarded ones.

The run started before the T-17000 table declarations were added to `include/main_api.h`; the
functions that read those tables were rerun afterwards (`--only`), so their `_runs` and `main/` folders
use the final header. The `environment.json` hashes are those of the first run.

## Matched from these drafts

Five drafts became matches with a small hand edit and are in `src/` (`status: matched` in their
result.json): `func_80075320`, `func_80053F04`, `func_80075AF8`, `func_8007959C`, `func_80077E30`
(1,948 bytes). `main/func_8007866C/hand.c` is a hand edit of the redraft at 14 funcdiff rows.
