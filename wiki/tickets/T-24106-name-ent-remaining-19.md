---
id: T-24106
title: Close out the remaining 19 unmatched functions in NAME_ENT
status: In Progress
priority: high
type: code
created: 2026-10-18
updated: 2026-10-18
---

## Summary

Attack and close the remaining 19 `INCLUDE_ASM` functions (22,272 B) across `src/ovl/NAME_ENT/`:
1. `src/ovl/NAME_ENT/80132000.c` (2 functions):
   - `func_80138C1C` (740 B), `func_80138F60` (1,248 B)
2. `src/ovl/NAME_ENT/80144AD0.c` (4 functions):
   - `prologue_line_start` (344 B), `prologue_line_run` (780 B), `func_80145FB0` (296 B), `func_801460D8` (452 B)
3. `src/ovl/NAME_ENT/80139740.c` (12 functions):
   - `func_801398EC` (492 B), `func_80139F30` (2,712 B), `func_8013A9C8` (732 B), `func_8013AF78` (296 B), `func_8013B1D8` (836 B), `func_8013B994` (396 B), `func_8013BB24` (828 B), `func_8013CB44` (964 B), `func_8013DF88` (964 B), `func_8014068C` (1,240 B), `func_80140F90` (2,392 B), `func_80141820` (2,124 B)
4. `src/ovl/NAME_ENT/80142A80.c` (1 function):
   - `func_80143580` (4,436 B)
