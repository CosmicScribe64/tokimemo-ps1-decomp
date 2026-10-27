---
id: T-25026
title: Reconcile func_8006D7EC in src/main/8006CB30.c
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-23003-w5-big-p03]]", "[[tickets/T-24600-finish-matching]]", "[[tickets/T-25013-compiler-level-levers]]", "[[tickets/T-25016-candidate-reconstruction]]"]
---

## Goal

Make a reliable high-byte near-candidate decision for func_8006D7EC (4692 bytes) by reconciling current whole-TU output with the historical one-row claim and subsequent evidence.

## Acceptance criteria

- [x] Reproduce current in-tree whole-TU resolved baseline with a precise metric.
- [x] Reconcile T-23003 and later ledger; inspect exact ucode/ugen mechanism.
- [x] Test only a novel justified lever or report prior tested closure/blocker.

## Notes

Exclusive src/main/8006CB30.c, only func_8006D7EC initially. GYOZI released at separate source-unchanged pin 5c2954688. Isolated tree /private/tmp/tokimemo-t25023, base 883760224 plus preceding documentation checkpoints. TEL is separately owned. No grids, shared declarations/build tools, automatic Astra or other functions. Preserve all source/ABI FAKE annotations.

## Result

Current whole-TU C confirms the historical one-position guard: 1173/1173 instructions, zero-based row94 only, two printed lines, aligned cost2. At 0x8006D964 target addiu v1,v1,1 versus candidate li v1,1. No source changes or new variants.

Input ucode loads idx M13/-12, INC1, stores it. Optimized ucode emits literal1/STR R3 at the same source increment, with no R3 dependency; stock as1 initial-node trace already contains li v1,1. Existing traced/stock full-TU outputs match .text/.rodata/.rel.text/.rel.rodata. This is a uopt fold, not offset or scheduler residue.

T-23003/T-24001/T-24204/T-24710 already cover direct-counter, equal-constant merge, aggregate/alias, dead/live backedge and other grids. Live backedges retain addiu with extra control instructions. T-25013 global zcopy-off worsens two printed lines to1630 and fails uniformity; reverted. T-24600 and T-25016 supply no newer full-TU D7EC source repair. Prior tests close those approaches, not all possible source. No novel reaching-value mechanism without extra target code established. Reliable high-byte near candidate, parked pending that evidence; no automatic Astra.

Full ledger and exact private trace references: drafts/T-25026/README.md. Preserved existing FAKE/ABI comments and fallback. Root handles integration/review; no C match or coverage. Ticket to In Review.

## Existing exact Astra consultation reused

Root authorized one consultation after checking for an identical predecessor. T-24001's Astra follow-up already covers this exact counter/addiu-vs-li blocker with 19 causal tests and full-TU row94 verification. Diff from d57c78028 to current source contains only the guard comment, not body/ABI/FAKE changes. Current whole-TU metric and phase evidence preserve its premise. Reused the recorded conclusions; no duplicate consultant or test. Its precise open barrier remains eliminating the artificial backedge without losing the nonconstant counter dependency. No evidenced new mechanism found.

## Checks

Single-object baseline refresh, whole-TU resolved measurement, pre/post-uopt capture and stock initial-as1 capture completed in Docker. Four-section trace fidelity passes. Wiki lint: zero broken links/status mismatches, one pre-existing large-page warning. `git diff --check` passes; production/header/tool/config diffs empty. No full integration build for the documentation-only checkpoint.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
