---
id: T-20830
title: autotweak edit kinds for the wave-3 recipes (GameState pointer, statement runs, chain stores, multi-use local)
status: Done   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee: tr-autotweak-recipes
created: 2026-10-17
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[matching-notes]]", "[[tickets/T-11170-cheat-sheet-auto-tweaker]]"]
---

## Goal

Make four recipes that closed wave-3 functions by hand into autotweak edit kinds, so the search finds them and the matchers stop re-deriving them. autotweak ran in about 10 wave-3 notes (686 edits on `func_80140A90`, 411 on `func_801518B8`, 228 on `func_80072D70`, 183 s on `func_80144094`, 150 s on `func_8004B358`) and mostly exhausted without gain, because the recipes that closed functions were not edit kinds.

- `gstate`: `GameState *g = &D_800E6280;` with every read through `g` (7 matches: main `func_8006EB64`, `func_8006EEC4`, `func_80072998`, `func_800726F0`; TACO `func_80133670`, `func_80133728`; DATE `func_80152E34`; 5 or more misses; 48 of the 101 guarded drafts read `D_800E6280` directly twice or more with no `g`).
- `group`: several statements on one source line (as1 schedules by line): TACO `func_80140ADC` (eight UV stores), `func_8013CCD0`, main `func_80055A38`, GYOZI `func_8013E638`, DATE `func_80149F48`. `t_lines` joins only an adjacent pair; a run of 5 to 8 is out of reach of the depth-2 search.
- `chain`: `a = b = c = v;` for equal stores (DATE `func_80155004`), comma-line colour stores (`func_80152E34`).
- `inline_multi`: a local that copies a global and is tested twice replaced by direct reads (main `func_800726F0`); `t_temp` inline handles exactly one definition and one use.

## Acceptance criteria

- [x] `tools/recipes.py` with the four edit kinds (pure text transforms on the `Func`/`Variant` interface), registered by `recipes.register` at the end of `tools/autotweak.py` (the `typefix.register` pattern).
- [x] `HINT_EDITS`: tie += group; ugen += group, chain; priority += gstate, inline_multi; structure and argument += gstate. `--only-kinds`; `--list` prints the per-kind counts.
- [x] `tools/test_recipes.py` (59 tests; synthetic bodies shaped like `func_80140ADC`, `func_80055A38`, `func_8006EB64`, `func_800726F0`; no-op guards; variant counts bounded). The full tool suite passes in Docker (`tools/docker.sh python3 -m unittest discover -s tools -p 'test_*.py'`).
- [x] Docs: [[decompile-workflow]] ("Recipe edits for autotweak (T-20830)"), [[register-cheatsheet]] (12 rows name their edit kind, 2 rows added), [[matching-notes]] ("T-20830"), [[log]], [[index]].
- [x] Optional item done in part: `regdiff.py` prints `ORDER-ONLY` when the structural differences only move instructions, `autotweak` maps the class `order-only` to group, order, join, split. The source lines of the moved instructions are not printed: the draft object has no line numbers.
- [x] First use over the 101 guarded drafts and the 16 wave-3 archive drafts: 6 new matches (2,660 B), hit table below and in `drafts/T-20830/README.md`.
- [x] Code review against `CODING_STANDARDS.md` (the lead), then Done.

## Notes

Rules for this ticket: only `tools/`, `wiki/` and `drafts/T-20830/` changed. `src/` and `include/` are untouched, so the matches are saved as plain C in `drafts/T-20830/<UNIT>/<func>/body.c` for the lead to apply. They were applied together in a scratch copy: full build 27/27 sha1 OK, `check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` pass.

Design in short: each kind is a function `t_<kind>(f) -> [Variant]` whose labels start with the kind name (`edit_kind` and `--only-kinds` read the first word). A shared `Layout` (brace depth, loops, stores and calls per line) and a name-based `may_alias` (the Access analysis of the order edit) guard `chain` and `inline_multi`. Widths come from locals, the `extern` objects and the struct field names of `include/**/*.h`. Per-function caps: `recipes.MAX_VARIANTS` (gstate 20, group 128, chain 24, inline_multi 12), taken in turn from the kinds' sub-lists. Details and validation numbers are in [[decompile-workflow]].

Validation with no game asm: the four transforms over all 6,370 function bodies of `src/` raise nothing and every variant parses; undoing each recipe in the matched source and asking for it back restores 96 of 106 `gstate` functions, 237 of 304 chain lines, 17 of 17 comma lines and 56 of 91 multi-statement lines (the rest hold dependent statements). `--list --all` on the 101 guarded drafts: 392 `gstate`, 1,853 `group`, 508 `chain`, 100 `inline_multi` edits.

