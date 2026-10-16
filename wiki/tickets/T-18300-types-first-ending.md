---
id: T-18300
title: "Types-first: ENDING"
status: In Review
assignee: claude (w3-ending-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[tickets/T-14000-ending-en-nichi-families]]", "[[ending-family-results]]"]
---

## Goal

Types-first pass for the ENDING overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover ENDING's shared
data types into `include/ovl/ENDING.h`, redraft every unmatched ENDING function with them (drivers adapted from
T-16550, T-16560 and T-18000 into `drafts/T-18300/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit ENDING`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/ENDING.h`; every matched ENDING function still matches.
- [x] Every unmatched ENDING function redrafted and scored full-TU; drafts in `drafts/T-18300/ENDING/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-18300)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/ENDING/*.c`, `include/ovl/ENDING.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 14 unmatched ENDING functions (16,188 bytes by `tools/siblings.py`), 1 of them a guarded draft (`func_80134B18`).

## Comments

### 2026-10-16 claude (w3-ending-types)

- Matched `func_80132F18` (280 bytes) as a port of main `check_k_scroll` (`tools/siblings.py` 0.978), with a
  FAKE unused `get_k_work` result that keeps the frame.
- Types (`include/ovl/ENDING.h`): `EdShorts18 D_8013C3B0`, `EdShorts14 D_8013CA60`/`D_8013CA7C` (stack-copied
  tables), `s32 **D_8013CAF0`, `MAIN_API_OVERRIDE_dialogue_render` (s32 result). Every ENDING file now includes
  `game.h` after `ovl/ENDING.h`. Location records and type_recovery's 0xC-stride arrays left as scalars
  (reasons on [[ending-family-results]]). `include/game.h` and `include/main_api.h` unchanged.
- Redraft (`drafts/T-18300/`): 13 functions; over the 12 unguarded ones 7 better than the census (7,268 bytes,
  4 of them from hand drafts of the scene scripts), 5 the same, none worse, median 161.5 to 142.5 rows. No
  draft scored 0.
- Packages: T-18301..T-18305 (`w3-ending-p01`..`p05`, 13 functions, 16,240 bytes in the package lists).
- Checks: clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`.
