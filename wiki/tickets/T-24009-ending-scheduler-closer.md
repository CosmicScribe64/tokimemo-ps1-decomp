---
id: T-24009
title: Close ENDING func_8013A2E8 scheduler residue
status: In Review
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[tickets/T-23002-w5-big-p02]]", "[[matching-notes]]"]
---

## Goal

Bounded 15-20 minute source-only closer for func_8013A2E8 in src/ovl/ENDING/801396E0.c (4,648 instruction bytes). Investigate new assembler-complexity or lifetime hypotheses without repeating T-23002 deletion and metadata experiments.

## Acceptance criteria

- An accepted match needs unguarded default literal proof, ENDING checksum, and header/global/prototype guards.
- Otherwise preserve the existing guard and archive precise negative evidence.
- Independent review before Done; no compiler flags, inline assembly or toolchain patches.

## Notes

T-23002's binasm probes implicate a whole-function as1 scheduling threshold: deleting about 240 tail instructions or the last 14 calls enabled the original load-delay slot. This pass tests whether discarded call-result metadata or source-level control-flow boundaries can change that threshold without changing code.

## Comments

- First 12 full-unit probes: baseline and explicit void discards stay 12; casting the last 1..9 func_8013B640 calls to void-return views gives 20/24/27/42/347/353/337/340/343. Empty tests of the text cursor after tail calls give 389. No source changed. Next hypothesis: explicit first-string pointer lifetime with a discarded condition.

- First-string lifetime probes: assigning the first string to the existing pad slot retyped as char pointer and adding an empty truthiness test gives aligned 10 cost, all 1,162 instructions aligned (8 different words / 16 diagnostic rows; scorer cost is not a raw row count). It fills the original load delay with the correct lui a1; the string low half instead occupies the first branch delay, leaving the second flag mask after the first branch arm. This bypasses the original nop but does not yet match. Production guard unchanged.

- Completed 76 targeted sequential probes; no exact result. Production source remains byte-for-byte unchanged. Archived a minimal two-case reproducer and negative evidence in drafts/T-24009/. The weighted score-10 form is not installed because eight words still differ and it adds an allocation device. Zero verified C bytes.

- Preservation checks: the archived reproducer reruns at baseline 12 / pointer-lifetime 10. Explicit default linked proof reports MATCH for 4,648 bytes of the retained assembly fallback; this is not a C match. Fresh ENDING SHA1 comparison passes. `git diff --check` passes and production source has no diff. Submitted In Review for independent review of the research archive.
