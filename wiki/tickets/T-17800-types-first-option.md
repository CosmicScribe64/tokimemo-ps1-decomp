---
id: T-17800
title: "Types-first: OPTION"
status: In Review
assignee: claude-opus (w3-option-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[option-family-results]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[shougatu-family-results]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit OPTION before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560 and their T-17500 adaptation), and cut the remaining work into matching packages.

## Acceptance criteria

- [x] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/OPTION.h`; globals carry their aggregate types; every matched OPTION function still matches (unit sha1 OK).
- [x] Every unmatched OPTION function has a fresh draft in `drafts/T-17800/OPTION/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [x] Drafts at or near 0 rows matched and committed.
- [x] Packages built with the wave-3 inventory scripts.
- [x] "Types-first pass (T-17800)" section on the OPTION family page.
- [x] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-option-types` from `w2-good`. Owns `src/ovl/OPTION/`, `include/ovl/OPTION.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 types, redraft, packages (claude-opus, branch `w3-option-types`)

- Types (`include/ovl/OPTION.h`): one include guard and `#include "game.h"`; the menu rows `D_8013D364`, `D_8013D372`, `D_8013D380`, `D_8013D38E` as `s16 [7]`; eight word tables (`D_8013D798`, `D_8013D880` of 58, `D_8013E148`..`D_8013E634` of 63); `D_8013D300`..`D_8013D308`, `u8 *D_8013D350[4]`; the full display-record view `OptActor44`; `OptSpr44`, `Arrs4x8` (with the `func_801325A0` prototype) and the three bit-field views moved from the `.c` files, plus `OptionDateBits`; prototype of `func_80132BA8`. `include/main_api.h`: `extern u8 D_800E7D37;` (addition). OPTION sha1 OK after each step.
- Redraft: `drafts/T-17800/redraft.py` (T-17500 driver with `OptActor44`, the `menu-arrays` rewrite and two m2c repairs) on the 32 functions still unmatched. Of the 30 unguarded, 19 better than the census (13,876 bytes), 10 the same, 1 worse; sum 5,722 to 5,362 rows. No score 0.
- Matched (4 functions, 1,548 bytes): `func_8013A09C`, `func_8013A1BC` (typed word tables, selector through a `u8` local), `func_80136434` (port of `func_80133AD8`), `func_80137040` (the `func_801368FC` shape). No new FAKE.
- Hand drafts for near misses: `func_8013394C` (5 funcdiff rows), `func_80139198` (12), `func_80132000` (52), `func_801342A8` (116), `func_8013A7EC` (218).
- Packages: T-17801..T-17803 (`w3-option-p01`..`p03`), 12 functions, 6,636 bytes.
- Details: [[option-family-results]], "Types-first pass (T-17800)".
