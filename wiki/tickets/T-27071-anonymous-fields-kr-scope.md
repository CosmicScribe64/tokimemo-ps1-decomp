---
id: T-27071
title: Preserve anonymous member and K&R parameter ownership in rename proofs
status: Done
assignee: constant-tooling
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27064-complex-overlay-reconciliation]]"]
---

## Goal

Fix the six reproduced TACO rename blockers without weakening scope or compiler proofs:
three anonymous nested ranking-score members and three K&R parameter requests.
Synthetic fixtures and diagnoses are pinned in the complex-overlay follow-up manifest.

## Acceptance criteria

- [x] Correct declaration/member ownership; ambiguous cases fail closed.
- [x] Synthetic regression coverage and full rename suite pass in Docker.
- [x] Independent review resolves findings; root integrates the exact frozen patch.
- [x] Root applies the supported requests through the tool and verifies matching.

## Notes

No live tooling or game source writes by the worker. Root owns integration and tracking.
Preserve all K&R types/promotions, aggregate layout, source provenance and rollback checks.

## Review checkpoint

The first scratch patch passed 115 synthetic tests, but independent Sol review reproduced
an unsafe anonymous-member rewrite: an unrelated owner's preexisting missing-member error
was mistaken for a requested use and normalized by the compiler proof. The patch is rejected
and remains unapplied. Add the regression and fail closed on that ambiguous baseline before
resubmitting. K&R masking has no additional review finding. Evidence is ignored
`.claude/orchestrator/state/T-27071/final-review.md`.

Corrected patch `8a255bb2489fc7f1bbcd1e2acefb218a7b300071a57160cd0ea6f0c9c830cbe9`
rejects anonymous fallback when the pre-edit baseline already contains the same missing-member
diagnostic. Independent verification approves the fix and confirms the counterexample causes
no source/header or ledger writes. Root integrated it and all 116 tests pass in 4.167 seconds.
The six real requests still await root source application and matching proof.

Final application: all six previously blocked TACO requests are applied in 4dab92732.
Independent actual-source review verifies the three K&R parameters and three anonymous
ranking-score members without declaration, type or layout changes. Both 437-unit scoped
ABI proofs pass; clean configure/build/guards and 27/27 independent hashes pass with
6,952 functions. The bounded tooling/application ticket is Done; wider phase 2 remains open.
