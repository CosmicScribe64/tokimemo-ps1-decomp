---
id: T-12400
title: Integrate verified matching branches on main
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal
Integrate completed matching branches and verified committed checkpoints directly onto remote main, as explicitly requested by the user. Preserve the original main checkout and active worker checkouts.

## Acceptance criteria
- [ ] Resolve source, header, and wiki conflicts without losing verified work.
- [ ] All 27 binary checksums, headers, globals, and branch prototypes pass on the combined tree.
- [ ] Push the validated integration commit to remote main without force.

## Notes
PR #33 was separately authorized and merged after independent full-game and seven-function literal verification. Integration starts at 1668c560. User explicitly superseded the original no-merge/no-push rule and declined draft PRs.

## Comments
