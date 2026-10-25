---
type: spec
updated: 2026-10-24
sources: ["src/main/8005A0B0.c", "config/renames/proposals/m-sysmenu.tsv", "config/renames/proposals/m-sysmenu-apply.tsv", "config/renames/proposals/core.tsv", "wiki/spec/core.md", "wiki/emulator.md", "wiki/obin.md"]
---

# Main: the player's room screens (system menus, clubs, rumors, holiday preparation), port spec

Phase 2 spec for [[tickets/T-27017-p2-m-sysmenu]] ([[tickets/T-27000-phase-2]]). The unit label says "system menus, clubs, rumors, holiday prep"; the file `src/main/8005A0B0.c` holds more than that: it is every screen that the game draws over the player's room picture. Facts are as the matched C implements them. Names are the proposals in `config/renames/proposals/m-sysmenu.tsv`; core's names (`config/renames/proposals/core.tsv`, [[spec/core]]) and the HUD, weekday, holiday and sound proposals are used where they exist. A claim about game meaning that the code does not prove is marked HYPOTHESIS. No game text is quoted: strings are described in words.

The predecessor proposal has 263 rows (92 `func`, 27 `data`, 126 `const`, 16 non-executable `comment`, 2 unsupported `file` rows); 9 rows carry HYPOTHESIS. The historical executable batch `config/renames/proposals/m-sysmenu-apply.tsv` contained 98 rows (93 function, 5 data), including the main handler `schedule_command_screen_handler` (recorded in the core ledger as `func_8005A0B0 -> mode_command_screen -> schedule_command_screen_handler`). Five clearly owned room/click-speed data rows were included; at that batch's point, 22 data rows remained deferred. The click-mode, confirm-button and cursor-speed value families were subsequently applied under T-27062 and are declared in `include/main/settings.h`; the applied enum/define ledger is `config/renames/main-constants.txt`. For current data and constant status, later applied ledgers and the owner closure are authoritative; do not read the historical batch disposition as a current deferral. The 16 kept O.BIN functions have explanatory rows in the predecessor proposal, not executable rename rows. All 109 function definitions in the original object are accounted for by these 93 candidates plus the 16 kept O.BIN names. The Docker dry run recorded for that batch exited 0: 98 rows, 11 affected files, no PROBLEM.

Evidence used for the candidate batch: matched C bodies and callers in `src/` and `asm/`, the current rename ledgers, core's field map, the English-patch name `system_settings_init`, and the O.BIN developer table (unapplied names are evidence of role/order, not proposed names). Emulator statements below are inherited from the earlier spec and were not rerun or treated as fresh evidence in this reconciliation. The mascot click spots, the club joining stage (dead on this screen) and the system-file branch of the data screen were not run.

## 1. What the file is

One original object. Every function draws or steps a screen over the same background, the player's room at night (the picture is chosen by season, `room_bg_sectors_weekday[season_index]`), under a window title that names the home. The file holds eight scenes:

| scene | game mode and stage | entry function | section |
|---|---|---|---|
| weekly command screen | mode 0x34 (`GAME_MODE_COMMAND_SCREEN`), stages 0, 1 and 0xFF | `settings_command_screen_mode_handler` (current source name `schedule_command_screen_handler`) | 3 |
| calendar check | mode 0x34 stage 2; mode 0x46 stage 0x30 | `settings_command_screen_mode_stage_calendar` | 4 |
| data screen (memory card save and load) | mode 0x34 stage 3 | `settings_save_load_mode_stage_dispatch` | 5 |
| system settings and mascot | mode 0x34 stage 4; mode 0x46 stage 0x40 | `settings_mode_stage_dispatch` | 6 |
| club joining | mode 0x34 stage 5 (unreachable); mode 0x46 stage 7 | `join_club` (O.BIN) | 7 |
| rumor report | mode 0x44 (`GAME_MODE_RUMOR`) | `uwasa_main` (O.BIN) | 8 |
| Christmas preparation | mode 0x42 | `settings_christmas_prep_mode_handler` | 9 |
| New Year preparation | mode 0x41 | `settings_new_year_prep_mode_handler` | 9 |

Call chain per frame: `game_mode_dispatch` switches on `GameState.game_mode` and calls the mode handler; the handler switches on `mode_stage`, the stage function switches on `mode_step` (and some steps switch on `substep`), then the handler draws the shared panels (parameters, calendar date, message window, window title, cursor, background, screen frame). The mode names are in the core enum; this unit’s local stage and step families are listed in section 16.

The executable batch uses only the assigned `settings_` and `rumor_` prefixes. Command-screen, room, save/load and annual-preparation helpers carry the owning settings_ prefix, including the two mode handlers; no new prefix assignment is required. Kept O.BIN families (`join_club*`, `uwasa*`, `pre_xmas*`, `pre_syogatu*`) retain their developer names. File and header renames are out of this batch.

## 2. Shared pieces of the room scene

- `settings_room_bg_load_start` (`func_8005AB4C`): `dec_bg_cd_read(room_bg_sectors_weekday[season_index], slot 0)`. Result 0 or 1 means the picture is already decoded: show it, set the brightness to 0x80 and `mode_stage_next`. Result -1 means a CD read started: `mode_step_next`. `settings_room_bg_load_finish` (`func_8005ABD0`) polls `cd_read_sync_status`; when done it shows the decoded slot; if the decode result is -1 it restarts the read, else sets brightness 0x80 and `mode_stage_set(1)`. The rumor scene has an inlined copy (`rumor_scene_init`, `rumor_bg_wait`).
- The sick marker (`settings_room_sick_icon_load`, `settings_room_sick_icon_show`): when the player's health is sick (`player_flags` bits 8-11 = 3) sprite quad 0 is set up as a 30 x 24 sprite at (-12, -10) from the sick image in VRAM (0x3F0, 0x160) with palette row 0xF, ordering layer 0xA. The school-day setup (800737A0) calls the same two functions.
- `icon_layout_stash(restore)` (`func_8005C4CC`, named `settings_save_load_layout_stash` in the table): argument 0 copies the live HUD layout into `GameState.scratch_words`: the 0x40 bytes of `icon_positions` to words 0-15, and five words at word index 32-36: mascot anchor x, anchor y, `hud_mascot_choice`, mascot string length, swing. Any nonzero argument copies them back (callers use 1). Because `scratch_words` lies inside GameState, the layout is saved with the game. The overlay loader (`func_80079800`) and TACO (`func_80142AF0`) call it too (they reuse the HUD memory). Core's statement "first 0x50 bytes" is the word-0-to-15 part plus padding; the five words sit at +0x80..+0x93.
- Click spots on the room picture (pointer = `cursor_x`, `cursor_y`; a click is `pad_pressed & 0x20`, and every function returns 1 only when it fired).

| function | box (screen coordinates from the centre) | action |
|---|---|---|
| `settings_room_click_window` | x -13..33, y -11..7 | fade the BGM fast (sound command 0xB1), `bgm_stop`, voice clip (0x1D000185, 0x17000397) speaker 0, then voice play (0x300). HYPOTHESIS: the object is a window (the box lies over the curtains in the picture; the developer name `hitsuji_check` points elsewhere) |
| `settings_room_click_help` | x 65..79, y -31..-17 | `game_mode_set(0x3A)`: the on-line help overlay (OLH); tried last |
| `settings_room_click_radio` | x -39..7, y 25..35 | fade the BGM slowly (0xD1), `bgm_stop`, clip pair `settings_room_radio_clips[day_count % 8]`, speaker 0xE, voice play. HYPOTHESIS: the object is a radio (developer name `radio_check`; the box lies over the stereo) |
| `settings_room_click_radio_holiday` | same box | same, table `settings_room_radio_clips_holiday`; only the holiday screen calls it |
| `settings_room_click_mascot` | integer dx -11..11 from `mascot_actor_x`, dy 1..47 from `mascot_actor_y` | needs the mascot visible (`mascot_draw_flags` bit 7) and `frame_count - settings_room_mascot_stamp >= 0xF1` (the stamp is never written, so it is 0); sets `current_girl` to `mascot_actor_frame`, plays `girl_voice_clips[girl]`, speaker = girl, voice play |
| `settings_room_click_mascot_holiday` | same | same with `settings_room_mascot_holiday_stamp` and `girl_voice_clips_by_state[girl][variant]`, variant = bit 7 of `get_g_zyotai_s(girl)`; only the holiday screen calls it |

(emu) the boxes were drawn on a screenshot of 1995-04-09: they lie over the window curtains, the stereo and the shelf corner. The window spot's developer name is `hitsuji_check` ("sheep"); nothing in the picture shows a sheep, so the table names it by place and marks that name HYPOTHESIS, as it does for the radio. In menu mode A the pointer snaps to the selected icon, so a spot can only be hit with a mouse or in mouse-like mode (menu mode 0, set through the settings screen); there the d-pad moves the pointer 3 pixels per frame at cursor speed 3. Clicks from a saved state in that mode: pointer (10, -2) calls `xa_clip_request_set(0x1D000185, 0x17000397, 0)` from `settings_room_click_window`; pointer (-20, 31) calls it with (0x07000035, 0x080000AE, 0xE) from `settings_room_click_radio`, which is entry 5 of the table on 1995-04-09 (`day_count` 5); pointer (73, -23) calls `game_mode_set(0x3A)` from `settings_room_click_help`. The mascot spots could not be run: no mascot is unlocked in the state.

## 3. The weekly command screen (mode 0x34)

Where the player picks the command for the coming week. Reached when the queue reader meets the end marker (core section 3.5), from OPTION, OLH (START leaves the help) and TEL (all three call only `game_mode_set(0x34)`, which zeroes the stage byte, so they enter stage 0), and with stage 0xFF from NAME_ENT (two places: `src/ovl/NAME_ENT/80144AD0.c:2730` and `80139740.c:2708`) and from `settings_save_load_load_finish`. It ends with `game_mode_set(0x30)` (the schedule step builds the week).

