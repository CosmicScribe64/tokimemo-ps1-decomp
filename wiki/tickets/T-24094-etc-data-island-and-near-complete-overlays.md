---
id: T-24094
title: Add ETC/80143240 .data island to match func_8014354C and attack final functions of DATE2, BUNKA_SD, BUNKASAI, and OMIMAI
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

1. **`ETC:func_8014354C` (248 B in `src/ovl/ETC/80143240.c`)**:
   - In [[tickets/T-24091-etc-shugaku-shougatu-siblings-d]], `ETC:func_8014354C` reached `score=5`, with the only remaining diff being shared `lui $at` across `D_8015013C.vx = 0; D_8015013C.vy = 0;`, because `ETC/80143240` owns `.data` range `0x80150130..0x80150630` (`config/objects/ETC.txt`: `data 80143240 80150130 80150630 owner owner at:2,ref:9`).
   - Add the `.data` island for `ETC/80143240` using `tools/data_island.py` (`config/overlays/ETC.yaml`), define `D_8015013C` in `src/ovl/ETC/80143240.c`, and match `ETC:func_8014354C` to `score=0`.
2. **Near-complete overlays (1–3 functions remaining)**:
   - **`DATE2:func_801395C8`** (2,644 B in `src/ovl/DATE2/80137D50.c` — the ONLY remaining function in `DATE2`, `101/102`!): guarded `#ifdef NON_MATCHING` where the two spill homes sit at `sp+0x40/0x44` instead of `sp+0x44/0x48`. Test local padding (`s32 pad;` / local declaration order / `buf` struct wrapper) in `build/scratch/t24094/` to shift the spill homes by +4 bytes and close `DATE2` to 100%!
   - **`BUNKA_SD:func_80134D94`** (in `src/ovl/BUNKA_SD/80134540.c`) and **`BUNKA_SD:func_80139290`** (in `src/ovl/BUNKA_SD/801388D0.c`) — the ONLY 2 remaining functions in `BUNKA_SD` (`96/98`)! Check `drafts/` and `build/siblings/` for both.
   - **`BUNKASAI:func_801509A4`** (472 B in `src/ovl/BUNKASAI/80150930.c`, guarded at 19 rows) and **`BUNKASAI:func_80142BE8` / `func_801433C0`** (`src/ovl/BUNKASAI/80142700.c`) — the ONLY 3 remaining functions in `BUNKASAI` (`204/207`).
   - **`OMIMAI:func_8013388C`** (1,956 B in `src/ovl/OMIMAI/801331B0.c`) — the ONLY remaining function in `OMIMAI` (`35/36`).

## Acceptance criteria

- [ ] Add `ETC/80143240` `.data` island and match `ETC:func_8014354C` (248 B)
- [ ] Close as many of the final overlay functions (`DATE2`, `BUNKA_SD`, `BUNKASAI`, `OMIMAI`) as possible to `score=0` in `build/scratch/t24094/`
- [ ] Verify all matches with `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
