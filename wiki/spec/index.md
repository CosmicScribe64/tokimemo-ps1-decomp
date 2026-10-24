---
type: index
updated: 2026-10-24
---

# Port specs (phase 2)

One line per unit: what the spec covers. Specs are written for the Godot port ([[tickets/T-27000-phase-2]]); renames proposed with each spec live in `config/renames/proposals/`.

- [[spec/o-taco]] - overlay TACO, the shooter mini game: stage machine (19 stages), ship, wingman, weapons, bombs, collisions and scoring, enemy scripts, three bosses, results, settings and exit
One line per unit. Specs are facts for the Godot port, in our own words; names are proposals in `config/renames/proposals/`.

- [[spec/h-graphics]] - main graphics layer: frame render, actor pool, texture-block cache, VRAM queues, primitives, screen layers, palette tools, debug HUD ([[tickets/T-27020-p2-h-graphics]])
One line per unit. A spec states what a system does, its state with the proposed names, its rules and its links to the rest of the game, in the words of this project; no game text. Parent: [[tickets/T-27000-phase-2]].

- [[spec/core]] - shared core: GameState map, mode/stage/step machine, calendar and day-event queue, player and girl state, shared record types, first enums, the 100 most used main functions (T-27010).
# Phase 2 port specs

One line per unit. Each spec states what the system does, its state and data (with the proposed names), the rules as the code implements them, how it connects to the rest of the game, and open questions. Rename proposals are in `config/renames/proposals/<unit>.tsv`.

- [[spec/h-cdio]] - main hardware layer: CD command ring and reads, checksum, XA clip request, STR movies, MDEC backgrounds, 3D setup, mouse, boot mode (ticket [[tickets/T-27021-p2-h-cdio]])
One line per unit. Specs state facts read from the matched code for the Godot port; names follow `config/renames/proposals/`.

- [[spec/o-rpgbat]] - overlay RPG_BAT: the RPG battle scene (stages, channels, damage and AI rules, girl actions, data tables).
# Port specs (phase 2 output)

One line per unit. Each spec states what the unit does, its state and data with the proposed names, the rules as the code implements them, how it connects to the rest of the game, and open questions. The rename proposals are in `config/renames/proposals/<unit>.tsv`.

- [[spec/o-tt]] - overlay TT, the TwinBee Time Attack mini game: stage, boss and ranking rules, state machine, data tables
- [[spec/m-dates-a]] - main sound driver (`src/main/80079B10.c`, the file the unit label calls date sequences): command words and classes, bank slots, BGM, sound effects, voice-clip (XA) timing, level meter ([[tickets/T-27014-p2-m-dates-a]])
- [[spec/m-scheduler]] - main scheduler: the weekly cycle (schedule steps, week queue builder, encounter and meeting selection, special events, queue reader and type-to-mode table), day notice, month start, calendar day advance and the weekly health check ([[tickets/T-27011-p2-m-scheduler]])
- [[spec/m-hud]] - main HUD unit: portrait face, parameter panel, date panel, message window and title banner, command icon bar, season and vacation helpers, calendar page with biorhythm graph, information magazine and date places, appointments, club list, save slot panel, command roll ([[tickets/T-27016-p2-m-hud]])
- [[spec/m-weekday]] - main school-day command loop (mode 0x40): stages and steps, background and SD-pack choice per command, the parameter_up_down formula, vacation days, week-end exit ([[tickets/T-27012-p2-m-weekday]])
- [[spec/o-date]] - overlay DATE, dates: mode 0x50 stages (meeting, place, girl birthday gifts, player birthday), the 21 places, answer scoring, end-of-date bookkeeping, ambush, amusement park, and the 48 special date scenes of mode 0x5E (46 of the girls, 2 intrusions of Miharu) ([[tickets/T-27022-p2-o-date]])
