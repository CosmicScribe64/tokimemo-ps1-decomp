---
type: plan
updated: 2026-10-24
---

# Phase 2 human-readable codebase

Phase 2 made the matched code readable. It was scoped on 2026-10-24 to serve a planned Godot port, which was later removed. It names and explains the game logic in full and covers the PS1 hardware layer only at module level and entry points. The matching decompilation stayed at 27/27 SHA-1 after every change under umbrella ticket [[tickets/T-27000-phase-2]]. Supporting rules live in four documents:

- `wiki/phase2-naming.md` ([[phase2-naming]]) for decisions, conventions, the disc-name map, and the prefix maps.
- `CODING_STANDARDS.md` sections 4, 5, and 8 for checkable review rules.
- `GLOSSARY.md` for terms and identifier stems.
- `drafts/T-25900/audit.md` (T-25900) for the survey with counts, the rename tool design, and the risks.

## Current source acceptance (2026-10-25)

Phase 2 source work has passed the original Astra review, separate Astra fixes, and reviewer verification at source digest `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. The final clean proof matches 27/27 binaries, 6,952 functions, and 2,276,920 bytes. All 437 C basenames are logical, and topic headers and logical overlay paths are applied. The TT directory is the documented contributor-agreement exception. Hardware/SDK residue and unknown gameplay meanings remain explicit scoped exceptions. T-27000 is Done, with publication recorded below. Asset tickets T-280xx remain separate open work. The work order and proposal-era open points below are retained as planning history, while current acceptance is tracked in [[tickets/T-27000-phase-2]] and [[spec/phase2-completion-audit]].

## Scope

The user set the scope on 2026-10-24; the full table and the accepted exception appear in [[phase2-naming]] (section Scope).

- Game logic is named and explained in full. Scope includes GameState and its records, enums, calendar and schedule, parameters, girls with affection and tokimeki, day events and scenes, dates, and confession and endings. It also covers save data, each overlay's state machine and data tables, and the files and headers of these modules. Explaining a symbol requires a name backed by evidence, a comment at the definition for any rule that is not plain, and the unit's port spec in `wiki/spec/` for state machines and tables.
- The PS1 hardware layer (GPU, sprites, VRAM, sound driver, CD, XA, and MDEC) is named at module level and entry points.
- Locals, parameters, and hex literals are named only where they carry game logic.
- SDK names are optional.
- Completion is measured per layer. No placeholder remains in game logic except unused or unknown fields and jump tables, while residue in the hardware layer and the SDK is listed rather than driven to zero.

### Complete C-file naming requirement

Per user clarification on 2026-10-24, every game-code C file across `main` and the overlays must have an understandable role-based name. No address or number-only basenames remain at completion, and hardware files also receive technical role names. Renames preserve original object boundaries and require source evidence, case-insensitive per-folder uniqueness, separate file-only rename tables, API-view proof, and matching verification. T-27059 and T-27060 own the remaining exhaustive filename census and reviewed mappings.

Emulator runs, existing save files, and debugger traces are authorized to resolve uncertain meanings. Record reproducible commands as evidence and keep save files, screenshots, and other game data ignored.

## Starting point (2026-10-23)

At the start of Phase 2, 6,649 of 6,952 functions had `func_` names, about 14,000 data labels had `D_` names, 1,401 of 2,152 struct members had `unk_` names, 461 of 661 type names described a size or an address, and all 437 C files carried address names. Usage is concentrated in the shared core. GameState (`D_800E6280`) has 12,306 uses across 295 files, and the 100 most-used function names account for 48% of all function-name references, so the shared core comes first.

## Work order

| step | unit | ticket | size | who |
|---|---|---|---|---|
| 0 | Tooling (`tools/rename.py`, `tools/symtab.py`, tests, the tool fixes of audit 3.8, merge replay) | T-27002 | about 1.5k lines of Python | Opus design and review, Sonnet code |
| 1 | Pilot O.BIN names into the overlays (overlay yamls load `symbol_addrs_obin.txt`, 105 names in 186 overlay files, 56 dual declarations merged, `cd_read_status` moved out of EVENT's range) | [[tickets/T-27003-pilot-obin-names-overlays]] (done on branch `p2-pilot`, In Review with 104 O.BIN and 24 SDK names, 8 overrides; see [[rename-tool]] "Alias rows") | one build | Opus |
| 2 | Shared core (GameState fields, the core types of `game.h` and `main_api.h` starting with the 0x44 display pools, the first enums for girl, place, club, parameter, weekday, game mode, and day-event type, and the 100 most-used main functions; takes over the GameState pilot of [[tickets/T-20500-emulator-assisted-naming]]) | [[tickets/T-27004-shared-core-names]] (applied on branch `p2-core-apply`, In Review with 733 of 751 rows, then the review fix of 133 rows that brought the names to the ADR and glossary stems; left out the `u54C` macro aliases, `DisplayActor44.unk_18`, the Month enum, the `MainMode` tag and `MODE_` constants with no row kind, and the two file rows; see the ticket comments) | 2-3 batches | Opus, with the emulator |
| 3 | Main modules, two at a time (table below), covering game-logic functions and globals in full, hardware-layer entry points, re-check of the non-O.BIN names in the range, O.BIN comments, then the module header and the file names | one ticket each | 9 units | Sonnet, Opus review |
| 4 | Overlays, two at a time (table below) with one header owner each, naming and explaining the state machine and data tables and writing the port spec in `wiki/spec/` | one ticket each | 24 passes | Sonnet, Opus review |
| 5 | Locals and parameters of game-logic functions per unit, where they carry game logic (11,174 m2c-style uses in all) | per unit | | Sonnet |
| 6 | Constants per unit with the step 2 enums, for ids, flags, and thresholds of game logic (53,215 hex literals in all, long tail) | per unit | | Sonnet |
| 7 | SDK placeholders (479 asm-only functions, about 150 of them called from game C) | optional (user scope) | | Sonnet |
| 8 | Final sweep (`count_names.py` residue per layer where game logic must be clear and the rest is listed; `include/main_api.h` and `include/game.h` deleted once every C file includes its topic headers, with CODING_STANDARDS section 8a and `tools/check_headers.py` moved to the topic headers; living wiki pages `source-files`, `game-state`, and `data-types` refreshed) | | 1 | Opus |

Steps 1 and 2 wait for step 0. No rename of any kind lands before the tool merges, and once it lands, `tools/rename.py` is the only way to rename anything (user decision, 2026-10-24). Inside every unit, work proceeds in order through functions and globals, types and fields, one-line English comments on O.BIN names, the header split, and finally file renames, each in its own commit. File names come last because they require understanding the file contents.

Step 2's enum list differs from the audit's on purpose. It adds place and club (whose orders are known from the name tables in `GLOSSARY.md`), the game mode (GameState 0x1108), and the day-event type. It leaves out the month, a number from 1 to 12 that the code compares and counts with. It also leaves out the selector bytes at 0x1109, 0x110A, and 0x110D, which `game_mode_set` clears when it sets the game mode. Step 2 names the first two fields (`mode_stage`, `mode_step`), whose values carry different meanings in each mode. Each mode's unit gives them constants, while 0x110D waits for the units that use it.

### Main modules (step 3)

| unit | files today | lines |
|---|---|---|
| M1 | `80041000`-`800451D0` (6 files) covering boot, game loop, pad, 2D drawing, CD | 2.4k |
| M2 | `80046500`-`8004C3F0` (6) covering CD helpers, tpage, sprites, primitives, layers | 2.5k |
| M3 | `8004E500`-`80053650` (4) covering kanji text, menus, speaker labels, memory card | 3.0k |
| M4 | `80056300`-`80061710` without `8005A0B0` (8) covering movies, backgrounds, 3D, mouse, title | 2.0k |
| M5 | `8005A0B0` covering system menus, clubs, rumors | 3.0k |
| M6 | `80062CD0`, `800674B0` covering HUD | 3.2k |
| M7 | `8006CB30`, `800737A0` covering scheduler, school-day loop | 3.2k |
| M8 | `80075320`-`80079B10` covering days off, overlay loader, dates | 3.1k |
| M9 | `8007C030`, `80085E30` covering girl selection, faces, dialogue, fades | 3.5k |

### Overlays (step 4)

Two passes of one overlay run back to back because they share the header and the symbol file. A near copy is named after its original and reuses the original's names under its own prefix. GYOZI follows SHOUGATU, EVENT follows DATE, and BUNKAKEN follows BUNKASAI.

| pass | overlays |
|---|---|
| large, two passes each | BUNKASAI, BUNKAKEN, RPG_BAT, DATE, EVENT, TACO |
| single | ETC, NAME_ENT, TAIIKU, GEKO, SHOUGATU, GYOZI |
| TT | only after agreement with its contributor |
| pairs | TEL + OPTION, MASTER + ENDING, BUNKA_SD + SHUGAKU, KANGEI + DATE2 |
| group | EN_NICHI + OLH + OMIMAI + RENSYU + VALEN |

Overlays marked "confirm" in the ADR map (BUNKAKEN, ETC, GEKO, KANGEI, SHOUGATU, TT, VALEN) check their contents with the emulator before adopting their folder name and prefix. EVENT and GYOZI ("confirm unused") check for a loader again before keeping `_unused` in their folder names ([[tickets/T-0200-event-gyozi-loader-and-address]]).

## Unit loop

1. Claim the ticket (from `wiki/tickets/_template.md`) and set it to In Progress.
2. The namer reads the code, `GLOSSARY.md`, and the maps, re-checks the non-O.BIN names in the range (list below), and writes a rename table in the `tools/rename.py` format. Every row's note gives the evidence (callers, data read, mode set, the string label passed, or an emulator run). Any meaning that the evidence does not prove goes in as a hypothesis.
3. Run `rename.py plan` until it reports no collision.
4. Opus reviews the table (below) before anything is applied.
5. Run `rename.py apply`, then run the clean build `rm -rf asm build; configure; ninja` under `nice -n 19`, with at most two builds on the machine at a time and Docker `--cpus=2`. The required proof is 27/27 SHA-1 and `ninja progress` remaining at 6,952.
6. Commit one table per commit, placing the header split and each file rename in their own commits. Running `apply` appends each row and its note to the unit's committed ledger (`config/renames/<unit>.txt`), keeping the evidence next to the name.
7. Move the ticket to In Review, run the code-review skill against `CODING_STANDARDS.md`, and then move it to Done.

Overlay units do not rename main symbols directly. They add requests to `config/renames/requests.txt`, and the owner of the main module applies the rename.

## Review rule

Under the final phase review rule set by the user on 2026-10-24, when implementation is ready for final review, a `gpt-6-astra` agent reviews Phase 2 as a whole. A separate `gpt-6-astra` agent resolves that review's findings, the reviewer verifies resolution, and the final clean matching proof runs before Phase 2 is reported complete. This gate is tracked in T-27055 and T-27056. It supplements unit reviews and is not used on routine ongoing batches.

Under the user authorization of 2026-10-24, after this final review, fix, verification sequence, and final matching check pass, push the reviewed changes to the repository and report the remote branch and commit. This authorization supersedes the earlier local-commit-only instruction.

Under the continuation model assignment of 2026-10-24, use Luna for bounded analysis and Sol 6.1 for harder analysis and independent review. This assignment supersedes the historical Sonnet and Opus assignments below, while the evidence requirements and review gates remain in force.

- Sonnet agents propose names, an Opus agent reviews every table before it is applied, and the code review checks the commits before Done.
- Every name requires evidence. The reviewer rejects any name whose note gives no evidence, any game-meaning claim without proof that is not marked as a hypothesis, and any `unk_` field renamed from a single use that shows nothing.
- Runtime evidence comes from the emulator ([[emulator]]) through breakpoints, memory watches, call stacks on main and overlay code, scripted input, and save states. The note records the run (subcommand and arguments) rather than a screenshot; save states and screenshots are game data and stay out of the repository.
- Hypotheses stay marked (in the HYPOTHESES symbol file or with `/* HYPOTHESIS: ... */`) until a later run proves them, at which point the proving ticket removes the mark.

## Old names in the wiki

Per the user decision on 2026-10-24, tickets, logs, and old notes keep the names they were written with as history. Current pages (status, playbook, specs, and the Phase 2 pages) use the new names, and the unit that owns a topic refreshes them. `tools/rename.py lookup` and an old-name column in the function catalog ([[notes/functions]]) map each old name to its new one.

## Names to re-check

These names existed before Phase 2, are not O.BIN names, and break the naming conventions. The unit that owns the range renames them through `tools/rename.py` with its first table; the list is not exhaustive, and each unit also checks its own range.

| name (address) | unit | conflict |
|---|---|---|
| `file_checksum_ok` (0x8004636C) | M1 | module prefix is `cd_` |
| `glyph_expand_1bpp` (0x8004F4F0), `font_glyphs` (0x800B6D50), `font_range_table` (0x800B402C) | M3 | module prefix is `text_` |
| `system_settings_init` (0x8005DC4C) | M5 | prefix is `settings_` |
| `appointment_find_today` (0x8006AEC4), `club_list_menu` (0x8006B648) | M6 | topics of the days-off and system-menu modules; HUD file takes `hud_` |
| `special_event_select` (0x8006FB4C) | M7 | bare `event`; stems are `schedule_` and `day_event_` |
| `telephone_class_init` (0x800782F0) | M8 | `phone` is the TEL overlay's stem; days-off code uses `holiday_` |
| `date_event_trigger` (0x801326B0), `date_event_choose` (0x8013272C) | DATE | bare `event` |
| `shougatu_stage_dispatch` (0x8013D180) | SHOUGATU | prefixes are `new_year_` and `christmas_`; `stage` is a trap word |

The T-27004 review fix on 2026-10-24 ([[tickets/T-27004-shared-core-names]]) renamed `main_state_set` to `game_mode_set`, and renamed `dialogue_render`, `dialogue_speak_part`, and the `Dialogue...` types to `talk_render`, `talk_speak_part`, and `Talk...`.

The identifiers `ovl_load_buf`, `ovl_load_lba`, and `girl_name_buf`, the parameter names `src` and `dst`, and the word `init` pass because the glossary lists `buf`, `lba`, `src`, `dst`, and `init`.

## Open points

- TT names and folder require agreement with its contributor first.
- The hypothesis file names in the ADR (`config/symbol_addrs_hypotheses.txt`, `config/symbols_<DISC>_hypotheses.txt`, and `config/symbol_addrs_main_hypotheses.txt` for main names in EVENT's range) must match what T-27002 implements; its `tools/symtab.py` (branch w19-rename, In Review) uses the same names.

## Publication (2026-10-25)

Reviewed Phase 2 source was pushed to `origin/main` at `182af9d13d418b159973e59fbeaedf5e39f87902`. T-27000 is Done, and asset work remains tracked separately.
