---
id: T-11190
title: Bulk m2c and auto-solve pipeline for small functions
status: Done
assignee: Claude
created: 2026-10-14
updated: 2026-10-17
links: [[[decompile-workflow]], [[register-cheatsheet]], [[tickets/T-11170-cheat-sheet-auto-tweaker]], [[tickets/T-1330-tooling-m2c-context-and-permuter]], tools/bulk_m2c.py, tools/test_bulk_m2c.py, tools/autotweak.py, tools/m2c.py, tools/permute.py]
---

## Goal

About 790 `INCLUDE_ASM` functions under 600 bytes remain. Run m2c, a mechanical cleanup, a guarded compile, the single-function scorer and the T-11170 hill climb over all of them unattended. Write verified matches as plain C, revert everything else to `INCLUDE_ASM` and keep the drafts with their scores outside the repo for later work.

## Acceptance criteria

- [x] `tools/bulk_m2c.py` with `--max-size` (default 600), `--unit`, `--exclude` (default TT), `--limit`, `--jobs`, `--keep-score` (default 30), `--time` (default 60), `--out` (outside the checkout), `--summary`.
- [x] Draft from `tools/m2c.py` `run_m2c` with the usual context; cleanup limited to mechanical edits (`?` to `s32`, `//` to `/* */`, `M2C_*` placeholders refused, m2c's missing-symbol declarations kept for the symbols the body uses); a `temp_*`-inlined variant is scored next to the plain one.
- [x] Draft placed as `#ifdef NON_MATCHING ... #else INCLUDE_ASM #endif`, compiled alone (autotweak `BuildScorer`); cheap fixes for undefined symbols and redeclarations, else the reason is recorded and `INCLUDE_ASM` restored.
- [x] Score 0 verified like a permuter output; scores under `--keep-score` go to `autotweak.climb`; a verified match replaces the guard with plain C. Extra declarations move to `include/main_api.h` or `include/ovl/<UNIT>.h`, then `check_headers.py` and the unit target must pass, else all is reverted.
- [x] Non-matches are reverted and saved to `<out>/drafts/<UNIT>/<func>.c`; `<out>/near.txt` sorted by score; summary with MATCH count/bytes and failures by reason.
- [x] Resumable (`<out>/state.jsonl`), crash recovery (`<out>/inflight/`), jobs work on disjoint C files and every ninja run and header write holds one lock.
- [x] Tests `tools/test_bulk_m2c.py` (mocked m2c/compile/score): cleanup, guard insertion and restore, threshold routing, header move and revert, resume, recovery, summary.
- [ ] Real run in Docker (orchestrator): one unit with `--limit 5`, then the full run with `--jobs`.

## Notes

- `autotweak.BuildScorer` got a class attribute `INSTRUCTION_TOLERANCE` (default 4, unchanged behaviour); bulk_m2c's scorer sets it very high so raw m2c drafts with a different instruction count still get a score.
- Stacked on the T-11170 branch (`tools/autotweak.py` is not on main yet).
- Found while reading `autotweak.climb`: when the base score is 0 and `verify` fails, it returns `matched=True` (the `while best > 0` loop is skipped). bulk_m2c never routes a score-0 draft to `climb`, so it is not affected; `autotweak.py` itself still is (T-11170).
- Not done: casts as a cheap fix (the IDO messages do not name a type pair the tool could act on), `U` suffix and `(u8)` cast removal (they can change code; the climb tries the width edits).

## Comments

- 2026-10-14: implemented without the game binaries (cloud worktree): only the synthetic tests ran (`python3 tools/test_bulk_m2c.py`, 21 OK; `tools/test_autotweak.py` OK; `check_headers.py include src` headers OK; `test_check_headers.py` OK). In Review until the orchestrator's real run and the code review.

2026-10-17: Work is on main (72a3faba9 through ccade9834, merge 6ccc26117). All acceptance criteria are met, including the real run: 58 + 3 functions matched, then +33 under T-11210, and the tool has been reused by later tickets. Code review against CODING_STANDARDS found no blocking issue in tools/bulk_m2c.py or the matched functions. Two housekeeping findings (71 stale banner comments in include/, and header recovery after a crash) go into the followup.

Followup opened: [[tickets/T-21007-bulk-m2c-banner-clutter-and-header-recovery|T-21007]].
