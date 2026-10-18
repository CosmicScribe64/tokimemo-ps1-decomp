---
id: T-24071
title: Match ETC menu selection handler
status: In Review
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

Fresh valid current-type baseline61 rows, distinct from historical plain67/typed130. Correct selection snapshot indexes slot_status after k_disp_inc; old plain draft indexed current global+1 after that call. Explicit sp28 copy/reload reflects original cross-call value, without adding guessed storage. Frame30/spill2C differs from original38/spill28. One GameState pointer is65 (spill28 but initial selectionv0), rejected. Existing-byte selected-slot comparison capture remains61 and flips branch operand, rejected. No per-arm narrowing/width sweep, FAKE buffer, return assumption, production or interface change.

Button snapshot reduces61 to57 fixes selection spill28, while frame remains30 versus38. Stored selected-field argument readback is neutral57. Named required status result first reaches frame38 but shifts spill2C; declaring it after existing locals restores frame30/spill28, also57. Both rejected; no declaration sweep. Real Work members are neutral61. Source-only forms/replay retained under drafts/T-24071; production unchanged, no C proof or verified bytes. Remains In Progress for a concrete next hypothesis.

Approved unchanged trace57 passes pinned same-input whole-object/four-section equality. Synthetic masked ranges64/65 get priority3/color1(v0), selection10 priority2/color2(v1), buttons16 priority0.5/a0. Stock final uopt capture still contains AND255/CVT L/STR v0; final assembly removes the mask for byte stores and narrows the call argument separately. This places the observed loss after allocation, rather than proving v0 unavailable. Source-only capture/fidelity helpers retained, no compiler changes. No supported next source form selected; In Progress pending focused coordination.

Bounded read-only existing ugen reference explains unsigned CVTL8 argument AND construction, but does not isolate removal of the earlier per-arm mask. The measured boundary is final post-cvt uopt output versus final object (ugen/as1 interval), not a proven exact ugen deletion rule. MAIN8005D448 lacks the empty-slot callback arm; no supported untried source condition beyond prior24 narrowing forms. No further compile/probe/binary. Source57 remains; exact lowering condition unresolved, no impossibility claim.

New parent-approved stock ugen/as1 capture resolves the stage gap: whole-object/four-section fidelity passes, ugen output equals as1 input, and assigned ENT165 still contains ANDv0,v1,255;SBv0;ANDa0,v0,255. Final object removes the earlier mask. Deletion is therefore in stock as1 processing, not ugen. Source-only backend_capture.py retains exact replay/audit; outputs ignored. No supported new source condition beyond avoiding redundant narrowing while preventing known word-mask hoisting; no new probe or binary.

Bounded existing-AS1 inspection identifies candidate func41917c ANDI needed-bit-width gate and mask-chain combination; runtime table/branch not observed, so no exact executed-rule claim. A full-word use could prevent deletion but old word/argument controls cover that route and can hoist masks; no supported untried ordinary source expression found. Exact source gap: retain per-arm masked word without redundant narrowing or common-mask hoisting. No diagnostic expansion/probe. backend_capture.py docstring corrected to stock5.3 and unused import removed, with no compiler rerun.

## Final handoff
Unresolved/In Review, not Done. Best source57 remains private; production stays INCLUDE_ASM. Whole-object-fidelity trace and stock ugen/as1 capture establish that per-arm masks survive ugen and disappear in stock as1 processing. Candidate AS1 width-rule attribution remains a hypothesis because runtime gate/table values were not observed. No supported untried expression; no further tracing, casts or compiler diagnostics in this pass. No impossibility claim. All sources, controls and safe reproducers are retained under drafts/T-24071.
