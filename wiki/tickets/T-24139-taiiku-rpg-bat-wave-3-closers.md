---
id: T-24139
title: Wave 3 closer sweep across TAIIKU (12 remaining) and RPG_BAT (8 remaining)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-20
depends_on:
  - T-24133
---

## Summary

Attack the remaining 12 unmatched functions in `TAIIKU` and 8 unmatched functions in `RPG_BAT` using the new `T-24131` `<expr> * 0` `ugen` subtree discard and `(v | 0) == C` `uoptinput` operand swap idioms:
- `TAIIKU` (12 remaining):
  - `src/ovl/TAIIKU/801452D0.c` (1 remaining — closing brings `801452D0.c` to 100.0%!): `func_80147B14`
  - `src/ovl/TAIIKU/80133C80.c` (5 remaining): `func_80134060`, `func_80134F98`, `func_80135F3C`, `func_80137784`, `func_80137C44`
  - `src/ovl/TAIIKU/80138950.c` (6 remaining): `func_80139AB0`, `func_8013DDB8`, `func_8013E1FC`, `func_8013E894`, `func_8013EC60`, `func_8013F3A4`
- `RPG_BAT` (8 remaining):
  - `src/ovl/RPG_BAT/801321D0.c` (1 remaining): `func_80132B34`
  - `src/ovl/RPG_BAT/80137280.c` (1 remaining): `func_80138324`
  - `src/ovl/RPG_BAT/801447E0.c` (2 remaining): `func_801447E0`, `func_80144C84`
  - `src/ovl/RPG_BAT/8014ADD0.c` (2 remaining): `func_8014BB80`, `func_8014BFE4`
  - `src/ovl/RPG_BAT/80154430.c` (2 remaining): `func_80154DD8`, `func_8015528C`

## Comments

- closing (2026-10-20): wave-3 sweep; its matches are on main (checkpoint 6d3d78ed8, reviewed: every new FAKE names its ticket, no inline asm, no guard over 30 rows). Remaining functions were consolidated into T-24142. Done.
