---
id: T-16554
title: "Types-first pilot package pt-taiiku-p4"
status: In Progress
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of package pt-taiiku-p4 starting from the T-16550 types-first redrafts (`drafts/T-16550/TAIIKU/<func>/`, best draft per function): `func_80140A90`, `func_80132970`, `func_8014357C`, `func_8014280C`, `func_80143F50`, `func_80142FB8`, `func_80143DD0`. Owned C files: `src/ovl/TAIIKU/80140810.c`, `80132000.c`, `80142240.c`.

## Acceptance criteria

- [ ] Each function matched (unit sha1 OK, no guard) or kept under `#ifdef NON_MATCHING` (< about 30 differing lines) with a ticket comment, else plain `INCLUDE_ASM`.
- [ ] Types extended in `include/ovl/TAIIKU.h` instead of byte-offset casts where the build allows.
- [ ] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [ ] Short T-16554 section in [[matching-notes]]; compiler-gap rows in `wiki/data/t0018-cases.md`.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p4`, branch `pt-taiiku-p4` from `w2-good`. Time box: stop starting new functions after about 2.5 hours. Pilot stage B (measured).

## Comments
