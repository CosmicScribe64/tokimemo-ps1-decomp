---
id: T-39527
title: Accept dropped disc images and archives in the browser patcher
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39512-browser-patcher]]", "[[tickets/T-39526-publish-review-patcher]]"]
---

## Goal

The owner wants to drag a disc image or archive onto the page, matching the decomp extractor's supported formats. Find and unpack the tracks locally, with file selection available as a keyboard-accessible alternative.

## Acceptance criteria

- [x] Accept BIN/CUE, IMG, ISO, CHD, ZIP and 7z inputs, including nested archive paths and combined-track images.
- [x] Detect missing audio or discarded sector data without producing an incomplete disc.
- [x] Keep processing local, show progress and preserve cancellation and integrity checks.
- [x] Verify real-disc conversions and browser drag/drop, selection, failures and responsive layout.
- [x] Review public copy with Deslop and Humanizer; publish only verified format support.

## Evidence

`tools/prepare_disc.py` accepts `.cue`, `.chd`, `.iso`, `.zip`, `.7z`, `.bin` and `.img`. The extractor can skip CD-DA/XA streams because it needs the executable and files. The patcher must preserve the complete playable disc, so accepting a cooked image requires checking whether necessary data is present.

The public page now accepts the verified disc-image and archive inputs.

## Implementation checks (2026-11-02)

The candidate has a multiple-file picker and drop area, with separate BIN fields retained. Its input worker reads CUE layouts and raw combined images, extracts ZIP/7z archives with 7z-wasm, and converts CHD with chdman. Extracted audio gets its own download and a matching CUE filename. All tracks still pass the existing size and SHA-1 checks before patching.

Real-disc ZIP, 7z and CHD fixtures each reconstructed source SHA-1 `00b595095e26108d0e07d76492a11aaf3f71ccf1` and audio SHA-1 `6cdd8500168ab4ecb18b78ef7589586d26bc4501` in Chromium. Logs: `build/patcher-{zip,7z,chd}-test.log`. The existing nine browser checks also passed on the candidate (`build/patcher-drag-browser.log`). The complete flow and visual results are recorded below.

The upstream chdman WASM has a fixed 259-page heap and failed CD extraction. Increasing only its memory limits to 2048 pages passed; `tools/resize_chd_memory.py` reproduces the change against the pinned input hash. Vendor directories include licenses, provenance and corresponding sources. The MAME source archive omits the unrelated BGFX renderer.

## Review

Checked the input parser, worker lifecycle, DOM writes, download filenames, package contents and tests against `CODING_STANDARDS.md`. Game code and raw disc sources are unchanged. Browser processing makes only same-origin GET requests. The decoder heap adjustment was reproduced byte-for-byte from its pinned input. No unresolved code findings.

The final package passed all 15 browser checks, including a malformed ZIP, real dropped ZIP, cancellation during extraction, both download hashes, CUE filenames and responsive layout. Separate real-disc tests passed for 7z, CHD, CHD inside ZIP and a combined raw ISO. The synthetic resolver suite checks CUE positions, nested/case-insensitive paths, ambiguous inputs, missing audio and cooked ISO rejection. Evidence is under `build/patcher-*-final.log`, `build/patcher-formats-browser.log` and `build/patcher-test/result.json`.

Opened and inspected `build/patcher-test/desktop.png` and `build/patcher-test/archive-mobile.png`. The drop area, instructions and download controls fit without clipping or horizontal overflow. The packaged game remains the previously verified `2f9fd663374c4e7aa55c4f46e3690be28624f806` review build.

## Publication

Published at https://cosmicscribe64.github.io/tokimemo-ps1-decomp/ from Pages commit `7cec4eb7c89d73591475cadf9ccbfefa48f63810`. Workflow `41525374227` succeeded. All 36 served files matched the tested package byte-for-byte, including decoders, patch, licenses and source archives. The live CHD worker independently recovered both original track hashes. Deslop and Humanizer review found no prose changes needed; upstream licenses and generated code were preserved. Receipt: `translations/en/reviews/browser-inputs-2026-11-02.json`.
