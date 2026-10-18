---
id: T-24090
title: Match GEKO and EVENT twin pair and guarded near misses C (func_80137120/func_801372D4, func_80102E54, func_80133AA0, func_8013F894)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following [[tickets/T-24085-event-geko-twins-b]] (2 matches, 1,712 B across `EVENT` and `GEKO`), target the next `GEKO` and `EVENT` twin pair and guarded near-misses:

- `GEKO:func_80137120` (436 B) and `GEKO:func_801372D4` (468 B) in `src/ovl/GEKO/80134D10.c` (904 B twin pair, ratio 0.938)
- `EVENT:func_80102E54` (672 B in `src/ovl/EVENT/801024B0.c`, guarded at 6 edits, sibling `DATE:func_801556D8` at ratio 0.859)
- `GEKO:func_8013F894` (`src/ovl/GEKO/8013F360.c`, guarded at 7 rows, sibling `EVENT:func_80108DD4`)
- `GEKO:func_80133AA0` (`src/ovl/GEKO/801338F0.c`, guarded at 8 rows)

## Acceptance criteria

- [x] Close as many of the target functions as possible to `score=0` in `build/scratch/t24090/`
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Matched 5 functions (+2,308 B):
  - `GEKO:func_80137120` (436 B in `src/ovl/GEKO/80134D10.c`): `MATCH`
  - `GEKO:func_801372D4` (468 B in `src/ovl/GEKO/80134D10.c`): `MATCH`
  - `GEKO:func_80140040` (300 B in `src/ovl/GEKO/8013F360.c`): `MATCH`
  - `GEKO:func_80133AA0` (432 B in `src/ovl/GEKO/801338F0.c`): `MATCH`
  - `EVENT:func_80102E54` (672 B in `src/ovl/EVENT/801024B0.c`): `MATCH` (via `s32 func_8004C6B0` `O.BIN` return-type discovery matching `SLPM_86.053` `func_800850D4`)
- `GEKO:func_8013F894` reduced from 7 rows to 2 rows (`$t8/$t9` swap after `GsGetWorkBase() + 4` load).
