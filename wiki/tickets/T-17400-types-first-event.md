---
id: T-17400
title: "Types-first: EVENT"
status: In Review
assignee: claude (w3-event-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[event-family-results]]"]
---

## Goal

Types-first pass for the EVENT overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover EVENT's shared data
types into `include/ovl/EVENT.h`, redraft every unmatched EVENT function with them (drivers adapted from T-16550,
T-16560 and T-17200 into `drafts/T-17400/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit EVENT`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/EVENT.h`; every matched EVENT function still matches.
- [x] Every unmatched EVENT function redrafted and scored full-TU; drafts in `drafts/T-17400/EVENT/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-17400)" section on [[event-family-results]].
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/EVENT/*.c`, `include/ovl/EVENT.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 40 unmatched EVENT functions (39,712 bytes), 4 of them guarded drafts.

## Comments

### 2026-10-16 (claude, w3-event-types)

Types: 90 step tables as `FnTblNN` in `include/ovl/EVENT.h` (63 local typedefs and externs moved out of 15 `.c`
files, 27 tables of unmatched handlers newly declared; `D_801218E8` from `Blob80` to `FnTbl32`; the `D_80122EC8`
override moved to the header); stack-copied data tables `U16x21` (moved), `EvPairs8`, `EvShorts82`, `EvShorts10`,
`EvShorts11`, `EvShorts5`; views `Ev44`, `EvEnt`/`EvTbl` moved; prototype `func_8010AFE4`. EVENT sha1 OK after
each step. The commit message of `f84376d6` says 29 new tables; the final count is 27 (`D_801220EC` and
`D_8012277C` were dropped: `main_api.h` has no override guard for them).

Redraft: `drafts/T-17400/redraft.py` (T-17200 driver with EVENT's display records `D_800E9620` and `Rec24`
records `D_800EC190` as cast views). 34 unguarded functions: 32 drafts compile, 9 better than the census (7,560
bytes), 21 the same, 2 worse; sum 11,600 to 11,395 diff rows; none 0.

Matched (4 functions, 1,416 bytes): `func_800FF320` (708, DATE clone), `func_80104C3C` (268, DATE clone),
`func_800FF61C` (244, typed redraft), `func_8010AE7C` (196, GEKO sibling port). Near misses with `hand.c`:
`func_800FD00C` 6 rows, `func_8010F340` 11, `func_801022C0` 14.

Packages: T-17401..T-17407 (w3-event-p01..p07, 27 functions, 18,224 bytes). Details: [[event-family-results]].

