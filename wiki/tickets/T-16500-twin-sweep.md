---
id: T-16500
title: Whole-tree twin sweep after every merge
status: In Progress
assignee: claude-tooling
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16200-matching-wave-2]], [[decompile-workflow]], [[build-system]]
---

## Goal

`tools/dupes.py` and `tools/neardupes.py` copy matched C onto byte-identical and near-identical unmatched twins, but
they were run per package (`--files`), so a donor matched in one package never reached its twin in another. Add
`tools/twin_sweep.py`: one command that plans every exact, near and lenient copy over the whole tree (dry run by
default), and applies only to the C files it is given, each copy verified with the unit sha1 (the existing `--check`).

## Acceptance criteria

- [ ] `tools/twin_sweep.py` prints `target <- donor`, mode (exact / near / lenient) and bytes for every planned copy
- [ ] `--apply --files <list>` writes only into the listed C files and keeps a copy only when the unit sha1 holds
- [ ] known candidates confirmed or explained: TAIIKU func_801406A8, GYOZI func_8013506C, GEKO func_8013E22C, TACO func_801347F4
- [ ] unit tests in `tools/test_twin_sweep.py`; full tool test suite passes
- [ ] short section in `wiki/decompile-workflow.md`; nothing under `src/` or `drafts/` edited
- [ ] one-line command for the orchestrator to run after merges

## Notes

Rules for this ticket: no edits under `src/` (matching agents own the C files), at most 2 compile jobs, no push or merge.

## Comments
