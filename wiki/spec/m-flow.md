---
type: spec
unit: m-flow
updated: 2026-10-24
sources: ["src/main/80041000.c", "src/main/800420D0.c", "src/main/80042540.c", "src/main/80042A00.c", "src/main/800789E0.c", "src/main/80061710.c", "src/main/80053650.c", "config/renames/proposals/m-flow.tsv", "config/renames/proposals/core.tsv", "wiki/spec/core.md", "tools/emu (probes from the title-menu save state)"]
---

# Port spec: game flow (m-flow)

Roadmap phase 2, unit `m-flow` ([[tickets/T-27019-p2-m-flow]], parent [[tickets/T-27000-phase-2]]). This is the top of the game: the code that boots the machine, runs one pass per frame, switches between modes, loads the scene overlays, shows the title, reads the pad and keeps the memory card. It covers all 115 functions of seven files; 87 get a name in this unit's table, 17 come from core, 3 already had names (`main_state_set`, `ovl_load_poll`, `ovl_load_verify`) and 8 are O.BIN names that stay. Facts come from the matched C, its callers and callees, the shared core names and specs, and the emulator (probes from a title-menu save state; frame numbers below are emulator frames, 60 per second).

Names are the proposals of `config/renames/proposals/m-flow.tsv`, written without the old placeholder except in tables that map the two. Where core, the hardware units or the other main units already name a symbol, this spec uses that name. Nothing here quotes game text.

## 1. Files and prefixes

| file today | proposed file | what it holds | prefixes |
|---|---|---|---|
| `80041000.c` | `game_init.c` | graphics bring-up (`gpu_init`), VRAM clears, `game_state_init`, the data initialisers, sprite and BG table clears | `gpu_`, `vram_`, `calendar_`, `player_`, `girls_`, `event_state_`, `settings_` |
| `800420D0.c` | `main_loop.c` | executable entry, `main_loop`, per-frame buffer setup, the vblank timer event, system init | `boot_`, `timer_`, `sys_`, `frame_` |
| `80042540.c` | `game_mode.c` | `main_state_dispatch` and the mode, stage, step setters (all core names) | `main_state_`, `main_stage_`, `main_step_` |
| `80042A00.c` | `pad_input.c` | controller detection, pad and mouse polling, soft reset, turbo, the secret pad code | `pad_`, `mouse_` |
| `800789E0.c` | `overlay_loader.c` | overlay loader (mode 2), loader screen, the TT loader (mode 0xD0) | `ovl_`, `mode_` |
| `80061710.c` | `title_screen.c` | title screen (mode 0x12) and its 3D intro, start mode (0x10) | `title_`, `mode_` |
| `80053650.c` | `memory_card.c` | memory-card layer, system and game save images, two MDEC still-image helpers | `card_`, `mdec_` |

Prefix choices that differ from the ADR table (`docs/adr/0001-phase-2-naming.md`): the memory card keeps core's `card_` (the ADR row says `memory_card_`; core's `card_start`, `card_stop` and the GameState fields `card_busy`, `card_result`, `card_step` already use it and sibling units use it too), and `timer_` moves from the `80053650` row to the `800420D0` row, because the only timer code is the vblank event in `800420D0.c` (80053650 has none). The ADR rows `boot_` (boot and startup), `pad_` (pad helpers), `title_` and `ovl_` fit as they stand; `mouse_` is the hardware unit's prefix and is used for the mouse functions that sit in `pad_input.c` (the ADR rule "One owner": a function takes the prefix of its topic). `sys_` is used for the two start-up helpers (`sys_init`, `sys_cache_flush`).

New headers (declarations move out of `main_api.h`): `include/main/game_init.h`, `main_loop.h`, `game_mode.h`, `pad_input.h`, `overlay_loader.h`, `title_screen.h`, `memory_card.h`. The save-image types (`CardBank`, `GameSaveImage`, `SystemSaveImage`) move with `memory_card.h`.

## 2. Boot and the frame loop

### 2.1 Start-up

`boot_start` (`func_800420D0`, the entry PC of the executable): stack top 0x801FEFF0, heap 0x801FF800 for 0x7F0 bytes, zero 0x45498 bytes from 0x800E60A0 (everything from just before `game_state` to the end of bss, 0x8012B538), remember the old stack pointer and gp, call `main_loop`.

`main_loop` (`func_80042134`, core name), once: reset the BIOS callbacks, SDK init, print a banner line to the debug console, `sys_init` (zero `system_data`, start the timer event, open the card events, init the pad driver), `vram_clear_framebuffers`, and then the "restart" block, which is `snd_init`, `cd_cmd_reset`, `gpu_init`, `game_state_init(1)`. `restart_request` is never written outside `game_state_init` (it is cleared there), so the jump back to the restart block never happens.

Start-up in the emulator (frames from power-on; the movie and boot-mode frames are from the h-cdio spec): the first frame already runs mode 0x10 (`MODE_START`); `mode_start` plays the start movie (frames 622-1256) in one blocking call, pauses and re-inits the CD, sets the MDEC buffers (`mdec_still_buffers_set(0x80167000, 0x80162000, 0x80162000)`), clears VRAM block 0, turns the back clear on and calls `main_state_set(0)`. Mode 0 is the boot mode (h-cdio: loads the HUD textures, 57 frames) and ends with `main_state_set(0x12)`. The title starts at frame 1322.

### 2.2 One pass of the loop

