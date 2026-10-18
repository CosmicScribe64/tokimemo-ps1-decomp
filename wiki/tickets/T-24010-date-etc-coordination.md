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
