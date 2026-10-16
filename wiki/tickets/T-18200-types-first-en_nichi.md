---
id: T-18200
title: "Types-first: EN_NICHI"
status: In Progress
assignee: claude (w3-en-nichi-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]"]
---

## Goal

Types-first pass for the EN_NICHI overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover EN_NICHI's shared
data types into `include/ovl/EN_NICHI.h`, redraft every unmatched EN_NICHI function with them (drivers adapted from
T-16550, T-16560 and later passes into `drafts/T-18200/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit EN_NICHI`, siblings, census drafts.
- [ ] Recovered structs, arrays and tables declared in `include/ovl/EN_NICHI.h`; every matched EN_NICHI function still matches.
- [ ] Every unmatched EN_NICHI function redrafted and scored full-TU; drafts in `drafts/T-18200/EN_NICHI/<func>/`.
- [ ] Score-0 or near-zero drafts finished and matched where possible.
- [ ] Packages computed; "Types-first pass (T-18200)" section on the family page.
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/EN_NICHI/*.c`, `include/ovl/EN_NICHI.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
