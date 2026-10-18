---
id: T-24083
title: Match DATE 80132000.c sibling batch B (func_80147BB0, func_8013A94C, func_8014EF60, func_80151110)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following the completion of [[tickets/T-24079-date-80132000-event-siblings]] (4 functions, 2,316 B matched in `src/ovl/DATE/80132000.c`), the next high-similarity unmatched sibling targets in `src/ovl/DATE/80132000.c` are:

- `DATE:func_8013A94C` (1,208 B, matched sibling `EVENT:func_80117610` at ratio 0.659 in `src/ovl/EVENT/8010FE10.c`)
- `DATE:func_8013EB8C` (688 B in `src/ovl/DATE/80132000.c`)
- `DATE:func_801407D0` (684 B in `src/ovl/DATE/80132000.c`)

## Acceptance criteria

- [x] Match `DATE:func_8013A94C`, `DATE:func_8013EB8C`, and `DATE:func_801407D0` to `score=0`
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Matched 3 functions (+2,580 B):
  - `DATE:func_8013A94C` (1,208 B in `src/ovl/DATE/80132000.c`): `MATCH`
  - `DATE:func_8013EB8C` (688 B in `src/ovl/DATE/80132000.c`): `MATCH`
  - `DATE:func_801407D0` (684 B in `src/ovl/DATE/80132000.c`): `MATCH`
