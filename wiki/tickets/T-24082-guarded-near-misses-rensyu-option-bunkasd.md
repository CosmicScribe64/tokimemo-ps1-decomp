---
id: T-24082
title: Close guarded 4-8 row near misses in RENSYU, OPTION, and BUNKA_SD
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Three `#ifdef NON_MATCHING` guarded functions in non-reserved overlays sit within 4 to 8 diff rows of a byte match:

- `RENSYU:func_80133628` (404 B in `src/ovl/RENSYU/80132000.c`, 4 diff rows — saved-register permutation `$s0/$s1/$s2`)
- `OPTION:func_8013394C` (648 B in `src/ovl/OPTION/80132000.c`, 8 diff rows)
- `BUNKA_SD:func_80134418` (464 B in `src/ovl/BUNKA_SD/80132000.c`, 8 diff rows)

## Acceptance criteria

- [ ] Close `RENSYU:func_80133628` (404 B) to `score=0`
- [ ] Close `OPTION:func_8013394C` (648 B) to `score=0`
- [ ] Close `BUNKA_SD:func_80134418` (464 B) to `score=0`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Subagent scratch workspace: `build/scratch/t24082/`
