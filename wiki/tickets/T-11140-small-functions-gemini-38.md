---
id: T-11140
title: Small functions (Gemini) 38
status: Done
assignee: gemini
created: 2026-10-14
updated: 2026-10-14
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[data/t0018-cases]]", "src/ovl/DATE/80132000.c", "include/ovl/DATE.h"]
---

## Goal

Match or guard with `NON_MATCHING` the 16 functions in `src/ovl/DATE/80132000.c` from `g38-list.txt` (5,012 bytes total).

## Acceptance criteria

- [x] Every function in `g38-list.txt` has been drafted, built, diffed, and iterated on.
- [x] Near misses under ~30 lines guarded with `#ifdef NON_MATCHING`.
- [x] `tools/docker.sh ninja` passes 27/27 OK with `build/headers.ok` and `build/globals.ok`.
- [x] Code review against `CODING_STANDARDS.md` completed and recorded.

## Notes

Owned file: `src/ovl/DATE/80132000.c`.

### Matched (11 functions, 3,444 linked bytes / 3,652 list bytes)
- `func_8013C0A4` (236 linked / 280 list bytes): chain assignments on `D_800CA21C`/`D_800CA224`/`D_800CA22C`/`D_800CA234` and `s32` return with implicit fallthrough.
- `func_80138220` (244 linked / 288 list bytes): `s32` return with implicit fallthrough and `return 0;` after `func_80083440(3)`/`func_80042940(0x3D)`.
- `func_801384D4` (260 linked / 308 list bytes): `s32` return with `return 0;` in early branches and `D_800CA14C == 2` branch.
- `func_80136214` (300 linked / 332 list bytes): `if / else if` chain on `D_80122CDC` and `D_800CA2FC`.
- `func_80136FF0` (356 linked / 356 list bytes): `s32` return with implicit fallthrough and `return 0;` on early exits.
- `func_80139F3C` (388 linked / 428 list bytes): `s32` return with `(*(u8 *)&D_8015B698)++` and `return 0;` on early exits.
- `func_80139944` (288 linked / 288 list bytes): `GameState *g = &D_800E6280;` with `*(g->unk_F5F * 0x24 + (u8 *)D_8015B5B0 + choice_offset + N)` prevents `uopt` from CSEing `unk_F5F * 0x24` across pointer stores.
- `func_8013A0C0` (372 linked / 372 list bytes): `s32` return with `return 0;` after `func_8013E7B0()`/`func_8013E980()`/`func_8013E9BC()`.
- `func_8013D468` (404 linked / 404 list bytes): `GameState *g = &D_800E6280;` for `g->unk_03F < 3` and `u16 one; one = 1;` for branch operand order.
- `func_8013DB6C` (240 linked / 240 list bytes): `u16 two; two = 2;` with `two == D_80122CDC` and `s32 prev;` matches the 48-byte frame and all 3 `bne a0, v1` branches.
- `func_80140B08` (356 linked / 356 list bytes): `GameState *g = &D_800E6280;` on the second `||` arm (`g->unk_03E == 0x60`) prevents `uopt` from CSEing `unk_03E` into `$a0`, and `((u8)x & 0x7F) != 4` consumes `$t8` so `andi/beq` use `$t9`.

### Guarded under `#ifdef NON_MATCHING` (5 functions, all < 30 diff lines)
- `func_8013D8B0` (1 diff line: `bne v1, a0` vs `bne a0, v1` at row 52).
- `func_8013E9F8` (3 diff lines: `beq v0, v1` vs `beq v1, v0` at rows 26, 43, 61).
- `func_8013BEB8` (8 diff lines: chain store scheduling and `$t4` vs `$v0` on second `= 3` chain).
- `func_80137D1C` (9 diff lines: retained from T-10510).
- `func_80139834` (24 diff lines: `s32 pad; s16 prev;` matches `0x2A(sp)` and `beq v0, t8`; `uopt` CSEs `unk_F5F` and `unk_110A` into `$t4`/`$t6` in later branches).

## Comments

### Inline Code Review (`CODING_STANDARDS.md`)
- Checked `src/ovl/DATE/80132000.c` and `include/ovl/DATE.h` against `CODING_STANDARDS.md`:
  - Fixed-width types (`s8`/`u8`/`s16`/`u16`/`s32`/`u32`), K&R 4-space indentation, no `volatile`, no inline assembly, no raw hex addresses.
  - All `/* FAKE: ... T-11140 */` and `/* NON_MATCHING: T-11140, ... */` comments present.
  - Header additions in `include/ovl/DATE.h` are append-only at EOF under `#ifndef OVL_DATE_T11140_H`.
  - `tools/docker.sh ninja`, `build/headers.ok`, and `build/globals.ok` pass (27/27 OK).

