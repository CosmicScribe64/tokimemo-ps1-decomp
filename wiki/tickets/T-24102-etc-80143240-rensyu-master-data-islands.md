---
id: T-24102
title: Match ETC/80143240 remaining functions and add RENSYU/80132040 and MASTER/80139000 .data islands
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24094
  - T-24096
---

## Context

Following [[tickets/T-24094-etc-data-island-and-near-complete-overlays]] (which added the `.data` island for `ETC/80143240` and brought `DATE2` and `BUNKASAI` to 100.0% matched):

1. **`src/ovl/ETC/80143240.c` remaining functions** (`func_80143A88`, `func_80143758`, and any other unmatched functions in `80143240.c`):
   - `ETC:func_80143A88` was flagged by [[tickets/T-24096-speedup-audit-and-automation]] (`build/scratch/t24096/audit_speedups.py`) as sharing `lui $at` across `.data` variables in `ETC/80143240.data`. Replacing its `INCLUDE_RODATA` `.data` lines with C definitions in `src/ovl/ETC/80143240.c` unlocks those shared-`%hi` relocations.
2. **Remaining 2 overlay `.data` islands from `T-24096`**:
   - **`RENSYU/80132040`** (`config/overlays/RENSYU.yaml`, `src/ovl/RENSYU/80132040.c`): configure `tools/data_island.py` for `RENSYU/80132040` and target `RENSYU:func_8013325C` (1,012 B) and `RENSYU:func_80133628` (404 B, guarded at 4 diff rows).
   - **`MASTER/80139000`** (`config/overlays/MASTER.yaml`, `src/ovl/MASTER/80139000.c`): configure `tools/data_island.py` for `MASTER/80139000` and target `MASTER:func_80139CC8` (1,076 B) and any companion unmatched functions in `80139000.c`.

## Acceptance criteria

- [x] Close as many of the target functions in `src/ovl/ETC/80143240.c`, `src/ovl/RENSYU/80132040.c`, and `src/ovl/MASTER/80139000.c` as possible to `score=0` in `build/scratch/t24102/`
- [x] Promote all `score=0` functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Result

5 functions (5,480 B) matched and promoted to unguarded default C:
- `ETC:func_80143758` (292 B in `src/ovl/ETC/80143240.c`)
- `ETC:func_80143968` (288 B in `src/ovl/ETC/80143240.c`)
- `ETC:func_80143A88` (1,180 B in `src/ovl/ETC/80143240.c`)
- `RENSYU:func_8013325C` (972 B in `src/ovl/RENSYU/80132040.c`)
- `MASTER:func_80139CC8` (2,748 B in `src/ovl/MASTER/80139000.c`)

