---
status: accepted
date: 2026-10-24
---

# Phase 2 naming rules

This record defines Phase 2 English naming backed by evidence for symbols, files, and headers. The following counts
describe the pre-Phase-2 naming baseline surveyed in `drafts/T-25900/audit.md`; they do not describe the current
working tree:

- Pre-Phase-2 baseline: 6,649 functions were `func_XXXXXXXX`, and nearly all data labels were `D_XXXXXXXX`.
- Pre-Phase-2 baseline: 65% of struct members were `unk_XX`, and 70% of type names described a size or an address.
- Pre-Phase-2 baseline (2026-10-24): all 437 C files carried address names and overlay headers carried disc names.

Phase 2 makes the code readable for someone who has never seen the binary. Its scope was set for a planned
Godot port, which was later removed. The user settled six questions and the scope on 2026-10-24. This record states them, the
conventions that follow from them, and their costs. A rename never changes an output byte, and every rename
batch ends with the 27/27 sha1 build that proves it.

Current path and header status (2026-10-25): the reviewed T-27074 table has been applied to the working source
tree. The current overlay path map is in [[source-files]]; the 26 disc unit identities and their assembly and
build roots remain intact. All overlay folders use the accepted logical names except `src/ovl/TT/`, which
retains the technical TT spelling pending contributor agreement. EVENT and GYOZI are named `date_spots` and
`annual_events` from their reviewed content evidence; this does not assert that either overlay is loaded or
unused. The `confirm` gates and HYPOTHESIS notes for GEKO, KANGEI, SHOUGATU and VALEN remain. `main_api.h` and
`game.h` are retired; the 70 registered topic-owner paths and 21 required type paths are listed in
`config/header_owners.json`.

## Decisions

1. Name everything from what the code and the emulator show. A name that says what the code
   does (`text_glyph_advance`, `sprite_list_clear`) needs no marker. A name that claims game meaning the code
   does not prove (which girl, which event, which screen) is a hypothesis. Hypothesis names of functions and
   globals go into a HYPOTHESES symbol file; fields, types and constants get a `/* HYPOTHESIS: ... */`
   comment. Only fields that are unused, or whose use shows nothing, stay `unk_XX` or `pad`.
2. Keep the developer names from the O.BIN symbol table (`hizuke_init`, `k_disp_start`, `sndi`, ...)
   as they are. Each game-side one gets a one-line English comment saying what it does. New names are
   English.
3. Rename files, folders and headers with the accepted path map, such as `src/ovl/TAIIKU/80133C80.c` to `src/ovl/sports_festival/runner_race_dispatch.c`,
   and give every other file and folder an English name the same way. The disc names (`TAIIKU.EXN`) stay in
   the build config; [[source-files]] is the current overlay path map. Headers are split by topic and named for
   what they declare. They replace the large `include/main_api.h` and the one header per disc file, and each C
   file includes the headers it uses. decomp.dev restarts its per-file history for renamed files; the user
   accepted that.
4. Use English game terms everywhere. Terms with no good English equivalent stay as the game has them: the
   heroines by given name (`shiori`), the tokimeki meter, and a few event and culture words (`hatsumode`,
   `omikuji`). `GLOSSARY.md` at the repository root maps every term to its identifier stem.
5. Once `tools/rename.py` (T-27002) has merged, it is the only way to rename a function, global,
   field, type, file or header, and any constant, local or parameter as well. No hand edit and no
   search-and-replace. Until it has merged, no rename lands. CODING_STANDARDS section 4 states this as the
   binding rule. (User decision, 2026-10-24, audit question 5.)
6. Keep old names in tickets, logs and old notes as history.
   Current pages (status, playbook, specs and the phase 2 pages) use the new names. `tools/rename.py lookup`
   and an old-name column in the function catalog (`wiki/notes/functions.md`) map an old name to its new one.
   (User decision, 2026-10-24, audit question 2.)

## Scope

The user set the scope on 2026-10-24: port-first, "not 100%, just enough for the Godot port" (the port was
removed on 2026-10-31). The matching decompilation stays exact: all 27 sha1 files match and `ninja progress` stays at 6,952 functions after
every change. Naming goes as deep as the port needs:

