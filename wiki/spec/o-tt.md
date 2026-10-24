---
type: spec
updated: 2026-10-24
---

# Overlay TT: port spec (TwinBee Time Attack mini game)

Phase 2 analysis of `src/ovl/TT` ([[tickets/T-27024-p2-o-tt]], phase [[tickets/T-27000-phase-2]]). Rename table: `config/renames/proposals/o-tt.tsv` (995 rows). Sources: the matched C, `include/ovl/TT.h`, the callers in the main exe, BUNKAKEN, BUNKASAI and OPTION, and the emulator ([[emulator]]). Facts only, in my own words; no game text, assets or asm are quoted. "HYPOTHESIS" marks a game-meaning claim the code does not prove.

## 1. What it is

A self-contained vertical shooter in the style of the TwinBee series, played against the clock. The overlay is the whole game: its own title screen, menus, four stages with a boss each, ranking table, name entry and attract demo. The emulator confirms the identity: the title screen shows the TwinBee Time Attack product logo, a four item menu (start, ranking, option, exit) and a copyright line.

- There is no score. The player is judged by the **elapsed time** until the last boss falls. Losing a life does not reset the clock.
- Speed is part of the game: the world scroll speed depends on how far up the screen the ship is, from 0.5 to 3 px per frame (six steps). Flying high is faster and shortens the stage.
- Per stage: scrolling field with air waves, clouds, ground targets, then a boss. After four stages the run is complete and the time can enter the top three ranking.
- Name prefix decision: functions and data use `tt_` (the established `Tt*` type prefix, overlay name TT, presumably stands for TwinBee Time attack: HYPOTHESIS); the source folder is `twinbee_time_attack`. The knowledge base proposed `twinbee` or `tt_game` (hypothesis); this is now confirmed by the title screen, `tt_` is kept as the short form.
- Vocabulary used below. **Option** is a follower ship (`tt_options`, `TtOption`; the name is the series term: HYPOTHESIS); the menu behind the title item labelled option is the **settings menu** (`tt_state_settings`, `tt_settings_menu_*`). **Bell** is the HYPOTHESIS name of the pickup family (6.4). **Form** is a boss fight stage (form 0, form 1; bosses 3 and 4) and **step** the attack or move cycle inside a form (`boss+6`). **Shot level** is the stored value 8, 9 or 10 (`TT_SHOT_LEVEL_8`, `_9`, `_10`). **Arm swing** is the ship's arm animation (6.3).

## 2. How it connects to the rest of the game

| Link | Facts |
| --- | --- |
| Mode | main mode (core `GameState.mode`, address `unk_1108`) equals `MODE_TT_GAME` (0xD0; also tested as 0xD0 or 0xD1 by `func_80079524`). |
| Loader | `func_8007959C` (src/main/800789E0.c). Step 0: sets bit 0 of `D_800E7D4F` (core type `ExtraModeFlags`, HYPOTHESIS name; the game has been started), clears sound, reads 0x60 sectors from LBA 0x9A6A to 0x80132000. Step 1 waits, step 2 checks the file sum, step 3 stores `tt_main` (0x8013E850) in `D_80123110` (core `overlay_entry`) and calls it. `tt_main` runs its own frame loop and returns only when the game is left. |
| Callers | OPTION overlay: the extras menu row 0, enabled by bit 0 of `D_800E7D4F` (the loader sets that bit on every TT start, so the entry appears after the first TT launch); it clears bit 1 (direct mode) of `D_800E7D4F` and calls `main_state_set(MODE_TT_GAME)`, so TT starts interactive with the title. BUNKAKEN and BUNKASAI: ten call sites (the festival game exhibit) set bit 1 of `D_800E7D4F` and the 3-bit field in bits 2-4 (stage 1..4), save the return mode in `unk_720..722`, then `main_state_set(MODE_TT_GAME)`. |
| Direct mode | Bit 1 and bits 2-4 of `D_800E7D4F` (word view `D_800E7D4C` bits 25 and 26-28) make TT skip the title and play stage N with the recorded input (see section 7.4). In that mode no player input is read. Bit 5 of `D_800E7D4F` (word bit 29) is copied by `tt_config_load` to `sys.use_pad2`, which makes the game read pad 2 as player input. HYPOTHESIS: this is the festival "game exhibit" where the recorded run is shown. |
| Result | On leaving, `D_800E7D4E = run.stage` (stages cleared, 0..4). BUNKAKEN and BUNKASAI compare it with 2, 3 or 4 to pick one of two dialogue lines, so it is the success measure of the exhibit. |
| Save area | Read at start by `tt_config_load`, written at exit by `tt_main`: three ranking entries `D_800E7D3C..D_800E7D4A`, shot and bomb button codes `D_800E7D4B`, `D_800E7D4C` (low byte), flags `D_800E7D4D` (bit 0 auto-fire, bit 1 display width), result `D_800E7D4E`. The whole area lies inside core's `system_data` (`D_800E7D10`), at offsets 0x2C to 0x3F. The ranking, button codes and result are always written; the flags only when the title is enabled or no stage was selected. |
| Services used | Main-exe services, named as in the core rename table (`config/renames/proposals/core.tsv`): the sound command queue `snd_command` (`func_80044750`); the sound group load, which first sets its destination with `func_800451E0` (`load_dest_base_set` in the h-cdio table); CD reads `cd_read_sectors_nowait` (`func_800462C8`), `cd_audio_command` (`func_80045414`) and the finished-command counter `cd_done_count` (`func_800460DC`); libgpu draw and display envs; libgte rsin/rcos (`func_800A0070` sine, `func_800A0140` cosine; 4096 units per turn, results scaled by 4096; no core name yet); memcpy `func_800AE0A0`, bzero `func_800AE080`, printf `func_800AE0B0` (debug prints of the event id and of the exit only; no core name yet). |

## 3. Memory model

All game state lives in 22 blocks laid out back to back at 0x80110000 (outside the overlay). The pointers sit at `D_80158A60..D_80158AB4` and never change. `tt_state_init` and `tt_state_clear_all` zero them (`func_800AE080`).

