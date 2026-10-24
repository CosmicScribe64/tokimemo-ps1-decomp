---
type: plan
updated: 2026-10-24
---

# Phase 2: human-readable codebase

Goal: make the matched code readable, port-first: "not 100%, just enough for the Godot port" (user scope,
2026-10-24). The game logic is named and explained in full, the PS1 hardware layer only at module level and
entry points. The matching decompilation stays exact: 27/27 sha1 after every change. Umbrella ticket:
[[tickets/T-27000-phase-2]]. The rules are fixed elsewhere:

- Decisions, conventions, the disc-name map and the prefix maps: `docs/adr/0001-phase-2-naming.md`.
- Checkable rules for review: `CODING_STANDARDS.md` sections 4, 5 and 8.
- Terms and identifier stems: `GLOSSARY.md`.
- Survey with counts, the rename tool design and the risks: `drafts/T-25900/audit.md` (T-25900).

## Scope

Set by the user on 2026-10-24; the full table and the accepted exception are in the ADR (section Scope).

- Game logic: named and explained in full. It covers GameState and its records, enums, calendar and schedule,
  parameters, girls with affection and tokimeki, day events and scenes, dates, confession and endings, save
  data, each overlay's state machine and data tables, and the files and headers of these modules. Explained
  means a name backed by evidence, a comment at the definition for a rule that is not plain, and the unit's
  port spec in `wiki/spec/` for state machines and tables.
- PS1 hardware layer (GPU, sprites, VRAM; sound driver; CD and XA; MDEC): module level and entry points.
- Locals, parameters and hex literals: only where essential, that is where they carry game logic.
- SDK names: optional.
- Done is measured per layer. No placeholder remains in game logic except unused or unknown fields and jump
  tables, and the residue in the hardware layer and the SDK is listed, not driven to zero.

## Starting point (2026-10-23)

6,649 of 6,952 functions are `func_` names, about 14,000 data labels are `D_` names, 1,401 of 2,152 struct
members are `unk_`, 461 of 661 type names describe a size or an address, and all 437 C files are named by
address. Usage is concentrated: GameState (`D_800E6280`) has 12,306 uses in 295 files, and the 100 most used
function names carry 48% of all function-name uses, so the shared core comes first.

## Work order

