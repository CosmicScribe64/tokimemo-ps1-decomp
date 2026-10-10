---
id: T-5030
title: Tooling: wave-3 bug fixes
status: Done
assignee: agent (r3-fixes)
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[tickets/T-3300-tooling-fix-wave-2-bugs]]", "[[tickets/T-3340-shared-main-prototypes-and-byte-queue]]"]
---

## Goal

Fix the tooling bugs the wave-3 agents reported, each with a regression test: `funcdiff.py` picking the wrong C file, stale and string-relocation results; `permute.py`; the speed of `sync_protos.py --fix` plus a branch guard; `queue.py` dying in Docker.

## Acceptance criteria

- [x] `funcdiff.py` resolves by definition (`tools/funcloc.py`), scoped by `--unit`/`UNIT:`, refuses ambiguous names with a list; no MATCH for a stale object with `--built`/`--no-build`; string relocations and renamed symbols resolved under `--resolve`.
- [x] `permute.py`: K&R definitions, `all <func>`, source and asm from one unit.
- [x] `sync_protos.py --fix` profiled and fast; `--check-branch` and the "before finishing" section of [[decompile-workflow]].
- [x] `queue.py` failure reproduced as far as possible, cause found, fixed (cache).
- [x] Unit tests for each; all `tools/test_*.py` pass.
- [x] Clean build 27 of 27 OK, headers OK, progress unchanged.

## Comments
- 2026-10-10 inline review against CODING_STANDARDS.md (sections 10 to 13): Python 3 with docstrings, tests next to the tools, no game data or asm committed, no C or build-graph change (so no matched function can change bytes). Findings fixed during review: `funcloc.index` counted a file twice when `INCLUDE_ASM` and a body sit in one file (`#ifdef NON_MATCHING`); the K&R regex of `permute.py` matched a prototype followed by declarations; `queue.py` printed "no INCLUDE_ASM functions" for a `--files` list that matched nothing. Not covered: argument counts in `--check-branch`; `docker.sh` itself (no retry of a dropped attach).
