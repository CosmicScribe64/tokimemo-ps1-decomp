---
id: T-12700
title: Match BUNKAKEN and BUNKASAI source families
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal
Increase literal verified game-code bytes through BUNKAKEN and BUNKASAI family reconstruction. Isolated branch from 95c3fc9f; no blind default sweeps.

## Acceptance criteria
- Literal linked-byte proof and affected overlay SHA1 checks for each accepted change.
- Review against CODING_STANDARDS before Done.

## Notes
Prior experiments read from integration wiki/matching-breakthroughs.md and wiki/broad-coverage-results.md. Prior 190-target sweep gained only 5 functions / 5036 bytes. Focus on flag, address and calling convention evidence.

## Comments
2026-10-15: Worktree created at /Users/lila/Documents/Projects/tokimemo-wt/bunka-12700. Child file assignments: T-12701 BUNKAKEN 80140F60.c; T-12702 BUNKASAI all files. Parent owns BUNKAKEN remaining files and integration. At most two compiler workers total.

Checkpoint review through bad8fc7b: both independent Standards and Spec axes report no actionable findings for twenty-one new functions / 22,404 bytes. Review used the recorded literal proofs and overlay checksum logs; reviewers did not independently rebuild. Fresh follow-up remains within owned BUNKAKEN/BUNKASAI families, with one compiler per lane.

## Final result

Done: 41 new functions / 40,252 literal verified bytes (35 / 33,716 BUNKAKEN; 6 / 6,536 BUNKASAI). Both independent review axes pass all source commits through 5f404ed5 with zero actionable findings. All 53 preexisting C bodies in touched files are unchanged. No compiler changes, generated-assembly edits, new FAKE workarounds, or shared main-header changes.

Final Docker validation: both overlay SHA1 hashes recalculated and match references; header/global checks pass; prototype check reports zero new disagreements and 15 known. Evidence: build/final-bunka-checks.log, build/final-bunka-sha1.log; per-function proof and rejected candidates in build/bunka12701, build/b12702 and build/b12703. Final progress on this branch: 5,848/6,958 functions, 1,385,776/2,279,368 bytes (60.8%). BUNKAKEN: 82.9% bytes; BUNKASAI: 88.9%. No full-game checksum build or publication performed. Overall 80% remains an integration goal, requiring 437,720 further aligned bytes from this branch state.

Branch codex/bunkaken-bunkasai-12700 at /Users/lila/Documents/Projects/tokimemo-wt/bunka-12700, based on 95c3fc9f. All source commits are ready for integration. Original checkout source remained read-only. See [[bunka-family-results]].
