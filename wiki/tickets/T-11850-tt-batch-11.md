---
id: T-11850
title: TT batch 11: one long Opus run per file group (two at a time)
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-11840-tt-batch-10]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Eleventh TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11840 (PR #34). Batch 10 showed that long
single-target Opus sessions convert most hard functions, so this batch gives one Opus agent per file group (about 90
minutes), two running at a time in separate worktrees, never two on the same C file. Each finished branch is merged into
`tt-batch11` before the next agent starts. Groups, in order:
1. `8014AF00.c`: `func_8014C5F8` (guarded), `func_8014C84C`
2. `801478D0.c`: `func_80147FD0` (guarded), `func_80147A18`
3. `80148210.c`: `func_80148764` (guarded)
4. `80148DB0.c`: `func_80148E7C`, `func_80149704`
5. `80133970.c`: `func_80134874`, `func_80134768`, `func_80134BBC`
6. `80142D40.c`: `func_801436BC`, `func_80144A04`, `func_801437F0`
7. `8013A040.c`: `func_8013A0DC`, then `80148DB0.c`: `func_8014A2B4`

## Acceptance criteria

- [ ] All seven groups run; matches committed one per commit, near misses guarded with `T-11850`
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
