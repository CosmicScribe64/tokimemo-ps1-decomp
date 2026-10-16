---
id: T-17105
title: Wave 3 package w3-rpg-bat-p05
status: In Review
assignee: w3-rpg-bat-p05 agent
created: 2026-10-16
updated: 2026-10-16
links: [[[tickets/T-17100-types-first-rpg_bat]]]
---

## Goal

Match the work list of package w3-rpg-bat-p05 in `src/ovl/RPG_BAT/80157AE0.c` and `src/ovl/RPG_BAT/8015A1F0.c` (wave 3, from [[tickets/T-17100-types-first-rpg_bat]]): RPG_BAT `func_8015A8B4` (236 B), `func_8015A744` (368 B, guarded draft), `func_80159B40` (568 B), `func_80158C4C` (1,112 B), `func_80159214` (1,080 B).

## Acceptance criteria

- [x] Each matched function is plain C, verified by the RPG_BAT sha1 in ninja, committed one per commit.
- [x] Near misses under about 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17105 comment; others go back to `INCLUDE_ASM`.
- [x] Clean build 27/27 sha1 OK; check_headers, migrate_globals --check and sync_protos --check-branch pass.
- [x] Notes in [[matching-notes]]; compiler-gap rows in [[data/t0018-cases]].

## Notes

Branch `w3-rpg-bat-p05` from `w2-good`. Owned C files: the two above only (plus `include/`, `config/`, `wiki/`).

## Comments

- 2026-10-16: `func_80159B40` matched in plain C (528 bytes, `funcdiff --resolve` MATCH and RPG_BAT sha1 OK, commit "T-17105: match RPG_BAT:func_80159B40"): the redraft's instructions matched at once, only the frame differed; the three scalars are declared between the second string table and the flag table and one counter serves both loops. `D_8015F000` typed as `RpgWordTbl4` in `RPG_BAT.h`.
- 2026-10-16: guarded near misses, all with a `NON_MATCHING: T-17105` comment: `func_80158C4C` 4 rows (from 77), `func_8015A744` 12 rows (from 17), `func_80159214` 16 rows (from 178), `func_8015A8B4` 28 rows (from 33; the starting draft was the T-17100 `hand.c`). `func_8015A744` and `func_8015A8B4` share one open gap (promoted `D_8015EC68`/`D_8015EC54` with register copies), rows in [[data/t0018-cases]]; notes in [[matching-notes]]. Three register-cheat-sheet rows added.
- 2026-10-16: clean rebuild (`rm -rf build && configure.py && ninja`) and the header, globals and prototype checks: see the log entry. Code review against `CODING_STANDARDS.md` still to do.
