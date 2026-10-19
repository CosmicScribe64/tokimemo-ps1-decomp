---
id: T-24121
title: Sweep and close remaining unmatched functions in ETC (21 functions)
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24107
---

## Summary

Attack and close as many of the 21 remaining `INCLUDE_ASM` functions in `src/ovl/ETC/` as possible.

## Results

Matched and promoted 5 functions (+3,348 B) across `src/ovl/ETC/`:
- `ETC:func_80141BE8` (552 B in `src/ovl/ETC/80140F50.c` — **`src/ovl/ETC/80140F50.c` is now 100.0% matched**): K&R implicit-int return (`s32 func_80141BE8(void)`), `(s32 *)((u32)&D_800B37D4[26]) + girl * 2` cast to prevent `uopt` from folding the `+26` word offset into `lw`.
- `ETC:func_80144F74` (608 B in `src/ovl/ETC/80143240.c`): `GameState *g = &D_800E6280;` pointer view and `(u8)D_800E8BEE != 0` compare.
- `ETC:func_80145C00` (884 B in `src/ovl/ETC/80143240.c`): single-line `for (i = 0; i < 8; i++) { sp5C[i] = 0x80180000 + i * 0x4000; }` prevents `uopt` unrolling; `s32 pad[2];` places `sp5C[8]` at `0x5C($sp)` in the `0x88` frame; chained `(&D_800E6280.unk_036)[0] = (&D_800E6280.unk_037)[0] = (&D_800E6280.unk_038)[0] = 0xFF;` shares `li $v0, 0xFF`.
- `ETC:func_80148F7C` (836 B in `src/ovl/ETC/80146400.c`): `s32 pad[2];` for `0x78` frame and `0x5C` CSE slot; modifying `arg3` directly and `if (var_s1 >= 0) { do { ... } while (var_s1 >= 0); }` unsigned digit loop.
- `ETC:func_8014A360` (468 B in `src/ovl/ETC/801492C0.c`): `D_800E6280.unk_0F6.b[1]` bitmask assignment by `D_80150E98` and `func_8014A2C4()`/`func_8014A32C()`.
