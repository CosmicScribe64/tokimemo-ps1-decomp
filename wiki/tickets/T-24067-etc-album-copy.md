---
id: T-24067
title: Match ETC album record copy
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-24064-etc-count-trio]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80141ABC (300 instruction bytes) in src/ovl/ETC/80140F50.c. This single function is split from the deferred T-24064 trio; both other ETC targets remain deferred. Preserve DATE's verified checkpoint 2da9bb5f8.

## Acceptance criteria
- Read existing album-copy notes and archived negative results before experiments.
- Reuse built checkout on codex/complete-etc-album-copy from 2da9bb5f8, one existing GPT-6.1 Sol medium worker/compiler, Docker only and serial ninja -j1.
- Focus only this function until match or actual external blocker; no broad batches or extra agents. Reserved OPTION/BUNKA_SD targets remain excluded.
- Coordinate exact shared-header/config changes before edits. Actual C requires unconditional default body, explicit linked literal funcdiff, ETC checksum, interface guards and audit of other bodies.
- Parent owns independent review, integration and push; wait for its DATE integration result before changing the baseline if needed.

## Notes
Prior source recovery identifies an eight-byte GsRec75E record copied into a save-side album cleared at root+0x58 for 0x800 bytes. Byte-only destination fields reproduce unaligned word operations. Correct eight-byte cursor stride and word counter reached35 then27 differing lines; saved-register order, copy temporary and stack arguments remain after five-minute search, pointer-order and end-pointer controls. Seek evidence-backed source/layout changes rather than repeat these sweeps.

## Comments
Parent assigned this exclusive target after receiving verified DATE pin; root integration remains in progress. Other deferred ETC work stays Ready in T-24064, not started by this ticket.
