---
id: T-11500
title: TT overlay batch 2: Opus near misses, Sonnet untried list
status: Done
assignee: claude (diman8 fork)
created: 2026-10-13
updated: 2026-10-14
links: ["[[tickets/T-11000-tt-overlay]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]"]
---

## Goal
Second TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2; the maintainer confirmed TT is ours). Two parts, in order:
1. One Opus agent on the closest guarded near misses from T-11000: `func_80134470` (1 line), `func_801349B0` (7),
   `func_80134BBC` (7), `func_80147150` (~8), `func_80142350` (11). Tests whether Opus pays off for near misses.
2. One Sonnet list over the unflagged TT functions not on the T-11000 lists.

## Acceptance criteria

- [x] Opus pass done; each near miss matched or its guard comment/row updated with what was tried
- [x] Sonnet list done; matches one per commit, near misses guarded with `T-11500`, T-0018 rows
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
TT: 187/303 -> 199/303 functions, 36204 -> 43364 bytes (32.5% -> 38.9%). Grand total 5398/6958, 1149612/2279368 (50.4%).

**Opus pass** (five closest T-11000 near misses; about 32 minutes, 240k tokens): matched `func_80134470`, `func_80147150`,
`func_801349B0`; `func_80134BBC` closer but still guarded; `func_80142350` unchanged (cause traced, no source form found).

**Sonnet list** (45 untried unflagged functions, smallest first; 17 attempted, about 70 minutes, 500k tokens): matched
`func_80141C6C`, `func_80141EBC`, `func_8013B1C4`, `func_80142DEC`, `func_801336F8`, `func_80135FA8`, `func_8013CDC0`,
`func_8014B7A8`, `func_8013FBD0`. Guarded near misses: `func_8013B6C8` (13 lines), `func_80147644` (18), `func_80144A04` (3),
`func_80136244` (register naming). Not kept: `func_80147A18`, `func_8014C84C`, `func_80134E20`, `func_80135B5C`, `func_8013AE3C`;
read only: `func_801333D4`, `func_80140624`, `func_8013F040`, `func_801427A8`. The other 28 list entries were not reached.

12 functions matched in total, 7,160 bytes. 14 rows in [[data/t0018-cases]]; idioms in [[matching-notes]] "TT overlay, batch 2
(T-11500)". Header changes: `TtSlot14` and TT externs in `include/ovl/TT.h`; `s32 func_80087E6C(s32, s32, s32)` in
`include/main_api.h`.

## Review
Inline, against CODING_STANDARDS.md:
- Every match verified by a full `ninja` (27/27 `.bin: OK`, re-checked after deleting every `.ok`); headers and globals OK;
  `sync_protos.py --check-branch` (0 new, 15 known) and `migrate_globals.py --check` clean.
- Guards use `#ifdef NON_MATCHING` with the `INCLUDE_ASM` fallback and `T-11500` in the comment.
- Finding: one pad local in the guarded `func_80136244` draft had no `FAKE` comment; fixed. Other pads are marked `FAKE`
  with the ticket; `pad_NN` struct members are unknown fields, not fakematches.
- C89; the new main-exe declaration is in `include/main_api.h` only (header check passes).
- No game data staged.

## Comments
