---
id: T-25008
title: "Closers: main func_8005B43C, func_8006BA40, parameter_up_down"
status: Done
assignee: n1
created: 2026-10-20
updated: 2026-10-20
links: ["[[matching-notes]]", "[[tickets/T-25003-class-c-main-8006ba40-80043510-8004443c]]", "[[tickets/T-25004-class-c-main-8005b43c-parameter-up-down-col2sepia]]", src/main/8005A0B0.c, src/main/800674B0.c, src/main/800737A0.c]
---

## Goal

Close the three guarded main functions left by T-25003/T-25004: `func_8005B43C` (3 rows), `func_8006BA40` (7 rows), `parameter_up_down` (11 rows).

## Acceptance criteria

- [x] Each function matched in plain C, or its guard note updated with rows and what was tried
- [x] `## T-25008` in [[matching-notes]]
- [ ] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch

## Notes

Branch n1.

## Comments

2026-10-20 (n1): `func_8006BA40` MATCH (`(u8)` cast on the nibble test spends the missing ugen slot; the T-25003 `(u16)step` cast removed). `func_8005B43C` guarded at 3 rows, `parameter_up_down` at 11 rows; notes in [[matching-notes]] T-25008. In Review.
- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): main func_8006BA40 MATCH (linked); func_8005B43C 3 rows and parameter_up_down 11 rows stay guarded with updated notes. FAKE comments ticketed; no inline asm or game data; merged with a clean 27/27 build and header/global/prototype checks. Done.
