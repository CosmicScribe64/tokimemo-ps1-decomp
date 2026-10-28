---
id: T-39000
title: Remove obsolete local orchestrator state
status: In Progress
assignee: Codex
created: 2026-10-27
updated: 2026-10-27
links: []
---

## Goal

Delete `.claude/orchestrator/state/` to free disk space, explicitly authorized by the user.

## Acceptance criteria

- [ ] Remove state and verify absence.
- [ ] Preserve source, raw inputs, asset cache, and earlier worktree backups.

## Notes

Historical task scratch files and local archives will no longer be available.
