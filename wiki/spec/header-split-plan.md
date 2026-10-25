---
type: plan
updated: 2026-10-24
ticket: T-27052
---

# Topic header split: ownership, order and deletion gates

Design record for [[tickets/T-27052-phase2-completion-audit]]. The inventory and recommendations below were written before the topic-only header migration; retain them as design history and use the current-state note below for implementation status. [[phase2]], `docs/adr/0001-phase-2-naming.md:198` and `CODING_STANDARDS.md` section 8a remain binding.

## Current topic-header and path status (2026-10-25)

The topic-only ownership migration is integrated in the working tree. `include/main_api.h` and `include/game.h`
are absent. `config/header_owners.json` registers 70 canonical owner paths and 21 required type paths; the
header guards pass with no umbrellas. This supersedes the older recommendation below to keep forwarding
umbrellas until a later deletion gate. The registered owner list, not the pre-migration inventory tables,
defines current ownership.

T-27074's 56-row overlay path table has been applied to the working tree: 27 directory moves and 29 header
path moves. The current folder and root-header map is in [[source-files]]. The EVENT root is
`include/ovl/date_spots.h`; the GYOZI root is `include/ovl/annual_events.h`. These paths describe content and
do not assert that either overlay is loaded or unused. TT remains `src/ovl/TT/` pending contributor agreement;
its technical header is `include/ovl/tt.h`. GEKO, KANGEI, SHOUGATU and VALEN confirmation/HYPOTHESIS gates
remain as recorded in the accepted map.

The 2026-10-24 inventory statement that no source, tool, config or header changed applied only to that
read-only audit snapshot. Address-based and disc-header paths in its tables remain evidence anchors for that
snapshot; current code paths follow the map in [[source-files]] and `config/header_owners.json`.

The first port slice needs explicit state records, calendar, parameters, scheduler, school-day and appointment interfaces. The human's latest priority is logical C filenames and header breakdown. Start with the bounded two-object/three-header pilot below, keeping the existing shared record closure available; full shared-record extraction follows separately. Header ownership follows the symbol's defining module or actual binary storage owner; a function's callers do not become its owner. File naming follows the original object boundary even when one object contains several topics.

## Measured baseline and method

Read-only inventory ran in existing image `tokimemo-decomp-arm64` (`sha256:4eb5a43f07957beae672ee446446ddda1478e2e25b80a8c50a2b9833a0186fd1`), Docker `--cpus=2`, repository mounted read-only, `PYTHONDONTWRITEBYTECODE=1`. Ignored evidence: `.claude/orchestrator/state/audit27052-header-inventory.py` and `audit27052-header-inventory.txt`. It uses `check_headers.declarations_text`, `sync_protos.Model` and `users_of`; counts are syntactic declarations, not distinct addresses, and include declarations under override guards. Typedef counts are lexical `typedef` occurrences. The include-user counts are static quote-include closures, not evidence that a file uses every declaration it receives.

| header | lines | function declarations | object declarations | typedefs | C files reaching it |
|---|---:|---:|---:|---:|---:|
| `include/common.h` | 18 | 0 | 0 | 6 | 437 |
| `include/game.h` | 463 | 0 | 0 | 29 | 406 |
| `include/main_api.h` | 3,176 | 835 | 1,230 | 44 | 437 |
| `include/game_enums.h` | 300 | 0 | 0 | 0 | 437 |

There are 437 C files in the scanned tree, 26 overlay headers, and 186 `MAIN_API_OVERRIDE_` definitions across 112 header/source locations. These are definition occurrences, not 186 distinct symbols or proof that every override is needed. Overlay-specific override families are additional. The existing read-only command `python3 tools/check_headers.py include src` passed with `headers OK`; output is in ignored `audit27052-header-check.txt`. No build, clean, `--fix`, `--write`, apply or rename was run.

Current include edges:

```text
common.h -> include_asm.h
libgpu.h -> common.h
libgte.h -> common.h
psx_types.h -> libgpu.h, libgte.h
main_api.h -> common.h, libgpu.h, libgte.h, game_enums.h
game.h -> common.h, libgpu.h, main_api.h
ovl/<DISC>.h -> game.h and/or main_api.h; selected ones also psx_types.h/libgpu.h
```

No literal cycle exists in this inspected graph. Future cycles arise if an API header includes `game_state.h` just because its implementation reads `game_state`, or if state includes memory-card APIs for save support. The declarations' actual type needs determine edges.

`game.h` is not one simple guard-delimited block: `GAME_H` closes at line 145 and the remaining types use separate guards through line 463. Move complete guarded blocks and comments, including macros such as `SRN_REC` (`game.h:447`); extracting only the first guard would lose 18 later typedefs. Do not normalize guards, bitfields, array/scalar views or pointer representations as part of a move.

## Dependency-ordered shared type owners

These destinations account for all 73 current typedef definitions in `main_api.h` and `game.h`. Names below are existing type names, including unresolved names; this plan does not authorize guesses or fold different access views together. New header names describe their contents. `game/` holds shared records; `main/` holds a module's private record/API declarations. Splitting a leaf record from its API prevents implementations' dependencies from entering the state layout.

| order / destination under `include/` | current definitions (file:line of first definition in each group) | prerequisites / reason |
|---|---|---|
| 0 `common.h` | six width typedefs (`common.h:6`) and INCLUDE_ASM selection | Keep foundational file and assembler wrapper; no game/SDK/API dependencies added. |
| 1 `game/word_views.h` | `WordBytes` (`main_api.h:30`), `BirthdayBits`, `WordView`, `HalfView` (`:132-164`), `NibbleView` (`game.h:311`) | `common.h`; preserve `BirthdayBits` before `WordView`. Shared physical views, not renamed semantic fields. |
| 1 `game/girl.h` | `LegacyGirlFlags`, `LegacyGirlRecord` (`main_api.h:101-110`), `GirlFlagBits`, `GirlFlags`, `GirlRecord` (`:228`, `:276`, `:289`) | word views; flags before records; keep legacy layout distinct. |
| 1 `game/calendar.h` | `MonthCalendar` (`main_api.h:379`) | width types only; enum/constants below also live here. |
| 1 `game/param.h` | `ParamValue` (`main_api.h:386`) | width types; keep nine separate GameState fields, not a new array. |
| 1 `game/club.h` | `ClubRecord` (`main_api.h:392`), `ClubFlagBits` (`game.h:341`) | width types; word/bit views remain distinct. |
| 1 `game/appointment.h` | `AppointmentEntry`, `AppointmentSlots` (`main_api.h:147-156`), `AppointmentDate`, `AppointmentBits` (`game.h:354-363`) | word views; entry before slots. |
| 1 `game/day_event.h` | `DayEvent`, `EventLogEntry` (`main_api.h:399-407`) | width types; queue and log records have different layouts. |
| 1 `game/player_flags.h` | `QuarterFlagBits` (`main_api.h:255`), `LastCommandBits`, `PlayerFlagBits`, `ScheduleGirlFlagBits` (`game.h:139`, `:324`, `:397`) | width types; preserve casts/bitfield widths, no new union. |
| 1 `game/menu_records.h` | `MenuItem` (`main_api.h:419`) | width types; shared because embedded in GameState. |
| 1 `game/draw_records.h` | `PrimArena` (`main_api.h:371`), `PaletteLoad`, `CsrLoad` (`:429-438`), `SpriteQuad` (`:304`) | width types; first three are embedded in GameState. Hardware records remain part of PS1 state layout. |
| 2 `game/game_state.h` | `GameState` (`main_api.h:450-612`), canonical `extern GameState game_state` (`:1042`) | word views, girl, calendar, param, club, appointment, day_event, menu_records, draw_records. Storage owner is main; this shared state interface is its single declared home. |
| 1 `game/system_data.h` | `AlbumParam`, `ShooterSettingsBits`, `SystemData` (`main_api.h:173-195`), `ExtraModeFlags` (`:1076`), canonical `extern SystemData system_data` (`:1046`) | width types. Do not absorb nearby globals or substitute `ExtraModeFlags` into the record without separate evidence/build. |
| 3 `game/save_image.h` | `SaveConfigBlock`, `SaveShooterSettings`, `SaveAlbumParam`, `SystemSaveImage`, `SystemDataSaveView`, `ProfileBlock`, `SceneFlagBlock`, `GameSaveImage` (`game.h:17-85`), `SaveBank` (`main_api.h:1875`) | word views, system_data, calendar, girl, day_event. Full GameState definition is unnecessary for these record definitions; comments can refer to its offsets. Preserve opaque SaveBank and typed images as distinct views. |
| 2 `game/album.h` | `AlbumSlotRecord` (`game.h:376`) | system_data for `AlbumParam`; no dependency on memory-card API. |
| 1 `game/actor.h` | `Actor`, `Fix32`, `ActorFixed`, `SpritePart`, `SpriteFrame` (`game.h:156-252`) | width types; Fix32 before ActorFixed, SpritePart before SpriteFrame. Keep `TaiikuActor44` overlay-local (signed-byte view). |
| 1 `game/color.h` | `ColorPair555` (`game.h:261`) | width types; keep bitfield packing. |
| 1 `game/layer.h` | `BgLayer` (`game.h:287`), `SrnLayer`, `ModTileSet` (`:422`, `:453`), `BgCell`, `BgMap`, `BgMapIndex` (`main_api.h:1742-1761`) | width types; BgCell before BgMap. Move `SRN_REC` to `main/layer.h` beside the canonical `D_80121750` declaration, so the record header has no global-macro dependency. |
| 1 `game/culture_festival.h` | `CultureFestivalDispatch` (`game.h:278`) | width types; used by BUNKASAI and BUNKAKEN, so neither overlay owns the shared definition. |
| 1 `main/audio.h` | `AudioState` (`main_api.h:329`), `AudioRequestPair` (`game.h:412`) | width types; `audio_state` (`main_api.h:1977`) has this canonical home with the driver entry points. |
| 1 `main/talk.h` | `TalkVoiceRef`, `TalkSelectors`, `TalkWork`, `TalkTables` (`main_api.h:39-87`) | width types; TalkVoiceRef before TalkTables. No state include required just for implementation access. |
| 1 `main/text.h` | `TextEntry` (`game.h:122`), `TextState`, `FontRange` (`main_api.h:756-769`) | width types; own `text_list_count` and `font_range_table` declarations (`:766`, `:773`). |
| 1 `main/memory_card.h` | `MemoryCardFileRequest` (`game.h:133`) | width types; include save_image only where a signature actually needs it; implementation includes state separately. |
| 1 `main/cd.h` | `CdCommand` (`main_api.h:1896`), `CdLevelWork` (`:3012`) | width types; canonical `cd_command_ring` (`:1901`) and game CD helpers. SDK CD entry points remain separately owned. |
| 1 `main/movie.h` | `StHEADER`, `StrDecEnv`, `StrCtx` (`main_api.h:1928-1954`) | width types and `libgpu.h` for embedded RECT; StrDecEnv before StrCtx; canonical `D_80125C58` (`:1962`). StHEADER keeps SDK spelling. |
| 1 `game/model_records.h` | `TaiikuBig` (`main_api.h:1774`) | width types. Shared 3D record, despite the existing name: main `80058D20.c:51-59`, main `80061710.c:46-54`, DATE `8014A1B0.c:315-328`, ETC `8013F980.c:13-28` / `80143240.c:94-108`, and TAIIKU use it. Canonical main-storage globals `D_801227A0`, `D_80122CA0`, `D_80122840`, `D_80122B10` (`main_api.h:1780-1781`, `:2063-2064`) go to `main/view.h`, beside main's 3D setup interface, and use this neutral shared record leaf. Keep the type spelling/layout until a separate rename batch. |

