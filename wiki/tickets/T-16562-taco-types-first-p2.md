---
id: T-16562
title: "Types-first pilot package pt-taco-p2"
status: In Review
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

- [x] Each worked function either matches (plain C, unit sha1 OK) or is guarded under `NON_MATCHING` with the differing line count (under about 30 lines) or restored to `INCLUDE_ASM`
- [x] Section in `wiki/matching-notes.md`; compiler-gap rows in `wiki/data/t0018-cases.md`
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `pt-taco-p2` (branch `pt-taco-p2`, from `w2-good`). Owns `src/ovl/TACO/80154230.c`, `src/ovl/TACO/801430B0.c`,
`src/ovl/TACO/80133870.c`; edits to `include/`, `config/` and `wiki/` where needed. Work list:
the scratchpad `pt-taco-p2-list.txt` (best draft per function).

## Results

Pilot stage B (measured): started 02:40 UTC, 2026-10-16. Matched in plain C (unit sha1 OK): `func_801430D0` (544 B), `func_801347F4` (144 B, TAIIKU/main clone idiom), `func_80143B58` (808 B, explicit `case 0`), `func_801342CC` (388 B), `func_80133870` (272 B, `s32` without return), 2,156 bytes in 5 functions. Guarded `NON_MATCHING`: `func_80154230` (8 lines), `func_801546FC` (8), `func_8015443C` (9; `s32 pad[2]` FAKE above `interval`), `func_801432F0` (8), `func_801345D4` (12). Left `INCLUDE_ASM`: `func_80154820` (64 lines), `func_80133E98` (126 lines at best). Details: [[matching-notes]], "T-16562"; rows in [[data/t0018-cases]].

## Comments
