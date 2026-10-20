---
id: T-24061
title: Recover short GEKO SHOUGATU GYOZI handlers
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[scenes-family-results]]", "[[shougatu-family-results]]", "[[event-family-results]]"]
---

## Goal
Recover GEKO func_80141870 in80140A00.c, SHOUGATU func_801359FC in80134890.c, and GYOZI func_8014494C in80143AD0.c using bounded novel control-flow/type/lifetime evidence.

## Acceptance criteria
- [x] Read prior negatives/current types/matched sibling forms; one Docker compiler.
- [x] Any promotion is unguarded normal default C with literal linked proof and unit checksum; pin individually.
- [x] Preserve useful source/reproduction and leave In Review for root review.

## Notes
Start01c0c2dae. Only the three assigned TUs; no headers without coordination. GEKO T-23005 improved81/74 to51 rows using word views/local/reload. SHOUGATU35-byte copied label table has a frame/selector gap. GYOZI postincrement gap was not retried after sibling func8013506C setup return fix; test that interface evidence in scratch before proposing any shared change. No forced registers/compiler patches or push/merge.

## Outcome
Source-only bounded checkpoint: GEKO44 rows (67/69 words), SHOUGATU56 (69/69), GYOZI84 (74/74) with scratch-only setup return view. No new C bytes; no production/header/config edits. Full semantic corrections, negative ledger and tested reproduction in drafts/T-24061/README.md. Production overlay checksums pass through ASM fallback. In Review pending root review.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
