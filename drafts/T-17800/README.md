# T-17800: OPTION redraft with the recovered types

Fresh drafts of the OPTION functions that were unmatched at `w2-good`, made after the types-first pass of T-17800
(wiki page `option-family-results`, section "Types-first pass (T-17800)").

- `redraft.py`: the driver (run it in Docker from the repo root; its docstring lists the rewrites). It is
  `drafts/T-17500/redraft.py` with three OPTION changes. The display records are written through the header's
  full view `OptActor44` instead of `DisplayActor44`. The `menu-arrays` rewrite types the four row arrays handed
  to `menu_set` (`func_8004F870`) as `s16 spXX[n]` or as one `Arrs4x8`. Two repairs of m2c output were added:
  `raw-display` for a folded label that m2c printed as a number, and `return-type` for `void` printed in an
  `s32 f(void)` of the header.
- `inventory.json`, `environment.json`: the 32 functions still unmatched when the final run started, and the
  header hashes of that run.
- `_runs/<variant>/OPTION/<func>/`: one census-path run per variant (the full-unit `context.c` files are pruned):
  - `plain`: m2c with the new headers, nothing else;
  - `typed`: m2c on the folded asm, then the type-driven rewrites (`rewrites` in result.json);
  - `census`: the T-14500 census body (or a better T-16510 one) through the same rewrites.
- `OPTION/<func>/`: the best of the three after `tools/draft_fixes.py`: `body.c`, the full unit `context.c`,
  `declarations.h`, and `result.json` with `scope`, `score`, `census_score`, `plain_score`, `typed_score`,
  `census_typed_score`, `vs_census` and provenance. `catalog.jsonl` has one line per function.
- `OPTION/<func>/hand.c`: hand drafts better than the driver's (score in `hand_score` of result.json).
- `OPTION/<func>/` for the four functions matched during the pass: `body.c` is the matched C, `context.c` the unit
  file, `result.json` has `status: matched` and `matched_in`.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. A score of 0 is a candidate match, not a verified one. The census
scored the hand-written guarded body of the two guarded functions (`func_80139E3C`, `func_80139F8C`), so compare
the other 30.

## Reproduce

```
tools/docker.sh python3 drafts/T-17800/redraft.py --jobs 2 --timeout 150
tools/docker.sh python3 drafts/T-17800/redraft.py --finish-only        # rebuild OPTION/ from _runs
tools/docker.sh python3 drafts/T-17800/redraft.py --rewrite BODY.c      # print the typed rewrites of one body
```

The final run reused the plain results of the run before it; the functions whose typed or census draft failed
there (`func_80132BA8`, `func_80135E48`, `func_80136C30`, `func_801373C8`) were rerun after the menu-arrays fix
and the `func_80132BA8` prototype.
