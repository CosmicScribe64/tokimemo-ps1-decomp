---
id: T-24044
title: TAIIKU and TACO sibling completion
status: In Review
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover TAIIKU func_8013B3B0 and TACO func_801424A8 from current matched siblings. Only the two assigned bodies; no shared-header change without parent notice.

## Acceptance criteria

- [x] No wins claimed; unchanged baseline unit checksums pass.
- [x] Useful failed source forms and exact prior evidence retained.
- [x] Header/global/prototype checks pass.
- [ ] Independent review.

## Notes

Read index, family results, T-16552/T-19008 counter and split-s16 assignment recipe, T-19024 renderer81-row negative and current matched siblings. Typed old TAIIKU draft contains wrong position fields/counter comparison; reconstruct from canonical types before scoring. One Docker compiler/ninja-j1, bounded25min.

## Bounded result

Zero of two matched, zero verified bytes added. No production C, header or compiler change; both targets remain ASM. Baseline one-job TAIIKU/TACO builds and unit SHA1 checks pass. Header/global/prototype checks pass,0 new/15 known disagreements. No default ASM control is claimed as recovered C.

TAIIKU func_8013B3B0: old typed archive has semantic defects: second threshold reads unk_00 instead of counter unk_02, both positions write unk_00, and y uses obsolete unk0. Fresh source fixes counter threshold, uses unk_04/unk_06 and ball unk_1C[1], with display record97 offsets0x18/0x2A. Current field form169 rows/165 aligned; matched-family unsigned counter/flag views164/160. Splitting the s16 result as in matchedB608, typed counter pointer and byte-address view stay164/160. Void with proper forward declaration162/158 is not a material improvement. Original repeatedly reloads the counter; current IDO promotes it into a register, changing entry and branch temporaries. No new volatile workaround or compiler-impossibility claim. Selected packed temporary and u16 views are FAKE marked, following T-19008.

TACO func_801424A8: current full-unit typed sibling control162/162, explicit meaningful output index k=j*2+o improves124/124. Initial matrix-copy temporary pool remains one register early, projection/view-index allocation differs. Pointer output forms218, byte offset175; u32/s16/u16 index206/214/217; parallel outer index163. Existing frame/spill layout recipe retained; new k is meaningful, not an unused pad. Original has a uopt reload in t3 at the outer-loop end, whereas current code does not reserve that register before matrix copy. Selected source needs scratch forward void func_801424A8(s32,s32) and extern u16 D_8015F590 for compilation; no canonical header change was made. Its inherited frame pads and single-line UV macro expansion are FAKE marked. Old T-19024's81-row report is a historical metric/context, not a fresh result.

Read index, taiiku/taco family results, T-16552/T-19008, T-19017/T-19024 and current matched siblings before work. Temporary prefix-context mistakes failed loudly and were repaired before measured controls. Selected sources are current-type unresolved forms; older archives untouched. All loose sources, input files, scripts and instruction diagnostics preserved intact in ignored build/T-24044-probes/.
