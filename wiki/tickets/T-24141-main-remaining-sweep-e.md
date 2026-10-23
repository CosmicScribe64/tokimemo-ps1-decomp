---
id: T-24141
title: Sweep remaining unmatched functions in src/main/ (wave E)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-20
depends_on:
  - T-24135
---

## Summary

Attack and close the next wave of unmatched functions in `src/main/` across:
- `src/main/80042A00.c`: `func_80042CB0` (1 function left in `80042A00.c`)
- `src/main/800451D0.c`: `func_800455C4` (1 function left in `800451D0.c`)
- `src/main/8004E500.c`: `disp_string` (1 function left in `8004E500.c`)
- `src/main/80046500.c`: `func_80046754`, `func_80046AC8` (2 functions left in `80046500.c`)
- `src/main/80047550.c`: `func_80047560`, `func_800476C0` (2 functions left in `80047550.c`)
- `src/main/800490C0.c`: `_sprite_set_light_effect1`, `func_80049A40` (2 functions left in `800490C0.c`)
- `src/main/800563F0.c`: `strCallback`, `func_80056BA8` (2 functions left in `800563F0.c`)
- `src/main/80061710.c`: `func_80061A3C`, `func_80062948` (2 functions left in `80061710.c`)
- `src/main/800737A0.c`: `func_80073CB0`, `parameter_up_down` (2 functions left in `800737A0.c`)
- Plus `80043510.c`, `80049FF0.c`, `8004AD60.c`, `80058D20.c`, `8006CB30.c`, `8004F870.c`, `80075320.c`

## Comments

- closing (2026-10-20): wave-3 sweep; its matches are on main (checkpoint 6d3d78ed8, reviewed: every new FAKE names its ticket, no inline asm, no guard over 30 rows). Remaining functions were consolidated into T-24142. Done.
