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

Refined `_constant_first` in `tools/cvt_pass.py` (`lines 428–435`) so that `str mt=MT_R` immediately followed by `rpar` (`idx + 1 < end and out[idx + 1].opc == OP["rpar"]`) is ignored when checking whether a register holds only an `RLOD` of a rewritten narrow variable. Added 2 unit tests in `tools/test_cvt_pass.py` (`44/44` pass) and matched 6 functions (+5,896 B) in `src/ovl/EVENT/8010FE10.c` (plus verified `score=0` for `DATE:func_8013FD6C` in `build/scratch/t24103/func_8013FD6C.c`):

- `EVENT:func_8011B848` (1,460 B symbol / 1,408 B code in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_80114C5C` (1,732 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_8011767C` (916 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_80118B4C` (608 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_80119110` (608 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- `EVENT:func_8011B60C` (572 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
