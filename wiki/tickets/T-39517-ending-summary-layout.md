---
id: T-39517
title: Fit the scrolling English ending summaries
status: In Progress
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[tickets/T-39510-no-japanese-left-check]]"]
---

## Goal

Keep the complete English career/relationship ending summary inside the screen while preserving readable scrolling and all text.

## Acceptance criteria

- [ ] Prove the summary window geometry and wrap every applicable summary record, including expanded names and career labels.
- [ ] Preserve all wording and correct scroll progression without text-entry/glyph overflow.
- [ ] Actual ending GPU captures show complete lines and final text; original build remains matching.
- [ ] Regression checks, final-disc audit and code review pass.

## Evidence

The 5,000-frame controlled Yoshio ending reaches graduation, career summary and song with zero original-font calls. Nevertheless `build/emu/ending-arena-proof/shot_2500.png` visibly clips several summary lines at the right edge. `ending_graduation_result_dialogue_select` renders them at x=-0x48 (screen x=88); `english_talk_render` expands rows but does not dynamically wrap them. The summary's source-record/window mapping must be inspected; do not accept clean font tracing as layout proof.

The affected records are in `CDROM/SERIFU/EPILOG.BIN` (profile `serifu-epilog`), especially table-11 and table-12. Their command-7 metadata currently selects generic narration x=-0x80, 252-pixel width; the actual summary calls use x=-0x48. Generic M substitution reserves only 56 pixels, but ending career labels such as "The second-tier company" are longer. A correct repair must use the summary's actual width and context-specific substitution bounds or runtime wrapping, not merely move the clipped text left.
