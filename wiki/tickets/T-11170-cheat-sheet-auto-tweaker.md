---
id: T-11170
title: Cheat-sheet auto-tweaker for near-miss drafts
status: In Review
assignee: Claude
created: 2026-10-14
updated: 2026-10-14
links: [[[register-cheatsheet]], [[decompile-workflow]], [[tickets/T-1330-tooling-m2c-context-and-permuter]], [[tickets/T-9810-permuter-farm-for-near-misses]], tools/autotweak.py, tools/test_autotweak.py, tools/permute.py]
---

## Goal

About 157 drafts sit under `#ifdef NON_MATCHING`, most of them register-allocation misses. The random permuter farm (T-9810) found almost nothing. Try the C changes that [[register-cheatsheet]] lists on every guarded draft, as a targeted search.

## Acceptance criteria

- [x] `tools/autotweak.py [UNIT:]func` and `--all`: parse the guarded drafts out of `src/**/*.c`.
- [x] Deterministic edits inside the one function: byte view of a switch selector, operand swap of `+`/`==`/`!=`, join/split of statement lines, `x = x + k` <-> `x += k`, empty test before a use, unused local word, adjacent declaration swap, `* 2^n` <-> `<< n`, inline or introduce a temporary, width/signedness of a narrow local or parameter.
- [x] Scoring through permute.py's single-function pipeline (setup + upstream Compiler/Scorer, `tools/cc.py`); greedy hill climb with depth-2 pairs and `--time` per function.
- [x] On score 0: `permute.verify_candidate` (ninja object, `funcdiff --resolve`, unit sha1), then the guard block is replaced by the plain C and `MATCH UNIT:func` is printed; fake edits carry `/* FAKE: ... T-11170 */`.
- [x] Synthetic unit tests with a mocked scorer (`tools/test_autotweak.py`).
- [ ] Orchestrator run on the real build (one function, then `--all`).

## Notes

- Written in a cloud environment without game binaries: the real scorer path (`BuildScorer`) is not run here; everything else is covered by the synthetic tests.
- Pass over the 157 real drafts without compiling (`--list --all`): 4 to 433 single edits per function, median 44; every variant keeps brackets balanced; `rewrite_guard` replaces every block cleanly.
- A match found by the tool still goes through review: FAKE edits stay marked; plain edits (operand order, line layout) need no comment.
- `--exclude TT` keeps contributor-claimed overlays out of an `--all` run.

## Comments

- 2026-10-14: implemented; `python3 tools/test_autotweak.py`, `python3 tools/check_headers.py include src` and `python3 tools/test_check_headers.py` pass. In Review pending the real-build run and code review.
