---
id: T-17300
title: "Types-first: ETC"
status: In Progress
assignee: claude (w3-etc-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[etc-family-results]]"]
---

## Goal

Types-first pass for the ETC overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover ETC's shared data
types into `include/ovl/ETC.h`, redraft every unmatched ETC function with them (drivers adapted from T-16550 and
T-16560 into `drafts/T-17300/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit ETC`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/ETC.h`; every matched ETC function still matches.
- [ ] Every unmatched ETC function redrafted and scored full-TU; drafts in `drafts/T-17300/ETC/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-17300)" section on [[etc-family-results]].
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/ETC/*.c`, `include/ovl/ETC.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
