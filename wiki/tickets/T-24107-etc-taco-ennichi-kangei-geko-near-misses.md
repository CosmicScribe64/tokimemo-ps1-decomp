---
id: T-24107
title: Close top live-scored near misses in ETC/80140F50, KANGEI, GEKO, EN_NICHI, and TACO
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24102
  - T-24104
---

## Context

From the repo-wide live score sweep in [[tickets/T-24104-ugen-pass-and-cvt-pass-near-miss-sweep]] (`build/scratch/t24104/ranked_drafts.txt`), the following functions on disjoint files are at `score = 0..13`:

1. **`ETC:func_80141ABC`** (`src/ovl/ETC/80140F50.c`): `score = 0` in `drafts/T-24067/body.c`!
2. **`KANGEI:func_8013852C`** (`src/ovl/KANGEI/80137B90.c`): `score = 4` in `drafts/T-24054/body.c` (plus `KANGEI:func_801389A4`, the only other remaining function in the `KANGEI` overlay!).
3. **`GEKO:func_80136C20`** (`src/ovl/GEKO/80134D10.c`): `score = 4` (`src_guarded`).
4. **`EN_NICHI:func_80132EE8`** (`score = 6`, `drafts/T-20920/EN_NICHI/func_80132EE8/hand.c`), **`EN_NICHI:func_80135FBC`** (`score = 8`, `drafts/T-20920/EN_NICHI/func_80135FBC/hand_fake.c`), and **`EN_NICHI:func_801362A4`** (`score = 22`, `drafts/T-20920/EN_NICHI/func_801362A4/hand.c`) in `src/ovl/EN_NICHI/80132000.c`.
5. **`TACO` near misses**: `TACO:func_8014A480` (`score = 2`), `TACO:func_801511C0` (`score = 4`), and `TACO:func_8014F5F0` (`score = 11`) in `src/ovl/TACO/80149F90.c`; `TACO:func_8013D838` (`score = 10`) in `src/ovl/TACO/8013D1E0.c`; `TACO:func_80134A18` (`score = 13`) in `src/ovl/TACO/80134930.c`.

## Acceptance criteria

- [ ] Promote `ETC:func_80141ABC` (`score=0`) and close as many of the `KANGEI`, `GEKO`, `EN_NICHI`, and `TACO` near misses as possible to `score=0` in `build/scratch/t24107/`
- [ ] Verify all promoted matches with `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`
