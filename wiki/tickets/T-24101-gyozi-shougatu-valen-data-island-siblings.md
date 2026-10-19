---
id: T-24101
title: Add .data islands for GYOZI/801418B0, SHOUGATU/8013F190, and VALEN/80132760 and match 5-function sibling cluster (4,404 B)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24096
  - T-24098
---

## Context

The [[tickets/T-24096-speedup-audit-and-automation]] `.data` island audit (`build/scratch/t24096/audit_speedups.py`) identified a 3-way sibling cluster across `src/ovl/GYOZI/801418B0.c`, `src/ovl/SHOUGATU/8013F190.c`, and `src/ovl/VALEN/80132760.c` where the target binary shares `lui $at, %hi(D_...)` across `.data` variables in the same object, plus a second `GYOZI`/`SHOUGATU` sibling pair in the same files:

1. **`.data`-island 3-way sibling cluster (3,480 B total):**
   - `GYOZI:func_801421B4` (1,088 B in `src/ovl/GYOZI/801418B0.c`)
   - `SHOUGATU:func_8013FB28` (1,152 B in `src/ovl/SHOUGATU/8013F190.c`)
   - `VALEN:func_80132834` (1,240 B in `src/ovl/VALEN/80132760.c`)
2. **Companion `GYOZI`/`SHOUGATU` sibling pair in the same files (924 B total):**
   - `GYOZI:func_801428EC` (416 B in `src/ovl/GYOZI/801418B0.c`, the only other unmatched function in `801418B0.c`)
   - `SHOUGATU:func_80140314` (508 B in `src/ovl/SHOUGATU/8013F190.c`)

## Results

- Configured `.data` islands in `config/overlays/{GYOZI,SHOUGATU,VALEN}.yaml`, `src/ovl/{GYOZI/801418B0,SHOUGATU/8013F190,VALEN/80132760}.c`, and `include/ovl/{GYOZI,SHOUGATU,VALEN}.h`.
- **Matched & promoted (+2 functions, +924 B):**
  - `GYOZI:func_801428EC` (416 B in `src/ovl/GYOZI/801418B0.c`)
  - `SHOUGATU:func_80140314` (508 B in `src/ovl/SHOUGATU/8013F190.c`)
- Archived `#ifdef NON_MATCHING` drafts for `GYOZI:func_801421B4` (`score=54`, first 231/272 insns exact), `SHOUGATU:func_8013FB28` (`score=61`, first 231/288 insns exact), `VALEN:func_80132834` (`score=51`, 308/310 insns), and `VALEN:func_80133088` (`score=171`).

## Acceptance criteria

- [x] Configure `tools/data_island.py` in `config/overlays/{GYOZI,SHOUGATU,VALEN}.yaml` and define the required `.data` variables in C where needed
- [x] Close as many of the 5 functions (`func_801421B4`, `func_8013FB28`, `func_80132834`, `func_801428EC`, `func_80140314`, 4,404 B total) as possible to `score=0` in `build/scratch/t24101/`
- [x] Promote all `score=0` functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`
