---
id: T-20004
title: Match the remaining GYOZI 80134FD0 dialogue helpers
status: Done
assignee: Codex
created: 2026-10-16
updated: 2026-10-16
links: ["[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match GYOZI func_80135118 (360 bytes) and func_80135280 (416 bytes), in src/ovl/GYOZI/80134FD0.c. Work claimed before starting at https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/39. No new decompilation outside those two functions; the shared setup return-type correction from PR #37 is a dependency in GYOZI.h/80134000.c. Excludes the PR #37 callback itself and TT.

## Acceptance criteria

- [x] Only actual default linked matches replace INCLUDE_ASM (360 + 416 = 776 bytes).
- [x] GYOZI/full-binary SHA1 and header/global/prototype checks pass.
- [x] Local standards review and evidence recorded.
- [x] Focused PR for verified work: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/pull/40.

## Notes

Branch codex/gyozi-dialogue-followup starts at upstream main 9af6ac58. No other open issue claims this file; TT remains claimed in #2. All decompilation and build tools run in Docker. Maintainer handles wiki merge conflicts, so do not repeatedly merge main into a contribution branch.

## Comments

Actual default linked proofs, full build/checksums and prototype guard pass. Keep unverified candidates in gitignored scratch; do not count them as matches.

Initial finding: archived m2c context declares D_800C3370 as a word scalar and indexes its address, but the original assembly reads a byte at an unscaled linear offset (girl index times seven plus the step byte shifted by three). Probe it as a byte array, with D_800F647D as u8. Canonical s16 timer updates and the setup callee's shared s32 type reduce the 360-byte helper to 29 differing listing lines; the 416-byte helper also needs fresh selector reads and reaches 49 lines. These remain scratch-only, not match claims. Evidence: gitignored build/T-20004/probe.log and probes/.

Structured control flow and the natural D_800F647D++ < 0x40 comparison reduce both helpers to the same 15-line difference: an unchanged-byte store before the sound call is missing, moving later branches by one instruction. Plain self-assignment is optimized away. Address/normalization forms retain the store, but introduce one temporary-register offset (4 listing rows in func_80135118; the same offset propagates further in func_80135280). Trying equivalent store/expression forms in scratch. No default match claimed and no production source/header changed.

Result: default-proof.log proves both actual default linked matches: func_80135118 360 bytes and func_80135280 416 bytes (776 total). Removed the redundant byte cast from the draft without changing the match and renamed the sound-code local. Each helper has one explicit FAKE, D_800F647D |= 0: it retains the original unchanged-byte store before the sound call, while plain self-assignment disappears and other pointer/normalization forms shift the temporary sequence. Original spelling remains unknown. No volatile, inline assembly, compiler-pass change or per-file flags. Scratch scoring logs and candidates remain gitignored in build/T-20004/.

The production header adds D_800C3370 as an unsized u8 array (lbu at a linear byte index) and D_800F647D as u8 (lbu/sb), once in main_api.h. The setup callee's shared s32 declaration/definition is the same correction already requested and verified in PR #37; there is no local override. This dependency adds no new decompiled-byte claim.

Final Docker verification: default-proof.log gives actual default linked MATCH for both helpers (360 / 416 bytes). full-build.log completes all 490 ninja steps, including GYOZI SHA1 and header/global gates; checksums.log reports 27/27 binary SHA1 checks passed. protos.log reports 0 new disagreements, 15 known and branch check OK. No production game data, extracted assets or generated outputs are included.

Local self-review against CODING_STANDARDS.md: C89 block-top declarations/comments and fixed-width types; symbols retained; existing source placement and SDK headers; one shared declaration per new main symbol; setup header/definition types agree with PR #37; both FAKE sites give their reason and T-20004. No guessed aggregate, inline assembly, volatile access, compiler rule, per-file flags or unrelated refactor. Every accepted match has actual linked proof and the full-unit checksum. Ticket/board/index synchronized; upstream review remains pending, so keep In Review.

2026-10-17, maintainer review (PR #40): no findings; the FAKE no-op store names T-20004 and the reason. Verified in the integration tree: clean 27/27, headers, globals, prototypes OK, funcdiff MATCH for func_80135118 (360) and func_80135280 (416). Status Done.
