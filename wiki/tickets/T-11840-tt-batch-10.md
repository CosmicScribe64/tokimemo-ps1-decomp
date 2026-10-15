---
id: T-11840
title: TT batch 10: one long Opus run per large function (two at a time), bulk_m2c with a long autotweak budget
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-11830-tt-wave1-handoff]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Tenth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11830 (PR #33). TT is ours including the
former wave-1 files. Every remaining TT function has failed at least once; the twelve large ones (1.1-2.2 KB) hold about
half the remaining bytes.
- Track 1: one Opus agent per large function, about 90 minutes each, two running at a time in separate worktrees (never
  two on the same C file); each finished branch is merged into `tt-batch10` before the next agent starts. Order:
  `func_80146348`, `func_8014A2B4`, `func_80143A44`, `func_80144D70`, `func_801441BC`, `func_80141450`, `func_801400B0`,
  `func_80137C20`, `func_8013230C`, `func_8013D988`, `func_80136C64`, `func_801455C8`.
- Track 3: `tools/bulk_m2c.py --unit TT --exclude ""` in its own worktree with `--time 900` and `--max-size 1100` (the
  mid-size plain-asm functions; the large ones are track 1's).

## Acceptance criteria

- [ ] Track 1 run over the list; matches committed one per commit, near misses guarded with `T-11840`
- [ ] Track 3 run; verified matches committed
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
