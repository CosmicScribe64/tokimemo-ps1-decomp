# T-17900: NAME_ENT redraft with the recovered types

Fresh drafts of the 32 NAME_ENT functions that were unmatched at `w2-good`, made after the types-first pass of
T-17900 (wiki page `name-ent-family-results`, section "Types-first pass (T-17900)"). Two of them were matched
by hand from matched siblings after the first run and are in `src/` (`status: matched` in their result.json):
`func_80139740` and `func_8013EE7C` (904 bytes).

- `redraft.py`: the driver, a copy of `drafts/T-17700/redraft.py` adapted to NAME_ENT (run it in Docker from the
  repo root; its docstring lists the rewrites). NAME_ENT reads three main-exe aggregates that `main_api.h`
  declares as bytes or a word; the m2c context gets typed views of them and the drafts read them through casts,
  as the matched NAME_ENT code does: `D_8011ECD0` (`DisplayActor44[]`), `D_80123120` (`NameRecordF00[]`, two
  records folded) and `D_800E7D10` (`NameMetricWork`, an object view `((NameMetricWork *)&D_800E7D10)->f`).
  Changes against T-17700: the object view; the product-first record form `(i * 0xF00) + D_80123120`; record
  byte offsets without the view cast (`*(u8 *)((unsigned char *)D_8011ECD0 + (6531))`, the form m2c prints for
  constant offsets) written as fields of `D_8011ECD0`, `D_80123120` and `D_801217D0`; a no-argument prototype
  of `func_80054E6C` in the m2c context (m2c read its callers' loop registers as arguments, `M2C_ERROR`, so
  `func_80140F90` and `func_80141820` had no compiling draft); `s32` for a `void` definition the unit calls
  earlier without a prototype (implicit `int`, `func_80139F30`).
- `hand.py`: a copy of `drafts/T-17700/hand.py` (also reads a guard with a blank line before `#else`): scores
  the guarded `NON_MATCHING` bodies in `src` through the same full-unit path and keeps the better of hand body
  and redraft (`variant: hand-guarded`, `hand_guarded_score`).
- `inventory.json`, `environment.json`: the functions and the header hashes the run used.
- `_runs/plain/NAME_ENT/<func>/`, `_runs/typed/NAME_ENT/<func>/`: the two runs (m2c with the new headers; m2c
  on folded asm with the typed views plus the rewrites, `rewrites` in result.json). The full-unit `context.c`
  files are pruned.
- `NAME_ENT/<func>/`: the best starting point (`body.c`, the full unit `context.c`, `declarations.h`, and
  `result.json` with `scope`, `score`, `census_score`, `census_path`, `plain_score`, `typed_score`,
  `vs_census`, provenance). `catalog.jsonl` has one line per function.

`func_80142A80` draws the lines of a song with `set_kanji_string`. Its drafts here write those 21 strings as
their rodata labels (`D_8014C4F0`..`D_8014C6F4`, declared in its `declarations.h`) instead of the text, and its
raw m2c outputs were removed; the scores were measured with the string literals.

Scores are the resolved full-unit diff rows of `tools/exhaustive_drafts.py`, the measure of
`drafts/T-14500/best-quality-catalog.jsonl`. The census used the hand-written guarded body for the 9 guarded
functions, so compare the other functions.

Results (21 unguarded functions not matched here): all compile (the census needed manual repair for
`func_80140F90` and `func_80141820`); against the census 14 better (19,372 bytes), 7 the same, 0 worse; sum
7,056 to 6,107 diff rows, median 271 to 195. The typed run beat the plain one in 12 of 19 functions that compiled
both ways and lost in 4. For the 9 guarded functions the hand bodies in `src` stay the best start. No draft
scored 0. Hand starts: `NAME_ENT/func_80149B08/hand.c` (port of `func_801486F8`, 35 differing instructions, one
load placed late) and `NAME_ENT/prologue_line_start/hand.c` (index register).
