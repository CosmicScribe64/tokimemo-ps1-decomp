---
id: T-24091
title: Match ETC, SHUGAKU, and SHOUGATU sibling batch D (func_8014354C, func_80144F74, func_80144DF0, func_80134658, func_8013ABAC, func_801352B0, func_8013D92C)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following [[tickets/T-24088-gyozi-shougatu-valen-siblings-c]], target the next high-similarity sibling ports in `ETC`, `SHUGAKU`, and `SHOUGATU` (all in files disjoint from `T-24083`, `T-24087`, `T-24089`, `T-24090`):

- `ETC:func_8014354C` (248 B in `src/ovl/ETC/80143240.c`, matched sibling `ETC:func_8013F980` in `src/ovl/ETC/8013F980.c` at ratio 0.864)
- `ETC:func_80144F74` (608 B in `src/ovl/ETC/80143240.c`, matched sibling `NAME_ENT:func_8013F758` in `src/ovl/NAME_ENT/80139740.c` at ratio 0.770)
- `ETC:func_80144DF0` (388 B in `src/ovl/ETC/80143240.c`, matched sibling `main:func_8005D448` in `src/main/8005A0B0.c` at ratio 0.753)
- `SHUGAKU:func_80134658` (368 B in `src/ovl/SHUGAKU/801344B0.c`, matched sibling `DATE:func_8015485C` in `src/ovl/DATE/80154260.c` at ratio 0.793)
- `SHUGAKU:func_8013ABAC` (356 B in `src/ovl/SHUGAKU/8013AA70.c`, matched sibling `GEKO:func_801337EC` in `src/ovl/GEKO/801323E0.c` at ratio 0.792)
- `SHUGAKU:func_801352B0` (384 B in `src/ovl/SHUGAKU/80134880.c`, guarded at ~20 rows)
- `SHOUGATU:func_8013D92C` (304 B) and `SHOUGATU:func_8013D820` (268 B) in `src/ovl/SHOUGATU/8013D660.c` (siblings `GYOZI:func_80140330` and `GYOZI:func_80140244` in `src/ovl/GYOZI/801400D0.c`, drafts in `drafts/T-24056/`)

## Acceptance criteria

- [x] Close as many of the target functions as possible to `score=0` in `build/scratch/t24091/`
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Result

Matched 2 functions in `SHUGAKU` (+740 B verified default C):
1. `SHUGAKU:func_8013ABAC` (356 B in `src/ovl/SHUGAKU/8013AA70.c`): `s32` return, `(u32)D_800E6280.unk_1104.w++ >= 0x400U`, and `else { return 0; }` on `func_80044E8C() == 1`.
2. `SHUGAKU:func_801352B0` (384 B in `src/ovl/SHUGAKU/80134880.c`): `SHUGAKU_OVERRIDE_func_801343B0` (`s32` return view) keeps `count` in `$v1` across the call; `s32 pad;` gives the `0x38` frame (`sp+0x30`/`sp+0x2C`), and `*(u32 *)&page` prevents `uopt` CSE across the branch (`FAKE`).
- `ETC:func_8014354C` (248 B in `src/ovl/ETC/80143240.c`) reached `score=5`, blocked only on creating the `.data` island for `ETC/80143240` (`0x80150130..0x80150630`) so IDO shares `lui $at` across `D_8015013C.vx = 0; D_8015013C.vy = 0;`; spun off into [[tickets/T-24094-etc-data-island-and-near-complete-overlays]].

