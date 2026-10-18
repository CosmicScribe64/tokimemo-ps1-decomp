---
id: T-22000
title: Wave 4 conveyor until weekly usage nears 95 percent
status: Done
assignee: claude-orchestrator
created: 2026-10-17
updated: 2026-10-17
links: [[tickets/T-16990-wave-3-conveyor]], [[build-step-gaps]]
---

## Goal

User decision 2026-10-17: keep matching until weekly usage nears 95 percent, conserving tokens. 122 file-disjoint packages (T-22001..), 426 functions / 356,688 bytes, from a fresh inventory that excludes TT, DATE2 and claimed functions. Sonnet matcher, Opus closer only for near misses of 15 rows or fewer, Sonnet review and locked merge. New packages stop at weekly 92 percent.

## Acceptance criteria

- [x] Every started package reviewed and merged or dropped, ticket Done
- [x] Results recorded here

## Comments

- 2026-10-17 result: stopped at the user's request when the weekly usage limit was reached. 29 packages merged (every started package reviewed, ticket Done), 40 functions matched; the 26 tail packages handed to [[tickets/T-23000-wave-5-big-conveyor]] were refused here and the rest never started. Main after both waves: 6,466/6,958 functions, 1,813,468/2,279,368 bytes (79.6%). Done.