| Pointer | Name | Address | Bytes | Content |
| --- | --- | --- | --- | --- |
| D_80158A60 | tt_sys | 0x80110000 | 0xA8 | pads, game input, ranking copy, options, flags, sound dedupe flags |
| D_80158A64 | tt_core | 0x801100A8 | 0x38 | scene state machine, fade, background mode, hard mode |
| D_80158A68 | tt_video | 0x801100E0 | 0xC | vsync event id, draw buffer index |
| D_80158A6C | tt_run | 0x801100EC | 0x20 | scroll speed, elapsed time, stage index, lives |
| D_80158A70 | tt_stage_script | 0x8011010C | 0x20 | stage progress and script cursors |
| D_80158A74 | tt_player | 0x8011012C | 0x74 | the ship |
| D_80158A78 | tt_options | 0x801101A0 | 3 x 0xA4 | follower ships ("options": series term, HYPOTHESIS name) |
| D_80158A7C | tt_arms | 0x8011038C | 8 x 0x60 | arm sprites: 2 for the ship, 2 for each option |
| D_80158A80 | tt_shots | 0x8011068C | 12 x 0x54 | player air shots |
| D_80158A84 | tt_bombs | 0x80110A7C | 8 x 0x58 | thrown ground bombs |
| D_80158A88 | tt_bomb_targets | 0x80110D3C | 8 x 0x58 | aiming markers for the bombs |
| D_80158A8C | tt_boss | 0x80110FFC | 0x424 | the current boss (shared by the four stages) |
| D_80158A90 | tt_bells | 0x80111420 | 32 x 0x6C | collectable pickups, and the falling hazard a broken one becomes (HYPOTHESIS: "bells", 6.4) |
| D_80158A94 | tt_clouds | 0x801121A0 | 6 x 0x68 | clouds |
| D_80158A98 | tt_air_enemies | 0x80112410 | 64 x 0x78 | flying enemies |
| D_80158A9C | tt_ground_targets | 0x80114210 | 32 x 0x78 | ground enemies |
| D_80158AA0 | tt_enemy_shots | 0x80115110 | 128 x 0x68 | enemy projectiles |
| D_80158AA4 | tt_sprites | 0x80118510 | 128 x 0x64 | generic sprite records (UI, titles, boss decoration) |
| D_80158AA8 | tt_texts | 0x8011B710 | 128 x 0x14 | text records |
| D_80158AAC | tt_boss_rope_buf | 0x8011C110 | 0x3BF0 | rope workspace of the stage 1 boss (the code uses 2 x 0xEFC of it) |
| D_80158AB0 | tt_xforms | 0x8011FD00 | 25 x 0x10 | sprite transform records (scale, centre, rotation) |
| D_80158AB4 | tt_frames | 0x801E2000 | 2 x 0x80B8 | double buffered GPU command buffers |

Records are free when byte 0 is 0. Every pool has a `find_free` and a `clear` function. Coordinates: x is centred (about -0x80..0x7F), y runs 0 (top) to 0xE8; positions are 16.16 fixed point (`pos_x` at +0x20, `pos_y` at +0x24 for most records) with the integer halves cached at +0x14 and +0x16; velocities at +0x28 and +0x2C are added every frame. All motion is per frame at 60 Hz.

### 3.1 Key blocks

`TtSys` (offsets from the table): +0x08 frame counter, +0x0A run frame counter (not advanced while paused), +0x10/0x12/0x14 pad 1 held, previous, newly pressed; +0x18.. pad 2; +0x20/0x22/0x24 game input held, previous, newly set (direction bits 0x8000 left, 0x2000 right, 0x1000 up, 0x4000 down, bit 0 shot, bit 1 bomb); +0x30 three 6-byte `TtRankingEntry` (u16 time in frames, 3 glyph codes, 0xFF); +0x43 auto-fire option; +0x44 shot button code (default 7); +0x45 bomb button code (default 6); +0x46/0x47 auto-fire counters; +0x49 display width option; +0x4C pointer to the recorded input table and +0x50 the frame index into it; +0x52 stage select (1..4, 0xFF attract demo, 0 none); +0x53 title enabled; +0x54 read pad 2; +0x55 leave-game flag; +0x56 64 per-frame sound dedupe flags; +0xA4 flag "a later stage's data is resident".

`TtCore` (all names are rows of the table): +0x00 `state` (0..8, section 5); +0x02 `sub_state` (0 init, 0x20 fade in, 0x40 run, 0x80 fade out, other values per state); +0x04 `sub_step` and +0x06 `phase`, further step words cleared on a state change; +0x09 `request_next` ("request follow-up state"); +0x0A/0x0B state flags; +0x0C `combo_frames` (u16 frames the reset combo has been held), +0x0E `combo_enabled`, +0x0F `combo_armed`; +0x10 `paused`; +0x11 `pause_allowed`; +0x12 `timer_running` (the clock runs while 1); +0x13/0x14/0x15 `tint_r/g/b` (flat background colour); +0x18 `tile_map` (descriptor pointer); +0x1C `bg_mode`; +0x1D..0x1F `fade_r/g/b` (colour scale, 0x80 = normal); +0x20 `code_timer` and +0x21 `code_step` (button code progress, pause code and title code); +0x22 `autoplay`; +0x24 `hard_mode` (HYPOTHESIS: name; return fire after hits, switched on by the title code), +0x25 `hit_counter`; +0x26 `timer` (state timer); +0x2E `fade_level` (the ranking screen reuses the word as `scroll_offset`); +0x32 `warp_phase` and +0x34 `warp_divisor` (warp background: the phase counts up one per frame; the divisor goes from 0x1000 down to 0x100 in steps of 0x20 for mode 0x10, and back up in steps of 0x40 for mode 0x11, after which the mode becomes 2).

`TtRun`: +0x00 scroll speed; +0x04 target speed; +0x08 elapsed time in frames (u16, cap 0x8CA0); +0x0A menu cursor; +0x0B stage (completed stages, 0..3 while playing); +0x0C lives (extra lives); +0x0D global bell counter; +0x0F run finished; +0x10..0x1E screen timers, ranking slot (+0x18), name entry column (+0x1A) and glyph (+0x1C).

