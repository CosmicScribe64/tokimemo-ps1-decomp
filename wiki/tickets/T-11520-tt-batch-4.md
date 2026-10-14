---
id: T-11520
title: TT overlay batch 4: Opus near misses, Sonnet drafts of the large functions
status: Done
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11510-tt-batch-3]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]"]
---

## Goal
Fourth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11510 (PR #5). In order:
1. Opus on the closest guarded near misses: `func_80140624` (~14 lines), `func_8013E850` (~14), `func_801392A0` (23),
   `func_8013DE10` (~29), `func_80134BBC` (phantom 0x40).
2. Sonnet over the 13 large untouched TT functions (1,180-2,200 bytes): draft each, match where possible, otherwise
   guard the close ones so a later Opus pass can close them.

## Acceptance criteria

- [x] Opus pass done; each near miss matched or its guard comment/row updated
- [x] Sonnet pass done; matches one per commit, near misses guarded with `T-11520`, T-0018 rows
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
TT: 203/303 -> 205/303 functions, 46360 -> 48828 bytes (41.6% -> 43.8%). Grand total 5423/6958, 1169276/2279368 (51.3%).

**Opus pass** (about 43 minutes, 296k tokens): no match; all five guarded drafts improved and recommitted:
`func_8013E850` 14 -> 2 lines, `func_8013DE10` 29 -> 7, `func_80140624` 14 -> 9, `func_801392A0` 23 -> 11,
`func_80134BBC` 47 -> 34 (one root cause, the unused hoisted 0x40).

**Sonnet pass** (about 45 minutes, 370k tokens): matched `func_80138300` (1,288 bytes) and `func_801430D4` (1,180).
Guarded `func_80139B98` (28 lines). Analysed and recorded but not kept: `func_80141450`, `func_80137C20`, `func_80143A44`,
`func_80144D70`, `func_8014A2B4`, `func_80146348`, `func_801400B0`, `func_801441BC`. Not drafted past m2c:
`func_80136C64`, `func_801455C8` (about 2.1 KB of init stores and nested collision loops each). The suggested
`func_80138E28` twin idiom did not help (73 lines, unchanged).

2 functions matched, 2,468 bytes. 14 rows in [[data/t0018-cases]]; idioms in [[matching-notes]] "TT overlay, batch 4
(T-11520)". `include/ovl/TT.h`: `D_80152C68`, `D_80152C78`, `D_80152C88`, `D_80156788`, all used.

## Review
Inline, against CODING_STANDARDS.md:
- Every match verified by a full `ninja` (27/27 `.bin: OK` after deleting every `.ok`); headers and globals OK;
  `sync_protos.py --check-branch` and `migrate_globals.py --check` clean.
- Fakematches carry `FAKE` with the ticket on their line: the `pad[2]` of `func_801430D4` and the
  `(&D_800E7D4E)[-1]` store in the guarded `func_8013E850` draft. No findings.
- Guards use `#ifdef NON_MATCHING` with the `INCLUDE_ASM` fallback and `T-11520`. No game data staged.

## Comments
