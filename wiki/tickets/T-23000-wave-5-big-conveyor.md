---
id: T-23000
title: Wave 5 biggest-first conveyor alongside wave 4
status: In Progress
assignee: claude-orchestrator
created: 2026-10-17
updated: 2026-10-17
links: [[tickets/T-22000-wave-4-conveyor]], [[build-step-gaps]]
---

## Goal

User decision 2026-10-17: run a second conveyor next to [[tickets/T-22000-wave-4-conveyor]] that takes the biggest functions first, for big byte wins. Wave 4 ranks packages by expected matched bytes per agent-hour; this one ranks by the size of the largest remaining function. 27 file-disjoint packages (T-23001..T-23027), 89 functions / 134,984 bytes, every seed at least 1,500 bytes. Functions wave 4 attempted this week are left out. Wave 4 keeps the files of its first 60 packages; the 26 unstarted wave 4 packages whose files a seed needed (w4-p65 and later) are absorbed whole and refused to wave 4. Two workers; Sonnet matcher, Opus closer for near misses of 30 rows or fewer, Sonnet review and locked merge. New packages stop at weekly 92 percent, as in wave 4.

## Acceptance criteria

- [ ] Every started package reviewed and merged or dropped, ticket Done
- [ ] Results recorded here

## Comments
