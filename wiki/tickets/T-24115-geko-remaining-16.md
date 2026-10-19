---
id: T-24115
title: Close remaining 16 unmatched functions in GEKO
status: Done
priority: high
type: code
created: 2026-10-18
updated: 2026-10-19
depends_on:
  - T-24107
---

## Summary

Attack and close as many of the 16 remaining `INCLUDE_ASM` functions in `src/ovl/GEKO/` as possible (starting from lowest score):
- `src/ovl/GEKO/80134D10.c`: `func_80136C20` (`score=4`), `func_801367EC` (`score=42`), `func_80138E10` (`score=75`), `func_801358CC` (`score=109`), `func_80135DC0` (`score=515`)
- `src/ovl/GEKO/801323E0.c`: `func_80133024` (`score=36`), `func_80132D04` (`score=114`), `func_80133308` (`score=140`), `func_80132B24` (`score=150`)
- `src/ovl/GEKO/80140A00.c`: `func_80141870` (`score=44`)
- `src/ovl/GEKO/80142760.c`: `func_80144198` (`score=57`), `func_80143F48` (`score=102`), `func_80142C9C` (`score=480`)
- `src/ovl/GEKO/80134040.c`: `func_80134808` (`score=80`)
- `src/ovl/GEKO/801338F0.c`: `func_80133C50` (`score=164`)
- `src/ovl/GEKO/80141AF0.c`: `func_801420E4` (`score=471`)

## Results

- Matched and promoted 4 functions (+2,024 B) in `GEKO`, bringing `src/ovl/GEKO/80140A00.c` to 100.0% matched (0 `INCLUDE_ASM`):
  - `GEKO:func_80141870` (268 B in `src/ovl/GEKO/80140A00.c`)
  - `GEKO:func_80138E10` (364 B in `src/ovl/GEKO/80134D10.c`)
  - `GEKO:func_80132D04` (800 B in `src/ovl/GEKO/801323E0.c`)
  - `GEKO:func_80143F48` (592 B in `src/ovl/GEKO/80142760.c`)
- Code review passed against `CODING_STANDARDS.md`; full local CI check suite passed.