The sole dependency needing a complete GameState is its own layout. It embeds 13 other typedefs: `PrimArena`, `MonthCalendar`, `HalfView`, `ParamValue`, `GirlRecord`, `AppointmentSlots`, `ClubRecord`, `DayEvent`, `EventLogEntry`, `WordView`, `MenuItem`, `PaletteLoad`, `CsrLoad`. Pointer forward declarations cannot replace these by-value fields. `GameSaveImage` independently embeds MonthCalendar, GirlRecord, WordView and EventLogEntry, plus ProfileBlock/SceneFlagBlock. Separating these records solves the state/save ordering without API cycles.

The existing `actor_pool` and `actor_pool_96` declarations are `u8[]` under overrides (`main_api.h:1239-1241`, `:1357-1359`). Their new declaration owner is `main/actor.h`, which can include `game/actor.h` without changing either declaration to `Actor[]`. `sprite_quads` (`:1682`) goes to `main/sprite.h` using the unchanged SpriteQuad type. Moving a header is not an opportunity to improve the type. Likewise, `main/80061710.c:475` identifies TaiikuBig pool entries as GsCOORDINATE2 records, but this split does not replace their type or fields. The section 8a example treating TaiikuBig as an overlay-only main-bss type is stale against these cross-unit users and must be refreshed with the standards update.

## Enum and macro owners

`game_enums.h` contains 15 enum definitions and 19 object-like defines. It is currently a leaf but is imported by all 437 C files through main_api. Move complete definitions without changing the current tag names, values or storage fields:

| destination | current definitions |
|---|---|
| `game/girl.h` | `GirlId` (`game_enums.h:9`) |
| `game/param.h` | `Parameter` (`:29`) |
| `game/calendar.h` | `Weekday`, `Vacation`, `DayKind`, `CalendarMarker` (`:42`, `:102`, `:110`, `:118`) |
| `game/club.h` | `Club` (`:53`) |
| `game/place.h` | `Place` (`:69`); main place/name table externs may have this shared owner, recorded explicitly |
| `game/player_flags.h` | `HealthState` (`:94`), three `PLAYER_FLAG_` defines (`:296-298`) |
| `game/day_event.h` | `DayEventType` (`:140`) |
| `game/game_mode.h` | `GameMode` (`:175`) and `GAME_MODE_*` under revised semantic batch T-27043 active in integration; accepted meanings are opening 0x11, file select 0x13, name entry 0x14 and prologue 0x15. The first audit snapshot still read `MainMode`. Resolve names against the live ledger before extraction; this is accepted work being integrated, not an unresolved naming decision. |
| `game/input.h` | `InputDevice`, `VoiceMode`, `MenuMode`, `IconMode` (`:247-269`), 14 PAD and two MOUSE masks (`:276-293`) |

The mandatory ADR deletion gate covers `main_api.h` and `game.h`. Retiring `game_enums.h` after this distribution is a useful separate cleanup, not an extra prerequisite for the first port slice. During extraction it may include the new enum owners. It must not cause a cycle through any function API.

## Main API owners and work sequence

After leaf extraction, route every main declaration to the topic whose implementation defines it, checked against actual definitions, symbol tables and original object map. These are the concrete destination names for ADR module rows; the table does not infer ownership of unresolved data merely from a nearby address.

| phase2 unit | destinations under `include/main/` |
|---|---|
| shared port slice / M7, M8 | `schedule.h`, `day_event.h`, `school_day.h`, `holiday.h`, `appointment.h`, `game_mode.h`; state/record homes above precede these APIs |
| M1 | `boot.h`, `game_mode.h`, `pad.h`, `sys.h`, `draw.h`, `vram.h`, `cd.h`; `audio.h` for sound driver routines embedded in the drawing object |
| M2 | `cd.h`, `tpage.h`, `sprite.h`, `actor.h`, `gpu.h`, `prim.h`, `color.h`, `layer.h`, `model.h` |
| M3 | `text.h`, `menu.h`, `speaker.h`, `girl.h`, `memory_card.h`, `timer.h` |
| M4 | `movie.h`, `bg.h`, `view.h`, `mouse.h`, `title.h`; owners of `80059A20`/`80059B40` decide names from contents before file rows |
| M5 | `settings.h`, `club.h`, `rumor.h` |
| M6 | `hud.h`, `portrait.h`, `param.h`, `calendar.h`, `message.h` |
| M8 beyond port slice | `ovl.h`, `date.h` for O.BIN `normal_date_*` helpers |
| M9 and cross-object sound | `select_girl.h`, `face.h`, `sd.h`, `talk.h`, `name_code.h`, `scene.h`, `vsync.h`, `fade.h`, `cursor.h`, `audio.h` |

A topic header owns the symbols, not every function in a source file with a similar name. Audio spans `80043510` and `80079B10` in the ADR; calendar/parameter interfaces span HUD objects; this is why an address-range-only automatic partition is insufficient. Keep source files at original object boundaries. One object may include several topic headers and its original range comment can list several responsibilities.

