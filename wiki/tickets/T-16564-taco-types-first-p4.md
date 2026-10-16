---
id: T-16564
title: "Types-first pilot package pt-taco-p4"
status: In Progress
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16560-taco-types-first]], [[taco-family-results]]
---

## Goal

Match the TACO functions of package pt-taco-p4 starting from the T-16560 types-first redrafts (`drafts/T-16560/TACO/<func>/`, best draft per function): `func_801563E4`, `func_80154960`, `func_80138D08`, `func_801557E8`, `func_80138B2C`, `func_80138964`, `func_80137D00`, `func_801553AC`. Owned C files: `src/ovl/TACO/80154960.c`, `80137A60.c`, `80138890.c`.

## Acceptance criteria

- [ ] Each function matched (unit sha1 OK, no guard) or kept under `#ifdef NON_MATCHING` (< about 30 differing lines) with a ticket comment, else plain `INCLUDE_ASM`.
- [ ] Types extended in `include/ovl/TACO.h` instead of byte-offset casts where the build allows.
- [ ] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [ ] Short T-16564 section in [[matching-notes]]; compiler-gap rows in `wiki/data/t0018-cases.md`.

## Notes

Worktree `tokimemo-wt/pt-taco-p4`, branch `pt-taco-p4` from `w2-good`. Time box: stop starting new functions after about 2.5 hours. Pilot stage B (measured); start 2026-10-16 04:58 UTC.

## Comments
