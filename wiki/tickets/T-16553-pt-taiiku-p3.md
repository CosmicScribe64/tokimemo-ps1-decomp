---
id: T-16553
title: "Types-first pilot package pt-taiiku-p3"
status: In Progress
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of `src/ovl/TAIIKU/801452D0.c` (game C) from the T-16550 types-first redrafts, using and extending the types in `include/ovl/TAIIKU.h`. Work list (best first): `func_8014666C`, `func_80145640`, `func_80145C28`, `func_8014607C`, `func_80147318`, `func_80147928`, `func_80146A60`, `func_80147F58`, `func_80145370`.

## Acceptance criteria

- [ ] Each matched function is plain C in `src/ovl/TAIIKU/801452D0.c`, TAIIKU sha1 OK, committed as `T-16553: match TAIIKU:func`.
- [ ] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a `NON_MATCHING: T-16553` comment; the rest are restored to `INCLUDE_ASM`.
- [ ] Compiler-gap rows go to `wiki/data/t0018-cases.md`.
- [ ] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [ ] Short T-16553 section in `wiki/matching-notes.md`; code review before Done.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p3`, branch `pt-taiiku-p3` from `w2-good`. Owned C file: `src/ovl/TAIIKU/801452D0.c` (plus `include/`, `config/`, `wiki/` where needed). Measured pilot (stage B): start 2026-10-16 02:25 UTC.

## Comments
