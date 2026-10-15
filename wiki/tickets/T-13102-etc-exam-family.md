---
id: T-13102
title: Reconstruct ETC exam handlers
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-13100-etc-families]]"]
---

## Goal
Test source-form transfer to remaining handlers in src/ovl/ETC/80146400.c, prioritizing medium state handlers and their shared work-memory layout.

## Acceptance criteria
- Full-TU exact scores and literal default linked bytes, ETC SHA1.
- Preserve existing C; no assembly edits or compiler changes.

## Comments
2026-10-15: Claimed before drafting; seven remaining functions / roughly 6.9KB; guarded 80146400 is a pre-existing control, not first priority.
