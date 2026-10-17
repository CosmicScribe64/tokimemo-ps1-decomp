---
id: T-11870
title: TT batch 13: the last three guarded near misses, one long Opus run each
status: Done
assignee: claude (diman8 fork)
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-11860-tt-batch-12]]", "[[decompile-workflow]]", "[[data/t0018-cases]]", "[[uopt-register-allocation]]"]
---

## Goal
Thirteenth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11860 (PR #47): the three guarded TT
near misses, one Opus agent each with a long budget (about 120 minutes), two at a time in separate worktrees, with the
batch 11-12 findings as tips. `func_8014D260` (gcc) stays parked: it needs maspsx changes ([[gcc-objects]]).
1. `func_80142110` (`80142110.c`, 4 lines: an else-arm load lands in c instead of a ugen temp)
2. `func_801427A8` (`80142570.c`, 12 lines: three tied hoisted constants coloured in bit order)
3. `func_8014A2B4` (`80148DB0.c`, 28 lines: four constants in t2..t5 behave like uopt registers)

## Acceptance criteria

- [x] All three run; matches committed one per commit, near misses stay guarded with updated notes (`T-11870`)
- [x] Inline review against `CODING_STANDARDS.md` (FAKE comment on every fake trick, including every chain store)
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
All three matched (2,740 bytes): `func_80142110` (one FAKE mask), `func_801427A8` (no FAKE; its old FAKEs removed),
`func_8014A2B4` (statement order of case 7). TT 302/303 functions, 99.7% of bytes; grand total
`6411/6958 1771564/2279368 77.7%`. Only `func_8014D260` (gcc, [[gcc-objects]]) is left in TT. Notes in
[[matching-notes]] ("TT overlay, batch 13"), rows in [[data/t0018-cases]].

## Review
Inline review against `CODING_STANDARDS.md`: one new fake trick (the redundant mask in `func_80142110`) carries a
`FAKE ... T-11870` comment; `func_801427A8`'s draft-era FAKEs are gone; no header changes; the `NON_MATCHING` blocks
of all three are removed. Clean rebuild 27/27 OK, headers/globals OK, `sync_protos.py --check-branch` and
`migrate_globals.py --check` clean. No open findings.
