---
id: T-39017
title: Write source specs for opening stages 12-22
status: In Progress
assignee: Claude (fan-out worker)
created: 2026-10-29
updated: 2026-10-29
links: ["[[port-pause-2026-10-29]]", "[[port-adversarial-review]]"]
---

## Goal

Write precise, source-cited porting specs for each remaining opening mode stage (reads, copies, actors, tpage slots, audio, text lines, resources to export) so implementation can proceed quickly.

## Acceptance criteria

- [ ] Implementation follows original source with explicit rejection where unsupported; no synthetic readiness, injected state or fabricated fields.
- [ ] Docker tests pass, including negative cases; evidence limits stated.
- [ ] Independent adversarial review against CODING_STANDARDS.md with all findings resolved.

## Notes

Started in the 2026-10-29 fan-out; worked in an isolated worktree, integrated by root after review.

## Comments

- 2026-10-29: Wrote [[spec/opening-stages-21-end]]. It covers stage 21 `prologue_finish_run`, the START skip, and the handoff to mode 0x34 stage 0xFF and then stage 1. It includes the five spring/COM_MIX bank reads with their LBA-to-file mapping and sector over-reads, CALROM and `C_02_02H.BS`, the tpage release at `tpage_buf_clear`, and the port gaps: stub reload services, the stage 0 step 1 mismatch, the build-rule divergence, the unsourced XA stop, and the alternate bank triple accepted at stage 21. Source-only; no emulator trace yet.
- 2026-10-29: Wrote [[spec/opening-stages-12-14]], covering stage 12 `shiori_load`, stage 13 line 18 and stage 14 line 19. Read LBAs map to CUT_14, O_14, P_01_09, O_15, CUT_16 and O_16; P_01_06 is reused. The page also records the memcpy extents, the actor 96-99 field setups with their table pointers, the per-frame RNG ordering, the lip sync and two-clip line 19, the bg cache and show-switch state, and the tpage slots. The current port stage 11-14 bodies do not follow the source: wrong sector counts and destinations, invented fades, slot 0 for the background, and misplaced RNG. The exporter must add the six resources the page lists. The page also corrects two CUT file names in [[spec/o-nameent]] and flags two commit-frame blink edges, one of which must be explicitly rejected. Source-only; no emulator trace.

- 2026-10-29: Stage 15-17 spec written: [[spec/opening-stages-15-17]]. It records the source substeps, reads (LBA → disc file verified from the ISO directory in Docker), the memcpy, actor fields, tpage slot maps from the O_16/O_07 frame tables, bank start, XA clip words, and the line text and speakers. It also lists the port divergences: the stage 15 substep map, fabricated slide/hold steps, wrong read sizes, the stage 16 bg slot, the stage 17 O_13 read size and extra wait, 100/101 voiced as Shiori, and fabricated lip-sync fields. It ends with exporter additions (O_16/CUT_16, P_01_10, P_01_12, O_13, new CUT_07/O_07 tuples and the 0xC800 copy). It also notes that o-nameent §8 mislabels CUT_14/CUT_16 as CUT_13/CUT_14. Spec only; no code changed.

- 2026-10-29: Wrote [[spec/opening-stages-18-20]] for stages 18-20 (lines 38-41, the flash, closing line 42). It records per-substep source lines, the line 38/40 YOSHIO.XA windows, the O_13 actor 96 setup and sequence, the CLUT row 0x1FB dependency on stage 0, and the tpage slot order (5 then 6 from `STAGE10_FREE_TPAGE_SLOTS`). It lists seven port divergences in `prologue_arrival.gd` stages 18-20 (missing XA wait substep, flash timing, closing off-by-one, missing actor and draws) and the wrong stage 16/17 reads (slot 0, 33 sectors to 0x8018C800). Exporter additions: P_01_10, P_01_12 (10-sector transfers) and O_13 (0x41 sectors).
