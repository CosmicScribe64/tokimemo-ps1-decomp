---
id: T-24049
title: Recover ETC exam ranking generator
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[etc-family-results]]"]
---

## Goal
Recover ETC func_801480D0 in src/ovl/ETC/80146400.c with a bounded source-semantic, data-view and loop pass.

## Acceptance criteria
- [x] Read current coordination and earlier ranking/type evidence; one Docker compiler.
- [x] Any promotion is unguarded normal default C with literal linked proof and ETC checksum.
- [x] Preserve useful corrected source/reproducer; leave In Review for root review.

## Notes
Start c6847bd31, TEL match pin unchanged. Exclusive target function/TU only; headers require coordination. Existing ranking aggregates were split back to three arrays in T-22029 to avoid a hoisted base register. T-17300 redrafts failed to compile. The older types repair reports1200 rows but uses an obsolete aggregate view in this checkout; reconstruct against target before interpreting its scores. No push/merge.

## Results

No match. Preserved current-view semantic reconstruction at885 printable rows,709 words versus702 target, frame216 versus208. Corrected signed byte-spread remainder operations, seed/score aggregate fields, obsolete ranking structure access, broken Shift-JIS debug spelling and a real final-bonus bound error (15 records in the prior repair versus11 in target). The old types repair's control flow itself is intact; its obsolete aggregate view and stale score were the initial compatibility problem.

Selector/noise lifetime changes and explicit phase/tie-counter forms are negative; details and reproduction in drafts/T-24049/README.md. Annotated source reproduces885 through actual scratch C. No production or header changes, zero new C bytes. Docker ninja -j1 ETC control is current and independent overlay SHA1 passes through unchanged ASM. In Review pending root source/archive review; no push/merge.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
