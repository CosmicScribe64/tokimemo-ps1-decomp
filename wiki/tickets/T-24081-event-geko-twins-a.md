---
id: T-24081
title: Match EVENT and GEKO 1.000 twin cluster A (func_800FAA04/func_8013A820, func_800FA25C/func_80139E1C)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

`tools/siblings.py` identifies two untouched `1.000` opcode-skeleton twin pairs between `EVENT` (`src/ovl/EVENT/800F9600.c`) and `GEKO` (`src/ovl/GEKO/80138D18.c`), each with a high-similarity matched sibling in `EVENT`:

- `EVENT:func_800FAA04` (508 B) + `GEKO:func_8013A820` (508 B), ratio `1.000`, matched sibling `EVENT:func_8010564C` (ratio 0.798 in `src/ovl/EVENT/800F9600.c`)
- `EVENT:func_800FA25C` (808 B) + `GEKO:func_80139E1C` (808 B), ratio `1.000`, matched sibling `EVENT:func_80104E20` (ratio 0.678 in `src/ovl/EVENT/800F9600.c`)

## Acceptance criteria

- [ ] Match `EVENT:func_800FAA04` from `EVENT:func_8010564C` and port to `GEKO:func_8013A820`
- [ ] Match `EVENT:func_800FA25C` from `EVENT:func_80104E20` and port to `GEKO:func_80139E1C`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Subagent scratch workspace: `build/scratch/t24081/`
