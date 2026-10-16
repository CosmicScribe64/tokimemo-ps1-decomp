---
id: T-17400
title: "Types-first: EVENT"
status: In Progress
assignee: claude (w3-event-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]"]
---

## Goal

Types-first pass for the EVENT overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover EVENT's shared data
types into `include/ovl/EVENT.h`, redraft every unmatched EVENT function with them (drivers adapted from T-16550,
T-16560 and T-17200 into `drafts/T-17400/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit EVENT`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/EVENT.h`; every matched EVENT function still matches.
- [ ] Every unmatched EVENT function redrafted and scored full-TU; drafts in `drafts/T-17400/EVENT/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-17400)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/EVENT/*.c`, `include/ovl/EVENT.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 40 unmatched EVENT functions (39,712 bytes), 4 of them guarded drafts.

## Comments
