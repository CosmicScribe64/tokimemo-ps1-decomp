---
id: T-24122
title: Close SHUGAKU final 3 girl-picker loops and sweep KANGEI, MASTER, EN_NICHI, RENSYU, ENDING, and VALEN
status: Done
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

## Results

- Matched **6 functions (+3,516 bytes)** across `SHUGAKU`, `KANGEI`, and `RENSYU`:
  - **`SHUGAKU` brought to 100.0% matched (`190 / 190` functions, `36,244 / 36,244` bytes, `0 INCLUDE_ASM`):**
    - `SHUGAKU:func_8013243C` (388 B in `src/ovl/SHUGAKU/80132000.c`)
    - `SHUGAKU:func_801337FC` (620 B in `src/ovl/SHUGAKU/80133430.c`)
    - `SHUGAKU:func_80139118` (412 B in `src/ovl/SHUGAKU/80138A60.c`)
  - **`KANGEI` (+2 functions / +1,288 B):**
    - `KANGEI:func_80133A54` (444 B in `src/ovl/KANGEI/80132E40.c`)
    - `KANGEI:func_80138F88` (844 B in `src/ovl/KANGEI/80137B90.c`)
    - Improved `#ifdef NON_MATCHING` guards for `KANGEI:func_80134724` (2 rows / 1 pair), `KANGEI:func_8013852C` (2 rows / 1 pair), and `KANGEI:func_80135004` (7 rows).
  - **`RENSYU` (+1 function / +808 B):**
    - `RENSYU:func_80133990` (808 B in `src/ovl/RENSYU/80132040.c`)
- Replaced `func_80052C88` / `func_80052648` with `get_h_yuukou` / `get_h_tokimeki` via `MAIN_API_OVERRIDE_*` in `src/ovl/SHUGAKU/{80132000,80133430,80138A60}.c` and `src/ovl/KANGEI/80137B90.c` per `CODING_STANDARDS.md`.

