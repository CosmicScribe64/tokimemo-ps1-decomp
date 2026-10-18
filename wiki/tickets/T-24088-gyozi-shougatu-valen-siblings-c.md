---
id: T-24088
title: Match GYOZI, SHOUGATU, and VALEN sibling batch C (func_8013907C, func_80139E4C/func_80134030, func_80138240, func_8013DF14)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following [[tickets/T-24084-shougatu-gyozi-siblings-b]] (3 functions, 1,528 B matched across `SHOUGATU` and `GYOZI`, including `GYOZI:func_8013E824`), target the remaining high-similarity siblings in `GYOZI`, `SHOUGATU`, and `VALEN` (in files disjoint from `T-24087`):

- `GYOZI:func_8013907C` (844 B in `src/ovl/GYOZI/80138B50.c`, sibling of newly matched `GYOZI:func_8013E824` at ratio 0.859; draft in `drafts/T-24019/`)
- `GYOZI:func_80139E4C` (696 B in `src/ovl/GYOZI/80139920.c`) and its `1.000` twin `VALEN:func_80134030` (696 B in `src/ovl/VALEN/80133C70.c`) (1,392 B pair, sibling `GEKO:func_801355E8` at ratio 0.951; drafts in `drafts/T-24019/` and `drafts/T-24033/`)
- `SHOUGATU:func_80138240` (276 B in `src/ovl/SHOUGATU/80137FF0.c`, guarded at 4 stack layout words, sibling `GYOZI:func_8013B0C4` at ratio 0.985)
- `SHOUGATU:func_8013DF14` (436 B in `src/ovl/SHOUGATU/8013DD90.c`, 12 rows in `drafts/T-24056/`, sibling `GYOZI:func_801408C4` at ratio 0.870)

## Acceptance criteria

- [ ] Close as many of the target functions as possible to `score=0` in `build/scratch/t24088/`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Subagent scratch workspace: `build/scratch/t24088/`
