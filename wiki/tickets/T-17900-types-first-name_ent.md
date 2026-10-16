---
id: T-17900
title: "Types-first: NAME_ENT"
status: In Progress
assignee: claude (w3-name-ent-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[name-ent-family-results]]"]
---

## Goal

Types-first pass for the NAME_ENT overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover NAME_ENT's shared
data types into `include/ovl/NAME_ENT.h`, redraft every unmatched NAME_ENT function with them (drivers adapted from
T-16550, T-16560 and T-17700 into `drafts/T-17900/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit NAME_ENT`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/NAME_ENT.h`; every matched NAME_ENT function still matches.
- [ ] Every unmatched NAME_ENT function redrafted and scored full-TU; drafts in `drafts/T-17900/NAME_ENT/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-17900)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/NAME_ENT/*.c`, `include/ovl/NAME_ENT.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 32 `INCLUDE_ASM` entries in 4 NAME_ENT files.

## Comments
