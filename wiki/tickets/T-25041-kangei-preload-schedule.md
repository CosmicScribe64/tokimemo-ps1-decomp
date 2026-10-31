---
id: T-25041
title: Match KANGEI local scheduling with upstream dependency evidence
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-22
links: `[tickets/T-25018-kangei-near-closure`, [[tickets/T-25037-bunka-upstream-emission]], [[as1-scheduler-rule]]]
---

## Goal

Match KANGEI func_8013852C through a C form that changes the local scheduling dependency or readiness while retaining the original instructions and semantics.

## Acceptance criteria

- [x] Read prior T-24202/T-24901/T-25018 negatives before new experiments.
- [x] Capture actual upstream emission and justify a distinct source lever.
- [x] Literal guarded and promoted default C proof, affected checksum and declaration checks.
- [x] Root review against CODING_STANDARDS.md before Done (integration pending).

## Notes

Exclusive source src/ovl/KANGEI/80137B90.c. Reuse /private/tmp/tokimemo-t25037 on codex/t25041-kangei after preserving BUNKA_SD pin5e132362e. No further BUNKA edits, no additional agents, compiler changes or per-function exceptions. Existing Docker only. Parent owns root integration and full27 gate.

## Comments

2026-10-22: Prior baseline is180 bytes with a single addu/lui swap. Unlike BUNKA, local as1 readiness differs (lui15, addu16), so a plain emission-order change is insufficient. New work starts with optimized ucode and block boundaries, preserving all previous cast/temp/store-order negatives.

### New source lever and trace correction

Captured stock full-TU baseline reproduces45 words, cost2. Empty tests of year after the sum store preserve the old pair. An empty test of D_80139ADC instead makes uopt promote the value and emit its store as an `rstr` on the following source line. This fixes the schedule while moving only the sum result t6 to t7 (cost4). Removing the redundant signed-halfword conversion of the u8 year compensates for the added result conversion: both `(u16)year` and plain `year` give cost0,45 words.

The new trace corrects the readiness-only explanation in T-24901/T-25018. At time16 the nodes still have aftercycles4 and besttime15 for lui versus16 for addu, but as1 picks addu first. The final dependency graph is the same apart from result register/source line; the store's address now has the following line from the emitted rstr. Readiness alone therefore does not determine this tie. No compiler modification or deleted-instruction patch was required.

### Final proof and review

The simpler plain `if (D_80139ADC) {}` matches with plain `year`; selected over the unsigned-false control. Removing the test with the same simplified year gives cost4, so the marked control is necessary. Both guarded and promoted default C independently pass private tools/funcdiff_verify.py exact_match: correct function address/span, **all180 bytes equal target**. Guard and assembly fallback removed.

Rebuilt all8 current KANGEI C units using the generated normal Ninja commands, then normal link/objcopy/SHA1 commands: **KANGEI.bin OK** (b94bbaceb7083c4b56d3b2ff56313cbcfbe6ac83). Read-only shared assembly inputs were retained; no compiler/build-rule changes. Header/global checks pass; branch prototype check reports0 new disagreements,15 known. One existing compiler warning about `&` before an array occurs in unchanged 80135F10.c.

Root final-source standards/spec review: no findings. u8 year makes the removed conversions value-preserving; the nonvolatile empty field test has no effect; all unusual source controls are marked FAKE. Ticket moves to In Review for root full27-unit integration and main push. Negative ledger in drafts/T-25041/README.md; superseding note appended to drafts/T-24901/NOTES.md. This branch adds one matched function/180 bytes after the separately pinned BUNKA_SD commit.

## Root acceptance

Independent standards/spec review passes. Integrated default-C literal proofs860/180bytes, fresh27/27binary checksums and declaration guards pass, prototypes0new/15known. No assembly fallback. Accepted and moved to Done.
