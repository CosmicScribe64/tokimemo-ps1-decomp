---
id: T-12050
title: Match cx5 main menu and text functions
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal

Deep matching pass over fourteen listed functions in src/main/8004F870.c.

## Acceptance criteria

- [x] Exact linked and unit checksum proof for every match.
- [x] Guard only measured near misses within about thirty differing rows.
- [ ] Full build and prototype verification; independent standards review before Done.

## Notes

Owned source: src/main/8004F870.c. Main and other worktrees remain untouched. Cutoff 21:45 UTC.

## Comments

2026-10-15: Read index, project/local instructions, standards, workflow and register cheat sheet. Initial history search covers all fourteen names. Baseline configure plus ninja -j2 passes 27/27 SHA1, headers OK and globals OK. Known cases: menu_bar_color lacks same-object data ownership for shared lui; menu_set masked argument allocation; menu_check_1 record/base registers; menu_bar_show packed-word/record repair previously reached eight rows.

### menu_bar_show: MATCH, 208 bytes

Reproduced the prior T-10010 seed from its retained scratch declaration-order-probes/best.c, then replaced scalar-neighbor selection reads with MenuSelection5 and flat record indexing with the interleaved GsRecF90 pair view. Direct packed-global extraction into mode/page/flag locals avoids the scheduling and frame changes of an extra cached color or base local. Exact linked comparison and main executable SHA1 both pass. No FAKE.

### Initial deep-pass triage

All fourteen listed functions have now been compared or reconstructed and compiled. x_taku_menu_set reaches 24 differing rows, exclusively array homes eight bytes above target; argument style, scalar homes and aggregate variants do not close it. Larger repaired drafts: menu_girl_taku_set 58 rows, sndisp 77, xa_wait 93 initially (safe signed-byte variant under refinement), sndi 332, func_8005215C 335, func_80052F68 327, get_h_tokimeki 817, get_near_menu 1424 and func_8004FC10 752. Corrected saved-draft pointer strides, real buffers and canonical GameState fields. Narrowing F61+2 to u8 gives an attractive smaller score but changes semantics and is rejected. User steering makes verified bytes primary: current focus is the 1260-byte state machine and its 580-byte shared-prefix sibling, plus the 1200-byte score builder. No broad scratch drafts are retained in production.

## T-12050: cx5 menu/text deep pass

One exact match: `menu_bar_show`, 208 bytes, plain C without FAKE. Linked relocation-aware comparison and main SHA1 pass. The prior T-10010 seed needed the canonical `MenuSelection5` and interleaved `GsRecF90[16][2]` views; read the packed global directly into mode/page/flag locals. Caching a packed color or base pointer changed scheduling. Seed provenance: retained T-10010 scratch `main-menu_bar_show/declaration-order-probes/best.c` beneath session `43264b98-bb59-4c1b-ad99-a2084d55fcf0`.

All 14 functions attempted; 13 remain unmatched. Retained guards: `menu_bar_color` 17 differing listing rows (shared lui/data ownership), `menu_check_1` 27 (base/selector registers, inherited FAKE duplicate pointer), `x_taku_menu_set` 24 (three array homes eight bytes too high). Added canonical `s16 D_800B40F0[4]` declaration for its row table. Array dimensions, local order, ANSI/K&R signatures, unused padding and aggregate variants did not resolve the stack layout; no padding retained. Removed the inherited `menu_set` guard, now 102 rows. Its argument/loop experiments reached 101 safely; a 73-row unmasked argument variant changes semantics and is rejected.

Larger candidates stay in ignored scratch: `func_8005215C` (1260 bytes) 335→219 rows after reusing disjoint m2c temporaries; an empty-test FAKE reaches 195 but remains too broad. Regdiff shows field-load lifetimes and frame/register differences, not an isolated allocator fix. `xa_wait` (580 bytes) reaches 49 safe rows; narrowing `F61+2` to a byte reaches 44 but breaks values 256/257 and is rejected. `func_80052F68` (1200 bytes) stays 327 rows across bounded loop variants: target peels two then unrolls four, draft rolls. `get_h_tokimeki` (1600 text bytes plus 24 rodata) improves 817→637 using indexed flag stores; target rolls the initial seven flags and hoists seven threshold bytes, while the draft unrolls and reloads thresholds inside cases. Explicit bound locals and case-local value/weight pointers did not close it. These need structural/type evidence before further mutation sweeps.

Other repaired drafts: `menu_girl_taku_set` 58 rows (array homes +12 and flag-load scheduling), `sndisp` 77 (buffer homes, saved registers, forwarded first byte), `sndi` 332, `get_near_menu` 1424, `func_8004FC10` 752. Corrected m2c pointer strides, byte buffers and canonical fields before scoring. Broad drafts are not production C. The external AS1 scheduling research was read as a future lead; no uncalibrated tracing result is claimed here. Full-TU scoring and saved diffs are under `build/scratch` in cx5; traced uopt was copied from the parent-provided r9-uopt trace variant and used for state-machine/threshold diagnostics.

### Final verification

Fresh Docker configure and ninja -j2 pass all 27 executable/overlay SHA1 checks, headers and globals. sync_protos --check-branch reports zero new disagreements. Final linked menu_bar_show remains MATCH (208 bytes). All fourteen targets attempted, no unattempted targets; one match, three retained guards, ten plain assembly targets. No new FAKE retained; inherited duplicate-pointer FAKE remains labeled. Independent standards review remains required before Done.
