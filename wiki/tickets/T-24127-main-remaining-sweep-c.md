---
id: T-24127
title: Sweep remaining unmatched functions in src/main/ (wave C)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24123
---

## Summary

Attack and close the next wave of unmatched functions in `src/main/` across:
- `src/main/80042A00.c`: `func_80042CB0` (1 function left in `80042A00.c`)
- `src/main/800451D0.c`: `func_800455C4` (1 function left in `800451D0.c`)
- `src/main/80085E30.c`: `Scroll` (1 function left in `80085E30.c`)
- `src/main/80079B10.c`: `func_8007B64C`, `func_8007BBE8` (2 functions left in `80079B10.c`)
- `src/main/80046500.c`: `func_80046754`, `func_80046AC8`
- `src/main/80047550.c`: `func_80047560`, `func_800476C0`
- `src/main/800490C0.c`: `_sprite_set_light_effect1`, `func_80049A40`
- `src/main/8004E500.c`: `disp_string`, `glyph_expand_1bpp`
- `src/main/800563F0.c`: `strCallback`, `func_80056BA8`
- `src/main/80061710.c`: `func_80061A3C`, `func_80062948`
- `src/main/800737A0.c`: `func_80073CB0`, `parameter_up_down`
- Plus `80049FF0.c`, `8004AD60.c`, `8004C3F0.c`, `8004F870.c`, `80058D20.c`, `8005A0B0.c`, `80062CD0.c`, `800674B0.c`, `8006CB30.c`, `80075320.c`