`TtStageScript`: +0x00 progress (16.16 px, `progress += scroll_speed` each frame); +0x04 cursor of the air script; +0x08 cursor of the ground script; +0x0C boss controller; +0x1E stage cleared flag.

## 4. Frame loop (`tt_main`, 0x8013E850)

1. Init once: `tt_state_init`, `tt_config_load`, `tt_vsync_event_init`, `tt_video_init`, `tt_vram_clear`, `tt_assets_load` (boot load of palettes, tile pages, sprite sheets), `tt_sound_banks_load`.
2. Outer loop (restart): core combo and pause flags cleared; sound commands 0x21, 0x22, 0x2F, 0x21, 0xF1; CD queue reset.
3. Inner loop, one pass per vsync:
   1. `tt_frame_begin` (flip buffers, clear the 16 entry ordering table).
   2. `tt_reset_combo_poll`: if SELECT+START+L1+R1 were held for 0x1F frames: with the title enabled (`sys.title_enabled`) the outer loop restarts (everything cleared, state 0 again); otherwise the state becomes 8 (exit).
   3. `tt_pause_update` if pausing is allowed (stage play only, not in autoplay); then, if not paused, the state handler `tt_state_table[core.state]()` and `sys.run_frame_counter++`.
   4. If `sys.leave_game` is set: leave the loop (exit path in section 2, Save area).
   5. The time text is refreshed (`tt_time_text_set(run.time, 0x27)`), `sys.frame_counter++`, then the draw side in this order: texts, generic sprites, player, options, player shots, bombs, bomb targets, air enemies, bells, boss, clouds, ground targets, enemy shots, background (`tt_bg_draw`); then draw sync, vsync wait, `tt_pad_update` for the next frame, `tt_frame_end`, CD queue update, sound dedupe flags clear.
4. The pause freezes the state handler but not the clock: the vsync event keeps counting while `core.timer_running` is set, so pausing gives no time advantage.

The vsync event (`tt_vsync_tick`, root counter 3 event, once per vblank) increments `run.time` while `core.timer_running` is set and stops at 0x8CA0 (36000 frames, 10 minutes).

## 5. Scene state machine

`tt_state_table` (0x801564B0), indexed by `core.state`. Transitions are written by the state handlers; "next" below means the value stored in `core.state`. A state asks for a change with `core.request_next` (byte +9); the handler picks the target.

| # | Name | Function | What it does | Leaves to |
| --- | --- | --- | --- | --- |
| 0 | load | `tt_state_load` | Initial state; it sets the state to title (1) first. In direct mode with a stage 1..4 selected it loads the data set of that stage (3 stage files, 2 common) and starts stage play instead. | title (1), stage (2) |
| 1 | title | `tt_state_title` | Logo animation, menu of four items, secret code for hard mode. Idle for 0x708 frames starts the attract demo (state 2 with `autoplay`, stage 0xFF). | stage 2 (start, demo), ranking 7, settings 3, exit 8 |
| 2 | stage | `tt_state_stage` | One stage: init (sub 0), reload for the next stage (sub 1), scroll phase (0x40) until progress > 0x400 px, boss phase (0x50), cleared (0xA0). All object updates run here. | game over 5, stage clear 4 (set by the player state machine) |
| 3 | settings | `tt_state_settings` | Settings menu (title item labelled option): button config, auto-fire, display width, back. Sub-state 0x50 is the button config screen. | title 1 |
| 4 | stage clear | `tt_state_stage_clear` | Loads the next data set (stages 1..3), shows the stage clear screen for 0x191 frames (0x1AF after the last stage), skippable unless autoplay. | stage 2 (sub 1) for stage 1..3; after stage 4: name entry 6 (or exit 8 in autoplay) |
| 5 | game over | `tt_state_game_over` | Scroll continues, no player, 0x1A4 frames (START skips unless autoplay). | ranking 7 (normal), title 1 (attract demo), exit 8 (direct mode) |
| 6 | name entry | `tt_state_name_entry` | Entered after the last boss. `tt_ranking_try_insert` puts the time in the top three; if it does not qualify the state ends at once. Otherwise the player edits a 3 glyph name (48 glyphs: left/right changes the glyph with auto-repeat, L1/R1 move the column, circle accepts and advances, cross erases, START ends; 0xE10 frame limit, then 0x3C frames of hold). | ranking 7 |
| 7 | ranking | `tt_state_ranking_view` | Shows the three entries for 3600 frames (300 in autoplay), START or circle skips. | title 1 or exit 8 |
| 8 | exit | `tt_state_exit` | Waits 0x1F frames, then raises `sys.leave_game`. | leaves `tt_main` |

State 7 chooses its follow-up as: title enabled -> title; else exit if `run.finished`, else title.

### 5.1 Stage state (2) in detail

- Sub-state 0 (init): in autoplay with stage 1..4 the data of that stage is loaded (waits for the CD) and `tt_stage_setup` sets the recorded input table, the starting time (stage 2: 0xC3E, stage 3: 0x131F, stage 4: 0x235D frames), the stage index (selected stage - 1) and the lives (autoplay: 2 for stages 1 and 4, 0 for stages 2 and 3 and for the attract demo; a normal run: stage 0, time 0, 2 extra lives). It clears the object pools, starts the stage BGM 0x201 (the attract demo stops the music instead, 0xB1), starts the clock (`core.timer_running = 1`), allows the pause unless autoplay, creates the life icons, the time text and the speed gauge, sets background mode 2 (tile map) and calls `tt_stage_script_init` (progress 0, scroll speed 0x8000, picks the stage tables and boss controller by `run.stage`).
- Sub-state 1 (next stage): reloads the sound bank, restarts the stage BGM, clears the field, resets the pools, ship state back to spawn (0x20), timer running, keeps lives, time and the recorded input index, restarts the script.
- Sub-state 0x40: scroll phase. When `progress >> 16 > 0x400` the boss block is zeroed and the state becomes 0x50.
- Sub-state 0x50: `stage_script.boss_callback()` (tt_boss1_update .. tt_boss4_update) runs each frame until the boss is gone (`stage_cleared`).
- Every frame of this state also runs: the option and arm updates, the player (`tt_player_update`), shots, bomb targets, bombs, bells, clouds, air enemies and their script, ground targets, enemy shots, the collision pass, the speed gauge and the scroll speed update.
- Pressing START or circle during the attract demo returns to the title.

