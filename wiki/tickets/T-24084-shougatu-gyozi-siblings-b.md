---
id: T-24084
title: Match SHOUGATU and GYOZI sibling batch B (func_801359FC, func_80132148/func_80134100, func_801345BC/func_80135EB0, func_80137480)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following the completion of [[tickets/T-24080-gyozi-shougatu-twins]] (4 functions, 1,840 B matched across `GYOZI` and `SHOUGATU`), the next short/medium targets in `SHOUGATU` and `GYOZI` are:

- `SHOUGATU:func_801359FC` (276 B in `src/ovl/SHOUGATU/80134890.c`, sibling of already-matched `GYOZI:func_80137188` in `src/ovl/GYOZI/80136220.c`)
- `SHOUGATU:func_801345BC` (364 B in `src/ovl/SHOUGATU/80132000.c`) and `GYOZI:func_80135EB0` (340 B in `src/ovl/GYOZI/801343A4.c`)
- `SHOUGATU:func_80132148` (572 B in `src/ovl/SHOUGATU/80132000.c`) and `GYOZI:func_80134100` (580 B in `src/ovl/GYOZI/80132000.c`)
- `SHOUGATU:func_80137480` (428 B in `src/ovl/SHOUGATU/80136E88.c`)

## Acceptance criteria

- [ ] Match `SHOUGATU:func_801359FC` from `GYOZI:func_80137188` and close as many of `func_801345BC`/`func_80135EB0`, `func_80132148`/`func_80134100`, and `func_80137480` as possible to `score=0`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Subagent scratch workspace: `build/scratch/t24084/`
