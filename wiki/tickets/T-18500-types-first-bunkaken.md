---
id: T-18500
title: "Types-first: BUNKAKEN"
status: Done
assignee: claude (w3-bunkaken-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[tickets/T-12700-bunka-families]]", "[[bunka-family-results]]"]
---

## Goal

Types-first pass for the BUNKAKEN overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover BUNKAKEN's
shared data types into `include/ovl/BUNKAKEN.h`, redraft every unmatched BUNKAKEN function with them (drivers
adapted from T-16550, T-16560 and T-18100 into `drafts/T-18500/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit BUNKAKEN`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/BUNKAKEN.h`; every matched BUNKAKEN function still matches.
- [x] Every unmatched BUNKAKEN function redrafted and scored full-TU; drafts in `drafts/T-18500/BUNKAKEN/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-18500)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/BUNKAKEN/*.c`, `include/ovl/BUNKAKEN.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 12 unmatched BUNKAKEN functions (14,256 bytes by `tools/siblings.py`), 1 of them a guarded draft (`func_801428D0`).

## Comments

### 2026-10-16 claude (w3-bunkaken-types): pass done, In Review

- Types in `include/ovl/BUNKAKEN.h`: `s32 D_801562A0[3][13]` (39 scalars absorbed; `func_80148ED0` rewritten as
  elements, same bytes), the per-heroine tables `D_80155EA0`/`D_80155ED0`/`D_80155F00`/`D_80155F0C`/`D_80155F3C`,
  the stack-copied tables `BunkakenBytes11`, `BunkakenBytes9x6`, `BunkakenBytes11x6`, `BunkakenNames3x3`,
  prototypes of `func_80145500`/`func_80145550`/`func_80145A64`. `include/main_api.h`: `get_g_zyotai_h`
  wrapped in `#ifndef MAIN_API_OVERRIDE_get_g_zyotai_h` (no declaration changed).
- Redraft (`drafts/T-18500/`, 12 functions): 5 better than the census, 6 the same, 1 worse; median 210 to 191
  rows; one score 0 (`func_8014A5EC`).
- Matched (6 functions, 6,232 bytes, BUNKAKEN sha1 OK): `func_80147914`, `func_80151A78` (sibling ports),
  `func_8014A0E0` (K&R overrides in `80148ED0.c`), `func_8014A5EC` (GameState local), `func_80145550`
  (FAKE unused byte for the home of an uninitialized local), `func_80145A64`.
- FAKEs: `func_80145550` `u8 pad;` (named in the comment).
- Packages: T-18501 `w3-bunkaken-p01` (`80144F70.c`, `80143DD0.c`, `801428D0.c`, 3 functions, 5,368 B),
  T-18502 `w3-bunkaken-p02` (`80140F60.c`, `8013B510.c`, 3 functions, 3,236 B).
- Details: [[bunka-family-results]] ("Types-first pass (T-18500)"), [[matching-notes]].

### 2026-10-16 claude (review): code review against `CODING_STANDARDS.md`, no findings

- Scope: `git diff w2-good...w3-bunkaken-types`. Outside `drafts/T-18500/` (new folder only; no existing `drafts/`
  archive touched) and the wiki, only `include/main_api.h`, `include/ovl/BUNKAKEN.h` and four
  `src/ovl/BUNKAKEN/*.c` files changed (`80144F70.c`, `80146560.c`, `80148ED0.c`, `80150D30.c`).
- Checked: C89 (declarations at block top, `/* */` only), no inline asm, `.word` or bit hacks, all six reported
  matches are plain C, the one new FAKE (`func_80145550` `u8 pad;`) starts its comment with `FAKE` and names
  T-18500, the only `NON_MATCHING` block in the owned files (`801428D0.c`, T-10320) is unchanged and ticketed,
  header changes are additions or a `#ifndef MAIN_API_OVERRIDE_get_g_zyotai_h` wrap (no declaration changed),
  struct layouts carry offsets and sizes, the K&R overrides sit before the includes with their reason, no game
  data, asm or binaries in the diff, commits small and prefixed with the ticket id.
- Re-verified in the worktree after touching the BUNKAKEN sources: `tools/docker.sh ninja -j2` rebuilt the
  overlay, `build/ovl/BUNKAKEN.bin: OK`; `check_headers.py` headers OK; `migrate_globals.py --check` globals OK;
  `sync_protos.py --check-branch` 0 new disagreements.
- Observation, not a finding: `func_8014A5EC` is `s32` without a return statement because its first use
  (`func_80149144`) precedes the definition and no header prototype exists, so the implicit `int` view needs
  the same type; changing it is outside this ticket's matching scope.
