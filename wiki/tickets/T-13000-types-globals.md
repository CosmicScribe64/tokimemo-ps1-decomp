---
id: T-13000
title: Recover evidence-backed types and globals
status: Done
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
- [x] Complete review against CODING_STANDARDS.md before Done.

## Notes

Baseline origin/main 95c3fc9f. Docker only, one compiler job. Parent T-13000; pilot T-13001. No broad speculative header cleanup.

## Comments

## Results

See [[master-types-pilot]]. Three functions: func_80139910 (952), func_8013A784 (1,388), func_8013ACF0 (1,308), total 3,648 literal linked bytes. MASTER SHA1 passes. Header/global audits pass; no shared headers changed. Full 27 rebuild not triggered. All artificial scalar-view and frame idioms carry FAKE comments. func_8013B804 remains assembly. Two-axis review completed against 95c3fc9f and these tickets.

Declaration audit after reusing generated assembly from all integration address spaces: check_headers OK; migrate_globals --check OK; sync_protos --check-branch OK, zero new disagreements / 15 known. Initial MASTER-only assembly audit found 12 known; the complete assembly inventory removes that coverage limitation. No full-game checksum claim.

## Review (2026-10-15)

Independent standards review: no hard violations or actionable smell findings; original matching constraints override generic refactor advice. Independent spec review: no findings for the bounded MASTER pilot. Both inspected f17ceea9 and literal-byte/checksum evidence. Standards review requested explicit prototype-audit documentation; it is recorded above, resolving that documentation gap. No unresolved findings. Full 27 checks remain an integration/PR gate, not claimed here. Both tickets moved to Done.
