---
id: T-11150
title: Twin detection for jump-table functions
status: Done
assignee: Claude
created: 2026-10-14
updated: 2026-10-17
links: [[[build-system]], [[tickets/T-1300-reuse-c-across-identical-functions]], [[tickets/T-3320-tooling-near-duplicate-function-reuse]], tools/dupes.py, tools/neardupes.py]
---

## Goal

`tools/dupes.py` and `tools/neardupes.py` refused every function with a `jtbl_` or `.L` relocation (516 unmatched functions, 544 KB). Fingerprint them too, so their twins are found and matched C can be copied to them.

## Acceptance criteria

- [x] Local label targets (`j .L`, `%hi/%lo(.L)`) are masked and kept as offsets from the function start.
- [x] `jtbl_` relocations are masked; the table words are part of the fingerprint as offsets from the function start.
- [x] A copy into a jump-table function is applied only when the build can verify it (`--apply --check`, table in its own function file); others are listed as "needs jtbl check".
- [x] Report line: jump-table groups, fillable members, bytes.
- [x] Synthetic tests: twins at different addresses compare equal, different tables do not, local labels compare by relative offset.
- [ ] Orchestrator: run on the real build (see Comments).

## Notes

Tables are read from the function file (`.late_rodata` after the rodata_pieces.py mark) or else from the unit's splat data files. A table that ends in zero words stops at the first word that is not a local label. A table entry outside the function is refused. String rodata in the function file still makes the function "data in function".

A jtbl plan is `island` when every table of the target is in its own function file: the C object then provides its rodata and the compiler's table replaces the handed-over one. `outside` (orphan chunk, no island) is never applied.

## Comments

- 2026-10-14: Implemented without game data (cloud worktree, no build). `test_dupes.py` 54 OK, `test_neardupes.py` 26 OK, `check_headers.py include src` OK, `test_check_headers.py` 16 OK. To verify locally: `tools/docker.sh python3 tools/dupes.py -v`, `tools/docker.sh python3 tools/neardupes.py --report`, then `--apply --check` for both and a full `tools/docker.sh ninja`.

2026-10-17: Cleanup pass. Merged on main: the jump-table fingerprinting in tools/dupes.py and tools/neardupes.py with tests (fe2db3347), 10 jump-table twins filled with --apply --check (67552e51a) and the parameter-name fix (8e5824b45). The last open item, the run on the real build, is met by that fill and by the T-16500 whole-tree sweep, which uses the same plans and holds back 'outside' tables. Review against CODING_STANDARDS.md: Python 3, docstrings, synthetic tests, no game data; only nits (8e5824b45 lacks a leading ticket id). Closing as Done.
