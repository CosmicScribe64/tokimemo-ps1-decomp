---
id: T-24142
title: Single-agent closer sweep (post-quota restart, consolidates T-24132 and T-24136..T-24141)
status: Backlog
priority: high
type: code
created: 2026-10-19
updated: 2026-10-20
depends_on:
  - T-24141
---

## Summary

The seven-agent pool died on the individual quota limit. Work continues with one Sonnet 5.5 subagent managed by the orchestrator. It owns every file under `src/` and `include/` (no parallel writers), working one function at a time and handing back small verified batches.

Scope: the ~183 remaining `INCLUDE_ASM` functions (`TACO` 20, `ETC` 12, `GEKO` 12, `TAIIKU` 11, `GYOZI` 8, `RPG_BAT` 7, `OPTION` 6, `SHOUGATU` 6, `MASTER` 6, `ENDING` 5, `NAME_ENT` 4, `KANGEI` 4, `VALEN` 4, `BUNKA_SD` 2, `EN_NICHI` 2, `TEL` 2, plus `src/main/`), starting with the lowest-score near misses.

Supersedes the open wave-3 tickets T-24132 and T-24136..T-24141 (they stay In Progress until a review of the checkpoint commit `6d3d78ed8` finishes).

## Status (2026-10-19, paused by the user)

First promoted match: `OPTION:func_80139F8C` (132 B, `src/ovl/OPTION/801389A0.c`, guard removed, funcdiff MATCH, full build 27/27 SHA1). The Sonnet 5.5 subagent (`6459fa57`) and the 30-minute timer were stopped on request. Resume by spawning a fresh single subagent on this ticket; remaining counts in the Summary. The older wave-3 tickets (T-24132, T-24136..T-24141) are still In Progress and need the standards review of checkpoint `6d3d78ed8` before Done.

## Comments

- closing (2026-10-20): paused by the user on 2026-10-19 and not resumed. Left: the remaining wave-3 functions listed in the Summary (those still INCLUDE_ASM in src/); first promoted match OPTION func_80139F8C is on main. Backlog.
