---
id: T-13102
title: Reconstruct ETC exam handlers
status: In Review
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

2026-10-15 tests: 80146714 word-view/void declaration gives exact full-TU zero. 80148BC8 real arrays and CharFlags give 40 rows, with no improvement from operand swap/direct array args. 80148F7C SDK SPRT view gives 336 rows (338 with GameState pointer). All rejected drafts remain scratch. Other rendering drafts need large local buffers and further types; no frame-padding guesses promoted.

2026-10-15: Accepted func_80146714: 176 literal linked bytes, default proof and ETC SHA1 pass. Remaining calculation/ranking/drawing probes rejected (310/192, 40, 336/338 rows). No experimental declarations retained. See [[etc-family-results]].
