---
id: T-24083
title: Match DATE 80132000.c sibling batch B (func_80147BB0, func_8013A94C, func_8014EF60, func_80151110)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following the completion of [[tickets/T-24079-date-80132000-event-siblings]] (4 functions, 2,316 B matched in `src/ovl/DATE/80132000.c`), the next high-similarity unmatched sibling targets in `src/ovl/DATE/80132000.c` are:

- `DATE:func_80147BB0` (468 B, matched sibling `DATE:func_80146FE4` at ratio 0.870 in `src/ovl/DATE/80132000.c`)
- `DATE:func_8014EF60` (580 B, matched sibling `DATE:func_8014C1A8` at ratio 0.720 in `src/ovl/DATE/80132000.c`)
- `DATE:func_80151110` (632 B, matched sibling `DATE:func_8014EA6C` at ratio 0.733 in `src/ovl/DATE/80132000.c`)
- `DATE:func_8013A94C` (1,208 B, matched sibling `EVENT:func_80117610` at ratio 0.659 in `src/ovl/EVENT/8010FE10.c`)

## Acceptance criteria

- [ ] Match as many of `DATE:func_80147BB0`, `DATE:func_8014EF60`, `DATE:func_80151110`, and `DATE:func_8013A94C` as possible to `score=0`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Subagent scratch workspace: `build/scratch/t24083/`
