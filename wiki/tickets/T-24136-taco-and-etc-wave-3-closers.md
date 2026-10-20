---
id: T-24136
title: Wave 3 closer sweep across TACO (21) and ETC (12)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-20
depends_on:
  - T-24129
---

## Summary

Attack the remaining 21 unmatched functions in `TACO` and 12 unmatched functions in `ETC` using the new `T-24131` `uopt` + `ugen` idioms (`<expr> * 0` `ugen` subtree discard, `(v | 0) == C` `uoptinput` operand swap, and constant LIFO ordering) plus `keepproof.py` `D_800E7384`/`D_800E71DF` checks:
- `ETC`: `func_80140AE4` (20 rows in `8013FF80.c`), `func_80144DF0` (26 rows in `80143240.c`), `func_8013FFAC` (`8013FF80.c`), `func_8013CCE4`/`func_8013D570`/`func_8013F054` (`8013BEF0.c`), `func_801328A4`/`func_80132C3C` (`80132880.c`), `func_80144224` (`80143240.c`), `func_80146D4C`/`func_8014790C`/`func_801480D0` (`80146400.c`)
- `TACO`: `func_801511C0` (5 rows in `80149F90.c`), `func_80138B2C` (`80138890.c`), `func_8013B460` (`8013B170.c`), `func_80144094` (`80143E80.c`), `func_80153B38` (`80153AA0.c`), `func_80144E3C`/`func_80145650` (`801447D0.c`), `func_8013BAC4`/`func_8013C6D4` (`8013B990.c`), `func_8013DEE4`/`func_8013EAA4` (`8013DB80.c`), `func_80139424`/`func_80139FB8`/`func_8013A790` (`80139170.c`), `func_8013F468`/`func_8013FBF0`/`func_80140028` (`8013F250.c`), `func_8014AB50`/`func_8014CB24`/`func_8014F820`/`func_8014FFDC` (`80149F90.c`)

## Comments

- closing (2026-10-20): wave-3 sweep; its matches are on main (checkpoint 6d3d78ed8, reviewed: every new FAKE names its ticket, no inline asm, no guard over 30 rows). Remaining functions were consolidated into T-24142. Done.
