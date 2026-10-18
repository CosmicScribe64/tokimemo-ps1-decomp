---
id: T-24085
title: Match EVENT and GEKO twin cluster B (func_80105DF8/func_8013E950, func_80109404/func_801406CC)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following the completion of [[tickets/T-24081-event-geko-twins-a]] (4 functions, 2,632 B matched across `EVENT` and `GEKO`), the next two high-similarity twin pairs between `EVENT` and `GEKO` are:

- `EVENT:func_80105DF8` (856 B in `src/ovl/EVENT/800F9680.c`) + `GEKO:func_8013E950` (856 B in `src/ovl/GEKO/8013D090.c`), ratio `1.000` (1,712 B total)
- `EVENT:func_80109404` (768 B in `src/ovl/EVENT/800F9680.c`) + `GEKO:func_801406CC` (780 B in `src/ovl/GEKO/8013D090.c`), ratio `0.992` (1,548 B total)

## Acceptance criteria

- [ ] Match `EVENT:func_80105DF8` and `GEKO:func_8013E950` to `score=0`
- [ ] Match `EVENT:func_80109404` and `GEKO:func_801406CC` to `score=0`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Subagent scratch workspace: `build/scratch/t24085/`
