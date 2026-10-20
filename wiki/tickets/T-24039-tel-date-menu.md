---
id: T-24039
title: Recover large TEL date menu C
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[matching-notes]]", "[[exhaustive-drafts]]"]
---

## Goal
Recover TEL func_8013D88C in src/ovl/TEL/8013CCD0.c using recovered menu arrays, data views and control flow.

## Acceptance criteria
- [ ] Bounded source-based pass with one Docker compiler, prior evidence read first.
- [ ] Any promoted C is unguarded normal default C, with literal resolved default proof and TEL checksum.
- [x] Preserve useful source-only evidence and leave In Review for root standards review.

## Notes
Start 8e742c12d. Only the named source file is assigned; TEL/80132000.c belongs to another worker. Notify root before shared header edits. No push/merge. T-15900 has a compiling mechanical draft scoring 1045, but its GameState pointer increments and halfword-array strides are inconsistent with target access widths; investigate actual byte/element views before tuning registers. T-19027 matched the adjacent profile setup, while its other menu draft remains distant.

## Results

Bounded source-based pass completed with one serial Docker compiler. Corrected array/state strides and current aggregate views; preserved a direct-store reconstruction at 885 rows (232-byte frame versus target224) and a marked lower-row diagnostic at850. Both annotated source-only files reproduce through drafts/T-24028/probe.py. Common fixed state base, natural indexed flags, eleven-element arrays, wider coordinates and volatile pointer reloads are negative; full ledger in drafts/T-24039/README.md and [[matching-notes]]. No header or production source changes. Target remains INCLUDE_ASM, zero new C bytes.

Docker ninja -j1 build/ovl/TEL.ok is current, and independent TEL SHA1 control passes. No literal C match was obtained or claimed. In Review for root standards review/integration; no push/merge. Exploration remains under ignored build/T-24039.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
