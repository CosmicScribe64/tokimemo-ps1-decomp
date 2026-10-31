---
id: T-25025
title: Trace zero reaching definition in src/ovl/GYOZI/80140C90.c
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-24907-func-80140e0c-80140C90]]", "`plan-remaining`"]
---

## Goal

Reproduce func_80140E0C whole-TU baseline and establish whether recovered v6's zero definition survives propagation to the store/call uses before allocation. Investigate source only with a new reaching-definition mechanism, no extra target instructions.

## Acceptance criteria

- [x] Record one consistent resolved baseline, separating historical word counts.
- [x] Inspect current and recovered-v6 use/definition evidence.
- [x] Pin a justified source result or precise unresolved trace question.

## Notes

Exclusive source src/ovl/GYOZI/80140C90.c. Same isolated tree as T-25023; BUNKA claim released with separate clean pin a9608aebab7fcb1d991d82a3cbb8df61d07ab31e. No other source/header/build changes, zero-local/promotion/assignment grids, automatic Astra or old workers. Docker-only analysis.

## Result

Current and recovered-v6 full-TU C both have 97 target / 96 candidate instructions, five printed diff lines, aligned cost 5. Missing entry a1-zero at 0x80140E24 plus store/call substitutions. Archived three-word/six-row counts are not this convention. Source unchanged.

Input ucode has z's entry zero store M23/-8 and merge load. Native post-propagation graph retains old z expression entries plus a new literal-zero store (bit76), but no z live range reaches allocation. Optimized ucode removes M23/-8 completely and creates a merge literal-zero R5 value used for the store and argument R4. Initial as1 nodes already use sb zero/move a0,zero: loss precedes as1 scheduling. Stock and traced v6 outputs match all four relevant sections. The old graph entries do not prove live reaching definitions.

No new source mechanism justified; no grids, new variants or Astra. Precise remaining question: identify uopt's statement rewrite/dead-use operation replacing the live z edge and ugen's register-constant substitution rule. Existing phase-boundary evidence answers the carrier question but not the exact internal subphase. Full bounded evidence: drafts/T-25025/README.md. Root owns review/integration; no source/header/tool changes or coverage.

## Checks

Docker single-object refresh, whole-TU baseline, v6 pre/post-uopt and initial-as1 captures completed. Four-section stock/trace fidelity passes. Wiki lint: zero broken links/status mismatches, one existing large-page warning. `git diff --check` clean; no source/include/tools/config changes. No full integration build for this documentation-only result.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
