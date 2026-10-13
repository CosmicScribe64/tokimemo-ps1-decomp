---
id: T-11020
title: Big functions (Sonnet) 25
status: Done
assignee: sonnet-s25
created: 2026-10-13
updated: 2026-10-13
links: []
---

## Goal
Match big functions in DATE/80132000, EVENT/8010FE10, main 8007C030 and 8006CB30 (47 functions, list astra25-list.txt).

## Acceptance criteria
- [x] ninja 27/27 OK, headers and globals OK
- [x] inline review against CODING_STANDARDS.md

## Notes
Matched 11 functions (7,520 bytes): EVENT func_80111F04, func_80112460, func_80112930, func_8011220C, func_80113A40, func_80113FD4; DATE func_8013426C, func_801344C8, func_8013518C, func_80135C14, func_801349A0. Near misses guarded with NON_MATCHING: DATE func_8013AF1C, func_8013B208, func_8013B488, func_8013D5FC, func_80140374; main func_80072D70. Details and idioms: [[matching-notes]] section "T-11020". The TT overlay was not touched.

Inline review against CODING_STANDARDS.md: new code uses struct fields and the shared views (GameState, EventStateView, DateParticleView44 and friends); byte-offset arithmetic only where the matched twins use it (`D_800B0860 + idx * 0x34`, documented in the notes); overlay declarations appended to include/ovl/DATE.h (tables as `u8 *`, `D_800CA158` as `s16` with a MAIN_API_OVERRIDE and the guard in main_api.h); no tool, config or other overlay edited; sync_protos --check-branch OK, migrate_globals --check OK, ninja 27/27 OK with headers.ok and globals.ok. The `*(u8 *)&X` and `((View *)&X)->field` reads that stop uopt promoting a global are comments in the notes, not FAKE tricks in the sources (they are the reads the original needs).

## Comments
