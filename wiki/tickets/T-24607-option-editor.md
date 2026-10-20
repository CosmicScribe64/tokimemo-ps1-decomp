---
id: T-24607
title: Match OPTION numeric editor
status: Done
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24600-finish-matching]]"]
---

## Goal

Complete OPTION `func_80139A30` and finish the user-requested commit/push handoff.

## Acceptance criteria

- [x] Default C matches all 1,036 literal linked bytes.
- [x] Full Docker build, executable hashes and declaration guards pass.
- [x] Independent standards and behavior reviews have no unresolved findings.

## Notes

Selector-lifetime recovery reaches a literal match in production. Word-sized staging, the nonvolatile halfword view of a global declared volatile for a neighboring function, and the empty decrement-result test carry FAKE explanations. Removing the test gives15/260 differing words; simplifying the byte staging gives122/259. Seven helper prototypes and one byte global declaration were added to the canonical OPTION header after checking definitions.

## Comments

- 2026-10-20: Created In Progress before production promotion. User requests wrapping up and pushing completed work; other matching lanes are preserving their findings.
- 2026-10-20: In Progress -> In Review. Root default linked proof matches1,036 bytes. Full ninja, fresh27 hashes, globals/headers and prototype guard pass (zero new disagreements,15 known). Independent behavior review has no findings, including the distinct minus-one and minus-ten/hundred/thousand zero-result behavior. Standards review pending.
- 2026-10-20: In Review -> Done. Independent standards review passes with no unresolved findings and separately verifies the literal object proof. All integration gates pass. Final census6,823/6,952 functions,2,098,084/2,276,920 bytes;129 functions remain. Completed session adds seven functions /6,516 bytes.