1. `frame_begin`: `draw_buffer` = the buffer being drawn; the primitive arena (`prim_arena_buffers`, 0x10C00 bytes per buffer) and a second work area (`gs_work_buffers`, 0xA000 per buffer, HYPOTHESIS: the libgs packet area) are pointed at it (the two areas swap roles in mode 0x90, the TACO shooter); the ordering table of the buffer is initialised; `cursor_moved_this_frame` is cleared.
2. `paused` == 0: `main_state_dispatch` (section 3); else `main_paused_step`, an empty function. The mode machine freezes while paused, drawing does not.
3. `frame_count` += 1 (counts loop passes, paused or not).
4. Only while `debug_hud_enabled` (`D_800B3220`, set by OPTION's debug page) is set: `debug_state_numbers_draw`; pad 2 L1+R1 held runs the VRAM view (the mode 0xFE handler) directly; a pad 2 R1 or R2 press calls `debug_hud_level_step(1)`.
5. If `restart_request` == 0: `frame_render(1)` (draws everything, waits for vblank only while the elapsed horizontal-blank count is below `frame_hcount_limit`, then reads the input with `pad_update`, then presents), `cd_cmd_pump`, and `snd_command_pump` while `sound_active`.

Input is therefore sampled once per frame, after the vblank wait, and is seen by the next pass's `main_state_dispatch`.

### 2.3 Clocks

| value | where | meaning |
|---|---|---|
| `frame_count` (`unk_10F8`) | `main_loop` | loop passes since boot or reset |
| `timer_ticks` (`unk_10F4`) | `timer_tick` | vertical blanks; the vblank event of root counter 3 (target 1, interrupt mode) fires `timer_tick` every blank. The title seeds `srand` with it |
| `system_data.seed` | `timer_tick` | += 0x377 every blank; NAME_ENT seeds `srand` with it and uses it as a list index; `timer_seed_step` (no caller) is the same step as a function |
| `mode_timer`, `stage_timer`, `step_timer` | setters and dispatcher | core spec section 2.1 |

`timer_event_start`/`timer_event_stop` are called again by the soft reset and by the TT loader, which closes the event while the TT code runs its own loop.

### 2.4 `game_state_init(clear)` and the data initialisers

`game_state_init(clear)` (core name) is the one reset. Steps in order: graphics reset; if `clear` != 0, clear all VRAM (`vram_clear_blocks(0xFF)`) and zero the first 0x1A8C bytes of `game_state`; then the defaults below; then `tpage_buf_clear`, sync `tpage_req`, `sprite_tables_clear`, `actor_clear_range(0)`, `dec_bg_reset`, `snd_bank_state_reset`, the menu bar colour, `pad_defaults_set`, the placeholder name strings, the birthday 1/1, cursor actor init and hide, the text marker toggled once, `game_data_init`, `card_block_clear`, zero `overlay_scratch` (0x100), `dialogue_work` (0x100) and `scene_work` (0x40), `settings_defaults_set`.

Defaults written directly: video 320x240 (`video_width`, `video_height`), `screen_center_x` = 0xA0 for both buffers, `screen_center_y` = 0x168 and 0x78, clear colour 0, all four timers 0, `mode` = `MODE_START` (0x10), stage and step 0, pad words 0, `paused` 0, `restart_request` 0, `frame_hcount_limit` 0x106, `actor_draw_last` = 0x9F (all 160 actors), `screen_brightness` 0x80, `back_clear` 0, 2D and 3D passes off, `sound_active` 1, `quads_enabled` 1.

`game_data_init` is the new-game state; it also runs at every reset and when NAME_ENT starts a new game (NAME_ENT also calls it, with `event_state_init`, for a loaded save dated 1995-04-04 or 1998-03-01, then returns to mode 0x14). It is `calendar_init`, `player_init`, `girls_init`, `event_state_init`:

| part | initial value |
|---|---|
| calendar | `vacation` none, `year` 95, `month` 4, `day` 4, `weekday` 1 (Tuesday), `day_count` 0; `month_calendar[0]` = April 1995 (30 days, 1st is a Saturday, `day_kind` 1 on the Sundays 2, 9, 16, 23, 30, 2 on the 29th, marker 1 on day 4); `month_calendar[1]` = May 1995 (31 days, 1st is a Monday, Sundays 7, 14, 21, 28, holidays 3, 4, 5); `clubs` and `quarter_flags` zeroed. Emulator dump of these bytes agrees |
| player | 0x7C profile block zeroed; `player_flags` health OK, club 11 (none), career bits 0; birthday 1/1 with the blood-type field 1; parameters (value, in PARAM_* order condition, liberal arts, science, art, athletics, trivia, looks, guts, stress) 100, 40, 40, 40, 40, 32, 60, 5, 0 |
| `progress_state` | the 21 place flags (m-hud) are 1 for places 0, 2, 4, 8, 9, 12, 15 (neighborhood park, shopping street, zoo, library, game center, amusement park, concert hall) and 0 for the rest, the nine command counters 0, and the profile snapshot is a copy of the profile block |
| girls | all 16 records: tokimeki 0, affection 0x32, `syoushin` 0, every flag bit cleared, `story_stage` 0, club 11; girls 0 (shiori) and 12 (rei) have `met` set, shiori also flag bits 3, 6, 7, rei bit 3 |
| birthdays (month/day) | 0 shiori 2/14, 1 mio 2/3, 2 yuina 7/7, 3 ayako 9/30, 4 saki 1/13, 5 yukari 6/13, 6 nozomi 12/3, 7 mira 11/15, 8 yuko 10/17, 9 megumi 9/5, 10 yumi 5/16, 11 miharu 3/3, 12 rei 8/23, 13 yoshio 4/4 (the player's own is the 1/1 default until NAME_ENT sets it) |
| events | `appointments`, `clubs`, the `day_events` queue and its index, `event_flags` (0x100 bytes, this includes `saved_settings`), appointment places, `date_place_history`, `girl_last_date_spot`, `date_girl`, `appointment_flag` and the `event_log` with its count are zeroed; `girl_last_date_spot_timer` and `girl_event_bits` are not touched here (only the full clear of `game_state_init(1)` zeroes them) |
| settings (`settings_defaults_set`) | `current_girl` 0, dialogue scroll state 0, `voice_mode` 2 (cancelable), `icon_mode` 0 (type A), `icon_count` 0xE, `click_mode` 0, `double_click_interval` 0x20, `confirm_button` 0 (left), `sd_anim_fast` 0 |
| input (`pad_defaults_set`) | `input_device` pad, `cursor_speed` 3, `menu_mode` 1, `overlay_entry` = 0x80132000 |

## 3. The mode machine

### 3.1 Levels and setters

The four levels (`mode`, `stage`, `step`, `substep`), the reset matrix of the setters and the timers are in [[spec/core]] section 2.1; this unit adds the facts that are not there:

- `main_state_set(m)` writes `mode`, clears stage, step, `mode_timer`, `stage_timer`, `step_timer`, both scratch bytes and `substep`, then runs `main_state_request_overlay`. After that it clears `dialogue_voice_wait_frames` (`D_800CA2AC`, the dialogue unit's counter) and hides the window title banner (`message_window_title_show(0)`) unless `overlay_target_mode` is 0x5E or 0x50 (a date mode). Note that `overlay_target_mode` is only written by an overlay request and keeps the last value, so the banner stays through mode changes while the last requested overlay was a date.
- `main_state_next` (mode + 1 with the same reset) has no caller in `src/`.
- `main_stage_next`, `main_step_next`, `main_stage_set`, `main_step_set` also clear `substep`; they clear it by writing through a pointer to a local zero, which is a match trick, not a rule.

### 3.2 Dispatcher

`main_state_dispatch` (`func_80042540`) switches on the mode byte once per pass and adds one to `mode_timer` after the handler returns (so a paused pass does not count). O.BIN's `game_main` aligns with it (obin.md, class low, not applied).

| mode | handler |
|---|---|
| 0x00 | `mode_boot` (core; h-cdio calls it `boot_mode_run`) |
| 0x01 | `main_state_set(0)` once `boot_hold` (`unk_111A`) is 0; no caller of mode 1 found |
| 0x02 | `mode_overlay_loader` |
| 0x03 | calls `overlay_entry` with no loader and no debug text; no caller found |
| 0x10 | `mode_start` |
| 0x12 | `mode_title` |
| 0x30 | `schedule_init` (O.BIN name; m-scheduler) |
| 0x31 | `mode_day_screen` |
| 0x32 | `mode_week_end` |
| 0x34 | `mode_command_screen` |
| 0x38, 0x39 | `mode_month_start`, `mode_day_notice` |
| 0x40 | `week_day` (O.BIN name) |
| 0x41, 0x42 | `mode_new_year_prep`, `mode_christmas_prep` (m-sysmenu names) |
| 0x44 | `uwasa_main` (O.BIN name; the rumor report) |
| 0x46 | `mode_holiday` |
| 0xD0 | `mode_tt_game` |
| 0xFE | the debug VRAM view (`func_8004F6B0`); the main loop also calls it directly |
| 0xFF | `exit_function` (reloads the executable by a name this disc does not have; nothing requests mode 0xFF) |
| the 48 overlay modes | `debug_print_string` of a fixed label at (0x14, 0x2C) (only drawn while `debug_hud_enabled`), then a call of the function at `overlay_entry` |
| anything else | `game_reset_to_title` (514 callers use it as the error exit) |

The 48 overlay modes are 0x11, 0x13-0x15, 0x20-0x22, 0x35-0x37, 0x3A, 0x43, 0x45, 0x50-0x63, 0x70-0x76, 0x80-0x82, 0x90, 0x91, 0xC2-0xC4. The same list is in `main_state_request_overlay`.

### 3.3 Overlay request and loader

A scene that lives in an overlay is entered by `main_state_set(mode)`. For one of the 48 overlay modes `main_state_request_overlay` stores the mode in `overlay_target_mode` (`unk_110E`) and sets `mode` = 2. For any other mode nothing is requested.

`mode_overlay_loader` runs once per frame in mode 2: one step by `ovl_load_state`, then `ovl_load_screen_draw`.

| `ovl_load_state` | function | rule |
|---|---|---|
| 0 `OVL_LOAD_START` | `ovl_load_start` | backdrop choice, then the disc position; if it equals `ovl_load_lba` (the file already in the slot) go to state 3, else `cd_read_sectors(0x60, 0x80132000, lba)` and state 1 |
| 1 | `ovl_load_poll` | `cd_read_sync_status() & 1` -> state 2 |
| 2 | `ovl_load_verify` | `file_checksum_ok(0x60, 0x80132000)`: ok -> state 3 and `sys_cache_flush`; mismatch -> state 0 with `ovl_load_lba` = 0, which repeats the read forever |
| 3 | `ovl_load_enter` | `overlay_entry` = the entry address of `overlay_target_mode` (section 3.4, default 0x80132000), state 0, then `mode` = `overlay_target_mode`; `stage`, `step`, `substep`, `mode_scratch` and `stage_scratch` are put back to the values they had before; `stage_timer` and `step_timer` are cleared, `mode_timer` keeps counting |

Rules the port must keep:

- **The resident-file shortcut.** `ovl_load_lba` is the LBA of the file in the slot (0 after `ovl_load_reset`). A request for a mode whose file is already resident skips the disc read, and the overlay is not reloaded, so its own initialised data and statics keep the values they had (GEKO modes 0x56, 0x57, 0x59, 0x5A, 0x5D; TEL modes 0x43 and 0x45; and so on). Only a different file, a reset or a failed checksum reloads it.
- **Stage and step survive the loader.** A caller may change stage, step or substep right after `main_state_set` (`main_state_set(0x5E); main_stage_set(3)`), and the overlay then starts there.
- **Frames.** At least two loader frames (mode 2, then state 3 with the shortcut). A real load of the OPTION file took frames 183-240 of a title-menu run: 52 frames of CD read, then about 5 frames of checksum.
- **`ovl_load_start` side effects.** `back_clear_switch(1)` always; sprite quad 0 is hidden when the player's health is `HEALTH_SICK`; modes 0x54, 0x55, 0x59, 0x5A, 0x5B, 0x5D and 0x72 first call `dec_bg_capture_screen(0xA0)` (the top 160 lines of the shown frame are kept as a still) and select screen mode 1; modes 0x13, 0x35, 0x50 and 0x81 select screen mode 0 without a capture.
- **The loader screen** (`ovl_load_screen_draw`, every loader frame): when any of the date panel, message window, parameter panel, mascot or cursor actor is visible, their per-frame functions keep running, then the screen frame is drawn and the backdrop follows `ovl_load_screen_mode`: 0 = the scene effects (`scene_effect_flags` bit 0 light effect, bit 1 title banner slide, set by the DATE and GEKO scenes) plus the last background through `dec_bg_draw`, 1 = wipe lines over the captured still (`dec_bg_wipe_step`, `dec_bg_draw(1)`), anything else (never set) = a cleared screen. The calendar picture (`cal_base_show`) is drawn first whenever `overlay_target_mode` is 0x45 (the TEL profile page), whatever is visible. Apart from that, nothing is drawn when no HUD piece is visible.

### 3.4 Mode, overlay and entry

Disc file = `/CDROM/EXEDIR/<name>.EXN`, 0x60 sectors (196608 bytes) each, read to 0x80132000. Folder names are the ADR map. "Core name" is the `MainMode` constant of core. The entry column is the address the loader stores in `overlay_entry`; the overlay's own function there runs once per frame while the mode lasts.

| mode | core name | overlay folder (disc file) | LBA | entry |
|---|---|---|---|---|
| 0x11 | `MODE_PROLOGUE` | `name_entry` (NAME_ENT) | 0x95EA | 0x80144AD0 |
| 0x13 | `MODE_NAME_ENTRY` | `name_entry` | 0x95EA | 0x8014231C |
| 0x14 | `MODE_NAME_ENT_3` | `name_entry` | 0x95EA | 0x8013209C |
| 0x15 | `MODE_NAME_ENT_4` | `name_entry` | 0x95EA | 0x8014AF28 |
| 0x20 | `MODE_OPTION` | `options` (OPTION) | 0x976A | 0x801335E8 |
| 0x21 | `MODE_OPTION_2` | `options` | 0x976A | 0x801389A0 |
| 0x22 | `MODE_OPTION_3` | `options` | 0x976A | 0x8013C780 |
| 0x35 | `MODE_EXAM` | `misc_scenes` (ETC) | 0x93AA | 0x801465F8 |
| 0x36 | `MODE_CAREER_GUIDANCE` | `misc_scenes` | 0x93AA | 0x8014ABD0 |
| 0x37 | `MODE_CAREER_CHOICE` | `misc_scenes` | 0x93AA | 0x801492C0 |
| 0x3A | `MODE_HELP` | `help` (OLH) | 0x96AA | 0x80132000 |
| 0x43 | `MODE_TELEPHONE` | `telephone` (TEL) | 0x9A0A | 0x801397D0 |
| 0x45 | `MODE_TELEPHONE_PROFILE` | `telephone` | 0x9A0A | 0x80132000 |
| 0x50 | `MODE_DATE` | `date` (DATE) | 0x922A | 0x801448D4 |
| 0x51 | `MODE_SCHOOL_TRIP` | `school_trip` (SHUGAKU) | 0x98EA | 0x80137490 |
| 0x52 | `MODE_NEW_YEAR_A` | `new_year_christmas` (SHOUGATU) HYPOTHESIS | 0x988A | 0x8013D180 |
| 0x53 | `MODE_NEW_YEAR_B` | `new_year_christmas` HYPOTHESIS | 0x988A | 0x801322D4 |
| 0x54 | `MODE_VALENTINE` | `valentine` (VALEN) | 0x9ACA | 0x80132274 |
| 0x55 | `MODE_SEASONAL_PARTY` | `new_year_christmas` HYPOTHESIS | 0x988A | 0x8013B484 |
| 0x56 | `MODE_PLAYER_BIRTHDAY` | `girl_encounters` (GEKO) HYPOTHESIS | 0x946A | 0x80134040 |
| 0x57 | `MODE_GIRL_BIRTHDAY` | `girl_encounters` HYPOTHESIS | 0x946A | 0x80134C90 |
| 0x58 | `MODE_AMUSEMENT_PARK` | `amusement_park_date` (DATE2) | 0x928A | 0x80132214 |
| 0x59 | `MODE_GIRL_ENCOUNTER` | `girl_encounters` HYPOTHESIS | 0x946A | 0x80132244 |
| 0x5A | `MODE_GIRL_INVITATION` | `girl_encounters` HYPOTHESIS | 0x946A | 0x801426D0 |
| 0x5B | `MODE_MEETING` | `new_year_christmas` HYPOTHESIS | 0x988A | 0x80137144 |
| 0x5C | `MODE_AMUSEMENT_PARK_2` | `amusement_park_date` | 0x928A | 0x80137F64 |
| 0x5D | `MODE_SPECIAL_EVENT` | `girl_encounters` HYPOTHESIS | 0x946A | 0x8013882C |
| 0x5E | `MODE_DATE_SCENE` | `date` | 0x922A | 0x8014C08C |
| 0x5F | `MODE_CLUB_CAMP` | `summer_camp` (KANGEI) HYPOTHESIS | 0x952A | 0x80132FF8 |
| 0x60 | `MODE_CLUB_CAMP_2` | `summer_camp` HYPOTHESIS | 0x952A | 0x80132000 |
| 0x61 | `MODE_SICK_VISIT` | `sick_visit` (OMIMAI) | 0x970A | 0x80133120 |
| 0x62 | `MODE_SICK_VISIT_2` | `sick_visit` | 0x970A | 0x80132000 |
| 0x63 | `MODE_ENDING` | `ending` (ENDING) | 0x92EA | 0x80132334 |
| 0x70 | `MODE_CULTURE_FESTIVAL_A` | `culture_festival` (BUNKASAI) | 0x916A | 0x80142700 |
| 0x71 | `MODE_CULTURE_FESTIVAL_B` | `culture_festival_tour` (BUNKAKEN) HYPOTHESIS | 0x910A | 0x80144F70 |
| 0x72 | `MODE_SPORTS_FESTIVAL` | `sports_festival` (TAIIKU) | 0x99AA | 0x80141280 |
| 0x73 | `MODE_PRACTICE_MATCH` | `practice_match` (RENSYU) | 0x97CA | 0x80132040 |
| 0x74 | `MODE_CLUB_MASTER_A` | `club_master` (MASTER) | 0x958A | 0x80133A2C |
| 0x75 | `MODE_CLUB_MASTER_B` | `club_master` | 0x958A | 0x80139150 |
| 0x76 | `MODE_CLUB_MASTER_C` | `club_master` | 0x958A | 0x80132140 |
| 0x80 | `MODE_CULTURE_STAGE` | `culture_stage_shows` (BUNKA_SD) | 0x91CA | 0x80137890 |
| 0x81 | `MODE_RPG_BATTLE` | `rpg_battle` (RPG_BAT) | 0x982A | 0x8013B5B0 |
| 0x82 | `MODE_FESTIVAL_STALLS` | `festival_stalls` (EN_NICHI) | 0x934A | 0x80132000 |
| 0x90 | `MODE_SHOOTER` | `shooter_3d` (TACO) | 0x994A (default arm) | 0x80132000 (default) |
| 0x91 | `MODE_ALBUM` | `misc_scenes` | 0x93AA | 0x801407F0 |
| 0xC2 | `MODE_ETC_C2` | `misc_scenes` HYPOTHESIS | 0x93AA | 0x80142974 |
| 0xC3 | `MODE_ETC_C3` | `misc_scenes` HYPOTHESIS | 0x93AA | 0x8014636C |
| 0xC4 | `MODE_ETC_C4` | `misc_scenes` HYPOTHESIS | 0x93AA | 0x80145A6C |

The sector switch names 47 of the 48 overlay modes (0x90 takes the default arm) and also lists 0xD0, which can never reach it (0xD0 is not an overlay-request mode); the entry switch names the same 47 modes and falls back to 0x80132000 for the rest. The EVENT.EXN (0x940A) and GYOZI.EXN (0x94CA) files are not in either table, so no code loads them.

HYPOTHESIS marks in the table: the ADR map still lists `summer_camp` (KANGEI), `culture_festival_tour` (BUNKAKEN), `girl_encounters` (GEKO), `new_year_christmas` (SHOUGATU) and the modes 0xC2-0xC4 of `misc_scenes` (ETC) as `confirm`, and core marks the matching mode names (0x5F/0x60, 0x55) as hypotheses. The disc file names, LBAs, modes and entries are facts from the loader code; only these folder names and the scene meanings behind them are unproven, and the five `OvlLba` constants carry the mark in the table.

Core's `MainMode` rows for 0x74, 0x75, 0x76 give the entries 0x80132140, 0x80133A2C, 0x80139150; the loader code has 0x74 -> 0x80133A2C, 0x75 -> 0x80139150, 0x76 -> 0x80132140 (see open question 1). The loader's `extern` labels (`D_8013xxxx`) are overlay addresses; each overlay unit names its entry function and asks for a request row in `config/renames/requests.txt` so the loader follows.

### 3.5 Returning from a scene

A scene that borrows the machine saves the caller's context and restores it when it ends: `saved_mode`, `saved_stage` and `saved_step` (often stage + 1 and step + 1) are written before the scene starts, and the end of the scene calls `main_state_set(saved_mode)`, `main_stage_set(saved_stage)`, `main_step_set(saved_step)` (the developer function `return_step`; `mode_tt_game` stage 6 does it inline). This is how mini games, festivals and menu overlays hand control back to the day loop.

### 3.6 Reset paths

| path | trigger | effect |
|---|---|---|
| `game_reset_to_title` (core) | the `default:` arm of mode switches and of the title exit | `ovl_load_reset`, `game_state_init(0)` (mode 0x10 for an instant), `main_state_set(0x12)`, sprite tables cleared, actors deactivated, back clear on, `dec_bg_reset`, sound bank state reset, `tpage_buf_clear`, `SND_CMD_STOP_ALL`, `snd_shutdown` then `snd_init`, `cd_cmd_reset`, `game_data_init`, `settings_defaults_set`; returns -1. All game progress in memory is gone |
| soft reset | `pad_poll`, section 4 | the same, but `main_state_set(0)` (boot mode, then the title) or `main_state_set(0x11)` for pad 2, plus `timer_event_stop`/`timer_event_start`, `display_set_512_or_320(0)`, `card_stop` when a card job runs and `cursor_actor_init(0)`; emulator: hold from frame 10, reset at frame 140 (130 frames later), `main_state_set(0)` at 140, `main_state_set(0x12)` again at 197 |

### 3.7 TT: the hidden mini game's own loader

Mode 0xD0 is not an overlay mode. `mode_tt_game` loads the TT file itself and runs it as a blocking call, stage by stage (`TT_STAGE_*`):

| stage | action |
|---|---|
| 0 | `extra_mode_flags` bit 0 := 1 (HYPOTHESIS: this is the flag that makes the extras menu entry appear after the first start; that reading comes from OPTION and core, this unit only sees the store), `debug_hud_enabled` := 0, VRAM cleared, sprite tables cleared, read 0x60 sectors from 0x9A6A to 0x80132000 |
| 1 | wait for the read |
| 2 | `file_checksum_ok` -> next stage, `sys_cache_flush`, then an unnamed SDK call with argument 0 (`func_8009C5E0`; HYPOTHESIS: it blanks the display, the movie player calls it with 1 to switch the display back on); mismatch -> stage 0 |
| 3 | `overlay_entry` := 0x8013E850 (`tt_main`), timer event closed, call it; it runs its own frame loop and returns when the game is left; VRAM cleared |
| 4 | timer event reopened, `snd_bank_state_reset`, `dec_bg_reset`, `cd_cmd_reset`, `SND_CMD_STOP_ALL`, load base 0x801B8000 |
| 5 | `mode_tt_game_save_step`: write the system file (substeps: card check, file 0 exists, read it, `system_bank_load`, `selected_slot` := 0, write it; success plays sound 0x50F, a timeout of 0x201 frames or result 0xF0 plays 0x502); a failure anywhere skips the rest |
| 6 | `options_dirty` == 0: back to `saved_mode`/`saved_stage`/`saved_step`; otherwise `main_state_set(0)` with sprites, tpage slots and the 2D/3D passes reset |

`mode_is_tt_game` (mode 0xD0 or 0xD1), `mode_is_shooter` (0x90) and `mode_is_option_2` (0x21) are queries used by the sound driver.

### 3.8 The shape of the whole game

`MODE_START` -> start movie -> mode 0 (boot data) -> `MODE_TITLE` (0x12). The title leads to NAME_ENT (mode 0x13: new game, load, copy and clear files, name and birthday entry) or OPTION (0x20), or idles into the opening movie (0x11, back to the title). A started or loaded game enters the weekly cycle at the weekday command screen, mode 0x34 stage 0xFF (the notice steps), from where the scheduler (mode 0x30), the day screen (0x31) and the day modes run as in [[spec/m-scheduler]] and [[spec/core]]; every scene overlay returns with the saved triple. The 18-step pad code at the title, the soft reset and `game_reset_to_title` are the only exits from that cycle besides the endings.

### 3.9 Who requests which mode

Literal `main_state_set(mode)` calls in `src/`, by requester (calls with a computed mode, such as the return through `saved_mode`, are not in the list). Together with the queue-type table of [[spec/core]] section 3.5 this is the whole transition graph.

| requester | modes it requests |
|---|---|
| `mode_start`, the mode 1 handler, the TT loader (stage 6), `pad_poll` (pad 1 reset) | 0 (boot mode) |
| `pad_poll` (pad 2 reset), ETC (3 sites) | 0x11 (opening) |
| `game_reset_to_title`, boot mode, ETC (4 sites), NAME_ENT, OPTION, TACO (on leaving) | 0x12 (title) |
| `mode_title` | 0x11, 0x13 (item 0), 0x20 (item 1), 0x21 and 0x22 (debug) |
| NAME_ENT | 0x10 (error path), 0x12, 0x14 (11 sites), 0x15, 0x34 (game start or loaded game) |
| OPTION | 0x12, 0x21, 0x22, 0x34; its debug page starts scenes by hand (0x50-0x5A, 0x5D, 0x5E, 0x63, 0x70-0x72, 0x90, 0x91, 0xD0) |
| command screen and system menus (`8005A0B0.c`) | 0x14, 0x30 (next week), 0x31, 0x34, 0x3A (help), 0x52 and 0x53 (New Year prep) |
| scheduler (`8006CB30.c`, the queue-type table) | 0x22, 0x31, 0x32, 0x34-0x39, 0x40-0x44, 0x46, 0x51, 0x54-0x5B, 0x5D, 0x5F, 0x61-0x63, 0x70-0x72, 0x74-0x76 |
| school-day loop (`800737A0.c`) | 0x31 |
| holiday loop (`80075320.c`) | 0x45, 0x50, 0x5D, 0x73 |
| TEL | 0x31, 0x34, 0x46 |
| OLH | 0x34 |
| DATE | 0x50, 0x5C, 0x5E, 0x81, 0x82; DATE2: 0x50 |
| GEKO | 0x52, 0x81; SHOUGATU: 0x56, 0x57; SHUGAKU: 0x81 |
| BUNKASAI, BUNKAKEN | 0x80 (stage shows, 113 sites), 0xD0 (TT game exhibit, 10 sites); BUNKA_SD returns with 0x70 or 0x71 |
| MASTER | 0x90 (shooter) |
| ENDING | 0xC2; ETC: 0x91, 0xC3, 0xC4 |

## 4. Input

`pad_update` runs at the end of `frame_render`; the movie player (`movie_play`), the VRAM view function and the TT pad reader (`tt_pad_update`) also call it. If `pad_device_detect` reports a change this frame, nothing else runs. Otherwise `mouse_to_pad` (mouse) or `pad_poll` (pad) runs, then: the turbo is cancelled by any pressed button except circle (mask `PAD_MASK_TURBO_CANCEL`), enabled while pad 2 holds triangle, circle, cross and square together, and pad 2 START flips `sd_anim_fast`.

**Device** (`pad_device_detect`, port 1 buffer `pad_buffer_1`): status byte 0 and id high nibble 1 -> mouse (and `menu_mode` := 0); status 0 otherwise -> pad; a bad status -> none (0xFF), except a 0xFF status while a card job is busy, which keeps the old device (the BIOS cannot read the port during card access). A change re-initialises the drivers (`pad_driver_init`, `mouse_driver_init`), clears the state (`pad_state_clear`) and returns 1. `INPUT_PAD` is the default.

**Pad word.** `pad_raw_read` builds one 32-bit word: pad 2 in the upper half, pad 1 in the lower, each (direction/system byte << 8) | button byte, all inverted so a pressed button is 1. `PadBits`: low byte L2, R2, L1, R1, triangle, circle, cross, square (0x01-0x80); second byte SELECT, L3, R3, START, up, right, down, left (0x100-0x8000). `pad_raw_read` also has a mouse branch (the two mouse buttons as 0x20 and 0x40) that `pad_poll` never reaches, because mouse mode uses `mouse_to_pad`; with no device (0xFF) the C has no return statement on that path, which `pad_poll` cannot reach either. `pad_poll`:

1. `pad_prev` := `pad_held`; `pad_held` := the word. A half-word with all 16 buttons set cannot be a real input (a port read glitch): an upper half of 0xFFFF is dropped, a lower half of 0xFFFF with a real upper part keeps only the upper part, both give 0.
2. `pad_pressed` := `~pad_prev & pad_held`, `pad_released` := `pad_prev & ~pad_held`.
3. Turbo: while `pad_turbo` is set a circle press is added to `pad_pressed` on every odd `frame_count`.
4. Pause: a START press of either pad toggles `paused` (and `actors_draw_enabled`) only while `pause_allowed` is 1; no code ever sets `pause_allowed`, so the pause never happens in the shipped game.
5. Soft reset: outside modes 0x20, 0x90 and 0xD0, and only with a pad as the device, holding SELECT+START+L1+R1 of pad 1 counts `pad_reset_frames`; the reset runs on the frame where the counter already holds `PAD_RESET_HOLD_FRAMES` (0x81), the 130th frame of the hold, and `pad_poll` returns -1 (the caller ignores it). The same four buttons on pad 2 do the same but enter mode 0x11 instead of 0. Any other state clears the counter.
6. `pad_code_update` (below).

**Mouse** (`mouse_to_pad`): the two motion bytes times `cursor_speed` / 3 move `mouse_x` (clamped -0x96 to 0x8C) and `mouse_y` (-0x64 to 0x64), which are copied to the cursor actor (`cursor_x`, `cursor_y`). The buttons: `mouse_held` is the inverted port byte 3, `mouse_buttons` (core) the newly pressed ones. Buttons map to the pad word as circle (0x20, confirm) and cross (0x40, cancel): with `confirm_button` 0 the left button (8) confirms and the right (4) cancels, with 1 the opposite. With `click_mode` 0 a button maps on every frame it is held. With a non-zero `click_mode` (double click) the first press of the right or left button starts `click_timer` = `double_click_interval` + 8 and remembers the button in `click_button`; a second press of the same button before the timer runs out maps for that one frame; nothing else maps. A digital pad in port 2 (id 0x4x) still adds its buttons to the upper half. The press and release words are derived the same way as for the pad.

**The secret code** (`pad_code_update`, pad mode only, every frame, not during a reset): `pad_held` must equal `pad_code_table[pad_code_index]` exactly; a match advances the index and sets `pad_code_timer` to 0x10, which the same call decrements, so the next step has to match within 15 frames or the index resets. The table is up, release, up, release, down, release, down, release, left, release, right, release, left, release, right, release, cross, circle (18 steps, so cross has to be replaced by circle with no frame of nothing between). Entering it sets `pad_code_entered`; it stays set for 241 frames counting the frame of the last match (240 more frames: the timer, 0x0F after the final decrement, counts up and clears the flag when it wraps). Only `mode_title` reads it (section 5.4). Emulator: the sequence typed at the title menu enters mode 0x11 at stage 1 on the frame of the circle.

## 5. The title screen (mode 0x12)

`mode_title` adds 1 to `stage_timer` and runs the handler of the current stage; a stage of 4 or more is the error arm (`game_reset_to_title`). Handlers add 1 to `step_timer` (the title uses it as the stage clock).

| stage | handler | steps |
|---|---|---|
| 0 `TITLE_STAGE_SETUP` | `title_setup_stage` | substeps (not steps) 0-3 below |
| 1 `TITLE_STAGE_INTRO` | `title_intro_stage` | 0 backdrop, 1 flash, 2 logo, with `title_photos_update` each frame |
| 2 `TITLE_STAGE_MENU` | `title_menu_stage` | 0 setup, 1 select, 2 confirm |
| 3 `TITLE_STAGE_EXIT` | `title_exit_stage` | 0 fade, 1 dispatch |

**Stage 0** (`title_setup_stage`, 242 frames in the emulator): substep 0 resets the screen state, stops sound and starts loading a sound bank; 1, when the bank is ready, starts the title music and reads one sector of model data (LBA 0xBE13) to 0x801F0000; 2 copies that sector into the next twelve 0x800-byte slots and reads 0xD6 sectors (LBA 0xA0CC) to 0x80180000; 3 uploads the palettes, 13 photo textures (tpage ids 0x15-0x1A, 0x25-0x27, 0x35-0x38, 0x4000 bytes each), the background blocks, sets up actor 96, the 3D view, the three lights, the 16 coordinate records and the model links, seeds `srand` with `timer_ticks` and ends the stage. `pad_code_index`, `pad_code_entered` and `pad_code_timer` are cleared in substep 0.

**Stage 1, the intro.** The scene is a still background picture and 13 photographs (13 textured models) flying through the 3D view (emulator screenshot of this stage). Step 0 raises `screen_brightness` by 1 per frame to 0x80 (and the background layer tint with it) and waits 601 frames (`TITLE_WAIT_FRAMES`, 0x259) for the photos. Step 1 raises a flash level (`screen_flash_gray(step_timer / 2)`) to 0xFF over 510 frames; at 0xFF it hides the background and the photos, sets the clear colour to white and shows the logo picture as actor 96. Step 2 lowers the flash level over 255 frames, revealing the logo. START, cross or circle in step 0 hides the photos, sets a white clear colour and jumps to stage 2; in step 1 they finish the step and jump to stage 2. Emulator, from the title start: stage 1 at frame 242, step 1 at 843, step 2 at 1353, stage 2 at 1608.

`title_photos_update` (every frame of stage 1): photo i (0 to 12) spins from `frame_count` with a speed that depends on `i & 3` and sits at x = `scene_work[i + 1]` (start -400 - 100 * i from `title_photo_offsets`), which grows by 2 or 3 per frame; y = a sine wobble minus x / 2, z = -4 * x - 400. Nothing wraps; the intro ends before the photos leave the scene. `title_models_link` links record i to TMD object (rand() + i) % 13, so the order of the 13 pictures changes with every start.

**Stage 2, the menu.** Two items in one menu (`menu_set(0, 2, ...)`: item rectangles x -0x30, width 0xA0, height 0x14 at y 0x42 and 0x52), selection in `scene_work[0]` (0 = `TITLE_ITEM_GAME_START`, 1 = `TITLE_ITEM_OPTIONS`), the white clear colour, the pointer actor and two label actors (97, 98). In pad mode, up selects item 0 and down item 1 (the pointer sits at y = item * 16 + 0x46); in mouse mode the item under the pointer is selected; each up or down press restarts `step_timer`, which is the idle clock, even when the selection does not change (in mouse mode it restarts on every frame the pointer rests on an item). Step 1: START or circle hides the pointer, plays sound 0x501 and goes to step 2; with 601 frames (`TITLE_WAIT_FRAMES`) without an up or down press (mouse mode: without the pointer resting on an item), or SELECT, the actors are cleared, `SND_CMD_STOP_ALL` is sent and `main_state_set(0x11)` starts the opening (NAME_ENT, which ends by returning to the title). Step 2 highlights the chosen item for 0x21 frames, then stage 3. Emulator: the idle timeout fires 601 frames after the menu appeared, the overlay loads, and mode 0x11 runs at frame 2270.

**Stage 3, the exit.** Step 0: `SND_CMD_FADE_BGM_SLOW` every frame, flash level `step_timer * 2`; at 0x80 frames the actors, pointer and sprite tables are cleared. Step 1: full flash level, then item 0 -> `main_state_set(0x13)`, item 1 -> `main_state_set(0x20)`; with `debug_hud_enabled` set and item 1 chosen, pad right held gives mode 0x21 and pad left held mode 0x22; any other value -> `game_reset_to_title`. Emulator: choosing item 0 at frame 10 calls `main_state_set(0x13)` at frame 172 (0x21 + 0x80 + 1 frames after the press); NAME_ENT was already resident in that run, so the loader skipped the read and mode 0x13 ran from frame 174.

**The pad code.** From stage 2 on, while `pad_code_entered` is set and `cd_queue_idle()` is true, the title hides the pointer, sends `SND_CMD_STOP_ALL`, calls `main_state_set(0x11)`, `main_stage_set(1)` and plays sound 0x501. Stage 1 of mode 0x11 is the opening movie (`movie_play(0, 0x33B, 0x820)` in NAME_ENT), so the code skips the prologue lines and plays the movie (HYPOTHESIS: a viewing shortcut). The NAME_ENT movie code clears the code state afterwards and returns with `main_state_set(0x12)`.

## 6. The memory card

### 6.1 Layers

1. BIOS events (O.BIN names, kept): `card_ev_set` opens the software events (`Sw_Start`: done, error, timeout, new card, class 0xF4000001) and the hardware events (`Hw_Start`, class 0xF0000011), inits and starts the card driver and turns the BIOS auto-detect off. `Sw_Test` polls the four software events and stores 1, 2, 4 or 8 in `card_result` (0 when none), clearing them; `Hw_Test` spins until a hardware event fires and stores 0x81, 0x82, 0x84 or 0x88.
2. File jobs: a caller runs `card_start()`, then calls one poll function every frame until `card_result` shows the expected code, handling the timeout itself, then `card_stop()`. Each poll call adds one to `card_timer`; callers end the job when it is above 0x200 (`CARD_TIMEOUT_FRAMES`) or when the result is 0xF0 (`CARD_RESULT_IO_ERROR`, HYPOTHESIS: the name; no code sets that result, see 6.2). `card_busy` is 1 while a job runs and also keeps `pad_device_detect` from dropping the pad.
3. Images: `system_bank_build/load` and `game_bank_build/load` convert between the images and `system_data`/`game_state`.

### 6.2 Poll functions

| function | job | steps (`card_step`) | final `card_result` |
|---|---|---|---|
| `card_check_poll` | is a formatted card present | 0 info, 1 wait, 2 load, 3 wait, 4 done; a new-card event at step 1 goes to 0x20 (wait a frame, clear the BIOS state, spin on the hardware event; new card -> 0x30 else 0), 0x30 reload, 0x31 wait (a new card here goes to 0x40), 0x32 done | 1 `CARD_RESULT_OK`, 0xF1 unformatted, 0xFF no card |
| `card_scan_poll` | which files exist | 0 system file (3 tries), 1-14 each save file into `slot_status` | 2 |
| `card_system_read_poll` | read file 0 into `card_block` | 0 `card_file_read(0)`, 1 `card_status_wait`, 2 done | 3, or 0xD0 when the read cannot start |
| `card_system_write_poll` | write file 0 | 0 `card_file_write(0)`, 1 wait, 2 done | 0x13, or 0xD0 |
| `card_save_read_poll` | read file `save_slot` | same shape | 4, or 0xD0 |
| `card_save_write_poll` | write file `save_slot` | same shape | 0x14, or 0xD0 |

`card_event_handle` is the event step of the check: done -> next step, new card -> the step it is given, timeout -> `card_retry_count` + 1 and step 0 (at 3 retries: `card_busy` 0 and result 0xFF), error -> step 0. `card_status_wait` spins on the BIOS status until it is not 0, closes the file, and counts a retry and restarts the job for the status codes 0x11 and 0x21. A job stepping to `card_step` 16 would end with 0xF0; nothing in `src/` sets that step. In the emulator (a blank card image, from the title menu with item 0 chosen at frame 10) the check from NAME_ENT starts at frame 395 and runs 76 poll calls, the scan follows at frame 473, and after 18 poll calls (3 for the missing system file, 14 for the slots, 1 to finish) it ends with result 2 and every `slot_status` 0 (no file); `card_retry_count` stays 3 from the system-file test.

Callers: NAME_ENT (file screens, new game, saves), the command screen's data page (save and load; the m-sysmenu spec, not merged yet), ETC (end of game save), OPTION and TACO (settings), the TT loader, and `card_save_poll` of the scheduler.

### 6.3 Files, names, header

File 0 is the system file, files 1 to 14 are the save slots (the name builder accepts 0 to 15). The name is `card_file_prefix` (the device `bu00:` plus the product code of the original release, BISLPS-00065) plus one hex digit for the file number (0-9, A-F). A file is one block of 0x2000 bytes:

| offset | content |
|---|---|
| 0x00 | magic "SC", icon flag 0x13 (three animation frames), block count 1 |
| 0x04 | title field, 64 bytes: the game title, then the slot label (a numeral label from `month_label_strings`, 6 bytes) or the system-file label |
| 0x44 | 0x1C reserved bytes, zero |
| 0x60 | icon palette, 16 colours (`card_icon_palettes[icon]`) |
| 0x80 | icon frames, 0x180 bytes (`card_icon_images[icon]`) |
| 0x200 | bank 0 (0xF00 bytes) |
| 0x1100 | bank 1, an identical copy of bank 0 |

The icon is the girl of the latest date-like log entry (scene 0, 1, 2 or 0xF; `event_log_scan`), or icon 0 when there is none or the value is 13 or more: 13 icons. The writer (`card_file_write`) opens for writing and creates the file when that fails (16 tries each), builds header and bank, copies bank 0 over bank 1 and writes the 0x2000 bytes (16 tries). `card_file_read` clears the block, opens, and reads 0x2000 bytes (16 tries). `card_file_exists` opens a file once for writing and closes it (one open and up to 8 retries for a save file, up to 16 retries for the system file: 9 and 17 opens at most). `card_file_create` makes a one-block file, `card_file_delete` removes it (after a failed delete it retries while a counter stays below 3; the original reads that counter before setting it, so the number of tries is undefined); the save screens create then delete a file to test that it can be created.

### 6.4 Banks and validity

Each bank starts with a 32-bit checksum: the sum of the 0xEFC bytes after it, each as an unsigned byte. `card_bank_validate` checks banks 0 and 1 in that order; a bank is valid when its sum equals the stored word and at least one of its bytes is not zero. The first valid bank wins and its index is stored in `album_slot` (`unk_1117`, core's name; here it is a bank index, see open question 2). A load rejects a game bank with year 0 or month 0.

### 6.5 System file bank (`SystemSaveImage`, built from `system_data`)

| image offset | content (source in `system_data`) |
|---|---|
| 0x000 | checksum |
| 0x004 | `slot_status[16]` (0 no file, 1 file exists; the save and load screens also store 0xFF for a slot that is being written or whose read failed) |
| 0x014 | `flags[16]` (HYPOTHESIS: persistent per-girl "cleared" flags; core's `girl_cleared` is the per-save copy) |
| 0x024 | `karaoke_unlock_bits[2]` |
| 0x026 | `selected_slot`, stored modulo 15 when it is not 0 |
| 0x028 | the 0x14-byte record at `system_data` +0x2C: the TT game's record (rankings, button codes, result; its last byte is `extra_mode_flags`) |
| 0x03C | the 0x18-byte record at `system_data` +0x40: the TACO settings and rankings |
| 0x054 | `album_state[0..1]` |
| 0x056 | `album_entries` (0x800 bytes: 256 event-log entries) |
| 0x856 | `year[16]`, `month[16]`, `day[16]` of the 16 slot summaries |
| 0x886 | `family_name[16][8]`, `given_name[16][8]`, `nickname[16][16]` |
| 0xA86 | `club[16]` (the player's club id of each summary) |
| 0xA98 | `metrics[9][16]` (the nine parameter words per summary) |
| 0xCD8 | `slot_girl_marks[16][16]` |
| 0xDD8 | `layout_settings` (0x100 bytes: icon positions 0x40, mascot anchor 4, `mascot_choice`, mascot length and swing) |
| 0xED8 | 0x28 bytes unused |

`system_bank_build` first marks `slot_status[selected_slot]` = 1; it writes the layout block only when `options_dirty` is 0 and a game slot is selected. `system_bank_load` merges the image into memory: `slot_status`, `flags`, summaries and album are replaced; `karaoke_unlock_bits[0]` is OR-ed (byte 1 replaced); the TT record is replaced only while `extra_mode_flags` bit 0 is clear, the TACO record only while its saved flag is clear (so settings changed this session are not overwritten by an older card); then the layout block is pushed into the live icon positions, mascot anchor, `mascot_choice`, length and swing. `card_slot_summary_store` is the writer of one summary (after a save, for slot `selected_slot & 0xF`): the date, the three name buffers, the nine parameter words, girl record 15's 16 counter bytes, and the club (`player_flags` bits 12-15).

### 6.6 Game save bank (`GameSaveImage`, built from `game_state`)

| image offset | content |
|---|---|
| 0x000 | checksum |
| 0x004 | `vacation`, `year`, `month`, `day`, `weekday` (one byte each), 0x00A `day_count` (u16) |
| 0x00C | `month_calendar[2]` (0x80 bytes) |
| 0x08C | `profile`: GameState +0xC4 to +0x13F (names, flags, birthday, nine parameters; 0x7C bytes) |
| 0x108 | `girls[16]` (0x38 bytes each) |
| 0x488 | `girl_cleared[16]` |
| 0x498 | `appointments[8]` (words), 0x4B8 `clubs[12]` (words) |
| 0x4E8 | `event_flags` (0x100 bytes; the last four are `saved_settings`: [2] cursor style, [3] text speed, copied in just before saving) |
| 0x5E8 | `date_place_history[4]`, 0x5EC `girl_last_date_spot[16]`, 0x5FC `girl_last_date_spot_timer[16]`, 0x60C `girl_event_bits[16]` |
| 0x61C | `event_log[256]` (8 bytes each) |
| 0xE1C | `icon_mode`, `icon_count`, `menu_mode`, `sd_anim_fast`, 0xE20 `voice_mode`, 0xE24 `last_command`, 0xE28 `cursor_speed`, `click_mode`, `double_click_interval`, `confirm_button`, 0xE2C `quarter_flags` |
| 0xE30 | 0xD0 bytes unused |

Not saved: `current_girl`, `date_girl` and `event_log_count` (recomputed on load by `event_log_scan`: the count of log entries up to the first one with year 0, the girl of the latest entry with scene 0, 1, 2 or 0xF as `current_girl` (0xFF when none) and the girl of the latest entry as `date_girl`), the appointment places, `appointment_flag`, `saved_mode/stage/step`, the video settings. A load that succeeds restores text speed and cursor style from `saved_settings`.

### 6.7 What the port keeps

Everything a save contains is in the two tables above; the byte layout, the two banks, the checksum, the icon and the BIOS events are card specifics. A port needs: one system record (slot existence, per-girl cleared flags, the unlock bits, selected slot, TT and shooter records, the album, the 16 slot summaries, the layout settings) and 14 game records (the `GameSaveImage` fields, with the sums and banks replaced by whatever the port's storage guarantees). The slot summary exists so the file screens can list slots without reading every file; `slot_info_from_system_data` (HUD name, `D_800E8BEE`) says whether the summaries can be trusted. `card_state_reset` forgets all file statuses and clears that flag before a new round of card jobs.

### 6.8 The two MDEC still-image helpers

`mdec_still_buffers_set(vlc_buffer, out_buffer, bitstream)` stores the three buffers after resetting the decoder; `mdec_still_decode_to_vram(x, y, w, h)` checks the bitstream header (0x3800, version 1; else returns -1), VLC-decodes it and sends it to VRAM at (x, y) in 16-pixel-wide slices. Used by the decoded backgrounds (`dec_bg`, h-cdio), DATE, DATE2, ENDING, ETC, NAME_ENT and TACO. Hardware layer: a port replaces them by image loading.

## 7. Globals owned by this unit

| address | new name | role |
|---|---|---|
| `D_80125120`, `D_80125124` | `boot_gp`, `boot_prev_sp` | saved at start, never read |
| `D_8011ECA8` | `timer_event` | vblank event handle |
| `D_800E94A0`, `D_800FD4A0` | `gs_work_buffers`, `prim_arena_buffers` | per-buffer work areas set by `frame_begin` |
| `D_800E8C74`, `D_800E8C84`, `D_800E8C88`, `D_800E90A0` | `ot_table_0_tags`, `ot_table_1`, `ot_table_1_tags`, `ot_tags_1` | alias labels inside `ot_table` and `ot_tags` that `gpu_init` writes |
| `D_800B6D30` | `ovl_load_screen_mode` | loader backdrop |
| `D_800B3CA0` | `pad_turbo` | turbo flag |
| `D_800B3CA4` | `pad_reset_frames` | soft reset counter |
| `D_800B3CA8`, `D_800B3CF8`, `D_800B3CFC`, `D_800B3D00` | `pad_code_table`, `pad_code_index`, `pad_code_entered`, `pad_code_timer` | secret code |
| `D_800E8BF0`, `D_800E8C30` | `pad_buffer_1`, `pad_buffer_2` | raw controller buffers |
| `D_800B5A40` | `title_photo_offsets` | start x of the 13 photos |
| `D_800B3938`, `D_800B4390`, `D_800B58B0`, `D_800B58E4` | `card_file_prefix`, `card_icon_palettes`, `card_icon_images`, `card_fd` | card file name, icons, descriptor |
| `D_8011ECAC` to `D_8011ECC8` | `card_sw_event_*`, `card_hw_event_*` | the eight BIOS event handles (done, error, timeout, new card; software then hardware class) |
| `D_80123122` to `D_80123320` | `card_header_*`, `card_banks` | alias labels inside `card_block` |
| `D_800E7D13`, `D_800E7D15`, `D_800E7D16` | `system_data_plus3`, `save_slot_status_1`, `save_slot_status_2` | alias labels inside `system_data` |
| `D_800E7D4F` | `extra_mode_flags` | TT flags byte (HYPOTHESIS: name and extras meaning follow core) |
| `D_80125C04`, `D_80125C08`, `D_80125C0C` | `mdec_bitstream`, `mdec_vlc_buffer`, `mdec_out_buffer` | still-image decode buffers |
| `D_8011ED17`, `D_8011F553`, `D_80122D44` | `message_window_draw_flags`, `parameter_panel_draw_flags`, `scene_effect_flags` | read by the loader screen |

Fields renamed with this unit: `GameState`: `saved_settings` (`unk_668`), `click_timer`, `click_button`, `mouse_held`, `mouse_held_prev`; `system_data`: `karaoke_unlock_bits`, `album_state`, `album_entries`, `slot_girl_marks` (HYPOTHESIS), `layout_settings`; and all fields of the two save images. The type `SaveBank` becomes `CardBank`. Alias labels inside actors (`D_801206xx`, `D_80120651` ...) and inside `bg_layers` (`D_801220E0-E2`, the layer tint) that the title code writes are left for `actor_pool[i].field` access; they are not renamed.

## 8. How this connects

- Every scene overlay is entered through section 3.3 and returns through 3.5; `game_reset_to_title` is the universal error exit.
- The title's two exits are NAME_ENT and OPTION; its pad code and idle timeout lead to NAME_ENT's opening movie.
- The memory card is used by NAME_ENT, the command screen, ETC, OPTION, TACO and the TT loader; the HUD reads the slot summaries.
- The sound driver queries the mode through `mode_is_tt_game`, `mode_is_shooter`, `mode_is_option_2`.
- DATE and GEKO set `scene_effect_flags` for the loader screen; HUD actors' draw flags decide what the loader keeps drawing.

## 9. Port notes and quirks not to copy

What the port needs from this unit:

- The mode machine (`mode`, `stage`, `step`, `substep` with the reset matrix and the timers), one handler per mode or per scene; the table in 3.4 says which scene belongs to which mode. The loader itself, its LBA table, the checksum and the CD calls disappear. Two loader effects are game-visible: the captured-frame wipe for the seven modes of 3.3 and the persistence of an overlay's own statics while the same file stays resident (3.3); a scene that is entered twice in a row from the same overlay may rely on state it left behind.
- The boot order (start movie, boot data, title), the new-game data of 2.4 (this is the whole initial state of a game), and the title timings of section 5 (601-frame intro wait, 510-frame flash, 255-frame logo reveal, 601-frame idle timeout, 0x21-frame confirm highlight, 0x80-frame exit fade).
- Input semantics of section 4 (`pad_held`, `pad_pressed`, `pad_released`, turbo, the 130-frame reset hold, the 18-step code); a mouse is optional and maps to circle and cross.
- Save content of 6.5 and 6.6, with the slot summaries.

Quirks of the original that a port should not copy: `vram_clear_blocks` with 0 or 0xFE ends with a clear of a rectangle that was never filled (stack leftovers); a bad overlay checksum repeats the read forever; the card check spins on the hardware event in step 0x20 until it fires; `card_file_delete` starts its retry count from an uninitialised value; `restart_request`, the pause toggle (`pause_allowed`), `main_state_next`, `timer_seed_step`, `card_file_write_sync`, `card_system_file_probe` and `card_save_files_probe` are dead; `event_state_init` leaves `girl_last_date_spot_timer` and `girl_event_bits` untouched, so a soft reset keeps them while a full clear zeroes them.

## 10. Open questions

1. Core's `MainMode` rows for 0x74-0x76 have the entries 0x80132140, 0x80133A2C, 0x80139150; the loader (`func_80079070`) has 0x74 -> 0x80133A2C, 0x75 -> 0x80139150, 0x76 -> 0x80132140. The code decides; core's notes should be corrected.
2. `unk_1117` is named `album_slot` by core with the note "index of the selected album/save slot", but the memory-card code sets it to the bank (0 or 1) that passed `card_bank_validate`, and NAME_ENT indexes the loaded file image with it. A name like `card_bank` fits the use; this table keeps core's name.
3. `unk_F77` (`mouse_buttons` in core) holds the buttons pressed this frame, not the held ones (`mouse_held` is). `mouse_pressed` would say it.
4. m-sysmenu proposes `CardResult` (1, 0xD0, 0xF0, 0xF1, 0xFF) and a separate `CardPoll` (2, 3, 4, 0x13, 0x14) for the same field. This table puts all values in `CardResult` with the same names for the shared rows; the `CardPoll` rows should fold into it.
5. h-cdio and core name the mode 0 handler differently (`boot_mode_run`, `mode_boot`). `title_lights_init` overwrites the three light records that h-cdio names `light_0` to `light_2` with red, blue and green tints, so those names describe the other setup (`initLight`), not the title.
6. The ADR prefix rows `memory_card_` and `timer_` (and the GLOSSARY Module words row `timer`, which names 80053650) should be changed as in section 1. The GLOSSARY marks `card` as a trap word (memory card or karuta card) with the stem `memory_card`, while core's reviewed names use `card_`; renaming the whole `card_` family later is one table's work, if wanted. Words without a glossary or abbreviation row: `mdec`, `vlc` and `gs` (SDK terms: motion decoder, variable-length code, the graphics library) and `photo` (the pictures of the title intro; this unit avoids `card` for them).
7. `restart_request`, `pause_allowed` and modes 1, 3 and 0xFF are never written or requested by any C in `src/`, overlays included, and `main_state_next` and `timer_seed_step` have no caller: they look dead.
8. The meaning of `slot_girl_marks` (per-girl values 0 to 7 shown as icons in the file screens) and of the bits of `karaoke_unlock_bits` (no setter in `src/`) is open.
9. `card_step` 16 and result 0xF0 have no setter in this unit.
10. The 13 title photos: the code shows 13 textures and 13 TMD objects and the screenshot shows pictures of characters; which picture belongs to which texture slot was not traced.
11. `D_80122D44` is `scene_effect_flags` here and `date_light_effect_flags` in the o-date table (scope `main`). The loader code backs this name: bit 0 gates `light_effect_draw` and bit 1 gates `message_window_title_update` in `ovl_load_screen_draw`, and GEKO also writes the flag. The o-date note for bit 1 ("special date event running") does not match that use. An overlay table may not rename a main symbol, so at merge the m-flow name stays and the o-date row becomes a request or is dropped.
12. core, h-graphics and h-cdio still disagree on a few shared names: `func_80044750` (`snd_command` or `sound_request`), `D_8011ECA0` (`draw_buffer` or `draw_buffer_index`), `func_80043510` (`frame_render` or `render_frame`). This table follows core. About 15 data rows are alias labels named by offset (`system_data_plus3`, `save_slot_status_1/2`, `ot_table_0_tags`, `ot_table_1`, `ot_table_1_tags`, `ot_tags_1`, `card_header_*`, `message_window_draw_flags`, `parameter_panel_draw_flags`); they may be left as `D_` placeholders until the code indexes the parent array. `memory_card.c` still holds the two MDEC still-image helpers (6.8) and moves with the MDEC background module later.
