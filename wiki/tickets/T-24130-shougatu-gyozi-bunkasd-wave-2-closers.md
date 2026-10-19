---
id: T-24130
title: Close remaining near misses in SHOUGATU, GYOZI, and BUNKA_SD (wave 2)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24128
---

## Summary

Attack the remaining guarded and unguarded near misses across `SHOUGATU` (13 functions), `GYOZI` (10 functions), and `BUNKA_SD` (2 functions) using `if (var) {}` / `if (const) {}` loop/block priority ordering (`T-24128`), `*(Type *)((u8 *)&sym)` copy-prop stoppers (`T-24120`), and `((Type *)((u8 *)base + ...))` `Uadd` vs `Uixa` (`T-24120`):
- `SHOUGATU:func_801347E0` (`80134120.c`, 2 rows left), `SHOUGATU:func_80139334` (`801390C0.c`, `score=17`), `SHOUGATU:func_8013D92C` & `func_8013DAAC` (`8013D660.c`), `SHOUGATU:func_8013737C` & `func_80137664` (`80136F90.c`), `SHOUGATU:func_80133B44` (`801330C0.c`), `SHOUGATU:func_801350F8` (`80134890.c`), `SHOUGATU:func_80138240` (`80137FF0.c`), `SHOUGATU:func_8013E50C`/`func_8013E6E4`/`func_8013EA98` (`8013E330.c`), `SHOUGATU:func_8013FB28` (`8013F190.c`)
- `GYOZI:func_80138C9C`/`func_8013907C`/`func_801393C8` (`80138B50.c`), `GYOZI:func_80140E0C`/`func_80140F90`/`func_801412A4` (`80140C90.c`), `GYOZI:func_80140464` (`801400D0.c`), `GYOZI:func_801421B4` (`801418B0.c`), `GYOZI:func_801368B8` (`80136220.c`), `GYOZI:func_80139E4C` (`80139920.c`)
- `BUNKA_SD:func_80134D94` (`80134540.c`) and `BUNKA_SD:func_80139290` (`801388D0.c`, 379/379 insns)
