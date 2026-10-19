---
id: T-24120
title: Sweep and close remaining unmatched functions in TACO (29 functions)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24107
---

## Summary

Attack and close as many of the 29 remaining `INCLUDE_ASM` functions in `src/ovl/TACO/` as possible.

## Results

Matched and promoted 6 functions (+3,696 B) across `src/ovl/TACO/` (completing `src/ovl/TACO/8013D1E0.c` and `src/ovl/TACO/80141DE0.c` to 100.0%) and added a 4-row `#ifdef NON_MATCHING` guard for `func_8014F820`:
- `TACO:func_8013506C` (240 B in `src/ovl/TACO/80134930.c`): leading `s32 pad[1];` before `i, x, y` to place the `$a3` spill at `0x3C($sp)` in the `0x48` frame, and separate `x`/`y` `func_800AE0D0()` statements.
- `TACO:func_8013D838` (840 B in `src/ovl/TACO/8013D1E0.c` — **`src/ovl/TACO/8013D1E0.c` 100.0% matched**): `(arg3 & 3) << 6` and `((arg3 & 4) << 4) + 0x80` inlined in the 1-line `setUVWH` expansion with `s32 pad;` for the 6th local word below `pt`.
- `TACO:func_801424A8` (888 B in `src/ovl/TACO/80141DE0.c` — **`src/ovl/TACO/80141DE0.c` 100.0% matched**): `k = *(s32 *)((u8 *)&D_8011ECA0); y = D_800E6280.unk_018[k];` prevents `uoptinput.p` intra-block copy-propagation of `k` in `BB_cond1` so `k` gets `$a1` directly.
- `TACO:func_80147B24` (372 B in `src/ovl/TACO/80146D60.c`): byte-pointer addition `((TcActor *)((u8 *)D_8015EDB4 + (i + 16) * 0x88))->unk84[1]` on the first access emits `Uadd` instead of `Uixa`, preventing `uopt` LFTR from replacing `i != arg0` with a pointer bound.
- `TACO:func_8014A480` (796 B in `src/ovl/TACO/80149F90.c`): `((TcActor *)((u8 *)D_8015EDB4 + sel * 0x88))->unk74` on the first store seeds `uopt`'s commutative `Uadd(dtype=A)` `ichain` with `D_8015EDB4` first (`addu $t4, $t6, $v1`) for the PRE reload at the end of the `if` body.
- `TACO:func_8014F5F0` (560 B in `src/ovl/TACO/80149F90.c`): repeated masked sum `(D_800E6280.unk_038 + 1) & 0xFF` CSE'd in `$v0` across all three stores.
- Guarded `TACO:func_8014F820` (`src/ovl/TACO/80149F90.c`, 400 B) at `score=4` (`100/100` instructions).
