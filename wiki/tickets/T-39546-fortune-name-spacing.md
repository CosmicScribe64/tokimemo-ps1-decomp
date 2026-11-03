---
id: T-39546
title: Separate full names in the computer fortune display
status: Done
assignee: codex
created: 2026-11-03
updated: 2026-11-03
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Keep a visible gap between family and given names in the festival computer-fortune display. Its original 56-pixel column pitch leaves only one pixel between the widest presets. Both editable English name fields allow up to 56 pixels.

## Acceptance criteria

- [x] Leave at least four pixels between full-width name fields and keep the blood-type label separate.
- [x] Inspect a native capture with the widest presets and verify actual rendered coordinates.
- [x] Preserve the original build's 27 hashes; pass disc and browser checks and publish.
- [x] Complete a standards review.

## Evidence

`build/emu/fortune-name-columns-corrected/shot_240.png` shows the longest presets running together. This controlled fixture calls the actual fortune handler but retains the bedroom backdrop and an earlier dialogue row. It verifies the added fortune rows only. An earlier fixture wrote the wrong GameState offsets and is rejected as name-width evidence; the corrected fixture uses the declared name offsets 0xD4 and 0xDC.

## Verification

The rebuilt native captures show a five-pixel gap for the widest presets and four pixels for two valid 56-pixel names. Both rows place the blood label at x=0 and its value at x=40. The maximum given name ends at x=-12, leaving twelve pixels before the label. Actual text-entry memory samples establish these coordinates; both captures were visually inspected.

All 27 default hashes match. Script, executable and generated-source audits pass, as do all 15 browser checks. The remaining 12 fortune-name entries and 12 RPG loot entries also fit their measured bounds. Receipt: `translations/en/reviews/fortune-name-spacing-2026-11-03.json`.

Standards review found no unresolved issue. The four coordinate changes are English-only, and the original constants remain in the default build. No wording, artwork or gameplay logic changed. Published target `45eac0efcc5981d60102a9de7e84e30757243b4c`. Pages run 41606770799 succeeded; all 37 served files match the tested package byte for byte. Publication receipt: `translations/en/reviews/published-fortune-spacing-2026-11-03.json`.
