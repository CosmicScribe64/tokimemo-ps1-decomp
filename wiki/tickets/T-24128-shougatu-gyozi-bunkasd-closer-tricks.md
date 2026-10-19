---
id: T-24128
title: Apply SHUGAKU girl-picker recipe and uopt CSE priority tricks to SHOUGATU, GYOZI, and BUNKA_SD
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24122
  - T-24125
---

## Summary

Apply the `T-24122` girl-picker loop recipe (`if (1) {}` + `s == best_s && s == best_s &&` + `(best_i & 0) + best_i`) and `T-24119` / `T-24125` `uopt` CSE priority tricks across `SHOUGATU`, `GYOZI`, and `BUNKA_SD`.

## Results

Matched and promoted 3 functions (+1,048 B) across `SHOUGATU` and `GYOZI` (completing `src/ovl/SHOUGATU/80132550.c` and `src/ovl/GYOZI/80134520.c` to 100.0%):
- `SHOUGATU:func_8013288C` (424 B in `src/ovl/SHOUGATU/80132550.c` — **`src/ovl/SHOUGATU/80132550.c` 100.0% matched**): `T-24122` girl-picker loop recipe (`if (1) {}`, `s == best_s && s == best_s`, `D_800E6280.unk_F5F = (best_i & 0) + best_i;`) + `MAIN_API_OVERRIDE_get_h_tokimeki`/`get_h_yuukou`.
- `GYOZI:func_801347D4` (356 B in `src/ovl/GYOZI/80134520.c` — **`src/ovl/GYOZI/80134520.c` 100.0% matched**): `if (best_i) {}` + `if (i == 11) {}` at top of the `0..10` loop orders `best_i` (`$s3`) and `11` (`$s4`) ahead of `&D_800F53A0` (`$s5`) and `56` (`$s6`); `if (s) {}` between `s = func_8005E0E0(i);` and `s &= 0x7F;` keeps `move $v1, $v0` before `andi $t9, $v1, 0x7F`.
- `SHOUGATU:func_8013D820` (268 B in `src/ovl/SHOUGATU/8013D660.c`): narrowing `s16 thresh = D_800E6280.unk_1BC[12].unk_0C.f.f9 * 25 + 150;` keeps `thresh` unpromoted in `$v0` while `uopt` promotes `D_800E6280.unk_1BC[12].unk_0C.f.f9` to `$v1` (zero-`FAKE` match).
- Improved `GYOZI:func_80140E0C` (`src/ovl/GYOZI/80140C90.c`) `#ifdef NON_MATCHING` guard from 21 rows down to 13 rows.