SDK declarations require an explicit second classification. Existing `libgpu.h`, `libgte.h`, `libapi.h`, `psx_types.h` retain their names and ABI types. `libgpu.h:17` already declares `func_8009F004()`; `libapi.h:9-11` owns three kernel entry points. `sync_protos.SDK_HEADERS` currently exempts only libgpu/libapi (`sync_protos.py:67`). Preserve those canonical declarations. Known PsyQ/libc functions currently in main_api should move to the existing or appropriately named library interface (`libgte.h`, `libcd.h`, `libc.h`, etc.) only with verified library ownership. Residual unnamed SDK-range calls can temporarily live in `main/sdk_calls.h`, explicitly registered as SDK storage and excluded from game-logic naming completion. This is compatibility debt, not evidence of gameplay ownership. Adding an SDK header without teaching the checker its ownership/exemption would be rejected as a duplicate or moved back to main_api. Do not add a real vendor header to replace recovered prototypes during this split: its tighter signatures can change code generation.

## Overlay destinations and size

Preserve disc names as link-unit keys. Use the ADR English folder for new header paths; a header split does not establish the ADR's runtime hypotheses. Counts below are parsed own declarations (functions + objects), excluding included main declarations, and lexical typedef counts. Large headers merit topic leaves; small headers can use the ADR's single English header if their contents form one coherent interface.

| disc | current header declarations / typedefs | proposed `include/ovl/` destination / first topic boundary |
|---|---:|---|
| BUNKAKEN | 55 / 8 | `culture_festival_tour/{work,tour}.h` after confirmation |
| BUNKASAI | 125 / 9 | `culture_festival/{work,exhibit,visitor}.h`; table records at `BUNKASAI.h:21-66` belong with their defining table topic |
| BUNKA_SD | 172 / 1 | `culture_stage_shows/{stage_show,karuta}.h`; split only where definitions/spec establish distinct owners |
| DATE | 1,362 / 60 | `date/{work,scene,spot,talk}.h`; arrival (`DATE.h:553`) and section fillers (`:1523-1526`) provide explicit boundaries |
| DATE2 | 228 / 9 | `amusement_park_date/{work,rides}.h` |
| ENDING | 86 / 16 | `ending/{ending,epilogue}.h` if actual symbol uses warrant two leaves; otherwise `ending.h` |
| EN_NICHI | 109 / 9 | `festival_stalls/{goldfish_scooping,shooting_gallery}.h`, shared records in `work.h` |
| ETC | 1,230 / 4 | `misc_scenes/{final_exam,career,album,system_file}.h`; confirm modes 0xC2-0xC4 before committing folder claims |
| EVENT | 1,507 / 68 | `date_spots_unused/{work,spot,scene}.h`; retain link unit EVENT and verify unused status |
| GEKO | 779 / 15 | `girl_encounters/{work,walk_home,invite,birthday}.h`; confirmation remains pending |
| GYOZI | 836 / 13 | `annual_events_unused/{work,annual_event}.h`; keep distinct from SHOUGATU types despite similarity |
| KANGEI | 133 / 11 | `summer_camp/{work,club,test_of_courage}.h` after confirmation |
| MASTER | 124 / 9 | `club_master.h` or topic leaves for tournaments/prizes once the owning unit proves boundaries |
| NAME_ENT | 266 / 3 | `name_entry/{file_select,name_entry,opening,prologue}.h` |
| OLH | 118 / 0 | `help.h` |
| OMIMAI | 63 / 2 | `sick_visit.h` |
| OPTION | 112 / 5 | `options/{options,sound_test,debug_menu}.h` |
| RENSYU | 25 / 4 | `practice_match.h` |
| RPG_BAT | 355 / 16 | `rpg_battle/{work,battle,menu}.h`; retain overlay override guards |
| SHOUGATU | 778 / 35 | `new_year_christmas/{work,new_year,christmas}.h` after confirmation |
| SHUGAKU | 490 / 12 | `school_trip/{work,school_trip}.h`; finer scene leaves only after symbol ownership is known |
| TACO | 465 / 17 | `shooter_3d/{work,level,model}.h` |
| TAIIKU | 464 / 55 | `sports_festival/{work,runner_race,folk_dance}.h`; TaiikuWork is at `TAIIKU.h:629`; scene tables start at `:847` |
| TEL | 255 / 3 | `telephone/{phone,profile}.h` |
| TT | 194 / 40 | contributor agreement first; do not choose an English folder/header unilaterally |
| VALEN | 96 / 2 | `valentine.h`; do not name a White Day module before confirmation |

These are concrete path targets and initial boundaries, not a claim that every anonymous declaration has been assigned. Each unit must attach a complete declaration-to-owner manifest before its split can be applied. A large overlay's `work.h` owns shared overlay records; leaf headers include it only if their signatures need it. Main topic headers never include an overlay header. Shared types used by main and overlays, such as TaiikuBig, and shared cross-overlay types such as CultureFestivalDispatch stay under `game/`. The main-bss overlay-type exception remains available only after the actual user inventory proves that the declaration is overlay-private.

EVENT overlays main bss (load base 0x800F6000); a naked address in that range is insufficient to identify storage. `sync_protos.Model.is_main` (`:207-222`) currently uses API membership and other headers as evidence. Register `(link unit, address, symbol, canonical header, storage owner, role)` explicitly for this migration. Preserve existing deliberate overrides, e.g. `DATE.h:5-9`, `ENDING.h:5`, and main-bss records whose only useful type is overlay-specific. No merged all-overlay umbrella is valid: separate overlays intentionally reuse addresses and can have incompatible local types.

## Tool changes that must precede declaration moves

A forwarding main_api is insufficient with the current checker. It reads only declarations physically in that file, so moving a declaration into a topic header fails ownership checks even while main_api includes that header. Tool support therefore comes before the first move, not only before final deletion.

| blocker and actual code | required behavior / test gate |
|---|---|
| `check_headers.py:379-381` calls `sync_protos.check_api` only when main_api exists; `sync_protos.py:577-582` returns no ownership findings when it is absent | Canonical ownership enforcement runs independently of umbrella presence; a missing registry/header is an error, never a quiet disable. Keep general structural/closure checks. Negative fixture without both umbrellas must reject conflicting or duplicate main declarations. |
| `sync_protos.py:65`, `:146-147`, `:589-625` identify one physical `API`; `:649-679` only checks source duplicates after API is in closure and requires every overlay header to reach it | Build canonical symbol map from declared owners; enforce unique canonical declaration even across disjoint main C closures; source checks depend on referenced owner, not umbrella reachability. Each C/header need include only the topic it uses. Preserve override exceptions. |
| `sync_protos.api_types` (`:245-256`), `skip_reason` (`:471-478`) assume moving types to main_api solves dependencies | Determine available types from destination closure; complete record definitions precede by-value use. Do not drag unrelated APIs into type leaves. Test a local-type main-bss exception and a record shared by two overlays. |
| `sync_protos.api_guards` (`:704-710`), render/write/fix (`:915-945`, `:1002-1015`, `:1060` onward), `--add-global` | Preserve guards and comments in each canonical owner; write/fix/add-global cannot recreate main_api or funnel all topics into it. Require a destination or registered owner for new declarations. Keep MAIN_API_OVERRIDE spelling for compatibility during this structural split. |
| `sync_protos.SDK_HEADERS` (`:67`), EVENT heuristic (`:207-222`) | Explicit SDK ownership and link-unit-aware canonical records. Test SDK prototypes outside main_api and EVENT/main symbols at the same address with different views. Do not use absence of an umbrella to reclassify EVENT data. |
| Requested `tools/check_globals.py` is absent; actual build guard is `tools/migrate_globals.py` (`configure.py:283-290`) | Update the active guard. `migrate_globals.Views` (`:409-416`) uses main_api fallback; migration order/guard removal (`:673-691`) and keep validation (`:736`) name that file. Resolve canonical owners and actual closure without it; preserved keep proofs must still validate after state declarations move. |
| `rename.py:1824-1826` TOOL_HEADERS blocks game/main_api/common moves; `plan_header` (`:1833-1893`) supports one-to-one header moves, warns on tool references but does not fix them | Preserve guardrails until consumers are fixed. Existing path renames remain `file include old new` rows and separate commits. One-to-many declaration extraction is not represented by a file row: use a reviewed deterministic pure-move operation that preserves identifiers and old umbrella paths, or explicitly extend the tool. Do not represent a split as several conflicting renames of one path. |
| `rename.py:2185` fixes `API_HEADER` to main_api; alias declaration merging returns early without it (`:2210`), and `views_proof` silently returns when that file is absent (`:2368-2369`) | Resolve alias declarations through canonical topic owners, including aliases crossing different topic headers. Run type-view proof regardless of umbrella presence. Add fixtures proving an alias conflict and a changed call prototype are rejected with no main_api/game umbrellas. Merely removing TOOL_HEADERS restrictions would leave these checks disabled. |
| `srcscan.header_units` (`:193-208`) learns direct C includes only; `unit_header` (`:212-220`) fixes main to game.h and chooses the first sorted overlay header after rename | Introduce plural topic discovery or require explicit destination in callers. Recognize nested headers and ownership even if only transitively included. Test two leaves of one overlay, shared game records used by two overlays, and main with no game.h. Keep original disc-unit identity. |
| Other live consumers: `m2c.py:282` preprocesses game.h; `dupes.py:485`, `:978-1015`, `:1092-1095`, `:1154` reads/writes old roots; `bulk_m2c.py:250`, `:502-504` assumes fixed declaration destinations | Route context gathering through C-specific closures/owner registry; write generated declarations to explicit topic owners; no helper may recreate a deleted umbrella. Update tests for the supported matching workflows. Historical campaign scripts can be documented unsupported rather than silently promoted to required tooling. |
| `configure.py:277-281` and `:283-290` list current tool inputs, `HEADER_FILES` recursively discovers headers (`:35`) | Keep recursive discovery/invalidation. Add new ownership/config modules to explicit dependencies of header/global/rename guards; otherwise changing a registry can leave stale `.ok` stamps. Exercise regeneration after nested header add/remove without clean. |

