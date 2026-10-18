---
id: T-24075
title: Recover ETC random selection with current pointer layout
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8013C8EC (904 bytes), src/ovl/ETC/8013BEF0.c. Preserve all neighboring C, including verified BFFC/3C330. Current primary target remains ASM; no active claim or later target-specific T-240xx packet found.

## Acceptance criteria
- Read latest primary wiki/source/history, original assembly and old92-row T-17300 draft. Fresh valid full-TU baseline before new source controls; old92 is not fresh progress.
- Correct real s32* D_801500C4 indexing: target byte stride8 means word-pair index*2, not old pointer+index*8/byte stride32. Verify six original DIVU arms against current signed RNG and actual C/K&R promotions before claiming any unsigned-cast correction.
- Same sole Sol medium Docker compiler/ninja -j1, built checkout from9f0b1b289 on codex/complete-etc-random-selection; no extra agents/instrumentation or broad sweeps.
- Source-led target-only corrections are authorized; coordinate exact shared header/config scope first. Actual C needs default literal904-byte proof, ETC hash/interfaces and exact neighbor audit; parent independently reviews/all27.

## Notes
Six disjoint old remainder temporaries may differ from target's single v1 web; inspect fresh output before joining them. Existing u8[13] pick-count arrays and actual pointer types provide semantic evidence, not a match promise. Historical score improved244 to92 with width recovery. T-24069 BFFC is independently Done in this TU. Rejected43758 selection was withdrawn after primary T-24055 showed its byte defect already corrected; avoid stale worktree history.

Fresh faithful full-TU corrected-pair baseline92/frame30. Actual object has six
DIVU, zero signedDIV: K&R u8 promotion already supplies unsigned modulo, so no
explicit unsigned cast control is justified. Joined disjoint remainder locals
worsen116; rejected. Baseline extra s1 constants and eliminated repeated global
stores motivate one ordinary global-assignment/short-circuit rejection-loop
control, preserving RNG call count and rejection sets. Production unchanged.

Ordinary global assignment plus short-circuit rejection loops improves92 to12
resolved rows, exact frame30/226words. All six RNG loops now agree, including
repeated global stores and at-immediates. Remaining six real words rotate
index/base/shift among t5/t6/t7 and reverse ADDU operands in the tail pair lookup.
Actual s32* pair loads and equivalent address operand reversal each remain12;
no sweep follows. Source-only checkpoint/replay in drafts/T-24075, production
ASM unchanged, ticket In Progress pending concrete source investigation.

Matched same-TU8013F320 uses direct arguments from the same s32 pair table.
Porting that source idiom closes all six tail words: private default-resolved
full-TU zero/904B, pending actual production proof. Sole caller8013BF34 ignores
result, original epilogue does not establish return; retain existing implicit
s32 matching view with conservative FAKE note, no header/config change.
