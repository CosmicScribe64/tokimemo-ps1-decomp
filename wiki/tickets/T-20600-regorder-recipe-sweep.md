---
id: T-20600
title: "Register-order family: recipe sweep"
status: Done
assignee: claude-opus (cg-regorder)
created: 2026-10-16
updated: 2026-10-17
links: ["[[speedup-research-2026-10-16]]", "[[data/t0018-cases]]", "[[register-cheatsheet]]", "[[matching-notes]]"]
---

## Goal

Re-apply the source recipes that later solved single register-order gaps across the whole `regorder` family of
[[data/t0018-cases]] (793 rows). Part of getting ahead of the compiler-limited tail estimated in
[[speedup-research-2026-10-16]].

Recipes to sweep, in order of fit (each from a matched function):
- integer-return view of a `void` callee whose call result lands in `$v0` (T-16210, T-20001); prefer the plain header
  type over an override (CODING_STANDARDS 8a);
- reading `GameState` or a global through a local pointer `GameState *g` (T-16920, T-16930);
- `u16` vs `s16` typing of stored constants so a constant joins the later compare's range (T-11840);
- zero-store chain pairs on one line (T-11850);
- a dead read of a selector before the first call (T-11690);
- a natural extra local declared first to fix an 8-byte frame (T-16930);
- data defined in the same unit for a shared `lui` (T-16551).

## Acceptance criteria

- [x] Target list: functions still `INCLUDE_ASM` or guarded with `regorder` rows, minus files owned by the wave 3
  conveyor, ranked by size and draft closeness (39 functions without TT; 17 guarded, 22 archived drafts).
- [x] Each target tried with the recipes; every match committed in plain C (no FAKE needed).
- [x] Update paragraph and three rows in [[data/t0018-cases]]; a T-20600 section in [[matching-notes]]; four
  [[register-cheatsheet]] rows.
- [x] Recipes for functions in busy files reported for the conveyor (Comments below).
- [x] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`.

## Notes

Worktree `tokimemo-wt/cg-regorder`, branch `cg-regorder`, base `w2-good`. At most 2 parallel compile jobs, no
permuter farm. Never edit a C file listed by the conveyor's busy-files script.

## Comments

2026-10-16 (claude-opus, cg-regorder): five functions matched in plain C, 3,400 bytes: DATE `func_80155004`
(368, chain assignment of the colour bytes), DATE `func_80152E34` (392, `setRGBn` comma lines and `GameState *g`),
RPG_BAT `func_80149C5C` (1,296, implicit-int definition and `RPG_BAT_OVERRIDE_func_8013F0F4`), RPG_BAT
`func_8013E54C` (624, implicit-int definition), TAIIKU `func_80145370` (720, `MAIN_API_OVERRIDE_func_80085E30`).
Header change: `include/ovl/RPG_BAT.h` wraps `func_8013F0F4` in `#ifndef RPG_BAT_OVERRIDE_func_8013F0F4`.
Later the same day: RPG_BAT `func_8013B5B0` (864, `MAIN_API_OVERRIDE_func_800452C4`, wrapped in `main_api.h`; a
`promo` row, found by a second sweep over the other guarded drafts in free files) and main `func_8006D6E0` (268,
`s32` definition; `include/main_api.h` declares it `s32`; its file was released by the conveyor during the session).
Seven functions, 4,532 bytes.
Details and negative results: [[matching-notes]], "T-20600". Ready for code review against `CODING_STANDARDS.md`.

For the conveyor (files it owns, not edited here):
- EN_NICHI `func_80132EE8` (`80132000.c`, T-18200 `hand.c`): read-only probe, the structure (including the
  preheader the T-17107 note compared with `func_8013E54C`) already matches; 44 register rows, 43 as `s32`. The gap
  is one range: `v & 0x7F` becomes a CSE range in `$v1` and pushes `v` to `$a0`, every later range one register up;
  the original keeps `v` in `$v1` and the mask in a ugen temporary. Try forms that do not repeat `v & 0x7F`.
- Any unmatched function with no prototype whose draft is `void` (callers see an implicit `int`): try `s32` with no
  return first. A read-only probe (`regdiff --src` on scratch copies) on such guarded drafts in owned files changed
  nothing for ENDING `func_80134B18`, EVENT `func_8010B614`, GEKO `func_801355E8`, GYOZI `func_80137188`,
  `func_8013E638` and main `csr_load_vram`, made ETC `func_80143F24` worse (10 to 13 register rows) and did not
  compile for EVENT `func_80118868`, so it is not enough on its own there.
- `$v0` never free after calls of `void` callees: `s32 f();` views per file (`regdiff --proto`), as for
  `func_80149C5C` and `func_80145370`. Candidates by symptom: GEKO `func_80140040`, GYOZI `func_80142A8C`,
  main `func_8006C988`, OPTION `func_80139E3C`, SHUGAKU `func_801357AC`.
- One loaded value stored to several fields, the original skipping a low register: one chain assignment
  (`func_80155004`). Candidates: EVENT `func_801022C0`, `func_80118868`.
- Colour words split into bytes: `setRGBn` comma lines with masks (`func_80152E34`); EVENT `func_800F9730`,
  GYOZI `func_80142A8C`, SHOUGATU `func_80136948`. Probed read-only on main `_sprite_set_box_shade_tarao`: worse
  (24 register rows), so its T-16220 shape stays.
- `GameState *g` (T-19016) still untried on DATE2 `func_80136794`.

2026-10-17 (claude-opus, code review against `CODING_STANDARDS.md`, diff `w2-good...cg-regorder`): no blocking
findings. Checked:
- Busy files: none of the 7 edited C files is in the conveyor's busy list (`src/main/8006CB30.c` was released
  during the session); headers `main_api.h` and `RPG_BAT.h` are edited only for the `#ifndef` wrappers and the
  `func_8006D6E0` type.
- Seven reported matches are plain C: no `asm`, `volatile`, `register` or pad added; no new `FAKE` (the existing
  `FAKE` comments in the same files belong to other functions and carry ticket and reason). The `idx` and `g` locals
  are ordinary names in place of the old FAKE pads.
- C89 (declarations first, `/* */` only, ASCII), fixed-width types; the overrides follow section 8a
  (`MAIN_API_OVERRIDE_func_800452C4`, `MAIN_API_OVERRIDE_func_80085E30` and `RPG_BAT_OVERRIDE_func_8013F0F4`, each
  with its reason before the includes and its own type after them).
- No build-pass change (no `tools/` edit), so no `wiki/toolchain.md` entry or pass test is needed. No game data,
  asm or binaries in the diff (src, include and wiki only).
- Rebuild in the worktree: `ninja` up to date, SHA1 OK; `check_headers` OK, `migrate_globals --check` OK,
  `sync_protos --check-branch` OK (0 new disagreements). The merge script rebuilds clean.
- One small fix: `func_80152E34` returns `s32` without a `return`; a comment now says why (its callers precede the
  definition). Comment only, `DATE.bin` SHA1 still OK.
Status Done.
