---
id: T-39512
title: Build and verify the local browser patcher
status: In Review
assignee: codex
created: 2026-11-01
updated: 2026-11-02
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

2026-11-02: repackaged the latest banner build at `build/patcher-banner-review`; all nine browser checks pass again against target `a0098b5c0568ebfe46f67cb3324ebb236d08b047`. The actual browser download matches the command-line disc and CUE. Desktop completion screenshot reviewed at `build/patcher-test/complete.png`; previous proof preserved in `build/patcher-layout-proof`. Local review server moved to localhost:8767; publication remains pending campaign completion and owner review.

2026-11-02: current `build/patcher-still-review` passes all nine acceptance checks against e00ffdf0b44b94ad97168b425c1ce564dad04b9f. Desktop completion and 390-pixel screenshots reviewed. Actual browser-downloaded disc and CUE are in `build/browser-still-disc`, with unchanged original audio; an input-only emulator slice through the first school day is running. The live localhost:8767 preview still serves the earlier banner review, not this newer package.

The current local review package is now served at http://localhost:8768 (Docker server); localhost:8767 remains the historical banner package. No public publication.

Browser-download emulator verification completed: `build/emu/route-en-browser-slice` boots through the first school day in 24,063 frames / 555 s with 82 screenshots and zero original-font uploads, unknown mappings or overflow. `cue-provenance.json` binds the run to the actual downloaded files. This closes the campaign's browser-built playable-slice evidence requirement, not remaining whole-game language coverage.
