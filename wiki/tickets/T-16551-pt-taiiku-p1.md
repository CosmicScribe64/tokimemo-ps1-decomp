---
id: T-16551
title: "Types-first pilot package pt-taiiku-p1"
status: Done
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of `src/ovl/TAIIKU/80133C80.c` (game A) from the types-first redrafts of [[tickets/T-16550-taiiku-types-first]], in the order of the package list: `func_801346FC`, `func_80136BB4`, `func_801368B8`, `func_80138224`, `func_80134060`, `func_80133CFC`, `func_80135480`, `func_80137544`. Pilot stage B: measured agent time.

## Acceptance criteria

- [x] Seven of the eight functions worked (in list order for the first four, then `func_80133CFC`, `func_80137544`, `func_80135480`; `func_80134060` not started); matches committed one by one as plain C.
- [x] Unmatched drafts kept under `#ifdef NON_MATCHING` only when under about 30 differing lines, with a `NON_MATCHING: T-16551` comment (`func_80135480` 22, `func_80137544` 26; `func_801346FC` and `func_80136BB4` matched in the closer pass); `func_80134060` stays `INCLUDE_ASM`.
- [x] Compiler-gap rows in `wiki/data/t0018-cases.md`.
- [x] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [x] Section in `wiki/matching-notes.md`; ticket In Review.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p1`, branch `pt-taiiku-p1` from `w2-good`. Ownership: `src/ovl/TAIIKU/80133C80.c`; `include/`, `config/`, `wiki/` where needed.

## Results

Final: 5 functions, 4,480 bytes matched in plain C (TAIIKU sha1 OK after each): `func_80138224` (1,116 B), `func_80133CFC` (868 B), `func_801368B8` (312 B) in the first pass (about 2 h 15 min of agent time), `func_801346FC` (1,688 B) and `func_80136BB4` (496 B) in the closer pass. Guarded: `func_80135480` (22 lines, new) and `func_80137544` (26, new). Not started: `func_80134060`. Details, idioms and blockers in [[matching-notes]] (T-16551) and [[taiiku-family-results]]. Closer pass: `func_801346FC` (1,688 B) and `func_80136BB4` (496 B) matched, 5 functions and 4,480 bytes in all (Comments).

Header changes (`include/ovl/TAIIKU.h`, one `main_api.h` line): `func_801365C8` is `void` (closer pass), prototypes for the callees defined later in the file (`func_80138680`, `func_801388E4`, `func_801356E0`, `func_80135778`, `func_80137544`), `D_80149364`, `D_80149398`, `D_801492A0`, `D_801492A4`, `D_801492A8`, `D_801492C4` is `s32` now (a word, the byte view made the copy an `sb`); `D_8012091E` in `main_api.h`. `D_801492F8[10]` and `D_801492A8[4]` are defined in `80133C80.c` (data island pieces replaced) for the shared `lui at`.

FAKE marks (all with T-16551): the unused word local `j` in `func_80138224` and `func_80133CFC` (frame slots above the tables), `(&D_80149208.player)[i]` in `func_801368B8` and `func_80135480`, `one` and the `(s8)` cast in the guarded `func_80137544`.

## Comments

- 2026-10-16: In Review. Review not yet run; no `src/` change outside `src/ovl/TAIIKU/80133C80.c`, `include/` and `wiki/`. Stage B measurements: start 02:25 UTC, last function work 04:41 UTC.
- 2026-10-16: Closer pass (Opus, 04:40-05:20 UTC, the two short guarded drafts only). Matched `func_801346FC` (1,688 B) and `func_80136BB4` (496 B) in plain C, TAIIKU sha1 OK after each, one commit each; the package is now 5 functions, 4,480 bytes. Still guarded: `func_80135480` (22), `func_80137544` (26), not worked in this pass; `func_80134060` stays `INCLUDE_ASM`.
  - `func_801346FC`: `func_801365C8` sets no return value (no `$v0` before its `jr`) and no caller reads one, so `include/ovl/TAIIKU.h` declares it `void func_801365C8(void);` (was `s32 func_801365C8();`). The value-returning prototype kept `$v0` busy in the block of the call in case 0, so the `D_80120676` decrement range took `$v1`. regdiff's FIRST DIVERGENCE hint named the switch temporary (bit 5) against the selector (bit 7); `regdiff -v` showed the real range, bit 45 / lr 169 (`S148+6566` = `D_80120676`) in `$v1`. `funcdiff --proto 'void func_801365C8();'` confirmed it before the header change. The matched callers of the file still match.
  - `func_80136BB4`: the f0C counter is read once into `u16 count` for the three threshold tests. The `cc -S` listing (ugen order, before as1) showed the cause: uopt emits the store-back of the promoted f03 at the end of the join block, after every plain store there, so the plain `sh` of the clear always came first, whatever the source order (local for f03, swapped statements, ternaries, labels, `do`/`while (0)`, record and pointer views of the clear: all 4 lines or worse). With the copy, the load needs no widening, uopt promotes f0C itself over the tests, the f03 update and the clear (one range in `$v1`), and the clear becomes a store-back too, emitted after f03's. A FAKE `if (D_80149208.player.unk_0C) {}` before the clear already fixed the order (8 lines, compares on a widened copy in `$a0`), which pointed at the promotion.
- 2026-10-16: Code review against `CODING_STANDARDS.md` (Opus), done; status Done. Checked: only `src/ovl/TAIIKU/80133C80.c` changed in `src/`; no `drafts/` change; no inline asm or `.word`; every reported match is plain C; the two remaining `NON_MATCHING` blocks (`func_80135480` 22 lines, `func_80137544` 26) name T-16551 and the lines left; the six fakematches (`j` in `func_80138224` and `func_80133CFC`, `(&D_80149208.player)[i]` in `func_801368B8` and `func_80135480`, `one` and the `(s8)` cast in `func_80137544`) carry FAKE and T-16551; header changes are consistent (`D_801492C4` is `s32` and only used in this file, `func_801365C8` `void` and the other overlay's definition is a different address space); no game data, asm or binaries added (the 10 address constants of `D_801492F8` follow the data-island rule of the file header). Clean rebuild 27/27 SHA1 OK, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` (0 new, 15 known) OK. Findings, all fixed in commit 8de5449f (comments and wiki only, build unchanged, TAIIKU sha1 OK): (1) the peeled two-word copy in `func_80133CFC` and the two unit-defined data pieces (`D_801492F8`, `D_801492A8`) had no comment in the C (section 9), added; (2) stale counts (3 of 8, 2,296 B) in `wiki/index.md`, `wiki/taiiku-family-results.md`, `wiki/matching-notes.md` and the Results of this ticket, updated to 5 of 8, 4,480 B.
