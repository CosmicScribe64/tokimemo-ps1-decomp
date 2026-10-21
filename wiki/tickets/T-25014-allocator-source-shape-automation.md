---
id: T-25014
title: "Automation for allocator-artifact and source-shape residuals: a packet-based variant search engine"
status: In Progress
assignee: octa
created: 2026-10-21
updated: 2026-10-21
links: [[[status]], [[plan-remaining]], [[build-step-gaps]], [[matching-notes]], [[as1-scheduler-rule]], [[tickets/T-25013-compiler-level-levers]]]
---

## Goal

T-25013 concluded the remaining ~119 functions are blocked on allocator
artifacts (uopt/ugen allocation choices the original build made
differently) and source shape (the C form that makes the pipeline emit
the original code), not on missing compiler switches. Those two classes
were previously worked by hand and by autotweak/permuter sweeps in the
full tree. Build a packet-based variant search engine that works from a
single-function packet (reference asm + draft TU) and hill-climbs source
transformations drawn from the project's own matching history:

- lifetime shaping for allocator artifacts: declaration order, copy
  introduction, empty-if priority probes, first-use reordering;
- expression respellings and type views from the matching-notes cheat
  sheet (operand order wrappers, byte views, `* 1` wrappers);
- CFG shape variants for as1 weight/window cases, informed by the
  lever 4 region test (a switch head's predecessor needs a region-exit
  successor for the +1 weight).

The engine runs in the remote packet environment (no disc needed),
scores with a funcdiff metric aligned to the project's score.py
(relocation-aware normalization), and keeps every variant's score so
results are reproducible. Any zero-diff result is then promoted through
the normal gate (PR, CI 27/27).

## Acceptance criteria

- [ ] Engine exists with unit tests for each transform (output compiles,
      transform is reversible/identifiable in the results table).
- [ ] Scorer validated against known baselines (func_8006A044 draft at
      the T-25012 score, func_801387B0 draft at the T-25002 score).
- [ ] Runs recorded for at least the allocator test bed
      (main func_8006A044) and the CFG test bed (GEKO func_80136C20),
      with the best score per transform family reported either way.
- [ ] Any function reaching zero diff is written up in matching-notes
      and prepared for the CI gate; no src/ change lands without it.

## Notes

- Branch octa/t25014-shape-automation, stacked on octa/t25013-levers
  (the T-25013 findings branch) so the lever evidence is in the tree.
- Development happens in the remote scratch tree; the engine and its
  tests are committed under tools/ only once they pass their tests.
- Packets stay outside the repo (remote scratch only), as with T-25013.

## Comments
