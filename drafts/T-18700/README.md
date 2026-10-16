# T-18700: BUNKASAI redraft with the recovered types

Fresh drafts of the 10 BUNKASAI functions that were unmatched after the types-first pass of T-18700 (wiki page
`bunka-family-results`, section "Types-first pass (T-18700)"). Two functions matched during the pass:
`func_80147F8C` (180 bytes, `BunkasaiNameTable` stack copy with a named string pointer; no draft here) and
`func_8015AF90` (2,328 bytes, from its `hand.c` below; `status: matched` in its result.json).

- `redraft.py`: the driver, copied from `drafts/T-18400/redraft.py` (itself from T-18000, T-17700, T-17400,
  T-17200, T-16560 and T-16550) with BUNKASAI paths; the rewrites are unchanged. Run it in Docker from the
  repo root; its docstring lists the rewrites.
- `hand.py`: scores the `hand.c` drafts through the same full-unit path and keeps the better of hand draft
  and redraft (`hand_score`, `variant: hand`). `python3 drafts/T-18700/hand.py [func ...]`.
- `inventory.json`, `environment.json`: the functions and the header hashes of the run.
- `_runs/plain/BUNKASAI/<func>/`, `_runs/typed/BUNKASAI/<func>/`: the two runs (m2c with the new headers; m2c
  on folded asm plus the rewrites, `rewrites` in result.json). The full-unit `context.c` files are pruned.
- `BUNKASAI/<func>/`: the best starting point: `body.c`, the full unit `context.c`, `declarations.h`, and
  `result.json` with `scope`, `score`, `census_score`, `census_path`, `plain_score`, `typed_score`,
  `hand_score`, `vs_census` and provenance. `catalog.jsonl` has one line per function (written before the
  hand drafts; the result.json files are current).
- `hand.c` drafts:
  - `func_8015AF90` (21 rows; census 400; now matched): port of the matched BUNKAKEN clone `func_80152680`
    (0.976 opcode ratio) with the BUNKASAI tables, no `D_80122EA4` terms in cases 1 and 25, `0x508` in
    case 22 and `func_80044750(0x50C)` added to case 23. With BUNKAKEN's file-level
    `MAIN_API_OVERRIDE_func_800847B8` the instructions were identical and only v0/v1 were swapped (the
    `player_blood_types` base `sp+0x3C` against the promoted `D_80122EBC`). Struct views, `GameState *g`
    removal, self-stores and a 7-minute permuter run did not close it; the cause was the callee
    declaration: BUNKAKEN calls `func_80132548` undeclared (implicit `int`), and calling `func_80132410`
    undeclared as well gives the match. The full-unit score of 21 includes the `andi` of the prototyped
    `func_800847B8` call because the scorer has no file prelude.
  - `func_80142BE8` (688; census 445, whose hand repair stays a valid starting point): the switch on the
    school year with three copies of the girl loop, as the target has; `(u8)(x >> 1)` gives the target's
    `andi`. Still 8 instructions short (the target indexes `unk_1BC[D_80122EBC]` with `multu` by a
    register 0x38) and the stack has three more slots above the selector (amount read uninitialized at
    `sp+0x11C`).
  - `func_801509A4` (63; census 59): the T-16100 draft with `BunkasaiGirlList`.
  - `func_80151364`: the guarded body from `src/ovl/BUNKASAI/801511E0.c` (2 lines from a match, see
    `wiki/matching-notes.md`); the hand scorer cannot build it outside the guard, so the redraft (122)
    stays in `body.c`.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. Over the 10 functions: 4 better than the census, 3 the same,
3 worse (the census held hand repairs for `func_80142BE8`, `func_801509A4` and the guarded
`func_80151364`); sum 2,576 to 2,453 rows, median 302 to 246 (better drafts: 5,960 bytes). No draft
scored 0.
