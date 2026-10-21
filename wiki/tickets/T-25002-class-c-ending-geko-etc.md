---
id: T-25002
title: "Class C: ENDING func_801387B0, GEKO func_80133C50, ETC func_80144224"
status: In Progress
assignee: claude-c-m2
created: 2026-10-20
updated: 2026-10-20
links: [[[plan-remaining]], [[playbook]], src/ovl/ENDING/80137340.c, src/ovl/GEKO/801338F0.c, src/ovl/ETC/80143240.c]
---

## Goal

Match three class C functions from [[plan-remaining]] in worktree c-m2: ENDING `func_801387B0` (src/ovl/ENDING/80137340.c), GEKO `func_80133C50` (src/ovl/GEKO/801338F0.c), ETC `func_80144224` (src/ovl/ETC/80143240.c). Owned files: those three C files only.

## Acceptance criteria

- [ ] Each function matched (`funcdiff --resolve` MATCH), or guarded under about 30 rows with a T-25002 note, or INCLUDE_ASM with the draft in drafts/T-25002/
- [ ] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch
- [ ] `## T-25002` section in [[matching-notes]]

## Notes

## Comments
