---
id: T-23000
title: Wave 5 biggest-first conveyor alongside wave 4
status: Done
assignee: claude-orchestrator
created: 2026-10-17
updated: 2026-10-17
links: [[tickets/T-22000-wave-4-conveyor]], [[build-step-gaps]]
---

## Goal

User decision 2026-10-17: run a second conveyor next to [[tickets/T-22000-wave-4-conveyor]] that takes the biggest functions first, for big byte wins. Wave 4 ranks packages by expected matched bytes per agent-hour; this one ranks by the size of the largest remaining function. 27 file-disjoint packages (T-23001..T-23027), 89 functions / 134,984 bytes, every seed at least 1,500 bytes. Functions wave 4 attempted this week are left out. Wave 4 keeps the files of its first 60 packages; the 26 unstarted wave 4 packages whose files a seed needed (w4-p65 and later) are absorbed whole and refused to wave 4. Two workers; Sonnet matcher, Opus closer for near misses of 30 rows or fewer, Sonnet review and locked merge. New packages stop at weekly 92 percent, as in wave 4.

## Acceptance criteria

- [x] Every started package reviewed and merged or dropped, ticket Done
- [x] Results recorded here

## Comments

- 2026-10-17 result: stopped at the user's request when the weekly usage limit was reached. 6 of 27 packages merged (w5-big-p01..p06, every started package reviewed, ticket Done), 2 functions matched (GEKO func_80144270, main load_tpage_buf_lock); the biggest functions (TEL func_80136724, ENDING func_8013A2E8, main func_8006D7EC, EVENT func_8011DDB8) stay unmatched, notes in [[matching-notes]]. Done.
