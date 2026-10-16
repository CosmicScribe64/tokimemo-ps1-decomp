---
id: T-16562
title: "Types-first pilot package pt-taco-p2"
status: In Progress
assignee: claude (pt-taco-p2)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-16560-taco-types-first]]", "[[taco-family-results]]"]
---

## Goal

Matching stage of the TACO types-first pilot ([[tickets/T-16560-taco-types-first]]), package 2 (pilot stage B, measured).
Put the redrafted bodies (written against the new `include/ovl/TACO.h` types) into the real files and work the diff,
best expected bytes first: `func_801546FC`, `func_801430D0`, `func_80154230`, `func_80133870`, `func_80143B58`,
`func_801342CC`, `func_801432F0`, `func_801347F4`, `func_80154820`, `func_8015443C`.

## Acceptance criteria

- [ ] Each worked function either matches (plain C, unit sha1 OK) or is guarded under `NON_MATCHING` with the differing line count (under about 30 lines) or restored to `INCLUDE_ASM`
- [ ] Section in `wiki/matching-notes.md`; compiler-gap rows in `wiki/data/t0018-cases.md`
- [ ] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `pt-taco-p2` (branch `pt-taco-p2`, from `w2-good`). Owns `src/ovl/TACO/80154230.c`, `src/ovl/TACO/801430B0.c`,
`src/ovl/TACO/80133870.c`; edits to `include/`, `config/` and `wiki/` where needed. Work list:
the scratchpad `pt-taco-p2-list.txt` (best draft per function).

## Comments
