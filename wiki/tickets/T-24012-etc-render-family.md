---
id: T-24012
title: Match assigned ETC rendering family
status: In Review
assignee: Codex ETC worker
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24010-date-etc-coordination]]", "[[etc-family-results]]"]
---

## Goal
Match func_80136448, func_80132C3C, func_80132FB0 and func_801328A4 in src/ovl/ETC/80132880.c.

## Acceptance criteria
Follow T-24010 scope and verification gates; preserve matching siblings, useful drafts and concrete negative evidence. Review before Done.

## Comments
Assigned isolated worker; no other C files claimed.

Setup: isolated `codex/complete-etc` worktree at `/private/tmp/tokimemo-complete-etc`, baseline `4c5123f11`; reused existing Docker image and APFS-cloned ignored outputs. Sibling scan confirms `NAME_ENT:func_80140044` (0.750) for the dispatch; no >=0.70 sibling for the three rendering routines. Dispatch census body initially fails without accurate forward declarations for its existing void callees.

## Verified checkpoint: dispatch

`func_80136448` is verified, 1,280 literal linked bytes. NAME_ENT `func_80140044` supplies the early-return family evidence; breaks and early returns both produce the same ETC output. The decisive change is the `s32` return view without an explicit return: the prior void view removes the outer non-8 branch's `lw ra` delay-slot instruction, shifting every switch-case branch. Conservatively marked FAKE because the original source declaration is unknown; callers ignore the result. Accurate forward declarations for all existing void dispatch callees live once in `include/ovl/ETC.h`. No shared main API edits, generated assembly edits, per-function tooling exceptions, or changes to existing matching C bodies.

Docker proof at `build/etc24012/final-proof.log`: `func_80136448: compared default build`; `MATCH (linked function bytes equal target (1280 bytes))`, from `tools/funcdiff.py --unit ETC --no-build --resolve --version default func_80136448` after `ninja -j1 build/ovl/ETC.ok`. `build/etc24012/final-build.log`: `build/ovl/ETC.bin: OK`. Guards: `header-guard.log` headers OK; `global-guard.log` globals OK; `proto-guard.log` 0 new / 15 known disagreements, branch check OK.

The other three assigned routines remain on their original INCLUDE_ASM paths while bounded typed-source experiments continue. This checkpoint awaits the coordinator's required two-axis review; In Review does not claim all four functions matched.

## Independent checkpoint review

Coordinator reports the required two-axis review of frozen `a0675cafe604146244c5d762cd915467b41a46a2`: Standards PASS, 0 actionable findings (FAKE tagged, overlay declarations match existing void definitions, explicit 1,280-byte linked proof and ETC checksum); Spec PASS, 0 actionable findings (assigned scope and gates correct, three remaining targets explicitly pending). Reviewed source checkpoint is preserved without amendment. Ticket returned to In Progress for bounded work on the three remaining routines; this does not invalidate the accepted dispatcher checkpoint or claim the whole ticket Done.

## Bounded remaining-target pass

No additional matches. Source-only drafts are archived in `drafts/T-24012/` with measured blockers and rejected hypotheses in its README. Each was separately recompiled in Docker in the complete reviewed source context; `build/etc24012/archive-score.log` records resolved differing-instruction costs: `func_801328A4` 217, `func_80132C3C` 180, `func_80132FB0` 150. All compile; all are nonzero and remain production assembly. These are stable diagnostic costs, not literal-byte proof or acceptance claims.

Filter: real flat 822-record RGB buffer (three 274-record rolling rows) and SDK RECT recover the correct overall frame/RECT position, but row base is +24 bytes and input/divisor saved registers and arithmetic scheduling differ. Quad: SDK POLY_FT4 and consistent cached shade leave frame 0x40 vs 0x70, spill +4 and colour/coordinate allocation differences; the redundant colour writeback is conservatively marked FAKE in the archive. Background reader: actual table indices replace absorbed aliases; factoring per-case status improves stable cost 200 to 150, but compiler table-base sharing and duplicated-tail layout differ. No guessed padding, absolute-address aliases, additional header changes or production edits were introduced.

Declared-variable-order, register-cache/assignment-chain and explicit byte-offset experiments are bounded negatives. More source-form variation without evidence is not justified. Three targets remain pending; ticket/card moved to In Review for the coordinator's archive review and next disposition. Accepted production C/header remain exactly the reviewed `a0675cafe` checkpoint. No raw assembly, copyrighted assets or build outputs are archived.

Checkpoint result: **1 of 4 targets verified (1,280 bytes); three explicit blockers archived.** Full ticket remains incomplete; do not mark Done while those targets are pending.

Final archive review: independent Standards and Spec reviewers both PASS with zero actionable findings for a0675cafe...920cbb61b. Production source and header remain identical to the reviewed 1,280-byte checkpoint. This completes the bounded pass and review, while the three nonmatching targets remain explicitly pending; no full-ticket completion claim.
