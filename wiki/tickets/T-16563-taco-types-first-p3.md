---
id: T-16563
title: "Types-first pilot package pt-taco-p3"
status: In Progress
assignee: claude (pt-taco-p3)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-16560-taco-types-first]]", "[[taco-family-results]]"]
---

## Goal

Matching stage of the TACO types-first pilot, package 3: match the functions of `src/ovl/TACO/80151C30.c` and
`src/ovl/TACO/8015ABF0.c` from the redrafts of [[tickets/T-16560-taco-types-first]] (best first, work list in the
orchestrator scratchpad). Measured: minutes of agent time and bytes matched.

## Acceptance criteria

- [ ] Functions worked in list order; matches committed one by one
- [ ] Unmatched drafts under 30 differing lines guarded with a ticketed `NON_MATCHING` comment, the rest restored to `INCLUDE_ASM`
- [ ] Section in `wiki/matching-notes.md`; T-0018 rows for compiler-gap families
- [ ] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch OK

## Notes

Worktree `pt-taco-p3` (branch `pt-taco-p3`, from `w2-good`). Owns `src/ovl/TACO/80151C30.c` and `src/ovl/TACO/8015ABF0.c`;
`include/`, `config/` and `wiki/` edits where needed.

## Comments