### 3.1 Stages

| stage | name | steps |
|---|---|---|
| 0 | `COMMAND_SCREEN_STAGE_SETUP` | 0 `settings_command_screen_setup_reset`, 1 `settings_command_screen_setup_wait`, 2 `settings_command_screen_build`, 3 `settings_room_bg_load_finish` (then stage 1) |
| 1 | `COMMAND_SCREEN_STAGE_SELECT` | 0 `settings_command_screen_select_frame` (the only step) |
| 2 | `COMMAND_SCREEN_STAGE_CALENDAR` | section 4 |
| 3 | `COMMAND_SCREEN_STAGE_DATA` | section 5 |
| 4 | `COMMAND_SCREEN_STAGE_SETTINGS` | section 6 |
| 5 | `COMMAND_SCREEN_STAGE_JOIN_CLUB` | section 7 (unreachable from here) |
| 0xFF | `COMMAND_SCREEN_STAGE_RELOAD` (from NAME_ENT and `settings_save_load_load_finish`) | 0 reset, 1 `settings_command_screen_reload_calendar`, 2 build, 3 background wait; the brightness rises 4 per frame to 0x80 from step 2 on; with the room picture cached the build runs a second time in stage 0 (section 5.4) |

`settings_command_screen_mode_handler` also calls `hud_mascot_update` when the mascot is visible and ends with `hud_screen_frame_draw(0)`. Stages 2-5 return to the screen with `mode_stage_set(0)` + `mode_step_set(2)`: the build step runs again, the reset and the one-frame wait do not.

Reset step (`settings_command_screen_setup_reset`): 2D and 3D drawing on, actors cleared, tile layer off, `boot_hold` = 0, brightness 0, text system reset, `k_disp_start(2)`, next step.

Reload step (stage 0xFF step 1, after NAME_ENT or the data screen put a game state in place): substep 0 waits until the sound bank is loaded and starts the BGM; substep 1 reads 0x95 sectors from LBA 0x9B94 to 0x801A0000 (the month data blob, the same read as `GAME_MODE_MONTH_START` step 0); substep 2 when the read is done runs `month_calendar_load(year, month, 0)`, masks `menu_mode` to two bits, sets `season_palette_fade` to 0xFE and restores the mascot (`mascot_set(hud_mascot_choice)`, `mascot_disp_switch(1)`) when `hud_mascot_choice` < 14.

### 3.2 The build step: what the player may choose

`settings_command_screen_build` initialises the panels (parameters twice, so the second pass draws the digits without highlight; calendar date; message window), puts the player's name on the speaker label, then takes the first matching row of this list. `marker(d)` is `month_calendar[0].marker[d]` of the current month (the lookahead does not read the next month: at the end of a month the index runs into the next record's day kinds, which never match). "Tomorrow" is `day + 1`.

| # | condition | usable icons | start cursor | `icon_count` | monologue topic |
|---|---|---|---|---|---|
| 1 | `marker(tomorrow) == 5` (MARKER_EXAM) | 14 only | 14 | 15 | the exams |
| 2 | `marker(tomorrow) == 8` (MARKER_CLUB_SEASON) and the player has a club | 4 only | 4 | 14 | baseball club, year 97 and baseball club level (`clubs[6].level & 0xF`) at least 8: the national tournament; every other case: the summer camp |
| 3 | `marker(day + 6) == 12` (MARKER_CULTURE_FESTIVAL) and club id < 6 | not sick: 4 only (cursor 4, topic: club work for the festival); sick: 7 only (cursor 7, sick marker shown, topic: the festival preparation and the sick body) | 4 or 7 | 14 | as listed |
| 4 | `marker(tomorrow) == 11` (MARKER_SCHOOL_TRIP) | 14 only | 14 | 15 | the school trip |
| 5 | `remedial_week` flag (bit 0 of the halfword at +0xF6) | 0, 1, 2 | 0 | 14 | the remedial week |
| 6 | health sick | 7 only (sick marker shown) | 7 | 14 | the sick body |
| 7 | otherwise | every command 0-7 (4 only with a club), no 14 | low nibble of `last_command` | 14 | the plan for tomorrow |

Every row first calls `icon_bar_init` and `icon_layout_apply(icon_count)` (`icon_count` 15 shows the extra icon 14 even in layouts A and B), then `settings_command_screen_icons_reset(lock_commands)`: icons 10, 8 and 9 are always off (they are holiday-only), icon 14 is off in the free layout, icon 4 is off for a player without a club, and a nonzero argument turns off the commands 0-7. Rows 1-6 pass 1 and then enable their icons; row 7 passes 0. After the list: the monologue starts (`k_disp_start(4)`; its text is not reproduced here), `cursor_actor_init(0)`, icon 13 off on 1995-04-04 (the first day cannot save), the menu bar colour, and `settings_room_bg_load_start`.

Note the row order: exams (1) and club season (2) win over the festival check (3) and the school trip (4); the remedial week (5) is only reached when none of these fires.

### 3.3 The selection stage

`settings_command_screen_mode_stage_select` runs every frame. Step 0 calls `settings_command_screen_select_frame` (`k_disp_inc`, panels, `hud_icon_bar_update(0)`, `cursor_update`, title, background draw, then the click spots: first `settings_command_screen_icon_confirm`, then mascot, radio and window; if none fired the help spot). It then shows the sick marker if needed and watches the background card job (section 5.5): result 0x13 = done (`UI_SE_CARD_JOB_DONE` 0x50F, job cleared, `memory_card_stop`); timer above 0x200 or result 0xF0 = failed (`UI_SE_ACTIVITY_POOR` 0x502, job cleared); with no job the card icon animation is switched off (`icon_card_anim_set(0)`).

`settings_command_screen_icon_confirm` (circle on `menu_cursor_0`, value >= 0). Ignored while L2 moves icons in the free layout. A press first stops any voice (`cd_command(9, 0, 0)`, `audio_command(SND_CMD_XA_FORGET)`) and restarts the BGM when nothing plays. Then by icon (`get_icon_can_use` must be 1 where stated):

| icon | name (HUD, holiday units) | effect |
|---|---|---|
| 0-3, 5-7 | weekday commands | `last_command` low nibble := icon, sound 0x500 (`UI_SE_COMMAND_CONFIRM`), `game_mode_set(0x30)` |
| 4 | club activity | with a club: the first girl 0-10 whose club field (`GirlFlagBits.f20`) equals the player's club (`func_80053564`, `girl_find_by_club` in the menutext table) gains 1 affection when her `GirlFlagBits.b2` is set (the scheduler table names that bit `acquainted` and marks its meaning HYPOTHESIS; this unit claims no meaning for it; emu: club 6, a girl with club field 6 and the flag set: affection 0x10 becomes 0x11, with the flag clear it stays 0x10); then as above. Without a club: nibble := 4 and stage 5 (unreachable, see section 7) |
| 8, 9, 10 | holiday-only icons | the code would play 0x50D (`UI_SE_MENU_SELECT`, icons 8 and 10) or 0x500 (icon 9) and schedule, but the icons are always locked here |
| 11 | system settings (`HUD_ICON_SYSTEM`) | sound 0x50D (`UI_SE_MENU_SELECT`), stage 4 (no availability test) |
| 12 | calendar (`HUD_ICON_CALENDAR`) | sound 0x50D, stage 2 (no availability test) |
| 13 | data (`HUD_ICON_CARD`) | when usable and `card_job` is 0: `icon_layout_stash(0)`, sound 0x50D, stage 3 |
| 14 | extra "proceed" (`HUD_ICON_EXTRA`) | when usable: nibble := 0xE; if tomorrow's marker is 5 (exam) or 11 (trip) sound 0x500, `game_mode_set(0x30)` and the matching notice; any other day: `game_mode_set(0x30)` and nibble := 7 (rest) (emu: in the exam week the confirm gives nibble 0xE) |

(emu) circle on icon 0 of the 1995-04-09 state calls `game_mode_set(0x30)` from `settings_command_screen_icon_confirm` (frame 30); icon 11 enters stage 4 at frame 46, icon 12 stage 2, icon 13 stage 3; circle on icon 4 (no club) does nothing. With menu mode A the d-pad steps the icons in index order: DOWN and RIGHT go to the next, UP and LEFT to the previous, with wrap-around over 0-13 (UP from 0 reaches 13).

(emu) rows 1-6 were run on patched states: tomorrow's marker 5 leaves the cursor on icon 14 with `icon_count` 15; marker 8 with a club, and marker 12 six days on with club 0, leave it on icon 4 with count 14; marker 11 gives icon 14 and count 15; the remedial flag gives icon 0; sick gives icon 7. Screenshots of the exam and sick cases show the monologue topics of the table and only the allowed icons lit.

A chosen weekday command ends the screen; its effects (parameter changes, the week queue) belong to the scheduler and weekday units. The only game-state change made here is the club girl's +1 affection and `last_command`.

## 4. The calendar check (stage 2)

A two-page notebook picture: this month on the left, next month on the right, weekday columns, day numbers coloured by day kind, event marks and three coloured wave curves (the 33, 28 and 23 day cycles of the HUD spec) over the pages; a caption says it shows the plans and results of this and next month. `settings_command_screen_calendar_init` sets the title and caption, `calendar_screen_init`, hides the icons; `settings_command_screen_calendar_wait` only reads cross (pad 0x40), circle does nothing; `settings_command_screen_calendar_exit` goes to stage 0 step 2. The page content is drawn by `calendar_view_update` (HUD unit). The holiday handler runs the same stage as its stage 0x30. (emu) icon 12 at frame 45: stage 2 step 0 at frame 46, wait until cross at 150, exit at 151.

## 5. The data screen: save and load on the memory card (stage 3)

