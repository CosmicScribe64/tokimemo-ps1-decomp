---
id: T-11560
title: TT batch 6: bulk_m2c pipeline over the remaining TT functions, then Opus on the close drafts
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11550-tt-opus-near-miss-pass]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Stacked on T-11550 (PR #16). 1. Run `tools/bulk_m2c.py --unit TT` (T-11190/T-11230: m2c variants, cleanup, autotweak with
the order stage) over every remaining `INCLUDE_ASM` TT function. 2. One Opus agent on the closest drafts it leaves and the
remaining guarded TT near misses.

## Acceptance criteria

- [ ] bulk_m2c run over TT; verified matches committed one per commit; useful near misses kept guarded
- [ ] Opus pass on the closest drafts
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
