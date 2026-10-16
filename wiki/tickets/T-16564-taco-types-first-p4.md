---
id: T-16564
title: "Types-first pilot package pt-taco-p4"
status: Done
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16560-taco-types-first]], [[taco-family-results]]
---

## Goal

Match the TACO functions of package pt-taco-p4 starting from the T-16560 types-first redrafts (`drafts/T-16560/TACO/<func>/`, best draft per function): `func_801563E4`, `func_80154960`, `func_80138D08`, `func_801557E8`, `func_80138B2C`, `func_80138964`, `func_80137D00`, `func_801553AC`. Owned C files: `src/ovl/TACO/80154960.c`, `80137A60.c`, `80138890.c`.

## Acceptance criteria

- [x] Each function matched (unit sha1 OK, no guard) or kept under `#ifdef NON_MATCHING` (< about 30 differing lines) with a ticket comment, else plain `INCLUDE_ASM`.
- [x] Types extended in `include/ovl/TACO.h` instead of byte-offset casts where the build allows.
- [x] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [x] Short T-16564 section in [[matching-notes]]; compiler-gap rows in `wiki/data/t0018-cases.md`.

## Notes

Worktree `tokimemo-wt/pt-taco-p4`, branch `pt-taco-p4` from `w2-good`. Time box: stop starting new functions after about 2.5 hours. Pilot stage B (measured); start 2026-10-16 04:58 UTC.

## Results

Six of eight matched (5,852 bytes), unit sha1 OK after each, in about an hour: `func_801563E4` (1,344 B), `func_80154960` (1,556), `func_80138D08` (1,040), `func_801557E8` (372), `func_80138964` (456), `func_801553AC` (1,084). Guarded drafts: `func_80138B2C` (30 differing instruction rows, registers only: count, second local and sum in `$a0/$a1/$a0` in the original, `$v0/$a0/$a1` here) and `func_80137D00` (8 rows, the T-9180 pointer row: `lw v1,0(t9)` against `lw t9,0(t9)`). FAKE comments: `u8 pad[4]` (`func_801563E4`, `func_80154960`), spare `TcPos sp48` and the one-line stores (`func_801553AC`), `*(u8 *)&D_800E6280.unk_110D` for the third switch (`func_80138D08`). Types: `POLY_F4` for the fade packet, `D_8015F490[5]` score words, `u16 *D_801FAA84`, prototypes for the callees defined later in the file (`func_80156178` is now `s32`, its result is read). Notes: [[matching-notes]] "T-16564"; rows in [[data/t0018-cases]]; five cheat sheet rows.

## Comments

- 2026-10-16: agent time about 67 minutes (04:58-06:05 UTC). Clean rebuild 27/27 (`rm -rf build && configure.py && ninja`), `check_headers.py` OK, `migrate_globals.py --check` OK, `sync_protos.py --check-branch` OK (0 new, 15 known). Code review against `CODING_STANDARDS.md` still to do before Done.
- 2026-10-16: closer pass (Opus, 06:05 UTC start). `func_80137D00` stays guarded at 8 rows; the NON_MATCHING comment now says why. uopt colours `p` `$v1` but copy-propagates the `D_801FAA84` load into the first store, and ugen keeps that shared load in a temporary for every use of `p`. ugen writes `$v1` only when `p` is read after a branch target or has three or more in-place `p += k` updates, and the updates add an `addiu v1` before the switch join. Details and the forms tried: [[matching-notes]] "T-16564", closer pass. `func_80138B2C` was not in the closer's scope. Still In Review.
- 2026-10-16: code review against `CODING_STANDARDS.md` (Opus). Checked: only the three owned C files and `include/ovl/TACO.h` changed (no other C file, no `drafts/` change, no game data, asm or binaries added); no inline asm, `.word`, `//` comment or non-ASCII text in the diff; C89 declarations at block top; all 6 reported matches are plain C and the unit sha1 is OK; both remaining `NON_MATCHING` drafts (`func_80138B2C`, `func_80137D00`) are guarded, name T-16564 and give the rows left (30, 8), and the default build uses `INCLUDE_ASM`; FAKEs (`u8 pad[4]` x2, spare `TcPos sp48` and one-line stores, `*(u8 *)&D_800E6280.unk_110D`) carry a `FAKE` comment and this ticket; `TACO.h` additions are single declarations (`check_headers.py` OK). One finding, fixed: the comment on `func_801557E8` explained the mixed operand order of the actor address but did not start with `FAKE`; it now does (comment only, no byte change, TACO sha1 OK after rebuild). No other findings. Status Done.
