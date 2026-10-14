---
id: T-11510
title: TT overlay batch 3: Opus near misses, Sonnet larger functions
status: Done
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11500-tt-batch-2]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]"]
---

## Goal
Third TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11500 (PR #4). In order:
1. Opus on the guarded near misses from T-11500: `func_80144A04` (3 lines), `func_8013B6C8` (13), `func_80147644` (18),
   `func_80136244` (register naming only).
2. Sonnet on a shorter list of 14 untried TT functions (the 4 read-only ones from T-11500 and the 10 smallest untouched).

## Acceptance criteria

- [x] Opus pass done; each near miss matched or its guard comment/row updated
- [x] Sonnet list done; matches one per commit, near misses guarded with `T-11510`, T-0018 rows
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
TT: 199/303 -> 203/303 functions, 43364 -> 46360 bytes (38.9% -> 41.6%). Grand total 5414/6958, 1164572/2279368 (51.1%).

**Opus pass** (four T-11500 near misses; about 41 minutes, 310k tokens): matched `func_8013B6C8`, `func_80147644`,
`func_80136244`. `func_80144A04` is still 3 lines off; the cause is narrowed to a ugen temp, and the return type is ruled out.

**Sonnet list** (14 functions, all drafted or analysed; about 100 minutes, 500k tokens): matched `func_80139770`.
Guarded near misses: `func_80140624` (~14), `func_8013E850` (~14), `func_801392A0` (23), `func_8013DE10` (~29),
`func_80140920` (~42), `func_80135360` (~45), `func_80138E28` (~73). The last three are kept because frame, spills and
structure match. Not kept: `func_801333D4`, `func_801427A8`, `func_8013D988`. Analysed only: `func_8013F040`,
`func_80138808`, `func_801410A0`.

4 functions matched in total, 2,996 bytes. 14 rows in [[data/t0018-cases]]; idioms in [[matching-notes]] "TT overlay,
batch 3 (T-11510)". `include/ovl/TT.h`: new TT tables and externs and three `void` prototypes, all used.

## Review
Inline, against CODING_STANDARDS.md:
- Every match verified by a full `ninja` (27/27 `.bin: OK` after deleting every `.ok`); headers and globals OK;
  `sync_protos.py --check-branch` and `migrate_globals.py --check` clean.
- Finding: `func_80139770` declared `extern int func_80134470();` while the function is `void`. Now
  `extern void func_80134470();` with a `void (*fn)()` pointer; still matches. Fixed.
- Finding: four pad locals in guarded drafts had no `FAKE` comment (`pad[4]`, `pad1`, `pad2`, unused `t`). Fixed.
- The `func_80144A04` guard comment claimed the return type mattered; corrected.
- Guards use `#ifdef NON_MATCHING` with the `INCLUDE_ASM` fallback and `T-11510`. `TtTileMap` is file-local inside a guard.
  No game data staged.

## Comments
