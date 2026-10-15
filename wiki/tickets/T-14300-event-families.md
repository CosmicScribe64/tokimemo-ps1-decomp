---
id: T-14300
title: Recover remaining EVENT source families
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[bunka-family-results]]", "[[etc-family-results]]", "[[broad-coverage-results]]"]
---

## Goal

Increase literal verified EVENT C bytes in isolated codex/event-14300, starting at 45d52546. Production ownership is EVENT sources and header only. Preserve existing C.

## Acceptance criteria

- Full-TU default literal linked proof and affected EVENT SHA1 for gains.
- Header, aggregate migration and prototype checks; coding standards review.
- Keep failed probes in ignored scratch with concise evidence.

## Notes

Read wiki index, local agent setup, CODING_STANDARDS, prior T-12801 EVENT results and Bunka/ETC transfer evidence before experiments. Do not repeat the 801103A0 dispatcher or historical entry-read/halfword-chain negatives. Prefer consistent GameState/CharFlags access and ordinary nested switches in reachable medium/large families. Docker only; one compiler at a time in this lane.

## Comments

2026-10-15: Created isolated worktree from origin/main; original checkout remains read-only. Ignored disc and expected inputs copied from integration without modifying raw sources.

2026-10-15 first checkpoint: five adjacent place handlers in src/ovl/EVENT/8010FE10.c pass default full-TU literal linked proof: 80112750 (480), 801130F0 (528), 80113300 (544), 801136FC (476), 801144AC (520), total 2,548 bytes. Existing EventStateView pointer used consistently for record selector, flags and calendar; existing table pointers viewed as u8 pointers as in matched siblings. No header edits or new FAKE. Evidence build/event14300/places-v1.diff and places-build.log. EVENT SHA1, headers, aggregate migration and prototype branch guard pass.

Initial 8010A91C record-loop probe improves from 12 differing rows to two after the target-supported bit1 view and consistent EventStateView pointer. Remaining pointer-initialization / zero-counter instruction order survives source initialization reversal. Rejected probe restored; scratch only. Disk pressure noted; no duplicate inputs deleted.

2026-10-15 second checkpoint: func_80117D80 adds 656 literal linked bytes (cumulative six / 3,204). Full-TU default proof build/event14300/interact-proof.diff; EVENT SHA1 interact-build.log; headers/globals/prototype guards pass. New EventStateView.unk_0FE is the signed halfword at D_800B095E, proven by the handler's lh and comparisons. Ordinary combined conditions replace m2c's cross-case labels. One marked FAKE: sp2A receives a discarded D_8012531C entry assignment before its real zero initialization; this keeps the first compare's operands in target order without adding a local or altering the frame. Rejected narrow/local/register/array/cast/return forms remain in interact-v1..v16.diff. The matching assignment emits no additional instructions.

Shop 80114C5C typed pointer and explicit-branch probes remain broad scheduling/register misses (577 output lines in places2-v2.diff), restored. Medal 801166A8 corrects a falsely narrow m2c sum and removes selector CVT hoisting with an assigned entry read, but remains seven differing instruction rows; scratch only. No claims for rejected variants.

2026-10-15 third checkpoint: func_801166A8 adds 480 literal linked bytes, cumulative seven / 3,684. Direct D_80094730 compound updates reproduce the target's untruncated sum forwarding, unlike m2c's s16 or explicit s32 intermediate. One marked FAKE discarded entry assignment reuses the real table-offset local to suppress selector CVT hoisting. Combined seven-function literal proof: build/event14300/seven-proof.diff; EVENT SHA1 medal-build.log; header/global/prototype guards pass. An initial normalized-zero diagnostic was not counted: rejected particle C in the same TU enlarged text and displaced all linked data by 16 bytes. Restoring particle ASM restores literal equality; no special string terminators needed or retained.

Particle 8011C164 existing EventParticleView44 probe remains a broad mismatch (147 differing output instruction rows) with register, signed high-half and position/velocity scheduling differences; restored to ASM. Scratch particle-final.c and medal-particle.diff retain evidence.

2026-10-15 fourth checkpoint: func_8010A91C adds 400 literal linked bytes (final cumulative eight / 4,084). Replacing the do-while with an ordinary for loop closes the two pointer/zero-initialization ordering rows; the earlier bounded negative is superseded. Existing Rec34 indexed accesses plus EventRecordFlags.bit1 reproduce the target's word shift/sign test. The bit view lives in EVENT.h, has documented size/offset, and adds no shared main declarations. No FAKE in this function.

Final combined evidence: build/event14300/eight-proof.diff proves all eight default full-TU literal linked spans; loop-build.log verifies EVENT SHA1. headers4.log and globals4.log pass. protos4.log: zero new disagreements, 12 known, branch check OK. Exact text body audit against 45d52546 preserves all pre-existing C in both touched source files (existing-c-audit.txt). Only EVENT C/header production paths changed. Two new FAKE assignments are explicitly marked and recorded above. No generated assembly, tooling flags, metrics changes, new raw inputs in Git, push or PR. Ticket moves In Review for independent standards/spec review; broader byte-coverage goal remains with integration.
