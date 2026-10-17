---
id: T-21500
title: Stop configure from re-splitting asm on every checkout or merge
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: [[build-system]], [[tickets/T-16990-wave-3-conveyor]]
---

## Goal

`build.ninja` depends on the source directories themselves (`src/`, `src/main/`, `src/ovl/<NAME>/`, `include/`,
`include/ovl/`). A git checkout or merge updates directory mtimes, so ninja re-runs `configure.py`, which re-splits
all of `asm/`; every object then rebuilds. Under load this made each merge take 4-8 minutes, each agent worktree setup
6-15 minutes (six wave-3 blocker notes), and defeated an incremental PR check (2026-10-17 retro).

## Plan

- Make the configure edge depend on a stamp that changes only when the set of source files changes (added, removed,
  renamed), not on directory mtimes: write the sorted file list to `build/src-files.txt` only when its content
  changes (restat), and depend on that.
- Make the split restat-stable: splat output files that are byte-identical keep their mtime, so a re-split does not
  dirty every object.
- Check that adding a C file, renaming one and editing config still regenerate correctly.

## Acceptance criteria

- [ ] After `git checkout`/`git merge` that only edits existing files, `ninja` rebuilds only the edited units
- [ ] Adding or removing a source file still reconfigures
- [ ] Clean build 27/27 unchanged; tests for the new stamp logic

## Comments
