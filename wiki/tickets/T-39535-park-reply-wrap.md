---
id: T-39535
title: Keep Kagami's park reply phrases together
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Remove awkward short rows from the four flagged amusement-park reply records without changing their wording or reply choices.

## Acceptance criteria

- [x] Preserve the phrase breaks in the two distinct affected payloads, with narrow scope and unchanged words.
- [x] Verify every reply variant fits and survives actual-disc readback.
- [x] Inspect all four records through complete final rows in the game renderer.
- [x] Review the change against CODING_STANDARDS.md.

## Evidence

The published-build replay `build/emu/mira-park-long` reaches the intended park with Kagami's portrait and actual DATE_KG tables. Groups 25, 41 and 57 share one payload; group 50 has another. Inspected 20 captures across three sheets. The first reply in each record scrolls cleanly but reflow leaves a short final fragment in its opening sentence. The existing English translation's phrase boundaries are clearer. Those initial captures did not approve the four records.

Added exact-match phrase rows for the first reply of groups 25, 41, 50 and 57. The source comparison checks every payload and confirms exactly those four change. All words and the other two reply variants remain identical; each resulting row passes the layout fit checks. Receipt: `build/park-wrap-source-check.json`. The existing English patch suite passes 46 cases with one optional full-disc test skipped. The next build and runtime review will be combined with T-39536's glyph-capacity repair; this change is not published yet.

## Verification and review

Completed fresh-disc park replay and all twelve variants across four groups. Inspected 64 captures on eight sheets, including settled frames 120 ticks after initial completion so closing quotes are visible. All four repaired first replies read naturally, scroll completely and retain all alternate choices. Actual-disc readback verifies all 498 strings and 498 pointers in DATE_KG, including every variant. Receipt: `translations/en/reviews/park-replies-2026-11-02.json`.

Reviewed the scoped changes against CODING_STANDARDS.md. Exact IDs and exact old payloads limit the override; full-corpus comparison confirms unchanged words, controls and unrelated payloads. Layout fit checks, existing regressions and actual renderer captures pass. No runtime code change or copyrighted asset is part of this ticket. No unresolved finding; moved to Done. Publication of this build remains separate.
