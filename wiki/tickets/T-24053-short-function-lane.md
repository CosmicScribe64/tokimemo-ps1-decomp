---
id: T-24053
title: Recover three short functions
status: In Review
assignee: codex/complete-option
created: 2026-10-18
updated: 2026-10-18
links: ["[[event-family-results]]", "[[bunka-sd-family-results]]", "[[shugaku-family-results]]", "[[tickets/T-20620-as1-scheduling]]"]
---

## Goal

Raise actual C function count: EVENT func_8010B614 (100 bytes), BUNKA_SD func_801355AC (132), SHUGAKU func_8013A1D4 (160). Own only their three assigned translation units; no header changes without coordination.

## Acceptance criteria

- [x] Reproduce T-20620's unintegrated EVENT recipe with current default actual C.
- [x] Brief source-driven passes on the other two targets after prior evidence.
- [x] Verify each promotion with literal default no-build proof, unit SHA1 and declaration guards.
- [ ] Independent review before Done.

## Notes

Preserve ancestry7da437a167ad. One Docker compiler; no blind sweeps. T-20620 identifies both the initial store and later selector read as byte views; a plain later read re-promotes the index. Mark scheduling FAKE explicitly.

## Comments

### First pinned wins

EVENT func_8010B614: default actual C literal MATCH100 bytes, EVENT SHA1 `ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e`. T-20620's both-byte-view recipe reproduces directly. A fresh canonical EventStateView Rec34 flag-member byte access differs26 rows, so the original opaque stride/index form remains explicitly FAKE marked alongside the constant/index byte lvalues.

BUNKA_SD func_801355AC: matched sibling T-22021's byte promotion and `*= 0` zero-retention recipe gives zero scratch rows and default actual C literal MATCH132 bytes. BUNKA_SD SHA1 `173ac942ad4efbbbb176f8522fc7d273540169b0`. Byte-view/zero-retention shaping marked FAKE. Keeps the compiling s32/implicit-int definition shape of the archived draft; no header change. One index loop clears64 real DisplayActor44 fade members.

Both production targets have no INCLUDE_ASM fallback. Affected builds pass with one compiler; headers/globals/prototype guards pass0 new/15 known. No compiler/config/header change. SHUGAKU remains under brief investigation. Independent review pending.

### Final short-target result

SHUGAKU func_8013A1D4 remains plain ASM. Ten bounded forms: generated control38 rows; removing redundant byte-result mask10; meaningful s32 girl before u8 result8/8 with original sp2B and frame. Only selector load/base, comparison register and missing delayed argument move remain. Byte view8, unsigned girl8, direct selector/full-word call result8; GameState pointer16; return-liveness forms10. No header or production SHUGAKU change. Source/reproducer and all bounded negatives preserved, instruction traces ignored. Total2 of3 matched,232 bytes. Root independently verified/published both matches at d40a1ece7 with all27 hashes and interface guards; no repeated checks for source-only closure. Ticket In Review pending archive review.
