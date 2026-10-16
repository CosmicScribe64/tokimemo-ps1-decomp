---
id: T-17100
title: "Types-first: RPG_BAT"
status: In Progress
assignee: claude-opus (w3-rpg-bat-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit RPG_BAT before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560), and cut the remaining work into matching packages.

## Acceptance criteria

- [ ] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/RPG_BAT.h`; globals carry their aggregate types; every matched RPG_BAT function still matches (unit sha1 OK).
- [ ] Every unmatched RPG_BAT function has a fresh draft in `drafts/T-17100/RPG_BAT/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [ ] Drafts at or near 0 rows matched and committed.
- [ ] Packages built with the wave-3 inventory scripts.
- [ ] "Types-first pass (T-17100)" section on the RPG_BAT family page.
- [ ] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-rpg-bat-types` from `w2-good`. Owns `src/ovl/RPG_BAT/`, `include/ovl/RPG_BAT.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments
