---
id: T-11000
title: TT overlay: remaining functions
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-13
updated: 2026-10-13
links: ["[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]", "[[overlays]]"]
---

## Goal
Match the remaining TT functions, in queue order (`tools/queue.py --files TT`). Start: TT 175/303 functions,
30728/111376 bytes (27.6%); 115 unflagged in the queue. Claimed upstream in issue CosmicScribe64/tokimemo-ps1-decomp#2.

## Acceptance criteria

- [ ] Every attempted function has an m2c draft and a `funcdiff`/`regdiff` result
- [ ] Matches committed one per commit; near misses guarded under `#ifdef NON_MATCHING` with `T-11000`; T-0018 rows logged
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, `build/headers.ok`, `build/globals.ok`; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
