---
id: T-16553
title: "Types-first pilot package pt-taiiku-p3"
status: In Review
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]], [[matching-notes]], [[data/t0018-cases]]
---

## Goal

Match the TAIIKU functions of `src/ovl/TAIIKU/801452D0.c` (game C) from the T-16550 types-first redrafts, using and extending the types in `include/ovl/TAIIKU.h`. Work list (best first): `func_8014666C`, `func_80145640`, `func_80145C28`, `func_8014607C`, `func_80147318`, `func_80147928`, `func_80146A60`, `func_80147F58`, `func_80145370`. Measured pilot (stage B): start 2026-10-16 02:25 UTC.

## Acceptance criteria

- [x] Each matched function is plain C in `src/ovl/TAIIKU/801452D0.c`, TAIIKU sha1 OK, committed as `T-16553: match TAIIKU:func`. (9 functions)
- [x] Unmatched drafts under about 30 differing lines stay under `#ifdef NON_MATCHING` with a `NON_MATCHING: T-16553` comment (`func_80145640` 8, `func_8014607C` 20, `func_80145370` 22, `func_80146D18` 31); `func_80147B14` stays `INCLUDE_ASM`.
- [x] Compiler-gap rows in [[data/t0018-cases]] (four new rows and an update line for `func_80146A60`).
- [x] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [x] Short T-16553 section in [[matching-notes]]; family page [[taiiku-family-results]] updated.
- [ ] Code review against `CODING_STANDARDS.md` (required before Done).

## Notes

Worktree `tokimemo-wt/pt-taiiku-p3`, branch `pt-taiiku-p3` from `w2-good`. Owned C file: `src/ovl/TAIIKU/801452D0.c` (plus `include/ovl/TAIIKU.h`). `tools/siblings.py` found no matched sibling above 0.70 for any function of the package, so the starting point was the redraft plus the asm.

## Results

Matched (9, 5,244 bytes): `func_8014666C` (480), `func_80145C28` (716), `func_80147318` (520), `func_80147928` (324), `func_80146A60` (300), `func_80147F58` (572) from the list; `func_80148370` (596), `func_80146338` (820), `func_80147594` (916) from the rest of the file.

Guarded: `func_80145640` (1,080 B, 8 lines: the counter's spill slot is one word too high), `func_8014607C` (700 B, 20 lines: a load that IDO splits into a global range and a copy), `func_80145370` (720 B, 22 lines: constant and pointer swapped in two loops), `func_80146D18` (648 B, 31 lines: a loop store scheduled before the pointer advance, the tail index in the wrong register). `func_80147B14` (1,020 B) was not started.

Header changes (`include/ovl/TAIIKU.h`): tables `D_8014A4F8[12]`, `D_8014A528[9]`, `D_8014A4D4[8]`, `D_8014A4E8[6]`, `D_8014A56C`, `D_8014A570`, `D_8014A5A0`, `D_8014A668` typed, `D_8014A4F4` is now `s32` (word access), `TaiWords9` and `TaiWords12`, prototypes of the game-C helpers called before their definition. `src` declares `D_8014A4A8` (a splat name inside `D_8014A454[4]`, used as the loop end of `func_80147594`) and `D_8014A614`.

What carried over from the pilot: the typed records made the m2c byte offsets readable (record number plus field), which is what let the loops be rewritten as index loops; the two record-init functions and `func_80147594` then needed only `order_search`, an alias symbol and the choice of index variables. Details in [[matching-notes]], "T-16553".

## Comments
