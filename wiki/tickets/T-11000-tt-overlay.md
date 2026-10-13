---
id: T-11000
title: TT overlay: remaining functions
status: Done
assignee: claude (diman8 fork)
created: 2026-10-13
updated: 2026-10-13
links: ["[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]", "[[overlays]]"]
---

## Goal
Match the remaining TT functions, in queue order (`tools/queue.py --files TT`). Start: TT 175/303 functions,
30728/111376 bytes (27.6%); 115 unflagged in the queue. Claimed upstream in issue CosmicScribe64/tokimemo-ps1-decomp#2.

## Acceptance criteria

- [x] Every attempted function has an m2c draft and a `funcdiff`/`regdiff` result
- [x] Matches committed one per commit; near misses guarded under `#ifdef NON_MATCHING` with `T-11000`; T-0018 rows logged
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, `build/headers.ok`, `build/globals.ok`; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
Two Sonnet lists over disjoint TT files (worktrees, `tools/queue.py --files TT --plan 70 --agents 2 --exclusive file`), 70 functions attempted or triaged.
TT: 175/303 -> 187/303 functions, 30728 -> 36204 bytes (27.6% -> 32.5%). Grand total 5352/6958, 1105788/2279368 (48.5%).

Matched, 12 functions, 5,476 bytes: `func_80148244` (540), `func_8013B928` (424), `func_80137878` (512), `func_8014CDBC` (560),
`func_80141A1C` (592), `func_8014C2F8` (436), `func_80133E30` (384), `func_80133FB0` (456), `func_801342CC` (420), `func_80134630` (312),
`func_80134178` (340), `func_801365C4` (508). `func_80133C1C` became an ANSI definition (same bytes); `func_8014D260` returns `s32`.

Guarded near misses (13): `func_80136AD8`, `func_801374C0`, `func_801330D8`, `func_80147150`, `func_8013B4A0`, `func_80134470` (1 line),
`func_801349B0` (7), `func_80134BBC` (7), `func_8013C1FC`, `func_80142350` (11), `func_8013C8DC`, `func_8013CC24`, `func_80142570`.
16 rows in [[data/t0018-cases]]; idioms in [[matching-notes]] "TT overlay (T-11000)". The other list entries were triaged from existing
notes (GPU packet writers, gcc atan2 `func_8014D260`, documented T-0018 cases) or drafted and restored to `INCLUDE_ASM`.

## Review
Inline, against CODING_STANDARDS.md:
- Every match verified by a full `ninja` (27/27 `.bin: OK`, re-checked after deleting every `.ok`), headers and globals OK;
  `sync_protos.py --check-branch` (0 new, 15 known) and `migrate_globals.py --check` clean.
- Guards use `#ifdef NON_MATCHING` with the `INCLUDE_ASM` fallback and `T-11000` in the comment.
- Fakematches (pad locals placing stack slots) are marked `FAKE` with the ticket. Finding: one pad in the guarded
  `func_8013CC24` draft lacked the ticket id; fixed.
- C89; new TT types and externs only in `include/ovl/TT.h`, `include/main_api.h` untouched; every new extern has a user.
- No game data staged.

## Comments
