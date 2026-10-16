# T-18400: KANGEI redraft with the recovered types

Fresh drafts of the 11 KANGEI functions still unmatched after the types-first pass of T-18400 (wiki page
`kangei-family-results`, section "Types-first pass (T-18400)"). Two more functions matched during the
pass and have no draft here: `func_801370A0` (port of the MASTER clone `func_80137BF4`) and `func_80132C24`
(`func_8013454C` form), 1,468 bytes.

- `redraft.py`: the driver, copied from `drafts/T-18000/redraft.py` (itself from T-17700, T-17400, T-17200,
  T-16560 and T-16550). Changes: KANGEI paths; the fold reads multi-dimensional array globals
  (`s32 D_8013A170[2][3][13]`); `fptab` knows the KANGEI dispatch table; a new `gamestate-words` rewrite
  writes the word view `*(s32 *)&D_800E6280.unk_100` of the GsRec0FC records that m2c prints as an
  improper struct cast. Run it in Docker from the repo root; its docstring lists the rewrites.
- `hand.py`: scores the `hand.c` drafts through the same full-unit path and keeps the better of hand
  draft and redraft (`hand_score`, `variant: hand`). `python3 drafts/T-18400/hand.py [func ...]`.
- `inventory.json`, `environment.json`: the functions and the header hashes of the run.
- `_runs/plain/KANGEI/<func>/`, `_runs/typed/KANGEI/<func>/`: the two runs (m2c with the new headers; m2c on
  folded asm plus the rewrites, `rewrites` in result.json). The full-unit `context.c` files are pruned.
- `KANGEI/<func>/`: the best starting point: `body.c`, the full unit `context.c`, `declarations.h`, and
  `result.json` with `scope`, `score`, `census_score`, `census_path`, `plain_score`, `typed_score`,
  `hand_score`, `vs_census` and provenance. `catalog.jsonl` has one line per function (written before the
  hand drafts; the result.json files are current).
- `hand.c` drafts: `func_801362F0` (22: `% 2` as a truth value gives the signed fixup; the table value and
  the switch value sit one register lower than in the original), `func_80135004` (33: frame and RECT slot
  right with a `u8 girl` local; the original keeps the constant 1 in `$v0` for two compares),
  `func_8013852C` (36: the year sits in `$v0`, the original keeps `unk_F5F` there), `func_80138F88` (171:
  best-girl loop with the four `get_h_*` calls in the original order), `func_80133A54` (125, worse than
  the plain draft's 79; kept for its field names).

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. Over the 11 functions: 7 better than the census, 3 the same,
1 worse (`func_8013769C`, 142 to 154: the census draft stays its starting point); sum 1,809 to 1,478 rows,
median 142 to 130 (better drafts: 4,448 bytes). No draft scored 0.
