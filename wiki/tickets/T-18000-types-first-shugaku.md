---
id: T-18000
title: "Types-first: SHUGAKU"
status: Done
assignee: claude (w3-shugaku-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[shugaku-family-results]]"]
---

## Goal

Types-first pass for the SHUGAKU overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover SHUGAKU's shared
data types into `include/ovl/SHUGAKU.h`, redraft every unmatched SHUGAKU function with them (drivers adapted from
T-16550, T-16560 and T-17700 into `drafts/T-18000/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit SHUGAKU`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/SHUGAKU.h`; every matched SHUGAKU function still matches.
- [x] Every unmatched SHUGAKU function redrafted and scored full-TU; drafts in `drafts/T-18000/SHUGAKU/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-18000)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/SHUGAKU/*.c`, `include/ovl/SHUGAKU.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 25 unmatched SHUGAKU functions (9,908 bytes), 3 of them guarded drafts.

## Comments

### 2026-10-16 claude (w3-shugaku-types)

- Types (`include/ovl/SHUGAKU.h`): the eight local `FnTblNN` callback tables moved into the header as `ShFnTblNN`,
  plus `D_8013C008` (44) and `D_8013C228` (41); display-record copies as `DisplayActor44`; `D_8013C2E4` is `u8`.
  `include/main_api.h`: added `void D_80085368(void);` (address of `func_80085368`, the name the overlay links to).
  Location tables and type_recovery's 0xC-stride arrays left as scalars (reasons on [[shugaku-family-results]]).
- Redraft (`drafts/T-18000/`): 25 functions; over the 22 unguarded ones 9 better than the census (3,368 bytes),
  13 the same, none worse, median 74 to 68 rows. No draft scored 0.
- Matched 5 functions, 2,292 bytes: `func_8013282C`, `func_801399F4`, `func_80135630`, `func_80134B94`,
  `func_80139054` (guard replaced). FAKE comments in `func_80135630` (two GEKO idioms). Hand near misses:
  `func_80135480` 5 rows, `func_80133E10` 13 rows.
- Packages T-18001..T-18004 (20 functions, 7,660 bytes).
- Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`: see the log entry.

### 2026-10-16 claude (review, CODING_STANDARDS section 13 checklist)

Reviewed `w2-good...w3-shugaku-types` (src, include, config, wiki, drafts).

- Owned files only: the C changes are in `src/ovl/SHUGAKU/*.c`; `include/ovl/SHUGAKU.h` plus the one allowed
  `include/main_api.h` line. No existing `drafts/` file changed (459 added files, all under `drafts/T-18000/`).
- No inline asm, `.word` or `INCLUDE_ASM` edits beyond the five functions moved to C; all five reported matches are plain C.
  The two remaining `NON_MATCHING` blocks (`func_80132000.c` T-10510, `func_80134880.c` T-11310) predate the branch and
  carry a ticket and the lines left; the `func_80139054` guard (T-10850) was removed because the function now matches.
- Fakematches: `func_80135630` has two `FAKE` comments with T-9770 and T-18000; `func_80134F18` keeps its T-9310 `FAKE`.
- Headers: `ShFnTblNN` and the new `D_8013C008`/`D_8013C228` declared once in `SHUGAKU.h` (the `.c` copies were removed);
  `D_80085368` follows the `D_800896E0` pattern; `check_headers` OK. No game data, asm or binaries in the diff.
- Findings (2, minor, fixed in 9e48504a): `func_80134B94` (u8 index reread after the call) and `func_80139054`
  (reload into the switch variable) used non-obvious shapes without a comment (section 9). Comments added; no code change.
- Rebuilt with `tools/docker.sh ninja -j2` after the fix: `build/ovl/SHUGAKU.bin: OK` (sha1), headers OK.
- Not part of this review: the package tickets T-18001..T-18004 named on the family page are not in this branch.
