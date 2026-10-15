---
id: T-13202
title: Reconstruct event sprite initializer family
status: Done
assignee: codex-events-13200
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-13200-event-family-transfer]]"]
---

## Goal
Test existing DisplayActor44 aggregate and chained structure assignment source forms on coherent GYOZI/SHOUGATU sprite initializers.

## Acceptance criteria
Preserve full TU context and existing C; require default literal proof and affected unit checksum for accepted changes. Bound failed probes and record scores.

## Notes
F494 m2c raw output has unresolved stack structs and copies. Reuse existing game.h DisplayActor44 rather than introducing another sprite layout.

## Comments

Both F494 and 41ACC reach full-TU0 with existing DisplayActor44 and natural chained copies. Two implicit temporary addresses initially differ by4 bytes; loop-local, register, loop-form, parameter and explicit-copy variants do not match. One declared unused s32 shifts both implicit temporaries into observed slots without changing frame or instructions; it is marked FAKE and tracked here. Held-out GYOZI sibling independently repeats4->0. Pending default literal and overlay checks.

Accepted F4941328 bytes and GYOZI41ACC1440 bytes: default linked function-byte proof and both full overlay SHA1 checks pass; header check passes. Evidence build/events13200/init-proof.txt and init-build.log. New overlay-only scalar declarations reflect observed byte/halfword widths. Total initializer gain2768 bytes; lane gain3424 bytes including selector match.

Two-axis GPT6.1Sol medium review confirmed byte evidence and found two issues: appended overlay externs bypassed guards, and GYOZI func_800BCE40 had only an implicit C89 declaration. Added per-overlay guards and the necessary central old-style s32 function declaration in main_api.h (actual call at800BCE40, source width/return identical to implicit int). This is the only shared header edit and requires all27 SHA1 checks before acceptance. The symbol semantics remain unknown; no rename.

Source work complete; two-axis review and failed probes summarized in [[event-family-results]]. At that checkpoint final all27 validation was pending; now completed below.

Final review/closure2026-10-15: GPT6.1Sol medium Standards and Spec reviews complete; all findings resolved. Final default literal proof656+1328+1440=3424 bytes, independent27/27 binary hashes, headers/globals and prototype branch check pass (0new/15known). All744 baseline bodies unchanged. Evidence: build/events13200/final-proof.txt, final-hashes.txt, all27-build.log, final-protos.txt, existing-c-audit.txt and progress.txt. Final closure review confirms six particle negatives add no production code or coverage. Ticket Done; broader80% objective belongs to integration and is not claimed complete.
