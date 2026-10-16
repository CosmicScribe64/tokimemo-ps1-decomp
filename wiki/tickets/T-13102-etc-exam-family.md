---
id: T-13102
title: Reconstruct ETC exam handlers
status: Done
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

2026-10-15 review: repository code-review skill applied to aa2b3158...aa1c51ec. GPT-6.1 Sol medium reviewers: standards axis no findings; spec axis no actionable findings. Documentation observation about missing prototype result resolved here: sync_protos.py --check-branch passed, **0 new / 15 known disagreements**, branch check OK. Header/global guards pass; all eleven combined default literal proofs and ETC SHA1 pass. Existing-body preservation audit: 91/91 unchanged. No production edits after reviewed commit. Ticket complete within its bounded ETC scope; broad game-wide 80% goal remains ongoing.
