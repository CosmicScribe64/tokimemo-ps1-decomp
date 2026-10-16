---
id: T-18100
title: "Types-first: BUNKA_SD"
status: In Progress
assignee: claude-opus (w3-bunka-sd-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[bunka-family-results]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit BUNKA_SD before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560 and their later adaptations), and cut the remaining work into matching packages.

## Acceptance criteria

- [ ] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/BUNKA_SD.h`; globals carry their aggregate types; every matched BUNKA_SD function still matches (unit sha1 OK).
- [ ] Every unmatched BUNKA_SD function has a fresh draft in `drafts/T-18100/BUNKA_SD/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [ ] Drafts at or near 0 rows matched and committed.
- [ ] Packages built with the wave-3 inventory scripts.
- [ ] "Types-first pass (T-18100)" section on the family page.
- [ ] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-bunka-sd-types` from `w2-good`. Owns `src/ovl/BUNKA_SD/`, `include/ovl/BUNKA_SD.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments
