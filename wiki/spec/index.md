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
- [[spec/m-dates-b]] - main scene step library (8007C030.c): scene step tables and step primitives, scene state, wipes and fades, portrait files by girl and season, dialogue engine (SERIFU script tables, 20 speak codes, name codes, choices, voice), heroine picker, day_plus, check_para_limit, SD sprite tables ([[tickets/T-27015-p2-m-dates-b]])
- [[spec/m-holiday]] - main holiday screen (mode 0x46): command menu and icon rules, the activity step and SD sets, Sunday club flows and the first-activity scene, appointment place question and launch, phone book, dial shortcut and dial tones, information magazine, shared-birthday check ([[tickets/T-27013-p2-m-holiday]])
- [[spec/o-bunkasai]] - overlay BUNKASAI, the club-member culture festival (mode 0x70): phase and stage machine (165 stages, 15 blocks), exhibits by club and year, visitor choice, festival result formula, fortune telling, TT game exhibit, birthday scene, data tables ([[tickets/T-27025-p2-o-bunkasai]])
