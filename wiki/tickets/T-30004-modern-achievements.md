---
id: T-30004
title: "Design achievements for the native port"
status: Backlog
assignee:
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]"]
---

## Goal

Nice-to-have modernization; RetroAchievements may inform design, not a promised service integration. Track after the faithful Phase 3 port; no implementation started.

## Acceptance criteria

- [ ] Draft spoiler-aware achievements for routes/endings, clubs, events and minigames using verified gameplay conditions.
- [ ] Implement deterministic event-based unlocking and persistent progress without changing simulation or RNG; handle reloads and duplicate unlocks.
- [ ] Review RetroAchievements references again before design. The PlayStation listing at https://retroachievements.org/game/19805 currently reports no achievements in the base set (checked 2026-10-25).
- [ ] Treat online/platform integration as a separate decision; do not claim RetroAchievements compatibility or copy unreviewed achievement definitions.

## Notes

Requested by the user on 2026-10-25. These additions do not replace full original-game port acceptance.

## Cross-version references (2026-10-25)

The user clarified that SNES and other versions are useful references and explicitly supports designing our own achievement set. The [RetroAchievements series hub](https://retroachievements.org/hub/18369) lists 97 achievements for [Densetsu no Ki no Shita de (SNES)](https://retroachievements.org/game/3913), while the PS1 base listing has none. A [Single-Session Confessions subset](https://retroachievements.org/forums/topic/19816) is another possible challenge-design reference. Counts are a dated lookup, not a dependency.

Review concepts across versions, then design original goals appropriate to the PS1 port. Verify each trigger against this version's actual rules/content; do not import SNES memory addresses or assume matching endings, thresholds, minigames or save restrictions. Cross-version references do not imply a planned RetroAchievements service connection.
