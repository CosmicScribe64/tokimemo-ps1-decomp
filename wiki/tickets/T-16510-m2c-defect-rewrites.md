---
id: T-16510
title: Detect and rewrite recurring m2c draft defects
status: Done
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16200-matching-wave-2]], [[decompile-workflow]], [[exhaustive-drafts]]
---

## Goal

Agents keep fixing the same m2c output defects by hand: the shared-store `goto` after switch cases, Shift-JIS strings printed as hex escapes, the wrong post-increment compare (`D += 1; if (D == n)`), and `void` drafts whose callers read `$v0` or whose original is implicit-int. Add detectors, and mechanical rewrites where the fix is mechanical, as a post-processing step usable on any draft body (`tools/draft_fixes.py`, optional flag in `tools/m2c.py`).

## Acceptance criteria

- [x] Detectors for: shared-store goto/label after switch cases, Shift-JIS hex-escape strings, post-increment compare rewrite; report-only: void draft whose callers read `$v0`, s32-without-return candidates (T-16210 rule).
- [x] Unit tests from real before/after pairs (`tools/test_draft_fixes.py`, 37 tests; `tools/test_m2c_args.py` +3); the full tool test suite passes.
- [x] Detector hit counts over the `drafts/T-14500` selected bodies (below, `drafts/T-16510/survey.json`).
- [x] Rewrites applied to the top 30 hits by score and rescored with full-translation-unit scoring (2 jobs); score changes and new zero-score drafts reported; improved alternatives saved under `drafts/T-16510/` with provenance.
- [x] Short section in `wiki/decompile-workflow.md`.
- [x] No edits under `src/`, no in-place edits of the existing `drafts/` archives.
- [x] Code review against `CODING_STANDARDS.md` (needed for Done): see Comments.

## Notes

Source fixes: `wiki/matching-notes.md`, T-12701, T-13201, T-15300, T-15400, T-15600, T-16210.

### What was built

- `tools/draft_fixes.py`: `analyze(body, asm_text, rewrite, callers, scope, select)` runs the detectors; CLI `check`, `fix`, `survey`, `rescore` (docstring and [[decompile-workflow]] "Recurring m2c defects"). `tools/m2c.py --fix-defects` runs the rewrites on m2c's output.
- `tools/m2c_args.py`: `post_increment_compares` (old/new per symbol) replaces the scan inside `post_increment_old`. The old scan stopped at an in-place `addiu Rl, Rl, 1`, so m2c.py never wrote `D++` for SHOUGATU `func_80137144` (the T-15600 case); `postinc_pattern` is shared.
- Tests: `tools/test_draft_fixes.py` (T-12701 BUNKAKEN switch, T-15400 and T-15300 Shift-JIS strings, T-15600 post-increment, T-16210 implicit int, synthetic asm for the `$v0` flow and the callers scan, archive survey and subset selection with a fake scorer).

### Detector hits over the 1,066 selected bodies (`drafts/T-16510/survey.json`)

| Detector | Bodies | Findings |
|---|---:|---:|
| `goto-shared-store` (rewritable) | 27 | 30 |
| `goto-shared-store-blocked` | 263 | 602 |
| `sjis-escape` | 13 | 18 |
| `postinc-compare` | 11 | 11 |
| `postinc-wrong` | 0 | 0 |
| `s32-no-return` (report) | 71 | 71 |
| `void-callers-read-v0` (report) | 0 | 0 |

Why labels stay gotos (602): shared tail with calls or `if`s after the label 384, real loops 188, empty tail (shared return) 8, tail of calls 10, break inside another loop 3, a temporary the tail reads that the switch assigns 4, a temporary not assigned at every jump 3, other 2. The rule base for `s32-no-return` was measured on the matched sources: 207 of 334 `s32` functions without a value return show the signal, 32 of 5,383 `void` ones do (86.6 percent of the flagged are `s32`). Callers that read `$v0` after a call: 209 of 4,007 call targets have such a caller, none of the archive's drafts.

### Rescore (50 drafts changed by a rewrite, ranks by archived score; ranks 1 to 30 are the "top 30")

Score = differing rows of the function in the full unit (`exhaustive_drafts.py` path), the unmodified body recompiled in this tree as baseline, rewrites tried alone and as a greedy build-up.

| | bodies | score 0 | improved | same | worse (kept original) |
|---|---:|---:|---:|---:|---:|
| ranks 1 to 30 | 30 | 3 | 18 | 2 | 7 |
| ranks 31 to 50 | 20 | 0 | 13 | 1 | 6 |
| `sjis-escape` | 13 | 2 | 8 | 3 | 0 |
| `postinc-compare` | 11 | 1 | 10 | 0 | 0 |
| `goto-shared-store` | 26 | 0 | 13 | 0 | 13 |

