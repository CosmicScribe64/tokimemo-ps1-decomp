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
