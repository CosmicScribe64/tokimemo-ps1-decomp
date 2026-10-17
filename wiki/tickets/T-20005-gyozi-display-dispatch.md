---
id: T-20005
title: Match GYOZI display and dispatch helpers
status: In Review
assignee: Codex
created: 2026-10-16
updated: 2026-10-16
links: ["[[matching-notes]]", "[[event-family-results]]", "[[tickets/T-17600-types-first-gyozi]]"]
---

## Goal

Match GYOZI func_8014206C (160 bytes, src/ovl/GYOZI/801418B0.c) and func_8013C774 (364 bytes, src/ovl/GYOZI/8013C3B0.c). Claimed before implementation in https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/41. Scope excludes the PR #37/#40 functions and TT.

## Acceptance criteria

- [x] Only actual default linked matches replace INCLUDE_ASM (160 + 364 = 524 bytes).
- [x] Affected overlay/full-binary SHA1 and header/global/prototype checks pass.
- [x] Local standards review and documentation complete.
- [x] Focused PR for verified work: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/pull/42.

## Notes

Branch codex/gyozi-display-dispatch starts at upstream main 2c272843. Archived T-17600 hand drafts report six compare-operand rows for func_8014206C, and fourteen scheduling/literal-placement rows for func_8013C774. Inspect their assembly and source context before changing production code. All tools run in Docker; keep probes ignored. Maintainer handles wiki integration; no repeated main merges.

## Comments

Both functions pass actual default linked proofs, the full build/checksums and prototype guard. Local standards review is complete; maintainer acceptance remains pending.

First result: func_8014206C passes actual default linked proof for all 160 bytes (build/T-20005/display-proof.log). A meaningful s32 local holding the u8 selector fixes the three compare operand orders; u8/u32 locals and direct comparison/view variants keep the six-row miss. Existing DisplayActor44 fields are used. No FAKE, callee override, header or compiler-pass change is needed for this helper.

The dispatcher's first bounded variants remain 12+ rows different: as1 moves the mode-address setup/load across the flag branch and fills slots that are nops in the original; extra local mode variables also grow the frame. Testing reuse of the existing index local to separate frame layout from scheduling. C strings also require full-unit rodata verification; no dispatcher C installed yet.

Dispatcher finding: a plain s32 return type (shared prototype and definition) gives zero instruction differences with the archived hand body. The void version lets as1 hoist mode-address/load instructions across the flag branch; the s32 fall-through preserves the original unfilled slots, consistent with v0 remaining live. Reusing the index local, unsigned/signed byte views and a volatile diagnostic did not fix the void version; volatile added address instructions and is not retained. Both the direct byte global and existing GyoziWork field view reach zero under s32. This candidate was privately linked to check real literal addresses before installation.

Second result: the installed dispatcher, with shared s32 func_8013C774(void) prototype/definition, passes actual default linked proof for all 364 bytes. The existing GyoziWork byte field is used instead of a standalone alias. default-proof.log also rechecks the 160-byte display helper. prebuild-hashes.log verifies the full GYOZI binary against the immutable checksum, including the S/T literals, other data and existing callers. The archived literal-placement concern needs no separate workaround once the return type and exact function span match.

No new FAKE, volatile access, frame padding, callee override, inline assembly, generated-data edit or compiler pass is retained. The observed return-type effect is consistent with keeping v0 live at fall-through; the literal-byte proofs and void/s32 controls establish the scheduling difference without claiming an allocator trace.

Final verification on branch base 2c272843, all Docker: default-proof.log reports actual default linked MATCH at 160 and 364 bytes; full-build.log completes the 489-step whole-tree build with binary/header/global gates; checksums.log reports 27/27 binary SHA1 checks passed; protos.log reports 0 new disagreements, 15 known and branch check OK. The full overlay hash verifies the direct caller's bytes and original literal/data placement after the shared return-type correction.

Local self-review against CODING_STANDARDS.md: plain C89 and fixed-width locals; existing SDK/DisplayActor44/GyoziWork/GzCallbacks90 types; original symbols and source order; one shared declaration for the dispatcher and no local override; no new FAKE, dummy local, volatile, assembly, compiler flags/pass or literal bypass. Only intended C/header/wiki files are staged. Meaningful selector/table-index locals account for their observed uses. Maintainer review remains pending; keep In Review.

Maintainer confirmed both functions are assigned to T-20005 and removed from automated runs in issue #41. Other functions in the same files may still be worked on upstream; the maintainer will resolve those merge conflicts. Latest pre-submission fetch still leaves both functions INCLUDE_ASM on main; no duplicate work is submitted.
