---
id: T-12020
title: Match cx2 TAIIKU functions
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal

Deep matching pass over fifteen cx2 functions in src/ovl/TAIIKU/80133C80.c.

## Acceptance criteria

- [ ] Matches verified by exact linked funcdiff and unit SHA1.
- [ ] Retained drafts guarded with measured differences.
- [ ] Clean build, prototype checks and standards review.

## Notes

Ownership: src/ovl/TAIIKU/80133C80.c plus include/config/wiki. Work cutoff 21:45 UTC.

## Comments

- 2026-10-15: Baseline configure plus ninja completed: 27/27 SHA1, headers OK, globals OK. Read wiki, workflow, cheat sheet, standards and local instructions. Known cases include alias hoisting, counter scheduling and decrement register allocation.