| layer | what it covers | depth |
|---|---|---|
| Game logic | GameState and its records, enums, calendar and schedule, parameters, girls with affection and tokimeki, day events and scenes, dates, confession and endings, save data, each overlay's state machine and data tables, and the files and headers of these modules | Named and explained in full. Every function, global, field and type gets an English name backed by evidence, and a field stays `unk_XX` only when its use shows nothing. Where a rule is not plain from the code (a formula, a threshold, a transition), a comment at the definition or the unit's port spec in `wiki/spec/` states it. |
| PS1 hardware layer | GPU, sprites and VRAM; the sound driver; CD and XA; MDEC | Module level and entry points. The module's files and header are named, and so is each function that game logic calls. Internal functions, globals and fields may keep placeholders. |
| Locals and literals | local variables, parameters, hex literals | Named only where essential, where the local or the literal carries game logic (a girl index, a place id, a threshold, a flag bit). Hardware-layer code may keep m2c names and raw hex. |
| SDK | PsyQ library functions and data | Optional. |

Accepted exception: about 150 SDK-range functions that no symbol file names are called about 1,200 times from
game C. They stay number-named unless a unit names one, and the user accepted that on 2026-10-24 with "SDK
names optional". The SDK-named placeholders that overlays still use (`strcpy` as `func_800AE0F0`, `rand`,
`rsin` and about 25 more) are not part of the exception. They are alias debt that T-27003 clears.

## Conventions

These rules apply to every phase 2 unit. CODING_STANDARDS.md sections 4 and 5 hold the short form that
reviews check.

### Case and form

- Functions, globals, struct fields, locals and parameters use `snake_case`. Types and struct tags use `PascalCase`.
  Enum constants and macros use `UPPER_SNAKE`, starting with the enum's or the module's prefix (`GIRL_SHIORI`).
- English words, American spelling (`color`, as in the SDK and O.BIN's `make_color_bar`). Abbreviations only
  from the glossary's abbreviation table (`param`, `ovl`, `bg`, `cd`, `gpu`, `vram`, `vsync`, ...). A unit that
  needs another abbreviation adds its row in the same commit as the first name that uses it.
- No address or size inside a new name, no leading underscore, no C or IDO keyword, nothing that collides
  with an SDK, libc or macro name. Names are unique without regard to case inside a link unit, because splat
  writes one `.s` file per function and macOS file systems ignore case.
- No `g_` prefix, because no module uses one.

### Prefixes

- A function, global, type or constant that belongs to an overlay starts with that
  overlay's prefix from the map below (`sports_`, type `Sports...`, constant `SPORTS_`). An overlay that holds
  several unrelated scenes has one prefix per scene, all listed in the map. Two overlays may reuse a name,
  but the prefixes make that rare.
- A valid prefix is a glossary stem, a listed abbreviation, or a row of the Module words table in
  `GLOSSARY.md`. That table holds the plain English words for modules (`boot`, `sprite`, `help`) and the short
  forms of stems (`sports` for `sports_festival`, `trip` for `school_trip`), each with its module. Every prefix
  in the two maps below meets this rule. A unit that needs a new prefix adds it to its map and, if it is not a
  stem or an abbreviation, to the Module words table, in the same commit as the first name that uses it.
- A game-term word, one for a thing in the game's world or rules (a girl, place, club,
  parameter, event, scene), comes from the glossary, and its trap row says which sense it has. An ordinary
  technical word (`list`, `clear`, `advance`, `glyph`, `buffer`) is plain English and needs no row. An
  abbreviation comes from the abbreviation table. Review checks game-term words and prefixes, not ordinary
  words.
- Each prefix belongs to one row of one map, and only that row's unit creates names with it. A
  function that sits in one module's file but has the topic of another module (`appointment_find_today`, a
  function of the HUD file `800674B0` with the days-off module's topic) takes its own module's prefix when it
  is renamed.
- Each main executable module uses the subsystem prefix from the table below, with no unit prefix in
  front; O.BIN names keep their own stems (`hizuke_`, `k_disp_`, `normal_date_`). `date_` belongs to the DATE
  overlay; main's dating code uses O.BIN's `normal_date_`. The one main name with a `date_` stem,
  `date_sasoi_judge` (0x80070850), is an O.BIN name and stays, as every O.BIN name does. The sound driver
  is hardware layer. Game code that plays music or effects puts the stems `bgm` or `se` after its own
  module's prefix (`title_bgm_start`). `sound_` is not a main prefix (the OPTION overlay owns `sound_test_`),
  and `snd_`, `Ss` and `Spu` are SDK names. The driver's own commands, bank loads and work area take `audio_`
  (T-27004 review: `audio_command`, `audio_state`).
- The mode handler function that runs one game mode takes the prefix of its screen's module and ends in
  `_mode_handler` (`title_mode_handler`, `holiday_mode_handler`); the bare word `mode` is a trap word.
- The two tables below are the starting maps. A unit that needs another prefix, or a different folder name,
  changes the row in the same commit as the first name that uses it; review checks it.

