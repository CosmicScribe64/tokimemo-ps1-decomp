---
id: T-24081
title: Match EVENT and GEKO 1.000 twin cluster A (func_800FAA04/func_8013A820, func_800FA25C/func_80139E1C)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

`tools/siblings.py` identifies two untouched `1.000` opcode-skeleton twin pairs between `EVENT` (`src/ovl/EVENT/800F9680.c`) and `GEKO` (`src/ovl/GEKO/80139910.c`), each with a high-similarity matched sibling in `EVENT`:

- `EVENT:func_800FAA04` (508 B) + `GEKO:func_8013A820` (508 B), ratio `1.000`, matched sibling `EVENT:func_8010564C` (ratio 0.798)
- `EVENT:func_800FA25C` (808 B) + `GEKO:func_80139E1C` (808 B), ratio `1.000`, matched sibling `EVENT:func_80104E20` (ratio 0.678)

## Acceptance criteria

- [x] Match `EVENT:func_800FAA04` from `EVENT:func_8010564C` and port to `GEKO:func_8013A820`
- [x] Match `EVENT:func_800FA25C` from `EVENT:func_80104E20` and port to `GEKO:func_80139E1C`
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- `EVENT:func_800FAA04` (508 B): `MATCH`
- `GEKO:func_8013A820` (508 B): `MATCH`
- `EVENT:func_800FA25C` (808 B): `MATCH`
- `GEKO:func_80139E1C` (808 B): `MATCH`
- Total: +4 functions / +2,632 bytes verified default C.
- Code review against `CODING_STANDARDS.md`: moved `typedef struct EventRec24` from `src/ovl/EVENT/800F9680.c` into `include/ovl/EVENT.h` (rule 3: typedefs go in `.h` files, not `.c` files) and verified all `/* FAKE: ... T-24081 */` comments and `funcdiff.py --resolve` proofs.
