# T-17500: SHOUGATU redraft with the recovered types

Fresh drafts of the 38 SHOUGATU functions that were unmatched at `w2-good`, made after the types-first pass of
T-17500 (wiki page `shougatu-family-results`, section "Types-first pass (T-17500)").

- `redraft.py`: the driver (run it in Docker from the repo root; its docstring lists the rewrites). It is
  `drafts/T-17200/redraft.py` (itself `drafts/T-16560/redraft.py` with the fold and stack-table copies of
  `drafts/T-16550/redraft.py`) plus the `census` variant of `drafts/T-17100/redraft.py`. Two SHOUGATU additions:
  the stack-table rewrite also turns m2c's `&sp40[0]` and `((s16 *)sp30)[i]` into `sp40.v` and `sp30.v[i]`, and the
  census variant moves a typedef of the census declarations (`UnalignedBytes3`) into the function body.
- `inventory.json`, `environment.json`: the functions and the header hashes at the start of the run.
- `_runs/<variant>/SHOUGATU/<func>/`: one census-path run per variant (the full-unit `context.c` files are pruned):
  - `plain`: m2c with the new headers, nothing else;
  - `typed`: m2c on the folded asm, then the type-driven rewrites (`rewrites` in result.json);
  - `census`: the T-14500 census body (or a better T-16510 one) through the same rewrites.
- `SHOUGATU/<func>/`: the best of the three after `tools/draft_fixes.py`: `body.c`, the full unit `context.c`,
  `declarations.h`, and `result.json` with `scope`, `score`, `census_score`, `plain_score`, `typed_score`,
  `census_typed_score`, `vs_census` and provenance. `status: matched` with `matched_in` for the functions now in
  `src/`. `catalog.jsonl` has one line per function.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. A score of 0 is a candidate match, not a verified one. The census
scored the hand-written guarded body of the two guarded functions (`func_801347E0`, `func_80136948`), so compare
the other 36.

The run started before `include/ovl/SHOUGATU.h` included `game.h`, before `func_80137664` had a prototype and
before the two stack-table fixes; the functions those changes affected were rerun afterwards (`--only`), so their
folders use the final headers. `environment.json` keeps the hashes of the first run.

Matched in `src/` during the pass (1,516 bytes): `func_801419D0` (typed draft, score 0), `func_80132A34` and
`func_80133760` (ports of the matched GYOZI twins `func_80134938` and `func_80135518`), `func_8013AFEC` (typed
draft with the idioms of its matched sibling `func_80137E54`).

## Reproduce

```
tools/docker.sh python3 drafts/T-17500/redraft.py --jobs 2 --timeout 150
tools/docker.sh python3 drafts/T-17500/redraft.py --finish-only        # rebuild SHOUGATU/ from _runs
tools/docker.sh python3 drafts/T-17500/redraft.py --rewrite BODY.c      # print the typed rewrites of one body
```
