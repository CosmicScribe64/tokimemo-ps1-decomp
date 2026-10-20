---
id: T-24603
title: Match TACO fan color lifetimes
status: Done
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24600-finish-matching]]"]
---

## Goal

Complete TACO `func_80153B38` using the existing geometry reconstruction and correct color lifetimes.

## Acceptance criteria

- [x] Default C matches all 832 literal linked bytes.
- [x] Full build and all executable hashes pass in Docker.
- [x] Standards and behavior reviews have no unresolved findings.

## Notes

The TACO lane moved bright-byte extraction after geometry and before the polygon loop while retaining faded-byte subtraction inside the loop. This recovers all 208 target instructions. A combined empty color test recovers the remaining blue-component spill offset (0x5C rather than 0x54). The retained test requires a FAKE explanation; no shared header change is expected.

## Comments

- 2026-10-20: Private full-TU comparison has zero cost; literal linked proof and cleaned production verification are underway. Parent handles integration and independent reviews.
- 2026-10-20: Private and default production literal proofs pass (832 bytes). Root `ninja -j 4` and fresh all-27 SHA1 checks pass. Moved In Progress -> In Review. Standards and spec reviewers independently report no findings; integer-fallthrough annotation clarified before commit.
- 2026-10-20: All review and integration gates are satisfied; In Review -> Done. Verified census: 6,819/6,952 functions, 2,093,652/2,276,920 bytes (92.0%); 133 functions remain.
