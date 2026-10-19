---
id: T-24109
title: Close low-score near misses in SHOUGATU, GYOZI, VALEN, and MASTER
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24101
  - T-24104
---

## Context

From the live re-scoring sweep (`build/scratch/t24104/ranked_drafts.txt`), the following functions in `SHOUGATU`, `GYOZI`, `VALEN`, and `MASTER` have low instruction-diff scores under current `HEAD` (`tools/ugen_pass.py` + `tools/cvt_pass.py` + `tools/trailing_pad.py`):

- `SHOUGATU:func_801347E0` (`score=6` in `src/ovl/SHOUGATU/80134120.c`)
- `SHOUGATU:func_80138240` (`score=8` in `src/ovl/SHOUGATU/80137FF0.c`)
- `MASTER:func_80132444` (`score=22` in `src/ovl/MASTER/80132140.c`)
- `GYOZI:func_80139E4C` (`score=27` in `src/ovl/GYOZI/80139920.c`, `drafts/T-24019/GYOZI/func_80139E4C/body.c`)
- `VALEN:func_80134030` (`score=27` in `src/ovl/VALEN/80133C70.c`)
- `SHOUGATU:func_80139334` (`score=40` in `src/ovl/SHOUGATU/801390C0.c`, `drafts/T-14500/SHOUGATU/func_80139334/body.c`)
- `SHOUGATU:func_80140DC0` (`score=44` in `src/ovl/SHOUGATU/8013F190.c`, `drafts/T-14500/SHOUGATU/func_80140DC0/body.c`)
- `SHOUGATU:func_8013D820` (`score=60` in `src/ovl/SHOUGATU/8013D660.c`, `drafts/T-24056/func_8013D820.c`)
- `SHOUGATU:func_80137664` (`score=62` in `src/ovl/SHOUGATU/80136F90.c`, `drafts/T-14500/SHOUGATU/func_80137664/body.c`)

## Acceptance criteria

- [ ] Attack and close as many of these near misses as possible to `score=0` in `build/scratch/t24109/`
- [ ] Promote all `score=0` matches to unguarded default C and verify `tools/funcdiff.py --resolve`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md`
