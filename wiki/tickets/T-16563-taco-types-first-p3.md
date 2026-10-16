---
id: T-16563
title: "Types-first pilot package pt-taco-p3"
status: Done
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

- [x] Functions worked in list order; matches committed one by one (9 of 10, plus the unlisted `func_8015BD30`)
- [x] Unmatched drafts under 30 differing lines guarded with a ticketed `NON_MATCHING` comment, the rest restored to `INCLUDE_ASM` (`func_8015B1B8` guarded, 8 lines; `func_8015ACCC` stays `INCLUDE_ASM`; the unlisted `func_8015BD30` was matched later)
- [x] Section in `wiki/matching-notes.md`; T-0018 rows for compiler-gap families
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch OK

## Notes

Worktree `pt-taco-p3` (branch `pt-taco-p3`, from `w2-good`). Owns `src/ovl/TACO/80151C30.c` and `src/ovl/TACO/8015ABF0.c`;
`include/`, `config/` and `wiki/` edits where needed.

## Comments

### 2026-10-16 (claude, pt-taco-p3): matching stage finished, In Review

Branch `pt-taco-p3` (from `w2-good`), about 60 minutes of agent time for the listed functions (03:55 to 04:55 UTC), about 80 minutes with the extras (to 05:15 UTC). 9 of the 10 listed functions matched in plain C, 4,164 bytes, one commit each, then the unlisted `func_8015BD30` (576 bytes, 4,740 in all): `func_8015C0A0` (360), `func_80151D1C` (632), `func_80152944` (796), `func_8015BF70` (304), `func_80152808` (316), `func_80152DB4` (692), `func_801523D0` (668), `func_801521EC` (196), `func_8015CE64` (200). `func_80151D1C` matched at the first build of a hand-written body, `func_80152944` and `func_801521EC` at the second; none matched from the printed m2c draft.

Fakematches (marked `FAKE` with this ticket): `func_801523D0` (`s32 pad[15]` and `pad2` reproduce the frame and the slot of `k`), `func_80152808` (pointer table read from the base `D_801603D4`, declared `s16 *D_801603D4[]` in `TACO.h`), `func_8015BD30` (`if (i) {}` splits the live range of `i`).

Left: `func_8015B1B8` guarded (8 differing lines, struct-copy temporary `t2` against `t3`, T-0018 row); `func_8015BD30` (not on the list, 576 bytes) matched after about 35 minutes (first pass left 65 rows; the 1-second scratch compile found the rest), with a FAKE empty test on `i`; `func_8015ACCC` (1260 bytes, no draft) rewritten by hand for about 20 minutes, 437 lines left, stays `INCLUDE_ASM` (T-0018 row).

Header changes (`include/ovl/TACO.h`): `D_80160290` (`TcPair8[]`) and `D_801603A8` (`TcPair2[]`), `D_80160444` from `s16 *` to `s16 *[]` (four scalar uses became `[0]`), `D_801603D4` (FAKE view), prototypes of `func_80152944`, `func_80152DB4`, `func_801523D0`. Checks: clean `rm -rf build && configure.py && ninja` 27/27 SHA1 OK, `check_headers.py` OK, `migrate_globals.py --check` OK, `sync_protos.py --check-branch` OK (0 new, 15 known). Notes: [[matching-notes]] "T-16563: TACO package p3", [[taco-family-results]], five new rows in [[register-cheatsheet]]. Waiting for the code review (CODING_STANDARDS.md checklist) before Done.

### 2026-10-16 (claude, pt-taco-p3): closer pass

Opus closer, about 40 minutes (05:15 to 05:55 UTC) including the clean build and checks, on the one guarded near miss. Clean build 27/27 SHA1 OK, `check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` OK. `func_8015B1B8` stays guarded at 8 lines; no new match. The NON_MATCHING comment now says what was learned.