### O.BIN names and names already in the tree

- O.BIN names are the entries of `config/symbol_addrs_obin.txt` and nothing else. They stay as they are.
- A game-side O.BIN name gets its one-line English comment at its C definition; every game function has one.
  The unit that owns the file writes it. The O.BIN names in the SDK region (77 functions and all 22 data
  labels, which are SDK bss such as `_snd_openflag` and `RingBase`) have no C definition. Their comment is
  optional, like all SDK names, and goes on the name's line in `config/symbol_addrs_obin.txt`.
- Every other name that exists before phase 2 follows the conventions, including the English-patch names (T-11090, in
  `config/symbol_addrs.txt` and `config/symbols_<DISC>.txt`, such as `talk_render` and `game_mode_set`, renamed from the patch names `dialogue_render` and `main_state_set` by the T-27004 review fix)
  and the names that matching tickets gave. Each unit re-checks the non-O.BIN names in its range against the
  glossary, the maps and the abbreviation table. A name that fails (an unlisted prefix, a bare trap word such
  as `event`, an abbreviation outside the table) is renamed through `tools/rename.py` in the unit's first
  table, with its evidence. A name that passes stays. `wiki/phase2.md` lists the failing names known today.

### Evidence and hypotheses

- Every rename row carries its evidence in the note column of the rename table (T-27002). The evidence is
  the callers, the data read, the mode set, the string passed (by label, never quoted), or an emulator run
  (`wiki/emulator.md`). After `tools/rename.py apply`, the unit's committed ledger
  (`config/renames/<unit>.txt`) holds each row with its note. A later review finds the evidence there.
- Hypothesis files, each with `HYPOTHESES` in its header: `config/symbol_addrs_hypotheses.txt` for main,
  `config/symbols_<DISC>_hypotheses.txt` for an overlay. Every yaml loads the main file, as it loads
  `config/symbol_addrs.txt`, so overlay asm uses the same names. `config/symbol_addrs_obin.txt` stays a separate
  HYPOTHESES file, because O.BIN names are hypotheses about the developers' names, not descriptions.
- EVENT's range is an exception. EVENT loads at 0x800F6000, so its file covers the main bss 0x800F6000-0x80126000
  (`wiki/overlays.md`). A main name for an address in that range, in a file that every yaml loads, would label
  EVENT's own data too, as `cd_read_status` did (audit gap G3). Such names go to the main-only files
  `config/symbol_addrs_main.txt` and, for hypotheses, `config/symbol_addrs_main_hypotheses.txt`. Only the
  main yaml loads them. The header of `symbol_addrs_main.txt` still says it holds facts that are not names.
  The first name that lands there changes it. `tools/check_symbols.py` checks every symbol file
  that a yaml loads, the hypothesis files included, and rejects a main name in EVENT's range in any file that
  EVENT loads (T-27002, T-27003).
- A name leaves its HYPOTHESES file, or loses its comment, when the code or an emulator run proves it; the
  evidence goes into the ticket.

### Fields and types

- Struct layouts keep their `/* 0xNN */` offset comments and the `/* size 0xNN */` line.
- A type loses its size or address suffix once it has a role name (`Rec38` becomes `GirlRecord`,
  `DisplayActor44` becomes `DisplayActor`). Union views keep one short name per view (`GsWord.w`, `.h`, `.b`).
- A field is named for its use when the code shows the use (written once a frame and compared to a limit, such as
  `frame_timer`), even before the game meaning is known.

### Enums and constants

- `enum` for closed sets (girls, places, clubs, parameters, game modes, day-event types), `#define` for bit
  masks, sizes and counts. SDK names where the SDK has them (GPU primitive codes, CD commands).
