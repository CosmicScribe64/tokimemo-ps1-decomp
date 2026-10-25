---
id: T-27073
title: Preserve constant provenance across independent macro families
status: Done
assignee: constant-tooling
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27070-signed-constant-replacement]]", "[[tickets/T-27072-caller-scoped-constant-sites]]"]
---

## Goal

Keep strict signed/caller-scoped check and replay valid after another reviewed macro family
is added to the same function. The current planner only normalizes its own named macros;
HUD failure-count macros cause a false provenance rejection for the completed scale request.

## Acceptance criteria

- [x] Independent simple constant macros can coexist without becoming editable targets of
      another family's request; ambiguous, function-like or unsupported expansion fails closed.
- [x] A restored raw literal is still detected and replayed; unrelated macros stay untouched.
- [x] Cross-family duplicate numeric values do not merge meanings or weaken source binding.
- [x] Synthetic CLI regression, full suite, independent review and actual read-only HUD check pass.

## Notes

All 27 binaries compiled and passed their SHA1 targets in the clean source wave; only the
final rename guard failed on hud_command_roll_poor_result. Root owns source/build integration.
Worker changes only ignored scratch and preserves caller, unit, type and rollback guarantees.

## Result

Exact patch `8bfcf4ba1779544a6e765c858df61508403f247685b71298ed4252ea8d5b65e8`
passed independent review and 126 integrated tests (5.413 seconds). Foreign macro spans are
noneditable; only active uniquely defined single-integer object macros normalize for provenance.
The coherent live read-only HUD request plans zero changes with unchanged input hashes.
An earlier partial-copy probe mixed old ETC with live HUD files and was withdrawn; it did
not identify stale live source. The clean build now finishes with 8,980 valid ledger rows,
27/27 independent hashes and 6,952 functions. Evidence is ignored T-27073/result reports
and next-integration/clean-wave-proof.json. No broader phase completion claim.
