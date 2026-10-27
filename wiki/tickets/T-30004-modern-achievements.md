---
id: T-30004
title: Design achievements for the native port
status: Ready
assignee:
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase4]]", "[[phase3]]", "[[tickets/T-30000-plan-phase4-modernization]]"]
---

## Goal

Design and implement a deterministic, spoiler-aware native achievement system for the Godot port that rewards story milestones, heroine confessions/endings, parameter/club mastery, subgame victories, and collection completion without altering native simulation rules or RNG.

## Exclusive proposed paths

- `port/godot/services/achievement_service.gd`
- `port/godot/rules/achievement_catalog.gd`
- `port/godot/ui/achievement_toast_and_gallery.gd`
- `port/godot/tests/services/run_achievement_tests.gd`

## Acceptance criteria

- [ ] Defines a deterministic catalog (`achievement_catalog.gd`) of at least 40 source-grounded achievements across 5 categories:
  1. **Story & Calendar Milestones**: First School Day, First Date, Sports Festival victory, Cultural Festival success, School Trip (Kyoto/Okinawa/Hokkaido), Christmas Eve party entrance, New Year Shrine visit, Graduation Day (`1998-03-01`).
  2. **Heroine Confessions & Endings**: Legendary Tree / Rei confession endings for all 12 main heroines (`Shiori`, `Mio`, `Yuina`, `Ayako`, `Yukari`, `Mira`, `Saki`, `Nozomi`, `Yuko`, `Megumi`, `Miharu`, `Yumi`), Best Friend (`Yoshio`), and Bad Ending (`Solo Graduation`).
  3. **Parameters, Exams & Clubs**: Academic Top Rank (`Rank 1` on term exams), Parameter Master (`>= 150` in Arts/Science/Sports/Looks/Guts), Club Master (`MASTER` title at Cultural Festival), Zero-Stress Semester.
  4. **Subgames & Arcade**: Force Gear (`TACO`) Boss Clear, TwinBee Time Attack (`TT`) High Score, RPG Battle (`RPG_BAT`) Victory, 100m Dash / Marathon / Tug-of-War (`TAIIKU`) 1st Place, Night Stall (`EN_NICHI`) Smart Ball / Goldfish Master.
  5. **Social & Special Challenges**: Bomb Defusal Specialist (defuse an active bomb via telephone/date), Popularity Peak (4+ heroines at Tokimeki state), Album Collector.
- [ ] Evaluates unlock predicates purely from read-only `GameState` + `FrameResult` transition snapshots at step boundaries — never mutating game state, advancing RNG, or altering frame timing.
- [ ] Persists unlocks atomically in `tokimemo.port-achievements.v1` JSON (`unlock_timestamp`, `route_seed`, `in_game_date`, `progress_counters`) and hides secret/ending achievement descriptions until unlocked (or until the user toggles "Reveal Spoilers" in the gallery).
- [ ] Provides a non-intrusive corner toast notification (with optional toggle in settings) and an interactive Achievement Gallery overlay with category filtering and completion percentage.
- [ ] Automated test suite (`run_achievement_tests.gd`) verifies deterministic unlocks across all 5 categories, idempotence on reload/replay, spoiler masking, and persistence in Docker.
- [ ] Code review against `CODING_STANDARDS.md` passes with all findings resolved before moving to `Done`.
