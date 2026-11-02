---
id: T-39523
title: Redraw the school gate plaque with clear lettering
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39508-english-art-on-disc]]", "[[tickets/T-39522-speaker-label-contrast]]"]
---

## Goal

Replace the cramped brush lettering on the opening school plaque with simpler lettering that fits its perspective and margins. The owner flagged clipped letters and an inappropriate horror-like appearance in the current candidate.

## Acceptance criteria

- [x] Remove the exaggerated brush treatment and keep clear letter spacing at native resolution.
- [x] Preserve the plaque, surrounding background and on-disc encoding constraints.
- [x] Inspect the encoded artwork and an actual opening-scene capture.
- [x] Complete regression and standards review.

## Evidence

The replaced `bs-p-01-01-gate-plate` crop used broad-nib brush strokes and exaggerated slant. The source is `CDROM/BS/P_01_01.BS`; the crop rectangle is (76, 68, 160, 82). The build now regenerates the crop from a source-hash-checked lettering plan, keeping generated pixels in the ignored cache.

The new lettering uses constant strokes and open spacing. A flat 150-by-70 layout is projected onto the plaque's four measured corners, as requested by the owner. The regression checks exact corner mapping and at least five pixels of glyph clearance from every edge. The obsolete angle-only placement helpers were removed.

The final disc (`build/speaker-gate-perspective-proof/disc`, target prefix `9935d1aacd90`) passes all 252 art checks. The plaque remains 18,432 bytes, with 33 edited macroblocks. The maximum change to surrounding pixels is two RGB555 steps, within the unchanged limit of four; mean error on edited pixels is 0.3805. The final generator reproduces the built crop exactly.

Inspected the decoded disc image and fresh opening capture in `build/visual-quality-review/speaker-gate-perspective/encoded-sign.png` and `shot_032.png`. Letters follow the plaque perspective, remain separated and stay within its edges. The surrounding background is intact.

## Review

The combined affected suite passes 109 tests with two opt-in skips. Both disc text audits and all nine browser checks pass. Standards review checked the source identity guard, exactly-one-crop replacement, projective geometry, pixel margins, unchanged encoding limits and exclusion of generated game images from Git. No unresolved code finding remains. T-39522 separately tracks the remaining dialogue-route review.
