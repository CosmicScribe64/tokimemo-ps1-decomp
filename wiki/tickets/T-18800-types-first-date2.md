---
id: T-18800
title: "Types-first: DATE2"
status: Done
assignee: claude (w3-date2-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[tickets/T-13900-scenes-families]]", "[[scenes-family-results]]"]
---

## Goal

Types-first pass for the DATE2 overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover DATE2's shared
data types into `include/ovl/DATE2.h`, redraft every unmatched DATE2 function with them (drivers adapted from
T-16550, T-16560 and the later types-first passes into `drafts/T-18800/redraft.py`), and cut the rest into work
packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit DATE2`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/DATE2.h`; every matched DATE2 function still matches.
- [x] Every unmatched DATE2 function redrafted and scored full-TU; drafts in `drafts/T-18800/DATE2/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-18800)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/DATE2/*.c`, `include/ovl/DATE2.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 10 unmatched DATE2 functions (15,752 bytes by `tools/siblings.py`), none guarded.

## Comments

### 2026-10-16 claude (w3-date2-types): pass done, In Review

- Types in `include/ovl/DATE2.h`: `s16 D_8013A5D4[3]`, `D_8013A5DC[3]`, `D_8013A5E4[3]`, `D_8013A5EC[3]` (the dots
  of `func_80135E50`), `u16 D_8013A5CC`, `s16 D_8013A5D0`, `s16 D_8013B5E4[4]` (label widths),
  `Date2Labels4x21 D_8013B634` (stack-copied labels), `#include "game.h"`, `s32 func_80135E50()`,
  `void func_801365DC(s32)`. `config/overlays/DATE2.yaml` and `src/ovl/DATE2/80132DE0.c`: `.data` island of
  `80132DE0` (`tools/data_island.py`), the four dot arrays defined in C.
- Redraft (`drafts/T-18800/`, 10 functions): 9 better than the census, 1 the same; diff rows 5,067 to 4,380,
  median 308.5 to 192.5. Driver: T-18500 plus the `Rec24` fold, `rect-view`, `array-views`, 2-D `stack-elements`.
- Matched `func_80135E50` (1,932 B) from the DATE twin, with one FAKE (an empty test before the counter compare,
  commented in the source). Hand drafts: `func_80135E50` 25 rows before the FAKE, `func_80136794` 38.
- Packages: T-18801 `w3-date2-p01` (`80137D50.c`, 3 functions, 3,784 B), T-18802 `w3-date2-p02` (`80132DE0.c`,
  4 functions, 3,200 B). `func_80133AF0` and `func_801350A4` left for later.
- Wiki: [[scenes-family-results]] "Types-first pass (T-18800)", [[matching-notes]] "DATE2 types-first pass (T-18800)".

### 2026-10-16 claude (review): code review against CODING_STANDARDS.md, Done

- Scope: only `src/ovl/DATE2/80132DE0.c`, `include/ovl/DATE2.h`, `config/overlays/DATE2.yaml`, new `drafts/T-18800/` folders and wiki pages changed; no existing drafts archive touched; no game data, asm or binaries; no inline asm, `.word` or pragma; C89 and ASCII only.
- `func_80135E50` (1,932 B) is plain C ported from the matched DATE twin `func_801489D0`; its one fakematch (empty `if (var_s3) {}`) carries a `FAKE (T-18800)` comment with the reason. No `NON_MATCHING` block in DATE2. The `.data` island follows the documented `tools/data_island.py` step (T-9010), same shape as the DATE files.
- Header: each new declaration appears once, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` 0 new disagreements (15 known).
- Finding 1 (minor, fixed): the `s32` return of `func_80135E50` (implicit int, only the case 2 error path sets `v0`) was explained in the wiki but not in the source. Added a comment; rebuilt, DATE2 sha1 OK.
- Matched: 1 function, 1,932 bytes (`DATE2:func_80135E50`).
