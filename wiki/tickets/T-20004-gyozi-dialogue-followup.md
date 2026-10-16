---
id: T-20004
title: Match the remaining GYOZI 80134FD0 dialogue helpers
status: In Progress
assignee: Codex
created: 2026-10-16
updated: 2026-10-16
links: ["[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Inspect and match GYOZI func_80135118 (360 bytes) and func_80135280 (416 bytes), in src/ovl/GYOZI/80134FD0.c. Work claimed before starting at https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/39. Excludes func_8013506C (PR #37), all other GYOZI files and TT.

## Acceptance criteria

- [ ] Only actual default linked matches replace INCLUDE_ASM.
- [ ] GYOZI/full-binary SHA1 and header/global/prototype checks pass.
- [ ] Local standards review, evidence and a focused PR for verified work.

## Notes

Branch codex/gyozi-dialogue-followup starts at upstream main 9af6ac58. No other open issue claims this file; TT remains claimed in #2. All decompilation and build tools run in Docker. Maintainer handles wiki merge conflicts, so do not repeatedly merge main into a contribution branch.

## Comments

Implementation and verification pending. Keep unverified candidates in gitignored scratch; do not count them as matches.

Initial finding: archived m2c context declares D_800C3370 as a word scalar and indexes its address, but the original assembly reads a byte at an unscaled linear offset (girl index times seven plus the step byte shifted by three). Probe it as a byte array, with D_800F647D as u8. Canonical s16 timer updates and the setup callee's shared s32 type reduce the 360-byte helper to 29 differing listing lines; the 416-byte helper also needs fresh selector reads and reaches 49 lines. These remain scratch-only, not match claims. Evidence: gitignored build/T-20004/probe.log and probes/.

Structured control flow and the natural D_800F647D++ < 0x40 comparison reduce both helpers to the same 15-line difference: an unchanged-byte store before the sound call is missing, moving later branches by one instruction. Plain self-assignment is optimized away. Address/normalization forms retain the store, but introduce one temporary-register offset (4 listing rows in func_80135118; the same offset propagates further in func_80135280). Trying equivalent store/expression forms in scratch. No default match claimed and no production source/header changed.
