---
id: T-39512
title: Build and verify the local browser patcher
status: In Review
assignee: codex
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

A static browser patcher for the original PlayStation the Best track, suitable for the planned GitHub Pages release. Disc bytes stay local. Produce a verified patched data track and a matching two-track cue, retaining the user's audio track.

## Acceptance criteria

- [x] Local file inputs, source identity check, patch integrity and output hash verification, progress and cancellation, useful failures.
- [x] Actual current xdelta applied in a headless browser; output matches the command-line build.
- [x] Desktop and narrow-screen visual review, keyboard-accessible controls.
- [x] Reproducible packaging contains no copyrighted input or output disc data.
- [x] Review against CODING_STANDARDS.md before Done. Publication awaits campaign owner sign-off.

## Notes

Vendor the MIT-licensed RomPatcher.js VCDIFF reader at an immutable revision, retaining licenses. All dependencies and tests run in Docker. Use a worker so patching does not freeze the page.

2026-11-01: `build/patcher-test/result.json` proves actual-disc Chromium patch/download SHA-1 equals the CLI output, exact CUE contents, wrong size, same-size wrong disc, corrupted patch and cancellation handling, and no upload/external requests. Desktop, 390-pixel and success screenshots reviewed. Keyboard traversal reaches both file controls and the submit button in order. `build/patcher-review` contains only static sources, vendored decoder/license, xdelta and hashes. Code reviewed against CODING_STANDARDS.md tooling/distribution rules: Docker-only dependencies, no raw game data, explicit integrity gates and bounded scope. Review caught stale-worker callbacks and case-insensitive output/audio filename collision; both corrected. Final browser recheck pending before moving to In Review.

Final recheck passed after the review fixes; moved to In Review. Added the Apache-2.0 and Victor Alexiev MIT license texts credited within the vendored module, retaining all upstream notices. Local page is ready for owner review; no public deployment made.