The registry is proposed tooling, not a file already present. Its final format should be shared by check_headers, sync_protos, rename, srcscan and aggregate tooling rather than separate ad hoc ownership lists. It must distinguish canonical declaration owner, storage owner and override location. Existing symbol tables remain the authority for `(unit,address)` identity; the registry must not become a competing symbol-name database.

## Execution and acceptance gates

1. **Prepare owners and tooling.** Create a bounded implementation ticket and independent tool review. Capture baseline header/type-view inventory and override locations. Add fixtures for the cases above to `test_check_headers.py`, `test_sync_protos.py`, `test_migrate_globals.py`, `test_srcscan.py`, `test_rename.py` and supported context-generation tests. Run tests in Docker. Before extraction, demonstrate that a topic-owned main declaration passes with a forwarding umbrella, fails when duplicated in a disjoint header, and remains enforced after both umbrellas are absent.
2. **Extract leaf records without changing names.** Follow the type-owner order, with complete field layouts, guards, comments and macro behavior preserved. During transition game.h/main_api may forward to new headers. All actual declarations live in exactly one registered owner. The table covers every current typedef; record any additional declarations introduced by intervening work before applying. Review state/save layout and signed/unsigned alternate views explicitly.
3. **Port-first APIs.** Move the scheduler/day-event, school-day, holiday/appointment and game-mode declarations, then the APIs their C files actually use (girl/param/calendar/text/menu/actor/etc.). A gameplay C file includes `game/game_state.h` and its needed topic APIs directly. Recompute its effective view before removing umbrella includes. The shared-state extraction does not split GameState storage into new globals or remove PS1 fields.
4. **Remaining main and overlays, one owner at a time.** Unit owners complete the declaration-to-header manifest, prove semantic names, then split headers. Preserve override defines before the first new include that could expose the canonical declaration. A shared overlay-level override that once preceded a single umbrella must precede all affected topic includes in every C file. Try pruning an override only in a separate build-backed change; the structural split preserves all current views.
5. **Proof for each applied structural batch.** `sync_protos --snapshot` before and `--compare` after; require **zero benign, zero check, zero risky** changes. Current compare can exit successfully with benign differences (`sync_protos.py:1424-1437`, CLI `:1627`), so exit code alone is insufficient. Its snapshots track used symbol declarations, not complete struct layouts, typedef availability or macro expansion: compare moved block text/field layout and preprocessed views as well. Run header checker, aggregate guard, rename/symbol checks and the appropriate tool tests. Primary chat then performs the authorized clean Docker build under `nice -n 19`, `--cpus=2`, respecting the project's build lock, requiring 27/27 sha1 and progress 6,952. This audit does not run that build.
6. **File/folder naming last.** Apply reviewed rename tables through `tools/rename.py`, with file/dir-only tables and commits distinct from symbol batches and structural splits. Example order: `dir TAIIKU TAIIKU sports_festival`, then `file TAIIKU 80133C80 runner_race`, and an existing header's one-to-one `file include ovl/TAIIKU.h ovl/sports_festival.h` only when applicable. A new nested set instead gets the separate pure-move extraction. Preserve original object range comments, `config/objects/<DISC>.txt` pointers, and disc-named build/asm roots. Require source/config/toolchain references updated, rename ledger replay/check and the same clean-build proof.
7. **Delete umbrellas only after a whole-tree gate.** Static search finds no active include of game.h or main_api.h in any C or live header, including disabled branches and generated templates. Every C file has needed topic includes; every canonical declaration and override is covered by the owner map. Header/global/rename guards still reject deliberate regression fixtures with both old files absent. Supported m2c/dupes/bulk helpers work without recreating old roots. Update CODING_STANDARDS 8a, living wiki/build docs and tool messages to topic ownership. Remove forwarding umbrellas in its own reviewed batch, rerun the snapshot/layout checks and full 27-output build. No new catch-all takes their place.

The first gameplay port slice can start after the relevant records/APIs and unit specs are reviewed; it does not wait for every overlay/header or optional SDK name. Conversely, readable names or successful rename plans alone do not satisfy the final header deletion gates. T-27052 produces this design; it does not mark phase 2 or any header split complete.

Independent Sol 6.1 review completed on 2026-10-24 with no unresolved findings. Its read-only Docker inventory reproduced the main/overlay counts and all 73 typedef entries. The review corrected the initial TaiikuBig overlay-private assignment to shared model records/main view declarations, using its actual main/DATE/ETC/TAIIKU users. No source/header/tool refactor or matching build was performed for this design.

## First migration: scheduler and school-day objects

This is the next bounded implementation, with the human's filename/header priority taking precedence over a full type split. It preserves two complete original C objects, 56 scheduler definitions and 19 school-day definitions. It does not split function bodies into new C objects. An executable file-only rename table needs just these two rows, separate from the structural header change:

```text
file main 8006CB30 scheduler T-27052: weekly scheduler, queue dispatch, calendar advancement and weekly health steps; preserve original object boundary
file main 800737A0 school_day T-27052: school-day/vacation command animation and parameter effects; preserve original object boundary
```

The legacy proposal tables contain path-shaped file rows mixed with identifier rows. They are evidence for these two names, not the executable migration batch: current `rename.py plan_file` matches the address/name **stem**, and file rows need their own table and commit. The primary chat owns the actual plan, ledger and application. No identifier, local, parameter, enum value or function order changes in this pilot. In particular, retain the applied `parameter_up_down` locals/parameters and O.BIN names/spellings.

### Exact paths and original intervals

All ranges below are half-open. `config/objects/main.txt:34-35` supplies text and rodata boundaries, and `:57-58` supplies recovered static-data ownership. `config/SLPM_86.053.yaml:102-104`, `:194-196` has the actual named C/rodata subsegments.

| current → logical source path | text VRAM / ROM interval | rodata VRAM / ROM interval | recovered data VRAM / ROM interval |
|---|---|---|---|
| `src/main/8006CB30.c` → `src/main/scheduler.c` | `[0x8006CB30,0x800737A0)` / `[0x2C330,0x32FA0)` | `[0x800B1140,0x800B14F0)` / `[0x70940,0x70CF0)` | `[0x800B6740,0x800B6760)` / `[0x75F40,0x75F60)` |
| `src/main/800737A0.c` → `src/main/school_day.c` | `[0x800737A0,0x80075320)` / `[0x32FA0,0x34B20)` | `[0x800B14F0,0x800B1590)` / `[0x70CF0,0x70D90)` | `[0x800B6760,0x800B6990)` / `[0x75F60,0x76190)` |

The data columns record original ownership, **not current C-defined `.data` islands**. Both intervals currently lie inside the generic assembler `data_800B5950` segment (`SLPM_86.053.yaml:209`, from ROM 0x75150 to 0x76190). There is no `main/8006CB30` or `main/800737A0` `.data` YAML subsegment to rename. Leave that generic segment and its bytes untouched. The neighboring named `.data main/80075320` starts at 0x76190 and is outside this pilot.

The two file rows rename only these named subsegments: `main/8006CB30` → `main/scheduler` for C and `.rodata`, and `main/800737A0` → `main/school_day` for C and `.rodata`. Keep offsets, compiler choice, original object manifest and all symbol addresses unchanged. `rename.py` supplies the original-address comment and updates affected config/path strings. New source comments should describe the whole original object rather than claim a new object boundary for each header topic.