## 6. Rules

### 6.1 Clock, speed and stage length

- Elapsed time is a frame count (1/60 s). It is shown as minutes, seconds and hundredths; 0x8CA0 shows the maximum and is also the "cheated" marker.
- Scroll speed target: 0x30000 (3 px/frame) when the ship's y is above 0x50; below that it is 0x28000 minus 0x8000 per 32 px of y below 0x50, clamped to 0x8000..0x30000; while `progress >> 16 > 0x400` (boss) the target is 0 (the field stops). The speed eases to the target: -0x400 per frame when above it, +0x200 per frame when below, snapping when within 0x1000.
- The six step speed gauge (`tt_speed_gauge_update`) lights step `speed / 0x8000` and plays SE 0x611 when it changes.
- A stage lasts `0x400` px of progress, i.e. 342 frames at top speed to 2048 frames at the lowest, before the boss appears.

### 6.2 Player

- States (`tt_player_update`): 0x20 spawn: flies in from y 0xF8 to 0xC0; 0x30 ready: controllable but invulnerable for 0x5A frames (blinking); 0x40 play; 0x60 hit: death animation, bounces downward (y velocity +1 px/frame) until the animation ends; 0x61 dead: if `run.lives == 0` the core state becomes 5 (game over, timer stopped), else `lives--` and the ship respawns at 0x20; 0xA0 clear exit: after a boss defeat flies off upward, then core state 4.
- Death (`tt_player_hit`): invulnerable flag on, options lost (`has_options = 0`), shot and speed levels kept, scroll speed forced to 0x8000 while still before the boss, sounds 0x610 and 0x607.
- Movement (`tt_player_control`, `tt_player_move`): per axis speed `speed_level * 0x8000`; diagonal moves use `speed_level * 0x6000` per axis. Speed level starts at 4 (maximum 10, minimum 1). x is clamped to +-0x78, y to 0x38..0xE0 (not while spawning or dying). Tilt frame 0..4 comes from how long left or right is held (thresholds 3 and 7 frames; 1-2 left, 3-4 right).
- Input (`tt_pad_update`): the game input word is built from the pad: direction bits as in the pad word, shot from the button whose code is `shot_button`, bomb from `bomb_button` (codes 0 L2, 1 R2, 2 L1, 3 R1, 4 triangle, 5 circle, 6 cross, 7 square). With auto-fire on, a held button produces a new press every 7 frames. Shooting and bombing react to new presses only. `use_pad2` reads pad 2 as the player. In autoplay the byte per frame from the recorded table is used instead: high nibble = direction bits (shifted to bits 15..12), low bits = shot and bomb.
- Starting power by stage select (`tt_player_power_preset`, direct/autoplay): stage 1 and attract: no options, speed 4, shot level 8; stages 2 and 3: shot level 9; stage 4: options on, speed 5, shot level 10.

### 6.3 Shots, bombs, options

- Air shots (`tt_shots`, 12): a new shot press with levels 8 and 9 takes the first free of slots 0-1 (one forward shot, up speed 6 px/frame); at level 10 three at once (slots 0, 2, 4: forward, left 3 px/frame, right 3 px/frame) when all three are free. With options active every press also fires one shot per option (slots 6-11). Hit sequence on impact: sprite flashes in states 0x80..0x83, freed at 0xA0.
- Damage to a boss: shot level 9 shot (record kind 9) 4, normal shot 2, option shots (slot index >= 6) 1.
- Bombs (`tt_bombs`, `tt_bomb_targets`, 8 each): a new bomb press needs a free pair and a free arm (`arm_active_left/right`); `next_arm` alternates. A marker is placed 0x60 px ahead of the shooter, snapping to a ground target 0x11..0x5F px ahead and within 0x20 px sideways that is not locked yet; markers scroll with the stage and expire after 0x79 frames. The bomb flies up at 4 px/frame and steers sideways toward the marker; when its y passes the marker y it explodes: the locked target goes to state 0x70, or if none was locked every ground target within 8 px in both axes. With options each option also drops bombs (pairs 1..3).
- Options (follower ships, `tt_options`, 3; they appear with bell kind 3, with the cheat, or by the stage 4 preset): each option replays the position its leader had about 10 frames earlier (10 sample history; option 1 follows the ship, option 2 follows option 1, option 3 follows option 2). They shoot and bomb together with the ship. They are lost when the ship is hit. Their boxes remove enemy shots (except 0xAB and 0xAD) and blow up air enemies they touch.
- The ship's two arms play a five step swing (`arm_step_*`, frame limits `tt_arm_swing_frames`) that a bomb press starts (`arm_active_*`); on step 4, frame 1 the swing clears two bytes (+1 and +0x3E) of the bomb whose arm index (+0x3D) matches. HYPOTHESIS: this is the bomb throw; the code shows the trigger and the release, the motion was not checked on screen (open question 6).

### 6.4 Bells (HYPOTHESIS: name)

HYPOTHESIS (whole family): "bell" is the TwinBee series name for this pickup and is a guess from outside the code, which shows only a pickup that changes kind when shot. Every name with "bell" in the table (functions `tt_bell*`, `tt_bells*`, data `tt_bells`, `tt_bell_kind_sequence`, `tt_anim_list_bell_*`, type `TtBell`, enum `TtBellKind`, fields `bell_kind`, `bell_counter`, `cycle_count`, `popup_*`, the constants `TT_BELL_*`, `TT_CMD_CLOUD_WITH_BELL`, `TT_LAYER_BELL`, `TT_SE_BELL_*` and the file `bell.c`) carries the HYPOTHESIS marker in its note. Notes of other rows that mention bells only descriptively say "pickup" or "bell family" and point here. The rules below are what the code does (read from the code; the falling hazard was not yet seen on screen).

