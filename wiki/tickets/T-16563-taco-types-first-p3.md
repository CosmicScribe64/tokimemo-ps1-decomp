---
id: T-16563
title: "Types-first pilot package pt-taco-p3"
status: In Review
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
- [x] Unmatched drafts under 30 differing lines guarded with a ticketed `NON_MATCHING` comment, the rest restored to `INCLUDE_ASM` (`func_8015B1B8` guarded, 8 lines; `func_8015BD30` and `func_8015ACCC` stay `INCLUDE_ASM`)
- [x] Section in `wiki/matching-notes.md`; T-0018 rows for compiler-gap families
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch OK

## Notes

Worktree `pt-taco-p3` (branch `pt-taco-p3`, from `w2-good`). Owns `src/ovl/TACO/80151C30.c` and `src/ovl/TACO/8015ABF0.c`;
`include/`, `config/` and `wiki/` edits where needed.

## Comments

### 2026-10-16 (claude, pt-taco-p3): matching stage finished, In Review

Branch `pt-taco-p3` (from `w2-good`), about 60 minutes of agent time (03:55 to 04:55 UTC). 9 of the 10 listed functions matched in plain C, 4,164 bytes, one commit each, then the unlisted `func_8015BD30` (576 bytes, 4,740 in all): `func_8015C0A0` (360), `func_80151D1C` (632), `func_80152944` (796), `func_8015BF70` (304), `func_80152808` (316), `func_80152DB4` (692), `func_801523D0` (668), `func_801521EC` (196), `func_8015CE64` (200). `func_80151D1C` matched at the first build of a hand-written body, `func_80152944` and `func_801521EC` at the second; none matched from the printed m2c draft.

Fakematches (marked `FAKE` with this ticket): `func_801523D0` (`s32 pad[15]` and `pad2` reproduce the frame and the slot of `k`), `func_80152808` (pointer table read from the base `D_801603D4`, declared `s16 *D_801603D4[]` in `TACO.h`), `func_8015BD30` (`if (i) {}` splits the live range of `i`).

Left: `func_8015B1B8` guarded (8 differing lines, struct-copy temporary `t2` against `t3`, T-0018 row); `func_8015BD30` (not on the list, 576 bytes) matched after about 35 minutes (first pass left 65 rows; the 1-second scratch compile found the rest), with a FAKE empty test on `i`; `func_8015ACCC` (1260 bytes, no draft) rewritten by hand for about 20 minutes, 437 lines left, stays `INCLUDE_ASM` (T-0018 row).

Header changes (`include/ovl/TACO.h`): `D_80160290` (`TcPair8[]`) and `D_801603A8` (`TcPair2[]`), `D_80160444` from `s16 *` to `s16 *[]` (four scalar uses became `[0]`), `D_801603D4` (FAKE view), prototypes of `func_80152944`, `func_80152DB4`, `func_801523D0`. Checks: clean `rm -rf build && configure.py && ninja` 27/27 SHA1 OK, `check_headers.py` OK, `migrate_globals.py --check` OK, `sync_protos.py --check-branch` OK (0 new, 15 known). Notes: [[matching-notes]] "T-16563: TACO package p3", [[taco-family-results]], five new rows in [[register-cheatsheet]]. Waiting for the code review (CODING_STANDARDS.md checklist) before Done.
