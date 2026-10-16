---
id: T-18500
title: "Types-first: BUNKAKEN"
status: In Progress
assignee: claude (w3-bunkaken-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[tickets/T-12700-bunka-families]]", "[[bunka-family-results]]"]
---

## Goal

Types-first pass for the BUNKAKEN overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover BUNKAKEN's
shared data types into `include/ovl/BUNKAKEN.h`, redraft every unmatched BUNKAKEN function with them (drivers
adapted from T-16550, T-16560 and T-18100 into `drafts/T-18500/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit BUNKAKEN`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/BUNKAKEN.h`; every matched BUNKAKEN function still matches.
- [ ] Every unmatched BUNKAKEN function redrafted and scored full-TU; drafts in `drafts/T-18500/BUNKAKEN/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-18500)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/BUNKAKEN/*.c`, `include/ovl/BUNKAKEN.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 12 unmatched BUNKAKEN functions (14,256 bytes by `tools/siblings.py`), 1 of them a guarded draft (`func_801428D0`).

## Comments
