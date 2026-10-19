---
id: T-24108
title: Close top live-scored near misses in ENDING/80133C10, ENDING/801396E0, SHUGAKU/80139D00, DATE/8014A1B0, and main
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24104
---

## Context

From the repo-wide live score sweep in [[tickets/T-24104-ugen-pass-and-cvt-pass-near-miss-sweep]] (`build/scratch/t24104/ranked_drafts.txt`), the next lowest-score unmatched functions on disjoint files are:

1. **`src/ovl/ENDING/80133C10.c`**: `ENDING:func_80134B18` (`score = 4`, `drafts/T-24057/ENDING/func_80134B18/body.c`) and `ENDING:func_80134C2C` (`score = 4`, `drafts/T-24057/ENDING/func_80134C2C/body.c`).
2. **`src/ovl/ENDING/801396E0.c`**: `ENDING:func_8013A2E8` (`score = 12`, 4,648 B).
3. **`src/ovl/SHUGAKU/80139D00.c`**: `SHUGAKU:func_8013A1D4` (`score = 8`, `drafts/T-24053/selected.c`).
4. **`src/ovl/DATE/8014A1B0.c`**: `DATE:func_8014A8FC` (`score = 12`, `src_guarded`).
5. **`src/main/8006CB30.c` & `src/main/80075320.c`**:
   - `main:func_8006D7EC` (`score = 2`, `src/main/8006CB30.c`)
   - `main:appointment_launch` (`score = 7`, `drafts/T-20940/main/appointment_launch/body.c` in `src/main/80075320.c`)
   - `main:func_800754C0` (`score = 9`, `src/main/80075320.c`)
   - `main:func_800755B4` (`score = 10`, `drafts/T-20940/main/func_800755B4/body.c` in `src/main/80075320.c`)
   - `main:func_80076630` (`score = 18`, `drafts/T-20940/main/func_80076630/body.c` in `src/main/80075320.c`)
   - `main:func_80077CA8` (`score = 20`, `drafts/T-20940/main/func_80077CA8/body.c` in `src/main/80075320.c`)

## Acceptance criteria

- [ ] Close as many of the target near misses as possible to `score=0` in `build/scratch/t24108/`
- [ ] Promote all `score=0` functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`
