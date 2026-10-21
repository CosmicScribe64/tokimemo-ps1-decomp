---
id: T-25006
title: "Class C: main srn_set, mod_set_gpu, func_8004A1A0"
status: In Progress
assignee: agent (branch c-m5)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[playbook]], src/main/8004C3F0.c, src/main/80049FF0.c]
---

## Goal

Match main `srn_set` (596 B) and `mod_set_gpu` (828 B) in src/main/8004C3F0.c and `func_8004A1A0` (628 B) in src/main/80049FF0.c in plain C, or leave a guard under about 30 rows.

## Acceptance criteria

- [ ] Each function: `funcdiff --resolve` MATCH (plain C), or a NON_MATCHING guard under ~30 rows with what is left, or INCLUDE_ASM with a draft in drafts/T-25006/.
- [ ] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch.
- [ ] `## T-25006` in [[matching-notes]].

## Notes

Drafts: drafts/T-14500, drafts/T-17000, drafts/T-17015 (srn_set), drafts/T-24006 (mod_set_gpu), drafts/T-24301.

## Comments
