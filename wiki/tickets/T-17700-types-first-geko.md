---
id: T-17700
title: "Types-first: GEKO"
status: In Review
assignee: claude (w3-geko-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[scenes-family-results]]"]
---

## Goal

Types-first pass for the GEKO overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover GEKO's shared data
types into `include/ovl/GEKO.h`, redraft every unmatched GEKO function with them (drivers adapted from T-16550,
T-16560 and T-17400 into `drafts/T-17700/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit GEKO`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/GEKO.h`; every matched GEKO function still matches.
- [x] Every unmatched GEKO function redrafted and scored full-TU; drafts in `drafts/T-17700/GEKO/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-17700)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/GEKO/*.c`, `include/ovl/GEKO.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 36 unmatched GEKO functions (24,148 bytes), 2 of them guarded drafts.

## Comments

### 2026-10-16 (claude, w3-geko-types)

Types (`include/ovl/GEKO.h`, block `OVL_GEKO_TYPES_T17700`): stack-copied tables `GekoMonthDays` (4 tables),
`GekoNames12`, `GekoWords11`/`GekoWords2`, `GekoFnTbl42`, `GekoShorts10` (3), `GekoShorts6` (2), byte and text
tables for `func_80142C9C`/`func_80143F48`/`func_80144270`, `GekoThresholds` reused for `D_80144CF8`, the `Rec24`
side arrays `s16 D_801463E0[12]`/`D_80146400[16]`, `u8 D_80145090`. No matched code used these symbols; GEKO sha1
OK after each step.

Matched: `func_8013E22C` (484 B, port of the EVENT clone `func_8010564C`). Guarded: `func_80136C20` (4 rows),
`func_80140040` (7, rewritten from the EVENT sibling), `func_80134228`/`func_80135AC8` (16 each),
`func_801355E8` (register numbers throughout). `func_80133024` hand edit (40 rows) kept in drafts.

Redraft: `drafts/T-17700/redraft.py` (T-17400 driver; fold after `fix_shared_hi` keeping the `%hi` offset, new
`stack-elements` and `gamestate-records` rewrites) and `drafts/T-17700/hand.py`. 29 unguarded functions: 14
better than the census (8,804 bytes), 11 same, 4 worse; sum 6,067 to 5,536 rows; none 0. Packages
T-17701..T-17708 (`w3-geko-p01`..`p08`, 29 functions, 16,196 bytes). Details: [[scenes-family-results]],
"Types-first pass (T-17700)". In Review, waiting for the code review.
