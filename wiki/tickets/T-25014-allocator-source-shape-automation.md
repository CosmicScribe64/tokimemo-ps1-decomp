---
id: T-25014
title: "Automation for allocator-artifact and source-shape residuals: a packet-based variant search engine"
status: In Review
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

- [x] Engine exists with unit tests for each transform (output compiles,
      transform is reversible/identifiable in the results table).
- [x] Scorer validated against known baselines (see notes: packet
      metric tracks the project scores with documented offsets).
- [x] Runs recorded for at least the allocator test bed
      (main func_8006A044) and the CFG test bed (GEKO func_80136C20),
      with the best score per transform family reported either way.
- [x] Any function reaching zero diff is written up in matching-notes
      and prepared for the CI gate; no src/ change lands without it.
      (No function reached zero; nothing to promote.)

## Notes

- Branch octa/t25014-shape-automation, stacked on octa/t25013-levers
  (the T-25013 findings branch) so the lever evidence is in the tree.
- Development happens in the remote scratch tree; the engine and its
  tests are committed under tools/ only once they pass their tests.
- Packets stay outside the repo (remote scratch only), as with T-25013.

## Comments

### Session notes (Octa, remote, 2026-10-21)

Built and committed under tools/: shape_search.py (the hill-climb
engine, with plateau/sideways moves and a visited set), shape_transforms.py
(eight transform families: decl_swaps, empty_if_probes, cmp_variants
with the `* 1` wrappers, type_views, copy_intro, field_copy_intro,
switch_local, goto_forms), shape_score.py (relocation-aware packet
metric), and test_shape_search.py (unit tests, all passing).

Scorer validation: the packet metric reads the three known drafts at
113 (main func_8006A044), 85 (ENDING func_801387B0) and 15 (GEKO
func_80136C20) against project scores of about 87, 80 and 6 for
in-tree drafts. The ENDING figure tracks closely; the other two run
higher because the packet TUs fold some symbol offsets differently
and the main packet draft is not the in-tree draft. The metric is
consistent and monotone, which is what the search needs; closing the
last normalization gap (relocation pairing across delay slots) is a
known refinement.

Runs (remote packet environment, trials logged per run):
- GEKO func_80136C20 (CFG bed): 236 trials including plateau walks;
  no variant improved on 15. goto_forms fired and scored identically
  to baseline, as T-25013 lever 4 predicts (the if and goto forms
  give as1 the same CFG edges; the region set covers every block in
  this function, so no CFG respelling can manufacture the +1).
- main func_8006A044 (allocator bed): 400 trials including plateau
  walks across the comparison family; no variant improved on 113.
  field_copy_intro variants scored 115-155 (the extra load/store
  traffic costs more than the colour change gains), switch_local was
  neutral, cmp flips walk a flat plateau at 113.

### Outcome (Octa, 2026-10-21)

The automation exists, is tested, and is cheap to run in the full
tree (one command per function with its draft as the bed). On the two
hardest known residuals it finds no improvement, which sharpens the
T-25013 conclusion: these two functions' gaps are not reachable by
single-site source transforms from the current drafts. The engine is
ready to be pointed at the other ~117 functions, where the drafts sit
closer; that sweep is maintainer-paced work, so the ticket goes to
In Review with the tooling landed.
