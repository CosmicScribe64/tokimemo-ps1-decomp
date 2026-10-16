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

Pilot stage B (measured): started 02:40 UTC, 2026-10-16. Matched in plain C (unit sha1 OK): `func_801430D0` (544 B), `func_801347F4` (144 B, TAIIKU/main clone idiom), `func_80143B58` (808 B, explicit `case 0`), `func_801342CC` (388 B), `func_80133870` (272 B, `s32` without return), `func_801432F0` (644 B, `*s` read directly), `func_80154820` (320 B, `*p` read directly, two-step r/g/b): 3,120 bytes in 7 functions. Guarded `NON_MATCHING`: `func_80154230` (8 lines), `func_801546FC` (8), `func_8015443C` (9; `s32 pad[2]` FAKE above `interval`), `func_801345D4` (12). Left `INCLUDE_ASM`: `func_80133E98` (126 lines at best). Details: [[matching-notes]], "T-16562"; rows in [[data/t0018-cases]].

## Comments

### Closer pass (Opus, 2026-10-16 04:29-05:00 UTC)

All four guarded near misses now match in plain C (unit sha1 OK), each in its own commit: `func_80154230` (236 B), `func_801546FC` (292 B), `func_8015443C` (704 B), `func_801345D4` (376 B), 1,608 bytes. Package total: 11 functions, 4,728 bytes. No `NON_MATCHING` guard and no FAKE left in the three files; `func_80133E98` stays `INCLUDE_ASM` (out of the closer's scope).
- Entry copy: the fold is as1's copy propagation (ugen's binasm has the original `move` and the loads through `$s0`). Walking the header in two steps, `arg0++; count = arg0[0]; limit = arg0[2]; ... arg0 += 8;`, gives the original `move s0,a0` and offsets. `func_8015443C` also walks `arg0` itself and gets its 88-byte frame from the named `command0`/`command1` constants of its siblings (they replace the FAKE `s32 pad[2]`).
- `func_801345D4`: reading the flag test through the same base as the else path's byte read-modify-write (`TcSave` view of `D_800E7D10`, word at +0x40) keeps `&D_800E7D10` in `$v0`; a follow-up commit replaced the byte-offset casts with that one view (same bytes).
- Lead for later: TACO `func_801471D0` (other file) has the same as1 shape before its second loop.
Details: [[matching-notes]], "T-16562", "Closer pass"; [[register-cheatsheet]] (two rows); [[data/t0018-cases]] (update paragraph).
