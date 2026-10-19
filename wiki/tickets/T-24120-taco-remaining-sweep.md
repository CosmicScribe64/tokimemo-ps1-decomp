---
id: T-24120
title: Sweep and close remaining unmatched functions in TACO (29 functions)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24107
---

## Summary

Attack and close as many of the 29 remaining `INCLUDE_ASM` functions in `src/ovl/TACO/` as possible, starting with the guarded and unguarded near misses:
- `src/ovl/TACO/80134930.c`: `func_8013506C` (now with `TacoLightBlock` `D_80122760` from `T-24107`)
- `src/ovl/TACO/80149F90.c`: `func_8014A480` (1 row), `func_8014F5F0` (6 rows), `func_8014AB50`, `func_8014CB24`
- `src/ovl/TACO/80151160.c` & `8015ABF0.c` & `8015BC10.c`: `func_801511C0` (5 rows), `func_8015B1B8` (8 rows), `func_8015C2CC`, `func_8015C718`
- `src/ovl/TACO/80133870.c`, `80138890.c`, `80139170.c`, `8013B170.c`, `8013B990.c`, `8013D1E0.c`, `8013DB80.c`, `8013F250.c`, `80141DE0.c`, `80143E80.c`, `801447D0.c`, `80146D60.c`