- New score-0 drafts: `BUNKAKEN:func_80145844`, `BUNKASAI:func_801431D4`, `SHOUGATU:func_80137144` (all three are already matched in `src/` by T-15300, T-15400 and T-15600: the tool reproduces the hand fixes; paths `drafts/T-16510/BUNKAKEN/func_80145844/`, `.../BUNKASAI/func_801431D4/`, `.../SHOUGATU/func_80137144/`). No other mechanical zero.
- Biggest gains: `main:func_80079070` 450 to 35, `GEKO:func_801327AC` 223 to 2, `BUNKA_SD:func_80139290` 509 to 133, `SHUGAKU:func_8013282C` 183 to 116, `BUNKASAI:func_8013B25C` 183 to 122, `RPG_BAT:func_801525B0` 35 to 20 (a call value moved into the one store). Total 1,499 differing rows removed over 34 bodies.
- Candidate for matching agents (not promoted, unverified): `GEKO:func_801327AC` (`INCLUDE_ASM` in `src/ovl/GEKO/801323E0.c`): after the rewrite one row differs (`sb` for `sh`); `*(s16 *)&D_80144C40 = 0;` (the initializer's line, T-9510) gives score 0: `drafts/T-16510/follow-up/GEKO/func_801327AC/`.
- Thirteen drafts got worse with the `goto` removal (seven of ranks 1 to 30) and keep the original: the "shared block" in m2c's output is often real.
- `SHOUGATU:func_8013737C`: the archived unit no longer compiles (another function of the file was matched since); baseline and rewrite were scored in a unit rebuilt from the current source, 208 to 194.
- Experiment, not part of the tool: signature-only change to `s32` on the 71 `s32-no-return` drafts: 3 better, 29 same, 12 worse, 27 do not compile against the existing `void` prototype.

### Follow-ups

- Hand off `GEKO:func_801327AC` (above) and the lowest new scores to matching agents: `main:func_80079070` 35, `RPG_BAT:func_801525B0` 20, `GEKO:func_80134228` 25, `GEKO:func_80135AC8` 25, `SHOUGATU:func_80139334` 27, `NAME_ENT:func_80142A80` 28.
- `m2c.py --fix-defects` is off by default; turn it on in `bulk_m2c.py` only after a bulk run shows no regression (the rewrite is scored there anyway).
- The s32 report cannot be judged by score until the header prototype is changed with the signature.

## Comments

- 2026-10-16 (review, CODING_STANDARDS.md checklist): diff w2-good...t16510-m2c-defects touches only `tools/` (new `draft_fixes.py` and `test_draft_fixes.py`, edits to `m2c.py`, `m2c_args.py`, `test_m2c_args.py`), new folders under `drafts/T-16510/` and wiki pages; nothing under `src/`, `include/`, `config/` or `asm/`, no existing `drafts/` archive file changed, no game data, binary or asm staged (the saved `context.c` files are the same kind of translation-unit text that `drafts/T-14500` already holds; `survey.json` quotes 18 Shift-JIS strings cut to 24 characters). Python 3 with docstrings, run through `tools/docker.sh`, no new dependency, `//`-free C in the saved bodies; commits are small and carry the ticket id and the `Co-Authored-By` trailer. Checks run in Docker at the reviewed head: all 51 `tools/test_*.py` pass (37 new tests in `test_draft_fixes.py`, 3 new in `test_m2c_args.py`, `test_m2c.py` 25); `draft_fixes.py check|fix SHOUGATU func_80137144` and `rescore --list` give the saved `postinc-compare` body byte for byte; `m2c.py BUNKASAI:func_801431D4 --fix-defects` lists the `sjis-escape` rewrite and without the flag the output is unchanged; the 34 saved bodies have no dangling `goto`, and one of them (`GEKO:func_80135AC8`) is a chosen subset, so a second `fix` run would apply the other label (by design). Counts re-derived from `results.jsonl` and `survey.json`: 50 rows (3 zero, 31 improved, 3 same, 13 worse; ranks 1 to 30: 3, 18, 2, 7), 1,499 rows removed, survey 27/263 goto bodies, 13 sjis, 11 postinc, 71 s32, all as reported. Findings and fixes: (1) the ticket said seven drafts got worse with the `goto` removal, the data says thirteen (seven of ranks 1 to 30) -> corrected; (2) the `rescore` text in [[decompile-workflow]] and the tool docstring did not say that single rewrites and the greedy build-up run only for two to six rewrites -> now stated; (3) [[decompile-workflow]] still quoted the first detector's 78 functions for the post-increment shape; the wider scan finds it in 207 of 6,958 asm functions (83 with the old scan; a sample hit, DATE `func_801383BC`, is a real `D++ == 0` in its matched C) -> count added. Not blocking: `m2c.py` now writes `D++` for the in-place case by default (`--no-postinc` turns it off); the branch sits 101 commits behind `w2-good` and merging it conflicts only in the append-only `wiki/log.md` and `wiki/matching-notes.md` (no code overlap); `wiki/decompile-workflow.md` has three `sources:` lines in its front matter from earlier tickets. The build needs no re-run (no source or header change). No unresolved findings. Status Done.
