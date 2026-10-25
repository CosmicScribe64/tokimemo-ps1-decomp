---
id: T-27072
title: Bound constant sites to their owning caller and state machine
status: Done
assignee: constant-tooling
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27062-main-essential-locals-constants]]", "[[tickets/T-27064-complex-overlay-reconciliation]]"]
---

## Goal

Enable precise per-caller constant insertion for reviewed game-rule inventories. Existing
callee/member selectors can otherwise mix unrelated state machines sharing mode fields or
dispatch helpers. Keep the existing selector forms and their default behavior intact.

## Acceptance criteria

- [x] A bounded caller selector filters existing call/member/local sites by exact unit and function.
- [x] Wrong units, missing/ambiguous functions and unproved source provenance fail closed.
- [x] Preserve original values, widths, signedness, preprocessing behavior and type views.
- [x] Synthetic regressions cover cross-caller and cross-overlay separation, macro/inactive
      code rejection, declaration ambiguity and ledger replay where supported.
- [x] Independent review and Docker proofs pass before root integration/application.

## Notes

Work in ignored scratch only; root owns live tooling/source, final contracts and commits.
Reuse the present const planner/proof machinery; avoid unrelated syntax or extraction work.

## Integration checkpoint

Independent review caught a missing public-CLI metadata route for positive wrappers. The
corrected patch `835bafb8fe807d8aa3332c9da7e694b996202ed30e9473ab58c024c4575587b1`
routes wrapped requests through strict planning, signed request metadata and rollback.
The original stale-literal CLI counterexample now fails check and is restored by replay;
repeat check/replay is stable. Root integrated the exact patch and all 122 tests pass in
3.961 seconds. Unwrapped behavior is preserved; wrapper use in game inventories follows.

Final bounded tool gate: independent review approves the corrected public CLI behavior.
The current 126-test suite, full clean build and all rename/header/symbol/global guards pass.
Actual caller-scoped inventory applications remain owned by the unit tickets, not this tool ticket.
