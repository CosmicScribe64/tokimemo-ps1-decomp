---
id: T-13001
title: Recover MASTER layouts and verify pilot matches
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[data-types]]", "[[game-state]]"]
---

## Goal

Recover types and calling conventions from original access patterns. MASTER is the implementation scope; other active teams' files remain read-only.

## Acceptance criteria

- [x] Record assembly evidence and distinguish hypotheses from demonstrated layouts.
- [x] Verify accepted C with full-TU scoring, literal linked bytes and affected checksums.
- [x] Run header/global/prototype audits and all 27 checks if shared declarations change.
- [ ] Complete review against CODING_STANDARDS.md before Done.

## Notes

Baseline origin/main 95c3fc9f. Docker only, one compiler job. Parent T-13000; pilot T-13001. No broad speculative header cleanup.

## Comments

## Results

See [[master-types-pilot]]. Three functions: func_80139910 (952), func_8013A784 (1,388), func_8013ACF0 (1,308), total 3,648 literal linked bytes. MASTER SHA1 passes. Header/global audits pass; no shared headers changed. Full 27 rebuild not triggered. All artificial scalar-view and frame idioms carry FAKE comments. func_8013B804 remains assembly. Awaiting two-axis review against 95c3fc9f and these tickets.
