---
id: T-24074
title: Recover ETC record scene with proven helper calls
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-24073-etc-record-init]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80144224 (796 bytes), src/ovl/ETC/80143240.c. Preserve verified4407C/441DC and all other functions. Parent exclusively reserves this target to the same single Sol medium worker.

## Acceptance criteria
- Read current wiki/prior negatives and original assembly; establish fresh valid full-TU baseline with current aggregates and actual helper arguments before ordinary target-only C controls.
- Correct old missing4407C argument: original44344 passes frame_counter/701, needed for proven arg14 positioning. Replace phantom4-arg441DC call with actual matched2-arg view; use real D5014C16-word order table and Rec24 layout. Verify every correction against assembly/current definitions.
- Same sole Docker compiler/ninja -j1, existing built tree froma84f8462e on codex/complete-etc-record-scene; no extra agents or instrumentation/broad sweeps.
- No helper/header/config changes without exact evidence and coordination. Actual C needs default unguarded literal796-byte proof, ETC hash/interfaces and exact neighbor audit; parent independently reviews/all27.

## Notes
New verified4407C exposes a semantic omission in the retained T-17300 draft, justifying reconstruction beyond old scores. No opcode sibling above0.7 or match promise. Parent published final T-24073 archive7837b7e4e, measured6515 functions/81.9%; preserve source and prior unresolved archives. Ticket uniqueness/current ASM checked before edits.