- Bells (`tt_bells`, 32) are pickups, except a broken bell that is hit again, which becomes a hazard (below). A cloud with kind 0x30 spawns a bell at its position (state 0x20, hidden under the cloud, moving with it). A player shot that hits a bell (`tt_bell_hit_by_shot`) releases it (state 0x40), bounces it away from the shot (vertical velocity -1.5 px per frame; horizontal +0x1000 (to the right) when the shot x minus the bell x is below -1, -0x1000 (to the left) when it is 2 or more, else 0; bells in the popup states 0x90 and above cannot be hit), and advances the **global** counter `run.bell_counter` (1..0x14). The bell kind is read from `tt_bell_kind_sequence[counter]`: counter 5 -> kind 1, 10 -> 2, 15 -> 3, 20 -> 6, every other value -> kind 0. The kind is then replaced by 0 when it would be useless: kind 1 only when the shot level is **exactly** 9 (`player.shot_level == 9`), kind 2 when the speed level is 10 or more, kind 3 when the options are already on, kind 6 when the speed level is 0 (never true: the level starts at 4 or 5 and stays at 1 or more, so a kind 6 bell is never blanked). The palette is `0x3C84 + kind`.
- Quirk to keep (a port must reproduce it or document the change): the shot level can be 10 (item 0x6F and the stage 4 preset set it). At level 10 a kind 1 bell is not replaced by 0, and `tt_bell_collect` kind 1 sets the level to 9, so collecting it **lowers** the shot. The shot level 8 -> 9 case is the normal gain. The check is made when the bell is hit, not when it is collected, so a bell released at level 8 and collected after an item raised the level to 10 downgrades as well. The recorded stage 4 run (preset level 10) is the regression test for this.
- Breaking: when a hit pushes the global counter past 0x14 it wraps to 1 and the `cycle_count` of the bell that was hit rises by one (each bell counts only the wraps its own hits cause). On the second wrap the bell breaks: state 0x60, broken sprite, kind 0 (collecting it does nothing but free it), and the global counter is set to 0. A hit on a broken bell only bounces it (its kind is not recomputed) while the counter, after its increment, is below 4. The counter keeps counting hits on every bell, so the fourth hit after a break, on any bell, can already be the one that turns a broken bell.
- **Falling hazard** (rule a port must implement): a hit on a broken bell with the counter at 4 or more does not bounce it. The bell changes to its falling animation and state 0x70 with horizontal speed 0, no gravity and a vertical speed of +2 px per frame (0x20000), straight down. It is freed when it passes y 0x100, when another shot hits it (SE 0x60C; the shot ends), or on contact with the ship. Contact uses the pickup test (`tt_hitboxes` 4 against the player box, only while the ship is in state 0x30 or 0x40), but `tt_bell_collect` handles state 0x70 differently: if `player.invulnerable == 0` it runs `tt_player_hit`, sets player state 0x60 (the death sequence of 6.2) and frees the bell; an invulnerable ship (spawn protection, blinking) lets it pass and the bell keeps falling. It never gives a pickup effect and cannot be collected.
- Collecting (`tt_bell_collect`, player box 3x4 against the 6x6 pickup box, only in player states 0x30 and 0x40): kind 0 nothing but the bell is freed (SE 0x60A); kind 1 shot level 9 (SE 0x605; also from level 10, see the quirk); kind 2 speed +1 (0x602, no upper clamp); kind 3 options on, three options appear (0x604); kind 6 speed -1, minimum 1 (0x603). A collected bell (kinds 1, 2, 3 and 6) becomes a label popup (state 0x90..0x92: grows to scale 0x1000, then holds 0x3C frames). A bell in state 0x20 (under a cloud) or in a popup state ignores the ship; state 0x70 follows the hazard rule above.

### 6.5 Clouds

Six clouds fall from above the screen at a speed given by the script command and pulse in size between 0xA01 and 0x1000; they are drawn above the bells. Script kind 0x30 hides a bell inside, 0x31 is empty. Clouds themselves are not hit by anything.

### 6.6 Air enemies and waves

- The air script (`stage_script.air_cursor`) holds 6-byte commands: s16 time (progress in px), u16 command, u8 argument 1, u8 argument 2; the list ends with a time of -1. A command fires when `progress >> 16 >= time`. Commands 0xC0..0xC7 start a formation of 3..6 enemies of kind 0x40..0x48 (argument 1: side, 0 enters from the left, 1 from the right). 0x30 and 0x31 spawn a cloud (`tt_cloud_spawn`: argument 1 = signed x, argument 2 = extra fall speed in units of 0.25 px per frame on top of 1 px). Command counts: stage 1 has 11 air commands, stage 2 10, stage 3 9, stage 4 13; times are 0..816 px.
- Kinds and movement (`tt_air_enemy_kind*_move`): 0x40 curved dive (x speed follows y), 0x41/0x42 two flankers and a leader aiming at the player, 0x43 sine weave, 0x44 half circle of radius 0x300, 0x45 swaying column, 0x46 spiral, 0x47 rises on a parabola then hovers, 0x48 waits until level with the player then charges across and rises.
- Fire: kinds 0x40-0x42 shoot one aimed shot after 0x19 frames on screen but only from stage 2 on; 0x43 after 0x11, 0x44 after 0x3C, 0x45 and 0x46 after 0x19, 0x47 after 5 frames of its falling phase, 0x48 after 0x19. Enemies that were seen on screen and leave are freed. Their shots use kind 0xA0 at speed 0x20 (shot speed units: the velocity is `speed * cos/sin(angle)` with the table scaled by 4096, so 0x20 is 2 px per frame).
- Destruction: a hit sets state 0x70 then 0x71 (explosion animation, SE 0x60C); freed when the animation ends. Enemies hurt the ship on contact.

### 6.7 Ground targets and drops

