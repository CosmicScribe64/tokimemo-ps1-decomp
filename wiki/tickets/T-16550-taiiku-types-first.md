---
id: T-16550
title: "Types-first pilot: TAIIKU"
status: In Progress
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[speedup-research-2026-10-16]], [[data-types]], [[taiiku-family-results]], [[tickets/T-16200-matching-wave-2]]
---

## Goal

Recover the TAIIKU overlay's shared data types (structs, arrays, bit-field views, callback tables) in `include/ovl/TAIIKU.h` before function matching, then redraft every unmatched TAIIKU function with the new headers and compare with the census scores. Pilot of the types-first idea from [[speedup-research-2026-10-16]]: in BUNKAKEN/BUNKASAI fresh m2c drafts matched 67% of the time once shared types were in headers, against 1.6% elsewhere (T-12700).

## Acceptance criteria

- [ ] Survey: `tools/type_recovery.py --unit TAIIKU`, sibling clusters, census drafts; family page [[taiiku-family-results]].
- [ ] Recovered types declared in `include/ovl/TAIIKU.h`, globals given aggregate types; every matched function still matches (TAIIKU sha1 OK).
- [ ] Every unmatched TAIIKU function redrafted under `drafts/T-16550/TAIIKU/<func>/` (body.c, context.c, result.json), scored full translation unit, compared with the census.
- [ ] Score-0 drafts moved into `src/` and committed as matches.
- [ ] Four file-disjoint work packages T-16551..T-16554 (pt-taiiku-p1..p4) defined.
- [ ] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.

## Notes

Worktree `tokimemo-wt/pt-taiiku`, branch `pt-taiiku` from `w2-good`. Ownership: C files under `src/ovl/TAIIKU/` and `include/ovl/TAIIKU.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments
