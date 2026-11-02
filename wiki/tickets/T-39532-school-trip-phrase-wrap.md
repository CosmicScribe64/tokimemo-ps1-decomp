---
id: T-39532
title: Keep the school-trip invitation phrase together
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

The sightseeing invitation splits "day after tomorrow" between rows. Preserve its wording and keep the phrase together when it fits.

## Acceptance criteria

- [x] Trace the current break and identify duplicate records.
- [x] Verify the revised layout fits the supported speaker widths without changing wording.
- [x] Inspect the revised invitation in the emulator.
- [x] Run relevant tests and review against CODING_STANDARDS.md.

- [x] Audit the complete rebuilt disc before publication.

## Notes

The current source retains Japanese row markers in all twelve invitation variants. The reflow step joins those markers before choosing new pixel-based breaks. The observed split is in `build/emu/school-trip-sightseeing-review/shot_2700.png` on target b8b5be606a64650203c4b6c452fb8f61cdadef83.

2026-11-02: the current pixel wrapper selects the split after joining inherited rows. The revised rows measure 99 and 134 pixels, including the runtime closing quote, against the conservative 183-pixel player window. Added an exact-payload/container layout rule; normal wrapping still checks fit. Compared every encoded record: exactly twelve whitespace-only changes, all placeable. Tests run 42 cases, with 41 passing and one optional skip. The isolated review disc changes only those twelve separators; target c3706ff4c86b9237ff3342ba332b58f08c0eb077. Fresh scene-entry replay is in progress.

2026-11-02: the fresh 9,000-frame scene replay completed. Inspected frames 8100/8400/8700/9000; frame 8400 shows the complete date phrase together on row two and the closing quote. Receipt: `translations/en/reviews/trip-invitation-layout-2026-11-02.json`. Reviewed the implementation against CODING_STANDARDS.md: exact container/payload matching, no wording change, normal width/overflow checks retained, no game-code or toolchain change. No unresolved code-review findings. The complete build is running at `build/trip-phrase-release/disc`; final disc audits and publication remain pending.

2026-11-02: the complete build exactly matches the inspected disc, target c3706ff4c86b9237ff3342ba332b58f08c0eb077. All 252 artwork checks, script/pointer and executable audits pass. All 15 browser checks pass; desktop/mobile result pages inspected. The implementation and verification are complete. The release receipt tracks publication separately.
