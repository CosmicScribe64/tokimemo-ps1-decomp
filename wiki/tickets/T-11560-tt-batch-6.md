---
id: T-11560
title: TT batch 6: bulk_m2c pipeline over the remaining TT functions, then Opus on the close drafts
status: Done
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-15
links: ["[[tickets/T-11550-tt-opus-near-miss-pass]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Stacked on T-11550 (PR #16). 1. Run `tools/bulk_m2c.py --unit TT` (T-11190/T-11230: m2c variants, cleanup, autotweak with
the order stage) over every remaining `INCLUDE_ASM` TT function. 2. One Opus agent on the closest drafts it leaves and the
remaining guarded TT near misses.

## Acceptance criteria

- [x] bulk_m2c run over TT; verified matches committed one per commit; useful near misses kept guarded
- [x] Opus pass on the closest drafts
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
TT: 211/303 -> 215/303 (48.0% -> 48.6%).
- bulk_m2c (`--unit TT --exclude "" --max-size 2400 --variants 3 --time 120 --jobs 4 --score-jobs 3`, output in the
  session scratch): 65 targets, 1 match (`func_8013E1E8`, 168 bytes, m2c leftovers tidied), 62 drafts ranked by score,
  2 failures (case labels outside the switch in `func_801333D4`, one m2c error). The default `--exclude` is TT.
- Opus on the ten closest drafts and `func_80142350` (about 70 minutes, 294k tokens): matched `func_80148154` (188),
  `func_80142C70` (208), `func_80144CF0` (128); guarded `func_80148764` (9 lines), `func_80135148` (4),
  `func_80149BB0` (5); not kept `func_801437F0`, `func_80144854`, `func_801356C8`, `func_801494D8`; `func_80142350`
  unchanged at 2 lines.
- 4 functions matched in total. Rows in [[data/t0018-cases]], idioms in [[matching-notes]] "TT overlay, batch 6".

## Review
Inline, against CODING_STANDARDS.md: full rebuild 27/27, funcdiff MATCH for all four, prototype and global checks
clean; the `volatile` pointer in the guarded `func_80148764` draft is marked FAKE with the ticket; the bulk match was
tidied into the project's style. No findings.

## Comments
