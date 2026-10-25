---
type: index
updated: 2026-10-25
---

# Port specs (phase 2)

Specs describe the matched code for the Godot port, in our own words. Proposal tables are historical inputs; current-name status is per page, and applied ledgers plus source take precedence.
Parent: [[tickets/T-27000-phase-2]].

- [[spec/o-taco]] - overlay TACO, the shooter mini game: stage machine (19 stages), ship, wingman, weapons, bombs, collisions and scoring, enemy scripts, three bosses, results, settings and exit
- [[spec/h-graphics]] - main graphics layer: frame render, actor pool, texture-block cache, VRAM queues, primitives, screen layers, palette tools, debug HUD ([[tickets/T-27020-p2-h-graphics]])
- [[spec/core]] - shared core: GameState map, mode/stage/step machine, calendar and day-event queue, player and girl state, shared record types, first enums, the 100 most used main functions (T-27010).
- [[spec/h-cdio]] - main hardware layer: CD command ring and reads, checksum, XA clip request, STR movies, MDEC backgrounds, 3D setup, mouse, boot mode (ticket [[tickets/T-27021-p2-h-cdio]])
- [[spec/o-rpgbat]] - overlay RPG_BAT: the RPG battle scene (stages, channels, damage and AI rules, girl actions, data tables).
- [[spec/o-tt]] - overlay TT, the technical TT mini game (branded expansion unconfirmed): stage, boss and ranking rules, state machine, data tables
- [[spec/m-dates-a]] - main sound driver (`src/main/80079B10.c`, the file the unit label calls date sequences): command words and classes, bank slots, BGM, sound effects, voice-clip (XA) timing, level meter ([[tickets/T-27014-p2-m-dates-a]])
- [[spec/m-scheduler]] - main scheduler: the weekly cycle (schedule steps, week queue builder, encounter and meeting selection, special events, queue reader and type-to-mode table), day notice, month start, calendar day advance and the weekly health check ([[tickets/T-27011-p2-m-scheduler]])
- [[spec/m-hud]] - main HUD unit: portrait face, parameter panel, date panel, message window and title banner, command icon bar, season and vacation helpers, calendar page with biorhythm graph, information magazine and date places, appointments, club list, save slot panel, command roll ([[tickets/T-27016-p2-m-hud]])
- [[spec/m-weekday]] - main school-day command loop (mode 0x40): stages and steps, background and SD-pack choice per command, the parameter_up_down formula, vacation days, week-end exit ([[tickets/T-27012-p2-m-weekday]])
- [[spec/m-dates-b]] - main scene step library (8007C030.c): scene step tables and step primitives, scene state, wipes and fades, portrait files by girl and season, dialogue engine (SERIFU script tables, 20 speak codes, name codes, choices, voice), heroine picker, day_plus, check_para_limit, SD sprite tables ([[tickets/T-27015-p2-m-dates-b]])
- [[spec/m-holiday]] - main holiday screen (mode 0x46): command menu and icon rules, the activity step and SD sets, Sunday club flows and the first-activity scene, appointment place question and launch, phone book, dial shortcut and dial tones, information magazine, shared-birthday check ([[tickets/T-27013-p2-m-holiday]])
- [[spec/m-menutext]] - main text system, menus and girl queries (`8004E500`, `8004F6B0`, `8004F870`): text list and typewriter, speaker plates and dialogue blocks with name codes, menu engine and cursor modes, girl scores, ranks, closeness and points, name builders, birthday and club helpers ([[tickets/T-27018-p2-m-menutext]])
- [[spec/m-sysmenu]] - main `src/main/8005A0B0.c`, the player's room screens: weekly command screen (icons, build rules), calendar check, memory-card data screen, system settings and mascot, club joining, rumor report, Christmas and New Year preparation ([[tickets/T-27017-p2-m-sysmenu]])
- [[spec/m-flow]] - main flow: boot and frame loop, `game_state_init` and the new-game data, the mode dispatcher and overlay loader (sector and entry tables for all 48 overlay modes), TT loader, pad and mouse input with soft reset and the secret code, the title screen, the memory-card layer with the system-file and game-save images ([[tickets/T-27019-p2-m-flow]])
- [[spec/o-date]] - overlay DATE, dates: mode 0x50 stages (meeting, place, girl birthday gifts, player birthday), the 21 places, answer scoring, end-of-date bookkeeping, ambush, amusement park, and the 48 special date scenes of mode 0x5E (46 of the girls, 2 intrusions of Miharu) ([[tickets/T-27022-p2-o-date]])
- [[spec/o-bunkaken]] - overlay BUNKAKEN, the culture festival for players outside the culture clubs (mode 0x71): entry phases, 142-entry stage table, invitation and companion rules, exhibit templates by club, year and girl, points tables, birthday scenes, fortune telling, stage-show and TT contracts ([[tickets/T-27026-p2-o-bunkaken]])
- [[spec/o-event]] - overlay EVENT, a dead development copy (linked against another main build) of the girl-scene and place-date code: entry points, step-table interpreter, 73 girl scenes, 21 place dates with the arrival phase, special event chooser, outcome scoring, ride choice, dev-to-shipped address map; step tables in [[spec/o-event-steps]] ([[tickets/T-27023-p2-o-event]])
- [[spec/o-etc]] - overlay ETC: final exams (score formula, ranking), career guidance and the entrance exam (result byte), the ending tail (staff roll, last photo, clear save with the per-girl clear counter), the photo album and its save, dead stubs ([[tickets/T-27029-p2-o-etc]])
- [[spec/o-nameent]] - overlay NAME_ENT, the game-start scenes: file screens (memory-card check, system file, 14 album slots, create, New, Copy, Clear, load), name and birthday entry with the new-game rules (club, blood compatibility, cheat names), the 43-line voiced prologue and the attract opening ([[tickets/T-27030-p2-o-nameent]])
- [[spec/o-bunkasai]] - overlay BUNKASAI, the club-member culture festival (mode 0x70): phase and stage machine (165 stages, 15 blocks), exhibits by club and year, visitor choice, festival result formula, fortune telling, TT game exhibit, birthday scene, data tables ([[tickets/T-27025-p2-o-bunkasai]])

## Continuation drafts (In Progress)

These pages are checkpoints with explicit open work. Their function-name batches are applied; the individual pages distinguish applied rules from remaining data, type, state and essential-constant coverage.

- [[spec/o-master-ending]] - MASTER/ENDING: partial mastery and companion selection, ending candidates and audio levels (T-27035)
- [[spec/o-bunkasd-shugaku]] - BUNKA_SD/SHUGAKU: partial stage-show dispatch and school-trip response rules (T-27036)
- [[spec/o-small]] - KANGEI, DATE2, EN_NICHI, OLH, OMIMAI, RENSYU and VALEN loader/primary-rule map (T-27037)
- [[spec/reconcile]] - Reconciliation decisions, applied ledgers and verified source integration (T-27009)

- [[spec/o-taiiku]] - Sports festival: partial body-backed transitions, input/movement and girl-stat rewards (T-27031)
- [[spec/o-geko]] - GEKO encounters: five mode entries, walk-home/birthday rules, invitation selection and appointments (T-27032)
- [[spec/o-shougatu]] - New Year/Christmas and GYOZI: partial candidate, shrine, fortune and birthday rules (T-27033)
- [[spec/o-tel-option]] - Telephone/options: partial invitation eligibility, relationship effects and sound-test flow (T-27034)
