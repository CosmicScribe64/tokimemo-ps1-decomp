---
id: T-16554
title: "Types-first pilot package pt-taiiku-p4"
status: In Review
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of package pt-taiiku-p4 starting from the T-16550 types-first redrafts (`drafts/T-16550/TAIIKU/<func>/`, best draft per function): `func_80140A90`, `func_80132970`, `func_8014357C`, `func_8014280C`, `func_80143F50`, `func_80142FB8`, `func_80143DD0`. Owned C files: `src/ovl/TAIIKU/80140810.c`, `80132000.c`, `80142240.c`.

## Acceptance criteria

- [x] Each function matched (unit sha1 OK, no guard) or kept under `#ifdef NON_MATCHING` (< about 30 differing lines) with a ticket comment, else plain `INCLUDE_ASM`.
- [x] Types extended in `include/ovl/TAIIKU.h` instead of byte-offset casts where the build allows.
- [ ] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [x] Short T-16554 section in [[matching-notes]]; compiler-gap rows in `wiki/data/t0018-cases.md`.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p4`, branch `pt-taiiku-p4` from `w2-good`. Time box: stop starting new functions after about 2.5 hours. Pilot stage B (measured).

## Results

Four of seven matched (3,208 bytes), unit sha1 OK after each: `func_80132970`, `func_80143DD0`, `func_80143F50`, `func_8014280C`. Guarded drafts: `func_80140A90` (33 differing instructions, ugen temporary numbering after a block copy; the two callee prototype overrides and one FAKE selector view are in the tree), `func_8014357C` (20 instructions, flags and rating register allocation). `func_80142FB8` stays `INCLUDE_ASM` (loop variables in caller-saved registers with spills in the original, `$s1..$s3` here). FAKE comments: `*(u8 *)&D_8014A110` views (`func_80143F50`, `func_8014280C`), `u8 pad[4]` (`func_8014280C`), `*(s32 *)&D_80122CDC` selector (`func_80140A90`, guarded). Notes: [[matching-notes]] "T-16554"; rows in [[data/t0018-cases]]; cheat sheet rows added.

## Comments
