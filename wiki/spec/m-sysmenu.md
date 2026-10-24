---
type: spec
updated: 2026-10-24
sources: ["src/main/8005A0B0.c", "config/renames/proposals/m-sysmenu.tsv", "config/renames/proposals/core.tsv", "wiki/spec/core.md", "wiki/emulator.md", "wiki/obin.md"]
---

# Main: the player's room screens (system menus, clubs, rumors, holiday preparation), port spec

Phase 2 spec for [[tickets/T-27017-p2-m-sysmenu]] ([[tickets/T-27000-phase-2]]). The unit label says "system menus, clubs, rumors, holiday prep"; the file `src/main/8005A0B0.c` holds more than that: it is every screen that the game draws over the player's room picture. Facts are as the matched C implements them. Names are the proposals in `config/renames/proposals/m-sysmenu.tsv`; core's names (`config/renames/proposals/core.tsv`, [[spec/core]]) and the HUD, weekday, holiday and sound proposals are used where they exist. A claim about game meaning that the code does not prove is marked HYPOTHESIS. No game text is quoted: strings are described in words.

Table size: 264 rows (92 `func`, 27 `data`, 1 `field`, 126 `const`, 16 `comment`, 2 `file`); 7 rows carry HYPOTHESIS. All 109 functions of the file are covered: 92 renamed, 16 O.BIN-named (kept, English comment rows), 1 named by core (`mode_command_screen`).

Evidence used: the matched C and every caller in `src/` and `asm/`; core's field map; the English-patch name `system_settings_init`; the O.BIN developer table (names that are not applied in `config/symbol_addrs_obin.txt` are quoted in notes as evidence of role and order, never used as the new name); and the emulator from the HUD unit's saved state of the weekday command screen on 1995-04-09 (a Sunday, no club, layout A, menu mode A). Emulator facts are marked (emu). The memory-card screens were run through save and load against the emulator's empty virtual card (section 5.4); the mascot spots, the club, rumor and preparation scenes, and the sick, exam and trip rows of the build step could not be reached.

## 1. What the file is

One original object. Every function draws or steps a screen over the same background, the player's room at night (the picture is chosen by season, `room_bg_sectors_weekday[season_index]`), under a window title that names the home. The file holds eight scenes:

| scene | game mode and stage | entry function | section |
|---|---|---|---|
| weekly command screen | mode 0x34 (`MODE_COMMAND_SCREEN`), stages 0, 1 and 0xFF | `mode_command_screen` (core, `func_8005A0B0`) | 3 |
| calendar check | mode 0x34 stage 2; mode 0x46 stage 0x30 | `command_screen_stage_calendar` | 4 |
| data screen (memory card save and load) | mode 0x34 stage 3 | `save_load_stage` | 5 |
| system settings and mascot | mode 0x34 stage 4; mode 0x46 stage 0x40 | `settings_stage` | 6 |
| club joining | mode 0x34 stage 5 (unreachable); mode 0x46 stage 7 | `join_club` (O.BIN) | 7 |
| rumor report | mode 0x44 (`MODE_RUMOR`) | `uwasa_main` (O.BIN) | 8 |
| Christmas preparation | mode 0x42 | `mode_christmas_prep` | 9 |
| New Year preparation | mode 0x41 | `mode_new_year_prep` | 9 |

Call chain per frame: `main_state_dispatch` (`func_80042540`) switches on `mode` and calls the mode handler; the handler switches on `stage`, the stage function switches on `step` (and some steps switch on `substep`), then the handler draws the shared panels (parameters, calendar date, message window, window title, cursor, background, screen frame). The modes are in core's mode table; the stage and step numbers are given below and as const rows (`CommandScreenStage`, `SaveLoadStep`, `SettingsStep`, and so on).

Module prefixes: the ADR gives this file `settings_`, `club_`, `rumor_`; the three kept O.BIN families (`join_club*`, `uwasa*`, `pre_xmas*`, `pre_syogatu*`) keep their stems. New prefixes requested in section 13: `command_screen_`, `room_`, `save_load_`. File name proposal `src/main/home_room.c`, header `include/main/home_room.h`.

## 2. Shared pieces of the room scene

- `room_bg_load_start` (`func_8005AB4C`): `dec_bg_cd_read(room_bg_sectors_weekday[season_index], slot 0)`. Result 0 or 1 means the picture is already decoded: show it, set the brightness to 0x80 and `main_stage_next`. Result -1 means a CD read started: `main_step_next`. `room_bg_load_finish` (`func_8005ABD0`) polls `cd_read_sync_status`; when done it shows the decoded slot; if the decode result is -1 it restarts the read, else sets brightness 0x80 and `main_stage_set(1)`. The rumor scene has an inlined copy (`rumor_scene_init`, `rumor_bg_wait`).
- The sick marker (`room_sick_icon_load`, `room_sick_icon_show`): when the player's health is sick (`player_flags` bits 8-11 = 3) sprite quad 0 is set up as a 30 x 24 sprite at (-12, -10) from the sick image in VRAM (0x3F0, 0x160) with palette row 0xF, ordering layer 0xA. The school-day setup (800737A0) calls the same two functions.
- `icon_layout_stash(restore)` (`func_8005C4CC`, named `save_load_layout_stash` in the table): argument 0 copies the live HUD layout into `GameState.scratch_words`: the 0x40 bytes of `icon_positions` to words 0-15, and five words at word index 32-36: mascot anchor x, anchor y, `mascot_choice`, mascot string length, swing. Argument 1 copies them back. Because `scratch_words` lies inside GameState, the layout is saved with the game. The overlay loader (`func_80079800`) and TACO (`func_80142AF0`) call it too (they reuse the HUD memory). Core's statement "first 0x50 bytes" is the word-0-to-15 part plus padding; the five words sit at +0x80..+0x93.
- Click spots on the room picture (pointer = `cursor_x`, `cursor_y`; a click is `pad_pressed & 0x20`, and every function returns 1 only when it fired).

