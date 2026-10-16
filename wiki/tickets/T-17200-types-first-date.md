---
id: T-17200
title: "Types-first: DATE"
status: Done
assignee: claude (w3-date-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[date-family-results]]"]
---

## Goal

Types-first pass for the DATE overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover DATE's shared data
types into `include/ovl/DATE.h`, redraft every unmatched DATE function with them (drivers adapted from T-16550 and
T-16560 into `drafts/T-17200/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit DATE`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/DATE.h`; every matched DATE function still matches.
- [x] Every unmatched DATE function redrafted and scored full-TU; drafts in `drafts/T-17200/DATE/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-17200)" section on [[date-family-results]].
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/DATE/*.c`, `include/ovl/DATE.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 (claude, w3-date-types)

Types: 68 step tables as `FnTblNN` in `include/ovl/DATE.h` (39 local typedefs and externs moved out of 16
`.c` files, 27 tables of unmatched handlers newly declared); stack-copied data tables `DateBytes11x21`,
`DateWords21` (x3), `Tbl21`, `DateShorts8` (x4), `DatePairs8`; prototype `func_8015A170`. DATE sha1 OK after
each step. Not converted (reasons on [[date-family-results]]): `D_8015E108` block, the three 13-word tables at
`D_8015CFD0`.

Redraft: `drafts/T-17200/redraft.py` (T-16560 driver + T-16550 fold + stack-table, fptab, record-temp,
display-records rewrites); 49 functions in `drafts/T-17200/DATE/`. Unguarded: 28 of 32 compile, 7 better
than the census (8,468 bytes), 16 same, 5 worse, sum 7,498 to 7,230 rows; none 0. draft_fixes changed nothing.

Matched (1,760 bytes): `func_8014AD9C` (TACO clone), `func_8013B208` (EVENT twin, one FAKE carried over from
the twin: discarded entry assignment, T-14300), `func_80159FAC`, `func_80158338`. Near misses with `hand.c`:
`func_801518B8` 28 rows, `func_80155004` 49, `func_80155E40`.

Packages T-17201..T-17208 (`w3-date-p01`..`p08`, 30 functions, 23,468 bytes). Checks: clean build 27/27,
`check_headers`, `migrate_globals --check`, `sync_protos --check-branch` (results in the log entry).

### 2026-10-16 (claude, code review against `CODING_STANDARDS.md`)

Reviewed `git diff w2-good...w3-date-types` (src, include, wiki, drafts). Checked: only `src/ovl/DATE/*.c` and
`include/ovl/DATE.h` changed outside `wiki/` and `drafts/T-17200/`; `drafts/` has new files only (875 added, none
changed); no inline asm, `.word` or `//` comments added; the four matches are plain C (the two sibling ports
carry no extra tricks; `func_8013B208` has one `FAKE` comment with reason and `T-17200`, copied from the EVENT twin
`func_80117D80`); every one of the 13 remaining `#ifdef NON_MATCHING` blocks in DATE has a `NON_MATCHING` comment
with a ticket and the rows left; header guards keep each declaration once (`check_headers` OK, `sync_protos
--check-branch` OK: 0 new, 15 known); no game data, asm dumps or binaries staged (drafts hold source, logs, JSON
as in earlier waves). DATE sha1 OK after a forced rebuild of the overlay.

Findings:
- Commit message of `8e4eb55c` says "47 local FnTblNN typedefs"; the real count is 39 typedef blocks in 16 files
  (this ticket and [[date-family-results]] have it right). Commit `2bdd2567` quotes interim redraft numbers (34
  unguarded, 10 better, 4 worse); the final numbers are 32 unguarded still open, 7 better, 16 same, 5 worse.
  Not fixed in history (rewriting the commits was refused); the corrected numbers are recorded here and on the
  results page.
- `wiki/index.md` said "9 stack tables"; the header declares 10 symbols (9 with new types plus `Tbl21`
  `D_8015B984`). Fixed in the index; the older log entry keeps "9" (new-type tables only).

No other findings. Code changes: none. Matched: 4 functions / 1,760 bytes.
