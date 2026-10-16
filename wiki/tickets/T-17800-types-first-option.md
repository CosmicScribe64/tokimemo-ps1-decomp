---
id: T-17800
title: "Types-first: OPTION"
status: In Progress
assignee: claude-opus (w3-option-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[shougatu-family-results]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit OPTION before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560 and their T-17500 adaptation), and cut the remaining work into matching packages.

## Acceptance criteria

- [ ] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/OPTION.h`; globals carry their aggregate types; every matched OPTION function still matches (unit sha1 OK).
- [ ] Every unmatched OPTION function has a fresh draft in `drafts/T-17800/OPTION/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [ ] Drafts at or near 0 rows matched and committed.
- [ ] Packages built with the wave-3 inventory scripts.
- [ ] "Types-first pass (T-17800)" section on the OPTION family page.
- [ ] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-option-types` from `w2-good`. Owns `src/ovl/OPTION/`, `include/ovl/OPTION.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments
