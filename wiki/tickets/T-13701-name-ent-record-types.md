---
id: T-13701
title: Recover NAME_ENT record-rendering types
status: Done
assignee: codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[name-ent-family-results]]", "[[data-types]]"]
---

## Goal

Recover func_80139F30 record and actor accesses using existing NameMetricWork, NameRecordF00 and DisplayActor44 layouts, grounded in original strides and matching callers. Score a coherent full-TU typed draft before deciding on sibling transfer.

## Acceptance criteria

- Every recovered pointer stride and field corresponds to observed target accesses.
- Full-TU exact scoring; literal-byte proof and NAME_ENT SHA1 before accepting matching C.
- No arbitrary padding or forced registers; no production regressions.

## Notes

The work flags start at D_800E7D10+0x14; stored record flags at D_80123120+0x688 with record stride0xF00. Actor stride0x44; display status bytes +0x2201/+0x2203 correspond to actors128..135 fields1/3. Fixed actors136..140 correspond to flag order12,8,9,10,11. Existing matching func_80139CB4 and func_801324AC corroborate record and actor layouts.

## Comments

2026-10-17: Research ticket finished. The typed full-TU draft of NAME_ENT func_80139F30 scored 769 rows (actor endpoint 746), so no bytes were accepted; strides and fields are recorded in name-ent-family-results. Only wiki files were merged, so there is no code to review. T-17900 later redrafted all unmatched NAME_ENT functions with these types; func_80139F30 stays INCLUDE_ASM and unpackaged. Moved to Done.

## Result

Complete full-TU typed draft compiles, exact resolved score769 rows; actor endpoint746, flag-address endpoint989. Zero accepted bytes. Original loop/control-flow behavior and full type/field evidence are recorded in [[name-ent-family-results]]. No production C or header change. Readable distant draft and reproduction retained only in ignored build/name13700/. Documentation awaits review.
