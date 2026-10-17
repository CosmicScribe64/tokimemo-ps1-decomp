---
id: T-20010
title: Match NAME_ENT resource loader
status: Ready
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-20009-name-ent-loader]]", "[[name-ent-family-results]]", "[[matching-notes]]"]
---

## Goal

Match func_80142E24 (1,652 bytes) in src/ovl/NAME_ENT/80142A80.c. Named function claim remains issue #52: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/52. The T-20009 submission matches only func_80149B08; this loader and its neighbors remain unchanged ASM.

## Acceptance criteria

- [ ] Recover a valid original table/induction source form without unread argument controls.
- [ ] Verify actual default linked C, all 27 hashes and declaration/global/prototype guards.
- [ ] Review standards and annotate any required FAKE source before submission.

## Evidence and attempted controls

The first loop fills two rows of four addresses, stores the fourth column first, and advances the row pointer by 0x10. Address streams start 0x80180000/4000/8000/C000 and advance 0x10000; the final stream stops at 0x801AC000. The actual target frame is 0x80, table sp+0x5C. T-17900's historical 44-row draft has a one-word array and out-of-bounds writes; its small score does not establish safe C or a match. A correct eight-word array puts the frame/table eight/four bytes low. A ninth capacity word is a diagnostic padding control, not recovered extent.

The status machine uses existing GameState and SpriteSlot24 fields. draw2d3d reads only its first two arguments. Keeping two m2c-inferred extra arguments reduces unrelated register differences but is not an accepted source form. No header or callee prototype changes are needed merely to preserve those arguments.

Ignored scratch build/T-20009 contains corrected full-unit baselines and measured controls: signedness of all four streams, fixed/record pointer strides, shared limit variables, register hints, declaration ordering, for/while/row loop forms, live-range tests and bounded180-second permuter output. The best extra-argument/nine-word control has23 aligned edits, all in the opening loop. Correct-source controls still differ substantially; none is promoted. Return-view sweep35 compiles, zero untested, no gain.

Allocator trace (documented pinned uopt from T-20008): shared 0x10000 has priority40 and takes V0 before row pointer30.5, final stream20.5 and other streams16/15.5. The original instead keeps the step unallocated (AT) and the bound in A3. Current draft splits the bound into an AT compare and a later A3 call constant. Recover the genuine induction/source structure; this is not a proven compiler limitation. Do not solve the table mismatch by retaining the malformed one-word declaration.
