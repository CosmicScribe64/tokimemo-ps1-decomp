# T-18600: MASTER redraft after the types-first pass

Fresh drafts of the 11 MASTER functions that were unmatched at `w2-good` (17,412 bytes), made after the
types-first pass of T-18600 (wiki page `master-family-results`, section "Types-first pass (T-18600)"). None
scored 0; two matched after hand edits and are in `src/` (`status: matched`, the matched body in `hand.c`):
`func_80135CF4` and `func_8013348C` (1,208 bytes).

- `redraft.py`: the driver, copied from `drafts/T-18300/redraft.py` (itself from T-18000, T-17700, T-17400,
  T-17200, T-16560 and T-16550) and adapted to MASTER. Its docstring lists the changes: the MasterSprite44
  views of `D_8011ECD0` and `D_80120650`, the innermost-range fold, and three new rewrites (`club-bits`,
  `record-copy`, `reroll`).
- `inventory.json`, `environment.json`: the functions and the header hashes of the first run.
- `_runs/plain/MASTER/<func>/`: m2c with the new headers through the T-14500 census path. Only the headers
  differ from the census.
- `_runs/typed/MASTER/<func>/`: m2c on folded asm plus the type-driven rewrites (`rewrites` in result.json).
  These are the second run, with `reroll` and `record-copy`; the first typed run had neither.
- `MASTER/<func>/`: the better of the two after `tools/draft_fixes.py` (`body.c`, the full unit `context.c`,
  `declarations.h`, and `result.json` with `census_score`, `plain_score`, `typed_score`, `score`,
  `vs_census`). `catalog.jsonl` has one line per function.
- `MASTER/<func>/hand.c` for `func_80132444`, `func_80135FB8` and `func_8013B804`: hand drafts closer than
  the redraft (`hand_score`, `hand_note` in result.json). `func_80135FB8/hand.c` is a port of its matched
  twin `func_80133F80`.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the same measure as
`drafts/T-14500/best-quality-catalog.jsonl`. Hand scores are `tools/funcdiff.py --resolve` lines (removed
plus added), which give the same count on these functions.

Reproduce: `tools/docker.sh python3 drafts/T-18600/redraft.py --jobs 2 --timeout 240` (rescoring needs the
header of the commit named in `environment.json`; the two matched functions are no longer `INCLUDE_ASM`, so
pass `--only` with the other nine).