| function | box (screen coordinates from the centre) | action |
|---|---|---|
| `room_click_window` | x -13..33, y -11..7 | fade the BGM fast (sound command 0xB1), `bgm_stop`, voice clip (0x1D000185, 0x17000397) speaker 0, then voice play (0x300) |
| `room_click_help` | x 65..79, y -31..-16 | `main_state_set(0x3A)`: the on-line help overlay (OLH); tried last |
| `room_click_radio` | x -39..7, y 25..35 | fade the BGM slowly (0xD1), `bgm_stop`, clip pair `room_radio_clips[day_count % 8]`, speaker 0xE, voice play. HYPOTHESIS: the object is a radio (developer name `radio_check`; the box lies over the stereo) |
| `room_click_radio_holiday` | same box | same, table `room_radio_clips_holiday`; only the holiday screen calls it |
| `room_click_mascot` | within 12 pixels of `mascot_actor_x`, 0..47 below `mascot_actor_y` | needs the mascot visible (`mascot_draw_flags` bit 7) and `frame_count - room_mascot_stamp >= 0xF1` (the stamp is never written, so it is 0); sets `current_girl` to `mascot_actor_frame`, plays `girl_voice_clips[girl]`, speaker = girl, voice play |
| `room_click_mascot_holiday` | same | same with `room_mascot_holiday_stamp` and `girl_voice_clips_by_state[girl][variant]`, variant = bit 7 of `get_g_zyotai_s(girl)`; only the holiday screen calls it |

(emu) the boxes were drawn on a screenshot of 1995-04-09: they lie over the window curtains, the stereo and the shelf corner. The window spot's developer name is `hitsuji_check` ("sheep"); nothing in the picture shows a sheep, so the table names it by place. In menu mode A the pointer snaps to the selected icon, so a spot can only be hit with a mouse or in mouse-like mode (menu mode 0, set through the settings screen); there the d-pad moves the pointer 3 pixels per frame at cursor speed 3. Clicks from a saved state in that mode: pointer (10, -2) calls `xa_clip_request_set(0x1D000185, 0x17000397, 0)` from `room_click_window`; pointer (-20, 31) calls it with (0x07000035, 0x080000AE, 0xE) from `room_click_radio`, which is entry 5 of the table on 1995-04-09 (`day_count` 5); pointer (73, -23) calls `main_state_set(0x3A)` from `room_click_help`. The mascot spots could not be run: no mascot is unlocked in the state.

## 3. The weekly command screen (mode 0x34)

Where the player picks the command for the coming week. Reached when the queue reader meets the end marker (core section 3.5), from NAME_ENT and OPTION (stage 0xFF), from OLH (START leaves the help, stage 0), from TEL, and from `save_load_load_finish`. It ends with `main_state_set(0x30)` (the schedule step builds the week).

### 3.1 Stages

| stage | name | steps |
|---|---|---|
| 0 | `COMMAND_SCREEN_STAGE_SETUP` | 0 `command_screen_setup_reset`, 1 `command_screen_setup_wait`, 2 `command_screen_build`, 3 `room_bg_load_finish` (then stage 1) |
| 1 | `COMMAND_SCREEN_STAGE_SELECT` | 0 `command_screen_select_frame` (the only step) |
| 2 | `COMMAND_SCREEN_STAGE_CALENDAR` | section 4 |
| 3 | `COMMAND_SCREEN_STAGE_DATA` | section 5 |
| 4 | `COMMAND_SCREEN_STAGE_SETTINGS` | section 6 |
| 5 | `COMMAND_SCREEN_STAGE_JOIN_CLUB` | section 7 (unreachable from here) |
| 0xFF | `COMMAND_SCREEN_STAGE_RELOAD` | 0 reset, 1 `command_screen_reload_calendar`, 2 build, 3 background wait; the brightness rises 4 per frame to 0x80 from step 2 on; with the room picture cached the build runs a second time in stage 0 (section 5.4) |

`mode_command_screen` also calls `mascot_update` when the mascot is visible and ends with `screen_frame_draw(0)`. Stages 2-5 return to the screen with `main_stage_set(0)` + `main_step_set(2)`: the build step runs again, the reset and the one-frame wait do not.

Reset step (`command_screen_setup_reset`): 2D and 3D drawing on, actors cleared, tile layer off, `boot_hold` = 0, brightness 0, text system reset, `k_disp_start(2)`, next step.

Reload step (stage 0xFF step 1, only after a game was loaded): substep 0 waits until the sound bank is loaded and starts the BGM; substep 1 reads 0x95 sectors from LBA 0x9B94 to 0x801A0000 (the month data blob, the same read as `MODE_MONTH_START` step 0); substep 2 when the read is done runs `month_calendar_load(year, month, 0)`, masks `menu_mode` to two bits, sets `season_palette_fade` to 0xFE and restores the mascot (`mascot_set(mascot_choice)`, `mascot_disp_switch(1)`) when `mascot_choice` < 14.

### 3.2 The build step: what the player may choose

`command_screen_build` initialises the panels (parameters twice, so the second pass draws the digits without highlight; calendar date; message window), puts the player's name on the speaker label, then takes the first matching row of this list. `marker(d)` is `month_calendar[0].marker[d]` of the current month (the lookahead does not read the next month: at the end of a month the index runs into the next record's day kinds, which never match). "Tomorrow" is `day + 1`.

