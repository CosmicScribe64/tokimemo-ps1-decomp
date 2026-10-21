---
id: T-25001
title: "Class C: main sndisp, strCallback, func_80065964"
status: In Review
assignee: c-m1
created: 2026-10-20
updated: 2026-10-20
links: [[[plan-remaining]], [[data/remaining-2026-10-20]], [[matching-notes]]]
---

## Goal

Match three class C main functions from their good drafts: `sndisp` (src/main/8004F870.c, 388 B), `strCallback` (src/main/800563F0.c, 640 B), `func_80065964` (src/main/80062CD0.c, 424 B).

## Acceptance criteria

- [x] Each function matched in plain C, or guarded under about 30 rows with a T-25001 comment, or left INCLUDE_ASM with its draft in drafts/T-25001/
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch pass
- [x] `## T-25001` section in [[matching-notes]]

## Notes

Starting drafts: drafts/T-14500/main/<func> (scores 98, 139, 160).

## Comments

2026-10-20: sndisp guarded at 2 rows (only the template address of `pos[]`: the object's two .data regions); strCallback (75 rows) and func_80065964 left INCLUDE_ASM, drafts in drafts/T-25001/main/. Clean build 27/27, headers, globals and the prototype branch check pass. In Review. Details: [[matching-notes]] T-25001.
