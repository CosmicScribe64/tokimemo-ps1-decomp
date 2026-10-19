---
id: T-24137
title: Wave 3 closer sweep across SHOUGATU (10), GYOZI (9), and BUNKA_SD (2)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24130
---

## Summary

Attack the remaining 10 unmatched functions in `SHOUGATU`, 9 unmatched functions in `GYOZI`, and 2 unmatched functions in `BUNKA_SD` using the new `T-24131` `uopt` + `ugen` idioms (`<expr> * 0` `ugen` subtree discard, `(v | 0) == C` `uoptinput` operand swap, and constant LIFO ordering) plus `keepproof.py` `D_800E71DF` proof for `SHOUGATU:func_80137664`:
- `SHOUGATU`: `func_801347E0` (2 rows in `80134120.c`), `func_80137664` (`80136F90.c` — test `keepproof.py check SHOUGATU:func_80137664 --symbol D_800E71DF`!), `func_80133B44` (`801330C0.c`), `func_801350F8` (`80134890.c`), `func_80138240` (`80137FF0.c`), `func_8013DAAC` (`8013D660.c`), `func_8013E50C`/`func_8013E6E4`/`func_8013EA98` (`8013E330.c`), `func_8013FB28` (`8013F190.c`)
- `GYOZI`: `func_80138C9C`/`func_8013907C` (`80138B50.c`), `func_801368B8` (`80136220.c`), `func_80139E4C` (`80139920.c`), `func_80140464` (`801400D0.c`), `func_80140E0C`/`func_80140F90`/`func_801412A4` (`80140C90.c`), `func_801421B4` (`801418B0.c`)
- `BUNKA_SD`: `func_80134D94` (`80134540.c`) and `func_80139290` (`801388D0.c`, 379/379 insns)
