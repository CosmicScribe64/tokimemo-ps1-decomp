---
id: T-17400
title: "Types-first: EVENT"
status: Done
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

### 2026-10-16 (claude, code review against CODING_STANDARDS.md)

Scope: `git diff w2-good...w3-event-types` (9 commits). Only `src/ovl/EVENT/*.c`, `include/ovl/EVENT.h`, `drafts/T-17400/`
(690 new files, nothing in an existing `drafts/` folder changed) and wiki pages changed; `main_api.h`, `game.h` and the other
overlays are untouched. No `//` comment, no inline asm or `.word`, no non-ASCII text in the added source; every reported
match (`func_800FF320`, `func_80104C3C`, `func_800FF61C`, `func_8010AE7C`) is plain C with the `INCLUDE_ASM` removed; the
drafts folder holds C, text, JSON and logs only (no asm, binaries or game data). Header: no duplicated extern, no local
`FnTblNN`/`Ev44`/`EvTbl`/`U16x21` definition left in the `.c` files; the `D_80122EC8` override sits in the header before
`common.h`.

Findings, both fixed (comment-only, commit `6d84b5df`):

1. `func_800FF320`: the `FAKE` comment (the 8 unused bytes `s32 pad[2]`) began with "Port of the DATE clone" and sat on the
   function's first line; section 7 wants a comment that starts with `FAKE`. Split into the port note and a `FAKE: ... T-9310`
   comment on the `pad[2]` declaration. The fakematch is the clone's own (DATE `func_80152684`); it stays tracked under
   T-9310 and is listed here for T-17400.
2. `func_8010B614` (`src/ovl/EVENT/8010B360.c`, guarded draft of package T-17407): the `NON_MATCHING: T-10610` comment did not
   say how much is left. Added "33 diff rows left (T-17400 score)". The other three guards (`func_80114B0C` 4 lines,
   `func_80118868` 8 lines, `func_8011EDFC` 13 lines) already name a ticket and the lines left.

Accepted without change: the cast views of the display records (`D_800EAFA0` as `Ev44`, `Rec24` over `D_800EC190`) are the
same views that the matched DATE ports and the existing EVENT code use; `main_api.h` types those symbols as `u8[]`, and the
views are described in the Survey of [[event-family-results]].

Verification after the fixes (Docker): `tools/docker.sh ninja` rebuilt EVENT, `build/ovl/EVENT.bin: OK`; `check_headers`
(headers OK), `migrate_globals.py --check` (globals OK) and `sync_protos.py --check-branch` (0 new disagreements, 15 known)
pass. The merge script runs the clean configure plus the full 27/27 build.
