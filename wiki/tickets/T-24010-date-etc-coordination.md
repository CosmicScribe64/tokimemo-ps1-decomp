---
id: T-24010
title: Coordinate isolated DATE and ETC matching
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24000-complete-matching]]", "[[date-family-results]]", "[[etc-family-results]]"]
---

## Goal
Recover verified C in the two assigned families from c485bb71a. Parent integrates and pushes; this lane never edits main or integration.

## Acceptance criteria
- DATE T-24011 owns src/ovl/DATE/80141EE0.c and 8015A640.c; targets func_80144E20, func_801449D4, func_8015A640.
- ETC T-24012 owns src/ovl/ETC/80132880.c; targets func_80136448, func_80132C3C, func_80132FB0, func_801328A4.
- Docker only; setup and ninja -j1 per worker. Bound fruitless target searches near 25 minutes and record concrete blockers.
- Verified wins require default linked funcdiff --resolve --version default, affected checksums and header/global/prototype guards, then standards/spec review.
- Preserve source-only drafts and provenance. No copyrighted material, inline asm, toolchain exceptions, contributor messaging or scope expansion.

## Comments
Baseline 1,813,468 / 2,279,368 game bytes (79.6%). Two focused GPT-6.1 Sol medium workers authorized; Astra reserved for evidenced blockers. Shared headers require coordination.

Initial source query: [[gcc-objects]] settles DATE func_8015A640 as hand-written libgte assembly (register order, multu, hazard nops and local jump; multiple gcc versions previously failed). Older [[matching-notes]] suggestions of a compiler swap are superseded by that measured result. Worker skips repeat compiler sweeps and focuses on T-17207 dispatcher evidence. This remains assembly and does not count as a new C match; no denominator change is proposed.

Worker branches: codex/complete-date at /private/tmp/tokimemo-complete-date and codex/complete-etc at /private/tmp/tokimemo-complete-etc. Common ticket ancestor 4c5123f11. Parent chat notified of scope, setup and established libgte blocker.

Resource audit: tools/funcdiff.py invokes ninja without a job cap in freshness/header checks. Workers instructed to prebuild affected .ok with ninja -j1 and then use --no-build --resolve --version default (freshness and header checks retained), or an untracked PATH wrapper that appends -j1. No tracked toolchain or per-function exception is introduced.

ETC checkpoint: a0675cafe604146244c5d762cd915467b41a46a2 on codex/complete-etc verifies func_80136448, 1,280 literal linked bytes. Docker default funcdiff MATCH, ETC checksum OK, headers/globals OK, prototypes 0 new / 15 known; proofs under /private/tmp/tokimemo-complete-etc/build/etc24012/. The s32 no-return epilogue view is conservatively marked FAKE. Two-axis pinned review started against 4c5123f11; no integration or push performed. Remaining ETC targets retain assembly and ignored diagnostic-only drafts. DATE near misses remain unverified.

Pinned ETC checkpoint review: independent Standards PASS (0 actionable findings) and Spec PASS (0 actionable findings). Findings confirm the FAKE is explained and ticketed, own-overlay prototypes match existing void definitions, literal proof and checksum pass, and remaining work is explicitly pending. Parent sent exact commit and proof for integration; ETC worker resumed to document review and continue remaining targets. T-24012 stays open for its remaining three functions.
