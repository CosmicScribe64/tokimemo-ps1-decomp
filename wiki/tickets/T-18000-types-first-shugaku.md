---
id: T-18000
title: "Types-first: SHUGAKU"
status: In Progress
assignee: claude (w3-shugaku-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]"]
---

## Goal

Types-first pass for the SHUGAKU overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover SHUGAKU's shared
data types into `include/ovl/SHUGAKU.h`, redraft every unmatched SHUGAKU function with them (drivers adapted from
T-16550, T-16560 and T-17700 into `drafts/T-18000/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit SHUGAKU`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/SHUGAKU.h`; every matched SHUGAKU function still matches.
- [ ] Every unmatched SHUGAKU function redrafted and scored full-TU; drafts in `drafts/T-18000/SHUGAKU/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-18000)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/SHUGAKU/*.c`, `include/ovl/SHUGAKU.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 25 unmatched SHUGAKU functions (9,908 bytes), 3 of them guarded drafts.

## Comments
