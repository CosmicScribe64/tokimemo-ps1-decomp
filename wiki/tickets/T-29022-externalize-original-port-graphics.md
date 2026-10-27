---
id: T-29022
title: Load original graphics externally and pass live HUD state
status: In Progress
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[port-readiness-review]]", "[[tickets/T-29019-independent-port-readiness-review]]"]
---

## Goal

Load original graphics externally and pass live HUD state against original PS1 behavior. Independent review found the prior completion claim unsupported.

## Acceptance criteria

- [ ] Implement real production behavior with original-backed inputs and explicit unsupported cases.
- [ ] Verify positive and negative cases through the actual app/service path; no synthetic substitutes presented as parity.
- [ ] Preserve simulation/RNG/save behavior and compare relevant original-game checkpoints.
- [ ] Independent code/standards review resolves findings before Done.

## Notes

Root serializes canonical edits. Candidate/proof directory: `.claude/orchestrator/state/T-29022/`. See the readiness review for exact findings and scope.
