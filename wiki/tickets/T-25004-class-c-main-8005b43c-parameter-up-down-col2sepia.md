---
id: T-25004
title: "Class C: main func_8005B43C, parameter_up_down, col2sepia"
status: Done
assignee: agent (c-m4)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[playbook]], src/main/8005A0B0.c, src/main/800737A0.c, src/main/8004AD60.c]
---

## Goal

Match main `func_8005B43C` (src/main/8005A0B0.c, 920 B), `parameter_up_down` (src/main/800737A0.c, 1228 B) and `col2sepia` (src/main/8004AD60.c, 580 B). Drafts: drafts/T-14500, drafts/T-17000, drafts/T-17013, drafts/T-17016, drafts/T-24015, drafts/T-24301, drafts/T-24303.

## Acceptance criteria

- [x] Each function matched in plain C, or guarded under about 30 rows with ticket and what is left, or INCLUDE_ASM with a draft in drafts/T-25004/
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch
- [x] Notes in [[matching-notes]] under T-25004

## Notes

Branch c-m4 (from w2-good).

## Comments

2026-10-20: `col2sepia` matched (plain C). `func_8005B43C` guarded at 3 rows, `parameter_up_down` guarded at 11 rows. Clean build 27/27; check_headers, migrate_globals --check, sync_protos --check-branch pass. Notes in [[matching-notes]] under T-25004. In Review.
- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): main col2sepia MATCH (linked); func_8005B43C guard 3 rows, parameter_up_down guard 11 rows. FAKE comments ticketed; no inline asm or game data; merged with a clean 27/27 build and header/global/prototype checks. Done.