The player's save game dialog. The background is the notebook picture (`cal_base_show`). Slots: 14 game files, numbered 1-14; slot 0 is the system file. The labels 1-14 come from `month_label_strings` (HUD name; a table of full-width numerals).

### 5.1 Memory-card interface used here

The card is handled asynchronously by the memory-card module (`src/main/memory_card.c`; not this unit). Current names below follow the applied `config/renames/remaining-symbols.txt` ledger and definitions in `src/main/memory_card.c` / `include/main/memory_card.h`; the earlier `p2a-m-flow` table is historical. `memory_card_start` initializes busy/step/result/retry/timer state, and `memory_card_stop` clears busy/timer state. Per-frame polls increment `card_timer`: `memory_card_check_poll` (`func_80053F04`) detects the card; `memory_card_scan_poll` (`func_80054140`) checks the system file and save slots; `memory_card_system_read_poll` (`func_80054284`) returns 3; `memory_card_save_read_poll` (`func_8005448C`) returns 4; `memory_card_save_write_poll` (`func_80054590`) returns 0x14; and `memory_card_system_write_poll` (`func_80054388`) returns 0x13. Results share `card_result`. Other current names include `memory_card_file_create` / `memory_card_file_delete` (`func_80054694` / `func_80054704`), `memory_card_bank_validate` (`func_80055A38`), `memory_card_game_bank_load` (`func_80055AFC`), `memory_card_system_bank_load` (`func_800552DC`), `memory_card_slot_summary_store` (`func_80054E6C`), and `memory_card_format` (`func_80054108`, formats `bu00:`). A card job times out at 0x201 frames or when `card_result` is 0xF0.

`D_800E8BEE` is set when the card's slot 0 file opens (the system file exists), by the scan, and cleared by the card reset (`memory_card_state_reset`, `func_80056284`; `memory_card_save_files_probe`, `func_80054BE4`), by ETC, and by the scan when three tries fail. HUD calls it `slot_info_from_system_data`. With it set, the slot summaries come from the SystemData arrays; without it each slot must be read from the card to show its information.

The slot summary (`memory_card_slot_summary_store`, `func_80054E6C`, run after a save for slot `selected_slot & 0xF`): year, month, day, family name, given name, nickname, the nine parameter words, 16 bytes of girl record 15's event counters and the club nibble, stored in SystemData arrays that are written to the system file. The 16 bytes are filled by the TEL overlay's girl evaluation page (0 for a girl who is not met, else her star rating + 1); the slot panel counts them as the "cleared" figure (HUD spec section 17); whether a nonzero value really means a cleared girl is not shown by the code.

### 5.2 Steps

