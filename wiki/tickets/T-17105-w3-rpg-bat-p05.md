---
id: T-17105
title: Wave 3 package w3-rpg-bat-p05
status: In Progress
assignee: w3-rpg-bat-p05 agent
created: 2026-10-16
updated: 2026-10-16
links: [[[tickets/T-17100-types-first-rpg_bat]]]
---

## Goal

Match the work list of package w3-rpg-bat-p05 in `src/ovl/RPG_BAT/80157AE0.c` and `src/ovl/RPG_BAT/8015A1F0.c` (wave 3, from [[tickets/T-17100-types-first-rpg_bat]]): RPG_BAT `func_8015A8B4` (236 B), `func_8015A744` (368 B, guarded draft), `func_80159B40` (568 B), `func_80158C4C` (1,112 B), `func_80159214` (1,080 B).

## Acceptance criteria

- [ ] Each matched function is plain C, verified by the RPG_BAT sha1 in ninja, committed one per commit.
- [ ] Near misses under about 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17105 comment; others go back to `INCLUDE_ASM`.
- [ ] Clean build 27/27 sha1 OK; check_headers, migrate_globals --check and sync_protos --check-branch pass.
- [ ] Notes in [[matching-notes]]; compiler-gap rows in [[data/t0018-cases]].

## Notes

Branch `w3-rpg-bat-p05` from `w2-good`. Owned C files: the two above only (plus `include/`, `config/`, `wiki/`).

## Comments
