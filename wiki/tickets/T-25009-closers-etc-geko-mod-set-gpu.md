---
id: T-25009
title: "Closers: ETC func_80146D4C, GEKO func_80133C50, main mod_set_gpu"
status: In Progress
assignee: n2
created: 2026-10-20
updated: 2026-10-20
links: ["[[matching-notes]]", "[[tickets/T-25002-class-c-ending-geko-etc]]", "[[tickets/T-25006-class-c-main-srn-set-mod-set-gpu-8004a1a0]]", "[[tickets/T-25007-class-c-etc-rpgbat-main]]"]
---

## Goal

Close three guarded functions: ETC `func_80146D4C` (12 rows), GEKO `func_80133C50` (13 rows), main `mod_set_gpu` (28 rows). Branch n2.

## Acceptance criteria

- [ ] Each function matched in plain C, or its guard note updated with T-25009, rows and what was tried
- [ ] `## T-25009` in [[matching-notes]]
- [ ] Clean build 27/27; check_headers, migrate_globals --check, sync_protos --check-branch pass

## Notes

## Comments
