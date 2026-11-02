---
id: T-39527
title: Accept dropped disc images and archives in the browser patcher
status: In Progress
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39512-browser-patcher]]", "[[tickets/T-39526-publish-review-patcher]]"]
---

## Goal

The owner wants to drag a disc image or archive onto the page, matching the decomp extractor's supported formats. Find and unpack the tracks locally, with file selection available as a keyboard-accessible alternative.

## Acceptance criteria

- [ ] Accept BIN/CUE, IMG, ISO, CHD, ZIP and 7z inputs, including nested archive paths and combined-track images.
- [ ] Detect missing audio or discarded sector data without producing an incomplete disc.
- [ ] Keep processing local, show progress and preserve cancellation and integrity checks.
- [ ] Verify real-disc conversions and browser drag/drop, selection, failures and responsive layout.
- [ ] Review public copy with Deslop and Humanizer; publish only verified format support.

## Evidence

`tools/prepare_disc.py` accepts `.cue`, `.chd`, `.iso`, `.zip`, `.7z`, `.bin` and `.img`. The extractor can skip CD-DA/XA streams because it needs the executable and files. The patcher must preserve the complete playable disc, so accepting a cooked image requires checking whether necessary data is present.

The current public review page accepts two raw BIN tracks. Expanded format support is being developed and is not yet published.
