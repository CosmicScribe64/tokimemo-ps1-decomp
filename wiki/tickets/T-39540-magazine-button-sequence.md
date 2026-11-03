---
id: T-39540
title: Keep the magazine button sequence on one row
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

The May 1997 magazine hint splits the ten-button sequence across two rows, leaving only its last two buttons on the final row. Preserve the wording and place the complete sequence together.

## Acceptance criteria

- [x] An exact-record layout correction preserves every word and button in order.
- [x] Production-wrapper checks and a magazine GPU capture show the complete sequence on one row.
- [x] The rebuilt disc passes text audits and the browser patcher checks before publication.
- [x] Standards review passes.

## Evidence

The preceding release payload measures 368 pixels before wrapping. The production wrapper with release metrics produces rows of 171, 169 and 20 pixels; the last row contains only the final two buttons and closing mark. Other wide menu candidates fit in two or three rows without clipping. This is a line-break correction, not a translation rewrite.

## Verification and review

The May 1997 magazine selects this record through `D_800B66A4[9]`. A controlled emulator fixture shows the complete sequence on one row in `build/emu/magazine-final-sequence/shot_1350.png`. The new row widths are 171, 89 and 100 pixels. All 15 wide gameplay strings pass the production wrapper with release font advances; two other wide strings are SDK diagnostics.

The production build and both disc text audits pass: 105 script containers, 31,471 strings and 31,334 pointers; 2,940 executable slots with no pending bindings. All 15 browser acceptance checks pass, including archive drag-and-drop and downloaded hashes. Comparing 1,371 raw file extents finds only the executable changed; its payload differs by one space replaced with a newline. Audio is unchanged. Receipt: `translations/en/reviews/magazine-sequence-2026-11-02.json`.

Reviewed against CODING_STANDARDS: the Python-only correction is keyed to one record and fails if its expected payload changes. It preserves wording, length and button order. No C, original sources, assets or unrelated records change. The existing 31 text tests and 21 route tests pass. No review findings remain.

Published to GitHub Pages in `bd4a52c3c3e7e82c073e45ab00353a14071690ac`; deployment 41590884305 succeeded. All 37 served files (including `.nojekyll`) match the verified package. Receipt: `translations/en/reviews/published-magazine-sequence-2026-11-02.json`.
