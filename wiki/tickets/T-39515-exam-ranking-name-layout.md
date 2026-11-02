---
id: T-39515
title: Fit full English names and scores in exam rankings
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39510-no-japanese-left-check]]", "[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Keep complete player names, ranks and scores legible in the English exam rankings, without abbreviation or overlap.

## Acceptance criteria

- [x] English layout keeps full maximum-width player names separate from ranks and scores in both columns.
- [x] Unknown-girl visibility and ranking values retain their original behavior.
- [x] Original-game build remains byte-identical; changes are English-only.
- [x] Actual ranking GPU captures verify the longest name and all populated rows.
- [x] Code review, rebuilt-disc verification and refreshed browser package pass.

## Evidence

`build/emu/exam-results-proof/shot_1500.png` on `build/table-padding-proof` shows the full-name row colliding with its score. The controlled Saturday fixture loads the real ETC exam mode and uses an injected accumulated score; 2,000 frames report zero original-font uploads/unknown/overflow. Rendering is in `src/ovl/misc_scenes/final_exam.c`: `exam_results_show` writes names at row_x + 24; `exam_ranking_numbers_draw` draws wide sprite digits beside them. Seven rows per column leave no room for two 12-pixel text lines per name.

## Implementation direction to verify

Use proportional English digits for ranks and scores, drawn once alongside names during the ranking setup. Preserve the two-column/seven-row structure and visibility rules. A 160-pixel column can budget 18 pixels for three rank digits, 116 pixels for the maximum name, 18 pixels for the score, and 8 pixels of gaps/margins. Verify actual font widths, rank limits and column coordinates before choosing final positions. The repeated per-frame sprite-number function must not accumulate persistent text entries; English static text belongs in setup. Keep the Japanese path unchanged.

## Progress

- Implemented `exam_english_row` behind the English flag. It right-aligns proportional ranks and scores around a 116-pixel full-name region in each 160-pixel column. It creates persistent text once during setup; the per-frame English sprite-digit renderer is disabled. Original visibility branches and score/rank arrays are retained.
- Production-helper C89 regression passes for every row and both columns with 116-pixel names and three-digit values. Actual digit advances are at most six pixels; a valid seven-W family and seven-W given name measures exactly 116 pixels.
- Original ETC checksum and all 27 default binary hashes pass. Corrected default ninja build passes headers/globals and the 14,449-row rename audit. The initial missing libc-owner-header check was resolved before these checks.
- Full English build `build/exam-layout-proof` passes in 255.4 seconds; refreshed browser package `build/patcher-exam-layout-review` passes all nine checks and its downloaded disc matches CLI output (target prefix cbbec1283f15). Main ELF changed with overlay address updates, so GPU tests use a fresh 7,326-frame power-on/card-load checkpoint.
- Both maximum-width, fully populated ranking fixtures are running; they also sample the renderer's 52-entry/384-glyph resource limits. Do not mark the layout accepted until captures and counters are inspected.
- Full-table fixtures failed with an invalid read after all row helpers returned. GDB identified a primitive GPU tag overwriting draw_buffer, with only 335/384 glyphs and 46/52 entries. GetWorkBase multiplied its already-byte-based usage by four; corrected only the English path and added a bounded mixed-allocation regression. Rebuild and fresh runtime fixtures pending.

## Final verification

- `build/exam-arena-proof`: full build passes in 165.7 s; target prefix 32fb03da865a; 6,563,732-byte xdelta. Frozen linked outputs accompany the disc. All 27 original checksums pass, as do globals and the 14,449-row rename audit.
- 38 English patch/layout regression tests pass, with one opt-in test skipped. Actual-disc audit verifies 105 containers, 31,471 strings and 31,334 pointers with zero original-font bytes.
- Browser package `build/patcher-exam-arena-review` passes all nine acceptance checks; browser download equals the CLI-built disc.
- Fresh power-on/card-load bootstrap completes 7,326 frames. `build/emu/exam-arena-left/shot_1500.png` and `build/emu/exam-arena-right-positive/shot_1500.png` visibly retain both seven-W name components, ranks and scores without collision in each column. `build/emu/exam-arena-sparse/shot_1500.png` preserves hidden names/numbers for unknown girls. Reaction dialogue fits below the table.
- Each fixture completes 2,000 frames with zero original-font calls/unknown/overflow. Sampled maxima: 335 glyphs, 46 entries, 19,876 primitive bytes of 68,608; draw_buffer stays 0/1. Machine-readable summary: `build/exam-arena-gpu-summary.json`.
- The initial right-column zero-score fixture is excluded as name-layout proof: the original ranking sorter uses zero as its consumed-value sentinel and omitted the zero-score player. The corrected positive score of 10 exercises the intended bottom-right player row. Sorting behavior is unchanged by this patch.

## Review

2026-11-02 Codex: reviewed against `CODING_STANDARDS.md`. Changes use C89 declarations and existing SDK/game declarations, with English-only guards and ticketed rationale. The row helper preserves visibility branches, exact numeric values and the player-half flag, and installs entries once rather than per frame. Allocator usage and addresses now share byte units; requested blocks remain aligned and non-overlapping. Production-function regressions cover both columns and buffers. Default checksums, globals and rename checks pass. No existing identifiers renamed, generated assembly edited or game assets staged. GPU and browser evidence above resolve the observed overlap and arena overwrite; no outstanding review findings.
