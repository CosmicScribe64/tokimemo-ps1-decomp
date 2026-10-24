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
- [[spec/m-sysmenu]] - main `src/main/8005A0B0.c`, the player's room screens: weekly command screen (icons, build rules), calendar check, memory-card data screen, system settings and mascot, club joining, rumor report, Christmas and New Year preparation ([[tickets/T-27017-p2-m-sysmenu]])