| step | function | what happens | next |
|---|---|---|---|
| 0 | `settings_save_load_menu_init` | title, two choices (save at x -80, load at x 16, y -16, 48 x 20), the checking text and four dotted lines, a card reset (`memory_card_state_reset`, `func_80056284`: `memory_card_stop`, slot 0's status and `D_800E8BEE` cleared, `memory_card_start`), menu 1 with 2 items, cursor 0 | 1 |
| 1 | `settings_save_load_card_check` | substep 0: wait for card detection: result 1 -> substep 1; 0xF1 (unformatted) -> step 0xA0; 0xFF -> step 0xFF; timeout -> 0xFF. 1: `memory_card_start`. 2: wait for the scan (2); timeout -> 0xFF. 3: if the system file exists restart the job, else show the choice text, enable both choices and go to step 2. 4: wait for the system-file read (3): apply it, `icon_layout_stash(1)`, show the choice text, step 2; timeout or 0xF0 -> 0xFF | 2 |
| 2 | `settings_save_load_menu` | cross: `icon_layout_stash(1)`, back to the command screen (stage 0 step 2). Circle: item 0 -> step 3, item 1 -> step 6 | 3 or 6 |
| 3 | `settings_save_load_save_init` | 14 slot labels in 3 columns of 5 (box 32 x 16, x = 32 * (i / 5) - 124, y = 16 * (i % 5) - 64: the left page), menu 1 with 14 items; picks the first slot with status 1; with the system file: cursor = `selected_slot` - 1, summary of that slot, step 4; with no used slot: step 4; else step 0xC0 for that slot | 4 or 0xC0 |
| 0xC0 | `settings_save_load_save_info_read` | `memory_card_start`, read the slot (returns 4), validate; ok: show its summary, step 4; read or validation failure: slot_status := 0xFF, step 0xFF; timeout or result 0xD0 -> 0xFF | 4 |
| 4 | `settings_save_load_save_select` | cross: back to the command screen and `card_job` := 0. Circle on a slot: empty (status 0): test that the file can be created (create then delete; failure -> step 0xF0), status := 0xFF, `memory_card_start`, step 5; the shown slot: `memory_card_start`, step 5; another slot: with the system file show its summary and move the cursor, else step 0xC0 | 5 |
| 5 | `settings_save_load_save_write` | substep 0 asks whether to overwrite (yes/no with button glyphs; asked even for an empty slot); 1 circle -> 2, cross -> step 3 and a pending status 0xFF goes back to 0; 2 saving status message (with the system file it adds a power warning); 3 wait for the write (returns 0x14): status := 1; on timeout or 0xF0 the girl `current_girl % 13` speaks `girl_voice_clips_save_failed` and the step becomes 0xFF; 4 with the system file: `memory_card_start`, `card_job` := 1, `selected_slot` := slot, slot := 0, `memory_card_slot_summary_store`; always `icon_layout_stash(1)` and stage 0 step 2 | command screen |
| 6, 7 | `settings_save_load_load_init`, `settings_save_load_load_select` | as steps 3 and 4 with the labels on the right page (x = 32 * (i / 5)); in step 7 circle on an empty slot (status 0) does nothing; cross returns | 8 or 0xB0 |
| 0xB0 | `settings_save_load_load_info_read` | as 0xC0 for the load side | 7 |
| 8 | `settings_save_load_load_read` | substep 0 loading status message; 1 wait for the slot read (4), timeouts -> 0xFF; 2 validate: ok -> 3, else step 0xE0; 3 show the summary and ask whether to continue with this data; 4 circle (sound 0x501, `UI_SE_ACTIVITY_GOOD`) -> 5, cross -> step 6; 5 apply (`memory_card_game_bank_load`, `func_80055AFC`): ok -> 9, else 0xE0 | 9 |
| 9 | `settings_save_load_load_finish` | see 5.3 | |
| 0xA0 | `settings_save_load_format_prompt` | message that the card is unformatted, then the format question (yes/no); circle formats: success -> step 0, failure -> command screen; cross -> asks for another card in slot 1 and returns to the command screen | 0 |
| 0xE0, 0xF0, 0xFF | `settings_save_load_data_broken`, `settings_save_load_card_full`, `settings_save_load_card_unreadable` | sound 0x502 (`UI_SE_ACTIVITY_POOR`, the cue the holiday unit names for a poor result), a message, and a START, circle or cross press: 0xE0 -> step 6; 0xF0 and 0xFF -> `icon_layout_stash(1)` and the command screen | |

The slot marks (`settings_save_load_save_marks`, `settings_save_load_load_marks`) draw, every frame, a 30 x 15 semi-transparent box over each of the 14 labels: a grey tint for status 0 (empty) and a second tint for 0xFF. `slot_status` (SystemData +4, 16 bytes, label `save_slot_status`): 0 empty, 1 holds a save, 0xFF file created for a save that has not finished or a slot whose read failed. The prompt lines of the two lists have their own texts (`settings_save_load_save_prompt_text`, `settings_save_load_load_prompt_text`). ETC (end-of-game system file save) calls `settings_save_load_save_prompt` (`func_8005C7C0`) and `settings_save_load_save_marks` (`func_8005C7FC`), but has its own clone of the init, `func_80144AC0` in `src/ovl/ETC/80143240.c:620` (commented as a port of `func_8005C5BC` with other state codes); nothing in that overlay calls `settings_save_load_save_init`.

### 5.3 After a load (`settings_save_load_load_finish`)

The loaded GameState replaces the live one first. Then:

- If the loaded date is 1998-03-01 or 1995-04-04: BGM fade (sound commands 5 and 0xB1), a sound bank load, `boot_game_data_init` (`func_80041878`: resets calendar, player, girls, queues and event log to the new-game values), `boot_event_state_init` (`func_80041F48`: clears appointments, clubs and queues), the HUD hidden, and `game_mode_set(0x14)` (NAME_ENT). NAME_ENT's own load path (title screen) does the same for 1995-04-04. HYPOTHESIS: a save made on the first day or the final day (clear data) starts a new run from the beginning.
- Any other date: sound 0xB1, a wait of 63 frames (substep 0xC0 counts up to 0xFF), the window title shown, `game_mode_set(0x34)`, `mode_stage_set(0xFF)` and `day_bgm_load`: the command screen is rebuilt with stage 0xFF.

### 5.4 What the player sees (emu)

Run from the command screen state against the empty virtual card (`build/emu/memcard1.mcd`):

- Icon 13 at frame 45: `icon_layout_stash(0)` once, stage 3 at 46, `settings_save_load_menu_init` once. The screen is a blank two-page notebook with the checking line and animated dots. The check ends at about frame 483 (the BIOS file open of the emulator takes about 34 frames per call, so the slot scan is slow; the real console is much faster): detection ok, no system file, then the two choices (save on the left page, load on the right) with a one-line instruction.
- Save: circle on save: `settings_save_load_save_init` and the prompt at once (no used slot), 14 numbered slots in 3 columns on the left page, all dimmed, the cursor on slot 1. Circle on slot 1: the create test passes, the slot gets the second tint and the overwrite question with the yes and no lines appears (frame 115). Circle at 250: the write ends before frame 391, then `icon_layout_stash(1)` and the command screen is rebuilt from stage 0 step 2 (frame 392); the slot is now status 1.
- Load (card with that save): circle on load: the scan found slot 1 used; its summary is read (about 130 frames) and `settings_save_load_load_select` starts at frame 165 with the right-page list (slot 1 undimmed). The left page shows the slot number, the date, the player's name, the nickname, the club and a count of clears. Circle on slot 1 starts `settings_save_load_load_read`, which reads, validates and asks whether to play on with this data (yes and no lines). Circle at 700: apply, `settings_save_load_load_finish` for 65 frames, then stage 0xFF: reset (frame 767), the month data reload (76 frames), the build step (844). The saved date 1995-04-09 is an ordinary date, so the special branch was not taken.
- Unformatted card (card image of zeros): detection returns 0xF1 at frame 54 and `settings_save_load_format_prompt` runs from frame 55: first the message, after a press the format question with the yes and no lines; circle at 250 formats the card, the check restarts from step 0 and the choice screen appears about 480 frames later.
- Stage 0xFF quirk: the room picture was already decoded, so `settings_room_bg_load_start` called `mode_stage_next` from stage 0xFF, and the stage byte wrapped to 0: the setup ran a second time (reset, wait, build; `settings_command_screen_build` ran twice). When the picture has to be read, step 3 of stage 0xFF ends with `mode_stage_set(1)` and there is no second pass.

### 5.5 The background system-file write

After a successful save with the system file present, `card_job` is 1 and the command screen's stage 1 keeps polling `memory_card_system_write_poll` (`func_80054388`): while it runs the card icon animates (`icon_card_anim_set`), and when it finishes the job ends with `UI_SE_CARD_JOB_DONE` (0x50F), or with `UI_SE_ACTIVITY_POOR` (0x502) on a timeout. Icon 13 does nothing while `card_job` is set. A port can turn the whole directory into a plain list of files and drop the job.

## 6. System settings and mascot (stage 4)

`settings_mode_stage_dispatch` dispatches on the step; every screen draws the root labels first (`settings_labels_draw(item)`, item highlighted) and then its own list. Common pattern per setting: an `_init` step builds the choices and sets the start cursor from the stored value; a `_menu` step handles cross (back to step 0) and circle (store the value, show a confirmation line, next step); a `_done` step waits for circle and returns to step 0 with `menu_cursor_0` set to the setting's root item. Step 0 (`settings_root_init`) builds the root list, so the root cursor is the last item used. Entering from the command screen with icon 11 leaves `menu_cursor_0` = 11, and `settings_root_init` reduces it modulo 8, so the root list starts on item 3 (emu).

Root list (menu 0, 8 boxes of 96 x 12 at x -124, y = -64 + 12 * i), `settings_root_menu` (step 1): cross leaves (`mode_stage_set(0)`, `mode_step_set(2)`); circle by item:

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
| mascot | 0x70-0x72 | `hud_mascot_choice` (through `mascot_set`) | girls 0-12, item 13 = hide | the stored value itself (see below) |

Details:

- Click speed (`settings_mouse_click_speed_menu`) acts only while the mouse button chosen as confirm button is held (`mouse_buttons` bit 4 when `confirm_button` is 1, bit 8 when 0). "Change": the first frame with the button held sets `settings_click_speed_ticks` to 0xFF; the next frame sets `double_click_interval` := 0xFF - ticks, clamped to 0x10..0x40, clears the counter and shows the confirmation. Nothing else changes the counter (bss word used only here, ETC clears it), so the result is always 0x10. HYPOTHESIS: the developer build counted frames between two clicks in this word and the counting code was lost.
- Cursor speed: the label set is the extended one (very slow to very fast) when `SystemData.flags[8] >= 9` or `girl_cleared[8] >= 2`, else slow, a little slow, normal, a little fast, fast. The confirmation text has a second wording when `flags[8]` or `girl_cleared[8]` is nonzero. Both are tests of girl 8's entries; the thresholds differ (2 and 9 against 0). After a change `input_device` is set to 0xFF (`INPUT_NONE`); the next controller poll (`func_80042AC8`) sets it again, sees the change, re-initialises the pad (`InitPAD`, `StartPAD`) and zeroes the pointer position and the pad words, so a new speed starts from a clean input state.
- Mascot (HYPOTHESIS on the semantics of the counters, which core names `girl_cleared`): girl i (0-12) appears in the list only when `SystemData.flags[i]` or `girl_cleared[i]` is nonzero. ETC's end-of-game save adds 1 to both `girl_cleared[current_girl]` and `SystemData.flags[current_girl]` (cap 99), so both arrays behave like clear counts per girl (the first per game, the second in the system file). Item 13 (`MASCOT_MENU_ITEM_HIDE`, a list index in `MascotMenuItem`; the stored `hud_mascot_choice` is a different domain, 0 = `MASCOT_NONE` and girl + 1) hides the mascot. The list has 14 boxes in 3 columns (x = 43 * (i % 3) - 8, y = 16 * (i / 3) - 64, 40 x 16; item 13 is wider). Each frame `settings_mascot_menu` calls `mascot_set(0)` and then `mascot_set(cursor + 1)` when that girl is unlocked, a live preview; cross and circle on item 13 leave the preview as it is; circle on an unlocked girl confirms and goes to step 0x72. Quirk: `hud_mascot_choice` is girl + 1 (0 = none) but the start cursor is set to the choice itself, so the highlight (and the first preview) starts one item after the current mascot's girl. (emu, patched unlock counts for girls 3 and 5): the right page lists only those two girls and the hide item; moving to girl 3 sets `hud_mascot_choice` 4 at once; confirming keeps it; entering the menu again starts on item 4, which is locked, and the first frame resets `hud_mascot_choice` to 0.
- `settings_unlocked_mascot_count` adds 13 tests of `SystemData.flags[0..12]` and 13 tests of `girl_cleared[0..12]` and returns the count of nonzero entries.
- Settings screens set no change flag; the GameState settings are saved with each game; text speed lives in TextState and is saved separately by the memory-card path. The title menu's OPTION overlay edits the same fields.

(emu) stored values checked in memory: icon mode item 1 gives `icon_mode` 1 (`icon_count` 14); cursor speed item 4 gives `cursor_speed` 6; voice mode item 1 gives `voice_mode` 0 (the default in the state was 2); menu mode item 0 gives `menu_mode` 0 and the pointer then moves with the d-pad, 3 pixels per frame. (emu) icon 11 at frame 45: `settings_root_init` once (frame 46) with `settings_unlocked_mascot_count` called twice; `settings_root_menu` per frame; text speed init at frame 116, menu from 117, done from 161; DOWN on the root list skipped the hidden item 6 (3, 4, 5, 7) and circle on item 7 returned to the command screen build.

## 7. Club joining (stage 5, O.BIN `join_club*`)

`join_club` steps: 0 `join_club_init` (hide the icons, clear the text, `club_list_menu`: the 11 clubs in 3 columns of 5, 5 and 1 (x = 84 * (i / 5) - 128, y = 16 * (i % 5) - 56) plus a cancel item at (40, 8); a club whose `ClubRecord.flags` bit 0 (`ClubFlagBits.quit`, named by the scheduler and HUD tables) is set is hidden; this unit only reads it, `holiday_club_select` sets it and `club_flag0_clear` clears it), 1 `join_club_select`, 2 `join_club_message`, 3 `join_club_exit` (`actor_clear_upper`, `game_mode_set(0x30)`).

- `join_club_select`: cross returns (`mode_stage_set(0)`, and in mode 0x34 also `mode_step_set(2)`); circle on the cancel item (11) does the same; circle on a club stores its id in `player_flags` bits 12-15 and goes to step 2.
- `join_club_message` substeps: 0 announces the joined club (text built from `club_name_table[club]`); 1 waits for text and voice; 2 for clubs 6 and up (baseball, soccer, tennis, swimming, basketball) the club captain gives a reminder about the monthly club day, otherwise it skips to 0xFF; 3 `talk_text_step`; 4 and 0xFF finish.
- Reachability: from this screen only icon 4 starts stage 5, and `settings_command_screen_icons_reset` locks icon 4 for a player without a club, so the stage is dead code in mode 0x34 (emu: circle on icon 4 in the no-club state does nothing). Run with the stage byte patched to 5 on that state: the club list shows the 11 clubs in the three columns and a cancel item at the lower right, the cursor starts on the first club; DOWN six times and circle selects baseball (club 6); the announcement and then the captain's reminder follow (each waits for a press), `player_flags` byte +0xF5 becomes 0x60 and `join_club_exit` calls `game_mode_set(0x30)`. The holiday mode runs the same three functions as its stage 7 (Sunday, no club), where the join happens in play.

## 8. The rumor report (mode 0x44)

Triggered by the queue reader: at a day-end record (0xFB) every met girl 0-10 with `syoushin >= 0x3D` whose `rumor_done` flag is clear gets the flag set; if at least one did, `game_mode_set(0x44)` (core section 3.5). The screen carries no data about the girls and changes no state:

- Stage 0: `rumor_setup_mode_stage_dispatch` step 0 `rumor_scene_init` (text reset, home title, the player's name label, panels, the icon bar shown and locked to the day record's command by `command_icons_lock_to_record`, then icon 0 off and the `last_command` icon on, a monologue about the rumor, room background read; the handler then keeps drawing the bar with `hud_icon_bar_update(1)`), step 1 `rumor_bg_wait`.
- Stage 1: `uwasa0` step 0 `rumor_wait_voice` (typewriter plus voice, `xa_wait`), step 1 `uwasa_exit0` = `game_mode_set(0x31)`, the day screen.

The effect of the rumor, if any, is not in this file. (emu, mode byte patched to 0x44) `rumor_scene_init` runs at frame 1, the picture wait takes 18 frames, the monologue types out and waits; a confirm press after the text (about frame 250) ends it with `game_mode_set(0x31)` from `uwasa_exit0`.

## 9. Christmas and New Year preparation (modes 0x42 and 0x41)

Queue types 0x28 and 0x2C (markers 13 and 15) start them (core table). Both lead into the SHOUGATU overlay.

- Christmas (`settings_christmas_prep_mode_handler`, proposed): stage 0 `pre_xmas_init` step 0: panels, no icons, a monologue about the Christmas party at the Ijuin mansion, room background read (`settings_room_bg_load_start`); step 1 `settings_room_bg_load_finish`. Stage 1 `pre_xmas`: substep 0 `xa_wait`, substep 1 `game_mode_set(0x52)`.
- New Year (`settings_new_year_prep_mode_handler`, proposed): `settings_new_year_card_sender_count` counts the met girls 0-10 whose weighted affection (`get_h_yuukou_table`) is at least 0x4D (`NEW_YEAR_CARD_AFFECTION_MIN`). Stage 0 step 0 `settings_new_year_card_scene_start` (proposed): with a count of 0 it sets brightness 0 and enters mode 0x53 at once (no scene); otherwise it grabs the screen (`dec_bg_capture_screen(0xA0)`), clears actors, shows only the calendar date and the message window, fades the BGM (sound 0xC1) and loads a sound bank. Steps 1-3 (`pre_syogatu_init1/2/3`): wait for the bank, read the card background (sector 0x4167 into slot 1) and show it, play the doorbell (sound 0x603, `UI_SE_DOORBELL`). Stage 1 `settings_new_year_scene_mode_stage_dispatch` (proposed): step 0 `pre_syogatu0` is the knock scene with the postman's voice clip (0x08000005..0x03000009); the next background (sector 0x4144) is read. Step 1 `pre_syogatu1` builds one line that lists every qualifying girl as a card sender (names from `get_g_name`, joined with a separator) and shows it; then `game_mode_set(0x53)`. The scene gives no reward in this file. (emu) Christmas (mode byte patched to 0x42): the room without icons, the monologue, confirm after the text calls `game_mode_set(0x52)` from `pre_xmas`. New Year (0x41, girl 0's affection set to 0x80; with 0 the mode goes to 0x53 at frame 1): steps 1-3 end at frame 162, the first background is a room with New Year decoration under the title word for New Year with the postman's line, the second is the entrance door with the line that names the girls, and the last confirm calls `game_mode_set(0x53)` from `pre_syogatu1`.

