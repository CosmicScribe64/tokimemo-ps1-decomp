---
id: T-14300
title: Recover remaining EVENT source families
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[bunka-family-results]]", "[[etc-family-results]]", "[[broad-coverage-results]]"]
---

## Goal

Increase literal verified EVENT C bytes in isolated codex/event-14300, starting at 45d52546. Production ownership is EVENT sources and header only. Preserve existing C.

## Acceptance criteria

- Full-TU default literal linked proof and affected EVENT SHA1 for gains.
- Header, aggregate migration and prototype checks; coding standards review.
- Keep failed probes in ignored scratch with concise evidence.

## Notes

Read wiki index, local agent setup, CODING_STANDARDS, prior T-12801 EVENT results and Bunka/ETC transfer evidence before experiments. Do not repeat the 801103A0 dispatcher or historical entry-read/halfword-chain negatives. Prefer consistent GameState/CharFlags access and ordinary nested switches in reachable medium/large families. Docker only; one compiler at a time in this lane.

## Comments

2026-10-15: Created isolated worktree from origin/main; original checkout remains read-only. Ignored disc and expected inputs copied from integration without modifying raw sources.

2026-10-15 first checkpoint: five adjacent place handlers in src/ovl/EVENT/8010FE10.c pass default full-TU literal linked proof: 80112750 (480), 801130F0 (528), 80113300 (544), 801136FC (476), 801144AC (520), total 2,548 bytes. Existing EventStateView pointer used consistently for record selector, flags and calendar; existing table pointers viewed as u8 pointers as in matched siblings. No header edits or new FAKE. Evidence build/event14300/places-v1.diff and places-build.log. EVENT SHA1, headers, aggregate migration and prototype branch guard pass.

Initial 8010A91C record-loop probe improves from 12 differing rows to two after the target-supported bit1 view and consistent EventStateView pointer. Remaining pointer-initialization / zero-counter instruction order survives source initialization reversal. Rejected probe restored; scratch only. Disk pressure noted; no duplicate inputs deleted.
