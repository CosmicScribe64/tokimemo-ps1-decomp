---
id: T-39515
title: Fit full English names and scores in exam rankings
status: In Progress
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39510-no-japanese-left-check]]", "[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Keep complete player names, ranks and scores legible in the English exam rankings, without abbreviation or overlap.

## Acceptance criteria

- [ ] English layout keeps full maximum-width player names separate from ranks and scores in both columns.
- [ ] Unknown-girl visibility and ranking values retain their original behavior.
- [ ] Original-game build remains byte-identical; changes are English-only.
- [ ] Actual ranking GPU captures verify the longest name and all populated rows.
- [ ] Code review, rebuilt-disc verification and refreshed browser package pass.

## Evidence

`build/emu/exam-results-proof/shot_1500.png` on `build/table-padding-proof` shows the full-name row colliding with its score. The controlled Saturday fixture loads the real ETC exam mode and uses an injected accumulated score; 2,000 frames report zero original-font uploads/unknown/overflow. Rendering is in `src/ovl/misc_scenes/final_exam.c`: `exam_results_show` writes names at row_x + 24; `exam_ranking_numbers_draw` draws wide sprite digits beside them. Seven rows per column leave no room for two 12-pixel text lines per name.

## Implementation direction to verify

Use proportional English digits for ranks and scores, drawn once alongside names during the ranking setup. Preserve the two-column/seven-row structure and visibility rules. A 160-pixel column can budget 18 pixels for three rank digits, 116 pixels for the maximum name, 18 pixels for the score, and 8 pixels of gaps/margins. Verify actual font widths, rank limits and column coordinates before choosing final positions. The repeated per-frame sprite-number function must not accumulate persistent text entries; English static text belongs in setup. Keep the Japanese path unchanged.
