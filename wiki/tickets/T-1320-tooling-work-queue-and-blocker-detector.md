---
id: T-1320
title: Tooling: work queue and blocker detector
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[tickets/T-0018-ugen-temp-register-order]]", "[[tickets/T-1321-register-promotion-build-step]]", "tools/queue.py", "tools/test_queue.py"]
---

## Goal

Give batch agents a ranked work list so they stop reading asm headers by hand and stop spending time on functions that cannot match with the current toolchain.

## Acceptance criteria

- [x] `tools/queue.py` lists every remaining `INCLUDE_ASM` function (main exe and the 26 overlays) with size, leaf or not, file, call count and flags (loop, jump table, string, single trailing nop).
- [x] Blocker detector for the T-0018 register-promotion gap (flags R and V), calibrated against the recorded cases and the matched functions; precision and recall in [[matching-notes]].
- [x] Output modes: table with `--files`, `--next N --files ...`, `--blocked`, `--summary`, `--calibrate`; duplicates marked when a dupes list exists (optional).
- [x] Backlog ticket [[tickets/T-1321-register-promotion-build-step]] with the evidence, the detector count and the rule to log skipped cases in `wiki/data/t0018-cases.md`.
- [x] [[decompile-workflow]] starts from `queue.py --next 20 --files <yours>` and logs T-0018 skips; `tools/list_leaves.py` is a wrapper.
- [x] `tools/test_queue.py` (synthetic asm); clean build 27 of 27 sha1 OK.
- [x] Inline code review against CODING_STANDARDS.md, findings resolved.

## Notes

Detector design and results: [[matching-notes]], section "Work queue and the T-0018 detector (T-1320)".

## Comments

2026-10-09 inline review (tooling code, CODING_STANDARDS sections 5, 7a, 8 checklist): no game data or asm committed (the data table has function names and one-line symptoms only); tests use synthetic asm; no `src/` or `include/` change. Findings: (1) `tools/queue.py` shadows the stdlib `queue` module for scripts run from tools/, fixed by a note in the docstring and by loading it by path in `list_leaves.py` and the tests; (2) calibration is in-sample, stated in [[matching-notes]]. Clean build 27 of 27 sha1 OK.
