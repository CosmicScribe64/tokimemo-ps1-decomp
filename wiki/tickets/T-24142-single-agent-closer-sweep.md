---
id: T-24142
title: Single-agent closer sweep (post-quota restart, consolidates T-24132 and T-24136..T-24141)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24141
---

## Summary

The seven-agent pool died on the individual quota limit. Work continues with one Sonnet 5.5 subagent managed by the orchestrator. It owns every file under `src/` and `include/` (no parallel writers), working one function at a time and handing back small verified batches.

Scope: the ~183 remaining `INCLUDE_ASM` functions (`TACO` 20, `ETC` 12, `GEKO` 12, `TAIIKU` 11, `GYOZI` 8, `RPG_BAT` 7, `OPTION` 6, `SHOUGATU` 6, `MASTER` 6, `ENDING` 5, `NAME_ENT` 4, `KANGEI` 4, `VALEN` 4, `BUNKA_SD` 2, `EN_NICHI` 2, `TEL` 2, plus `src/main/`), starting with the lowest-score near misses.

Supersedes the open wave-3 tickets T-24132 and T-24136..T-24141 (they stay In Progress until a review of the checkpoint commit `6d3d78ed8` finishes).
