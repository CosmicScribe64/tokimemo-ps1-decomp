---
id: T-25016
title: "Candidate reconstruction for the fragment functions: extractor v2, canonical census, then per-function matching"
status: In Progress
assignee: octa
created: 2026-10-21
updated: 2026-10-21
links: [[[status]], [[matching-notes]], [[build-step-gaps]], [[tickets/T-25015-lever-expansion-tooling]], [[tickets/T-25014-allocator-source-shape-automation]]]
---

## Goal

T-25015's census showed the remaining wall is candidate quality: of
96 INCLUDE_ASM functions in src, only 9 have a machine-usable
candidate; the other 87 keep their work as fragments in drafts/ or in
guard forms the extractor cannot read yet. With references for all
96 now building in the remote scratch (mirrored asm bundle), the work
to actual matches is:

1. Extractor v2: assemble candidates from drafts/ fragments (wrap
   body fragments in the src signature), score every variant, keep
   the best per function.
2. Register-canonicalized census scoring, so register-only near
   misses rank by true distance (the packet metric overstates them).
3. Re-census all 96, rank, and work the list top-down with the shape
   engine plus hand reconstruction; every function that reaches zero
   locally is prepared for the CI gate on a PR.

## Acceptance criteria

- [ ] Extractor v2 + canonical scoring landed; census covers the
      large majority of the 96 with a scored candidate.
- [ ] A ranked work list exists in the sweep ledger (baseline scores
      recorded per function).
- [ ] Each function attempted is recorded (method, budget, best) in
      the ledger and, on success, in matching-notes with a PR for CI.

## Notes

- Branch octa/t25016-reconstruction, stacked on octa/t25015-tooling
  (PR #66 carries the T-25013/14/15 stack into main).
- Assembly of the reconstruction happens in the remote scratch;
  drafts produced here are filed under drafts/T-25016/ per project
  convention before any src change is proposed.

## Comments
