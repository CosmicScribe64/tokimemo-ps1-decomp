---
id: T-39529
title: Translate the ending staff credits
status: In Progress
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39508-english-art-on-disc]]", "[[tickets/T-39510-no-japanese-left-check]]", "[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Replace the Japanese ending staff-credit text while preserving the original credits, presentation and attribution.

## Acceptance criteria

- [ ] Inventory all credit headings and names, including both staff-roll variants.
- [ ] Verify name romanizations from reliable sources; do not guess readings.
- [ ] Render English credits with legible spacing and faithful colors and timing.
- [ ] Verify actual-disc assets and both credit variants in the emulator.
- [ ] Review the change against coding standards and preserve the matching default build.

## Evidence

The current public disc (`ca7ff1e93b03c8f7c0be9bebd61a02d6187cc439`) shows Japanese credits in `build/emu/ending-current-verified-entry/shot_5000.png`. The preceding English ending summary scrolls correctly. Ending credits are not among the approved exceptions in T-39500; its separate opening-movie exclusion does not cover this screen.

`src/ovl/misc_scenes/staff_roll_asset_load.c` loads the credit texture pages from original LBA `0xA069`, count `0x52`, into `0x801D0000`. `staff_roll_credit_table` and the sprite renderer are in `src/ovl/misc_scenes/staff_roll_and_clear_save.c`. The alternate staff roll uses the same loaded credit pages. Font-upload tracing alone cannot certify these sprite-based credits.
