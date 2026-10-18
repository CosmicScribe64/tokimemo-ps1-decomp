---
id: T-24095
title: Match GEKO and EVENT batch D (func_8013A3C4, func_8013AB4C, func_8013B10C, func_801404FC, func_80134808, func_801035BC)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following [[tickets/T-24090-geko-event-twins-and-near-misses-c]] (where all 5 targets matched for +2,308 B), target the next `GEKO` and `EVENT` functions:

1. `GEKO:func_8013A3C4` (416 B in `src/ovl/GEKO/80139910.c`, draft in `drafts/T-17700/GEKO/func_8013A3C4/body.c`)
2. `GEKO:func_8013AB4C` (404 B in `src/ovl/GEKO/80139910.c`, draft in `drafts/T-17700/GEKO/func_8013AB4C/body.c`)
3. `GEKO:func_8013B10C` (416 B in `src/ovl/GEKO/8013AF80.c`, draft in `drafts/T-17700/GEKO/func_8013B10C/body.c`)
4. `GEKO:func_801404FC` (464 B in `src/ovl/GEKO/8013F360.c`, draft in `drafts/T-17700/GEKO/func_801404FC/body.c`)
5. `GEKO:func_80134808` (in `src/ovl/GEKO/80134040.c`, draft in `drafts/T-17700/GEKO/func_80134808/body.c`)
6. `EVENT:func_801035BC` (344 B in `src/ovl/EVENT/801024B0.c`, draft in `drafts/T-14500/EVENT/func_801035BC/body.c`)

## Acceptance criteria

- [ ] Close as many of the 6 targets as possible to `score=0` in `build/scratch/t24095/`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md`
