---
id: T-18600
title: "Types-first: MASTER"
status: In Review
assignee: claude (w3-master-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[master-types-pilot]]", "[[master-family-results]]"]
---

## Goal

Types-first pass for the MASTER overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover MASTER's shared
data types into `include/ovl/MASTER.h`, redraft every unmatched MASTER function with them (drivers adapted from
T-16550, T-16560 and their descendants into `drafts/T-18600/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit MASTER`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/MASTER.h`; every matched MASTER function still matches.
- [x] Every unmatched MASTER function redrafted and scored full-TU; drafts in `drafts/T-18600/MASTER/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-18600)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/MASTER/*.c`, `include/ovl/MASTER.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 11 unmatched MASTER functions (17,412 bytes by `tools/siblings.py`), none guarded; no sibling at 0.9 or
above (one at 0.85: `func_8013B448` from `func_80138088`).

Result ([[master-family-results]], section "Types-first pass (T-18600)"):

- Types in `include/ovl/MASTER.h`: nine `MasterWords13` stack tables, `MasterHalfwords13 D_8013C6A4`, the step
  tables `FnTbl4/7/30` and `PtrTbl3` moved from four `.c` files, `u8 D_8013C788[5]`, `D_8013C780`,
  `D_8013C794`, one `MasterClubBits` for the two local `MasterSlot` copies, `void func_8013348C(void)`.
  `include/game.h` and `include/main_api.h` unchanged.
- Driver `drafts/T-18600/redraft.py` (from T-18300) with MasterSprite44 views, an innermost-range fold and
  three new rewrites (`club-bits`, `record-copy`, `reroll`). 11 redrafts: 8 better than the census
  (14,240 bytes), 3 the same, none worse; sum 4,793 to 3,044 rows; none 0.
- Matched: `func_80135CF4` (708 bytes, one FAKE: byte views and an index local above the table) and
  `func_8013348C` (500 bytes). Hand drafts for `func_80132444` (22 lines), `func_8013B804` (72) and
  `func_80135FB8` (132, port of the matched twin `func_80133F80`).
- Packages T-18601..T-18603 (7 functions, 10,872 bytes); `func_801344E0` and `func_80139CC8` are not in a
  package.

## Comments

- 2026-10-16: work done on branch `w3-master-types`; In Review, waiting for the code review against `CODING_STANDARDS.md`.
- 2026-10-16: final checks: clean `rm -rf build && configure.py && ninja` 665/665, all 27 sha1 OK; `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` 0 new disagreements (15 known).
