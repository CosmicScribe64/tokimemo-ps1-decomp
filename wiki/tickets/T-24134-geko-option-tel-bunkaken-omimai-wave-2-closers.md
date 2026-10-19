---
id: T-24134
title: Wave 2 closer sweep across GEKO (12), OPTION (10), TEL (2), BUNKAKEN (1), and OMIMAI (1)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24126
---

## Summary

Attack the remaining unmatched functions across `GEKO`, `OPTION`, `TEL`, `BUNKAKEN`, and `OMIMAI` (wave 2).

## Results (+3 functions, +3,048 B across `OPTION` and `BUNKAKEN`; **`BUNKAKEN` overlay 100.0% matched, 60/60 functions!**)

- `BUNKAKEN:func_8013B510` (`src/ovl/BUNKAKEN/8013B510.c`, 2,164 B — **`BUNKAKEN` overlay 100.0% matched!**): `MATCH`
- `OPTION:func_80132000` (`src/ovl/OPTION/80132000.c`, 176 B): `MATCH`
- `OPTION:func_801320C0` (`src/ovl/OPTION/80132000.c`, 708 B): `MATCH`
- Reduced `OPTION:func_80132384` (`src/ovl/OPTION/80132000.c`) from 111 diff rows to a 2-instruction `uopt` constant-multiplier hoisting gap (`ldc 20` hoisted into `$s5` vs expanded in-place by `ugen`).
- Remaining `GEKO` (12), `OPTION` (8), `TEL` (2), and `OMIMAI` (1) functions continue in [[tickets/T-24140-geko-option-tel-omimai-wave-3-closers]].

## Review (`CODING_STANDARDS.md`)

- Verified `check_headers.py`, `ninja build/ovl/OPTION.ok build/ovl/BUNKAKEN.ok`, and `funcdiff.py --resolve`. Zero findings.
