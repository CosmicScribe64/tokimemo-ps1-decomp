---
type: plan
updated: 2026-10-30
---

# Port completion plan (2026-10-30)

This plan replaces the route-by-route approach in [[port-finish-plan]] for everything after the opening. It keeps the Godot port, its GDScript and its tests. The change is in what gets ported first and how pieces are joined: port the shared engine and the game's memory layout once, then port the remaining C file by file on top of them.

## Where the port stands

Measured on `port/merge-pending` (all pending work merged, 2026-10-30):

| Area | State |
|---|---|
| Opening (boot, name entry, prologue, first arrival) | Frame-exact against the emulator: pad 2, 104 substeps, 6,709 frames, 0 mismatches. Fast mode runs at 75 frames/s. |
| Main loop | Actual route reaches command select on 4/4, 4/9, 4/16, 4/23 and 4/30 of 1995 with the Sunday holiday. Stops at the May month rollover. |
| Gameplay families | Every family is refused in real play (`app/overlay_dispatcher.gd` `PRODUCTION_REJECTIONS`, 21 entries). Most refusals say the overlay runs on an invented roster, fixed lists or default stats. |
| C source covered | The port cites 71 of the 437 C files: 25 of 34 in the main executable, 1-11 per overlay, none in eight overlays. |
| English | 55,094 text records approved. Artwork: 52 reviewed, 169 candidates, 21 missing, 29 deliberately untranslated. |
| Modern features | Settings v2, widescreen host, wing art, modern text box, Parity mode exist. The playable boot does not use the widescreen host yet. |

GDScript size: about 67,000 production lines and 53,000 test lines. About 20,000 lines are game rules; the rest is infrastructure (content, VRAM upload, presentation, audio, settings).

## Why more code has not added up to more game

1. **No shared game state.** The C keeps the game in `GameState` (0x1A90 bytes, 161 documented fields in `include/game/game_state.h`) plus girl, calendar and schedule tables. The port keeps a Dictionary `rule_state` with about 40 hand-chosen keys. Each module projects what it needs, so an overlay cannot read the real roster, affection or flags and has to invent them. This is the most common refusal reason.
2. **The engine was never ported as an engine.** Overlays make 26,381 calls into 315 main-executable functions (scene steps, dialogue, CD reads, backgrounds, portraits, audio, sprites). Only 14 of them exist in the port as reusable functions under their C names (9% of call sites). Each opening scene instead got its own renderer (`closing_scene_renderer.gd`, `shiori_scene_renderer.gd`, `opening_stages_15_17_renderer.gd`, ...). That approach does not scale to 360 more files.
3. **One content exporter per scene.** Each new scene added a content role and exporter (`school_day_media`, `spring_media`, `weekday56_media`, `club_media`, ...), so every step needed new tooling before any game code.
4. **Route-driven discovery.** Work was chosen by running from New Game to the next refusal and replaying 14,000+ frames each time. The 3x speedup and checkpoints help, but the order of work was still one stop at a time.
5. **Unmerged parallel work.** Parallel lanes produced patches on old bases and 104 worktrees. Fixed on 2026-10-30; the rules below keep it fixed.

## The approach

Port the engine once, then the content. Four foundation layers come first, and each one is small enough to finish and verify on its own.

### Layer 1: game memory model

A GDScript model of the PS1 main RAM (2 MiB, `PackedByteArray`) with generated, named views:

- `tools/port/gen_game_memory.py` (Docker) reads `include/game/*.h` and `include/main/*.h` and generates typed accessors for `GameState`, the girl records and the other gameplay globals at their original addresses. Widths and signedness come from the headers and go through `core/native_math.gd`.
- The executable's and overlays' data sections are loaded into the model from the user's installed disc at their original addresses. A ported C function reads `D_800xxxxx` tables where the C does, so most tables need no exporter. Original data stays outside Git.
- C pointers become PS1 addresses into the model. Pointer tables inside loaded data work unchanged.
- One bridge maps the existing `rule_state` keys onto the model in both directions, so current rules, tests and saves keep working while modules move over.

What this gives: saves are the original memory-card bytes; the emulator's RAM at any frame can be compared byte for byte with the port's; and an emulator save state can seed a development run at any date. A seeded run is dev-only and labelled; acceptance still starts at New Game.

### Layer 2: the engine API

Port the main-executable functions that overlays call, once, under their C names, one GDScript file per C file (`port/godot/engine/<c file>.gd`), reached through one engine facade.

- The top 100 functions cover 94% of overlay call sites, and most live in about a dozen files: `scene_library.c`, `hud_panels.c`, `menu_speaker_girl.c`, `text_system.c`, `scene_display.c`, `frame_render_audio.c`, `hud_screens.c`, `memory_card.c`, `cd_queue.c`, `background_images.c`, `actor_sprites.c`, `game_mode.c`, `gpu_primitives.c`.
- Existing infrastructure is the backing: dialogue and the text layer behind `text_system.c`/`normal_date_speak`, the native audio engine behind `audio_command` and the XA calls, the upload bridge's VRAM behind the background and portrait loaders.
- The existing `rules/` modules (scheduler, calendar, school day, holiday, week end) are the starting point for their C files and move into this layout.

### Layer 3: a generic PS1 renderer

One renderer for every scene instead of one per scene:

- A VRAM model (1024x512, 16-bit; the upload bridge already keeps one) and a rasterizer for the primitives the game issues: sprites, tiles, flat, Gouraud and textured polygons, lines, the four semi-transparency modes, dithering and 4/8/15-bit textures with CLUTs, drawn in ordering-table order.
- Rasterization is a C++ GDExtension next to the native audio engine. Per-pixel GDScript drawing is the measured bottleneck (about 12 of 25 ms per frame), which is the case [[phase3]] allows native code for. Game rules stay in GDScript.
- Output is still the native 320x240 Image, so `NativeDisplayHost`, wings, the modern text box and Parity mode work unchanged.
- Translated artwork is applied by texture hash at VRAM upload, one hook for every scene, using `translated_image_store.gd`.
- The opening keeps its verified renderers. They retire only if the generic renderer reproduces their frames exactly.

### Layer 4: one installed disc pack

`setup_content` installs one pack of the original files the game reads (path, LBA, size, SHA-256) from the user's disc. `cd_read_sectors` and friends read from it. Existing roles keep working; new code adds no roles.

## Porting the content

Once the core of layers 1-4 exists, the remaining C is translated file by file:

- One GDScript file per C file, same function names, with the C path in its header comment. Integer widths through `native_math`, `switch` to `match`, pointers through the memory model. The preview overlays' invented inputs are removed as each real file lands; their source-backed rules are reused.
- Files of different overlays never overlap, so three to five workers can run at once with no route runs. Each overlay merges when its tests pass.
- Pilot first: `sick_visit` (4 files, 979 lines) and `valentine` (5 files, 812 lines), to settle the contract before fanning out.

Order, by when a player meets them:

| Group | Overlays | Files / lines |
|---|---|---|
| Main loop (engine layer) | `scheduler.c`, `calendar.c`, `school_day.c`, the rest of `src/main` | 34 / 27,000 |
| Everyday events | `girl_encounters` (GEKO), `telephone`, `sick_visit`, `misc_scenes`, `help`, `options` | 53 / 24,500 |
| Dates | `date`, `date_spots`, `amusement_park_date` | 43 / 30,000 |
| Calendar events | `annual_events`, `new_year_christmas`, `valentine`, `sports_festival`, `school_trip`, `summer_camp`, `club_master`, `practice_match`, `festival_stalls` | 116 / 37,800 |
| Culture festival | `culture_festival`, `culture_festival_tour`, `culture_stage_shows` | 49 / 45,800 |
| Endings | `ending` | 8 / 4,050 |
| Minigames | `rpg_battle`, `shooter_3d`, `TT` | 130 / 42,100 |

