---
id: T-24111
title: Close low-score near misses in main (80043510, 800451D0, 8004F870, 80053650, 80058D20, 8005A0B0, 80061710, 80079B10, 80085E30)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24099
  - T-24104
---

## Context

Closed **7 functions (+3,140 B)** across 6 files in `src/main/`:

1. **`main:func_8007B144`** (`src/main/80079B10.c`, 368 B): `MATCH` (K&R `u16 arg0;` with `if (1) { temp_v0 = arg0 & 0xFF; } switch (temp_v0)`).
2. **`main:x_taku_menu_set`** (`src/main/8004F870.c`, 340 B): `MATCH` (`s32 pad; s32 i;` before `x[4], width[4], height[4]`).
3. **`main:func_800460FC`** (`src/main/800451D0.c`, 376 B): `MATCH` (`u8 pad[8]; s32 retries; WordBytes4 *state; u8 local[4]; u8 *index = &D_80125128; s32 saved; u8 status[8];` + single-line `retries = 0; (&state->bytes[0])[0] = ... = 0;`).
4. **`main:func_80054140`** (`src/main/80053650.c`, 324 B): `MATCH` (`GameState *g = &D_800E6280;` passing `g->unk_111C` in the `g->unk_111C == 0` arm and `g->unk_1118` in the `< 0xFU` arm).
5. **`main:func_8008647C`** (`src/main/80085E30.c`, 452 B): `MATCH` (`s32 sum1; s32 sum2; s32 cur; s32 list[14];` + `(s32)D_800E6280.unk_1BC[...].unk_06`).
6. **`main:func_80062210`** (`src/main/80061710.c`, 468 B): `MATCH` (single-line row stores `y, x, w, h` + `j += 2` in `for` step).
7. **`main:func_8005CC40`** (`src/main/8005A0B0.c`, 812 B): `MATCH` (`switch (*(u8 *)&D_800E6280.unk_110D)` + `extern s32 D_800B39C4[];` in `include/main_api.h`).

## Acceptance criteria

- [x] Attack and close 7 `main` near misses to `score=0` (+3,140 B)
- [x] Promote all `score=0` matches to unguarded default C and verify `tools/funcdiff.py --resolve`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md`
