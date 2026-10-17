---
id: T-14000
title: Match ENDING and EN_NICHI coherent families
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
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

2026-10-15 checkpoint 3: EN_NICHI func_80135600 adds 976 literal linked bytes; default full-TU funcdiff MATCH, EN_NICHI SHA1 OK, headers OK. Existing 64-word EnWords64 gains an 8x8 matrix union view. Natural 12-column / 15-row loops recover original unrolling. Keeping the /7 invariant in one scalar and the /5 invariant directly in the matrix expression recovers register ordering and avoids one unnecessary home slot. No FAKE in this function. Cumulative verified gain 2032 bytes / three functions. The AI 34800 natural EnObj reconstruction is exact except its pointer spill at sp+0x28 versus target sp+0x2C; two random locals and a marked test home fix the spill but grow the frame by eight. None is accepted. Spawn 34258 remains 26 listing rows after four bounded line-placement forms. Diagnostics remain scratchpad/t14000.

2026-10-17: The three matches (ENDING func_80136EE4, EN_NICHI func_801333B0 and func_80135600, 2,032 bytes) are on main and still plain C. Review against CODING_STANDARDS.md found nothing blocking: the one accommodation carries a FAKE comment with ticket, and only stray double blank lines remain. The wider ENDING and EN_NICHI work went on under T-18200 and T-18300 and is now live in T-20920 and T-20930, so this ticket is closed as Done. The ending-14000 worktree is idle and holds only an unmatched func_80134270 draft.
