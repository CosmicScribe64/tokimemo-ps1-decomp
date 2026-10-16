---
id: T-18300
title: "Types-first: ENDING"
status: Done
assignee: claude (w3-ending-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[tickets/T-14000-ending-en-nichi-families]]", "[[ending-family-results]]"]
---

## Goal

Types-first pass for the ENDING overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover ENDING's shared
data types into `include/ovl/ENDING.h`, redraft every unmatched ENDING function with them (drivers adapted from
T-16550, T-16560 and T-18000 into `drafts/T-18300/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit ENDING`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/ENDING.h`; every matched ENDING function still matches.
- [x] Every unmatched ENDING function redrafted and scored full-TU; drafts in `drafts/T-18300/ENDING/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-18300)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/ENDING/*.c`, `include/ovl/ENDING.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 14 unmatched ENDING functions (16,188 bytes by `tools/siblings.py`), 1 of them a guarded draft (`func_80134B18`).

## Comments

### 2026-10-16 claude (w3-ending-types)

- Matched `func_80132F18` (280 bytes) as a port of main `check_k_scroll` (`tools/siblings.py` 0.978), with a
  FAKE unused `get_k_work` result that keeps the frame.
- Types (`include/ovl/ENDING.h`): `EdShorts18 D_8013C3B0`, `EdShorts14 D_8013CA60`/`D_8013CA7C` (stack-copied
  tables), `s32 **D_8013CAF0`, `MAIN_API_OVERRIDE_dialogue_render` (s32 result). Every ENDING file now includes
  `game.h` after `ovl/ENDING.h`. Location records and type_recovery's 0xC-stride arrays left as scalars
  (reasons on [[ending-family-results]]). `include/game.h` and `include/main_api.h` unchanged.
- Redraft (`drafts/T-18300/`): 13 functions; over the 12 unguarded ones 7 better than the census (7,268 bytes,
  4 of them from hand drafts of the scene scripts), 5 the same, none worse, median 161.5 to 142.5 rows. No
  draft scored 0.
- Packages: T-18301..T-18305 (`w3-ending-p01`..`p05`, 13 functions, 16,240 bytes in the package lists).
- Checks: clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`.

### 2026-10-16 claude (review, w3-ending-types)

Code review against `CODING_STANDARDS.md` (checklist in section 13), diff `w2-good...w3-ending-types`.

- Scope: C and header changes only in `src/ovl/ENDING/*.c` and `include/ovl/ENDING.h`; `include/game.h`,
  `include/main_api.h` and `config/` unchanged; `drafts/` has new files under `drafts/T-18300/` only (no existing
  archive touched, no `.o`, asm, binaries or game data); no inline asm or `.word` in `src/`.
- Match: `func_80132F18` is plain C (no `INCLUDE_ASM`, no `NON_MATCHING`); ENDING sha1 OK. No new `NON_MATCHING`
  block; the one existing guard (`func_80134B18`, T-10520) is unchanged and the default build matches.
- Headers: `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` pass. The
  `MAIN_API_OVERRIDE_dialogue_render` define sits before the includes with its reason, as for `set_kanji_string`.
- Finding (fixed): the FAKE note of `func_80132F18` was the second line of a comment that began "Port of main
  check_k_scroll", so it did not start with `FAKE` (section 7). Split into the port note and a separate
  `/* FAKE: ... T-18300 */` comment. Comment-only; rebuilt with `tools/docker.sh ninja`, ENDING sha1 OK.
- No other findings. Result: 1 function matched (280 bytes), 13 redrafted, packages T-18301..T-18305 handed on.
