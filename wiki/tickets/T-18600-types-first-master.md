---
id: T-18600
title: "Types-first: MASTER"
status: In Progress
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

- [ ] Survey: `tools/type_recovery.py --unit MASTER`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/MASTER.h`; every matched MASTER function still matches.
- [ ] Every unmatched MASTER function redrafted and scored full-TU; drafts in `drafts/T-18600/MASTER/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-18600)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/MASTER/*.c`, `include/ovl/MASTER.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
