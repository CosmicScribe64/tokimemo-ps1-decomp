---
id: T-24069
title: Match ETC menu state machine
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8013BFFC (820 instruction bytes) in src/ovl/ETC/8013BEF0.c. Preserve matched same-file func_8013C330 and every other body. T-24067/24068 remain unresolved archives with no further probes.

## Acceptance criteria
- Current primary015c3496f retains target INCLUDE_ASM and actual matched same-file sibling func_8013C330 (976 bytes, sibling ratio0.766). Read current wiki and prior target negatives first; confirm ownership remains unclaimed.
- Reuse clean built worktree from af25ebdc7 on codex/complete-etc-menu-state, the same single Sol medium worker/compiler, Docker only and ninja -j1. No extra agents or broad sweeps.
- Reconstruct target-specific cases from original assembly using matched sibling structure and verified GameState fields; do not copy incorrect old field folds.
- Coordinate exact shared-header/config scope before changes. Actual C requires default unguarded literal proof, ETC checksum/interface guards and unchanged-body audit including sibling.
- Parent owns independent review/integration; remain In Review until reviewed.

## Notes
T-17300 typed draft had12 repair failures and no score; apparent case1/2 GameState110D updates were misfolded as unk_000. T-9520 records a redeclaration failure. Matched sibling supplies concrete menu-step/load/fade-tail structure, sequential load-state if statements and an existing s32 fallthrough-return view. Verify target semantics and return usage rather than assuming every sibling detail transfers. No live ownership found for this TU; old T-24055 scope is a different TU, and contributor NAME_ENT/TT/DATE2 scopes remain excluded.

## Comments
Target selected read-only under parent's next-unclaimed-ETC direction, with target/rationale sent before edits. Checkpoint af25ebdc7 and prior unresolved packets preserved. No target edit or compile before ticket setup.
