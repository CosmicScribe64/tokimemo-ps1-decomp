---
id: T-16551
title: "Types-first pilot package pt-taiiku-p1"
status: In Progress
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of `src/ovl/TAIIKU/80133C80.c` (game A) from the types-first redrafts of [[tickets/T-16550-taiiku-types-first]], in the order of the package list: `func_801346FC`, `func_80136BB4`, `func_801368B8`, `func_80138224`, `func_80134060`, `func_80133CFC`, `func_80135480`, `func_80137544`. Pilot stage B: measured agent time.

## Acceptance criteria

- [ ] Each function in the list worked in order; matches committed one by one as plain C.
- [ ] Unmatched drafts kept under `#ifdef NON_MATCHING` only when under about 30 differing lines, with a `NON_MATCHING: T-16551` comment; otherwise plain `INCLUDE_ASM`.
- [ ] Compiler-gap rows in `wiki/data/t0018-cases.md`.
- [ ] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [ ] Section in `wiki/matching-notes.md`; ticket In Review.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p1`, branch `pt-taiiku-p1` from `w2-good`. Ownership: `src/ovl/TAIIKU/80133C80.c`; `include/`, `config/`, `wiki/` where needed.

## Comments

