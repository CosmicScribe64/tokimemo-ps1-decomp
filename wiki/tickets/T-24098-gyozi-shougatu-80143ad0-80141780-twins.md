---
id: T-24098
title: Match GYOZI/80143AD0 and SHOUGATU/80141780 1.000 twin cluster (10 functions, 8,544 B)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

`src/ovl/GYOZI/80143AD0.c` and `src/ovl/SHOUGATU/80141780.c` contain a massive 5-pair (10-function, **8,544 B**) `1.000` / `0.995` opcode-skeleton twin cluster:

1. `GYOZI:func_80145084` (716 B) & `SHOUGATU:func_80142DA0` (716 B) — **1,432 B** (`1.000` twins; matched sibling `EVENT:func_800FAA04` / `GEKO:func_8013A820` at ratio 0.765)
2. `GYOZI:func_8014494C` (296 B) & `SHOUGATU:func_8014265C` (296 B) — **592 B** (`1.000` twins)
3. `GYOZI:func_801444CC` (752 B) & `SHOUGATU:func_801421D4` (760 B) — **1,512 B** (`0.995` twins)
4. `GYOZI:func_80144018` (1,204 B) & `SHOUGATU:func_80141D20` (1,204 B) — **2,408 B** (`1.000` twins)
5. `GYOZI:func_80144B70` (1,300 B) & `SHOUGATU:func_8014288C` (1,300 B) — **2,600 B** (`1.000` twins)

Solving each `GYOZI` function immediately ports to its `SHOUGATU` twin.

## Acceptance criteria

- [ ] Close as many of the 5 twin pairs (10 functions, 8,544 B) as possible to `score=0` in `build/scratch/t24098/`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`
