---
id: T-24125
title: Sweep remaining SHOUGATU, GYOZI, and BUNKA_SD functions
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24114
  - T-24116
---

## Summary

Attack and close remaining `INCLUDE_ASM` functions in `SHOUGATU`, `GYOZI`, and `BUNKA_SD`.

## Results

- Matched and promoted **`SHOUGATU:func_8013BA7C`** (432 B in `src/ovl/SHOUGATU/8013BA00.c` — **`src/ovl/SHOUGATU/8013BA00.c` is now 100.0% matched**):
  - Solved the `GameState D_800E6280` unification CSE priority swap (`$v0/$v1` vs `$a1/$a2` in the `0..10` girl loop) by adding two empty `if` reads inside `if (i != (s32)D_800E6280.unk_F5F)`: `if (D_800E6280.unk_1BC[i].unk_06) {}` (boosts `&D_800E6280 + i * 0x38` into `$v0`) and `if (D_800E6280.unk_1BC[0].unk_06) {}` (boosts `&D_800E6280` into `$a1`).
- Improved `#ifdef NON_MATCHING` guard for `SHOUGATU:func_801347E0` (`src/ovl/SHOUGATU/80134120.c`) from 3 rows down to 2 rows.
- Reached exact 379/379 instructions on `BUNKA_SD:func_80139290` (`src/ovl/BUNKA_SD/801388D0.c`, zero scheduling/branch/stack diffs).
