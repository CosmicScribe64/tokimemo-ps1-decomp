---
id: T-39519
title: Separate new-album choices from their button symbols
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[tickets/T-39508-english-art-on-disc]]"]
---

## Goal

Make the new-album confirmation row read as two clear label/button pairs. The owner explicitly requested direct visual review throughout the game, including asset quality and awkward formatting.

## Acceptance criteria

- [x] Separate Yes/No labels align with the corresponding circle/cross symbols; wording and controls preserved.
- [x] Fresh GPU capture verifies the corrected row and surrounding file-select layout.
- [x] Original build matches, focused checks pass, standards review resolves all findings.

## Evidence

Reviewed all 145 current main-route captures in 13 native-resolution sheets at `build/visual-quality-review/main-00.png` through `main-12.png`. Most dialogue/menu/date/save screens are readable and consistent. The new-album prompt in `route-en-ending-stream-full/014_file_select_new_album.png` has labels bunched left while symbols retain fixed positions. Original capture confirms two intended label/button groups. Pink flecks in the roster animation also occur in the original; not a patch regression. Phone captures with apparently unfinished quotes need timing/continuation verification before labeling them bugs. Additional event/art coverage remains required; this review does not certify the whole game.

## Plan

Use an English-only prompt wrapper, keeping the original function tokens unchanged. Draw the two labels separately at positions corresponding to the existing controller symbols. Rebuild and verify a fresh file-select route.

## Verification and review

English-only wrapper and entry hook preserve the original function tokens and input handler. Separate labels at x=-36 and x=48 sit before the existing circle/cross icons. Seven entries are displayed. Fresh power-on route `build/emu/route-en-album-prompt` completes 4,850 frames in 113 seconds, 14 captures, zero original-font calls/unknown/overflow. Capture 014 and `build/visual-quality-review/album-prompt-before-after.png` visually confirm the corrected spacing.

Complete build `build/album-prompt-proof` passes disc/art/xdelta gates: 248 verified art placements, 28 retained exceptions, patch 6,557,713 bytes, target prefix e083e202eb3c. All 27 original hashes match; headers, globals and 14,449 rename ledger rows pass. Standards self-review: C89, guarded implementation, original function unchanged, no raw assets staged, no input behavior change, text-entry count matches calls. No unresolved findings. In Progress -> In Review -> Done after this review. Broader visual review and phone punctuation remain under T-39511/T-39520.
