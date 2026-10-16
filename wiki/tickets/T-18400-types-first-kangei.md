---
id: T-18400
title: "Types-first: KANGEI"
status: In Progress
assignee: claude-opus (w3-kangei-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit KANGEI before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560 and their later adaptations), and cut the remaining work into matching packages.

## Acceptance criteria

- [ ] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/KANGEI.h`; globals carry their aggregate types; every matched KANGEI function still matches (unit sha1 OK).
- [ ] Every unmatched KANGEI function has a fresh draft in `drafts/T-18400/KANGEI/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [ ] Drafts at or near 0 rows matched and committed.
- [ ] Packages built with the wave-3 inventory scripts.
- [ ] "Types-first pass (T-18400)" section on the family page.
- [ ] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-kangei-types` from `w2-good`. Owns `src/ovl/KANGEI/`, `include/ovl/KANGEI.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments
