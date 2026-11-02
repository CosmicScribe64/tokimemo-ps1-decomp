---
id: T-39522
title: Distinguish speaker names from dialogue
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Use a distinct name color and a little more spacing in English dialogue boxes, as requested by the owner.

## Acceptance criteria

- [x] Apply the distinction consistently to script and telephone speaker labels.
- [x] Use the same speaker spacing in build-time wrapping and runtime drawing.
- [x] Inspect classroom and phone captures with long names; check for clipping and awkward wrapping.
- [x] Pass relevant regression, default-build and standards checks.

## Evidence

Compared original `build/emu/route-jp-review-reference/036_prologue_08.png` with English `build/emu/route-en-phone-width-voice/038_prologue_08.png`. The original uses same-color inline names, but its short names and opening bracket distinguish the text more clearly. The owner selected a distinct name color and more spacing.

Script and telephone names now use the existing pink font palette with an eight-pixel gap before the opening mark. Quotes and dialogue keep their existing colors. The build-time speech window uses the same gap. The ten-pixel draft was rejected because it split the introduction after its short opening phrase; the eight-pixel version restores two complete rows.

Fresh capture `build/visual-quality-review/speaker-gate-perspective/shot_032.png` shows the restored introduction with full player names. Fresh classroom captures 038 and 039 also show distinct names and complete quote/thought closings. The voice-enabled route completed 33,106 frames through phone, date and save/load checks. All 132 captures were inspected; original-font calls, unknown calls and overflow calls are zero. The eight-pixel gap leaves two additional long-page flags, in EVENT_AS and EVENT_KO; broader text-layout review remains in T-39511.

## Review

The combined affected suite passes 109 tests with two opt-in skips. Both disc text audits and all nine browser checks pass for the combined plaque/spacing candidate, target prefix `9935d1aacd90`. All 27 original binaries match; globals and 14,449 rename records pass.

Standards review checked C89 declarations, English-only compilation, unchanged dialogue colors, consistent speaker spacing in both rendering paths, and the matching build-time width calculation. No unresolved code issue remains. Final route inspection passes the speaker-label requirements. A separate short final row in the assembled date confirmation is tracked in T-39524.
