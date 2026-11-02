---
id: T-39510
title: Prove no Japanese is left in the English game
status: Ready
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

The owner's bar for the English patch: no Japanese on screen anywhere, apart from the approved exceptions listed in [[tickets/T-39500-english-translation-patch]]. Prove it with checks that cover the whole game, not only the screens a route reaches.

## Acceptance criteria

- [ ] Text: every text container and C string in the English build is scanned for Shift-JIS glyph codes; each hit is either an approved exception, a control code, or a bug to fix.
- [ ] Runtime: a debug option of the English build logs every 2-byte glyph the text renderer draws; the emulator route and extra save-state stops (dates, festival, exams, sports, battle, endings) run with it and report zero unexpected glyphs.
- [ ] Art: every artwork item marked "no change required" or "no Japanese text" is checked against its decoded pixels, and every placed image is read back from the disc (T-39508's check).
- [ ] A screenshot sheet of the main screens, Japanese beside English, for the owner's review.

## Notes

Runs once [[tickets/T-39506-english-script-runtime]], [[tickets/T-39508-english-art-on-disc]], [[tickets/T-39509-opening-movie-pills]] and the date-scene text fit have landed.

## Comments
