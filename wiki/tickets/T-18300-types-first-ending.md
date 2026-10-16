---
id: T-18300
title: "Types-first: ENDING"
status: In Progress
assignee: claude (w3-ending-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[tickets/T-14000-ending-en-nichi-families]]"]
---

## Goal

Types-first pass for the ENDING overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover ENDING's shared
data types into `include/ovl/ENDING.h`, redraft every unmatched ENDING function with them (drivers adapted from
T-16550, T-16560 and T-18000 into `drafts/T-18300/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit ENDING`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/ENDING.h`; every matched ENDING function still matches.
- [ ] Every unmatched ENDING function redrafted and scored full-TU; drafts in `drafts/T-18300/ENDING/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-18300)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/ENDING/*.c`, `include/ovl/ENDING.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
