---
id: T-39528
title: Keep the patcher copy focused on players
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39527-patcher-disc-inputs]]"]
---

## Goal

Remove unnecessary development details and repetitive copy from the patcher page, as requested by the owner.

## Acceptance criteria

- [x] Keep disc compatibility, file selection, privacy and download instructions clear.
- [x] Remove build hashes and verification jargon from the player-facing flow.
- [x] Review with Deslop and Humanizer, inspect desktop/mobile rendering and verify patching still works.
- [x] Publish the reviewed page.

## Review

Removed build IDs, the SHA-1 details panel, repeated verification copy, decorative step numbers and decoder-specific footer wording. Shortened file-format help and progress/error messages. Kept the supported disc edition, privacy note, incomplete-ISO limitation, credits/licenses and download instructions. Integrity checks still run before any download is offered.

All 15 browser acceptance checks pass with the current game patch (`ca7ff1e93b03c8f7c0be9bebd61a02d6187cc439`), including real dropped ZIP input, cancellation and exact downloaded track hashes. Opened the desktop and mobile result screenshots: the copy and controls fit without clipping or horizontal overflow. Local preview: http://localhost:8766/.

Reviewed the HTML and JavaScript changes against `CODING_STANDARDS.md`. Only display text and removed display bindings changed; decoding, hashing, cancellation and file handling remain intact. No unresolved findings. Deslop and Humanizer review covers the exact staged copy; game wording and upstream license text are unchanged.

## Publication

Pages commit `e0e00a9d3375b5279500d364dce49880054ec41e` is live; workflow `41528238761` succeeded. All 36 served files match the tested package. The game patch itself remains `ca7ff1e93b03c8f7c0be9bebd61a02d6187cc439`. Receipt: `translations/en/reviews/patcher-player-copy-2026-11-02.json`.
