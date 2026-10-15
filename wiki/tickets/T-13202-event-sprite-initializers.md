---
id: T-13202
title: Reconstruct event sprite initializer family
status: In Progress
assignee: codex-events-13200
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-13200-event-family-transfer]]"]
---

## Goal
Test existing DisplayActor44 aggregate and chained structure assignment source forms on coherent GYOZI/SHOUGATU sprite initializers.

## Acceptance criteria
Preserve full TU context and existing C; require default literal proof and affected unit checksum for accepted changes. Bound failed probes and record scores.

## Notes
F494 m2c raw output has unresolved stack structs and copies. Reuse existing game.h DisplayActor44 rather than introducing another sprite layout.

## Comments

Both F494 and 41ACC reach full-TU0 with existing DisplayActor44 and natural chained copies. Two implicit temporary addresses initially differ by4 bytes; loop-local, register, loop-form, parameter and explicit-copy variants do not match. One declared unused s32 shifts both implicit temporaries into observed slots without changing frame or instructions; it is marked FAKE and tracked here. Held-out GYOZI sibling independently repeats4->0. Pending default literal and overlay checks.

Accepted F4941328 bytes and GYOZI41ACC1440 bytes: default linked function-byte proof and both full overlay SHA1 checks pass; header check passes. Evidence build/events13200/init-proof.txt and init-build.log. New overlay-only scalar declarations reflect observed byte/halfword widths. Total initializer gain2768 bytes; lane gain3424 bytes including selector match.
