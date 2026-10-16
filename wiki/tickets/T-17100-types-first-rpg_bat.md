---
id: T-17100
title: "Types-first: RPG_BAT"
status: In Review
assignee: claude-opus (w3-rpg-bat-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit RPG_BAT before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560), and cut the remaining work into matching packages.

## Acceptance criteria

- [x] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/RPG_BAT.h`; globals carry their aggregate types; every matched RPG_BAT function still matches (unit sha1 OK).
- [x] Every unmatched RPG_BAT function has a fresh draft in `drafts/T-17100/RPG_BAT/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [x] Drafts at or near 0 rows matched and committed.
- [x] Packages built with the wave-3 inventory scripts.
- [x] "Types-first pass (T-17100)" section on the RPG_BAT family page.
- [x] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-rpg-bat-types` from `w2-good`. Owns `src/ovl/RPG_BAT/`, `include/ovl/RPG_BAT.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 types, redraft, packages (claude-opus, branch `w3-rpg-bat-types`)

- Types in `include/ovl/RPG_BAT.h`: stack tables `RpgDirTable`, `RpgStrTbl4`, `RpgWordTbl4`, `RpgWordTbl10`, `RpgHalfTbl12` (12 globals), `RpgStat` pad named `unk_0C`..`unk_34` (`func_801447E0` updated), `void` prototypes for `func_80149AA8`/`func_8014A16C`. RPG_BAT sha1 OK after each.
- Redraft: `drafts/T-17100/redraft.py` (T-16560 runner + T-16550 fold + stack-copy and record-offset rewrites + census variant). 74 drafts in `drafts/T-17100/RPG_BAT/`: 20 better than the census, 53 same, 1 worse; sum 17,474 to 16,558 rows.
- Matched without a guard: `func_80157640`, `func_8014F88C` (FAKE `sp28`), `func_801525B0` (FAKE `sp28`), `func_801498C0`: 3,168 bytes. Near misses in `hand.c`: `func_80152080` (frame), `func_8014A3DC` (one `lui`), `func_8015A8B4` (16 lines).
- Packages: 19 (`w3-rpg-bat-p01`..`p19`, T-17101..T-17119), 62 functions, 64,940 bytes.
- Not done: the `D_8015EB7C` work block and the `8014E780.c` `.data` scalars as structs (see [[rpg-bat-family-results]]).
- Waiting for the code review against `CODING_STANDARDS.md`.

