---
id: T-20010
title: Match NAME_ENT resource loader
status: In Review
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-20009-name-ent-loader]]", "[[name-ent-family-results]]", "[[matching-notes]]"]
---

## Goal

Match func_80142E24 (1,652 bytes) in src/ovl/NAME_ENT/80142A80.c. Named function claim remains issue #52: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/52. The merged T-20009 submission matches func_80149B08; this ticket completes the separately claimed loader while preserving neighboring matches.

## Acceptance criteria

- [x] Recover a valid original table/induction source form without unread argument controls.
- [x] Verify actual default linked C, all 27 hashes and declaration/global/prototype guards.
- [x] Review standards and annotate any required FAKE source before submission.

## Historical evidence and attempted controls

The first loop fills two rows of four addresses, stores the fourth column first, and advances the row pointer by 0x10. Address streams start 0x80180000/4000/8000/C000 and advance 0x10000; the final stream stops at 0x801AC000. The actual target frame is 0x80, table sp+0x5C. T-17900's historical 44-row draft has a one-word array and out-of-bounds writes; its small score does not establish safe C or a match. A correct eight-word array puts the frame/table eight/four bytes low. A ninth capacity word is a diagnostic padding control, not recovered extent.

The status machine uses existing GameState and SpriteSlot24 fields. draw2d3d reads only its first two arguments. Keeping two m2c-inferred extra arguments reduces unrelated register differences but is not an accepted source form. No header or callee prototype changes are needed merely to preserve those arguments.

Ignored scratch build/T-20009 contains corrected full-unit baselines and measured controls: signedness of all four streams, fixed/record pointer strides, shared limit variables, register hints, declaration ordering, for/while/row loop forms, live-range tests and bounded180-second permuter output. The best extra-argument/nine-word control has23 aligned edits, all in the opening loop. Correct-source controls still differ substantially; none is promoted. Return-view sweep35 compiles, zero untested, no gain.

Allocator trace (documented pinned uopt from T-20008): shared 0x10000 has priority40 and takes V0 before row pointer30.5, final stream20.5 and other streams16/15.5. The original instead keeps the step unallocated (AT) and the bound in A3. Current draft splits the bound into an AT compare and a later A3 call constant. Recover the genuine induction/source structure; this is not a proven compiler limitation. Do not solve the table mismatch by retaining the malformed one-word declaration.

## Continuation on main 567b5663

A valid two-row/four-column nested loop with row * 0x10000 and column * 0x4000 recovers the original bound, step and stream register allocation without unread call arguments. Full-unit control reaches 12 aligned edits: stack layout plus store/update ordering. Shift expressions do not give the same strength reduction. Ignored scratch evidence is build/T-20010/nested; the clean-declaration and explicit-column controls below recover the final match.

## Recovered matching source

The explicit four-column loop uses one row index: addresses[row * 4 + column] = bank_start + row * 0x10000, with four separate column assignments in increasing order. IDO strength-reduces these products into the original four address streams, keeps the 0x10000 step in AT, and retains the 0x801AC000 bound in A3. Scalar-shift variants do not recover this form. Declaring the meaningful row index before the exact eight-word table puts the table at sp+0x5C in the original 0x80-byte frame. No oversized array, dummy padding, dead initializer or unread call arguments are needed.

Full-unit resolved instruction comparison reaches zero edits. The production body uses existing GameState fields and the existing SpriteSlot24 view of the work area, including byte color stores. Existing shared and overlay declarations suffice; no header, compiler or build-tool changes. Docker default build passes (531 steps), with headers OK and globals OK. All 27 independent SHA1 checks pass. Actual default linked funcdiff reports MATCH (1,652 bytes); this is the new C body, with no ASM fallback for the function. Prototype guard: 0 new disagreement(s), 15 known (config/proto_known.txt); branch check OK. Logs remain ignored under build/T-20010.

## Review

Self-review against CODING_STANDARDS.md: fixed-width C89 declarations and comments; exact eight-word bounds; existing GameState/SpriteSlot24 layouts and canonical API declarations; no new FAKE constraints, dead locals, forced registers, compiler changes or nonmatching guards. Only the claimed body changes in production; neighboring T-22007 matches are preserved. No generated assembly, game assets, binaries, probe scripts or instruction listings are submitted. Ticket and kanban move to In Review; maintainer review remains pending.
