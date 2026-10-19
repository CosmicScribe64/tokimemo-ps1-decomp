---
id: T-24124
title: Sweep remaining uninvestigated functions in TAIIKU and RPG_BAT
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24117
  - T-24118
---

## Summary

Attack and close the remaining uninvestigated `INCLUDE_ASM` functions in `TAIIKU` and `RPG_BAT`:
- **`TAIIKU`**:
  - `src/ovl/TAIIKU/80133C80.c`: `func_80134060`, `func_80134F98`, `func_80135F3C`, `func_80137784`, `func_80137C44`
  - `src/ovl/TAIIKU/80138950.c`: `func_80139AB0`, `func_8013B3B0`, `func_8013B78C`, `func_8013B94C`, `func_8013D2B4`, `func_8013D788`, `func_8013DDB8`, `func_8013E1FC`, `func_8013E894`, `func_8013EC60`, `func_8013F3A4`
  - `src/ovl/TAIIKU/801452D0.c`: `func_80147B14`
- **`RPG_BAT`**:
  - `src/ovl/RPG_BAT/80134D30.c`: `func_80134E78`
  - `src/ovl/RPG_BAT/801447E0.c`: `func_801447E0`, `func_80144C84`
  - `src/ovl/RPG_BAT/8014ADD0.c`: `func_8014ADD0`, `func_8014B294`, `func_8014BB80`
  - `src/ovl/RPG_BAT/801525B0.c`: `func_80152F30`

## Results

- Matched **6 functions (+6,388 B / 6,396 B linked)** across `TAIIKU` and `RPG_BAT` — **`src/ovl/RPG_BAT/80134D30.c` and `src/ovl/RPG_BAT/801525B0.c` brought to 100.0% matched**:
  - `TAIIKU:func_8013B3B0` (528 B in `src/ovl/TAIIKU/80138950.c`)
  - `TAIIKU:func_8013B94C` (496 B in `src/ovl/TAIIKU/80138950.c`)
  - `RPG_BAT:func_80134E78` (1,248 B in `src/ovl/RPG_BAT/80134D30.c` — `80134D30.c` 100.0% matched!)
  - `RPG_BAT:func_8014ADD0` (1,212 B / 1,220 B linked in `src/ovl/RPG_BAT/8014ADD0.c`)
  - `RPG_BAT:func_8014B294` (1,188 B in `src/ovl/RPG_BAT/8014ADD0.c`)
  - `RPG_BAT:func_80152F30` (1,716 B in `src/ovl/RPG_BAT/801525B0.c` — `801525B0.c` 100.0% matched!)