The primary chat's read-only Docker `rename.py plan` of `.claude/orchestrator/state/audit27052-main-file-pilot.tsv --ledger main-files` passed: two rows, six affected paths; each row edits two YAML names (C and rodata), zero asm strings and zero config keys. The school-day row also updates the old source-path comment in `src/ovl/KANGEI/80132E40.c:319`; both moved C files receive original-address comments. These are expected review changes, not extra gameplay edits. No apply/build was performed by that plan.

### Exact header move set and include edges

Create `include/main/schedule.h` (`MAIN_SCHEDULE_H`), `include/main/day_event.h` (`MAIN_DAY_EVENT_H`) and `include/main/school_day.h` (`MAIN_SCHOOL_DAY_H`). Each includes **only `common.h`**: the exact declarations below use fixed-width scalars/pointers and arrays, with no GameState or overlay type in a signature. They own no typedef or enum definition in this pilot.

Move exactly **63 existing declarations** from main_api: 47 function declarations and 16 objects. Of the function declarations, 41 correspond to current named definitions in the two objects; six address-named declarations are still referenced by GYOZI and have no standalone current named definition at that exact address. Preserve all six and their existing `()` signatures: `func_80072734`, `func_80072FE8`, `func_800744A0`, `func_800748B8`, `func_80074950`, `func_80074A14`. Their token references below are evidence of continued consumers, not a claim that the current main object has a function boundary at each address.

The other **34 of the 75 definitions have no shared prototype today** (27 scheduler, seven school-day). Do not introduce declarations for them as part of this structural move. A definition signature is not permission to replace another C file's implicit call view. Keep `week_day_main` and `week_day_exit` source-local declarations (`800737A0.c:4-5`) in place, and keep source function order. The complete definition inventory below makes every omission explicit.

Nine of those omitted shared prototypes have consumers outside their defining object. Preserve these existing implicit views; they do not gain a new declaration merely by including a topic header:

| function with no current shared prototype | current external source consumers |
|---|---|
| `schedule_init` | `src/main/80042540.c` |
| `schedule_command_icons_lock_to_record` | `src/main/8005A0B0.c`, `src/main/800737A0.c`, `src/ovl/TEL/801397D0.c`, `src/ovl/ETC/80146400.c` |
| `schedule_command_girl_select` | `src/main/800737A0.c`, `src/main/80075320.c` |
| `schedule_day_screen_handler` | `src/main/80042540.c` |
| `schedule_day_notice_mode_handler` | `src/main/80042540.c` |
| `schedule_month_start_mode_handler` | `src/main/80042540.c` |
| `schedule_week_end_mode_handler` | `src/main/80042540.c` |
| `week_day` | `src/main/80042540.c` |
| `parameter_up_down` | `src/main/80075320.c`, `src/ovl/KANGEI/80132E40.c` |

The remaining 25 omitted-prototype definitions have no token consumers outside their defining C file in this inventory; source ordering/local declarations still determine their intra-object views.

Required directed edges for this pilot:

```text
main_api.h -> main/schedule.h, main/day_event.h, main/school_day.h
main/{schedule,day_event,school_day}.h -> common.h
src/main/scheduler.c -> common.h, game.h,
                        main/schedule.h, main/day_event.h, main/school_day.h
src/main/school_day.c -> common.h, game.h,
                         main/schedule.h, main/day_event.h, main/school_day.h
other existing C/header closures -> game.h/main_api.h -> the three topic leaves
```

The two pilot C files can include all three leaves explicitly while retaining `common.h`/`game.h`; including the already-guarded leaves again adds no declarations. Other consumers listed below continue to receive exactly the same views through the forwarding main_api closure, so this bounded pilot does not edit dozens of overlay includes. Full direct-include migration and umbrella deletion remain later work. The three leaf headers never include each other or main_api/game, so there is no new cycle.

Keep the scheduler's first two `MAIN_API_OVERRIDE_` defines before **all** includes and its exact source-local views (`8006CB30.c:1-7`):

```c
#define MAIN_API_OVERRIDE_func_80053418 /* retain existing full reason */
#define MAIN_API_OVERRIDE_func_80066104 /* retain existing full reason */
/* Existing include order, then direct topic includes. */
void func_80066104();
void func_80053418();
```

Neither canonical declaration moves in this pilot: they remain with their existing main_api/other-module ownership. No override currently targets any of the 63 moved declarations in the inspected tree; re-inventory before application in case concurrent work adds one. Preserve any newly found guard with its declaration. Do not prune overrides or change `s32 day_event_queue_resync_after_master();` to `(void)`.

The shared records used by the bodies stay where they are for this pilot: GameState and SystemData in main_api; Actor/ActorFixed, Girl/appointment/player bit views and related records through game/main_api; no layout changes. The long-term type owners above remain the follow-up design. Bss globals such as `D_801230D0`, `D_80125CA4`/`CA8`/`CAC`/`CB0`, actor pools and sprite_quads retain their current declarations; their users span other objects and their canonical storage/semantic assignment is outside the two recovered data intervals. Likewise `D_800B69E0` is holiday-owned data and stays outside this move.

The exact declaration bodies and consumers below come from the read-only Docker inventory `.claude/orchestrator/state/audit27052-header-pilot.py` and generated `audit27052-header-pilot*.txt`. Consumer lists are actual identifier-token references with comments removed, not claims about runtime reachability. Source paths are the pre-migration paths. Move attached comments as well as the displayed declaration text, including the T-17000 table-shape evidence preceding `D_800B6890` and T-25049 byte-region evidence preceding `D_800B68C0`. No array becomes a scalar and no signed-byte table becomes unsigned.

#### `include/main/schedule.h` — 26 functions, 6 objects

```c
extern s8 schedule_last_picked_girl;
extern s32 schedule_command_rank_bar_liberal_arts_athletics;
extern s32 schedule_command_rank_bar_art;
extern s32 schedule_command_rank_bar_trivia;
extern s32 schedule_command_rank_bar_shiori_athletics;
extern s32 schedule_command_rank_bar_science_looks;
void _schedule_init(void);
void schedule_step_prepare(void);
void schdeule_girls_param(void);
void schedule_step_advance_day(void);
void last_date_spot_timer_dec(void);
void restore_bgm(void);
s32 schedule_memory_card_save_poll(void);
void schedule_girl_spots_expire(void);
void schedule_progress_state_update(void);
s32 schedule_meeting_select_catchup(u8 *arg0, u8 *arg1);
s32 schedule_meeting_select_normal(u8 *arg0, u8 *arg1);
void schedule_girls_gakko(void);
void yuukou_down(void);
void syoushin_up(void);
void schedule_day_screen_step_dispatch(void);
void schedule_ending_start(void);
void calendar_advance_day(void);
void schedule_vacation_notice_step(void);
void func_80072734();
s32 schedule_month_start_step_dispatch(void);
s32 schedule_month_data_load_step(void);
void schedule_month_start_music_wait(void);
void schedule_week_end_hud_init(void);
void schedule_health_step_dispatch(void);
void func_80072FE8();
s32 schedule_meeting_select_club(u8 *arg0, u8 *arg1);
```

External source consumers (token references; excluding the two pilot objects):

- `src/main/80075320.c`: `restore_bgm`, `schedule_progress_state_update`.
- `src/main/8007C030.c`: `calendar_advance_day`.
- `src/ovl/ETC/8014C2D0.c`: `calendar_advance_day`.
- `src/ovl/GYOZI/80134C10.c`: `func_80072734`.
- `src/ovl/GYOZI/801372B0.c`: `func_80072FE8`.
- `src/ovl/GYOZI/80138B50.c`: `func_80072734`.
- `src/ovl/GYOZI/8013A920.c`: `func_80072FE8`.
- `src/ovl/GYOZI/8013C3B0.c`: `func_80072734`.
- `src/ovl/GYOZI/8013CDD0.c`: `func_80072734`.
- `src/ovl/GYOZI/8013D470.c`: `func_80072734`.
- `src/ovl/GYOZI/801400D0.c`: `func_80072734`.
- `src/ovl/GYOZI/80140740.c`: `func_80072734`.
- `src/ovl/GYOZI/80140C90.c`: `func_80072734`.
- `src/ovl/GYOZI/80143110.c`: `func_80072734`.
- `src/ovl/GYOZI/80143AD0.c`: `func_80072734`.
- `src/ovl/KANGEI/80132E40.c`: `calendar_advance_day`.
- `src/ovl/MASTER/80133680.c`: `calendar_advance_day`.
- `src/ovl/OPTION/8013A550.c`: `calendar_advance_day`.
- `src/ovl/SHUGAKU/8013AA70.c`: `calendar_advance_day`.

#### `include/main/day_event.h` — 5 functions, 0 objects

