---
id: T-18100
title: "Types-first: BUNKA_SD"
status: In Review
assignee: claude-opus (w3-bunka-sd-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[bunka-sd-family-results]]", "[[bunka-family-results]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit BUNKA_SD before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560 and their later adaptations), and cut the remaining work into matching packages.

## Acceptance criteria

- [x] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/BUNKA_SD.h`; globals carry their aggregate types; every matched BUNKA_SD function still matches (unit sha1 OK).
- [x] Every unmatched BUNKA_SD function has a fresh draft in `drafts/T-18100/BUNKA_SD/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [x] Drafts at or near 0 rows matched and committed.
- [x] Packages built with the wave-3 inventory scripts.
- [x] "Types-first pass (T-18100)" section on the family page.
- [x] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-bunka-sd-types` from `w2-good`. Owns `src/ovl/BUNKA_SD/`, `include/ovl/BUNKA_SD.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 types, redraft, packages (claude-opus, branch `w3-bunka-sd-types`)

- Types (`include/ovl/BUNKA_SD.h`, now including `game.h`): `BsdEvent` and the event-list and text tables of the three scenes (`D_8013C654`, `D_8013BA20`, `D_8013CE8C`, `D_8013C814`, `D_8013D90C`, `D_8013D0C0`); the seven loaded picture sets (`s32 *` page table, two `u8 *` images, palette word) at `D_8013C6E8`, `D_8013C710`, `D_8013C720`, `D_8013C730`, `D_8013C740`, `D_8013CEDC`, `D_8013D9A8` (matched `func_80135630` and `func_80136578` now index the page table); prototypes of 14 overlay callees. No `game.h` or `main_api.h` change. BUNKA_SD sha1 OK after each step.
- Redraft: `drafts/T-18100/redraft.py` (the T-17800 driver plus `pointer-index` and `record-walk`) on the 15 functions unmatched at that point: 6 better than the census, 8 the same, 1 worse (`func_80139290`, operand-order effect of the GameState field). No score 0 from the driver; hand drafts improve the three event interpreters to 57-59 rows (from 131-135), `func_80137890` to 19, `func_80134D94` to 137.
- Matched (9 functions, 4,820 bytes): `func_801336AC`, `func_80136B3C`, `func_80137CAC`, `func_80133984`, `func_80137FEC`, `func_80134938`, `func_80135CD4`, `func_80136F3C`, `func_80135F38` (FAKE: `(&D_80122EAC)[0]` as a non-plain compare operand).
- Packages: T-18101..T-18103 (`w3-bunka-sd-p01`..`p03`), 11 functions, 5,908 bytes.
- Checks: clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` (0 new) pass.
- Details: [[bunka-sd-family-results]], "Types-first pass (T-18100)".
