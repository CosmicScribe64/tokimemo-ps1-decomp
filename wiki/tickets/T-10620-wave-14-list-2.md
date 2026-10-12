---
id: T-10620
title: Wave 14: list 2
status: Done
assignee: wave-14 batch agent 2
created: 2026-10-12
updated: 2026-10-12
links: ["[[matching-notes]]", "[[data/t0018-cases]]", "[[gcc-objects]]"]
---

## Goal
Attempt every function on wave-14 list 2 (56 functions in 33 files, about 23.5 KB), in the order of the queue list.

## Acceptance criteria

- [x] Every function has an m2c draft, a build and a `funcdiff` (drafts for all 56 are in the T-10620 scratch directory).
- [x] Matches committed; near misses guarded under `#ifdef NON_MATCHING` with the ticket id; T-0018 rows logged.
- [x] Inline review against `CODING_STANDARDS.md`.
- [x] `ninja` ends with every unit `OK`, `build/headers.ok`, `build/globals.ok`; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean.

## Results
Matched, 8 functions, 1,784 bytes: OMIMAI `func_80132AD0` (628), main `func_8004B590` (488), EVENT `func_80119C9C` (144) and `func_8011B5A8` (100), GYOZI `func_8013FD9C` (144), KANGEI `func_801323A8` (136), GEKO `func_8013F800` (80), TACO `func_80143AF4` (64).
Guarded near misses added: TACO `func_8015C0A0` (6 lines), `func_8015CCC8` (2 lines). Earlier guarded drafts re-tested with the permuter or regdiff and left as they were: `junban_init`, TT `func_8013D2E0`, TAIIKU `func_8013AE14`, `func_801368B8`, DATE `func_8013E6A4`, OPTION `func_80139F8C`.
Not matched, with drafts in the scratch directory and 26 rows in [[data/t0018-cases]]: see the section "Wave 14, list 2" in [[matching-notes]] for the patterns (loop shape of the RPG_BAT clear loops, constants IDO folds, data it will not hoist, ugen temporaries of the TT search loops). Skipped as not compiler output: DATE `func_8015A640`, `func_8015A980`, TACO `func_8015D0D0` (hand-written rotation helpers, [[gcc-objects]]) and TT `func_8014D260` (gcc, open in [[gcc-objects]]).

## Review (inline, against CODING_STANDARDS.md)
- Matches verified by `funcdiff --resolve` and by `ninja` (all unit sha1 `OK`); no unverified claim. One `MATCH` from `funcdiff` for TT `func_80149B20` under load did not survive a rebuild and was reverted (noted in [[matching-notes]]).
- The two guards use `#ifdef NON_MATCHING` with the INCLUDE_ASM fallback and `T-10620` in the comment; the default build matches.
- C89 only; K&R definitions where the callee had narrow home slots; FAKE comment on the dead first chain in EVENT `func_8011B5A8` (copied from DATE `func_8013FA6C`).
- Declarations: the new main-exe globals (`D_800EC1A4`..`D_800EC1CA`, six bytes used by EVENT `func_8011B5A8`) went through `sync_protos.py --add-global`/`--fix` into `include/main_api.h`; `func_8008D610` uses a `MAIN_API_OVERRIDE_` view with its reason in GYOZI `8013EB10.c`; overlay-only symbols stayed in overlay headers.
- File-local bit-field types in `src/main/8004AD60.c` carry a comment; no SDK type is redefined.
- No game data staged; `git status` clean of `asm/`, `build/`, `expected/`.

## Comments
