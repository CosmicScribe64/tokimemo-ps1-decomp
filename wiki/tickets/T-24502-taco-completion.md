---
id: T-24502
title: Match remaining TACO functions
status: In Progress
assignee: codex
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[playbook]]]
---

## Goal

Match remaining TACO functions using ordinary C and the existing Docker toolchain.

## Acceptance criteria

- [ ] Record function experiments and avoid repeated documented failures.
- [ ] Prove promoted C with literal default funcdiff and all 27 binary checksums.
- [ ] Review against CODING_STANDARDS.md before Done.

## Notes

Isolated branch codex/taco-24502 from 2c7293275. TACO files only.

## Comments

- 2026-10-20: claimed; inspect close func_8013B460 first, then tractable remaining functions.

- 2026-10-20: func_8013B460 comma-expression grouping leaves documented two moved stores unchanged; restored baseline. Fresh typed func_80140028 matches all 828 literal linked bytes in the default build. Parent review requested moving func_80140ADC's forward declaration to TACO.h; applied, full checks running. Details in [[matching-notes]].

- 2026-10-20 func_80140028 review: parent checked CODING_STANDARDS.md; sole finding was the local forward declaration, resolved by declaring func_80140ADC once in TACO.h. Docker verification after that change: default funcdiff MATCH (828 linked bytes), 27/27 independently recomputed binary SHA1 checks, headers OK, globals OK, prototype branch check OK (0 new disagreements). Campaign remains In Progress for additional TACO functions.

- 2026-10-20: func_8014FFDC, largest remaining TACO function, matched in default C (2,292 literal linked bytes). Parent reviewed focused source and accepted the explicit random-call order and FAKE source grouping; no blocking findings. After the approved own header declaration: all 27 binary hashes OK, header/global guards pass through ninja, prototype branch check reports 0 new disagreements. See [[matching-notes]]. Continuing with stale typed drafts of larger remaining functions.

- 2026-10-20 checkpoints: typed drafts of func_80145650 (32-byte copy-register selection), func_8013EAA4 (projection induction/spill allocation), func_8013FBF0 (color multiply destinations) saved under drafts/T-24502/TACO with exact bounded experiments. All remain production asm. See [[matching-notes]].

- 2026-10-20: func_8013F468 matches880 literal linked bytes in default C. Parent reviewed SDK types, bounded point indexes and source-local FAKE frame/group controls; no blockers. Docker all27 SHA1 checks, headers/globals through ninja and prototype branch check pass. Campaign total3 functions4000 bytes; remaining probes continue.

- 2026-10-20: func_8014CB24 matches 1,756 default linked bytes; all27 binary hashes, ninja header/global guards and prototype branch check pass. func_8014AB50 repaired draft is guarded, seven constant-register instruction words (28 bytes) differ after the final causal zero-chain check; default fallback still matches. Own declarations added to TACO.h with parent approval. Parent source review pending. Campaign total4 functions5756 bytes; see [[matching-notes]].

- 2026-10-20 review: parent checked CB24/AB50 and own TACO.h declarations against CODING_STANDARDS.md; no blocking findings. CB24 retains original implicit-int s32 return view with bare early return/fallthrough, already literal-proven with its linked caller. Guard controls are source-local FAKE; default remains original. Continuing func_80144E3C (1888 bytes), a stale raw Gouraud quad draft; first fresh POLY_G4 pointer-array compile succeeds, cost600 and464 vs472 words.

- 2026-10-20: func_80144E3C matched1888 literal default bytes using POLY_G4 array and six byte channels; u8 arg4 is proven by original entry caller-home lbu. Full27 hashes cover all callers; ninja guards and prototype branch check pass. Parent code review has no blocking findings; source-local FAKE pad/order and two loop groupings documented. Campaign total5 functions7644 bytes.

- 2026-10-20: archived typed BAC4 collision (cost330,456/460 words) and C6D4 pickup (cost251,348/347) drafts under drafts/T-24502/TACO. Broad remaining allocation/loop gaps documented, production stays asm; no repeated grids. Next A790 scrolling/table loops.

- 2026-10-20 archive review: parent standards10 finding on raw comparison dump resolved by deleting it and adding small context wrapper plus exact replay commands; concise first-divergence prose retained. Independent AB50 consult confirms priority0.5 constant reuse and no source fix; chained stores worsen469 to471 instructions. No further AB50 grid.

- 2026-10-20: func_8013A790 matched1492 literal default bytes; accurate AD64 declaration and callers covered by27/27 hashes, ninja guards,0 new prototype disagreements. Parent no-blocker standards review recorded. Read-only trace explains count/bound priority swap, resolved with source-local FAKE zero-subtree use; frame pad and sampler spill view also marked. Campaign total6 functions9136 bytes. Work now on clean ancestry codex/taco-24502-clean after reviewed source-only archive7bac2d12b.

- 2026-10-20: archived typed font39424 (cost513,404/419 words), broad entry promotion and coordinate scheduling gap; default stays asm. Proceeding53B38 fan generator using recorded in-loop color source form, not repeating prior parameter/decode grids.

- 2026-10-20: reviewed53B38 guard retained at26 aligned diff lines (12 target/14 built; two extra real words), default832-byte MATCH/all27 hashes. SDK finding resolved by documented preserved func_8009F004 declaration in libgpu.h; guard output unchanged,518-target rebuild and0 new proto disagreements pass. Negative narrow consult recorded in packet. Focused donor check found no >=0.70 twin forDEE4/39FB8/45650; semantic E778 donor improves DEE4 to108cost with original frame/homes, continuing privately.

- 2026-10-20: DEE4 donor packet frozen at70cost (334 target/335 draft real instructions), exact frame/geometry homes and now exact saved-register allocation. Counter homes/zero preheader/latch remain; no production guard. Private old108 trace superseded by current inner-for source. Concise archived replay available for bounded consult; all default binaries unchanged.

- 2026-10-20 resumed: root assigns EAA4 coordinate-loop reconstruction. Original middle inner loop counts pixels0..512 by256; original third outer loop ends on Y512, whereas archived scalar-index loops induce different counters. Test one causal change at a time in the full TU, preserving point offsets and signed widths. Prior scalar-splitting/pointer-walk negatives and DEE4 consult are excluded; production stays original until literal proof.

- 2026-10-20: final DEE4 initializer-only probes do not improve70; parent is assigning an independent frame/counter consult. Continuing39FB8 with corrected byte-table access and original negation shape. Old shared-variable/index grids excluded; a bounded source lifetime probe improves scratch implicit-int view264 to252, still broad register allocation mismatch and production unchanged.

- 2026-10-20: independent DEE4 consult negative; scope correction recorded: standalone70/tail alignment does not transfer to full TU69 with additional allocation differences.39FB8 lifetime work frozen at full-TU198 (standalone200), versus full-TU280 baseline; accurate allocator cause and weaker shared-web behavior documented, source-only replay packet saved. No production/header changes, no new match, review requested.

- 2026-10-20 EAA4 handoff: preserved full-TU control121/488 words, target491 including4 trailing nops. Real row-coordinate use plus separate sine height/cosine width recover middle loop counters/registers; final Y512 endpoint recovered. Candidate491 words, aligned cost186,192 positional rows/96 words differ:84 sp-based pairs plus12 others (first-grid init swap2, first CLUT registers8, final IV arithmetic order2). Exact tested source, full-TU replay, real-size/lifetime home map and bounded negatives saved beside control in drafts/T-24502/TACO/func_8013EAA4. No new compiles merely for handoff, no padding/zero controls, production/header unchanged; narrow consultation next.