- A field or variable keeps its declared fixed-width type; an `enum` type would make it 4 bytes and signed and
  change the code. The field gets a comment naming the enum: `/* 0x0F5F */ u8 current_girl; /* GirlId */` (the enum's name, as `rename.py const` writes it).
- A literal is replaced only where it is tied to a named site, meaning compared with, assigned to, a `case` of, or
  passed as an argument that has the enum's meaning. Bit patterns and offsets are written in hex, counts in
  decimal. Game logic gets this treatment (the scope table); hardware-layer code may keep raw hex.

### Locals and parameters

- A local or parameter that carries game logic gets a role name (`girl`, `sprite`, `sector`); `i`, `j`, `k`
  are loop counters. A name that is given is never an m2c name (`var_s0`, `temp_v0`, `sp10`, `arg0`, `phi_*`).
  Locals that carry nothing the port needs, and hardware-layer code, may keep m2c names.
- A temporary that exists only to make the match keeps its `FAKE` comment and gets a name that says so
  (`fake_order`). Header prototypes use the same parameter names as the definition.

### Files and folders

- A C file is named for what its original object does (`runner_race_dispatch.c`, `cd_read.c`), in `snake_case`, with
  no address; stems are unique without regard to case in their folder. The first comment of the file keeps
  the address range and points to `config/objects/<DISC>.txt` (`config/objects/main.txt` for the
  executable), which keeps the original object boundaries.
- Overlay folders take the English names of the map. The splat yaml keeps the disc name in
  `name: ovl_TAIIKU`, `basename`, `target_path`, `asm_path` and `build_path`. What changes is the name of the
  object's subsegments, namely the `c` subsegment and the `.rodata` and `.data` subsegments that carry the same name
  (`TAIIKU/80133C80` becomes `sports_festival/runner_race_dispatch`). The `<DISC>_data_<address>` subsegments keep
  their names.
- Because `src_path` is `src/ovl`, splat writes the object's asm under the new path inside the disc-named
  root (`asm/ovl/TAIIKU/data/sports_festival/runner_race_dispatch.data.s`), and the `INCLUDE_ASM` and `INCLUDE_RODATA`
  path strings in the C file change with it. `tools/rename.py` makes these edits for `file` and `dir` rows.
- A file or folder is named only from content that the code or the emulator proves. A name that claims
  game meaning beyond that (`runner_race_dispatch.c` for code whose use is unknown) is a hypothesis; the file's first
  comment carries `HYPOTHESIS: <what is unproven>`, and a folder keeps the `confirm` status of the map below
  until a unit proves it.
- A file rename is a commit of its own with nothing else in it, so git keeps the history. It comes last in
  each unit, once the contents are understood.

### Headers

- A header is named for what it declares: `include/main/<module>.h` for each main module (the symbols its C
  files define), `include/game/<topic>.h` for shared game types (`game_state.h`), `include/ovl/<folder>.h` (or
  `include/ovl/<folder>/<topic>.h` for a large overlay) for overlay symbols. SDK headers keep their SDK names.
- Include guards follow the path, so `include/main/cd.h` uses `MAIN_CD_H`.
- Each C file includes the headers whose symbols it uses. During the transition, `include/main_api.h` and
  `include/game.h` were umbrellas. They have now been retired; `config/header_owners.json` records the
  70 canonical owner paths and 21 required type paths. The one-declaration rule of CODING_STANDARDS section
  8a applies to whichever registered header declares the symbol.
- Current ownership checks and `tools/check_headers.py` operate with the topic headers and without either
  umbrella. The old umbrella names in earlier phase-two evidence refer to the transition design.
- A header split is a pure move; `tools/sync_protos.py --snapshot` before and `--compare` after show that no
  file sees a different type for any symbol.

### Disc-name map

The current overlay source-path inventory is [[source-files]]. The main object and prefix map remains in
`src/main/README.md`. "Confirm" means the unit checks the contents (code and emulator) before it adopts a
folder name and prefix; until then the name is a hypothesis. Historical loader research for EVENT and GYOZI
remains in [[overlays]] and T-0200, but the current names do not assert either unit is loaded or unused.

| disc file | contents | folder | prefix | status |
|---|---|---|---|---|
| BUNKAKEN.EXN | culture festival variant that also visits six culture clubs | `culture_festival_tour` | `tour_` | |
| BUNKASAI.EXN | culture festival (exhibits, concert, speech contest, game corner) | `culture_festival` | `culture_` | |
| BUNKA_SD.EXN | culture festival stage shows played by SD sprites | `culture_stage_shows` | `stage_show_` | |
| DATE.EXN | dates at the 21 places, girl-specific date scenes | `date` | `date_` | |
| DATE2.EXN | amusement park date with rides and karaoke | `amusement_park_date` | `amusement_` | |
| ENDING.EXN | graduation ending and the girls' futures | `ending` | `ending_` | |
| EN_NICHI.EXN | festival stall games (goldfish scooping, shooting gallery) | `festival_stalls` | `stalls_` | |
| ETC.EXN | exams and rank list, career guidance and choice, album, staff roll, last photo, clear save, shared helpers and unused confession/epilogue stubs | `misc_scenes` | `exam_`, `career_`, `album_`, `system_file_`, `staff_roll_`, `last_photo_`, `clear_save_`, `confession_`, `epilogue_`, `misc_` | modes 0xC2-0xC4 source-confirmed; folder migration separate |
| EVENT.EXN | an older set of date-place scenes | `date_spots` | `spot_` | |
| GEKO.EXN | meetings outside dates (walking home, invitations, birthdays) | `girl_encounters` | `encounter_` | confirm |
| GYOZI.EXN | parallel seasonal-event scenes | `annual_events` | `annual_` | |
| KANGEI.EXN | club rooms, a lodge, a test of courage; probably the summer camp | `summer_camp` | `camp_` | confirm |
| MASTER.EXN | club tournaments and prizes after mastering a club | `club_master` | `master_` | |
| NAME_ENT.EXN | file select screens, name, birthday and blood-type entry; the attract opening; the voiced prologue | `name_entry` | `name_entry_`, `file_select_`, `opening_`, `prologue_` | |
| OLH.EXN | help pages | `help` | `help_` | |
| OMIMAI.EXN | sick-bed visits | `sick_visit` | `sick_visit_` | |
| OPTION.EXN | options, sound test, debug menu, extras entry points | `options` | `options_`, `sound_test_`, `debug_menu_` | |
| RENSYU.EXN | club practice match | `practice_match` | `practice_` | |
| RPG_BAT.EXN | RPG-style battle mini game | `rpg_battle` | `rpg_` | |
| SHOUGATU.EXN | New Year shrine visit and the Christmas party | `new_year_christmas` | `new_year_`, `christmas_` | confirm |
| SHUGAKU.EXN | school trip | `school_trip` | `trip_` | |
| TACO.EXN | hidden 3D shooter | `shooter_3d` | `shooter_` | |
| TAIIKU.EXN | sports festival races and folk dance | `sports_festival` | `sports_` | |
| TEL.EXN | phone calls and the girl profile page | `telephone` | `phone_` | |
| TT.EXN | hidden 2D shooter; branded identification unconfirmed | `TT` (technical disc namespace only) | `tt_` (technical disc namespace only) | folder and English game name await contributor agreement |
| VALEN.EXN | Valentine's Day (White Day probably too) | `valentine` | `valentine_` | confirm White Day |

### Main subsystem prefixes

The living main object and prefix map is now [`src/main/README.md`](../src/main/README.md),
following the logical C filename migration. It retains the grouped prefix ownership and shared-data
entries from this decision. Body-backed object attribution separates the sound driver at
`80079B10` from the scene library at `8007C030`, and executable reload from STR playback. These
corrections preserve grouped ownership. The living map also assigns one boot/startup group, one debug subgroup, and bounded mixed-object audio helpers after source review.

## Out of scope

- Moving `INCLUDE_RODATA` data into C definitions, removing `FAKE` code and `*_OVERRIDE_*` views, and splitting
  functions are phase 2 work, not naming. They follow their own tickets.
- Names in the PsyQ SDK region (479 asm-only placeholders) are optional (see Scope).
- Game text in names or comments beyond proper names and short terms (CODING_STANDARDS section 10). Translating
  the game is phase 5.
- Disc and build artifacts keep their disc names, including the yaml fields listed under Files and folders, the asm root
  `asm/ovl/<DISC>/`, `build/ovl/<DISC>.bin` and the sha1 files. Object paths inside the asm root follow the
  file names.

## Consequences

- Renames go through one table-driven tool (`tools/rename.py`, T-27002) that edits symbol files, sources and
  configs, keeps a ledger and replays it on branches and contributor PRs. Once it has merged it is the only
  way to rename (decision 5); until then no rename lands and placeholders stay. CODING_STANDARDS section 4
  says the same.
- Every main rename touches overlay files, so main units and overlay units coordinate through the ledger; an
  overlay unit asks for a main rename instead of making it.
- The right yamls must load the hypothesis files; main names must reach the overlays' asm (the O.BIN alias
  debt, T-27003) before the shared core is named.
- Splitting `main_api.h` changes what each file sees; `tools/check_headers.py`, `tools/sync_protos.py` and
  the matching-era tools that hardcode `main_api.h`, `D_800E6280` or `unk_` names have to learn the new layout
  or move to `tools/legacy/`.
- decomp.dev per-file history restarts for renamed files; totals stay.
- Agents will want to name a field after one use. Review rejects a name without evidence and keeps the
  hypothesis marks honest.

## 2026-10-24 prefix closure

ETC retains one owner row including its source-backed ending-screen/stub subgroups; `misc_` is the existing English abbreviation for shared helpers. TT uses unexpanded technical `tt_` solely to distinguish TT.EXN from TACO's `shooter_`; no branded English title, English folder or contributor agreement is claimed. The reserved English TT term remains pending. Main boot/debug/audio mixed-object exceptions are listed once in src/main/README.md. This resolves technical namespace ownership without changing object boundaries or adopting an unproved game identity.
