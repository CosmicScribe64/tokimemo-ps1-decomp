---
id: T-11920
title: Clean redundant local worktrees while preserving game data
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal
Free local disk space at the user’s request. Preserve the main project, all game data, local commits, and uncommitted work.

## Acceptance criteria
- Verify compressed backups before removing clean worktrees.
- Omit game files from backups only when SHA-256 matches the retained main-project file.
- Keep dirty and locked worktrees; preserve commit references.

## Notes
Cleanup manifests and backups live in /Users/lila/Documents/Codex/2026-10-15/help-me-free-up-space-x20/outputs/.

## Results
Removed 35 clean registered worktrees and 4 abandoned folders with missing Git metadata. Retained all six dirty tokimemo-wt checkouts, the main project, and other agent worktrees. Preserved registered checkout commits under refs/cleanup-backup/2026-10-15/. Verified every archived regular file against SHA-256 before removal and rechecked 1,360 retained game files afterward. Backups occupy about 1.54 GiB. No code was changed; local maintenance documentation is ready for review.
