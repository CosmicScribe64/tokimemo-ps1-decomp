---
id: T-24078
title: Recover ETC renderer with actual two-slot polygon buffer
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8013D570 (2352 bytes), src/ovl/ETC/8013BEF0.c. Preserve matched D32C/BFFC/C8EC and every other body. Current primary ASM/no active claim or later T-240xx target packet found.

## Acceptance criteria
- Read latest primary wiki/history and original assembly; old plain792/typed957 rows are historical, not a near-zero promise. Fresh valid full-TU SDK baseline first.
- Replace old scalar sp84 buffer with actual POLY_FT4*poly[2]: original frame0x90/s0=sp84, four-byte step and two iterations prove slots84/88. Verify full target addresses before source changes. Use real GameState.unk_018 frame-buffer indexing, not aggregate+frame*2 scaling.
- Matched same-TU D32C/T-22019 supplies real two-polygon SDK initialization/AddPrim idiom. Transfer target semantics one change at a time, not old scalar/register casts.
- Same sole Sol medium Docker compiler/ninja -j1, built checkout fromeff5d1b96 on codex/complete-etc-two-poly-renderer; no extra agents/instrumentation or broad sweeps.
- No assumed shared headers/config changes; coordinate exact evidence first. Actual C needs default literal2352-byte proof, ETC hash/interfaces and unchanged-neighbor audit; parent reviews/all27. Archive replay must work after any promotion.

## Notes
Both retained drafts use a singleton pointer despite two-slot accesses; typed fixed pointer step to one but not buffer extent, plain step4 scales sixteen bytes. Correcting actual layout is new faithful reconstruction, not a compiler workaround. Preserve exact SDK field widths/order and original two-iteration/draw-buffer semantics. T-24077 remains unresolved In Review with probes stopped.

Original full VRAM8013D5BC sets cursor tosp84, 8013DE64 steps4bytes and
8013DE4C/DE68 enforce two iterations, proving slots84/88 in frame90. Fresh
baseline repairs singleton to real POLY_FT4*poly[2], typed pointer step1, all16
GameState.unk_018 frame-index reads and exact two-argument AddPrim byte-ordering
table address. Old plain792/typed957 remain history. No production/header change.

Fresh valid current baseline800/frameC0; joining12 disjoint same-type aliases
preserves expressions/store order and reduces frame to98 with800 rows. Direct
SDK UV assignments remove inherited signed-byte casts, restore word loads and
reach790/frame98. Source-only controls/replay are drafts/T-24078; production,
headers/config and helpers remain unchanged. Original frame90; recovery active,
no match or linked C proof claimed. Initial field-substitution syntax error was
repaired before scoring and is not a baseline result.

Metric correction: printed +/- rows are diagnostic only, with different equal-
length alignment rules. Existing tools/funcdiff.py diff_score measures aligned
resolved-key cost. Original588words/frame90: baseline638cost/800rows/594words/C0;
joined638/800/594/98 is retained; scalar SDK UV678/790/582/98 is WORSE despite
fewer rows. UVpair638/800/596/98 restores LW/saved136 but adds2words; per-case
height638/800/596/A8, inlineheight678/790/582/98, namedwidth648/806/594/A0 are
rejected. drafts/T-24078 replay now prints alignedcost/words/frame evidence.
No supported form simultaneously restores original stackUV50/54, saved136/320
and DIV-before-MULTU scheduling. Active In Progress pending focused direction;
no further unsupported probes, no match/impossibility claim. Production ASM.

Parent-authorized bounded Astra read-only review returned one source-backed
D32C transfer: derive pageX as0x140 + var_s4*0x80 and remove only independent
var_fp declaration/init/increment. Single indexed-page control neutral638cost/
800rows/594words/frame98. Compiler reconstructs320 induction but spills atsp50
rather thanbaseline60; no targetfp320/frame90 gain. drafts/T-24078/indexed-page.c
and replay preserve exact source; measured ignored build/etc24078/indexed-page.
Astra idle, no further unsupported control/instrumentation; still active pending
direction, production unchanged.
