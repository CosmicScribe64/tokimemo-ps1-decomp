---
id: T-25006
title: "Class C: main srn_set, mod_set_gpu, func_8004A1A0"
status: In Review
assignee: agent (branch c-m5)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[playbook]], src/main/8004C3F0.c, src/main/80049FF0.c]
---

## Goal

Match main `srn_set` (596 B) and `mod_set_gpu` (828 B) in src/main/8004C3F0.c and `func_8004A1A0` (628 B) in src/main/80049FF0.c in plain C, or leave a guard under about 30 rows.

## Acceptance criteria

- [x] Each function: `funcdiff --resolve` MATCH (plain C), or a NON_MATCHING guard under ~30 rows with what is left, or INCLUDE_ASM with a draft in drafts/T-25006/.
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch.
- [x] `## T-25006` in [[matching-notes]].

## Notes

Drafts: drafts/T-14500, drafts/T-17000, drafts/T-17015 (srn_set), drafts/T-24006 (mod_set_gpu), drafts/T-24301.

## Comments

2026-10-20: `srn_set` matched (plain C, one FAKE unused local; `SRN_REC` macro and the record-load rule in [[matching-notes]]). `mod_set_gpu` guarded at 28 rows (`ModRec14` in game.h, u16 view of search_tpage_multi inside the guard). `func_8004A1A0` stays INCLUDE_ASM, draft drafts/T-25006/main/func_8004A1A0.c (76 rows). Clean build 27/27, check_headers, migrate_globals --check and sync_protos --check-branch pass. In Review.
