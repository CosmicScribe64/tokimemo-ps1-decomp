# T-18200: EN_NICHI redraft after the types-first pass

Fresh drafts of the 19 EN_NICHI functions that were unmatched at `w2-good`, made after the types-first pass of
T-18200 (wiki page `en-nichi-family-results`, section "Types-first pass (T-18200)"). None scored 0; five matched
after hand edits and are in `src/` (`status: matched` and a `hand.c` with the matched body in their result
folder): `func_80132834`, `func_80136B74`, `func_80134258`, `func_80133CA8`, `func_80135A48` (2,500 bytes).

- `redraft.py`: the driver, copied from `drafts/T-17900/redraft.py` (itself from T-16550 and T-16560) and adapted
  to EN_NICHI (run it in Docker from the repo root; its docstring lists the rewrites).
- `inventory.json`, `environment.json`: the functions and the header hashes the run used.
- `_runs/plain/EN_NICHI/<func>/`: m2c with the new headers through the T-14500 census path. Only the headers
  differ from the census. The plain runs used the header before `game.h` was included.
- `_runs/typed/EN_NICHI/<func>/`: the same draft after the type-driven rewrites (`rewrites` in result.json).
- `EN_NICHI/<func>/`: the better of the two after `tools/draft_fixes.py` (`body.c`, the full unit `context.c`,
  `declarations.h`, and `result.json` with `census_score`, `plain_score`, `typed_score`, `score`, `vs_census`).
  `catalog.jsonl` has one line per function.
- `EN_NICHI/func_8013358C/hand.c`, `EN_NICHI/func_80132EE8/hand.c`: index-form hand drafts whose remaining
  difference is register allocation (`hand_note` in result.json).

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the same measure as
`drafts/T-14500/best-quality-catalog.jsonl`. The census scored the hand-written guarded body of the two guarded
functions (`func_801330D0`, `func_80136A2C`), so a fresh m2c draft of those scores worse.

Reproduce: `tools/docker.sh python3 drafts/T-18200/redraft.py --jobs 2 --timeout 240` (rescoring needs
the header of the commit named in `environment.json`).
