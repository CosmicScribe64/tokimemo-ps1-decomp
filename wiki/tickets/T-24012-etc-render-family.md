---
id: T-24012
title: Match assigned ETC rendering family
status: In Progress
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
