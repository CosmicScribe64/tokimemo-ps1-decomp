---
id: T-24123
title: Sweep remaining unmatched functions in src/main/ (wave B)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24118
---

## Summary

Attack and close the next tier of unmatched functions in `src/main/`.

## Results

Matched and promoted 5 functions (+2,284 B) in `src/main/` and improved 2 `#ifdef NON_MATCHING` drafts in `src/main/80053650.c`:
- `main:func_80045414` (432 B in `src/main/800451D0.c`): K&R `u8 arg0` narrowing on entry (`MAIN_API_OVERRIDE_func_80045414`), single-block clamp `arg1 = ((u32) arg1 >= 0x4AB51U) ? 0x4AB50 : arg1;`, `u8 buf[0x20];` at `sp+0x38`, and `case 27: break;` jump-table extent.
- `main:func_8004B358` (568 B in `src/main/8004AD60.c`): `s32` return type in `include/main_api.h`, live-range start after early `return 0;` guards so `(p, x, c, flag, ptr)` allocate to `($s0, $s1, $s2, $s3, $s4)`, `setWH(p, 6, 0xC)`, and `s32 pad;` for `0x88` frame.
- `main:func_80056070` (532 B in `src/main/80053650.c`): `s32` return type and `extern u8 D_800B3938[];` in `include/main_api.h`, same-line `i = 0; while (i < 0x14)` unrolled zero loop.
- `main:func_80079C70` (184 B in `src/main/80079B10.c`): signed `~0x0FFFFFFF` in loop condition vs unsigned `0xF0000000U` in body to keep separate `uopt` constant entries (`$a2`, `$a3`, `$at`).
- `main:func_8007AF0C` (568 B in `src/main/80079B10.c`): explicit 16-bit sign-extension `(D_80125E20[(u16)D_80125D4A][D_80125D3A] << 16) >> 16` for `$a1` on unprototyped `func_8008FF5C`.
- Improved `#ifdef NON_MATCHING` guards in `src/main/80053650.c`: `func_80054884` (`score=36`, exact 156/156 insns) and `func_80054E6C` (`score=84`).
