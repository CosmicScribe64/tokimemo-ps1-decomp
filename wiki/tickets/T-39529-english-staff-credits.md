---
id: T-39529
title: Preserve the original ending staff credits
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39508-english-art-on-disc]]", "[[tickets/T-39510-no-japanese-left-check]]", "[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Preserve the original ending staff credits, as approved by the owner on 2026-11-02. This supersedes the initial translation plan.

## Acceptance criteria

- [x] Record the owner's approval and update the campaign's exception list.
- [x] Verify the published credit asset still matches the original disc bytes.
- [x] Preserve attribution, artwork and both staff-roll variants.
- [x] Review the documentation change against coding standards; no game changes are needed.

## Decision

The owner said the credits are fine to leave. `translations/en/graphics/decisions/staff-credits.json` records the approved exception and the original/published asset hash. The 167,936-byte `STAFF_R.NBN` file matches exactly. Its 164 credit frames were decoded for investigation, but no translations or pixel edits were applied.

## Evidence

The current public disc (`ca7ff1e93b03c8f7c0be9bebd61a02d6187cc439`) shows Japanese credits in `build/emu/ending-current-verified-entry/shot_5000.png`. The preceding English ending summary scrolls correctly. This discovery prompted the owner's approval to retain the credits. T-39500 now includes that exception.

`src/ovl/misc_scenes/staff_roll_asset_load.c` loads the credit texture pages from original LBA `0xA069`, count `0x52`, into `0x801D0000`. `staff_roll_credit_table` and the sprite renderer are in `src/ovl/misc_scenes/staff_roll_and_clear_save.c`. The alternate staff roll uses the same loaded credit pages. Font-upload tracing alone cannot certify these sprite-based credits.