- The ground script (`ground_cursor`): 6-byte commands s16 x, s16 time, u16 kind (0x60..0x63), ending with a time of -1 (stage 1 has 4 targets, stage 2 4, stage 3 6, stage 4 10). The target is placed at the top of the screen when progress reaches `time`; `y = progress - spawn_y` so it scrolls with the field. Only bombs (and the clear-all effects of smart bomb and boss start) destroy ground targets; air shots pass through them. Per frame (`tt_ground_targets_update`) the state machine: 0x40 live; 0x70 destroyed (SE 0x60D, explosion state 0x71; with hard mode on, alternately a fan of 4 or a 3-way aimed burst); at the end of the explosion `tt_drop_roll` decides the item from the displayed time digits: hundredths 00 -> kind 0x6E, hundredths 33 -> 0x6F, else 0x6D (each special case happens on one frame of 60); 0x6D waits 0x3C frames (state 0x78), blinks 0x1E frames (0x79) and disappears (0x80); 0x6E and 0x6F fall with the scroll (state 0x7A) and are collectable by the ship.
- Item 0x6E: smart bomb (all live air enemies destroyed, all live enemy shots removed, SE 0x60F). Item 0x6F: shot level 10 plus a popup.
- A live target fires one aimed shot when its animation cycle ends while y < 0xC0 unless the player is within 0x20 px in both axes.

### 6.8 Enemy shots

128 slots, kinds: 0xA0, 0xA1, 0xA2 small round shots (3x3 box, different palettes), 0xA8 boss small (3x3), 0xA9 and 0xAA boss shots (5x5), 0xAB sprite-animated shot of the stage 3 boss (7x7), 0xAC homing (5x5), 0xAD beam of the stage 4 boss. Patterns (each takes a position, a speed, a kind and for the fans a spread): single aimed (`tt_enemy_shot_fire_aimed`), single at an angle, 3-way aimed (aim, aim-spread, aim+spread), 3-way straight down (angles 0x400, 0x400+-spread), fan of 4 aimed, fan of 9 aimed, unused twin down. Velocity is `cos(angle)*speed` and `sin(angle)*speed` in 16.16 (angles from `ratan2` of the player position difference). A shot is freed when it leaves y -0x10..0x100 or x +-0x88. Homing (0xAC) turns 0x18 angle units per frame toward the player for up to 0x100 frames at speed 0x18. The beam (0xAD): state 0x20 for 15 frames follows the boss x, state 0x40 grows the length by 4 per frame to 0x20, state 0x50 travels down 4 px per frame; its box is 2 wide and half the length high while growing, 0x10 high afterwards.

### 6.9 Collisions (`tt_collisions_update`, `tt_box_overlap`)

Boxes are `TtHitbox` (offset x, y, half width, half height); two boxes overlap when `|dx| < w1 + w2` and `|dy| < h1 + h2`. Table `tt_hitboxes`: 0 player 3x4, 1 shot 5x3, 2 air enemy 5x5, 3 small enemy shot 3x3, 4 pickup 6x6, 5 large shot 7x7. Player vs air enemy: the enemy is destroyed and the ship is hit if not invulnerable. Player vs enemy shot: shot freed, ship hit. Player vs bell (ship in state 0x30 or 0x40 only): collect by kind, but a bell in state 0x70 is the falling hazard of 6.4 (the ship is hit when not invulnerable and the bell is freed). Player vs 0x7A item. Player shots vs bells (bell state below 0x90, same ship states): `tt_bell_hit_by_shot` (6.4), the shot ends (state 0x80); a hit frees a state 0x70 bell. Player shots vs air enemies: the shot ends (state 0x80) and the enemy is destroyed (kind 0x47 only from its phase 2). Options' boxes remove air enemies and enemy shots (not 0xAB, 0xAD). Player shots vs enemy shots 0xA2, 0xA9, 0xAA are cancelled (SE 0x509), shots vs 0xAC destroy it with a small burst. With hard mode on (`core.hard_mode`), return fire follows hits of player shots: every second hit on an air enemy or a destroyed ground target (counter `core.hit_counter`) fires a fan of 4 or a 3-way aimed burst alternately (kind 0xA0, speed 0x20); every fourth hit on a boss part (`func_801427A8`, the counter is shared) fires back: in stages 1 and 2 (`run.stage` < 2) a fan of 4 (`func_801342CC`, every eighth hit) or a 3-way burst (`func_80133FB0`, the other fourth hits), in stages 3 and 4 one aimed shot (`func_80133D14`); all with kind 0xA0 and speed 0x20.

### 6.10 Bosses

Common (`tt_boss`, section 3): state 0 init, 0x20 enters from above 1 px/frame, 0x30 intro animation, 0x40 fight, 0x80 dying (sound 0xC1, explosion timeline table, parts removed), 0xA0 gone (timer stops, `stage_cleared = 1`, player state 0xA0). `hp` at +0x30C; `hp_phase` (+6) is 0 above 0x40, 1 below 0x40, 2 below 0x20 for the first form. Shots are tested by `tt_boss_check_hits` against the first four hit box slots (damage: HP -2, -4 for level 9 shots, -1 for option shots) and the next four slots absorb shots without damage; the ship is hurt by slots 4-11. Parts are animated by scripts (`tt_boss_part_anim_step`: frame, hold; -1 end, -2 loop, -3 hide). The boss BGM is 0x200.

