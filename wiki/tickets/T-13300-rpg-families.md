---
id: T-13300
title: Recover RPG_BAT source families
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[broad-coverage-results]]"]
---

## Goal
Recover coherent RPG_BAT source/type/calling-convention families from existing near misses, adding literal verified linked bytes.

## Acceptance criteria

- [ ] New C verified through default full translation unit and RPG_BAT SHA1.
- [ ] Headers, globals and prototypes checked; existing matching bodies preserved.
- [ ] Standards review completed and findings resolved.

## Notes
Start aa2b3158; owned production paths src/ovl/RPG_BAT/*.c and include/ovl/RPG_BAT.h. Existing broad sweep and rejected 801454F0 counter probes read before work. Docker only.

## Comments

### 2026-10-15 bounded near-miss experiments

No production source changes and **0 new verified bytes** at this checkpoint. All runs use existing tokimemo-decomp-arm64 image, one CPU/compiler. Scratch evidence is retained in build/rpg13300; driver scripts copied there for reproduction.

- func_8013AF1C: corrected full-TU baseline is six rows, not the archived broad ledger's ten. Canonical GameState.unk_0D4 replaces ambiguous D_800E6354, missing byte-store declarations are temporary context, and existing void func_8013B534(void) needs its forward declaration. The sole residual gap is the case-1 negative argument: target loads D_8015EDB4 into v0 before negating into a1; candidate loads a1. Existing RpgBatWords3 member form, local load, assignment expression, unsigned negation, subtraction, return-type and old-style call views do not close it. Multiplication and the sibling dead-test form worsen it.
- func_801568A4: corrected baseline is twenty rows. Both increments of D_8015EE0C choose v0 instead of target v1. Existing D_8015EE00 aggregate member/pointer forms, selector+counter aggregate form, s32 return, and sibling empty-test forms do not improve it. No artificial register-forcing source accepted.
- func_80152080: corrected baseline is thirty-two rows with temporary D_8012163C byte declaration. m2c emits a bad SJIS escape (literal x96 prefix); target frame is eight bytes larger and several live ranges differ. It remains assembly.
- Small helper func_801495DC: target word spill establishes the clamped local is s32; changing m2c's s16 local reduces 114 to forty rows. Remaining stack-slot order and redundant global stores need source/alias evidence. Adjacent func_8014AB84/8014AC74 drafts expose stride-indexed state records rooted at D_8015EC74/EC88; their current raw-pointer drafts are unsuitable for production until aggregates are recovered.

Missing canonical globals D_80121316, D_8012135A, D_80120E0A, D_8012163C were only supplied in scratch compiler context. No shared header edits are proposed without a verified function. Existing matching bodies remain untouched.

### Bounded stride-record reconstruction

`type_recovery.py --sym D_8015EC74` reports a high-confidence but over-broad D_8015EB7C 112-word array proposal (154 functions). That proposal alone does not justify replacing the many legacy scalar declarations. Direct target evidence in 8014AB84 proves activity[4] at EBAC+0xC8 and position[4] pairs at EBAC+0xDC (word strides 4 and 8 respectively). Held-out 8014AC74 walks the activity words by four bytes and consumes those position pairs. Scratch `RpgWaterState` models offsets 0xC8 and 0xDC, size 0xFC, via a legacy scalar address view; it is **not accepted as a canonical shared type**.

The compact existing guarded AB84 draft, rewritten through this view, remains twelve differing rows (six instruction pairs), solely the reloaded parameter a3/t9 range. Unsigned parameter, register hint and implicit-int return do not improve it. Fresh m2c introduces redundant locals and grows the frame; the existing guarded draft is the correct starting point. Held-out AC74 typed view remains thirty-six rows; initialization reordering reaches thirty-four, moving the entry-position expression first worsens to forty-four and operand commutation to 164. The existing guarded updater correctly advances a word pointer by one; fresh m2c incorrectly prints +=4 for the target four-byte increment. Do not reuse that fresh body without repairing the scaling.

Final accepted gain: **0 functions / 0 verified bytes**. No production edits or shared declarations. Diagnostic-only evidence is committed for integration review; function recovery remains unfinished. Driver scripts and objects are retained in build/rpg13300; no generated data is staged.

### Diagnostic handoff

This ticket is In Review for the diagnostic-only work delivered here. No match, overlay SHA1, or coverage gain is claimed. Existing source/header files were never modified. Future function recovery should use a follow-up ticket.

2026-10-17: Superseded by T-17100 (RPG_BAT types-first) and its packages T-17101 to T-17119, plus T-16210, T-16930 and T-12300. All six functions this ticket probed (func_8013AF1C, 801568A4, 80152080, 801495DC, 8014AB84, 8014AC74) are now plain C on main. The ticket itself delivered only diagnostics and no accepted bytes.
