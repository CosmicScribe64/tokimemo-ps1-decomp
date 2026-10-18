---
id: T-24060
title: Recover two SHUGAKU short selectors
status: In Review
assignee: codex/complete-option
created: 2026-10-18
updated: 2026-10-18
links: ["[[shugaku-family-results]]"]
---

## Goal

Match func_801392B4 (272 bytes) and func_801398E8 (268), owning src/ovl/SHUGAKU/80138A60.c only. Preservece196db90 ancestry. Current types and matched source recipes first; one Docker compiler, coordinate headers, no broad sweep.

## Acceptance criteria

- [x] Refresh historical T-14500/T-18000 controls and T-10310 negatives with current types.
- [ ] Default actual C proof, unit SHA and guards for any promoted function.
- [x] Preserve useful current source/reproducer and bounded negatives if unmatched.
- [ ] Independent standards review before Done.

## Notes

Prior398E8 source reported58/71 rows, selector v1 versus generated v0/t6. Both old archives use deprecated main scalar views; typed T18000 bodies recover canonical fields but add generated temporaries. Current TU's matched selector39054 reuses one byte variable across the two call ranges. The target398E8 also calls a later void-defined helper399F4; scratch forward declaration is required before judging codegen, with production header coordination only if a match needs it.

## Comments

### Bounded result

Zero of2 matches,0 C bytes; production/header/config unchanged. Phase typed67, natural63, corrected phantom check_para_limit argument56, meaningful word next-state/reordered phase load52/52; structured final source remains52. Selector typed75, natural65, same-TU shared byte-range62/58; initial choice-v1/copy and sp2B now correct, girl register/load order differ. No close census sibling above0.70. All bounded negative scores, missing scratch-declaration failure repair and source-only reproduction in drafts/T-24060/README.md. Selected sources use canonical fields, no new FAKE; rejected byte lvalues marked and retained ignored. Valid successful fallthrough replaces generated return-of-void expressions.

Baseline SHUGAKU hash `e9c64e952a1ef5dfea5e9b3e6a659aa6ccb8eb8c` and one-job unit build pass. No actual C proof or fallback coverage claim. Interface unchanged; no full rebuild for source-only archive. In Review pending independent standards/archive review.
