---
id: T-24900
title: as1 global delay-slot filler: rule and source lever
status: In Review
assignee: claude-orchestrator
created: 2026-10-20
updated: 2026-10-20
links: ["[[as1-scheduler-rule]]", "[[as1-scheduling]]", "[[build-step-gaps]]", "[[playbook]]"]
---

## Goal

User 2026-10-20: agents report being blocked by "as1 hoist/scheduler". Eight guarded near misses differed only in as1 scheduling. Find what decides the hoist and a way to match.

## Result

- Rule of as1's cross-block delay-slot filler established by trace and confirmed on matched functions: [[as1-scheduler-rule]].
- Lever: declaring the globals whose `lui` must stay in the default arm `volatile` pins them (volatile accesses are never moved). Matches VALEN `func_80134030` and GYOZI `func_80139E4C` (FAKE: real declarations unknown).
- Not matched (notes updated): VALEN `func_80132D0C`, GEKO `func_80136C20` (block-weight limit), KANGEI `func_8013852C` (same-block order).
- Companion study T-24800 ([[as1-scheduling]]): per-file compile options do not explain the family; the original ran as1's cross-block scheduler.

## Comments
