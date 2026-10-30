---
id: T-39052
title: Remove the +2 RNG draws by opening iteration 650
status: Backlog
assignee: Claude (phase 4 root, Lane A)
created: 2026-10-30
updated: 2026-10-30
links: ["[[playtest]]", "[[tickets/T-39018-playtest-differential-harness]]"]
---

## Goal

The opening trace differential (WP-05) first diverges at `rand_calls` at iteration 650: native 32, port 34.

## Acceptance criteria

- [ ] The two extra port rand draws are traced to their call site.
- [ ] The opening trace differential shows no rand_calls divergence through iteration 650.

## Notes

Proposed by playtest-diff WP-05 (`wp05-opening-trace-v1/compare-v6`).

## Comments
