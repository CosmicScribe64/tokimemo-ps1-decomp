---
id: T-24033
title: VALEN selector sibling completion
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-20
links: ["[[event-family-results]]", "[[tickets/T-19016-w3-d-p16]]"]
---

## Goal

Recover VALEN func_80134030 and func_80132D0C in their two assigned files using matched selector siblings. Other functions and shared headers remain unchanged unless parent is notified of a necessary repair.

## Acceptance criteria

- [x] No win claimed; guarded draft only, baseline default unit checksum passes.
- [x] Retain useful source/provenance for unresolved forms.
- [x] Header/global/prototype checks pass.
- [x] Independent review.

## Notes

Read index, T-19016 and T-24019 GYOZI analog negatives first. VALEN34030 has the same two default-case LUI hoists as GYOZI39E4C27; VALEN32D0C has selected-girl allocation differences. Prefer matched GEKO/SHOUGATU direct stores and canonical GameState fields. One Docker compiler/ninja-j1, bounded25min; no exhaustive brute force.

## Bounded result

Zero of two matched; zero verified bytes added. func_80132D0C is retained under NON_MATCHING at7 resolved rows, while default ASM fallback stays intact. func_80134030 retains its prior27-row guard without any production edit. No shared header or other function changed. VALEN default SHA-1 passes:356aed2481987108f908d7013cc0392cea9dcc8c. Headers/globals pass; prototype branch check0 new/15 known disagreements. Default ASM fallback is a baseline control, not C recovery proof.

New source lead for func_80132D0C: direct girl accesses through a local GameState pointer reduce the current64 rows/54 aligned to13/13. A meaningful s32 mode local reduces13 to7, fixing the three comparison operand orders. The original shifts its girl dispatch index in the first girl==9 branch delay slot and retains a later nop; current compiler shifts in a later girl!=0 branch delay slot, leaving one fewer instruction plus branch offsets. All other instruction operands match. Mode reloads/u8/u32, explicit nested first test, selector casts and one bounded source-line grouping family keep7. Cached girl locals worsen allocation; a self-store adds narrowing and worsens218/160, so it was rejected. No FAKE is needed in the selected natural seven-row form.

func_80134030 reproduces the T-19016/GYOZI39E4C default-block LUI hoisting gap:27/27. State pointer, u32 selector and equivalent fallback-after-switch layout leave the same result. Earlier second-label/default-range and autotweak negatives were read and not exhaustively repeated. The matched GEKO/SHOUGATU direct case-store recipe already underlies its prior guard. No compiler-impossibility claim.

Read wiki index, T-19016, T-24019 GYOZI analog notes, current sibling C and target disassembly before probes. Selected source/provenance is kept here; all loose experiment sources, scripts, inputs and instruction diagnostics remain intact in ignored build/T-24033-probes/. No older archive was edited.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
