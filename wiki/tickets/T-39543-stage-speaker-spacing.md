---
id: T-39543
title: Separate stage-show speaker names from opening quotes
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-03
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

The literature speech renderer places Fujisaki and Kisaragi at -120, where their English widths reach -84 and -82. Its opening quote starts at -86. Give every speaker an eight-pixel gap and the English speaker color without changing dialogue rows or the default build.

## Acceptance criteria

- [x] Preserve original build behavior and existing script text.
- [x] Verify the English speaker bounds and native-resolution rendering.
- [x] Build, review and publish the correction.

## Evidence

The same-row census found seven possible overlaps in 1,100 direct text calls. Five compare mutually exclusive switch cases. Two compare a speaker with the quote drawn after that switch and are real geometry defects. The source is `stage_show_script_line_render` in `src/ovl/culture_stage_shows/literature_speech_contest.c`.

The mascot picker has another confirmed narrow-column case. Its 43-pixel pitch is smaller than Kiyokawa's 45-pixel name, which precedes another populated column. Tatebayashi is 60 pixels but occupies the last column. Expand only the English mascot grid to 56-pixel pitch with a -24 origin; its longest last-column label then ends at 148, inside the 156-pixel usable right edge. Keep the existing row order and selection behavior.

## Visual revision

Rejected the first wider three-column mascot layout. It left names under the preview sprite and crossed the notebook divider. The final English layout keeps the original -8 origin, uses two columns with a 54-pixel pitch, and fits seven 12-pixel rows. The longest second-column name ends at 106, before the preview begins. This supersedes the three-column proposal above.

The long-player-name capture also rejected right-aligning names against the old quote column. Matsukawa entered the frame border. The final stage helper keeps names at the standard -128 margin and moves both the quote and dialogue column according to the name width. All 214 stage rows are at most 143 pixels wide, leaving room for the longest supported player names in the adjusted column.

## Verification and standards review

The final candidate passes both actual-disc audits, all 27 default-binary hashes and all 15 browser checks. Of 70 focused tests, 69 pass and the opt-in full-build test is skipped; the production build was run separately. The fresh boot loads the existing save through run-local card copies. The stage fixtures verify the long player name and Kisaragi's name at the actual sampled coordinates. The player speech completes and returns to the festival. The initialized mascot menu shows all fourteen choices in two columns without covering the notebook divider or preview.

The command comparison proves that all 214 prefixed rows retain the same visible English. The independent audit rejects a missing or altered command even when writer output and pointer agree. The 39 actual-disc telephone, club and mascot labels pass their column bounds. Evidence is in `translations/en/reviews/stage-controls-and-menu-spacing-2026-11-02.json`.

Reviewed the C89 guards, original-build equivalence, field bounds, menu selection geometry, source-prefix provenance and negative audit tests against CODING_STANDARDS.md. No unresolved findings remain. Pages deployment 41600020995 succeeded. All 37 served files match the tested package. The publication receipt is `translations/en/reviews/published-stage-controls-2026-11-03.json`.
