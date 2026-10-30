---
id: T-39220
title: Add Switch-informed modern presentation options
status: Backlog
assignee:
created: 2026-10-30
updated: 2026-10-30
links: ["[[switch-remake-reference]]", "[[switch-remake-footage]]", "[[phase4-design-2026-10-29]]"]
---

## Goal

Fold the fidelity-safe lessons from the 2025 Switch release into phase 4, as opt-in, display-only options that keep the native 320x240 image and game state unchanged.

## Acceptance criteria

- [ ] Optional command-screen "Side panels" (off by default, not in the Original preset): week schedule and, only on explicit opt-in, biorhythm, read from existing game state and drawn in the wings over the continued checker.
- [ ] Modern text box mirrors the native message window frame, proportions, name column and centred width, changing only the font; it covers the message, date, location and parameter windows and keeps the original font on command select.
- [ ] Every settings item has a one-line explanation.
- [ ] Button names inside translated text follow the chosen prompt style, substituted only in the text layer.
- [ ] Name entry accepts typed keyboard/system text, converted into the existing name buffer.
- [ ] Hold-to-fast-forward runs several native ticks per presented frame without skipping any tick; fidelity tests unchanged.
- [ ] Port-level save manager (many slots, memos) wrapping unmodified memory-card images; controls/manual page in the pause menu.
- [ ] User decision recorded before any decorative wallpaper wing fill (original assets only, no new art or branding).

## Notes

Evidence: [[switch-remake-reference]] (cited sources) and [[switch-remake-footage]] (frame log). The Switch keeps original art 4:3 with nearest-neighbour scaling, fills sides with a fixed wallpaper in events, offers optional command-screen side panels, and uses full 16:9 only with newly drawn art.

## Comments
