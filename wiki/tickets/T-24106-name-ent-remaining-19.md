---
id: T-24106
title: Close out the remaining 19 unmatched functions in NAME_ENT
status: Done
priority: high
type: code
created: 2026-10-18
updated: 2026-10-18
---

## Summary

Attacked the remaining 19 `INCLUDE_ASM` functions in `src/ovl/NAME_ENT/` and closed **8 of 19 functions (+4,772 B linked / +4,988 B asm)**, bringing **`src/ovl/NAME_ENT/80144AD0.c` to 100.0% matched (4/4 remaining functions closed)**:

1. **`src/ovl/NAME_ENT/80144AD0.c` (4/4 matched — 100.0% matched!)**:
   - `NAME_ENT:prologue_line_start` (328 B linked / 344 B asm): `MATCH`
   - `NAME_ENT:prologue_line_run` (780 B): `MATCH`
   - `NAME_ENT:func_80145FB0` (296 B): `MATCH`
   - `NAME_ENT:func_801460D8` (432 B linked / 452 B asm): `MATCH`
2. **`src/ovl/NAME_ENT/80139740.c` (4/12 matched)**:
   - `NAME_ENT:func_8013B994` (380 B linked / 396 B asm): `MATCH`
   - `NAME_ENT:func_8013BB24` (756 B linked / 828 B asm): `MATCH`
   - `NAME_ENT:func_8013CB44` (900 B linked / 964 B asm): `MATCH`
   - `NAME_ENT:func_8013DF88` (900 B linked / 964 B asm): `MATCH`
3. **Remaining 11 functions in `NAME_ENT` (live-scored for follow-up)**:
   - `80139740.c`: `func_8013AF78` (`score=105`), `func_8014068C` (`score=191`), `func_801398EC` (`score=253`), `func_8013B1D8` (`score=254`), `func_8013A9C8` (`score=266`), `func_80140F90` (`score=561`), `func_80139F30` (`score=577`), `func_80141820` (`score=599`)
   - `80132000.c`: `func_80138C1C` (`score=242`), `func_80138F60` (`score=665`)
   - `80142A80.c`: `func_80143580` (`score=1683`)
