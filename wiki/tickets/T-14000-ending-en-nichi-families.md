---
id: T-14000
title: Match ENDING and EN_NICHI coherent families
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ['[[matching-notes]]', '[[decompile-workflow]]']
---

## Goal

Increase verified byte coverage in ENDING and EN_NICHI using typed family reconstruction.

## Acceptance criteria

- Preserve existing matched C.
- Verify each conversion through full-TU literal funcdiff and affected overlay SHA1 in Docker.
- Review against CODING_STANDARDS.md before Done.

## Notes

Owned scope: src/ovl/ENDING, src/ovl/EN_NICHI and their overlay headers. Isolated codex/ending-14000 worktree from origin/main 0e670a13.

## Comments

2026-10-15: Read wiki index, workflow, matching notes, coding standards and local agent instructions. Investigating medium/large remaining functions.

2026-10-15 checkpoint: ENDING func_80136EE4 adds 580 literal linked bytes. Full-TU default funcdiff MATCH and ENDING SHA1 OK. Existing Rec38 CharFlags b14 replaces the raw mask so the target sll/bltz and subsequent temporary sequence match. No FAKE, compiler or ASM changes. First scene-handler probes 37BB4/37E40/387B0 retain known register differences; EN_NICHI 362A4 currently has a frame/register gap. Rejected forms restored to ASM, diagnostics remain scratchpad/t14000.

2026-10-15 checkpoint 2: EN_NICHI func_801333B0 adds 476 literal linked bytes; full-TU default funcdiff MATCH and EN_NICHI SHA1 OK. Natural ten-element loop reproduces original peel/unroll behavior; s32 return type without an explicit return preserves v0 for the final clamp and split epilogue, where void differed by six listing rows. Header reflects this matched return type and the existing void definition of func_801337E0. Rejected 3358C accumulator and 35600 copied-table forms remain diagnostic scratch files. Cumulative verified gain 1056 bytes / two functions.

Review correction: func_801333B0 return/liveness form is explicitly marked FAKE because the original return type is unconfirmed. Its s32 declaration exists to preserve the clamp accumulator and split epilogue. No additional byte claim.
