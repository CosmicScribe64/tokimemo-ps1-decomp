---
id: T-39007
title: Complete player Continue and save recovery lifecycle
status: Ready
assignee: Codex
created: 2026-10-29
updated: 2026-10-29
links: ["[[port-adversarial-review]]", "[[port-testing]]"]
---

## Goal

Resolve review R10–R12: expose normal Continue and explicit backup recovery, and persist ending clear/album state through real storage.

## Acceptance criteria

- [ ] Normal launch offers a valid Continue path without replaying New Game.
- [ ] Corrupt/incomplete saves provide actionable recovery with valid backups preserved.
- [ ] Ending clear state is marked complete only after successful persistence; failures remain recoverable.
- [ ] Actual quit/relaunch tests cover normal save, backup recovery, failed writes and clear-state/album retention.
- [ ] Independent code review resolves all findings against CODING_STANDARDS.md.

## Notes

Save codec passed37 integrity checks, but production routing and clear-save behavior are missing. Review report distinguishes serialization from player lifecycle acceptance.
