---
id: T-17000
title: "Types-first: main"
status: In Review
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

- [x] Survey: type_recovery, siblings, census drafts, wiki notes for main
- [x] Types declared; every matched function still matches (main sha1 OK)
- [x] Every unmatched main function redrafted under `drafts/T-17000/main/<func>/` with score and census score
- [x] Score-0 or near-zero drafts finished and committed as matches
- [x] Packages listed (inventory2 / mklists3)
- [x] "Types-first pass (T-17000)" section on [[main-family-results]]
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch pass

## Notes

Branch `w3-main-types` from `w2-good`. Ownership: `src/main/*.c`, `include/main_api.h`; `include/game.h`
additions only.

## Comments

### 2026-10-16 claude (w3-main-types)

- Survey: type_recovery over main (1,724 proposals), siblings (157 of 163 targets without a matched sibling
  at 0.70 or more), census drafts (byte casts mostly on GPU packets and the shared main tables). The large
  main aggregates are shared with overlay C under other views; outside this ticket's files.
- Types (`include/main_api.h`, additive): main-only tables `D_800B6890`/`D_800B68A8`, `D_800B68E0`/`D_800B6928`,
  `D_800C6200`, `D_800C62C0`, `D_80125DA0`/`D_80125E20`, `D_800B5AA4`/`D_800B5AE4`, `D_800B6CF8`; the
  `Flags7D4F` bit-field view; prototypes of the seven callees of `func_80075320`. Main SHA1 OK after each.
- Redraft (`drafts/T-17000/redraft.py`, T-16550 fold + T-16560 rewrites + new `struct-walk`): 163 functions;
  140 unguarded: 45 better than the census (34,208 bytes), 72 same, 9 worse, 14 without a compiling draft;
  median 153.5 to 151. No score 0.
- Matched from the redrafts: `func_80075320`, `func_80053F04`, `func_80075AF8`, `func_8007959C`,
  `func_80077E30` (5 functions, 1,948 bytes; three with the FAKE byte-view selector of T-10630).
- Packages: T-17001..T-17027 (`w3-main-p01`..`p27`), 105 functions, 75,216 bytes.
- Checks at the end: clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`
  (results below). Waiting for the code review.
