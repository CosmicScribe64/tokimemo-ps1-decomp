---
id: T-18800
title: "Types-first: DATE2"
status: In Progress
assignee: claude (w3-date2-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[tickets/T-13900-scenes-families]]", "[[scenes-family-results]]"]
---

## Goal

Types-first pass for the DATE2 overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover DATE2's shared
data types into `include/ovl/DATE2.h`, redraft every unmatched DATE2 function with them (drivers adapted from
T-16550, T-16560 and the later types-first passes into `drafts/T-18800/redraft.py`), and cut the rest into work
packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit DATE2`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/DATE2.h`; every matched DATE2 function still matches.
- [ ] Every unmatched DATE2 function redrafted and scored full-TU; drafts in `drafts/T-18800/DATE2/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-18800)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/DATE2/*.c`, `include/ovl/DATE2.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