First use (`autotweak.py --all --only-kinds gstate,group,chain,inline_multi --time 30 --score-jobs 2`, scratch copies, about 95 minutes on a loaded machine, 2 compile jobs): 117 drafts. Guarded set of 101: 6 matches, 20 lower, 73 unchanged, 2 not set up. Archive set of 16: 1 match (a known one found again), 6 lower, 9 unchanged; 8 of the 16 needed externs or types before they compiled. The GYOZI match came from a second pass after the loop-head join learned to take the empty counter test along (the first pass had no variant for that draft).

| draft | size | stripped score | exact | edits | set |
| --- | --- | --- | --- | --- | --- |
| main:func_80072D70 | 1064 B | 355 -> 0 | match (exact) | gstate all last | 101 guarded |
| DATE:func_8013AF1C | 600 B | 495 -> 0 | match (exact) | gstate all | 101 guarded |
| ETC:func_80143F24 | 188 B | 283 -> 0 | match (exact) | chain join L6-8 210, gstate all | 101 guarded |
| GYOZI:func_8013E638 | 236 B | 90 -> 0 | match (exact) | group head L7 (2) | 101 guarded |
| NAME_ENT:func_8013BE18 | 348 B | 121 -> 0 | match (exact) | gstate all | 101 guarded |
| TT:func_8013D2E0 | 224 B | 105 -> 0 | match (stripped) | inline_multi v L7 use1 | 101 guarded |
| TT:func_8013E498 | 520 B | 1005 -> 580 | exact 198 -> 62 | chain join L16-17 | 101 guarded |
| DATE:func_8013B488 | 808 B | 699 -> 325 | exact 75 -> 54 | gstate all last | 101 guarded |
| TACO:func_80134A18 | 884 B | 685 -> 344 | exact 36 -> 13 | chain split L10 rtl | 101 guarded |
| TT:func_801374C0 | 404 B | 274 -> 18 | exact 60 -> 4 | chain join L24-26 210, group head L15 (3) | 101 guarded |
| main:func_80077CA8 | 392 B | 1247 -> 1107 | exact 28 -> 20 | gstate all | 101 guarded |
| main:func_80062210 | 468 B | 234 -> 130 | exact 24 -> 8 | group chunks4 L9-16 | 101 guarded |
| main:func_800764AC | 408 B | 203 -> 99 | exact 54 -> 26 | gstate all | 101 guarded |
| TACO:func_801484E4 | 392 B | 285 -> 195 | exact 12 -> 9 | gstate all last | 101 guarded |
| TT:func_80136AD8 | 396 B | 175 -> 85 | exact 58 -> 30 | chain join L25-26 | 101 guarded |
| TT:func_801392A0 | 1060 B | 105 -> 45 | exact 22 -> 18 | chain join L95-97 210 | 101 guarded |
| RENSYU:func_80133628 | 900 B | 730 -> 675 | exact 42 -> 20 | gstate first3 | 101 guarded |
| OPTION:func_80139E3C | 200 B | 470 -> 430 | exact 60 -> 46 | inline_multi g L20 use2 | 101 guarded |
| TT:func_80142570 | 384 B | 170 -> 130 | exact 56 -> 44 | chain join L14-15 10 | 101 guarded |
| BUNKA_SD:func_8013987C | 148 B | 270 -> 240 | exact 14 -> 10 | group head L7 (2) | 101 guarded |
| GEKO:func_80134228 | 244 B | 140 -> 120 | exact 16 -> 8 | gstate all | 101 guarded |
| GEKO:func_80135AC8 | 252 B | 140 -> 120 | exact 16 -> 8 | gstate all | 101 guarded |
| OPTION:func_80139F8C | 132 B | 136 -> 116 | exact 11 -> 5 | gstate all | 101 guarded |
| TT:func_80146000 | 548 B | 40 -> 20 | exact 14 -> 8 | inline_multi b L27 use1 | 101 guarded |
| NAME_ENT:func_801460D8 | 452 B | 220 -> 205 | exact 40 -> 34 | inline_multi temp_a1 L16 use1 | 101 guarded |
| SHUGAKU:func_801352B0 | 384 B | 294 -> 284 | exact 37 -> 33 | gstate all | 101 guarded |
| NAME_ENT:func_8013B994 | 380 B | | skipped | setup: invalid base: instruction shape differs from target ( | 101 guarded |
| TAIIKU:func_8014357C | 664 B | | skipped | setup: invalid base: instruction shape differs from target ( | 101 guarded |
| DATE:func_80155004 | 368 B | 385 -> 0 | match (exact) | chain join L23-25 210, group head L22 (3) | archive |
| TEL:func_80136724 | 7044 B | 10685 -> 7638 | exact 1401 -> 984 | gstate all last | archive |
| DATE:func_801449D4 | 1180 B | 714 -> 414 | exact 138 -> 48 | gstate all | archive |
| TACO:func_8013B460 | 1328 B | 305 -> 228 | exact 82 -> 52 | gstate all | archive |
| RPG_BAT:func_8014BFE4 | 892 B | 3495 -> 3435 | exact 128 -> 126 | group head L40 (2) | archive |
| DATE:func_80152E34 | 396 B | 663 -> 618 | exact 148 -> 144 | gstate all last | archive |
| TACO:func_80144094 | 924 B | 1759 -> 1739 | exact 252 -> 248 | group head L23 (2) | archive |

Not chosen, as the lead listed: compiler-gap rows, worktree seeding and Docker contention, the quick fixes (siblings --package outside the repo, funcdiff "linked address differs", check_headers/sync_protos and RPG_BAT overrides). Follow-ups worth a ticket: a second autotweak pass with all kinds from the `best.c` texts of the 26 drafts that fell (TT `func_801374C0` is at 4 exact rows); the drafts with 0 variants (no `D_800E6280` read, no equal stores) need other recipes.

## Comments

2026-10-17, self-review against `CODING_STANDARDS.md` section 13 before In Review. Scripts are Python 3 and Docker-run, with a module docstring and tests; no host installs. No game data, asm or binaries committed (`drafts/T-20830/` holds C text only). The generated edits stay C89 (declarations at block top; `GameState *g = &D_800E6280;` is an initializer of an automatic pointer). No FAKE comment is added by any new kind. Commits are small with the ticket id. Open for the reviewer: whether the six matches carry a comment on the non-obvious tricks when they are applied (a chain of byte stores, the declaration order of `g`, the empty counter test).

2026-10-17, code review (lead) against `CODING_STANDARDS.md` section 13, branch `tr-autotweak-recipes` against `w2-good`. Result: no blocking findings, one small fix.

- Scope: only `tools/`, `wiki/` and `drafts/T-20830/` changed (47 files); `src/`, `include/`, `config/` untouched. No game data, asm, binaries or disc files; the drafts are C text (no `glabel`, `.word` or register-form asm lines, no `//` comments). Hashes of nothing original are committed.
- Tests: the full tool suite through Docker (`tools/docker.sh python3 -m unittest discover -s tools -p 'test_*.py'`): 1,250 tests OK (3 skipped). `tools/test_recipes.py` has 59 tests.
- Tool run on a real input: `autotweak.py --list TT:func_8013D2E0` prints the per-kind counts (`group 8, inline_multi 1`, 50 edits); `autotweak.py TT:func_8013D2E0 --only-kinds inline_multi,gstate,chain,group --time 60 --dry-run` printed `MATCH TT:func_8013D2E0  105 -> 0  [inline_multi v L7 use1] (dry run)` in 41 s and left the tree clean. This reproduces the hit table row.
- Code: Python 3, module docstrings that state the rules, run through the Docker wrapper, no host installs, no stray prints, only the standard library. `recipes.register` follows the `typefix.register` pattern; `--only-kinds` rejects unknown names and lists the valid ones.
- Docs: [[decompile-workflow]] (kinds, guards, flags, validation numbers, first use), [[register-cheatsheet]], [[matching-notes]], log and index agree with the code and with the hit table in `drafts/T-20830/README.md`.
- Wiki: ticket status, kanban column, index line and log entries match.
- Fixed in review: `import unittest.mock` stood at the end of `tools/test_recipes.py`; moved to the other imports.
- For the lead when the six matches in `drafts/T-20830/<UNIT>/<func>/body.c` go into `src/`: GYOZI `func_8013E638` keeps the `FAKE` comment of its draft for the empty counter test (`if (i && i) { }`; section 7); the other five add no `FAKE`, and each should get a short comment on the non-obvious form (`GameState *g` declared first or last among the locals, the byte-store chain in ETC `func_80143F24`, the direct re-read of `p->cursor` in TT `func_8013D2E0`). The matches are not counted in progress until applied and built.