| step | unit | ticket | size | who |
|---|---|---|---|---|
| 0 | Tooling: `tools/rename.py`, `tools/symtab.py`, tests, the tool fixes of audit 3.8, merge replay | T-27002 | about 1.5k lines of Python | Opus design and review, Sonnet code |
| 1 | Pilot: O.BIN names into the overlays (overlay yamls load `symbol_addrs_obin.txt`; 105 names in 186 overlay files; 56 dual declarations merged; `cd_read_status` moved out of EVENT's range) | [[tickets/T-27003-pilot-obin-names-overlays]] | one build | Opus |
| 2 | Shared core: GameState fields, the core types of `game.h` and `main_api.h` (the 0x44 display pools first), the first enums (girl, place, club, parameter, weekday, game mode, day-event type), the 100 most used main functions; takes over the GameState pilot of [[tickets/T-20500-emulator-assisted-naming]] | [[tickets/T-27004-shared-core-names]] | 2-3 batches | Opus, with the emulator |
| 3 | Main modules, two at a time (table below): game-logic functions and globals in full, hardware-layer entry points, re-check of the non-O.BIN names in the range, O.BIN comments, then the module header and the file names | one ticket each | 9 units | Sonnet, Opus review |
| 4 | Overlays, two at a time (table below), one header owner each; state machine and data tables named and explained, port spec in `wiki/spec/` | one ticket each | 24 passes | Sonnet, Opus review |
| 5 | Locals and parameters of game-logic functions per unit, where they carry game logic (11,174 m2c-style uses in all) | per unit | | Sonnet |
| 6 | Constants per unit with the step 2 enums, for ids, flags and thresholds of game logic (53,215 hex literals in all; long tail) | per unit | | Sonnet |
| 7 | SDK placeholders (479 asm-only functions; about 150 of them called from game C) | optional (user scope) | | Sonnet |
| 8 | Final sweep: `count_names.py` residue per layer (game logic must be clear, the rest is listed); `include/main_api.h` and `include/game.h` deleted once every C file includes its topic headers, with CODING_STANDARDS section 8a and `tools/check_headers.py` moved to the topic headers; living wiki pages (`source-files`, `game-state`, `data-types`) refreshed | | 1 | Opus |

Steps 1 and 2 wait for step 0. No rename of any kind lands before the tool has merged, and once it has, `tools/rename.py` is the only way to rename anything (user decision, 2026-10-24). Inside every unit the order is: functions and globals, types and fields, the
one-line English comments on O.BIN names, then the header split and the file renames, each in a commit of
its own. File names come last because they need the contents understood.

Step 2's enum list differs from the audit's on purpose. It adds place and club, whose orders are known from
the name tables (`GLOSSARY.md`), the game mode (GameState 0x1108) and the day-event type. It leaves out the
month, a number from 1 to 12 that the code compares and counts with. It also leaves out the selector bytes
at 0x1109, 0x110A and 0x110D, which `main_state_set` clears when it sets the game mode. Step 2 names the
first two fields (`mode_stage`, `mode_step`), but their values mean something else in every mode, so each
mode's unit gives them constants; 0x110D waits for the units that use it.

### Main modules (step 3)

| unit | files today | lines |
|---|---|---|
| M1 | `80041000`-`800451D0` (6 files): boot, game loop, pad, 2D drawing, CD | 2.4k |
| M2 | `80046500`-`8004C3F0` (6): CD helpers, tpage, sprites, primitives, layers | 2.5k |
| M3 | `8004E500`-`80053650` (4): kanji text, menus, speaker labels, memory card | 3.0k |
| M4 | `80056300`-`80061710` without `8005A0B0` (8): movies, backgrounds, 3D, mouse, title | 2.0k |
| M5 | `8005A0B0`: system menus, clubs, rumors | 3.0k |
| M6 | `80062CD0`, `800674B0`: HUD | 3.2k |
| M7 | `8006CB30`, `800737A0`: scheduler, school-day loop | 3.2k |
| M8 | `80075320`-`80079B10`: days off, overlay loader, dates | 3.1k |
| M9 | `8007C030`, `80085E30`: girl selection, faces, dialogue, fades | 3.5k |

### Overlays (step 4)

Two passes of one overlay run one after the other (they share the header and the symbol file). A near copy is
named after its original, reusing the original's names under its own prefix: GYOZI after SHOUGATU, EVENT
after DATE, BUNKAKEN after BUNKASAI.

| pass | overlays |
|---|---|
| large, two passes each | BUNKASAI, BUNKAKEN, RPG_BAT, DATE, EVENT, TACO |
| single | ETC, NAME_ENT, TAIIKU, GEKO, SHOUGATU, GYOZI |
| TT | only after agreement with its contributor |
| pairs | TEL + OPTION, MASTER + ENDING, BUNKA_SD + SHUGAKU, KANGEI + DATE2 |
| group | EN_NICHI + OLH + OMIMAI + RENSYU + VALEN |

Overlays marked "confirm" in the ADR map (BUNKAKEN, ETC, GEKO, KANGEI, SHOUGATU, TT, VALEN) check their
contents with the emulator before they adopt the folder name and prefix. EVENT and GYOZI ("confirm unused")
look for a loader again before their folder names keep `_unused` ([[tickets/T-0200-event-gyozi-loader-and-address]]).

## Unit loop

1. Claim the ticket (from `wiki/tickets/_template.md`), set it In Progress.
2. The namer reads the code, `GLOSSARY.md` and the maps, re-checks the non-O.BIN names in the range (list
   below), and writes a rename table in the `tools/rename.py` format. Every row's note gives the evidence (callers, data read, mode set, the string label passed, or an
   emulator run). A meaning the evidence does not prove goes in as a hypothesis.
