---
id: T-11870
title: TT batch 13: the last three guarded near misses, one long Opus run each
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-11860-tt-batch-12]]", "[[decompile-workflow]]", "[[data/t0018-cases]]", "[[uopt-register-allocation]]"]
---

## Goal
Thirteenth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11860 (PR #47): the three guarded TT
near misses, one Opus agent each with a long budget (about 120 minutes), two at a time in separate worktrees, with the
batch 11-12 findings as tips. `func_8014D260` (gcc) stays parked: it needs maspsx changes ([[gcc-objects]]).
1. `func_80142110` (`80142110.c`, 4 lines: an else-arm load lands in c instead of a ugen temp)
2. `func_801427A8` (`80142570.c`, 12 lines: three tied hoisted constants coloured in bit order)
3. `func_8014A2B4` (`80148DB0.c`, 28 lines: four constants in t2..t5 behave like uopt registers)

## Acceptance criteria

- [ ] All three run; matches committed one per commit, near misses stay guarded with updated notes (`T-11870`)
- [ ] Inline review against `CODING_STANDARDS.md` (FAKE comment on every fake trick, including every chain store)
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
