---
id: T-24116
title: Close mid-score batch in GYOZI and SHOUGATU (score 62..252)
status: Done
priority: high
type: code
created: 2026-10-18
updated: 2026-10-19
depends_on:
  - T-24109
---

## Summary

Attack and close the mid-score (`score=62..252`) remaining functions in `GYOZI` and `SHOUGATU`:
- **`GYOZI`**:
  - `GYOZI:func_80143160` (`score=78` in `src/ovl/GYOZI/80143110.c`)
  - `GYOZI:func_8013A5F4` (`score=86` in `src/ovl/GYOZI/8013A140.c`)
  - `GYOZI:func_801347D4` (`score=110` in `src/ovl/GYOZI/80134520.c`)
  - `GYOZI:func_80140464` (`score=111` in `src/ovl/GYOZI/801400D0.c`)
  - `GYOZI:func_80140E0C` (`score=136` in `src/ovl/GYOZI/80140C90.c`)
  - `GYOZI:func_801393C8` (`score=185`), `GYOZI:func_8013907C` (`score=205`), `GYOZI:func_80138C9C` (`score=252`) in `src/ovl/GYOZI/80138B50.c`
- **`SHOUGATU`**:
  - `SHOUGATU:func_80137664` (`score=62`) and `SHOUGATU:func_8013737C` (`score=208`) in `src/ovl/SHOUGATU/80136F90.c`
  - `SHOUGATU:func_8013D92C` (`score=70`) and `SHOUGATU:func_8013DAAC` (`score=129`) in `src/ovl/SHOUGATU/8013D660.c`
  - `SHOUGATU:func_80133B44` (`score=90` in `src/ovl/SHOUGATU/801330C0.c`)
  - `SHOUGATU:func_8013288C` (`score=110` in `src/ovl/SHOUGATU/80132550.c`)
  - `SHOUGATU:func_8013BA7C` (`score=123` in `src/ovl/SHOUGATU/8013BA00.c`)

## Results

- Matched and promoted 2 functions (+812 B) in `GYOZI`:
  - `GYOZI:func_8013A5F4` (376 B in `src/ovl/GYOZI/8013A140.c`, bringing `8013A140.c` to 100.0% matched)
  - `GYOZI:func_80143160` (436 B in `src/ovl/GYOZI/80143110.c`, bringing `80143110.c` to 100.0% matched)
- Added 13 improved `#ifdef NON_MATCHING` guards across `src/ovl/GYOZI/{80134520,80138B50,801400D0,80140C90}.c` and `src/ovl/SHOUGATU/{80132550,801330C0,80136F90,8013BA00,8013D660}.c`.
- Code review passed against `CODING_STANDARDS.md`; full local CI check suite passed.
