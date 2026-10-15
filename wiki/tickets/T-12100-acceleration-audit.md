---
id: T-12100
title: Measure and reduce matching setup overhead
status: In Review
assignee: Codex acceleration investigation
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[tickets/T-11250-faster-candidate-scoring]]"]
---

## Goal

Find measured throughput gains without weakening byte verification or competing with the active cx matching run.

## Acceptance criteria

- [x] Measure a bounded experiment in the existing Docker image with one CPU.
- [x] Preserve exact generated compiler input for any setup optimization.
- [x] Record priorities, existing negative results, verification, and limitations.
- [ ] Review changes against CODING_STANDARDS.md before Done.

## Notes

Own worktree: `/private/tmp/tokimemo-acceleration`, branch `codex/acceleration-audit`, base `7a65b3d8`. Main and all existing matching worktrees remain read-only. No TT work, merges, pushes, rebases, resets, or stashes.

Existing T-11250 already caches stripping per draft and reports about 25x candidate throughput. T-11260 establishes that stripped and full-unit scores differ, including ranking; exact rescoring and linked verification remain necessary. Investigate setup shared across drafts rather than duplicate those changes.

## Comments

2026-10-15: Implemented offset-based stripping and reproducible tools/bench_strip.py. See [[acceleration-audit]] for measurements and prioritized next actions. All 63 comparisons across 21 files yielded identical compiler input; summed medians 14.633 s -> 0.800 s, 18.29x stripping speedup. 229 affected tests and header check passed in Docker. No new game matches claimed. Acceptance items for bounded measurement, input preservation, and findings are complete; moved to In Review for independent standards/spec checks.
