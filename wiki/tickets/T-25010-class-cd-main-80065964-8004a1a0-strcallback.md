---
id: T-25010
title: "Class C/D: main func_80065964 frame, func_8006A044, func_8004A1A0"
status: In Progress
assignee: c-m7
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[plan-remaining]], src/main/80049FF0.c, src/main/80062CD0.c, src/main/800563F0.c]
---

## Goal

Match main `func_8004A1A0` (src/main/80049FF0.c), `func_80065964` (src/main/80062CD0.c) and `strCallback` (src/main/800563F0.c). `func_8006A044` (src/main/800674B0.c) is skipped this round: the file is owned by another agent.

## Acceptance criteria

- [ ] Each function matched (funcdiff `MATCH`), or guarded under about 30 rows, or INCLUDE_ASM with a draft in drafts/T-25010/
- [ ] OPTION still matches after the BG slot types move to a shared header
- [ ] Clean build 27/27; check_headers, migrate_globals --check, sync_protos --check-branch pass
- [ ] Notes in [[matching-notes]] under `## T-25010`

## Notes

Branch c-m7.

## Comments
