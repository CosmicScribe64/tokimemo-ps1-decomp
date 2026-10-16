---
id: T-16561
title: "Types-first pilot package pt-taco-p1"
status: In Progress
assignee: claude (pt-taco-p1)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-16560-taco-types-first]]", "[[taco-family-results]]"]
---

## Goal

Matching stage (pilot stage B) of the TACO types-first pilot: take the best redraft of each function in
`src/ovl/TACO/801335A0.c`, `80137590.c`, `80159090.c` and `80146D60.c` (9 functions, list in the orchestrator
scratchpad `pt-taco-p1-list.txt`) and match as many as possible in about 2.5 hours. Measured pilot.

## Acceptance criteria

- [ ] Every matched function committed on its own ("T-16561: match UNIT:func"), unit sha1 OK
- [ ] Unmatched drafts under 30 differing lines kept under `#ifdef NON_MATCHING` with a T-16561 comment
- [ ] Compiler-gap rows added to `wiki/data/t0018-cases.md`
- [ ] T-16561 section in `wiki/matching-notes.md`
- [ ] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch OK

## Notes

Worktree `pt-taco-p1` (branch `pt-taco-p1`, from `w2-good`). Owns the four C files above; `include/`, `config/`
and `wiki/` edits where needed. Started 2026-10-16 02:40 UTC.

## Comments
