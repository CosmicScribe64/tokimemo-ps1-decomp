---
id: T-24133
title: Wave 2 closer sweep across TAIIKU (21 remaining) and RPG_BAT (8 remaining)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24124
---

## Summary

Attack the remaining 21 unmatched functions in `TAIIKU` and 8 unmatched functions in `RPG_BAT` (wave 2).

## Results (+9 functions, +6,064 B in `TAIIKU`; `src/ovl/TAIIKU/80142240.c` at 100.0% matched!)

- `TAIIKU:func_80135480` (`src/ovl/TAIIKU/80133C80.c`, 608 B): `MATCH`
- `TAIIKU:func_801365C8` (`src/ovl/TAIIKU/80133C80.c`, 752 B): `MATCH`
- `TAIIKU:func_80137544` (`src/ovl/TAIIKU/80133C80.c`, 576 B): `MATCH`
- `TAIIKU:func_8013B78C` (`src/ovl/TAIIKU/80138950.c`, 448 B): `MATCH`
- `TAIIKU:func_8013BCEC` (`src/ovl/TAIIKU/80138950.c`, 524 B): `MATCH`
- `TAIIKU:func_8013D2B4` (`src/ovl/TAIIKU/80138950.c`, 1,236 B + `.data` `s16 D_80149958 = 0;`): `MATCH`
- `TAIIKU:func_8013D788` (`src/ovl/TAIIKU/80138950.c`, 556 B): `MATCH`
- `TAIIKU:func_8014357C` (`src/ovl/TAIIKU/80142240.c`, 664 B — **`src/ovl/TAIIKU/80142240.c` 100.0% matched**): `MATCH`
- `TAIIKU:func_8014607C` (`src/ovl/TAIIKU/801452D0.c`, 700 B): `MATCH`
- Remaining `TAIIKU` (12) and `RPG_BAT` (8) functions continue in [[tickets/T-24139-taiiku-rpg-bat-wave-3-closers]].

## Review (`CODING_STANDARDS.md`)

- Verified `check_headers.py`, `ninja build/ovl/TAIIKU.ok build/ovl/RPG_BAT.ok`, and `funcdiff.py --resolve`. Zero findings.
