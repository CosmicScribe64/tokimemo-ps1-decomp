---
id: T-24560
title: Reconstruct ENDING level halfword slot
status: In Review
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24512-ending]]", "[[families/ending-family-results]]"]
---

## Goal

Test whether330E4's original level home at54 is a real2-byte aggregate member rather than the high half of a4-byte scalar home at56, retaining chance/table/points widths and recovered layout.

## Acceptance criteria

- Default literal C, linked bytes, all27 checksums, interface guards and standards/spec review before any match.
- One narrow object-layout hypothesis with measured negatives; no padding grid. Mark array/box as FAKE unless original semantics are supported.

## Notes

Base f37fd808aa5aad169acc1091cdbc7421c779a55d, isolated /private/tmp/tokimemo-t24560, branch codex/t24560-ending-slot. Initial4c5cd6524 advanced before work to current main; no source/probe preceded this base. T24520 path/ID belonged to another coordinator and was left untouched. Own ENDING only; no NAME_ENT, shared source/header changes, compiler patches, agents or host dependencies. Dockerj2/original disc read-only.

Read current index/status/playbook, local guidance/standards, function catalog, all330E4 family/T24512 history and fresh MAIN767A4 separated-slot evidence. Chance EdShorts18 is36 bytes ending54, points at58/rank78/frameB0 already exact. T24512 scalar level spills56; one-element/box layout is untried. Helper audit finds current declarations for retained calls, including53418 s32() and800AE0F0 void strcpy view; no renamed-void mismatch detected.

## Comments

## Narrow object evidence

Fresh ENDING checksum baseline passes. Scalar control84/199 positions uses sh/lh56. One-element s16 array87/200 fixes54 but introduces early stores. Two-byte struct member80/199 fixes sh/lh54 while preservingB0/table30/points58/rank78 and the original call/store positions. Box semantics remain unproven, so retained source is marked FAKE. No source/header edits or match claim. The bonus raw-word/extracted-value lifetime is unchanged; this slot probe does not solve that separate issue.

## Raw predicate control and handoff

Original rows84..104 load flags4C/4E, raw word a0 and extract bits9..11 through a temporary, copying extraction to v0 after the clamp. One real u32 raw_bonus local replaces one pad word; shift for the sum plus direct raw&0xE00 predicate gives96/199. It emits extraANDI, keeps raw v1/level a0 and does not match target. No further raw/padding variants. Default replay80 and --raw-mask96 are narrow Docker drivers; no full context retained.

Box80 changes exactly the two sh/lh immediates56->54 from scalar84. First84 diagnostic keys agree. Fresh ENDING SHA1/header checks and C/Python compile pass; production diff empty. Array/box and inherited pad uncertainty are explicitly FAKE-marked. Standards/spec self-review finds no raw ASM/binary/ucode, invalid C or unintended scope. No new C bytes or full27 claim. Ready for coordinator review.
