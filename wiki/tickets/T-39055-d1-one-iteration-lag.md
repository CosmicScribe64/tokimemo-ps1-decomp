---
id: T-39055
title: Remove the D1 one-iteration lag (T-39030) seen in the opening trace
status: Backlog
assignee: Claude (phase 4 root, Lane A)
created: 2026-10-30
updated: 2026-10-30
links: ["[[playtest]]", "[[tickets/T-39018-playtest-differential-harness]]"]
---

## Goal

D1 (T-39030) reproduces in the trace at iteration 4548 (8/0/2, native 128, port 63, one iteration late).

## Acceptance criteria

- [ ] The lag is fixed or explained, and the trace shows no D1 divergence.

## Notes

Proposed by playtest-diff WP-05; tracked against defect id T-39030.

## Comments
