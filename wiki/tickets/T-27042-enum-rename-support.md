---
id: T-27042
title: Support enum tags and enumerator renames
status: Done
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[rename-tool]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Extend tools/rename.py so the remaining MainMode tag and MODE_ names can be renamed with scope, collision checks, proofs and replay; never edit the game identifiers manually.

## Acceptance criteria

- [x] Evidence and supported scope documented; no unsupported semantic claims.
- [x] Meaningful Docker validation; applied game names preserve clean 27/27 hashes and 6,952 functions.
- [x] Independent Standards and Spec review resolved before Done.
- [x] Wiki and ledger/replay behavior updated as relevant.

## Comments

2026-10-24: Claimed for the sustained phase 2 continuation.

2026-10-24 implementation: type rows now discover enum tags/typedefs, and the new enumerator row kind renames existing enum members through the type/header-closure planner. UPPER_SNAKE validation, collision checks, hypothesis marks, inverse/gcc/prototype name maps, check and replay are integrated. Synthetic coverage includes shared/private scope, initializer references, anonymous enums with string/comma expressions, literal-string preservation, invalid targets, hypotheses, stale-use detection and repeated replay. First tests exposed override-guard suffix false positives (GAME_MODE_RUN incorrectly counted as MODE_RUN); exact enum identifier filtering fixes that without changing symbol guard checks. All 73 rename tests now pass in Docker. Independent review pending; no game enum names applied.

2026-10-24 independent review: Luna Standards found no hard violations/new smells. Sol 6.1 correctness review reproduced three P2 issues: enumerator check/replay exceeded declaration scope, cross-header enum typedef aliases were unresolved, and targets collided case-insensitively. Fixed all three and added a regression per counterexample. All 76 rename tests now pass in Docker; reviewer verification pending. No game enum names applied.

2026-10-24: Done. Sol 6.1 verified all three P2 fixes and reran their3regressions; no unresolved review finding. Current clean scheduler build passes27/27 and6,952functions.