| Stage | Controller | HP | Structure |
| --- | --- | --- | --- |
| 1 | `tt_boss1_update` | 0x60 (body), then four side parts of 0x18 each, then 0x48 (core) | Body tracks the player's x slowly; two rope arms of 32 joints (`tt_rope_update`) follow the player and fire aimed shots from the tips every 0x18 frames. At body HP < 0 the rope arms break apart joint by joint (0x50), 16 sprites burst outward (0x51), then the four side parts (parts 2-5) must be shot down (0x53) before the core (part 6, HP 0x48) can be damaged (0x54); shots at the core earlier are blocked (SE 0x506). At core HP < 0 the death sequence runs (0x80). Attack patterns of the later forms are table driven (`tt_boss1_fan_*`). |
| 2 | `tt_boss2_update` | 0x60 | Eight outer ring parts and six inner parts. Moves on a figure-eight; the ring reverses its spin in five steps (`tt_boss2_ring_swing`). Fires every 0x60 frames: aimed (phases 0-1), 3-way aimed (phase 2). |
| 3 | `tt_boss3_update` | 0x80 (form 0), then 0xA0 (form 1); four parts of 0x60 each | Four parts with their own boxes and HP: they die separately and the survivors are removed at the form change. Form 0 moves on sine sweeps (sub-steps 0..3) and attacks by sub-step (aimed shots, downward fans, 3-way, animated shots of kind 0xAB). At HP < 0 the boss moves to the centre (sub-steps 4-6), its HP becomes 0xA0 and form 1 starts: Lissajous path, rings of homing shots (kind 0xAC) at frames 0x80 and 0x100 and fans of kind 0xA0. At HP < 0 in form 1 the death sequence runs. |
| 4 | `tt_boss4_update` | 0xE0 (form 0), then 0x100 (form 1) | Step cycle per form: attack (step 0), sweep sideways (step 1), second attack (step 2). Form 0: spiral aimed shots with a 9-way fan (step 0), three drone parts (0x10..0x12) firing rotating rings (step 2). Form 1: a row of ten beams (eight when boss byte +0x106 is not 0; tables `tt_boss4_shot_*`) and sweeping 0xA1 streams (step 0), a body part swelling and thrown at the player (step 2). At HP < 0 in form 0 a 0x3C frame pause precedes form 1 (HP 0x100); the form change step is state 0x50. |

Open: the exact HP thresholds of the stage 3 and 4 form changes and the attack timelines were read only at the structure level.

### 6.11 Secret codes

- Title: up, up, L1, R1, R1, R1 on newly pressed buttons turns on hard mode (return fire), SE 0x607, and shows a marker text. Each step must follow within 0x10 frames, a wrong press resets the code (`tt_title_code_check`). It works only while the title is enabled and hard mode is still off; the attract demo and a new title visit turn hard mode off again.
- Pause menu: up, up, down, down, left, right, left, right, cross, circle (`tt_pause_code_masks`). Each step must follow within 0x10 frames. On completion: SE 0x70A (before the boss) or 0x80A (during the boss, core state 2 with sub-state >= 0x50), then `tt_cheat_grant_all`: time set to the cap 0x8CA0 (the run can no longer enter the ranking), options on, shot level 9.

### 6.12 Ranking and name entry

Three entries of (u16 time, 3 glyph codes, 0xFF). Defaults: 25544, 29144 and 32744 frames (7'05"73, 8'05"73, 9'05"73) with fixed three-glyph names. `tt_ranking_try_insert` compares the final time with the entries in order and shifts lower ones down; it returns 0 if the time is not below the third. Glyph codes: 0x0A.. are letters A..Z, 0x24 is a dot; code 0x3F is skipped by the text draw (blank). The new entry starts as the first letter plus two filler glyphs (0x2F) and the cursor edits it. Entries are saved at exit.

## 7. Data

### 7.1 Script and level tables
Per stage a pair of 6-byte command lists (air, ground), selected by `run.stage` in `tt_stage_script_init`: `tt_stageN_air_script`, `tt_stageN_ground_script` (N = 1..4), and a tile map descriptor stored at `core+0x18` (`tt_stage_tile_map` at 0x80156494 for every stage; the title stores `tt_title_tile_map` at 0x8015676C). The tile map is 16 columns by 16 rows of 16 px cells drawn from a 3-byte tile record table indexed by a 0x400 entry index table; progress selects the row (`scroll = 0x300 - progress`).

### 7.2 Sprites and animation
`tt_sprite_set_table` (0x80151A50): 12 pointers to sprite sets. A set has frames (u16 sequence index), sequences (part count, first part), parts (tile, x, y, palette byte, flags) and an attribute byte table (bits 0-4 draw mode word, bit 6 adds 0x80 to v, bit 7 adds 0x80 to u) plus a palette base; a tile index gives its cell (low 6 bits: 8 columns of 16 px) and its attribute byte (index >> 6). `tt_part_sprite_draw` draws the parts back to front as 16x16 sprites with the flip bits (bit 0 flips y, bit 1 flips x) xor-ed into the part flags; palette = base + low nibble + 4 * high-nibble row. The frame animators step either a sprite-set frame number with hold counters and loop flags (`tt_sprite_anim_step`, frame entries: tile, hold, loop-back or 0xFF end), or the simple 3-byte list (hold count, tile code (low 3 bits u, rest v), flip byte; 0xFE ends and sets the done flag, `tt_anim_list_step`) for shots, enemies and bells (`tt_anim_lists`, 24 pointers).

