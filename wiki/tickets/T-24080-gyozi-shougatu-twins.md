---
id: T-24080
title: Match GYOZI and SHOUGATU 1.000 twin pair cluster (func_80136488/func_80134CB4, func_801378D0/func_80136204)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

`tools/siblings.py` identifies two untouched `1.000` opcode-skeleton twin pairs between `GYOZI` (`src/ovl/GYOZI/80136220.c`, `src/ovl/GYOZI/801372B0.c`) and `SHOUGATU` (`src/ovl/SHOUGATU/80134890.c`, `src/ovl/SHOUGATU/80135B90.c`):

- `GYOZI:func_80136488` (604 B in `src/ovl/GYOZI/80136220.c`) and `SHOUGATU:func_80134CB4` (604 B in `src/ovl/SHOUGATU/80134890.c`)
- `GYOZI:func_801378D0` (316 B in `src/ovl/GYOZI/801372B0.c`) and `SHOUGATU:func_80136204` (316 B in `src/ovl/SHOUGATU/80135B90.c`)

## Acceptance criteria

- [x] Close the remaining diffs on `GYOZI:func_80136488` and port the match to `SHOUGATU:func_80134CB4`
- [x] Match `SHOUGATU:func_80136204` (from sibling `SHOUGATU:func_80135AC4`) and port the match to `GYOZI:func_801378D0`
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- `GYOZI:func_80136488` (604 B): `MATCH`
- `SHOUGATU:func_80134CB4` (604 B): `MATCH`
- `SHOUGATU:func_80136204` (316 B): `MATCH`
- `GYOZI:func_801378D0` (316 B): `MATCH`
- Total: +4 functions / +1,840 bytes verified default C.
- Code review against `CODING_STANDARDS.md`: renamed `var_s0` -> `i` and `temp_v0` -> `r`, verified `/* FAKE: ... T-24080 */` comments and `funcdiff.py --resolve` proofs.
