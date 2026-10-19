---
id: T-24103
title: Refine cvt_pass _constant_first to ignore rpar argument stores and match EVENT:func_8011B848 + remaining EVENT/8010FE10 functions
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24097
---

## Context

In [[tickets/T-24097-event-movie-final-and-date-event-twins]], tracing `EVENT:func_8011B848` (1,460 B in `src/ovl/EVENT/8010FE10.c`, twin of `DATE:func_8013FD6C` 1,436 B in `src/ovl/DATE/80132000.c`) revealed a bug in `tools/cvt_pass.py`'s `_constant_first` pass (`tools/cvt_pass.py:427–436`):

- `_constant_first` collects register definitions across the whole procedure `[start:end]` and marks any `str mt=MT_R` as `"other"`.
- In `uopt` output ucode, outgoing call arguments in `$a0..$a3` (`r4..r7`) are emitted as `str mt=MT_R` immediately followed by `rpar` (or inside `mst .. cup`/`icuf`). When a function promotes a narrow global (`D_800B1AF6` in `func_8011B848`, `D_800CA156` in `func_8013FD6C`) to `$a1` (`r5`) and later calls a 2+ argument function at the end of the procedure (`func_8001A650(-0xA0, -0x78, ...)`), the `str mt=MT_R` before `rpar` pollutes `defs[5]` with `"other"`, preventing `_constant_first` from restoring the `const == var` operand order on the earlier `==`/`!=` comparisons (`360/365` instructions match, with only the 5 `bne s2, a1` operand orders differing).

Additionally, check the remaining unmatched functions in `src/ovl/EVENT/8010FE10.c` (`func_80110564`, `func_80112BC0`, `func_801146E0`, `func_801164B0`, `func_8011A53C`, `func_8011B60C`).

## Acceptance criteria

- [ ] Test ignoring `str mt=MT_R` before `rpar` in `_constant_first` in `build/scratch/t24103/`, verify 0 regressions across all 431 compilation units, and add unit test coverage in `tools/tests/test_cvt_pass.py`
- [ ] Match `EVENT:func_8011B848` (1,460 B in `src/ovl/EVENT/8010FE10.c`) and attack the remaining unmatched functions in `src/ovl/EVENT/8010FE10.c`
- [ ] Verify with `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
