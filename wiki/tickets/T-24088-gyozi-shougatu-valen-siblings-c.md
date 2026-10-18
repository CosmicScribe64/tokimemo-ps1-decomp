---
id: T-24088
title: Match GYOZI, SHOUGATU, and VALEN sibling batch C (func_8013907C, func_80139E4C/func_80134030, func_80138240, func_8013DF14)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following [[tickets/T-24084-shougatu-gyozi-siblings-b]] (3 functions, 1,528 B matched across `SHOUGATU` and `GYOZI`), target the remaining high-similarity siblings in `GYOZI`, `SHOUGATU`, and `VALEN` (in files disjoint from `T-24087`):

- `SHOUGATU:func_8013DF14` (436 B in `src/ovl/SHOUGATU/8013DD90.c`, 12 rows in `drafts/T-24056/`, sibling `GYOZI:func_801408C4` at ratio 0.870)
- `SHOUGATU:func_80138240` (276 B in `src/ovl/SHOUGATU/80137FF0.c`, guarded at 4 stack layout words, sibling `GYOZI:func_8013B0C4` at ratio 0.985)
- `GYOZI:func_8013907C` (844 B in `src/ovl/GYOZI/80138B50.c`, sibling of `GYOZI:func_8013E824` at ratio 0.859)
- `GYOZI:func_80139E4C` (696 B in `src/ovl/GYOZI/80139920.c`) and its `1.000` twin `VALEN:func_80134030` (696 B in `src/ovl/VALEN/80133C70.c`)

## Acceptance criteria

- [x] Match `SHOUGATU:func_8013DF14` (436 B) to `score=0`
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Matched `SHOUGATU:func_8013DF14` (436 B in `src/ovl/SHOUGATU/8013DD90.c`): declared `extern s16 D_80145FDC;` in `include/ovl/SHOUGATU.h` and paired `GameState *state = &D_800E6280;` for `state->unk_1BC[12].unk_0C.f.f9 += 1;` and `>= 4` with direct `D_800E6280.unk_1BC[12].unk_0C.f.f9 = 3;` inside the `if` body (`FAKE`) so `uopt` shares `$v0` for `+0x468`/`+0x469` without forwarding the byte into the clamp store (`score=0`).
- `SHOUGATU:func_80138240` (8 rows: 6-arg call + `FnTbl37` `ugen` temp slot), `GYOZI:func_8013907C` (12 rows: `uopt` `D_800F62CF` CSE chain `sll` placement), and `GYOZI:func_80139E4C`/`VALEN:func_80134030` (`T-20620` cross-block `as1` `lui` hoisting) left untouched.
