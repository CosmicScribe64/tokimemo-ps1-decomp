---
id: T-15400
title: Recover BUNKASAI menu label draft
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-14500-exhaustive-drafts]]"]
---

## Goal
Resolve the fresh func_801431D4 draft (440 coverage bytes, two diagnostic rows), checking string encoding and canonical GameState access before control-flow experiments.

## Acceptance criteria
- [x] Preserve original archived attempt and matching production C.
- [x] Explicit default linked proof, affected checksum and guards before promotion.

## Notes
Initial draft contains mixed Unicode and escaped Shift-JIS in the computer fortune-telling label. No prior ticket mentions this target in the current wiki.

## Results
Canonical GameState access alone retains two differing rows. The escaped draft string emits literal x83 where the original has Shift-JIS byte 0x83. Replacing it with ordinary Unicode パソコン占い yields a zero full-TU candidate. Default linked literal proof, BUNKASAI SHA1 and header/global/prototype guards pass (build/scratch/goal-bunkasai-label.log). Adds 440 coverage bytes; no artificial code or header changes, and prior matching C is unchanged. Both experimental contexts and diffs are retained in build/scratch/t15400. Generator notified to investigate related encoding failures.