```c
s32 day_event_queue_resync_after_master();
void day_event_queue_build_week(void);
void day_event_dispatch(void);
void day_event_today_clear(u8 arg0);
s32 day_event_next(s32 arg0);
```

External source consumers (token references; excluding the two pilot objects):

- `src/main/80075320.c`: `day_event_next`.
- `src/ovl/BUNKAKEN/80144F70.c`: `day_event_next`.
- `src/ovl/BUNKASAI/80142700.c`: `day_event_next`.
- `src/ovl/DATE/80132000.c`: `day_event_next`.
- `src/ovl/DATE/8014B1F0.c`: `day_event_next`.
- `src/ovl/DATE2/80132DE0.c`: `day_event_next`.
- `src/ovl/ETC/80146400.c`: `day_event_next`.
- `src/ovl/ETC/801492C0.c`: `day_event_next`.
- `src/ovl/ETC/8014ABD0.c`: `day_event_next`.
- `src/ovl/ETC/8014C2D0.c`: `day_event_next`.
- `src/ovl/GEKO/801323E0.c`: `day_event_next`.
- `src/ovl/GEKO/80134040.c`: `day_event_next`.
- `src/ovl/GEKO/80134D10.c`: `day_event_next`.
- `src/ovl/GEKO/80142760.c`: `day_event_next`.
- `src/ovl/KANGEI/80132090.c`: `day_event_next`.
- `src/ovl/KANGEI/80132E40.c`: `day_event_next`.
- `src/ovl/MASTER/80132140.c`: `day_event_next`.
- `src/ovl/MASTER/80133680.c`: `day_event_queue_resync_after_master`, `day_event_next`.
- `src/ovl/MASTER/80139000.c`: `day_event_next`.
- `src/ovl/OMIMAI/801320A0.c`: `day_event_next`.
- `src/ovl/OMIMAI/801331B0.c`: `day_event_next`.
- `src/ovl/RENSYU/80132040.c`: `day_event_next`.
- `src/ovl/SHOUGATU/80132550.c`: `day_event_today_clear`.
- `src/ovl/SHOUGATU/80135B90.c`: `day_event_next`.
- `src/ovl/SHOUGATU/80136F90.c`: `day_event_next`.
- `src/ovl/SHOUGATU/8013BA00.c`: `day_event_next`.
- `src/ovl/SHOUGATU/8013F190.c`: `day_event_next`.
- `src/ovl/SHUGAKU/8013AA70.c`: `day_event_next`.
- `src/ovl/TAIIKU/80141280.c`: `day_event_next`.
- `src/ovl/TEL/80132000.c`: `day_event_next`.
- `src/ovl/VALEN/801339D0.c`: `day_event_next`.

#### `include/main/school_day.h` — 16 functions, 10 objects

```c
extern s32 D_800B6760[];
extern s32 D_800B6780[];
extern s32 D_800B67AC[];
extern s32 D_800B6810[];
extern u8 D_800B6890[12][2];
extern u8 D_800B68A8[12][2];
extern u8 D_800B68C0[16];
extern u8 D_800B68D0[16];
extern s8 school_day_command_deltas[8][9];
extern s8 club_activity_deltas[][9];
void func_800737A0(void);
u8 week_day_init(void);
s32 get_weekly_bg_sector(void);
u8 school_day_screen_init(void);
u8 school_day_sd_load(void);
u8 school_day_sd_actors_init(void);
void func_800744A0();
s32 week_day_main0(void);
void week_day_exit0(void);
void func_800748B8();
void func_80074950();
void func_80074A14();
u8 vacation_day_init(void);
u8 school_day_vacation_screen_init(void);
u8 school_day_vacation_sd_load(void);
u8 school_day_vacation_sd_actors_init(void);
```

External source consumers (token references; excluding the two pilot objects):

- `src/main/80075320.c`: `D_800B6780`, `D_800B6810`, `D_800B6890`, `D_800B68A8`.
- `src/ovl/GYOZI/80134000.c`: `func_800748B8`, `func_80074A14`.
- `src/ovl/GYOZI/80134520.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/80138510.c`: `func_800748B8`, `func_80074A14`.
- `src/ovl/GYOZI/801388E0.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013A140.c`: `func_800744A0`, `func_800748B8`, `func_80074950`, `func_80074A14`.
- `src/ovl/GYOZI/8013A920.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013AE80.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013B920.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013BD50.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013CDD0.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013D120.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013D470.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013DB50.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013DE30.c`: `func_800748B8`, `func_80074A14`.
- `src/ovl/GYOZI/8013E1D0.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/GYOZI/8013EB10.c`: `func_800748B8`, `func_80074A14`.
- `src/ovl/GYOZI/801400D0.c`: `func_800744A0`, `func_80074950`.
- `src/ovl/KANGEI/80132E40.c`: `vacation_day_init`.

#### Complete definition inventory and intentional declaration omissions

`src/main/8006CB30.c`: 56 definitions. Line numbers before migration. An em dash means no shared prototype exists in the current header closure; preserve that omission.

| source line | definition signature | destination / existing shared declaration |
|---:|---|---|
| 9 | `void schedule_init(void)` | `schedule.h`: — |
| 16 | `void _schedule_init(void)` | `schedule.h`: `void _schedule_init(void);` |
| 42 | `void last_date_spot_timer_dec(void)` | `schedule.h`: `void last_date_spot_timer_dec(void);` |
| 52 | `void restore_bgm(void)` | `schedule.h`: `void restore_bgm(void);` |
| 58 | `void schedule_step_prepare(void)` | `schedule.h`: `void schedule_step_prepare(void);` |
| 79 | `s32 schedule_memory_card_save_poll(void)` | `schedule.h`: `s32 schedule_memory_card_save_poll(void);` |
| 97 | `void schedule_command_icons_lock_to_record(void)` | `schedule.h`: — |
| 112 | `void day_event_queue_clear(void)` | `day_event.h`: — |
| 122 | `void girl_flags_bit0_clear(void)` | `schedule.h`: — |
| 130 | `void schedule_amusement_park_flag_reset(void)` | `schedule.h`: — |
| 136 | `void schedule_girl_spots_expire(void)` | `schedule.h`: `void schedule_girl_spots_expire(void);` |
| 147 | `void schedule_progress_state_update(void)` | `schedule.h`: `void schedule_progress_state_update(void);` |
| 267 | `void schedule_club_flag0_clear(void)` | `schedule.h`: — |
| 275 | `void day_event_queue_push_birthdays(s32 *arg0, s32 arg1, s32 arg2)` | `day_event.h`: — |
| 302 | `s32 day_event_queue_resync_after_master(void)` | `day_event.h`: `s32 day_event_queue_resync_after_master();` |
| 347 | `void day_event_queue_build_week(void)` | `day_event.h`: `void day_event_queue_build_week(void);` |
| 698 | `s32 schedule_miharu_scene4_ready(void)` | `schedule.h`: — |
| 720 | `s32 schedule_meeting_select_catchup(u8 *arg0, u8 *arg1)` | `schedule.h`: `s32 schedule_meeting_select_catchup(u8 *arg0, u8 *arg1);` |
| 790 | `s32 schedule_meeting_select_normal(u8 *arg0, u8 *arg1)` | `schedule.h`: `s32 schedule_meeting_select_normal(u8 *arg0, u8 *arg1);` |
| 859 | `s32 schedule_meeting_select_club(u8 *arg0, u8 *arg1)` | `schedule.h`: `s32 schedule_meeting_select_club(u8 *arg0, u8 *arg1);` |
| 925 | `s32 schedule_command_girl_select(u32 arg0)` | `schedule.h`: — |
| 1138 | `s32 schedule_special_scene_select(char *arg0, char *arg1)` | `schedule.h`: — |
| 1343 | `s32 geko_judge(s8 *arg0)` | `schedule.h`: — |
| 1367 | `s32 date_sasoi_judge(s8 *arg0)` | `schedule.h`: — |
| 1411 | `void schedule_girls_gakko(void)` | `schedule.h`: `void schedule_girls_gakko(void);` |
| 1444 | `void yuukou_down(void)` | `schedule.h`: `void yuukou_down(void);` |
| 1467 | `void syoushin_up(void)` | `schedule.h`: `void syoushin_up(void);` |
| 1477 | `void schdeule_girls_param(void)` | `schedule.h`: `void schdeule_girls_param(void);` |
| 1485 | `void schedule_step_advance_day(void)` | `schedule.h`: `void schedule_step_advance_day(void);` |
| 1493 | `void schedule_day_screen_handler(void)` | `schedule.h`: — |
| 1506 | `void schedule_day_screen_step_dispatch(void)` | `schedule.h`: `void schedule_day_screen_step_dispatch(void);` |
| 1516 | `void schedule_ending_start(void)` | `schedule.h`: `void schedule_ending_start(void);` |
| 1526 | `void girl_counters_clamp(void)` | `schedule.h`: — |
| 1555 | `s32 schedule_yuina_highest_points_check(void)` | `schedule.h`: — |
| 1585 | `void girl_flag4_may_clear(void)` | `schedule.h`: — |
| 1599 | `void day_event_dispatch(void)` | `day_event.h`: `void day_event_dispatch(void);` |
| 1944 | `void schedule_quarter_flags_rotate(void)` | `schedule.h`: — |
| 1967 | `void calendar_advance_day(void)` | `schedule.h`: `void calendar_advance_day(void);` |
| 2032 | `void day_event_today_clear(u8 arg0)` | `day_event.h`: `void day_event_today_clear(u8 arg0);` |
| 2037 | `void schedule_day_notice_mode_handler(void)` | `schedule.h`: — |
| 2070 | `void schedule_vacation_notice_step(void)` | `schedule.h`: `void schedule_vacation_notice_step(void);` |
| 2118 | `void schedule_month_start_mode_handler(void)` | `schedule.h`: — |
| 2136 | `s32 schedule_month_start_step_dispatch(void)` | `schedule.h`: `s32 schedule_month_start_step_dispatch(void);` |
| 2147 | `s32 schedule_month_data_load_step(void)` | `schedule.h`: `s32 schedule_month_data_load_step(void);` |
| 2188 | `void schedule_month_start_music_wait(void)` | `schedule.h`: `void schedule_month_start_music_wait(void);` |
| 2195 | `s32 day_event_next(s32 arg0)` | `day_event.h`: `s32 day_event_next(s32 arg0);` |
| 2208 | `void schedule_week_end_mode_handler(void)` | `schedule.h`: — |
| 2229 | `void schedule_week_end_hud_init(void)` | `schedule.h`: `void schedule_week_end_hud_init(void);` |
| 2236 | `void schedule_health_step_dispatch(void)` | `schedule.h`: `void schedule_health_step_dispatch(void);` |
| 2266 | `s32 schedule_health_check_roll(void)` | `schedule.h`: — |
| 2379 | `s32 schedule_health_injury_start(void)` | `schedule.h`: — |
| 2411 | `s32 schedule_health_neurosis_start(void)` | `schedule.h`: — |
| 2432 | `s32 schedule_health_sick_start(void)` | `schedule.h`: — |
| 2454 | `s32 schedule_health_injury_recover(void)` | `schedule.h`: — |
| 2476 | `s32 schedule_health_neurosis_recover(void)` | `schedule.h`: — |
| 2498 | `s32 schedule_health_sick_recover(void)` | `schedule.h`: — |

