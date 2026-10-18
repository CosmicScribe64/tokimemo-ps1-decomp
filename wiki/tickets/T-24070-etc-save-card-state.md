---
id: T-24070
title: Match ETC save-card state machine
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-20900-types-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80144F74 (608 bytes) in src/ovl/ETC/80143240.c. Preserve every other body, including archived T-24055 helpers. Read NAME_ENT func_8013F758 solely as a matched reference; do not edit contributor-owned NAME_ENT.

## Acceptance criteria
- Read current wiki, prior T-17300/T-20900 and target negatives before edits. Establish a fresh valid current-type full-TU baseline.
- Same sole Sol medium worker/compiler, existing built checkout on codex/complete-etc-card-state from f76548701, Docker only and ninja -j1; no additional agents or broad sweep.
- Confirm ownership: T-24055 is a frozen source-only In Review archive, no active TU worker found; current app inventory has only parent and this matching chat active. Parent assigns function-level scope.
- Use target assembly and actual matched sibling's structure/types, one concrete source change at a time. Coordinate exact interface/config scope before editing shared files.
- Actual C needs unguarded default literal proof, ETC checksum/interface guards and exact unchanged-neighbor audit; parent independently reviews/all27 before integration.

## Notes
Matched NAME_ENT func_8013F758 (556 bytes, sibling ratio0.770) shares save-card status cases1/3 and known GameState110D/1118/1120/1115 fields. T-20900 plain46 rows improves old115, but retained scalar aliases require current-type normalization. Original target uses v1 step-byte allocation and two capped byte-array updates in case0. Verify target-specific case conditions rather than assume full sibling identity. T-24055 closed source-only scope concerns other functions4354C/43758/43968 in this TU; preserve their production ASM and all other functions. TT/DATE2/NAME_ENT writes remain excluded.

## Comments
Selection and rationale sent to parent before edits. T-24069 published BFFC820-byte C and provenance remain preserved; T-24067/68 unresolved archives receive no probes. No target compile or source edit before this ticket setup.