### 7.3 Text
Text records show strings from `tt_text_strings` (39 pointers plus 10): 8x8 glyph ids, 0xFF terminated, 0x3F is skipped. Special buffers are written at run time: id 0x27 the time (minutes ' seconds " hundredths), id 0x26 the countdown, ids 0x28-0x2D ranking times and names.

### 7.4 Recorded input
`tt_demo_input_attract`, `tt_demo_input_stage1..4`: 4096 bytes each, one byte per frame, played from frame 0 of the run by `tt_pad_update` (index `sys.demo_index`, wraps at 0x1000). The recording continues across stages (the index is reset only when a run starts). Direct mode and the attract demo both set `core.autoplay`; autoplay disables the pause and the skip buttons. In the emulator the stage 1 direct run left the game about 1500 frames after the start. The result `run.stage` is whatever the recording achieves.

### 7.5 Boss tables
Hit box lists, animation scripts, death timelines (5 s16 per record: frame, x, y, size, part; ends with -1) and attack offset tables per boss, named `tt_boss1_*` .. `tt_boss4_*` in the rename table.

## 8. Hardware layer (coarse)

- Draw buffers: two 0x80B8-byte buffers (`tt_frames`), each with a packet cursor, a 16 entry ordering table (layer 0 front, 15 back) and a packet area. Draw functions append GPU packets (sprite codes 0x64/0x65/0x74/0x75/0x7C/0x7D, textured quads 0x2C/0x2D, flat tiles 0x60/0x61, gouraud quad 0x39) and a draw-mode packet. `tt_sprite_packet_transform` implements flip (modes 0..3), scale (`TtSprXform.sx, sy`, 12-bit) and rotation (`angle`, 4096 per turn). The port replaces all of this with its renderer; the layer order is the contract (`TtLayer` consts in the table).
- Video: draw clip 256x232 inside a 320x240 frame; display is 320 wide or, with the display option, 256 wide (offset 0x20).
- VRAM and CD: `tt_assets_load` reads the full asset set at boot (palettes into a 256x16 block at (0, 0xF0), texture pages, sprite sheets). A stage data set is 3 blocks: 0x2420 bytes at 0x80016000, 0x69E4 bytes at 0x8002E800, and 0x27D60 bytes at 0x8019A000 (the sound API group loads read from that address, so probably a sound bank: HYPOTHESIS); plus common blocks 0xE20 at 0x8001A000 and 0x1A680 at 0x801C2000. LBAs per set: 1: 0xC812, 0xC7C1, 0xC7C5; 2: 0xC869, 0xC817, 0xC81A; 3: 0xC8BD, 0xC86D, 0xC871; 4: 0xC916, 0xC8C1, 0xC8C6; common 0xBFD1 and 0xBF9C. Loads are asynchronous; completion is detected by the core counter `cd_done_count` advancing by 5. Which disc file holds these sectors is not identified (BUNKA/TWINBEE.BIN is one sector; TWINSGD/TBTA.DAT is 170 sectors). `tt_assets_load` also reads these blocks with the blocking reader: the palette block (0x54900 bytes at 0x80162000, LBA 0xBECA, uploaded to VRAM), then 0xE20 bytes at 0x80010000 (0xC08B), 0xFED0 at 0x80162000 (0xC06B), 0x2420 at 0x80012000 (0xD6D4), 0x69E4 at 0x8001C000 (0xD678), 0x27D60 at 0x80172000 (0xD685), a stage-shaped set at the stage set addresses (0x2420, 0x69E4, 0x27D60 from 0xC8A8, 0xC857, 0xC85B) and the common blocks (0xC067, 0xC032). These boot sectors are not part of the four stage sets listed above.
- Sound: everything goes through the core queue `snd_command`. BGM ids 0x200..0x206 (boss, stage, stage clear, game over, ranking, title, name entry), commands 0xB1 fade out, 0xC1 boss defeated, 0xE1/0xF1 pause and resume, groups 0x21, 0x22, 0x2F, 0x4 and bank loads; SE ids 0x501-0x513 (boss) and 0x600-0x618 (player, pickups, menus), `tt_se_request` allows one play per id per frame.
- Pad: `func_80042A00` and `func_80043010` give the two pad words (held buttons, active high); the game keeps its own copies. The masks are core's `PadBits`: `PAD_UP` 0x1000, `PAD_RIGHT` 0x2000, `PAD_DOWN` 0x4000, `PAD_LEFT` 0x8000, `PAD_START` 0x800, `PAD_SELECT` 0x100, `PAD_L1` 4, `PAD_R1` 8, `PAD_L2` 1, `PAD_R2` 2, `PAD_TRIANGLE` 0x10, `PAD_CIRCLE` 0x20, `PAD_CROSS` 0x40, `PAD_SQUARE` 0x80. The button codes 0..7 of the config (`TtButtonCode`) are the bit index in the low byte: L2, R2, L1, R1, triangle, circle, cross, square.

## 9. Port notes

- Logic runs at 60 fixed frames per second; keep integer 16.16 math (positions, velocities) and the 4096-unit angle with rsin/rcos tables so the recorded input tables replay to the same result.
- Rebuild per pool: update functions in the order of `tt_state_stage`, with the same free-slot search (first free slot) so enemy spawn order is stable.
- The recorded input and the exhibit result depend on exact frame timing of the script times, the speed easing and the bomb target rules; they make a good regression test: the stage 1 direct run was checked in the emulator and leaves `D_800E7D4E = 0`.
- Everything under section 8 is replaceable. Data blocks (sprite sets, scripts, tile maps, recordings) are extracted from the CD blocks above.

## 10. Open questions

1. Which disc file holds the absolute sectors read by `tt_assets_load` (LBAs 0xBECA, 0xC032, 0xC067, 0xC06B, 0xC08B, 0xC857, 0xC85B, 0xC8A8, 0xD678, 0xD685, 0xD6D4; the last block ends near 0xD6D9) and by the stage sets (0xBF9C to 0xC916). The overlay itself is a separate file read by the loader at LBA 0x9A6A.
2. Exact form change thresholds and attack timelines of bosses 3 and 4; the boss 2 part hit box table at `D_80155B96` has no label of its own.
3. The meaning of the ranking glyph codes above 0x24 and the entry default name text.
4. Why the exhibit mode resets lives to 0 in stages 2 and 3 (HYPOTHESIS: tuned so the recorded runs end at fixed points).
5. Whether bells are drawn under clouds by design (layer 6 vs 5) or by the sprite order; the port should keep both layers.
6. The bell names (HYPOTHESIS, see section 6.4) and the arm swing purpose (HYPOTHESIS: bomb throw, 6.3); both are read from the code and not verified on screen. The falling hazard of a broken bell (6.4) is likewise read from the code only.
7. Whether the shot level downgrade by a kind 1 bell at level 10 (section 6.4) is intended or a bug in the original; the port keeps it unless the owner decides otherwise.
8. What boss byte +0x106 is: it selects pattern a or b of the stage 4 form 1 beam row (6.10) and has no row in the table.

## 11. Emulator checks used

Save states made under `build/emu/states` (ignored): title menu, TT title, in stage, direct stage 1. Observed: the TT title; stage 1 with clouds, air waves, ground targets and the bomb sight; the stage 1 boss with rope arms at about 22 s of the clock; the ranking screen after a game over with the defaults above; core state sequence 1 -> 2 -> 5 -> 7 at frames 146, 1190, 1612 of a no-input run; direct mode with stage 1 selected plays itself and returns to the main game.
