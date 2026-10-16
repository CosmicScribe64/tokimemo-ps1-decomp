---
id: T-17700
title: "Types-first: GEKO"
status: In Progress
assignee: claude (w3-geko-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]"]
---

## Goal

Types-first pass for the GEKO overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover GEKO's shared data
types into `include/ovl/GEKO.h`, redraft every unmatched GEKO function with them (drivers adapted from T-16550,
T-16560 and T-17400 into `drafts/T-17700/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit GEKO`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/GEKO.h`; every matched GEKO function still matches.
- [ ] Every unmatched GEKO function redrafted and scored full-TU; drafts in `drafts/T-17700/GEKO/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-17700)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/GEKO/*.c`, `include/ovl/GEKO.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
