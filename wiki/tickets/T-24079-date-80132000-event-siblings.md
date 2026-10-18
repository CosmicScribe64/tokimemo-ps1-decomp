---
id: T-24079
title: Match DATE 80132000.c siblings from matched EVENT 8010FE10.c
status: Done
assignee: Antigravity
created: 2026-10-18
updated: 2026-10-18
links: ["[[date-family-results]]", "[[tickets/T-22017-w4-p17]]", "[[tickets/T-22024-w4-p24]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal

Match the `DATE` `src/ovl/DATE/80132000.c` functions `func_8013C410` (532 B), `func_8013EFA0` (484 B), `func_8013F214` (492 B), and `func_8013B488` (808 B) using their matched `EVENT` `src/ovl/EVENT/8010FE10.c` siblings (`func_80118DAC`, `func_8011AC18`, `func_8011AD7C`, `func_80118010`).

## Acceptance criteria

- [x] Each matched function verified as unguarded default C with `funcdiff.py --resolve` literal-byte proof
- [x] Clean build 27/27 hashes (`ninja`), `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch`
- [x] Code review against `CODING_STANDARDS.md` with all findings resolved before moving to Done

## Notes

- `DATE:func_8013B488` (808 B): `MATCH`
- `DATE:func_8013C410` (532 B): `MATCH`
- `DATE:func_8013EFA0` (484 B): `MATCH`
- `DATE:func_8013F214` (492 B): `MATCH`
- Total: +4 functions / +2,316 bytes verified default C.
- Code review against `CODING_STANDARDS.md`: renamed `m2c` local names (`sp3A` -> `sel`, `sp39` -> `rank`, `temp_v0` -> `r2` in `func_8013B488`; `var_v0` -> `i`, `temp_v1` -> `val`, `sp18` -> `tbl`, `loop_2`/`loop_17` -> `loop1`/`loop2` in `func_8013C410`) and verified all `/* FAKE: ... T-24079 */` comments and `funcdiff.py --resolve` proofs.
