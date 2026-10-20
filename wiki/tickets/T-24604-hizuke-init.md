---
id: T-24604
title: Match date display initialization
status: Done
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24600-finish-matching]]"]
---

## Goal

Complete MAIN `hizuke_init` with the recovered actor loop and unsigned month/day lifetimes.

## Acceptance criteria

- [x] Default C matches all 1,048 literal linked bytes.
- [x] Full Docker build, executable hashes and declaration guards pass.
- [x] Independent standards and behavior reviews have no unresolved findings.

## Notes

The final unsigned candidate matches all 262 instructions. Entry reads of month/day into the existing loop index before its initialization establish the documented unsigned-load conversion pass entry condition. Both reads are necessary: removing month gives cost174/256 words; removing day110/256. Joining the second year-digit assignment and its x store on one source line closes the last scheduling pair; splitting all eight month/day call/store pairs preserves the match. Both retained mechanisms have FAKE explanations. No compiler-pass or shared-header change was needed. Signed-byte experiments were diagnostic only.

## Comments

- 2026-10-20: Created from the T-24600 date-display lane before production promotion. Literal proof remains pending; no coverage credit yet.
- 2026-10-20: In Progress -> In Review. Minimized private and default production linked proofs pass for 1,048 bytes. Full ninja, fresh all-27 SHA1 hashes, globals/header checks and prototype guard pass (zero new disagreements,15 known). Independent spec and standards reviewers report no findings on the final minimized diff. They checked actor records16–23, year/month/day digits, unsigned conversions, hidden tens digits, weekday/event colors, decoration and the FAKE constraints.
- 2026-10-20: In Review -> Done after all acceptance gates passed. Census: 6,820/6,952 functions, 2,094,700/2,276,920 bytes (92.0%); 132 functions remain. Parent completion work continues.
