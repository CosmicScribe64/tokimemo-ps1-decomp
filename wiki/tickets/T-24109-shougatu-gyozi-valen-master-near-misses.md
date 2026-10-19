---
id: T-24109
title: Close low-score near misses in SHOUGATU, GYOZI, VALEN, and MASTER
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24101
  - T-24104
---

## Context

- **`SHOUGATU:func_80140DC0`** (`src/ovl/SHOUGATU/8013F190.c`, 404 B): `MATCH` (`score = 0` via `((DisplayActor44 *)D_8011ECD0)[96]` and pre-increment `++((DisplayActor44 *)D_8011ECD0)[96].unk_16 >= 3`, with `func_80141224`/`func_801413F4`/`func_801414E0`/`func_80141678` declared in `include/ovl/SHOUGATU.h`).
- **`SHOUGATU:func_8013D820`** (`src/ovl/SHOUGATU/8013D660.c`): `#ifdef NON_MATCHING` guard improved from `score=60` down to `score=13` (67/67 insns).
- **`SHOUGATU:func_80139334`** (`src/ovl/SHOUGATU/801390C0.c`): `#ifdef NON_MATCHING` guard improved from `score=40` down to `score=17` (120/120 insns).

## Acceptance criteria

- [x] Attack and close `SHOUGATU:func_80140DC0` to `score=0` (+404 B) and improve `#ifdef NON_MATCHING` guards in `SHOUGATU/{8013D660,801390C0}.c`
- [x] Promote `score=0` match to unguarded default C and verify `tools/funcdiff.py --resolve`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md`
