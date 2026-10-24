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