## 10. State and data

GameState fields (core names), with who writes them here:

| field | role here |
|---|---|
| `mode`, `stage`, `step`, `substep`, `step_timer` | the machine; this file adds 1 to `step_timer` in two stage handlers |
| `last_command` | low nibble written by `settings_command_screen_icon_confirm`; start cursor of the build step |
| `menu_cursor_0`, `menu_cursor_1` | icon cursor, list cursor (settings root, mascot list use menu 0/1 as in the table above) |
| `pad_pressed` | 0x20 circle, 0x40 cross, 0x860 start/circle/cross for message acknowledgements |
| `player_flags`, `player_flags_hi`/`remedial_week`, `clubs[]` | club nibble (written by `join_club_select`), health, remedial week; `clubs[6].level` read by the build step |
| `icon_mode`, `icon_count`, `menu_mode`, `cursor_speed`, `click_mode`, `double_click_interval`, `confirm_button`, `voice_mode`, `input_device` | settings (section 6) |
| `card_job`, `card_result`, `card_timer`, `save_slot`, `scratch_words` | card state (section 5) |
| `screen_brightness`, `clear_color_*`, `boot_hold` | screen state set by the reset and build steps |
| `girl_cleared[]`, `SystemData.flags[]` | mascot unlock counts (section 6) |
| `SystemData.slot_status`, `.selected_slot` | the save directory (aliases `save_slot_status`, `save_selected_slot`) |

Module globals (new names): `settings_room_radio_clips`, `settings_room_radio_clips_holiday` (8 clip pairs each), `settings_room_mascot_stamp`, `settings_room_mascot_holiday_stamp` (never written), `girl_voice_clips`, `girl_voice_clips_by_state`, `girl_voice_clips_save_failed` (13 girls; shared with ETC and NAME_ENT), `choice_yes_line_text`, `choice_no_line_text` (shared with NAME_ENT), `settings_click_speed_ticks`, `mascot_actor_frame/x/y` (aliases into actor 27), `settings_room_sick_icon_image/palette`, the rodata strings (`settings_command_screen_calendar_title_text`, `settings_command_screen_calendar_caption_text`, `settings_save_load_save_prompt_text`, `settings_save_load_load_prompt_text`, `pre_syogatu_title_text`, `pre_syogatu_doorbell_text`, `settings_save_load_check_text`, `settings_save_load_dots_text`). The voice clip pairs are (start, end) positions for `xa_clip_request_set(start, end, speaker)` (h-cdio spec): the port needs to map each pair to an audio asset of the speaker's XA file.

## 11. Connections

- In: `game_mode_dispatch` cases 0x34, 0x41, 0x42, 0x44; the queue reader (types, section 8 and 9); OPTION, OLH and TEL send the player back to mode 0x34 at stage 0, NAME_ENT at stage 0xFF (two places).
- Out: mode 0x30 (command chosen), 0x3A (help spot), 0x14 (load of a first or last day save), 0x31 (after the rumor), 0x52 and 0x53 (SHOUGATU flows).
- Shared with the holiday unit: stages 0x30 and 0x40 run `settings_command_screen_mode_stage_calendar` and `settings_mode_stage_dispatch`; stage 7 runs `join_club_init/select/message`; `settings_command_screen_icons_reset`, `settings_room_click_radio_holiday`, `settings_room_click_mascot_holiday`. With the weekday unit (800737A0): `settings_room_sick_icon_load/show`, `room_bg_sectors_weekday`. With the overlay loader and TACO: `settings_save_load_layout_stash`. With ETC: `settings_save_load_save_prompt`, `settings_save_load_save_marks`, the voice clip tables (ETC has its own clone of `settings_save_load_save_init`).

### Callees that other units own

Roles shown by this file's use. The current names below follow applied ledger entries (no duplicate rows here): `func_80053F04` `memory_card_check_poll`, `func_80054140` `memory_card_scan_poll`, `func_80054284` `memory_card_system_read_poll`, `func_80054388` `memory_card_system_write_poll`, `func_8005448C` `memory_card_save_read_poll`, `func_80054590` `memory_card_save_write_poll`, `func_80054694` `memory_card_file_create`, `func_80054704` `memory_card_file_delete`, `func_80054108` `memory_card_format`, `func_80055A38` `memory_card_bank_validate`, `func_80055AFC` `memory_card_game_bank_load`, `func_800552DC` `memory_card_system_bank_load`, `func_80054E6C` `memory_card_slot_summary_store`, `func_80056284` `memory_card_state_reset`, `func_80054BE4` `memory_card_save_files_probe`, `func_80041878` `boot_game_data_init`, `func_80041F48` `boot_event_state_init`; the applied menutext name at `func_80053564` is `girl_find_by_club` (8004F870: first girl 0-10 whose club field equals the argument, -1 for none). Poll return values are represented by the shared `card_result` byte; the earlier `p2a-m-flow` proposal table is historical, not a pending merge.

## 12. Peculiarities and dead code

1. `settings_icon_layout_repair` (`func_8005A410`): no caller. It checks the stored icon positions of the free layout (outside x -0x86..0x7C or y -0x54..0x54, or two or more at (0, 0)) and resets them.
2. The weekday club-join branch (stage 5) cannot be reached; the cancel handling for mode 0x34 in `join_club_select` is the same vestige.
3. `settings_room_mascot_stamp` and `settings_room_mascot_holiday_stamp` are never written, so the mascot click cooldown (0xF0 frames) never blocks after the first four seconds of the program.
4. The click speed change always yields 0x10 (section 6).
5. The mascot menu starts one item off (section 6); merely opening it and pressing cross can change or hide the mascot, because the preview runs every frame.
6. The save screen asks the overwrite question before writing to an empty slot as well.
7. Cursor speed thresholds differ between the label set and the message wording (section 6).
8. After a load the command screen is built twice when the room picture is cached (section 5.4): the first pass runs in stage 0xFF, `mode_stage_next` wraps the stage byte to 0 and stage 0 runs again.
9. `D_800E7D11` (`system_data_flags_view`) and `D_800E7D2C` (`system_flags_8`) are byte aliases into SystemData used through an odd cast; `settings_unlocked_mascot_count` and the cursor speed screen should read `SystemData.flags[]` when the code is cleaned.

## 13. Prefix, shared-name and ownership requests

The ADR's main row for `8005A0B0` assigns `settings_`, `club_` and `rumor_`. The executable batch uses those existing owners: `settings_command_screen_*`, `settings_room_*`, `settings_icon_*`, `settings_save_load_*`, `settings_christmas_*` and `settings_new_year_*`. The current main-handler chain is `func_8005A0B0 -> mode_command_screen -> schedule_command_screen_handler`; its proposed final name is `settings_command_screen_mode_handler`. The new names name the owning settings module and do not claim overlay prefix ownership. O.BIN families remain unchanged. Root owns shared-source application.

Five owned data rows are now included: the two radio clip tables (hypothesis names), two mascot frame-reference stamps, and the click-speed word. All other data remains deferred. Shared-name requests: `girl_voice_clips*` are used by this file, ETC and NAME_ENT; `choice_yes_line_text`/`choice_no_line_text` are also used by NAME_ENT; `mascot_actor_*` are views into the HUD actor pool; `room_bg_sectors_*` are read across HUD, school-day and holiday code. The alias labels `system_data_flags_view`, `save_slot_status`, `system_flags_8`, and `save_selected_slot` overlap the shared `SystemData` aggregate and need coordination with the core/memory-card owners. Resolve owners before adding those rows. `settings_click_speed_ticks` is cleared by ETC as well as read here. The English-patch name `system_settings_init` is included as `settings_root_init` because the settings prefix is already assigned to this module.

## 14. Open questions

