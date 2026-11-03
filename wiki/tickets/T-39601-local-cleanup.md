---
id: T-39601
title: Remove merged worktrees and obsolete local build data
status: Done
assignee: Codex
created: 2026-11-03
updated: 2026-11-03
links: ["[[tickets/T-39600-recover-claude-session]]"]
---

## Goal

Leave a clean main checkout after merging the English patch. The owner authorized removing worktrees, old Docker resources and Claude session history.

## Acceptance criteria

- [x] Remove merged patch worktrees, disposable outputs and project session history.
- [x] Remove project preview containers, unused project images and build cache.
- [x] Preserve original game inputs and committed work; verify a clean main checkout.

## Review

All eleven local branch tips are included in main at bd4989e66. The remaining recovery and script notes were preserved by PR 71. Original game inputs are retained as an explicit exception to a fresh-clone layout. Other projects' sessions and Docker resources are outside the cleanup scope.

## Results

Removed all eight merged worktrees and ten local patch branches. The repository shrank from about 117 GB to 1.7 GB. Removed disposable root build output, generated assembly and Python caches, plus 863 MB of project-specific Claude history. The already-merged recovery-note backup stash was removed; the committed historical notes remain in main.

Stopped and removed the five project preview servers. Removed six unused tokimemo Docker images and all unused build cache (Docker reported 9.73 GB). Two small general-purpose images and eleven empty volumes remain; these are not project build output.

Preserved the original game archive, extracted original files and original disc tracks. These are the deliberate exception to a fresh clone. Main is the only worktree and local branch. Documentation and cleanup scope reviewed against CODING_STANDARDS.md; no source or raw game input was changed, and no unresolved findings remain.
