---
id: T-24520
title: Coordinate TAIIKU RPG_BAT and seasonal matching lanes
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[status]]", "[[playbook]]"]
---

## Goal

Find the fastest verified remaining matches in three isolated, file-disjoint lanes: TAIIKU T-24521, RPG_BAT T-24522, GYOZI/VALEN/SHOUGATU T-24523. Main integration stays with the originating chat.

## Acceptance criteria

- [ ] Return pinned focused commits for any default-C matches, with private linked proof, all 27 binary SHA1 checks, declaration guards and standards review.
- [ ] Record useful negative findings without claiming coverage.

## Notes

Baseline dba33c4f5 (source e51680da0): 6,798/6,952 game functions and 91.0% bytes. Three user-authorized Sol 6.1 medium workers own 33 remaining functions. Worktrees live under /private/tmp; tools run in the existing Docker image with one compiler job per worker and shared raw inputs read-only. No coordinator compiler grids, integration-tree edits, pushes or GitHub comments.

Exact/near/lenient/wider-shape automatic copying was already exhausted by the parent. Seasonal scheduler negatives T-24211/T-24212/T-24301/T-24303 and T-24029 must be read before proposing new source forms. New typed reconstruction, loop and aggregate hypotheses take priority over repeated grids.

## Comments

2026-10-20: Read wiki index, status, playbook, standards, local orchestration guidance. Dispatched three isolated workers. No new match claimed.

### Verified handoff checkpoints

| Lane | Function | Bytes | Pinned commit |
| --- | --- | ---: | --- |
| RPG_BAT | func_8014BB80 | 1,124 | bf7e243bd214f8e52a4d81f130776c1c4638de36 |
| SHOUGATU | func_80133B44 | 408 | c182f981586a85cebf11675dea9ea0d731d75ff1 |
| TAIIKU | func_80134060 | 1,348 | f278c1b6cbdc8793c0175ae41d6acc364bc1140d |
| RPG_BAT | func_801447E0 | 924 | 66f4bbe19 (parent bf7e243bd) |
| TAIIKU | func_80134F98 | 1,256 | 263294a8f6971a79d86544c8072a47053b77982c (parent f278c1b6c) |

All five compile as literal default C and match privately linked target bytes, totaling 5,060 bytes. Each passes 27 binary SHA1 checks using its private modified overlay plus 26 unchanged root outputs mounted read-only; the worker tickets state that provenance. Headers, globals and prototype checks pass. Parent source review found no blocking findings after RPG_BAT indentation repair. The originating chat integrated the first RPG_BAT match at d2665864e; workers continue. No global progress total is changed in this isolated coordinator tree.
