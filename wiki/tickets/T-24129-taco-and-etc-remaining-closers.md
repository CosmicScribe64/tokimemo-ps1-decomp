---
id: T-24129
title: Close remaining near misses in TACO and ETC (wave 2)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24120
  - T-24121
---

## Summary

Attack the remaining unmatched functions in `src/ovl/TACO/` and `src/ovl/ETC/`.

## Results (+6 functions, +6,576 B linked / +6,028 B asm; `src/ovl/ETC/80142B80.c`, `src/ovl/ETC/801492C0.c`, `src/ovl/TACO/80133870.c`, and `src/ovl/TACO/801447D0.c` at 100.0% matched!)

- `ETC:func_80132FB0` (`src/ovl/ETC/80132880.c`, 832 B): `MATCH`
- `ETC:func_8013CF2C` (`src/ovl/ETC/8013BEF0.c`, 824 B): `MATCH`
- `ETC:func_80142B80` (`src/ovl/ETC/80142B80.c`, 1,728 B linked / 1,404 B asm — **`src/ovl/ETC/80142B80.c` 100.0% matched**): `MATCH`
- `ETC:func_80149F48` (`src/ovl/ETC/801492C0.c`, 568 B — **`src/ovl/ETC/801492C0.c` 100.0% matched**): `MATCH`
- `TACO:func_80133E98` (`src/ovl/TACO/80133870.c`, 1,076 B linked / 884 B asm — **`src/ovl/TACO/80133870.c` 100.0% matched**): `MATCH`
- `TACO:func_80146754` (`src/ovl/TACO/801447D0.c`, 1,548 B linked / 1,516 B asm): `MATCH` (with `keep src/ovl/TACO/801447D0.c D_800E7384 proof=func_80146754:156:5e8407a9` in `config/migrate_globals.txt`)
- Added `#ifdef NON_MATCHING` guards for `ETC:func_80140AE4` (`src/ovl/ETC/8013FF80.c`, 20 rows) and `ETC:func_80144DF0` (`src/ovl/ETC/80143240.c`, 26 rows).
- Remaining `TACO` (21) and `ETC` (12) functions continue in [[tickets/T-24136-taco-and-etc-wave-3-closers]].

## Review (`CODING_STANDARDS.md`)

- Verified `check_headers.py`, `migrate_globals.py --check`, `ninja build/ovl/ETC.ok build/ovl/TACO.ok`, and `funcdiff.py --resolve`. Zero findings.
