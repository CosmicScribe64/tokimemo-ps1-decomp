---
id: T-25030
title: Qualify older large overlay reconstructions without source sweeps
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[plan-remaining]]", "[[tickets/T-25028-read-only-candidate-triage]]"]
---

## Goal

Read-only review of at most twelve different actual remaining functions, prioritizing older large TAIIKU/TACO/RPG_BAT/MASTER/EN_NICHI reconstructions. Stop at a strong concrete unrepaired semantic/control/ABI defect with exact target evidence and a minimal proving experiment.

## Acceptance criteria

- [x] Verify current remaining membership and newest useful tracked body.
- [x] Distinguish repaired/stale raw holes and removed symbols from semantic defects.
- [x] Preserve actionable evidence or bounded negative table, no production changes.

## Notes

No builds, source grids, production edits, subagents or Astra. Exclude GEKO/OPTION, TEL36724 and MAIN strCallback (T-25029 owner). This is source qualification, not source ownership. Isolated tree /private/tmp/tokimemo-t25023; prior T-25028 preserved at pin768808209.

## Result

Current main2be3d3304 still119 remaining; all12 reviewed names confirmed. Different set from T-25028: two EN_NICHI, three MASTER, four TACO, two TAIIKU, one RPG_BAT. No strong new unrepaired semantic/control/ABI defect qualified. Newest useful bodies and specific negative ledgers listed in drafts/T-25030/README.md; earlier raw actor/dispatcher/array/callee/load defects are already repaired.

Bounded direct-call-count check for first eight bodies versus target has identical multisets after existing callee rename normalization. This is not argument/order/CFG semantic proof. Field/control spot checks found no new demonstrated defect; source lifetimes, homes and copy temporaries remain unresolved codegen questions. No build, new source variant, production/root edits or agents. Do not claim other107 functions blocked. Ticket to In Review for root selection/review; no implementation candidate nominated from this scope.

## Checks

Read-only Docker source membership and bounded direct-call audit completed; no compiler invoked. Wiki lint zero broken links/status mismatches, one existing large-page warning. `git diff --check` passes; production/header/tools/config differences empty. T-25030 remains In Review.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
