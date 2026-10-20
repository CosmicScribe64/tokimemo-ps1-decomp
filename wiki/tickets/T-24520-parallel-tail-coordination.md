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
