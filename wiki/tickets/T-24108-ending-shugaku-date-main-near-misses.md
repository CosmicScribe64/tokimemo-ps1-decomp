---
id: T-24108
title: Close top live-scored near misses in ENDING/80133C10, ENDING/801396E0, SHUGAKU/80139D00, DATE/8014A1B0, and main
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24104
---

## Context

Closed **6 functions (+2,388 B)** across `SHUGAKU`, `DATE`, `ENDING`, and `main`:

1. **`SHUGAKU:func_8013A1D4`** (`src/ovl/SHUGAKU/80139D00.c`, 160 B — **`80139D00.c` 100.0% matched!**): `MATCH` (`SHUGAKU_OVERRIDE_func_801386C4` + dead entry read of `unk_F5F`).
2. **`DATE:func_8014A8FC`** (`src/ovl/DATE/8014A1B0.c`, 1,140 B — **`8014A1B0.c` 100.0% matched!**): `MATCH` (`MAIN_API_OVERRIDE_D_80122760` `DateLightBlock` sharing one base symbol).
3. **`ENDING:func_80134B18`** (`src/ovl/ENDING/80133C10.c`, 208 B): `MATCH` (`((DisplayActor44 *)D_80120650)[1].unk_07 += 4;`).
4. **`ENDING:func_80134C2C`** (`src/ovl/ENDING/80133C10.c`, 244 B): `MATCH` (`((DisplayActor44 *)D_80120650)[1].unk_07 -= 4;`).
5. **`main:func_800754C0`** (`src/main/80075320.c`, 244 B): `MATCH` (dead entry read `m = D_800E6280.unk_03F;` before `func_80053418(rank, points)`).
6. **`main:func_80077CA8`** (`src/main/80075320.c`, 392 B): `MATCH` (`switch (D_800E6280.unk_03E)` without `default:` + `if (D_800E6280.unk_03F >= 0)` in `case 0x62`).

## Acceptance criteria

- [x] Close 6 target near misses to `score=0` (+2,388 B)
- [x] Promote all `score=0` functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`
