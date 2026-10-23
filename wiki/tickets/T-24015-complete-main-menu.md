---
id: T-24015
title: Complete MAIN menu family package
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-20
links: ["[[main-family-results]]"]
---

## Goal

Recover verified C in src/main/8005A0B0.c: func_8005CA94, func_8005CC40, func_8005B43C, func_8005A410, func_8005F730.

## Acceptance criteria

- [x] No production C recovered; baseline MAIN checksum passes. Literal win proof not applicable.
- [x] Header/global/prototype checks pass.
- [x] Independent review.

## Notes

Bounded types/sibling pass from unstarted w4-p41. Read T-17000, T-16000 and T-16510 recipes before probes. Reuse clean worktree; only this MAIN C file is owned; one compiler/ninja -j1.

## Comments

2026-10-17: Consulted wiki index and main-family results. Archived drafts include lossy aggregate folds and incomplete menu-row typing; repair those before allocation work.

## Bounded result

Zero of five matched; no production C or header changed, zero verified bytes added. All five remain ASM. Current resolved rows/aligned: CA94 57/57, CC40 118/114, B43C 112/112, A410 41/41, F730 113/111.

A410 is the strongest new lead: signed coordinate halfwords plus a same-array 64-byte end bound remove IDO alignment peeling and recover frame/spill offsets. End-label address sharing, counter allocation and scheduling remain. The form visits sixteen coordinate pairs, two per iteration; an unrelated end-label bound scores 187 rows. This is a concrete retry lead, not proof of a match or compiler limitation.

CA94 repairs the lossy old GameState fold to unk_1118 and uses byte strides through Work800E7D10; selected-index spill remains four bytes high and narrowing/copy allocation differs. CC40 repairs GameState fields, byte strides and unsigned timer comparisons. Its D_800B39C4 table appears to hold two words per index, still an unresolved scratch scalar extern. Its s32 fallthrough definition preserves the existing implicit caller view; a void signature needs a deliberate canonical prototype decision before promotion.

B43C uses Rec38 unk_0C.f.b2 instead of a word mask; original frame and input-nibble allocation differ. F730 follows matched F26C/FDEC siblings with five-row halfword arrays. D_800E67C4 maps to GameState unk_53C[8]. Frame matches but all four array homes are eight bytes high. Coordinate variable names are inherited from T-16510; original menu_set order and values are retained. No FAKE padding was added to selected forms; a rejected padding experiment is preserved only in the ignored probe archive.

Baseline MAIN SHA-1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 matches config/SLPM_86.053.sha1. Ninja -j1 build/SLPM_86.053.ok was current. Headers/globals pass; prototype branch check reports zero new disagreements, fifteen known. Literal no-build win proof is not applicable because no function was promoted.

Selected source-only forms: drafts/T-24015/. All loose experiments preserved in ignored build/T-24015-probes/.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
