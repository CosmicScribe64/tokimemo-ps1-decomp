---
id: T-24122
title: Close SHUGAKU final 3 girl-picker loops and sweep KANGEI, MASTER, EN_NICHI, RENSYU, ENDING, and VALEN
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24114
  - T-24115
---

## Summary

1. Test `MAIN_API_OVERRIDE_get_h_tokimeki` / `MAIN_API_OVERRIDE_get_h_yuukou` / `MAIN_API_OVERRIDE_get_g_zyotai_s` (and local declaration order) on the 3 remaining identical girl-selection loops in `SHUGAKU` (`func_8013243C` in `80132000.c`, `func_801337FC` in `80133430.c`, `func_80139118` in `80138A60.c`) to bring `SHUGAKU` to 100%.
2. Sweep the remaining low-to-mid-score functions in `KANGEI` (7), `MASTER` (7), `EN_NICHI` (4), `RENSYU` (4), `ENDING` (6), and `VALEN` (4).
