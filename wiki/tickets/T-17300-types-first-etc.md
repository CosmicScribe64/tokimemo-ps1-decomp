---
id: T-17300
title: "Types-first: ETC"
status: Done
assignee: claude (w3-etc-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[etc-family-results]]"]
---

## Goal

Types-first pass for the ETC overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover ETC's shared data
types into `include/ovl/ETC.h`, redraft every unmatched ETC function with them (drivers adapted from T-16550 and
T-16560 into `drafts/T-17300/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit ETC`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/ETC.h`; every matched ETC function still matches.
- [x] Every unmatched ETC function redrafted and scored full-TU; drafts in `drafts/T-17300/ETC/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-17300)" section on [[etc-family-results]].
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/ETC/*.c`, `include/ovl/ETC.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 (claude, w3-etc-types)

Types in `include/ovl/ETC.h`: `EtcExamRank` (`D_80150668`, 0x8C), the ranking row positions `s16[14]`, per-girl
tables (`u8[13]`, `s32[13][2]`, `s32 *[13]`), the CD file table `s32 *[22]`, `SVECTOR D_8015013C`,
`D_8015014C[16]`, 14 scalar widths, `EtcWork40` moved in from `80146400.c`, `#include "psx_types.h"`. ETC sha1
OK after each step.

Redraft of all 40 still-unmatched functions with `drafts/T-17300/redraft.py` (T-17200 driver + work-prim,
display-base, work-view, Rec24 fold, lui pairing; three runs plain/typed/typed1): 36 of 38 unguarded drafts
compile, 11 better than the census (8,316 bytes), 20 the same, 5 worse; sum 6,856 to 6,532 rows; none 0. The
headers alone improved 7; the typed rewrites beat the plain run in 7 of 25 and lost in 15.

Matched 3 functions / 1,572 bytes, all sibling ports: `func_80144AC0` (main `func_8005C5BC`), `func_8013E524`
(TACO `func_80153E78`), `func_80145318` (NAME_ENT `func_80142028`). Near misses with `hand.c`:
`func_8013C700` (4 rows, delay-slot order), `func_8014354C`, `func_8013F980`. Packages T-17301..T-17308
(`w3-etc-p01`..`p08`, 29 functions, 22,400 bytes). Section "Types-first pass (T-17300)" on
[[etc-family-results]]; rows in [[data-types]] and [[matching-notes]].

Checks: clean build 27/27 OK, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK
(0 new, 15 known). Moved to In Review.

### 2026-10-16 (claude, review)

Code review of `w2-good...w3-etc-types` against `CODING_STANDARDS.md`: no findings, no fixes needed.
- C files changed: `src/ovl/ETC/8013BEF0.c`, `80143240.c`, `80146400.c` only (all owned); no other C, tools or config
  file. `drafts/` gained only the new folder `drafts/T-17300/` (no existing archive touched); no asm, `.word`,
  inline asm, disc data or binaries in it.
- The three matches (`func_80144AC0`, `func_8013E524`, `func_80145318`; 516 + 392 + 664 = 1,572 bytes) are plain
  C89 sibling ports with no FAKE, no `NON_MATCHING` and no new guard; `func_80145318` is s32 without a return like
  its NAME_ENT sibling `func_80142028`, and the comment says so. Existing guards (`80146400.c`, T-10850) unchanged.
- `include/ovl/ETC.h`: every new symbol is declared once, `D_8015014C[16]` and `SVECTOR D_8015013C` carry comments,
  `EtcWork40` moved from `80146400.c` into the header, `func_80145318` prototype is s32 and matches the definition.
- Package totals (29 functions, 22,400 bytes), the redraft split (11 better, 20 same, 5 worse of 36) and the
  matched bytes agree between ticket, family page, catalog and log. Wiki links resolve.
- Re-run on the branch: ETC rebuild after touching the sources and header, `build/ovl/ETC.bin: OK`;
  `check_headers` OK; `migrate_globals --check` OK; `sync_protos --check-branch` OK (0 new, 15 known).

Moved to Done.
