---
id: T-12400
title: Integrate verified matching branches on main
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: []
---

## Goal
Integrate completed matching branches and verified committed checkpoints directly onto remote main, as explicitly requested by the user. Preserve the original main checkout and active worker checkouts.

## Acceptance criteria
- [x] Resolve source, header, and wiki conflicts without losing verified work.
- [x] All 27 binary checksums, headers, globals, and branch prototypes pass on the combined tree.
- [x] Push the validated integration commit to remote main without force.

## Notes
PR #33 was separately authorized and merged after independent full-game and seven-function literal verification. Integration starts at 1668c560. User explicitly superseded the original no-merge/no-push rule and declined draft PRs.

## Comments

Combined verification: 27/27 binary SHA1 checks pass; header and global checks pass; branch prototype audit reports 0 new / 15 known. Matching-tool suite: 55 tests pass. Coverage: 5807/6958 functions, 1345524/2279368 bytes (59.0%), an increase of 67 functions and 52232 bytes over remote main 1668c560. Logs: build/scratch/integration-verify.log and integration-permute-tests.log. Header conflict resolutions preserve both independent declaration guards; stale duplicate cx7 wiki entries removed. Prepared for direct fast-forward push under explicit user authorization.

2026-10-17: Review done. The integration finished on 2026-10-15 (114 checkpoint commits; all 27 checksums, headers, globals and prototypes passed). The two header conflict resolutions (main_api.h, RPG_BAT.h) keep both sides, so there are no findings. All acceptance criteria are met. Moved to Done.