- ugen's temporaries come from a first-in, first-out free list. Before a struct copy takes its data register, ugen skips one free temporary when the last store it emitted went through a pointer, and two when that store was a direct stack or global store, or when there was no store yet. Calls, loads and outgoing-argument stores do not change this. Probe: `.set noat` appears twice before the copy in the one-skip case and once in the two-skip case (`cc -S`).
- Here the last stores before the copy are the loop's stores to `matrix` through pointers, so IDO skips `t9` and takes `t2`. The original has `t3`: it skips two, or `t2` belongs to uopt. Both give `t3` in a scratch compile, but neither without other changes. `matrix.m[0][0] = 0;` before the copy adds a store (30 lines), and a `v` temporary in the inner loop makes uopt use `t2` but shifts the loop registers (12 lines).
- Tried without effect (still `t2`): about 60 scratch variants (index forms of both stores, `matrix[1]`, a union, `register`, counter widths, extra or dead counters, pointer locals for the tables, dead stores to `arg3`/`i`/`j`, implicit `int` return), `tools/autotweak.py` 120 s, `tools/permute.py` 420 s (nothing better than the base score of 40).
- TACO `func_801472D4` (also a guarded draft) has the same miss: a MATRIX built in a nested loop and then copied, original `t8` against IDO `t7`. The T-11610 note on main `func_8005938C` shows the opposite case (the original skips one after stack stores). The rule may differ in the original compiler. T-0018 row added.

### 2026-10-16 (claude, pt-taco-p3): code review, Done

Reviewed `git diff w2-good...pt-taco-p3` against the `CODING_STANDARDS.md` checklist. Scope: only `src/ovl/TACO/80151C30.c`, `src/ovl/TACO/8015ABF0.c` and `include/ovl/TACO.h` changed outside `wiki/`; no `drafts/` archive, game data, asm or binaries touched; no inline asm, `.word`, `//` comment or C99 construct; headers keep one type per symbol (`D_80160444` is `s16 *[]` with the four scalar uses now `[0]`; the DATE overlay has its own, unrelated `D_80160444`). The one remaining `#ifdef NON_MATCHING` draft (`func_8015B1B8`) is guarded, names T-16563 and says 8 differing lines; `func_8015ACCC` stays `INCLUDE_ASM`.

Findings, all fixed in commit `de1a6c3f`:
- `func_801521EC` read the `TcPair8`/`TcPair2` tables through `(s32 *)`/`(u8 *)` casts with an explanatory but unmarked comment: a cast view of declared aggregates is a fakematch (section 7/8). The comment now starts with `FAKE` and ends with the ticket.
- `func_8015BD30` addresses the actor through `(TcActor *)((u8 *)D_8015EDB4 + arg0 * 0x88)` (the neighbours `func_8015B59C` etc. mark the same idiom `FAKE`): comment now `FAKE`. Its empty `if (i) {}` was already marked.
- `func_8015BD30` had a dead `i = 0;` just before a loop that sets `i = 0` itself: removed (a useless statement, section 7); TACO sha1 still OK.
- The ticket's acceptance criteria still said `func_8015BD30` stays `INCLUDE_ASM`: corrected (it was matched later).
- Double blank lines left by the `INCLUDE_ASM` replacements (4 places) and a trailing blank line at the end of `8015ABF0.c`: collapsed.

Left as is, no finding: `(arg + D_8015EDB4)->field` index-first spellings (a plain C spelling, explained in [[matching-notes]] and [[register-cheatsheet]], same output as the array form for the compiler's index order); `func_80152DB4`/`func_801523D0` `arg1++` table walk (plausible source, explained); `D_801603D4` FAKE view base (marked in `TACO.h` and at the use, with the ticket); `func_801523D0` pad slots (FAKE, ticket). Every fakematch of this ticket now carries `FAKE` and T-16563: `func_801523D0`, `func_80152808` (and its `TACO.h` view), `func_801521EC`, `func_8015BD30` (two).

Checks after the fixes: `tools/docker.sh ninja` TACO sha1 OK, `tools/check_headers.py`, `tools/migrate_globals.py --check`, `tools/sync_protos.py --check-branch` (0 new, 15 known) OK. Moved to Done: 10 functions matched (4,740 bytes).
