---
id: T-24107
title: Close top live-scored near misses in ETC/80140F50, KANGEI, GEKO, EN_NICHI, and TACO
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24102
  - T-24104
---

## Context

From the repo-wide live score sweep in [[tickets/T-24104-ugen-pass-and-cvt-pass-near-miss-sweep]] (`build/scratch/t24104/ranked_drafts.txt`), the following functions on disjoint files were attacked:

1. **`ETC:func_80141ABC`** (`src/ovl/ETC/80140F50.c`, 300 B): `MATCH` (`score = 0` via `tools/ugen_pass.py` `B`/`F` block-copy direction pass).
2. **`TACO:func_80134A18`** (`src/ovl/TACO/80134930.c`, 884 B): `MATCH` (`score = 0` via `MAIN_API_OVERRIDE_D_80122760` `TacoLightBlock` sharing one base symbol so `as1` hoists `b1`/`b2` loads across `r0`/`g0` stores + repeated `(b2 + 1) & 0xFF`).
3. **`EN_NICHI:func_80135FBC`** (`src/ovl/EN_NICHI/80132000.c`, 744 B): `MATCH` (`score = 0` via single-line UV `+ 1` byte store grouping).
4. **`EN_NICHI:func_801362A4`** (`src/ovl/EN_NICHI/80132000.c`, 1,116 B): `MATCH` (`score = 0` via `s32 pad;` above `x`/`y` and `y = x;` inside `if (D_80139B00 < y)` killing `uopt` copy propagation of `$v0` across the branch).
5. **`KANGEI:func_8013852C`** (`src/ovl/KANGEI/80137B90.c`): Preserved 3-row `#ifdef NON_MATCHING` block from `drafts/T-24054/8013852C.c`.

## Acceptance criteria

- [x] Promote `ETC:func_80141ABC` (`score=0`) and close `TACO:func_80134A18`, `EN_NICHI:func_80135FBC`, and `EN_NICHI:func_801362A4` to `score=0` (+3,044 B total)
- [x] Verify all promoted matches with `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`
