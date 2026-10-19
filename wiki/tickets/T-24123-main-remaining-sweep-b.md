---
id: T-24123
title: Sweep remaining unmatched functions in src/main/ (wave B)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24118
---

## Summary

Attack and close the next tier of unmatched functions in `src/main/` across:
- `src/main/80079B10.c`: `func_8007AF0C` (the final remaining `INCLUDE_ASM` function in `80079B10.c`!), `func_80079C70`, `func_8007A4DC`, `func_8007ADD8`, `func_8007B64C`, `func_8007B7E0`, `func_8007B930`
- `src/main/80043510.c`: `csr_load_vram`, `func_80043510`, `func_8004443C`, `palette_load_vram`
- `src/main/800451D0.c`: `func_80045414`, `func_800455C4`
- `src/main/80046500.c`: `func_80046754`, `func_80046AC8`
- `src/main/80047550.c`: `func_80047560`, `func_800476C0`
- `src/main/80049FF0.c`: `func_8004A1A0`, `func_8004A414`, `func_8004A814`
- `src/main/8004AD60.c`: `col2sepia`, `func_8004B358`, `make_color_bar`, `make_color_bar16`
- `src/main/80053650.c`, `80058D20.c`, `80060490.c`, `80061710.c`, `80065090.c`, `8006B590.c`, `80081FF0.c`, `80085850.c`, `80085E30.c`