3. `rename.py plan` until it reports no collision.
4. Opus reviews the table (below) before anything is applied.
5. `rename.py apply`, then the clean build: `rm -rf asm build; configure; ninja` under `nice -n 19`, at most two
   builds on the machine at a time, Docker `--cpus=2`. Proof: 27/27 sha1 and `ninja progress` still 6,952.
6. Commit one table per commit; the header split and each file rename in their own commits. `apply` appends
   each row with its note to the unit's ledger (`config/renames/<unit>.txt`), which is committed, so the
   evidence stays next to the name.
7. Ticket to In Review; the code-review skill checks the work against `CODING_STANDARDS.md`; then Done.

Overlay units do not rename main symbols. They add requests to `config/renames/requests.txt`, and the owner
of the main module makes the rename.

## Review rule

- Sonnet agents name; an Opus agent reviews every table before it is applied, and the code review checks the
  commits before Done.
- Every name needs evidence. The reviewer rejects a name whose note gives none, a game-meaning claim without
  proof that is not marked as a hypothesis, and an `unk_` field renamed from a single use that shows nothing.
- Runtime evidence comes from the emulator ([[emulator]]): breakpoints, memory watches and call stacks on
  main and overlay code, scripted input and save states. The note names the run (subcommand and arguments), not
  a screenshot; save states and screenshots are game data and stay out of the repository.
- Hypotheses stay marked (HYPOTHESES symbol file, or `/* HYPOTHESIS: ... */`) until a later run proves them;
  the proving ticket removes the mark.

## Old names in the wiki

User decision, 2026-10-24. Tickets, logs and old notes keep the names they were written with, as history.
Current pages (status, playbook, specs, the phase 2 pages) use the new names, and the unit that owns a topic
refreshes them. `tools/rename.py lookup` and an old-name column in the function catalog ([[notes/functions]])
map an old name to its new one.

## Names to re-check

Names that exist before phase 2, are not O.BIN names, and break the conventions. The unit that owns the range
renames them through `tools/rename.py` with its first table; the list is not complete, because each unit also
checks its range.

| name (address) | unit | conflict |
|---|---|---|
| `main_state_set` (0x80042878) | M1 | English-patch name, not O.BIN; `game_mode_set` by the rules |
| `file_checksum_ok` (0x8004636C) | M1 | module prefix is `cd_` |
| `glyph_expand_1bpp` (0x8004F4F0), `font_glyphs` (0x800B6D50), `font_range_table` (0x800B402C) | M3 | module prefix is `text_` |
| `system_settings_init` (0x8005DC4C) | M5 | prefix is `settings_` |
| `appointment_find_today` (0x8006AEC4), `club_list_menu` (0x8006B648) | M6 | topics of the days-off and system-menu modules; HUD file takes `hud_` |
| `special_event_select` (0x8006FB4C) | M7 | bare `event`; stems are `schedule_` and `day_event_` |
| `telephone_class_init` (0x800782F0) | M8 | `phone` is the TEL overlay's stem; days-off code uses `holiday_` |
| `dialogue_render` (0x80081190), `dialogue_speak_part` (0x8007EF48), types `DialogueWork`, `DialogueTables`, `DialogueVoice8` | M9 | stem is `talk` |
| `date_event_trigger` (0x801326B0), `date_event_choose` (0x8013272C) | DATE | bare `event` |
| `shougatu_stage_dispatch` (0x8013D180) | SHOUGATU | prefixes are `new_year_` and `christmas_`; `stage` is a trap word |

`ovl_load_buf`, `ovl_load_lba`, `girl_name_buf`, the parameter names `src` and `dst`, and the word `init` pass
since the glossary lists `buf`, `lba`, `src`, `dst` and `init`.

## Open points

- TT: names and folder need agreement with its contributor first.
- The hypothesis file names in the ADR (`config/symbol_addrs_hypotheses.txt`, `config/symbols_<DISC>_hypotheses.txt`,
  and `config/symbol_addrs_main_hypotheses.txt` for main names in EVENT's range) must match what T-27002
  implements; its `tools/symtab.py` (branch w19-rename, In Review) uses the same names.