1. Policy: the table quotes the unapplied O.BIN names (`system_config_*`, `data_save_*`, `uwasa_set`, `pre_xmas_main`, `hitsuji_check`, ...) as evidence and gives English names. If the user prefers the developer names for chains whose neighbours are kept (`uwasa_*`, `pre_xmas*`, `pre_syogatu*`), seven rows change; the dev name is in each row's note.
1a. Prefix ownership for the function batch is resolved with settings_ for annual preparations; no new christmas_/new_year_ main prefix is introduced.
1b. Resolve one owner for `room_` and `room_bg_sectors_*` across the main module, HUD, school-day and holiday users.
2. `D_800E8BEE`: HUD proposes `slot_info_from_system_data`; the code here shows it is set from "slot 0 file can be opened" (the system file exists). The owner of 80053650 should settle the name.
3. Applied game-mode and scheduler names win over all historical proposal references. No shared core rename is included except the physically owned mode-0x34 handler.
4. `girl_cleared` semantics: clear counts (ETC's end-of-game save increments both arrays, cap 99) rather than flags; core marks it HYPOTHESIS. The cursor speed unlock on girl 8 is probably a hidden-extra gate; no name for it.
5. The room spots: what each one does is confirmed in the emulator (section 2); which object a spot stands for (`radio`, `window`) comes only from the developer names and the picture, and both names are marked HYPOTHESIS in the table. The two mascot spots were not run.
6. Mode 0x14: this unit only shows that NAME_ENT's new-run entry is used for clear data and first-day saves; its role is for the NAME_ENT unit.
7. `scratch_words` layout: core says the layout occupies the first 0x50 bytes; the code shows 0x40 bytes at +0 and five words at +0x80..+0x93.
8. Whether the "cleared" count of the slot summary (nonzero entries among the 16 bytes of girl record 15, filled by TEL's evaluation page) means cleared girls; the value 1 is the case "points >= 500" in TEL, which looks like a completion state.
9. Where the rumor's effect on the girls is applied, if anywhere.
10. Prefix owners of shared tables: the three voice clip tables are used by this file, ETC and NAME_ENT; their prefix (`girl_`) belongs to another row of the ADR map, so the memory-card or dialogue owner should confirm or rename them.

T-27050 review correction: mascot hit boxes use strict inequalities (integer dx -11..11, dy 1..47), and the cooldown difference is compared as u32. Historical emulator references above were not rerun. Hypothesis names for the window/radio spots retain an initial HYPOTHESIS: marker in the executable table.

T-27050 data extension: independent review checked D_800B5970/59B0 pair-table indexing, D_800B59F0/F4 signed references with unsigned frame deltas, and D_801230E4 writes/read. These five rows use settings_ and are included in the 98-row executable batch. No new meaning for shared core aliases or girl voice tables is approved.

Exact deferred draft-row counts: 22 data, 126 constant, 16 comment and 2 file/header rows; no placeholder function is deferred. The 22 data rows are eight text/rodata labels (`D_800AFF6C`, `D_800AFF78`, `D_800B00FC`, `D_800B0194`, `D_800B0B64`, `D_800B0B6C`, `D_800B59F8`, `D_800B5A1C`), five shared clip/choice labels (`D_800B37D4`, `D_800B39C4`, `D_800B3B9C`, `D_800B3C70`, `D_800B3C88`), two image/palette hardware internals (`D_800C9A60`, `D_800C9D60`), four SystemData aliases (`D_800E7D11`, `D_800E7D14`, `D_800E7D2C`, `D_800E7D36`) and three HUD actor aliases (`D_8011F414`, `D_8011F422`, `D_8011F426`). Text labels await a separate data batch; shared labels and aggregate aliases require their owner's decision; hardware internals may remain unnamed under phase-2 scope. Constant rows require literal-site conversion, comments require reconciliation with current definitions, and file/header work requires separate moves and declaration-view proof.

T-27050 final verification (2026-10-24): independent Sol 6.1 review PASS for this executable table and reconciled spec. Docker `tokimemo-decomp-arm64 --cpus=2`, `python3 tools/rename.py plan config/renames/proposals/m-sysmenu-apply.tsv --ledger m-sysmenu` exited 0 with 98 rows, 11 affected files, no PROBLEM. Combined four-table validation found no duplicate source keys or target names. No application or build was performed by this lane; the integration owner must apply and obtain clean 27/27 hashes before source completion.

## 15. Source object and topic-header plan

Keep this as one original C object. Recommended source move: `src/main/8005A0B0.c` -> `src/main/home_screens.c`. `home_screens` describes the object’s command, calendar, card, settings, club, rumor, Christmas and New Year screens together; do not split its C subsegments. Optional stem-only file proposal: `config/renames/proposals/m-sysmenu-files.tsv`.

Split declarations by topic, not by object address. Each main symbol must have exactly one declaration. The address ranges below name the function definitions owned by the header; shared data aliases in the frozen 98-row batch remain with their resolved owner.

| Header | Guard | Owned function starts | Declaration role and dependencies |
|---|---|---|---|
| `include/main/command_screen.h` | `MAIN_COMMAND_SCREEN_H` | `0x8005A0B0`, `0x8005A1A0`–`0x8005A668`, `0x8005AC70`, `0x8005AE60`, `0x8005B39C`, `0x8005B43C`, `0x8005B798`–`0x8005B8E0` | Mode 0x34 command, selection and calendar handlers. Its implementation depends on the home-room helpers, HUD icons, menu/cursor, calendar, CD/MDEC, audio and shared game state. |
| `include/main/home_room.h` | `MAIN_HOME_ROOM_H` | `0x8005AB4C`, `0x8005ABD0`, `0x8005AD1C`, `0x8005AD70`, `0x8005AF28`, `0x8005AFC8`, `0x8005B040`, `0x8005B0F4`, `0x8005B1A8`, `0x8005B2BC` | Room background, sick marker and room click helpers used by command, rumor, Christmas and holiday screens. Depends on CD/MDEC, audio/voice and HUD actor declarations. |
| `include/main/save_load.h` | `MAIN_SAVE_LOAD_H` | `0x8005B908`–`0x8005D804` | Mode 0x34 data/card state machine, including `settings_save_load_layout_stash` at `0x8005C4CC`, which is also called by the overlay loader and TACO. Depends on the memory-card API, save/load routines, HUD layout, menu/text and shared save types. |
| `include/main/settings.h` | `MAIN_SETTINGS_H` | `0x8005D9B4`–`0x80060504` | System settings, input modes, cursor speed, voice and mascot screens. Depends on shared input/voice/icon/menu enums, menu/text, controller and HUD mascot APIs. |
| `include/main/club.h` | `MAIN_CLUB_H` | O.BIN functions `0x8006052C`, `0x800605D4`, `0x80060614`, `0x80060710`, `0x80060930` | Retained `join_club*` declarations. Depends on club-name data, shared `Club`, text/voice and game-mode APIs. |
| `include/main/rumor.h` | `MAIN_RUMOR_H` | `0x80060958`–`0x80060C3C`, including O.BIN `uwasa_exit0` (`0x80060BC8`), `uwasa0` (`0x80060BE8`) and `uwasa_main` (`0x80060C3C`) | Rumor setup and mode handler. Depends on HUD, text/voice, CD/MDEC and shared mode/state declarations. |
| `include/main/holiday_prep.h` | `MAIN_HOLIDAY_PREP_H` | `0x80060CC4`–`0x80061688` (includes retained O.BIN `pre_xmas*` and `pre_syogatu*`) | Christmas/New Year preparation. Depends on room background, audio-bank/CD/MDEC, HUD/text, girl-affection and girl-name/voice owners. |

Current shared type homes are registered in `config/header_owners.json`: enums including `GirlId`, `Club`, `CalendarMarker`, `DayEventType`, `GameMode`, `HealthState`, `InputDevice`, `MenuMode`, `IconMode`, `VoiceMode`, `PadBits` and `MouseButtons` remain in `include/game_enums.h`. `GameState`, `SystemData`, `GirlRecord`, `ClubRecord`, `MonthCalendar`, `SpriteQuad` and other shared records are in registered `include/game/*.h` topic headers. `include/main_api.h` and `include/game.h` have been retired. Do not add M5-local copies or duplicate declarations in topic headers.

The following include guidance records the pre-T27061 umbrella transition. Topic ownership is now registered in `config/header_owners.json`, and the umbrella headers are retired; use those current owner paths. The fixed-width signatures, `MAIN_API_OVERRIDE_prim_gradient_rect`, local old-style `void prim_gradient_rect();` view and existing `()` prototype forms remain constraints. The header is a declaration home, not a way to add duplicate `extern`s.

Historical proof order for the pre-T27061 transition: apply the reviewed rename batch; snapshot declaration views; move declarations to canonical topic owners; update includes; compare views and run header checks; then retire both umbrellas and prove the whole tree. The umbrella-retirement step is complete in the current tree. This port-spec sequence does not authorize another source/header move.

## 16. Literal inventory for a later const batch

Use `const`-row literal-site conversion only for the contexts below. The function addresses are the primary evidence; preserve comparison operators, index expressions, casts and fixed-width storage. This inventory does not authorize a blanket replacement of matching integers in unrelated contexts.

### Reuse existing enums and owners

| Existing owner/name | Exact M5 sites and values | Use |
|---|---|---|
| `GameMode` in `include/game_enums.h` | Literal sites only: `0x8005AFC8` sets HELP; `0x8005B43C` sets SCHEDULE; `0x8005D804` sets NAME_ENTRY and COMMAND_SCREEN; `join_club_select` (`0x80060614`) tests 0x34; `join_club_exit` (`0x80060930`) sets SCHEDULE; `uwasa_exit0` (`0x80060BC8`) sets DAY_SCREEN; `pre_xmas` (`0x80060DA4`) sets NEW_YEAR_A; `0x80061044` and `pre_syogatu1` (`0x80061498`) set NEW_YEAR_B. | Reuse `GAME_MODE_HELP`, `GAME_MODE_SCHEDULE`, `GAME_MODE_NAME_ENTRY`, `GAME_MODE_COMMAND_SCREEN`, `GAME_MODE_DAY_SCREEN`, `GAME_MODE_NEW_YEAR_A/B`. Keep context-only associations separate: `0x8005A0B0` and `0x8005D9B4` handlers run under mode 0x34; `uwasa_main` (`0x80060C3C`) runs under 0x44; Christmas/New Year handlers at `0x80060DF8` and `0x80061688` run under 0x42/0x41. Those are context values, not replacement literals in those functions. The core enum’s 0x11/0x13/0x14/0x15 are `GAME_MODE_OPENING`, `GAME_MODE_FILE_SELECT`, `GAME_MODE_NAME_ENTRY`, `GAME_MODE_PROLOGUE`; do not restore historical `MODE_NAME_ENTRY` aliases. |
| `CalendarMarker` | `0x8005A668`: `event == 5`, `event == 8`, marker at `day - 1 + 7 == 0xC`, and `event == 0xB`. | Reuse `MARKER_EXAM`, `MARKER_CLUB_SEASON`, `MARKER_CULTURE_FESTIVAL`, `MARKER_SCHOOL_TRIP`. Preserve marker lookup at `day - 1 + 2` (tomorrow) and the separate `day - 1 + 7` lookahead (day + 6); do not substitute a date arithmetic constant without retaining those exact indices. |
| `Club`, `GirlId`, `HealthState` | `0x8005A668`, `0x8005B43C`, `0x80060614`, `0x80060710`, `0x80060EA0` and `0x80061498`. | Club nibble is extracted from `player_flags.h >> 12`: `0xB` is `CLUB_NONE`, `6` is `CLUB_BASEBALL`, sports-club branch is `>= 6`, culture-festival branch is `< 6`. Girl loops use 0..10 (`i < 11`) in affection and club lookup. Sick checks are `(player_flags.b[1] & 0xF) == 3`, reuse `HEALTH_SICK`; do not type fixed-width fields as C enums. |
| `InputDevice`, `MenuMode`, `IconMode`, `VoiceMode`, `PadBits`, `MouseButtons` | Settings functions `0x8005D9B4`–`0x80060504`, click/confirm paths `0x8005AF28`–`0x8005B43C`. The data-screen acknowledgement mask at `0x8005BB84`, `0x8005BC38`, `0x8005C14C` is `0x860`. | Reuse these existing enums/macros. Replace the acknowledgement mask only as `PAD_START | PAD_CIRCLE | PAD_CROSS`; keep the current `pad_pressed` width and the `mouse_buttons`/confirm-button relationship. |
| Coordination proposals: `HudIcon`/`HUD_ICON_*`, `TextSpeed`/`TEXT_SPEED_*`, `UiSe`/`UI_SE_*` (not currently defined in the shared enum header) | `0x8005A668`, `0x8005B43C` use icon ids 0–14; `0x8005D9B4` uses menu root item ids; `0x8005E150`/`0x8005E2C0` store text-speed values; `0x8005A1A0`/`0x8005AE60` and card/confirmation paths use UI sound ids. | These names remain coordination proposals, not existing shared constants. Agree one shared icon definition with HUD/holiday and one sound-command owner before creating literal sites; scheduler command IDs0–7 are a different domain from icon IDs. The menutext inventory owns the proposed text speeds. Reuse those single definitions once landed; do not create competing icon/sound enums or claim they already exist. |
| Memory-card owner (`m-flow`) | `0x8005BE8C`, `0x8005C14C`, `0x8005C968`, `0x8005CC40`, `0x8005D31C`, `0x8005D56C`; scan/read/write poll calls are in `0x8005BE8C` onward. | Request the memory-card owner's single definitions before conversion: `CARD_RESULT_*` and `CARD_TIMEOUT_FRAMES` are proposed names, absent from the current shared headers/ledgers. The candidate result domain includes 0, 1, 2, 3, 4, 0x13, 0x14, 0xD0, 0xF0, 0xF1 and 0xFF, and a timeout name only where its owner proves the same threshold. Keep this unit’s exact tests: some use `(u32)memory_card_timer > 0x200`, others `>= 0x201U`; do not change signs or cast width in the naming pass. Propose slot status 0/1/0xFF and save-slot count 14 to the card owner; no shared slot-status enum exists in `game_enums.h`. |

### M5-owned local state and rule values

| Family and proposed names | Exact sites and signedness/meaning |
|---|---|
| Command stages: `SETTINGS_COMMAND_MODE_STAGE_SETUP`, `_SELECT`, `_CALENDAR`, `_DATA`, `_SETTINGS`, `_JOIN_CLUB`, `_RELOAD` | `0x8005A0B0` switches stages 0, 1, 2, 3, 4, 5, 0xFF in mode 0x34. Holiday mode uses stages 0x30 and 0x40 in `src/main/80075320.c`; those stay with the holiday owner. |
| Command setup/calendar steps: `SETTINGS_COMMAND_SETUP_STEP_*`, `SETTINGS_COMMAND_RELOAD_STEP_*`, `SETTINGS_COMMAND_CALENDAR_STEP_*` | `0x8005A1A0` and `0x8005A2A8`: steps 0,1,2,3; reload substeps 0,1,2; calendar `0x8005B830`/`B8A0`/`B8E0`: steps 0,1,2. Values select functions, not persisted domain IDs. |
| Command choices and IDs | `0x8005A668`: weekday icons 0–7, special icon 14, icon count 0xE/0xF, marker conditions above, one clubless sentinel 0xB. Icon 14 stores last command 0xE for exam/trip and 7 for rest at `0x8005B43C`; reuse HUD icon names and `GAME_MODE_SCHEDULE`. Exam/trip notices are selected by exact marker comparisons 5/0xB. The first-day card lock is `year == 0x5F && month == 4 && day == 4` at `0x8005A668`, then disables icon 0xD. |
| Club rule values | `0x8005A668`: tournament notice requires club id `== 6`, `year == 0x61`, and `(*(u16 *)&clubs[6].level & 0xF) >= 8`; preserve `>=` and nibble mask. `0x8005A668` culture branch uses club `< 6`. `join_club_message` at `0x80060710` takes captain-text branch when club `>= 6`; selector item `0xB` at `0x80060614` is the cancel item, not `CLUB_NONE`. |
| Affection thresholds: `SETTINGS_NEW_YEAR_CARD_AFFECTION_MIN` | `0x80060EA0` and `0x80061498`: `tbl[i] >= 0x4D` **and** `girls[i].flags.f.met`, with `i < 11`; the former returns a count, the latter builds the sender line. Preserve both `>=` tests and the met check. |
| Save/load state: `SETTINGS_SAVE_LOAD_STEP_*` | `0x8005B908` dispatches exactly 0–9, 0xA0, 0xB0, 0xC0, 0xE0, 0xF0, 0xFF. Substeps in read/write/format handlers are separate 0–5 or 0–4 states. Define names only beside this state machine; do not conflate the step numbers with card result values that happen to match 1–4 or 0xF0/0xFF. |
| Settings state: `SETTINGS_STEP_*`, `SETTINGS_ITEM_*` | `0x8005D9B4`/`0x8005DDB4`/`0x8005E018` through `0x80060504`: root step 0/1/0xFF; nested groups 0x10–0x12, 0x20–0x22, 0x30–0x3B, 0x40–0x42, 0x50–0x52, 0x60–0x62, 0x70–0x72. Root item ids are 0–7; speed items 0–2; icon-mode items 0–2; mouse subitems 0–2; confirm/click items 0–1; click-speed items 0–1; menu-mode items 0–3; cursor-speed items 0–4; voice items 0–2; mascot list 0–12 plus hide item 13. Keep menu item indices separate from stored setting values. |
| Stored setting values | Proposed `ClickMode` (not a current shared enum) 0/1 at `0x8005EE00`; confirm button 0/1 at `0x8005EB74`; cursor speed 1,2,3,4,6 at `0x8005FA20` (5 is not written); menu mode choices 0,1,3,2 at `0x8005F478`; voice mode choices 1,0,2 at `0x8005FF68`; mascot choice 0 means none and a girl is stored as item+1 in `0x800603C0`. Use existing enum/macro owners where present; otherwise define a module-local named value set in `settings.h`, with fixed-width fields unchanged. |
| Click timing and unlock thresholds | `0x8005EF44` clears `D_801230E4`; `0x8005F09C` writes 0xFF, computes `0xFF - ticks`, clamps with `< 0x10` then `> 0x40`, and clears the word. Cursor-speed labels at `0x8005F730` use `flags[8] >= 9 || girl_cleared[8] >= 2`; confirmation wording at `0x8005FA20` instead tests `flags[8] != 0 || girl_cleared[8] != 0`. Preserve this intentional difference; do not label either value as a score/rank without its writer proving that meaning. |
| Room clip selection | `0x8005B040` and `0x8005B0F4` use `((u16)day_count & 7) * 2` to select one of eight `(start,end)` pairs and pass speaker `0xE` to the CD request. A possible local name is `SETTINGS_ROOM_CLIP_INDEX_MASK=7`; preserve the `u16` cast and bit-mask operation, not `% 8`. |
| Rumor/club/New Year local substates | `rumor_setup_mode_stage_dispatch` / `uwasa_main`: stage 0/1 and step 0/1 (`0x80060B24`, `0x80060C3C`, `0x80060BE8`). Club join: steps 0–3 and message substeps 0–4/0xFF (`0x8006052C`–`0x80060930`). Christmas/New Year: mode stages 0/1; New Year stage-0 steps 0–3 and stage-1 steps 0/1 (`0x80061250`, `0x80061634`, `0x80061688`). Keep these local to their owning headers, not in `GameMode`. |
| Shared UI sounds (coordination proposal; no existing UI_SE enum) | `UI_SE_COMMAND_CONFIRM` 0x500, `UI_SE_ACTIVITY_GOOD` 0x501, `UI_SE_ACTIVITY_POOR` 0x502, `UI_SE_MENU_SELECT` 0x50D, card-job-complete 0x50F and doorbell 0x603. Sites: command icon confirm `0x8005B43C`, card job monitor `0x8005AE60`, load prompt `0x8005D56C`, write complete `0x8005CC40`, New Year doorbell `0x800611F0`. Doorbell 0x603 is class B; do not classify it as class A merely because other `UI_SE_*` values are. Keep sound command bytes 0xB1/0xD1/0xC1 and voice command 0x300 in the audio/hardware residue unless their audio owner supplies names. |

### Hardware and presentation literals to leave deferred

The coordinate bounds are measurable but remain presentation values unless a shared UI owner adopts them. At `0x8005A410`, free-layout points fail when `x >= 0x7D`, `x < -0x86`, `y < -0x54` or `y >= 0x55`; zero points require both coordinates equal to 0, and reset occurs only when `zero >= 2 && err != 0`. Room clicks use strict/inclusive tests exactly as written: window `x < 0x22 && x >= -0xD`, `y < 8 && y >= -0xB` (`0x8005AF28`); help `x < 0x50 && x >= 0x41`, `y < -0x10 && y >= -0x1F` (`0x8005AFC8`); radio `x < 8 && x >= -0x27`, `y < 0x24 && y >= 0x19` (`0x8005B040`, `0x8005B0F4`). Mascot clicks require visibility bit `0x80`, `x < anchor+0xC && anchor-0xC < x`, `y < anchor+0x30 && anchor < y`, circle, and `(u32)(frame_count - stamp) >= 0xF1U` (`0x8005B1A8`, `0x8005B2BC`). Keep these signed coordinate comparisons distinct from the unsigned elapsed-frame test. Draw coordinates/colors, VRAM source/destination, CLUT/tpage fields and sprite flags at `0x8005AD1C`/`0x8005AD70`, actor slot numbers, raw XA sector/LBA addresses, voice clip offsets, audio-bank addresses and sample IDs are presentation/hardware/media values. Keep existing widths; the unnamed sprite/mascot bits and other SDK/GPU fields stay deferred until their hardware owner proves a shared definition. Screen strings remain labels/rodata names and are not copied into constants or this spec.

No enum should replace the type of a fixed-width field or local. Enum/macro names annotate the literal site; the stored field retains its current `u8`, `s8`, `u16`, `s16` or `u32` type. Every eventual const row must identify only the listed literal site(s) and preserve comparison signs, casts and operand widths.

T-27050 source-file plan: Docker tokimemo-decomp-arm64 --cpus=2, rename.py plan config/renames/proposals/m-sysmenu-files.tsv --ledger m-sysmenu-files exited0; 1 row, 3 affected files, no PROBLEM. Dry run only.

T-27050 second-work-package verification (2026-10-24): independent Sol 6.1 review PASS for the filename/header recommendations and constants inventory, with all findings resolved. The separate m-sysmenu-files.tsv Docker plan exited0 with 1 rows/3 affected files and no PROBLEM. This review approves recommendations, not implemented headers/constants. Existing symbol batch remains frozen; no source move, header extraction, literal replacement, apply or build was performed.

## T-27062 essential locals and exact constant site manifest

`config/renames/proposals/m-sysmenu-essential.tsv` is the supplementary local/parameter packet. Its historical function scopes resolve through the applied function ledger. The expanded coverage and final plan are recorded below; the frozen 98-row apply packet and file table are unchanged.

`config/renames/proposals/m-sysmenu-constant-sites.tsv` records exact values, domains, signedness constraints and existing owners. Existing `CalendarMarker`, `Club`, `HealthState`, `PadBits` and other core definitions are reuse-only. Proposed `SETTINGS_*` and card-owner names are recommendations pending tool plan and owner agreement, not declarations. Preserve stored field widths, comparison operators, casts, and the separate settings-step/menu-item/stored-value/result domains.

The current `rename.py const` site selectors cover member names across all users of the shared header, not one function's occurrence. Mixed-domain `mode_stage`, `mode_step` and menu state sites therefore remain evidence only. The three consistent settings fields have separately reviewed field-wide dry plans below. The value passed as `arg0` to `settings_command_screen_icons_reset` (0/1) remains deferred until the post-ledger function resolves in a Docker plan. Existing core enum values can be cited at exact sites but are not redeclared. Memory-card result/timeout values remain with the card owner; presentation bounds and GPU/audio/media values remain deferred per the existing hardware section.

## T-27062 completion update: source-wide essentials and tool limits

The expanded `m-sysmenu-essential.tsv` now has 39 evidence-backed local/parameter rows across command-screen, layout repair, save/load, options, card sender-count and New Year sender-text code. It includes each required role review: selected girl and player club nibble, tomorrow marker and menu result; both layout counters and saved icon mode; stash/restore and highlighted-item parameters; card job/result/slot state; stored icon/click/voice setting views; unlock-check counter/index; weighted-affection sender data and all New Year text buffers. The root `available` local only zeros `w[6]` when the count is zero; it does not select a cursor. The count is individual nonzero checks across the clear/unlock fields, not distinct girls.

The constant manifest now has 229 five-column rows covering the literal domains in section 16, including separate command mode stages, setup/reload/calendar substeps, icon ids versus command ids, save/load states versus card results, setting substates versus stored choices/menu indexes, thresholds, masks, date checks, sound proposals, and presentation/hardware deferrals. `REUSE:` rows point to existing core owners. New `SETTINGS_*`, `CARD_*` and `UI_SE_*` families are proposals or explicit `DEFER:` rows, not tool-ready replacements. No broad replacement is proposed: shared `GameState` member selectors scan all files that use the shared header, and mode-stage/state values overlap other semantic domains. The manifest also records the two exact save/load date conjunctions (1998-03-01 and 1995-04-04) separately from the initial command-screen date lock.

A code inspection also found `rename.py const` parses a negative literal as a unary minus on a positive leaf, then can replace the leaf without preserving the sign. Consequently every negative-bound candidate remains deferred until that behavior is fixed and separately verified. A declaration scan covered all 109 function definitions in `home_screens.c`; rows are limited to locals/parameters that carry game-rule or flow state. Loop counters, coordinate arrays, frame-only fake locals and CD decode results remain addressable from the source but are excluded from this essential packet. The final serialized Docker plan passed: 39 rows, two affected files, exit 0 and no `PROBLEM`. Existing-name inverse checks were skipped at reported sites; the compiler error-set proof passed.

T-27062 final manifest reconciliation: the 81 added groups come from exact C AST state/menu case, assignment and comparison sites. Nested get_icon_can_use return-zero tests and flags[index] zero tests are excluded from cursor-value groups. Canonical local packet is m-sysmenu-essential.tsv (39 rows); the earlier m-sysmenu-locals.tsv is a superseded overlapping analysis draft. icon11 opens settings stage4; calendar exit returns to stage0/step2; NewYear/card/UI sound and poll-code contexts are stated at their actual handlers. No literal or source edit has been performed.

T-27062 final review (2026-10-24): independent Sol review passed the essential table, corrected 229-record manifest and the three positive settings families. `invalid_icon_coordinate_count` counts invalid X/Y components across 16 position pairs; two invalid components in one pair count twice. The frozen 98-row symbol plan passed after the main file moves (exit 0, 11 affected files).

The following dry runs use existing `include/main_api.h` as visibility root and no `--apply`; this does not decide the eventual topic-header owner or change u8 storage. Values files are under `config/renames/proposals/`.

| Family / values file | Site | Final Docker result |
|---|---|---|
| `SettingsClickMode` / `m-sysmenu-click-mode.values` | `GameState.click_mode` | 2 values, 5 literals, 3 files, exit 0, 0 unresolved |
| `SettingsConfirmButton` / `m-sysmenu-confirm-button.values` | `GameState.confirm_button` | 2 values, 16 literals, 6 files, exit 0, 2 unresolved |
| `SettingsCursorSpeed` / `m-sysmenu-cursor-speed.values` | `GameState.cursor_speed` | 5 values, 19 literals, 6 files, exit 0, 0 unresolved |

Confirm-button values are stored 0 = left and 1 = right; they select the existing `MOUSE_LEFT`/`MOUSE_RIGHT` masks. The two unresolved zeros occur in compound comparisons in `src/main/home_screens.c:2073` and `src/ovl/OPTION/settings_sound_test_and_extras.c:1255`; the tool reports `0 not found on its line (macro?)` even though the source contains the comparisons. Preserve them pending matcher support. Cursor speed menus skip 5; save/debug restoration can supply other values. Logs are the ignored `T-27062-sysmenu-{click,confirm,cursor}-final-const-plan.txt` and `T-27062-sysmenu-essential-final-plan.txt` under `.claude/orchestrator/state/`. Application, owner headers and matching builds remain with root.

T-27062 post-application revalidation (2026-10-24): after root applied the 247 main UI symbols through `config/renames/main-ui.txt`, this unchanged 39-row local packet passed a fresh Docker plan (exit 0, two affected files, no `PROBLEM`). All three settings constant plans were repeated with the field selectors above and retained 5/3/0, 16/6/2 and 19/6/0 literal/file/unresolved counts. Independent Sol follow-up review passed scope meanings and selectors. New exact nonnegative `--reuse` support is available for eligible existing core enums, but this manifest still requires individually prepared exact comparison/subscript selectors; shared mixed-domain blanket replacement remains invalid. Evidence: ignored `T-27062-sysmenu-post-main-ui-plan.txt` and `T-27062-sysmenu-{click,confirm,cursor}-post-main-ui-const-plan.txt` under `.claude/orchestrator/state/`.
