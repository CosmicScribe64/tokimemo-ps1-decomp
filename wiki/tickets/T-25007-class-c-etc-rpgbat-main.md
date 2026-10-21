---
id: T-25007
title: "Class C: ETC func_80146D4C, RPG_BAT func_8014BFE4, main func_8005938C"
status: In Progress
assignee: agent c-m6
created: 2026-10-20
updated: 2026-10-20
links: [[[plan-remaining]], [[playbook]], [[matching-notes]], src/ovl/ETC/80146400.c, src/ovl/RPG_BAT/8014ADD0.c, src/main/80058D20.c]
---

## Goal

Match ETC `func_80146D4C` (1076 B), RPG_BAT `func_8014BFE4` (892 B) and main `func_8005938C` (764 B), class C of [[plan-remaining]]. Owned files: `src/ovl/ETC/80146400.c`, `src/ovl/RPG_BAT/8014ADD0.c`, `src/main/80058D20.c`.

## Acceptance criteria

- [ ] Each function matched in plain C, or guarded under about 30 rows with a NON_MATCHING note, or INCLUDE_ASM with the draft in `drafts/T-25007/`
- [ ] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch
- [ ] `## T-25007` section in [[matching-notes]]

## Notes

Branch c-m6 from w2-good.

## Comments
