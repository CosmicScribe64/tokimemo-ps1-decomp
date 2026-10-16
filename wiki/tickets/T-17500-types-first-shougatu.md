---
id: T-17500
title: "Types-first: SHOUGATU"
status: Done
assignee: claude-opus (w3-shougatu-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[shougatu-family-results]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[event-family-results]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit SHOUGATU before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560 and their T-17100 adaptation), and cut the remaining work into matching packages.

## Acceptance criteria

- [x] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/SHOUGATU.h`; globals carry their aggregate types; every matched SHOUGATU function still matches (unit sha1 OK).
- [x] Every unmatched SHOUGATU function has a fresh draft in `drafts/T-17500/SHOUGATU/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [x] Drafts at or near 0 rows matched and committed.
- [x] Packages built with the wave-3 inventory scripts.
- [x] "Types-first pass (T-17500)" section on the SHOUGATU family page.
- [x] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-shougatu-types` from `w2-good`. Owns `src/ovl/SHOUGATU/`, `include/ovl/SHOUGATU.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 types, redraft, packages (claude-opus, branch `w3-shougatu-types`)

- Types (`include/ovl/SHOUGATU.h`): the 26 `FnTblNN` step-table typedefs and 45 externs moved from 21 `.c` files to the header, plus the four tables of unmatched handlers; `ShoShorts7`, `ShoStr7x5`, `ShoColors6x2`, `ShoBytes3x12` for the stack-copied constant tables; the 50-entry work arrays of the `func_80141D20` family; `#include "game.h"`; prototype of `func_80137664`. `include/main_api.h`: `extern u8 D_800CA248[];` (addition). SHOUGATU sha1 OK after each step.
- Redraft: `drafts/T-17500/redraft.py` (T-17200 driver plus the T-17100 census variant) on all 38 functions. Of the 32 unguarded functions still unmatched, 15 better than the census (10,560 bytes), 16 the same, 1 worse; sum 5,854 to 5,567 rows. One score 0.
- Matched (4 functions, 1,516 bytes): `func_801419D0` (typed draft, score 0), `func_80132A34` and `func_80133760` (ports of GYOZI `func_80134938`, `func_80135518`), `func_8013AFEC` (typed draft plus the sibling `func_80137E54` idioms). No FAKE.
- Packages: T-17501..T-17507 (`w3-shougatu-p01`..`p07`), 33 functions, 18,404 bytes.
- Details: [[shougatu-family-results]], "Types-first pass (T-17500)".

### 2026-10-16 code review (claude-opus)

Review of `w2-good...w3-shougatu-types` against `CODING_STANDARDS.md` (section 13 checklist). Only SHOUGATU `.c` files, `include/ovl/SHOUGATU.h` and one line in `include/main_api.h` changed; `drafts/` has new files under `drafts/T-17500/` only, with no earlier archive touched; no inline asm, `.word`, game data or binaries; no `NON_MATCHING` block added (the two earlier ones, T-11310 and T-10910, are untouched); no new FAKE and no `//` comment. All four new matches are plain C. The 49 `FnTblNN` externs and 26 typedefs are declared once, and the stray early `#endif` in `SHOUGATU.h` now sits at the end of the file, so the whole header is guarded.

Findings, all fixed in commit `093c1307`:

1. `801330C0.c` defined `MAIN_API_OVERRIDE_func_800847B8` but declared no type of its own (section 8a); now `void func_800847B8();` as in the twin `80134120.c`.
2. `func_801419D0` had a redundant `(u8)` cast on `D_800E6280.unk_110A`, which is already `u8`; the unit still matches without it, so the cast is gone.
3. `func_8013AFEC`: the byte-view chain store is a documented idiom (T-9150, three earlier users) but had no comment; one added.

Checks after the fixes: `tools/docker.sh ninja` SHOUGATU sha1 OK, `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` pass. 4 functions matched (1,516 bytes). Result: no open findings.
