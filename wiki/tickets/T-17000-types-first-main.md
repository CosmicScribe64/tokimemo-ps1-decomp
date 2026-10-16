---
id: T-17000
title: "Types-first: main"
status: In Progress
assignee: claude (w3-main-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[main-family-results]]", "[[data-types]]", "[[tickets/T-16990-wave-3-conveyor]]"]
---

## Goal

Recover the main executable's shared data types (structs, arrays, stride tables, bit-field views, stack tables,
callback tables) in `include/main_api.h` and additions to `include/game.h` before function matching, then redraft
every unmatched function in `src/main/` with them and cut the rest into packages for wave 3. Same method as the
pilots [[tickets/T-16550-taiiku-types-first]] and [[tickets/T-16560-taco-types-first]].

## Acceptance criteria

- [ ] Survey: type_recovery, siblings, census drafts, wiki notes for main
- [ ] Types declared; every matched function still matches (main sha1 OK)
- [ ] Every unmatched main function redrafted under `drafts/T-17000/main/<func>/` with score and census score
- [ ] Score-0 or near-zero drafts finished and committed as matches
- [ ] Packages listed (inventory2 / mklists3)
- [ ] "Types-first pass (T-17000)" section on [[main-family-results]]
- [ ] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch pass

## Notes

Branch `w3-main-types` from `w2-good`. Ownership: `src/main/*.c`, `include/main_api.h`; `include/game.h`
additions only.

## Comments
