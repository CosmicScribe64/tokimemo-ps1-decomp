---
id: T-24121
title: Sweep and close remaining unmatched functions in ETC (21 functions)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24107
---

## Summary

Attack and close as many of the 21 remaining `INCLUDE_ASM` functions in `src/ovl/ETC/` as possible, starting from the lowest-score drafts:
- `src/ovl/ETC/8013FF80.c`: `func_8013FFAC` (5 rows), `func_80140AE4`
- `src/ovl/ETC/80143240.c`: `func_80144DF0` (13 rows), `func_80144224`, `func_80144F74`, `func_80145C00`
- `src/ovl/ETC/80142B80.c`: `func_80142B80` (39 rows)
- `src/ovl/ETC/8013BEF0.c`: `func_8013CF2C` (48 rows), `func_8013CCE4`, `func_8013D570`, `func_8013F054`
- `src/ovl/ETC/80132880.c`: `func_801328A4`, `func_80132C3C`, `func_80132FB0`
- `src/ovl/ETC/80140F50.c`: `func_80141BE8`
- `src/ovl/ETC/80146400.c` & `801492C0.c`: `func_80146D4C`, `func_8014790C`, `func_801480D0`, `func_80148F7C`, `func_80149F48`, `func_8014A360`
