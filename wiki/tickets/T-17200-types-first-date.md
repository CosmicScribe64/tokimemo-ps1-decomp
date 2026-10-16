---
id: T-17200
title: "Types-first: DATE"
status: In Progress
assignee: claude (w3-date-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[date-family-results]]"]
---

## Goal

Types-first pass for the DATE overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover DATE's shared data
types into `include/ovl/DATE.h`, redraft every unmatched DATE function with them (drivers adapted from T-16550 and
T-16560 into `drafts/T-17200/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit DATE`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/DATE.h`; every matched DATE function still matches.
- [ ] Every unmatched DATE function redrafted and scored full-TU; drafts in `drafts/T-17200/DATE/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-17200)" section on [[date-family-results]].
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/DATE/*.c`, `include/ovl/DATE.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