`src/main/800737A0.c`: 19 definitions. Line numbers before migration. An em dash means no shared prototype exists in the current header closure; preserve that omission.

| source line | definition signature | destination / existing shared declaration |
|---:|---|---|
| 7 | `void func_800737A0(void)` | `school_day.h`: `void func_800737A0(void);` |
| 18 | `void week_day(void)` | `school_day.h`: — |
| 47 | `void week_day_exit(void)` | `school_day.h`: — |
| 54 | `u8 week_day_init(void)` | `school_day.h`: `u8 week_day_init(void);` |
| 66 | `void week_day_main(void)` | `school_day.h`: — |
| 73 | `s32 get_weekly_bg_sector(void)` | `school_day.h`: `s32 get_weekly_bg_sector(void);` |
| 85 | `s32 school_day_background_read(void)` | `school_day.h`: — |
| 96 | `u8 school_day_screen_init(void)` | `school_day.h`: `u8 school_day_screen_init(void);` |
| 158 | `void school_day_sd_set_select(void)` | `school_day.h`: — |
| 316 | `u8 school_day_sd_load(void)` | `school_day.h`: `u8 school_day_sd_load(void);` |
| 348 | `u8 school_day_sd_actors_init(void)` | `school_day.h`: `u8 school_day_sd_actors_init(void);` |
| 413 | `s32 week_day_main0(void)` | `school_day.h`: `s32 week_day_main0(void);` |
| 445 | `void week_day_exit0(void)` | `school_day.h`: `void week_day_exit0(void);` |
| 453 | `s32 parameter_up_down(s32 command, s32 holiday_factor, s32 result_divisor)` | `school_day.h`: — |
| 518 | `u8 vacation_day_init(void)` | `school_day.h`: `u8 vacation_day_init(void);` |
| 530 | `s32 school_day_vacation_background_read(void)` | `school_day.h`: — |
| 543 | `u8 school_day_vacation_screen_init(void)` | `school_day.h`: `u8 school_day_vacation_screen_init(void);` |
| 589 | `u8 school_day_vacation_sd_load(void)` | `school_day.h`: `u8 school_day_vacation_sd_load(void);` |
| 607 | `u8 school_day_vacation_sd_actors_init(void)` | `school_day.h`: `u8 school_day_vacation_sd_actors_init(void);` |


### Pilot application order and acceptance

1. **First commit: two logical filenames.** The primary chat reviews and plans the two file-only rows, then applies through rename.py after concurrent semantic enum work is settled. Require exactly two C moves and their four named YAML C/rodata name edits, with any tool-generated path/config updates and address comments accounted for. No named data island is created, no C object is split, no function/local rename is bundled. Remap type snapshots by the file move map before comparison, as rename.py's existing proof does. Require all 75 function bodies/ordering and original text/rodata/data boundaries unchanged.
2. **Second commit: owner-aware header tooling.** Before moving a single declaration, make sync_protos/check_headers accept multiple registered canonical headers, retain current checks through forwarding main_api, preserve writer/fixer destinations, and prove positive/negative fixtures for these three topic owners. The earlier hardcoded-API blockers still apply. This bounded pilot needs no full 73-typedef extraction and no srcscan root deletion; the legacy root remains valid during transition. Do not let `--fix` move the 63 declarations back into main_api.
3. **Third commit: the 63-declaration pure move.** Create the three guarded leaves exactly as listed, remove their old declarations from main_api, add its forwarding includes, and add explicit leaf includes to the two logical C files while retaining the old state closure. Preserve source-local declarations, deliberate omissions, comments and all K&R lists. Verify the six GYOZI address-call entries and all listed consumers, including the main holiday object sharing four school-day data arrays. Their actual effective declarations must be unchanged even though their owner header paths differ.
4. **Proof and review.** Capture a fresh snapshot once the active revised semantic enum batch is settled; do not compare against a transient source state from a rename plan. For the file commit map old→new source paths; for the header commit require zero benign/check/risky view changes, unchanged type layouts/macros and no new implicit or narrowed call. Run owner/header/rename/symbol/aggregate guards and affected tooling fixtures in Docker; the primary chat then performs its required clean matching build with 27/27 sha1 and progress 6,952 after each applied batch. Independently review against CODING_STANDARDS before ticket Done. This design has not applied these commits or run a build.

The pilot's exact move lists remain current-source observations, not approved new function names. Its accepted semantic enum integration is a dependency for a stable proof baseline; it does not block review of these filenames and header boundaries. Success is two readable source paths and three useful topic owners with identical ABI views. The full shared-type and direct-include migrations continue afterward.

## T-27054 scratch implementation handoff

Root has applied the T-27044 file pilot to `src/main/scheduler.c` and `src/main/school_day.c`; its
external mapped-source-key comparison is zero benign/check/risky across 437 C units. Root owns the
clean matching build and application proof. The old source paths in this design's inventory are the
pre-migration names, not an instruction to recreate them.

T-27054 implements the bounded topic tooling in ignored copies only. Deliverable:
`.claude/orchestrator/state/T-27054/topic-header-tooling.patch` (SHA256
`167a8eb2ac3e21659e230fa71602731b3da418bb88cb9f3f1f02a01a44dae57f`). It changes exactly
`tools/check_headers.py`, `tools/sync_protos.py` and their two existing synthetic test files. No new
helper/config file is needed. `patch_manifest.json` records baseline/reviewed hashes and verifies that
the live four files still match the frozen baseline. Root checks/applies this patch separately from
the real header extraction; this worker has applied neither.

