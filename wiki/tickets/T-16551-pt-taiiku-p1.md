---
id: T-16551
title: "Types-first pilot package pt-taiiku-p1"
status: In Review
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of `src/ovl/TAIIKU/80133C80.c` (game A) from the types-first redrafts of [[tickets/T-16550-taiiku-types-first]], in the order of the package list: `func_801346FC`, `func_80136BB4`, `func_801368B8`, `func_80138224`, `func_80134060`, `func_80133CFC`, `func_80135480`, `func_80137544`. Pilot stage B: measured agent time.

## Acceptance criteria

- [x] Seven of the eight functions worked (in list order for the first four, then `func_80133CFC`, `func_80137544`, `func_80135480`; `func_80134060` not started); matches committed one by one as plain C.
- [x] Unmatched drafts kept under `#ifdef NON_MATCHING` only when under about 30 differing lines, with a `NON_MATCHING: T-16551` comment (`func_801346FC` 8, `func_80136BB4` 4, `func_80135480` 22, `func_80137544` 26); `func_80134060` stays `INCLUDE_ASM`.
- [x] Compiler-gap rows in `wiki/data/t0018-cases.md`.
- [x] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [x] Section in `wiki/matching-notes.md`; ticket In Review.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p1`, branch `pt-taiiku-p1` from `w2-good`. Ownership: `src/ovl/TAIIKU/80133C80.c`; `include/`, `config/`, `wiki/` where needed.

## Results

Matched (TAIIKU sha1 OK after each): `func_80138224` (1,116 B), `func_80133CFC` (868 B), `func_801368B8` (312 B); 3 functions, 2,296 bytes, in about 2 h 15 min of agent time. Guarded: `func_801346FC` (8 lines), `func_80136BB4` (4), `func_80135480` (22, new), `func_80137544` (26, new). Not started: `func_80134060`. Details, idioms and blockers in [[matching-notes]] (T-16551) and [[taiiku-family-results]].

Header changes (`include/ovl/TAIIKU.h`, one `main_api.h` line): prototypes for the callees defined later in the file (`func_80138680`, `func_801388E4`, `func_801356E0`, `func_80135778`, `func_80137544`), `D_80149364`, `D_80149398`, `D_801492A0`, `D_801492A4`, `D_801492A8`, `D_801492C4` is `s32` now (a word, the byte view made the copy an `sb`); `D_8012091E` in `main_api.h`. `D_801492F8[10]` and `D_801492A8[4]` are defined in `80133C80.c` (data island pieces replaced) for the shared `lui at`.

FAKE marks (all with T-16551): the unused word local `j` in `func_80138224` and `func_80133CFC` (frame slots above the tables), `(&D_80149208.player)[i]` in `func_801368B8` and `func_80135480`, `one` and the `(s8)` cast in the guarded `func_80137544`.

## Comments

- 2026-10-16: In Review. Review not yet run; no `src/` change outside `src/ovl/TAIIKU/80133C80.c`, `include/` and `wiki/`. Stage B measurements: start 02:25 UTC, last function work 04:41 UTC.

