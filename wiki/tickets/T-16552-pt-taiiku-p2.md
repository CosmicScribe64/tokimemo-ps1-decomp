---
id: T-16552
title: "Types-first pilot package pt-taiiku-p2"
status: In Progress
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of `src/ovl/TAIIKU/80138950.c` from the types-first redrafts of [[tickets/T-16550-taiiku-types-first]]: `func_80138DF8`, `func_80138AA8`, `func_80139FE4`, `func_8013B3B0`, `func_8013B608`, `func_8013BCEC`, `func_8013B78C`, `func_80139AB0`, `func_8013B94C` (9 functions, 6,196 bytes), in that order, with the new types in `include/ovl/TAIIKU.h` instead of byte-offset casts.

## Acceptance criteria

- [ ] Each function is matched (unit sha1 OK, no guard) or left under `#ifdef NON_MATCHING` (under about 30 differing lines) or as plain `INCLUDE_ASM`.
- [ ] Compiler-gap rows in `wiki/data/t0018-cases.md`.
- [ ] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [ ] Section T-16552 in `wiki/matching-notes.md`.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p2`, branch `pt-taiiku-p2` from `w2-good`. Ownership: `src/ovl/TAIIKU/80138950.c`; `include/`, `config/`, `wiki/` where needed.

## Comments