The explicit `CANONICAL_HEADERS` tuple registers only main_api and these three topic paths. Model.api
is their declaration union with per-symbol owner/provenance; duplicates across disjoint owners are
errors. An arbitrary main/ or SDK header is not adopted as a game declaration owner. A registered
owner remains main-owned regardless of consumer inference or a temporarily stale symbol-name map.
The legacy profile remains when none of the three topics exists. Topic mode preserves existing
canonical types/text and deliberately does not export definition-only functions: it must retain the
34 shared-prototype omissions, including calls made from logical source filenames. New noncanonical
candidate declarations default to main_api only while it exists; without it, missing explicit owners
fail before writing and never recreate the umbrella. Overlay-private main-bss/type exceptions remain.

Topic writers/fixers retain declarations in their registered owners, preserve K&R lists and deliberate
existing overrides, and add guarded multiline declarations without reflowing their text. A failed
topic rewrite restores exact original header/source bytes, including failures found after overlay
edits. Review found and resolved this late partial-write case. The actual-copy probe additionally
found that dropping ENDING's wide `talk_render` declaration as benign left its canonical declaration
hidden by the existing override; topic fix now keeps that declared view and a synthetic regression
covers it. No ENDING source/header edit is part of this patch.

The independent Sol 6.1 review passed **118/118** header/prototype tests (95 sync + 23 header).
Additional scratch regression suites passed 78 rename tests and 159 dupes/aggregate/srcscan/bulk
tests: **355 total tests** across those runs. Tests ran in the existing Docker image with --cpus=2,
the repository read-only and only ignored scratch writable. The complete 63-declaration copied-header
probe, at source HEAD `2298a9b2fe2dedd3842d5e3c62d7ce43400bd4e1`, verifies:

- The actual logical paths `scheduler.c`/`school_day.c` exist; no address-basename inference is needed.
- All 63 declarations remain in the three intended owners (32/5/26), with no API repatriation or
  addition of the 34 intentionally absent shared prototypes; the six GYOZI `()` views remain.
- Before/after extraction and before/after `--fix` are both exactly zero benign/check/risky changes;
  header checks pass, all non-pilot API declarations remain, and source C bytes are unchanged.

This is evidence from `.claude/orchestrator/state/T-27054/pilot_probe.py` on a copy of only include,
src and config; `pilot_probe_result.json` records hashes/counts. It is not an applied-header or new
game-checksum proof. Root repeats the proof at its actual integration revision.

For structural application use `sync_protos.py --snapshot <file>` before extraction, then
`sync_protos.py --compare <file> --strict-compare` after. The new optional strict flag exits nonzero
for any benign, check or risky difference; ordinary --compare retains its existing return convention.
Generic --fix may still log benign repairs of invalid non-override views. Its zero exit alone is not
the structural proof: require the strict comparison as well. Run owner/header/symbol/rename/aggregate
guards and root's clean 27/27/6,952 proof before Done. Keep game/main_api forwarding closures for the
pilot. The remaining srcscan/rename/aggregate context and full owner-registration work above still
precedes final umbrella deletion; this bounded patch does not authorize that deletion.

## T-27061 next extraction: four fixed-width API topics

The next executable packet is in ignored `.claude/orchestrator/state/T-27061/`:
`01-register-topic-owners.patch`, `02-extract-main-topics.patch`, `manifest.json` and `manifest.md`.
It is based on a stable 808-input snapshot at HEAD `c3053b47caab9689cb9fabfd898eb049bdcdb289`, with
identical before/copy/after digest
`930f11212a9f41ec6a79aa279e05ee2a8046cfab3f6a5993089b79197fd680c5`. Live inputs included the
new logical main source paths. Unapplied `config/renames/proposals/` and independent `tools/assets/`
work were excluded because header/view/symbol/aggregate proofs do not read them; all actual source,
header and proof configuration/tool inputs remained hash guarded.

| new canonical header | function declarations | initialized data | explicit BSS | provider |
|---|---:|---:|---:|---|
| `include/main/game_mode.h` | 6 | 0 | 0 | `src/main/game_mode.c`, object [0x80042540,0x80042A00) |
| `include/main/pad.h` | 10 | 5 | 0 | `src/main/pad_input.c`, object [0x80042A00,0x80043510) |
| `include/main/holiday.h` | 35 | 8 | 0 | `src/main/holiday.c`, object [0x80075320,0x800789E0) |
| `include/main/ovl.h` | 7 | 5 | 1 | `src/main/overlay_loader.c`, object [0x800789E0,0x80079B10) |

All **77 declarations** already exist in main_api. Each new header has its path-derived guard and
includes only common.h; no typedef, enum, SDK interface or GameState layout moves in this stage.
The one-line owner-registration patch adds these four paths to CANONICAL_HEADERS and changes no
tool logic. It is separate because the extraction requires explicit canonical registration before
declarations leave main_api. Registration is not inferred from paths or address ranges.

The extraction moves complete declaration/override/comment blocks unchanged, adds four forwarding
includes to main_api, and adds 28 owner-specific direct includes to **19 logical main C files** whose
actual tokens use the moved declarations. The exact source-to-header-to-symbol evidence is in
manifest.json. Examples: game_mode.c gets game_mode.h/ovl.h, game_init.c gets ovl.h, scheduler.c gets
game_mode.h, and school_day.c gets game_mode.h/holiday.h. Source bodies and pre-existing include/
override order are unchanged; the remaining state/type closure stays through game.h/main_api.
Overlay C files are untouched, which keeps this packet independent of root's overlay filename work.

The ovl_entry declaration is a specifically proven main-bss exception to the initialized-data
selection: extern u32 at 0x80123110 in config/symbol_addrs_main.txt. game_init.c:func_80041840
initializes it, overlay_loader.c:func_80079070/func_8007959C assign entry addresses, and game_mode's
dispatcher invokes it. Its symbol configuration remains main-only because EVENT overlaps this
address. No other BSS/shared-state declaration is reassigned by this packet.

Preserved cases and deliberate deferrals:

- Keep the previous 34 prototype omissions, plus the 18 omissions in the four new providers. Do
  not add game_mode_dispatch/game_mode_next, holiday_mode_handler or ovl_loader_mode_handler merely
  because their definitions are known.
- Keep the previous six GYOZI compatibility declarations and six additional compatibility views:
  pad's func_800433D0 retains `(s32 arg0)`; holiday's func_80075BFC, func_80076450, func_80078950,
  func_80078970 and func_800789A0 retain their existing `()` lists. They are not assertions of current
  standalone function boundaries. `game_mode_set()` remains unprototyped as before.
- Keep 13 old rodata labels and func_80079800 in main_api. The latter is the mixed loader object's
  memory-card/system-file routine, so an overlay-loader owner would be misleading. Missing current
  owning-C references to the rodata labels do not establish unused status. The manifest lists exact
  names, types, addresses and reasons for all deferrals.
- Preserve attached T-20940/T-17000 evidence, signed and array views, and existing override guards.
  The generator asserts the new leaf sets are exactly the planned 77 and old main_api is exactly
  its previous declaration map minus those 77, without altered raw declarations.

The move and subsequent --fix compare exactly zero benign/check/risky changes across 437 source
units in the copied packet; --fix also leaves all copied source/header bytes unchanged. All seven
selected suites pass (355 tests), as do header, symbol, rename and aggregate guards and both patches'
`git apply --check`. Independent Sol 6.1 review has no unresolved findings; it inspected the exact
patches, manifest, capture/proof scripts and the author's hash-validated results rather than starting
another duplicate container. The initializer-writer evidence was corrected to include game_init's
func_80041840. Detailed results are in proof.json/proof.log and the T-27061 ticket.
The proof container was the sole heavy job for this chat;
redundant parent/reviewer test containers were stopped on root's resource instruction, rather than
repeating identical frozen-input work. No live source/header/tool edit or game build occurs here.

Root integration order: verify touched-file base hashes, apply registration as its own metadata
change, then apply extraction. Capture a fresh 437-unit snapshot at the actual integration revision
and require `--compare <snapshot> --strict-compare`, header/symbol/rename/aggregate guards and clean
27/27/6,952 proof. If overlay filenames changed since this packet, take that fresh snapshot using
current YAML paths; do not compare the old copied snapshot's path keys directly. The patch changes
no overlay source paths. Keep both umbrellas until the remaining type/owner/direct-include gates are
met; this packet is a practical next step rather than a claim that the final split is complete.

Independent Sol 6.1 review reproduced the bounded pilot in read-only Docker: all 63 declaration texts matched main_api exactly with zero missing/extra/duplicate entries; 47 function/16 object counts, 75 definitions and 27+7 shared-prototype omissions were confirmed. It also checked the width-only include requirements, six retained GYOZI call views, object intervals and the two-row rename plan. No unresolved findings remained; application and matching builds are still outstanding.
