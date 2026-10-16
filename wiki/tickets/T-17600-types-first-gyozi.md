---
id: T-17600
title: "Types-first: GYOZI"
status: In Progress
assignee: claude (w3-gyozi-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]"]
---

## Goal

Types-first pass for the GYOZI overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover GYOZI's shared data
types into `include/ovl/GYOZI.h`, redraft every unmatched GYOZI function with them (drivers adapted from T-16550,
T-16560 and their descendants into `drafts/T-17600/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit GYOZI`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/GYOZI.h`; every matched GYOZI function still matches.
- [ ] Every unmatched GYOZI function redrafted and scored full-TU; drafts in `drafts/T-17600/GYOZI/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-17600)" section on the GYOZI family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/GYOZI/*.c`, `include/ovl/GYOZI.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
