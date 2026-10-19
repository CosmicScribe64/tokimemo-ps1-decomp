---
id: T-24127
title: Sweep remaining unmatched functions in src/main/ (wave C)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24123
---

## Summary

Attack and close the next wave of unmatched functions in `src/main/` across `80042A00.c`, `80079B10.c`, `80085E30.c`, and remaining `src/main/*.c` units.

## Results (+3 functions, +1,184 B; `src/main/80079B10.c` and `src/main/80085E30.c` 100.0% matched!)

- `main:func_8007B64C` (`src/main/80079B10.c`, 148 B): `MATCH`
  - Used a single `return ret;` in a join block after `if (D_80125CC0 == 0)` / `else` so `D_80125CC0--` is outside the basic block containing `Ustr(R 0 2, ...)` (`return`), preventing `uopt`'s `f_updateforbidden` from forbidding `$v0` (`r2`) on `D_80125CC0`.
- `main:func_8007BBE8` (`src/main/80079B10.c`, 276 B — **`src/main/80079B10.c` 100.0% matched, 36/36 functions!**): `MATCH`
  - Used `*(s32 *)(i * 4 + (u32)arg0)` so the signed-to-unsigned `Ucvt(L, J)` on `i * 4` prevents `uopt` from folding `arg0` into the 4x-unrolled induction variable (`0, 4, 8, 12` in `$v1, $t1, $t2, $t3`, step `16`, end `0x4C`).
- `main:Scroll` (`src/main/80085E30.c`, 760 B — **`src/main/80085E30.c` 100.0% matched, 14/14 functions!**): `MATCH`
  - Typed `p` as `POLY_FT4 *`, used `s16 arg2, s16 arg3, s16 arg4`, ordered loop-invariant expression bit positions (`pad1 = i * 0x20; pad2 = (u32)arg4;`), and added post-loop `if ((u8)arg2) {}` so `$s6 = arg2, $s7 = arg3, $s8 = arg2 + arg3`.
  - Also replaced raw `func_80085F0C` call-site aliases in `src/ovl/TAIIKU/{80133C80,80138950,80142240,801452D0}.c` with `Scroll`.
- Added `#ifdef NON_MATCHING` guard for `main:func_80042CB0` (`src/main/80042A00.c`).
- Remaining `src/main/` functions continue in [[tickets/T-24135-main-remaining-sweep-d]].

## Review (`CODING_STANDARDS.md`)

- Verified `check_headers.py`, `test_check_headers.py`, `test_sync_protos.py`, `test_cvt_pass.py`, `ninja build/SLPM_86.053.ok build/ovl/EN_NICHI.ok build/ovl/TAIIKU.ok`, and `funcdiff.py --resolve`. Zero findings.