`name_entry` is already ported.

## Testing

Four tiers, cheapest first:

1. **Function differential.** The emulator stops at a function, records its arguments and the memory it reads and writes, then records the result. A GDScript unit test replays the record against the ported function. Captures come from saved states near each scene (51 exist under `.asset-cache/`). This runs in seconds and replaces most route replays.
2. **State checkpoints.** The long-route driver (`config/port/input-scripts/three_year_default.json`, plus one script per family and per heroine ending) plays the emulator and the port with the same input. The memory model is compared byte for byte at every day boundary, and a difference names the first field that differs.
3. **Sampled visuals.** At those checkpoints the native screenshots are compared with the existing exact tools, and scenes get playtest contact sheets.
4. **Frame timing.** The opening keeps its frame-exact gate. Elsewhere timing that matters (RNG use, timers, input latching) lives in game memory and is covered by tier 2.

Proposed bar: memory exact at every checkpoint, pixels exact on sampled frames, frame-exact only for the opening. Acceptance runs in Docker, in Parity mode, from New Game. Dev runs may use native macOS and seeded states.

## Main loop to three years

Part of layer 2: the May rollover and every later month, the full day-event queue, vacations, exams, the ending dispatch, saving and Continue. The three-year route is about 1,100 in-game days; at 75 frames/s with checkpoints it is an overnight run, not a blocker for daily work.

## English

- Text: route all game text through the ported text functions. The text layer then applies the 55,094 approved records in every scene, with no per-scene hooks.
- Art: finish the 21 missing items, then review the 169 candidates in batches using in-place screenshots from the generic renderer.

## Modern presentation

- Use `NativeDisplayHost` in the playable boot (not wired yet) and feed wing art from the memory model.
- The modern text box draws from the ported text functions.
- Every modern feature stays in the feature registry and off in Parity mode.
- Optional later: achievements ([[tickets/T-30004-modern-achievements]]), player customization.

## Packaging

- Native audio and renderer extensions for macOS and Windows, built reproducibly.
- Godot export, the first-run setup from the user's disc, and the G5 checklist.

## Process rules

- One integration branch. Every unit ends with review, merge and commit; nothing starts from an unmerged base.
- Porting workers own whole files; at most five at once, at most two route-running workflows.
- Remove a worker's worktree once its work is merged.
- Each unit has a ticket and moves on [[kanban]] as usual.

## Milestones

| Milestone | Done when |
|---|---|
| M0 Merge | All finished work merged and verified (2026-10-30). |
| M1 Memory model | Port memory equals emulator RAM byte for byte at the first command select; the bridge passes every existing suite. |
| M2 Engine core | Top 100 engine functions, generic renderer and disc pack; the pilot overlays play from the real route with matching memory. |
| M3 First year | The three-year script reaches April 1996 with exact memory at every day boundary. |
| M4 Story overlays | Everyday events, dates, calendar events, culture festival and endings, each with its family script passing. |
| M5 Minigames | RPG battle, shooter and TT. |
| M6 Full game | Three years and every heroine's ending reachable from New Game; saves and Continue. |
| M7 English and modern | All text and art in English in place; widescreen and the modern text box in the playable boot. |
| M8 Release | Packaged builds and G5. |

## Decisions (approved by the user, 2026-10-30)

1. The testing bar above: memory exact at every checkpoint, pixels exact on sampled frames, frame-exact only for the opening.
2. C++ for the rasterizer, beside the existing audio extension.
3. One disc pack instead of new content roles.

## Risks

- The `rule_state` bridge could change behaviour of verified code. Mitigation: it is tested both ways, and the opening frame-timing and loop routes must stay identical before any module moves.
- Some overlay data may live in regions the headers do not describe. Mitigation: the memory model loads the real overlay images, so unknown regions still hold the right bytes.
- GDScript speed with byte-level memory access. Mitigation: game logic is a small part of each frame; measure on the pilot before fanning out.
