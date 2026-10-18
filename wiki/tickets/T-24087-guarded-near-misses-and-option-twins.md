---
id: T-24087
title: Close guarded near misses and twin pairs in OPTION, VALEN, TAIIKU, and main
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following [[tickets/T-24082-guarded-near-misses-rensyu-option-bunkasd]] (2 of 3 guarded near misses matched, 540 B), target the next set of short guarded near-misses and high-similarity twins in disjoint files:

- `OPTION:func_80139E3C` (196 B in `src/ovl/OPTION/801389A0.c`, guarded at 5 rows)
- `OPTION:func_801378DC` (832 B) and `OPTION:func_80138080` (836 B) in `src/ovl/OPTION/80132000.c` (1,668 B twin pair, ratio 0.993)
- `VALEN:func_80132D0C` (892 B in `src/ovl/VALEN/80132760.c`, guarded at 7 rows; sibling of newly matched `GYOZI:func_8013E824` at ratio 0.844)
- `TAIIKU:func_8013D114` (528 B in `src/ovl/TAIIKU/80138950.c`, guarded at 2 instructions missing / 12 lines)
- `main:func_8006DF78` (in `src/main/8006CB30.c`, guarded at 1 row) and `main:func_8004556C` (in `src/main/800451D0.c`, guarded at 4 lines)

## Acceptance criteria

- [ ] Close as many of the target functions as possible to `score=0` in `build/scratch/t24087/`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Subagent scratch workspace: `build/scratch/t24087/`
