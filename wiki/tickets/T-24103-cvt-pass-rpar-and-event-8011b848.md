---
id: T-24103
title: Refine cvt_pass _constant_first to ignore rpar argument stores and match EVENT:func_8011B848 + remaining EVENT/8010FE10 functions
status: Done
priority: high
type: tooling
created: 2026-10-18
updated: 2026-10-18
---

## Summary

Refined `_constant_first` in `tools/cvt_pass.py` (`lines 428–457`) so that `str mt=MT_R` immediately followed by `rpar` (`idx + 1 < end and out[idx + 1].opc == OP["rpar"]`) records the register in `rpar_regs` without disqualifying it from `var_regs`, swapping `var == const` when the constant is in a callee-saved register (`r >= 16`, as in `EVENT:func_8011B848` / `DATE:func_8013FD6C`) while keeping the order when the constant is in a caller-saved register (`r < 16`, as in `EVENT:func_80117D80`). Added 3 unit tests in `tools/test_cvt_pass.py` (`45/45` pass) and matched 6 functions (+5,896 B) in `src/ovl/EVENT/8010FE10.c` (plus `DATE:func_8013FD6C` via `T-24099`):

- `EVENT:func_8011B848` (1,460 B symbol / 1,408 B code in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_80114C5C` (1,732 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_8011767C` (916 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_80118B4C` (608 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_80119110` (608 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_8011B60C` (572 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
