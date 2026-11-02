---
id: T-39524
title: Avoid orphaned words in assembled phone sentences
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Keep a short final word with the preceding word when runtime wrapping splits an assembled telephone sentence. Preserve explicit paragraph breaks and the existing width limits.

## Acceptance criteria

- [x] Correct the isolated final word in current route capture 100.
- [x] Test width limits, substitutions, explicit breaks and the six-row limit.
- [x] Inspect the corrected sentence in the game.
- [x] Complete regression and standards review.

## Evidence

Current candidate `9935d1aacd90` shows a one-word final row in `build/visual-quality-review/speaker-gate-perspective/shot_100.png`. The date and destination are assembled after build-time layout, so `english_phone_wrap` handles this sentence. Its greedy wrap does not balance the final row. The speaker color and closing quote remain correct.

## Verification

The wrapper moves the previous word onto a short final row only when the break was inserted at runtime, the previous row retains text, and the resulting row fits. It includes the existing 116-pixel name reservation. Explicit paragraphs and row count stay unchanged.

C89 tests cover movable orphans, explicit breaks, name substitution, the six-row cap and words that cannot fit together. The affected suites pass 41 tests with one opt-in skip. Standards review checked the bounded scans and English-only compilation; no unresolved issue remains.

Candidate `2f9fd663374c4e7aa55c4f46e3690be28624f806` built in 168.4 seconds. All 252 art checks, both disc text audits and browser checks pass. All 27 original binaries match. Browser evidence is preserved in `build/patcher-phone-orphan-test-evidence` and `build/phone-orphan-browser.log`.

The voice-enabled route completed 33,572 frames and 134 captures through date and save/load, with zero original-font, unknown or overflow calls. Inspected the repaired phone page and later farewell, save-confirmation and loaded-state captures. Full names and closing marks remain intact. `build/visual-quality-review/phone-orphan/shot_100.png` shows the final two-word farewell together. Capture hashes are in `translations/en/reviews/visual-quality-2026-11-02.json`.

T-39525 tracks the separate build-time boundary changes.
