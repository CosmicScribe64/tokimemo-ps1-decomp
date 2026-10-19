---
id: T-24130
title: Close remaining near misses in SHOUGATU, GYOZI, and BUNKA_SD (wave 2)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24128
---

## Summary

Attack the remaining guarded and unguarded near misses across `SHOUGATU`, `GYOZI`, and `BUNKA_SD` (wave 2).

## Results (+4 functions, +2,312 B; `src/ovl/SHOUGATU/801390C0.c` at 100.0% matched!)

- `SHOUGATU:func_8013737C` (`src/ovl/SHOUGATU/80136F90.c`, 744 B): `MATCH`
- `SHOUGATU:func_80139334` (`src/ovl/SHOUGATU/801390C0.c`, 480 B — **`src/ovl/SHOUGATU/801390C0.c` 100.0% matched**): `MATCH`
- `SHOUGATU:func_8013D92C` (`src/ovl/SHOUGATU/8013D660.c`, 304 B): `MATCH`
- `GYOZI:func_801393C8` (`src/ovl/GYOZI/80138B50.c`, 784 B): `MATCH`
- Improved `#ifdef NON_MATCHING` guards for `SHOUGATU:func_80133B44` (`score=84`), `SHOUGATU:func_80137664` (`score=55`), and `SHOUGATU:func_8013DAAC` (`score=116`).
- Remaining `SHOUGATU` (10), `GYOZI` (9), and `BUNKA_SD` (2) functions continue in [[tickets/T-24137-shougatu-gyozi-bunkasd-wave-3-closers]].

## Review (`CODING_STANDARDS.md`)

- Replaced `#define func_..._hide` workarounds with canonical `SHOUGATU_OVERRIDE_func_80137AB4` and `MAIN_API_OVERRIDE_func_800BCE10` overrides per section 8a.
- Verified `check_headers.py`, `ninja build/ovl/SHOUGATU.ok build/ovl/GYOZI.ok`, and `funcdiff.py --resolve`. Zero findings.
