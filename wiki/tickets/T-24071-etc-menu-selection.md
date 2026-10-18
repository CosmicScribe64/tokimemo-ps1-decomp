---
id: T-24071
title: Match ETC menu selection handler
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80144DF0 (388 bytes) in src/ovl/ETC/80143240.c. Preserve all helpers, matched bodies and unresolved func_80144F74 ASM. MAIN func_8005D448 is a read-only matched reference.

## Acceptance criteria
- Read current wiki and target history; establish fresh valid current-type full-TU baseline before new source controls. Earlier plain67/typed130 are historical, not new progress.
- Same sole Sol medium worker/compiler, existing built checkout from2480c7150 on codex/complete-etc-menu-selection, Docker only/ninja -j1. No extra agents or blind sweeps.
- Parent explicitly releases old OPTION/T-24055 TU ownership to this single-target lane. T-24070 archive published108379749; exclude it and all contributor scopes from edits.
- Reconstruct target-specific fields/calls/selection from original assembly and matched MAIN8005D448, one source-led change at a time. Coordinate exact header/config scope before shared edits.
- Actual C requires unguarded default literal proof, ETC hash/interface guards and exact unchanged-neighbor audit; parent independently reviews/all27 before integration.

## Notes
Matched sibling8005D448 (292 bytes, ratio0.753) has menu selection spilled across k_disp_inc, real GameState1094/F88/1118 and slot_status family. T-17300 typed draft incorrectly folds selection equality/store to unk_000, uses invalid array-address arithmetic and a phantom argument; correct semantics before measuring. Target differs with extra menu call/order, signed selection, sp28 reload and unsigned ready byte. Read known MAIN5CA94 per-arm narrowing51-row negatives; do not assume its FAKE two-word reserve transfers. No later target-specific exhaustion found in current matching notes/family/ticket search; faithful baseline remains necessary.

## Comments
Read-only selection/rationale sent before assignment. Parent approved same-worker source-led work conditional on checking prior evidence, and confirms TU ownership released. Ticket unique/current main108379749 checked before edits. Prior unresolved packets and published C wins preserved.