| # | condition | usable icons | start cursor | `icon_count` | monologue (in words) |
|---|---|---|---|---|---|
| 1 | `marker(tomorrow) == 5` (MARKER_EXAM) | 14 only | 14 | 15 | the exams start tomorrow |
| 2 | `marker(tomorrow) == 8` (MARKER_CLUB_SEASON) and the player has a club | 4 only | 4 | 14 | if club is baseball, year is 97 and the baseball club level (`clubs[6].level & 0xF`) is at least 8: the national tournament starts tomorrow; else: the summer camp starts tomorrow |
| 3 | `marker(day + 6) == 12` (MARKER_CULTURE_FESTIVAL) and club id < 6 | not sick: 4 only (cursor 4, message: club work for the culture festival); sick: 7 only (cursor 7, sick marker shown, message: must prepare but the body does not move) | 4 or 7 | 14 | as listed |
| 4 | `marker(tomorrow) == 11` (MARKER_SCHOOL_TRIP) | 14 only | 14 | 15 | the school trip starts tomorrow |
| 5 | `remedial_week` flag (bit 0 of the halfword at +0xF6) | 0, 1, 2 | 0 | 14 | a week of forced remedial classes |
| 6 | health sick | 7 only (sick marker shown) | 7 | 14 | the body does not move |
| 7 | otherwise | every command 0-7 (4 only with a club), no 14 | low nibble of `last_command` | 14 | what to do from tomorrow |

Every row first calls `icon_bar_init` and `icon_layout_apply(icon_count)` (`icon_count` 15 shows the extra icon 14 even in layouts A and B), then `command_screen_icons_reset(lock_commands)`: icons 10, 8 and 9 are always off (they are holiday-only), icon 14 is off in the free layout, icon 4 is off for a player without a club, and a nonzero argument turns off the commands 0-7. Rows 1-6 pass 1 and then enable their icons; row 7 passes 0. After the list: the monologue starts (`k_disp_start(4)`), `cursor_actor_init(0)`, icon 13 off on 1995-04-04 (the first day cannot save), the menu bar colour, and `room_bg_load_start`.

Note the row order: exams (1) and club season (2) win over the festival check (3) and the school trip (4); the remedial week (5) is only reached when none of these fires.

### 3.3 The selection stage

`command_screen_stage_select` runs every frame. Step 0 calls `command_screen_select_frame` (`k_disp_inc`, panels, `icon_bar_update(0)`, `cursor_update`, title, background draw, then the click spots: first `command_screen_icon_confirm`, then mascot, radio and window; if none fired the help spot). It then shows the sick marker if needed and watches the background card job (section 5.5): result 0x13 = done (sound 0x50F, job cleared, `card_stop`); timer above 0x200 or result 0xF0 = failed (sound 0x502, job cleared); with no job the card icon animation is switched off (`icon_card_anim_set(0)`).

`command_screen_icon_confirm` (circle on `menu_cursor_0`, value >= 0). Ignored while L2 moves icons in the free layout. A press first stops any voice (`cd_command(9, 0, 0)`, `snd_command(3)`) and restarts the BGM when nothing plays. Then by icon (`get_icon_can_use` must be 1 where stated):

| icon | name (HUD, holiday units) | effect |
|---|---|---|
| 0-3, 5-7 | weekday commands | `last_command` low nibble := icon, sound 0x500 (`SND_COMMAND_CONFIRM`), `main_state_set(0x30)` |
| 4 | club activity | with a club: if the club's girl (`func_80053564`: first girl whose club nibble equals the player's club) has the `club_member` flag her affection rises by 1; then as above. Without a club: nibble := 4 and stage 5 (unreachable, see section 7) |
| 8, 9, 10 | holiday-only icons | the code would play 0x50D (8, 10) or 0x500 (9) and schedule, but the icons are always locked here |
| 11 | system settings (`ICON_SYSTEM`) | sound 0x50D, stage 4 (no availability test) |
| 12 | calendar (`ICON_CALENDAR`) | sound 0x50D, stage 2 (no availability test) |
| 13 | data (`HUD_ICON_CARD`) | when usable and `card_job` is 0: `icon_layout_stash(0)`, sound 0x50D, stage 3 |
| 14 | extra "proceed" (`HUD_ICON_EXTRA`) | when usable: nibble := 0xE; if tomorrow's marker is 5 (exam) or 11 (trip) sound 0x500, `main_state_set(0x30)` and the matching notice; any other day: `main_state_set(0x30)` and nibble := 7 (rest) |

(emu) circle on icon 0 of the 1995-04-09 state calls `main_state_set(0x30)` from `command_screen_icon_confirm` (frame 30); icon 11 enters stage 4 at frame 46, icon 12 stage 2, icon 13 stage 3; circle on icon 4 (no club) does nothing. With menu mode A the d-pad steps the icons in index order: DOWN and RIGHT go to the next, UP and LEFT to the previous, with wrap-around over 0-13 (UP from 0 reaches 13).

A chosen weekday command ends the screen; its effects (parameter changes, the week queue) belong to the scheduler and weekday units. The only game-state change made here is the club girl's +1 affection and `last_command`.

## 4. The calendar check (stage 2)

A two-page notebook picture: this month on the left, next month on the right, weekday columns, day numbers coloured by day kind, event marks and three coloured wave curves (the 33, 28 and 23 day cycles of the HUD spec) over the pages; a caption says it shows the plans and results of this and next month. `command_screen_calendar_init` sets the title and caption, `calendar_screen_init`, hides the icons; `command_screen_calendar_wait` only reads cross (pad 0x40), circle does nothing; `command_screen_calendar_exit` goes to stage 0 step 2. The page content is drawn by `calendar_view_update` (HUD unit). The holiday handler runs the same stage as its stage 0x30. (emu) icon 12 at frame 45: stage 2 step 0 at frame 46, wait until cross at 150, exit at 151.

## 5. The data screen: save and load on the memory card (stage 3)

The player's save game dialog. The background is the notebook picture (`cal_base_show`). Slots: 14 game files, numbered 1-14; slot 0 is the system file. The labels 1-14 come from `month_label_strings` (HUD name; a table of full-width numerals).

### 5.1 Memory-card interface used here

