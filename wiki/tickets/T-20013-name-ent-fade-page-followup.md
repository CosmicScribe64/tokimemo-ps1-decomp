---
id: T-20013
title: Match NAME_ENT fade and character-page selection
status: Ready
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-20011-name-ent-coordinate-selection]]", "[[tickets/T-22008-w4-p08]]"]
---

## Goal

Continue func_80132320 (396 bytes) and func_80134248 (2,256 bytes) in src/ovl/NAME_ENT/80132000.c under the maintainer-confirmed claim #57. T-20011 now submits only the coordinate helper. These functions remain guarded/nonmatching; no new C bytes are claimed for them.

## Evidence

Ignored build/T-20011 contains full-unit controls: fade baseline7 aligned instruction edits (14 printed rows), character-page baseline227. Bounded 180-second/two-worker permuter searches do not find zero; the fade's stripped best70 is not a full-unit match. State switches, snapshots, shared row/column expressions and valid SpriteSlot24/GameState views do not improve the final full-unit baselines. Initial s32 fade probes with a void header were rejected; correcting both declarations still gives7 edits. Branch-only slot pointers that leave the final join uninitialized are discarded, as are destination-pointer controls that can evaluate an uninitialized destination on otherwise unused paths. None is eligible for promotion.

The coordinate result shows that real actor fields plus integer no-value exits can recover a register shape that explicit return values miss. Do not infer that the remaining source is compiler-limited from these negative controls. Preserve canonical headers until an actual matching view is measured.

## Acceptance criteria

- [ ] Recover and verify actual default linked C before promotion.
- [ ] Pass all 27 hashes and declaration/prototype guards.
- [ ] Review all required FAKE controls and C89/shared-type use.
