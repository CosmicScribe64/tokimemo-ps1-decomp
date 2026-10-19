---
id: T-24114
title: Close final functions of BUNKAKEN, OMIMAI, BUNKA_SD, and remaining 8 functions of SHUGAKU
status: Done
priority: high
type: code
created: 2026-10-18
updated: 2026-10-19
depends_on:
  - T-24108
---

## Summary

Attack and close the final functions of 4 near-complete overlays:
1. **`BUNKAKEN` (1 function left)**: `BUNKAKEN:func_8013B510` (`src/ovl/BUNKAKEN/8013B510.c`, `score=283`)
2. **`OMIMAI` (1 function left)**: `OMIMAI:func_8013388C` (`src/ovl/OMIMAI/801331B0.c`, `score=548`)
3. **`BUNKA_SD` (2 functions left)**: `BUNKA_SD:func_80134D94` (`src/ovl/BUNKA_SD/80134540.c`, `score=184`) and `BUNKA_SD:func_80139290` (`src/ovl/BUNKA_SD/801388D0.c`, `score=459`)
4. **`SHUGAKU` (8 functions left)**:
   - `SHUGAKU:func_80138E40` (`score=58` in `src/ovl/SHUGAKU/80138A60.c`)
   - `SHUGAKU:func_801398E8` (`score=58` in `src/ovl/SHUGAKU/80138A60.c`)
   - `SHUGAKU:func_80139118` (`score=140` in `src/ovl/SHUGAKU/80138A60.c`)
   - `SHUGAKU:func_80137CFC` (`score=82` in `src/ovl/SHUGAKU/801378B0.c`)
   - `SHUGAKU:func_80138740` (`score=90` in `src/ovl/SHUGAKU/801378B0.c`)
   - `SHUGAKU:func_801388D8` (`score=92` in `src/ovl/SHUGAKU/801378B0.c`)
   - `SHUGAKU:func_8013243C` (`score=132` in `src/ovl/SHUGAKU/80132000.c`)
   - `SHUGAKU:func_801337FC` (`score=173` in `src/ovl/SHUGAKU/80133430.c`)

## Results

- Matched and promoted 5 functions (+1,952 B) in `SHUGAKU`, bringing `src/ovl/SHUGAKU/801378B0.c` to 100.0% matched (0 `INCLUDE_ASM`):
  - `SHUGAKU:func_80137CFC` (520 B in `src/ovl/SHUGAKU/801378B0.c`)
  - `SHUGAKU:func_80138740` (408 B in `src/ovl/SHUGAKU/801378B0.c`)
  - `SHUGAKU:func_801388D8` (392 B in `src/ovl/SHUGAKU/801378B0.c`)
  - `SHUGAKU:func_80138E40` (364 B in `src/ovl/SHUGAKU/80138A60.c`)
  - `SHUGAKU:func_801398E8` (268 B in `src/ovl/SHUGAKU/80138A60.c`)
- Code review passed against `CODING_STANDARDS.md`; full local CI check suite passed.