The card is handled asynchronously by the memory-card module (80053650; not this unit). Calls used, with the names this unit gives them in notes (all have `func_` names today): `card_start` (`func_80053CE0`: busy := 1, phase := 0, result := 0, retry := 0, timer := 0), `card_stop` (`func_80053D10`: busy := 0, timer := 0), and polls that advance the job when called every frame and add 1 to `card_timer`: card detection (`func_80053F04`, final results in `card_result`), the slot scan (`func_80054140`, returns 2 at the end; it checks every slot file and sets `slot_status`, and for slot 0 sets `D_800E8BEE`), the system-file read (`func_80054284`, returns 3), a slot read (`func_8005448C`, returns 4), a slot write (`func_80054590`, returns 0x14) and the system-file write (`func_80054388`, returns 0x13). Other calls: `func_80054694`/`func_80054704` (create and delete a slot file: a free-space test), `func_80055A38` (validate what was read), `func_80055AFC` (apply it), `func_800552DC` (apply the system file), `func_80054E6C` (write the slot's summary into the system data), `format("bu00:")` through `func_80054108`. A card job times out when `card_timer` exceeds 0x200 (tests `>= 0x201`) or `card_result` is 0xF0 (an error code set by the card module).

`D_800E8BEE` is set when the card's slot 0 file opens (the system file exists), by the scan, and cleared by the card reset (`func_80056284`, `func_80054BE4`), by ETC, and by the scan when three tries fail. HUD calls it `slot_info_from_system_data`. With it set, the slot summaries come from the SystemData arrays; without it each slot must be read from the card to show its information.

The slot summary (`func_80054E6C`, run after a save for slot `selected_slot & 0xF`): year, month, day, family name, given name, nickname, the nine parameter words, 16 bytes of girl record 15's event counters and the club nibble, stored in SystemData arrays that are written to the system file. The 16 bytes are filled by the TEL overlay's girl evaluation page (0 for a girl who is not met, else her star rating + 1); the slot panel counts them as the "cleared" figure (HUD spec section 17); whether a nonzero value really means a cleared girl is not shown by the code.

### 5.2 Steps

| step | function | what happens | next |
|---|---|---|---|
| 0 | `save_load_menu_init` | title, two choices (save at x -80, load at x 16, y -16, 48 x 20), the checking text and four dotted lines, a card reset (`func_80056284`: `card_stop`, slot 0's status and `D_800E8BEE` cleared, `card_start`), menu 1 with 2 items, cursor 0 | 1 |
| 1 | `save_load_card_check` | substep 0: wait for card detection: result 1 -> substep 1; 0xF1 (unformatted) -> step 0xA0; 0xFF -> step 0xFF; timeout -> 0xFF. 1: `card_start`. 2: wait for the scan (2); timeout -> 0xFF. 3: if the system file exists restart the job, else show the choice text, enable both choices and go to step 2. 4: wait for the system-file read (3): apply it, `icon_layout_stash(1)`, show the choice text, step 2; timeout or 0xF0 -> 0xFF | 2 |
| 2 | `save_load_menu` | cross: `icon_layout_stash(1)`, back to the command screen (stage 0 step 2). Circle: item 0 -> step 3, item 1 -> step 6 | 3 or 6 |
| 3 | `save_load_save_init` | 14 slot labels in 3 columns of 5 (box 32 x 16, x = 32 * (i / 5) - 124, y = 16 * (i % 5) - 64: the left page), menu 1 with 14 items; picks the first slot with status 1; with the system file: cursor = `selected_slot` - 1, summary of that slot, step 4; with no used slot: step 4; else step 0xC0 for that slot | 4 or 0xC0 |
| 0xC0 | `save_load_save_info_read` | `card_start`, read the slot (returns 4), validate; ok: show its summary, step 4; read or validation failure: slot_status := 0xFF, step 0xFF; timeout or result 0xD0 -> 0xFF | 4 |
| 4 | `save_load_save_select` | cross: back to the command screen and `card_job` := 0. Circle on a slot: empty (status 0): test that the file can be created (create then delete; failure -> step 0xF0), status := 0xFF, `card_start`, step 5; the shown slot: `card_start`, step 5; another slot: with the system file show its summary and move the cursor, else step 0xC0 | 5 |
| 5 | `save_load_save_write` | substep 0 asks whether to overwrite (yes/no with button glyphs; asked even for an empty slot); 1 circle -> 2, cross -> step 3 and a pending status 0xFF goes back to 0; 2 message "saving" (with the system file: do not switch the power off until the icon stops blinking); 3 wait for the write (returns 0x14): status := 1; on timeout or 0xF0 the girl `current_girl % 13` speaks `girl_voice_clips_save_failed` and the step becomes 0xFF; 4 with the system file: `card_start`, `card_job` := 1, `selected_slot` := slot, slot := 0, `func_80054E6C`; always `icon_layout_stash(1)` and stage 0 step 2 | command screen |
| 6, 7 | `save_load_load_init`, `save_load_load_select` | as steps 3 and 4 with the labels on the right page (x = 32 * (i / 5)); in step 7 circle on an empty slot (status 0) does nothing; cross returns | 8 or 0xB0 |
| 0xB0 | `save_load_load_info_read` | as 0xC0 for the load side | 7 |
| 8 | `save_load_load_read` | substep 0 message "loading"; 1 wait for the slot read (4), timeouts -> 0xFF; 2 validate: ok -> 3, else step 0xE0; 3 show the summary and ask whether to continue with this data; 4 circle (sound 0x501) -> 5, cross -> step 6; 5 apply (`func_80055AFC`): ok -> 9, else 0xE0 | 9 |
| 9 | `save_load_load_finish` | see 5.3 | |
| 0xA0 | `save_load_format_prompt` | message that the card is unformatted, then the format question (yes/no); circle formats: success -> step 0, failure -> command screen; cross -> asks for another card in slot 1 and returns to the command screen | 0 |
| 0xE0, 0xF0, 0xFF | `save_load_data_broken`, `save_load_card_full`, `save_load_card_unreadable` | sound 0x502 (the "negative" cue the holiday unit names `SND_ACTIVITY_POOR`), a message, and a START, circle or cross press: 0xE0 -> step 6; 0xF0 and 0xFF -> `icon_layout_stash(1)` and the command screen | |

The slot marks (`save_load_save_marks`, `save_load_load_marks`) draw, every frame, a 30 x 15 semi-transparent box over each of the 14 labels: a grey tint for status 0 (empty) and a second tint for 0xFF. `slot_status` (SystemData +4, 16 bytes, label `save_slot_status`): 0 empty, 1 holds a save, 0xFF file created for a save that has not finished or a slot whose read failed. The prompt lines of the two lists have their own texts (`save_prompt_text`, `load_prompt_text`). ETC (end-of-game system file save) reuses `save_load_save_init`, `save_load_save_prompt` and `save_load_save_marks`.

### 5.3 After a load (`save_load_load_finish`)

The loaded GameState replaces the live one first. Then:

- If the loaded date is 1998-03-01 or 1995-04-04: BGM fade (sound commands 5 and 0xB1), a sound bank load, `func_80041878` (resets calendar, player, girls, queues and event log to the new-game values), `func_80041F48` (clears appointments, clubs and queues), the HUD hidden, and `main_state_set(0x14)` (NAME_ENT). NAME_ENT's own load path (title screen) does the same for 1995-04-04. HYPOTHESIS: a save made on the first day or the final day (clear data) starts a new run from the beginning.
- Any other date: sound 0xB1, a wait of 63 frames (substep 0xC0 counts up to 0xFF), the window title shown, `main_state_set(0x34)`, `main_stage_set(0xFF)` and `day_bgm_load`: the command screen is rebuilt with stage 0xFF.

### 5.4 What the player sees (emu)

Run from the command screen state against the empty virtual card (`build/emu/memcard1.mcd`):

- Icon 13 at frame 45: `icon_layout_stash(0)` once, stage 3 at 46, `save_load_menu_init` once. The screen is a blank two-page notebook with the checking line and animated dots. The check ends at about frame 483 (the BIOS file open of the emulator takes about 34 frames per call, so the slot scan is slow; the real console is much faster): detection ok, no system file, then the two choices (save on the left page, load on the right) with a one-line instruction.
- Save: circle on save: `save_load_save_init` and the prompt at once (no used slot), 14 numbered slots in 3 columns on the left page, all dimmed, the cursor on slot 1. Circle on slot 1: the create test passes, the slot gets the second tint and the overwrite question with the yes and no lines appears (frame 115). Circle at 250: the write ends before frame 391, then `icon_layout_stash(1)` and the command screen is rebuilt from stage 0 step 2 (frame 392); the slot is now status 1.
- Load (card with that save): circle on load: the scan found slot 1 used; its summary is read (about 130 frames) and `save_load_load_select` starts at frame 165 with the right-page list (slot 1 undimmed). The left page shows the slot number, the date, the player's name, the nickname, the club and a count of clears. Circle on slot 1 starts `save_load_load_read`, which reads, validates and asks whether to play on with this data (yes and no lines). Circle at 700: apply, `save_load_load_finish` for 65 frames, then stage 0xFF: reset (frame 767), the month data reload (76 frames), the build step (844). The saved date 1995-04-09 is an ordinary date, so the special branch was not taken.
- Stage 0xFF quirk: the room picture was already decoded, so `room_bg_load_start` called `main_stage_next` from stage 0xFF, and the stage byte wrapped to 0: the setup ran a second time (reset, wait, build; `command_screen_build` ran twice). When the picture has to be read, step 3 of stage 0xFF ends with `main_stage_set(1)` and there is no second pass.

### 5.5 The background system-file write

After a successful save with the system file present, `card_job` is 1 and the command screen's stage 1 keeps polling `func_80054388`: while it runs the card icon animates (`icon_card_anim_set`), and when it finishes the job ends with sound 0x50F, or with sound 0x502 on a timeout. Icon 13 does nothing while `card_job` is set. A port can turn the whole directory into a plain list of files and drop the job.

## 6. System settings and mascot (stage 4)

`settings_stage` dispatches on the step; every screen draws the root labels first (`settings_labels_draw(item)`, item highlighted) and then its own list. Common pattern per setting: an `_init` step builds the choices and sets the start cursor from the stored value; a `_menu` step handles cross (back to step 0) and circle (store the value, show a confirmation line, next step); a `_done` step waits for circle and returns to step 0 with `menu_cursor_0` set to the setting's root item. Step 0 (`settings_root_init`) builds the root list, so the root cursor is the last item used. Entering from the command screen with icon 11 leaves `menu_cursor_0` = 11, and `settings_root_init` reduces it modulo 8, so the root list starts on item 3 (emu).

Root list (menu 0, 8 boxes of 96 x 12 at x -124, y = -64 + 12 * i), `settings_root_menu` (step 1): cross leaves (`main_stage_set(0)`, `main_step_set(2)`); circle by item:

| item | label | goes to step | note |
|---|---|---|---|
| 0 | text speed | 0x10 | |
| 1 | icon mode | 0x20 | |
| 2 | mouse settings | 0x30 | only when `input_device` is `INPUT_MOUSE`; with the pad nothing happens |
| 3 | menu mode | 0x40 | |
| 4 | cursor speed | 0x50 | |
| 5 | voice mode | 0x60 | |
| 6 | mascot | 0x70 | the box has zero width (the label is not drawn, the cursor skips it) while `settings_unlocked_mascot_count` is 0 (emu: new game) |
| 7 | end | 0xFF | |

Each setting (steps `init/menu/done`), the stored field (core name), choices and start cursor:

| setting | steps | stored in | choices -> stored value | start cursor |
|---|---|---|---|---|
| text speed | 0x10-0x12 | text system (`k_speed_set`, `get_k_speed`) | slow 3, normal 1, fast 0 | 3 -> 0, 1 -> 1, 0 or other -> 2 |
| icon mode | 0x20-0x22 | `icon_mode` | TYPE-A 0 (`icon_bar_init`), TYPE-B 1 (`icon_bar_init`), free 2 (`icon_count` := 15 when it was not free, `icon_layout_apply`) | the mode |
| mouse list | 0x30-0x32 | none | confirm button -> 0x33, click mode -> 0x36, click speed -> 0x39 (each `_done` returns to 0x30) | 0 |
| confirm button | 0x33-0x35 | `confirm_button` | right 1, left 0 | 1 -> 0, else 1 |
| click mode | 0x36-0x38 | `click_mode` | single 0, double 1 | the mode |
| click speed | 0x39-0x3B | `double_click_interval` | "initial": 0x20; "change": see below | 0 |
| menu mode | 0x40-0x42 | `menu_mode` | mouse-like 0, menu A 1, menu B 3, mouse-like plus A 2 (items 1-4) | 0 -> 0, 1 or other -> 1, 3 -> 2, 2 -> 3 |
| cursor speed | 0x50-0x52 | `cursor_speed` (and `input_device` := 0xFF) | 1, 2, 3, 4, 6 | 1 -> 0, 2 -> 1, 3 -> 2, 4 -> 3, 6 -> 4, other -> 0 |
| voice mode | 0x60-0x62 | `voice_mode` | always on 1, always off 0, cut by cross 2 | 1 -> 0, 0 -> 1, 2 or other -> 2 |
| mascot | 0x70-0x72 | `mascot_choice` (through `mascot_set`) | girls 0-12, item 13 = hide | the stored value itself (see below) |

Details:

- Click speed (`settings_mouse_click_speed_menu`) acts only while the mouse button chosen as confirm button is held (`mouse_buttons` bit 4 when `confirm_button` is 1, bit 8 when 0). "Change": the first frame with the button held sets `settings_click_speed_ticks` to 0xFF; the next frame sets `double_click_interval` := 0xFF - ticks, clamped to 0x10..0x40, clears the counter and shows the confirmation. Nothing else changes the counter (bss word used only here, ETC clears it), so the result is always 0x10. HYPOTHESIS: the developer build counted frames between two clicks in this word and the counting code was lost.
- Cursor speed: the label set is the extended one (very slow to very fast) when `SystemData.flags[8] >= 9` or `girl_cleared[8] >= 2`, else slow, a little slow, normal, a little fast, fast. The confirmation text has a second wording when `flags[8]` or `girl_cleared[8]` is nonzero. Both are tests of girl 8's entries; the thresholds differ (2 and 9 against 0). After a change `input_device` is set to 0xFF (`INPUT_NONE`); the next controller poll (`func_80042AC8`) sets it again, sees the change, re-initialises the pad (`InitPAD`, `StartPAD`) and zeroes the pointer position and the pad words, so a new speed starts from a clean input state.
- Mascot (HYPOTHESIS on the semantics of the counters, which core names `girl_cleared`): girl i (0-12) appears in the list only when `SystemData.flags[i]` or `girl_cleared[i]` is nonzero. ETC's end-of-game save adds 1 to both `girl_cleared[current_girl]` and `SystemData.flags[current_girl]` (cap 99), so both arrays behave like clear counts per girl (the first per game, the second in the system file). Item 13 hides the mascot. The list has 14 boxes in 3 columns (x = 43 * (i % 3) - 8, y = 16 * (i / 3) - 64, 40 x 16; item 13 is wider). Each frame `settings_mascot_menu` calls `mascot_set(0)` and then `mascot_set(cursor + 1)` when that girl is unlocked, a live preview; cross and circle on item 13 leave the preview as it is; circle on an unlocked girl confirms and goes to step 0x72. Quirk: `mascot_choice` is girl + 1 (0 = none) but the start cursor is set to the choice itself, so the highlight (and the first preview) starts one item after the current mascot's girl.
- `settings_unlocked_mascot_count` adds 13 tests of `SystemData.flags[0..12]` and 13 tests of `girl_cleared[0..12]` and returns the count of nonzero entries.
- Settings screens set no change flag; all fields are in GameState and are saved with each game. The title menu's OPTION overlay edits the same fields.

(emu) stored values checked in memory: icon mode item 1 gives `icon_mode` 1 (`icon_count` 14); cursor speed item 4 gives `cursor_speed` 6; voice mode item 1 gives `voice_mode` 0 (the default in the state was 2); menu mode item 0 gives `menu_mode` 0 and the pointer then moves with the d-pad, 3 pixels per frame. (emu) icon 11 at frame 45: `settings_root_init` once (frame 46) with `settings_unlocked_mascot_count` called twice; `settings_root_menu` per frame; text speed init at frame 116, menu from 117, done from 161; DOWN on the root list skipped the hidden item 6 (3, 4, 5, 7) and circle on item 7 returned to the command screen build.

## 7. Club joining (stage 5, O.BIN `join_club*`)

`join_club` steps: 0 `join_club_init` (hide the icons, clear the text, `club_list_menu`: the 11 clubs in 3 columns of 5, 5 and 1 (x = 84 * (i / 5) - 128, y = 16 * (i % 5) - 56) plus a cancel item at (40, 8); a club whose `quit_flags` bit 0 is set is hidden), 1 `join_club_select`, 2 `join_club_message`, 3 `join_club_exit` (`actor_clear_upper`, `main_state_set(0x30)`).

- `join_club_select`: cross returns (`main_stage_set(0)`, and in mode 0x34 also `main_step_set(2)`); circle on the cancel item (11) does the same; circle on a club stores its id in `player_flags` bits 12-15 and goes to step 2.
- `join_club_message` substeps: 0 announces the joined club (text built from `club_name_table[club]`); 1 waits for text and voice; 2 for clubs 6 and up (baseball, soccer, tennis, swimming, basketball) the club captain says that the third Sunday of every month is club day and members must not miss it, otherwise it skips to 0xFF; 3 `dialogue_text_step`; 4 and 0xFF finish.
- Reachability: from this screen only icon 4 starts stage 5, and `command_screen_icons_reset` locks icon 4 for a player without a club, so the stage is dead code in mode 0x34 (emu: circle on icon 4 in the no-club state does nothing). The holiday mode runs the same three functions as its stage 7 (Sunday, no club), where the join happens in play.

## 8. The rumor report (mode 0x44)

Triggered by the queue reader: at a day-end record (0xFB) every met girl 0-10 with `syoushin >= 0x3D` whose `rumor_done` flag is clear gets the flag set; if at least one did, `main_state_set(0x44)` (core section 3.5). The screen carries no data about the girls and changes no state:

- Stage 0: `rumor_stage_init` step 0 `rumor_scene_init` (text reset, home title, the player's name label, panels, the icon bar shown and locked to the day record's command by `command_icons_lock_to_record`, then icon 0 off and the `last_command` icon on, a monologue that a strange rumor goes around among the girls, room background read; the handler then keeps drawing the bar with `icon_bar_update(1)`), step 1 `rumor_bg_wait`.
- Stage 1: `uwasa0` step 0 `rumor_wait_voice` (typewriter plus voice, `xa_wait`), step 1 `uwasa_exit0` = `main_state_set(0x31)`, the day screen.

The effect of the rumor, if any, is not in this file.

## 9. Christmas and New Year preparation (modes 0x42 and 0x41)

Queue types 0x28 and 0x2C (markers 13 and 15) start them (core table). Both lead into the SHOUGATU overlay.

- Christmas (`mode_christmas_prep`): stage 0 `pre_xmas_init` step 0: panels, no icons, the player says that today there is a Christmas party at the Ijuin house and asks whether to go to the venue, room background read (`room_bg_load_start`); step 1 `room_bg_load_finish`. Stage 1 `pre_xmas`: substep 0 `xa_wait`, substep 1 `main_state_set(0x52)`.
- New Year (`mode_new_year_prep`): `pre_syogatu_card_count` counts the met girls 0-10 whose weighted affection (`get_h_yuukou_table`) is at least 0x4D (`NEW_YEAR_CARD_AFFECTION_MIN`). Stage 0 step 0 `pre_syogatu_card_scene_start`: with a count of 0 it sets brightness 0 and enters mode 0x53 at once (no scene); otherwise it grabs the screen (`dec_bg_capture_screen(0xA0)`), clears actors, shows only the calendar date and the message window, fades the BGM (sound 0xC1) and loads a sound bank. Steps 1-3 (`pre_syogatu_init1/2/3`): wait for the bank, read the card background (sector 0x4167 into slot 1) and show it, play the doorbell (sound 0x603, `SND_DOORBELL`). Stage 1 step 0 `pre_syogatu0`: the player wonders who it is, the postman announces New Year cards (voice clip 0x08000005..0x03000009), the next background (sector 0x4144) is read. Step 1 `pre_syogatu1`: builds one line "got New Year cards from <names>" listing every qualifying girl (names from `get_g_name`, joined with a separator) and says it; then `main_state_set(0x53)`. The scene gives no reward in this file.

## 10. State and data

GameState fields (core names), with who writes them here:

| field | role here |
|---|---|
| `mode`, `stage`, `step`, `substep`, `step_timer` | the machine; this file adds 1 to `step_timer` in two stage handlers |
| `last_command` | low nibble written by `command_screen_icon_confirm`; start cursor of the build step |
| `menu_cursor_0`, `menu_cursor_1` | icon cursor, list cursor (settings root, mascot list use menu 0/1 as in the table above) |
| `pad_pressed` | 0x20 circle, 0x40 cross, 0x860 start/circle/cross for message acknowledgements |
| `player_flags`, `player_flags_hi`/`remedial_week`, `clubs[]` | club nibble (written by `join_club_select`), health, remedial week; `clubs[6].level` read by the build step |
| `icon_mode`, `icon_count`, `menu_mode`, `cursor_speed`, `click_mode`, `double_click_interval`, `confirm_button`, `voice_mode`, `input_device` | settings (section 6) |
| `card_job`, `card_result`, `card_timer`, `save_slot`, `scratch_words` | card state (section 5) |
| `screen_brightness`, `clear_color_*`, `boot_hold` | screen state set by the reset and build steps |
| `girl_cleared[]`, `SystemData.flags[]` | mascot unlock counts (section 6) |
| `SystemData.slot_status`, `.selected_slot` | the save directory (aliases `save_slot_status`, `save_selected_slot`) |

Module globals (new names): `room_radio_clips`, `room_radio_clips_holiday` (8 clip pairs each), `room_mascot_stamp`, `room_mascot_holiday_stamp` (never written), `girl_voice_clips`, `girl_voice_clips_by_state`, `girl_voice_clips_save_failed` (13 girls; shared with ETC and NAME_ENT), `choice_yes_line_text`, `choice_no_line_text` (shared with NAME_ENT), `settings_click_speed_ticks`, `mascot_actor_frame/x/y` (aliases into actor 27), `room_sick_icon_image/palette`, the rodata strings (`calendar_title_text`, `calendar_caption_text`, `save_prompt_text`, `load_prompt_text`, `pre_syogatu_title_text`, `pre_syogatu_doorbell_text`, `save_load_check_text`, `save_load_dots_text`). The voice clip pairs are (start, end) positions for `xa_clip_request_set(start, end, speaker)` (h-cdio spec): the port needs to map each pair to an audio asset of the speaker's XA file.

## 11. Connections

- In: `main_state_dispatch` cases 0x34, 0x41, 0x42, 0x44; the queue reader (types, section 8 and 9); OPTION, NAME_ENT, OLH and TEL send the player back to mode 0x34 (stage 0 or 0xFF).
- Out: mode 0x30 (command chosen), 0x3A (help spot), 0x14 (load of a first or last day save), 0x31 (after the rumor), 0x52 and 0x53 (SHOUGATU flows).
- Shared with the holiday unit: stages 0x30 and 0x40 run `command_screen_stage_calendar` and `settings_stage`; stage 7 runs `join_club_init/select/message`; `command_screen_icons_reset`, `room_click_radio_holiday`, `room_click_mascot_holiday`. With the weekday unit (800737A0): `room_sick_icon_load/show`, `room_bg_sectors_weekday`. With the overlay loader and TACO: `save_load_layout_stash`. With ETC: `save_load_save_init`, `save_load_save_prompt`, `save_load_save_marks`.

### Callees that other units own

Roles shown by this file's use, with suggested names for the owners (no rows in this table, to keep one name per address): `func_80053F04` `card_detect_poll`, `func_80054140` `card_slot_scan_poll`, `func_80054284` `card_system_file_read_poll`, `func_80054388` `card_system_file_write_poll`, `func_8005448C` `card_slot_read_poll`, `func_80054590` `card_slot_write_poll`, `func_80054694` `card_slot_file_create`, `func_80054704` `card_slot_file_delete`, `func_80054108` `card_format`, `func_80055A38` `card_slot_data_validate`, `func_80055AFC` `card_slot_data_apply`, `func_800552DC` `card_system_data_apply`, `func_80054E6C` `card_slot_summary_store`, `func_80056284` `card_reset` (80053650, memory-card unit); `func_80053564` `club_girl_find` (8004F870: the girl whose club nibble equals the argument, -1 for none); `func_80041878` `new_game_state_reset` and `func_80041F48` `game_queues_clear` (80041000, boot unit).

## 12. Peculiarities and dead code

1. `icon_layout_repair` (`func_8005A410`): no caller. It checks the stored icon positions of the free layout (outside x -0x86..0x7C or y -0x54..0x54, or two or more at (0, 0)) and resets them.
2. The weekday club-join branch (stage 5) cannot be reached; the cancel handling for mode 0x34 in `join_club_select` is the same vestige.
3. `room_mascot_stamp` and `room_mascot_holiday_stamp` are never written, so the mascot click cooldown (0xF0 frames) never blocks after the first four seconds of the program.
4. The click speed change always yields 0x10 (section 6).
5. The mascot menu starts one item off (section 6); merely opening it and pressing cross can change or hide the mascot, because the preview runs every frame.
6. The save screen asks "overwrite?" before writing to an empty slot as well.
7. Cursor speed thresholds differ between the label set and the message wording (section 6).
8. After a load the command screen is built twice when the room picture is cached (section 5.4): the first pass runs in stage 0xFF, `main_stage_next` wraps the stage byte to 0 and stage 0 runs again.
9. `D_800E7D11` (`system_data_plus1`) and `D_800E7D2C` (`system_flags_8`) are byte aliases into SystemData used through an odd cast; `settings_unlocked_mascot_count` and the cursor speed screen should read `SystemData.flags[]` when the code is cleaned.

## 13. Prefix and glossary requests

New prefixes for the module row of `8005A0B0` in the ADR map (the ADR row today lists `settings_`, `club_`, `rumor_`): `command_screen_` (the weekly command screen and its calendar check; a game-term word, glossary Systems row "command screen"; note the HUD and scheduler units use `command` in `COMMAND_*`), `room_` (plain word: the player's room picture shared by the weekday and holiday screens: background load, click spots, sick marker), `save_load_` (stems `save` and `load`). Module words rows: `command_screen`, `room`, `save_load`. The English-patch name `system_settings_init` breaks the prefix rule (`system_settings` is not in the map) and is renamed `settings_root_init`.

## 14. Open questions

1. Policy: the table quotes the unapplied O.BIN names (`system_config_*`, `data_save_*`, `uwasa_set`, `pre_xmas_main`, `hitsuji_check`, ...) as evidence and gives English names. If the user prefers the developer names for chains whose neighbours are kept (`uwasa_*`, `pre_xmas*`, `pre_syogatu*`), seven rows change; the dev name is in each row's note.
2. `D_800E8BEE`: HUD proposes `slot_info_from_system_data`; the code here shows it is set from "slot 0 file can be opened" (the system file exists). The owner of 80053650 should settle the name.
3. Core uses `main_state_*` for the machine setters while the ADR and the glossary say `game_mode_*`; this table follows core.
4. `girl_cleared` semantics: clear counts (ETC's end-of-game save increments both arrays, cap 99) rather than flags; core marks it HYPOTHESIS. The cursor speed unlock on girl 8 is probably a hidden-extra gate; no name for it.
5. The room spots: what each one does is confirmed in the emulator (section 2); which object a spot stands for (`radio`, `window`) comes only from the developer names and the picture. The two mascot spots were not run.
6. Mode 0x14: this unit only shows that NAME_ENT's new-run entry is used for clear data and first-day saves; its role is for the NAME_ENT unit.
7. `scratch_words` layout: core says the layout occupies the first 0x50 bytes; the code shows 0x40 bytes at +0 and five words at +0x80..+0x93.
8. Whether the "cleared" count of the slot summary (nonzero entries among the 16 bytes of girl record 15, filled by TEL's evaluation page) means cleared girls; the value 1 is the case "points >= 500" in TEL, which looks like a completion state.
9. Where the rumor's effect on the girls is applied, if anywhere.
