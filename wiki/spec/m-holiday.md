---
type: spec
updated: 2026-10-24
sources: ["src/main/80075320.c", "config/renames/proposals/m-holiday.tsv", "config/renames/proposals/core.tsv", "wiki/spec/core.md", "wiki/ref-english-patch.md", "wiki/data-types.md", "wiki/emulator.md", "asm/data/main/80075320.data.s"]
---

# Port spec: holiday screen, appointments and telephone (unit m-holiday)

Unit `m-holiday` of phase 2 ([[tickets/T-27013-p2-m-holiday]], parent [[tickets/T-27000-phase-2]]). It covers `src/main/80075320.c` (41 functions, one original object): the handler of main mode 0x46 `MODE_HOLIDAY`, the screen the game shows on every Sunday and national holiday. Names are the proposals in `config/renames/proposals/m-holiday.tsv` (140 rows: 28 `func`, 22 `data`, 4 `field`, 73 `const`, 12 `comment`, 1 `file`; 2 rows marked HYPOTHESIS). Core names (`wiki/spec/core.md`) are used as they are, and so are the names the units merged meanwhile gave to things this unit touches (m-hud, m-scheduler, m-dates-a, m-dates-b: `WeekdayCommand`, `command_roll_poor_result`, `command_girl_select`, `season_index`, `in_summer_months`, `magazine_mark_read`, `magazine_page_build`, `progress_flags_update`, `appointment_prompt_noop`, `icon_enabled`, `SND_CMD_*`); old placeholder names are given in backticks where a reader needs the link to the C. A claim of game meaning that the code does not prove is marked HYPOTHESIS. No game text is quoted: messages are described by what they say.

Scope is the port-first one of 2026-10-24: the game logic, state and flow in full; the hardware layer (CD reads, MDEC backgrounds, SD sprite sets, sound driver, text system) only by its entry points (section 11).

Evidence: the matched C and its callers and callees; the data tables decoded from `asm/data`; and an emulator session on a save state of the title menu (`tools/emu/run.sh shell` with a GDB script that stops at the main loop, forces `mode = 0x46` and pokes `pad_pressed`, `menu_cursor_0`/`menu_cursor_1`, the calendar bytes, the appointment words and the club and girl flags). Rows and sentences backed by it say "emu". The probes: icon availability at stage 0 (normal, appointment today, sick, remedial week); command 0 end to end; a phone-book call of the rival; the appointment question with two places; the button dial shortcut; the first club activity on a Sunday; the Sunday join list; the sports-club club day with the practice match. Save states and logs stay in the gitignored `build/emu/`.

## 1. What the unit does

On a holiday the player sees one picture (the player's room, one background per season) with the icon bar of the command screen, the parameter panel, the date panel and a one-line monologue. The player picks one icon:

- a command (icons 0 to 7): a short animation plays and the player's parameters change; the day ends;
- the club command (icon 4) has its own flows on Sundays: join a club when there is none, a menu "do the activity / leave the club" when there is one;
- the date icon (9): only with a date promise today; asks "where was it?" when several promises exist, then starts the date (mode 0x50);
- the phone icon (8): a phone book, or typing a girl's number with three pad buttons; hands over to the TEL scene (mode 0x45);
- the magazine icon (10), the calendar icon (12) and the system icon (11) open pages and come back;
- a special case replaces the whole screen: on a shared birthday the first frame starts a special event of girl 0 (mode 0x5D).

The unit also owns the rules that tie a Sunday to club life: club exp, the third-Sunday club day, the practice match and the first-club-activity scene.

## 2. Mode 0x46 and its stages

`mode_holiday` (`func_80075320`, core name) is one `switch` on `stage`; each stage function switches on `step`, and uses `substep` where noted. After the stage returns it always runs the common tail: `mascot_update` when actor 27 is visible (flag 0x80), `message_window_show`, `dec_bg_draw(0)`, `hizuke_show`, `parameter_show`, `cursor_update`, `screen_frame_draw(1)`.

| stage (`HolidayStage`) | function | steps | what |
|---|---|---|---|
| 0x00 `HOLIDAY_STAGE_OPEN` | `holiday_stage_open` (`func_80075468`) | 0 `holiday_open_init`, 1 `holiday_open_wait_bg` | build the screen, read the season background; then `main_stage_next` to stage 1 |
| 0x01 `HOLIDAY_STAGE_MENU` | `holiday_stage_menu` (`func_80075BE8`) | 0 `holiday_menu_frame` | per-frame input |
| 0x05 `HOLIDAY_STAGE_ACTIVITY` | `holiday_stage_activity` (`func_80076F48`) | 0 `holiday_activity_load` (substeps 0-3), 1 `holiday_activity_sd_setup`, 2 `holiday_activity_play` | play the chosen command |
| 0x06 `HOLIDAY_STAGE_CLUB_MENU` | `holiday_club` (O.BIN) | 0 `holiday_club_init`, 1 `holiday_club_select`, 2 `holiday_club_exit`, 3 `holiday_taibu_club_exit` | Sunday, in a club |
| 0x07 `HOLIDAY_STAGE_CLUB_JOIN` | `holiday_stage_club_join` (`func_8007894C`) | 0 `join_club_init`, 1 `join_club_select`, 2 `join_club_message`, 3 `holiday_club_join_exit` | Sunday, no club (the join steps are shared with mode 0x34 stage 5) |
| 0x10 `HOLIDAY_STAGE_TELEPHONE` | `holiday_tel` (O.BIN) | 0 `holiday_tel_init`, 1 `holiday_tel_call`, 2 `holiday_tel_exit` | phone book, dial tones, hand-over |
| 0x20 `HOLIDAY_STAGE_MAGAZINE` | `holiday_stage_magazine` (`func_80077BE0`) | 0 `magazine_init`, 1 `magazine_show`, 2 `magazine_exit` (O.BIN) | information magazine page |
| 0x30 `HOLIDAY_STAGE_CALENDAR` | `command_screen_stage_calendar` (`func_8005B798`, m-sysmenu) | 0 init, 1 wait for cross, 2 exit | this-month/next-month calendar page with the biorhythm graph, shared with mode 0x34 stage 2 |
| 0x40 `HOLIDAY_STAGE_SYSTEM` | `settings_stage` (`func_8005D9B4`, m-sysmenu) | 0 root init, 1 root menu, 0x10.. one range per setting | system settings, shared with mode 0x34 stage 4 |
| 0xFE `HOLIDAY_STAGE_APPOINTMENT` | `holiday_stage_appointment` (`func_80076EC0`) | 0 `appointment_start`, 1 `appointment_place_choose`, 2 `appointment_place_confirm`, 3 `appointment_launch` | the date promise of today |

Stages 0x30 and 0x40 return with `main_stage_set(0)` and `main_step_set(2)`; `holiday_stage_open` runs `holiday_open_init` for every step except 1, so the menu is rebuilt each time. Stage 5 uses `step_timer` as a frame counter that its dispatcher increments itself (`step_timer += 1` at the top of `holiday_stage_activity` and `holiday_tel`).

Entry: the day-event reader `day_event_dispatch` turns a queue record of type 0x10 (`DAY_EVENT_HOLIDAY`) into `main_state_set(0x46)`; the queue builder writes that record for every day whose `DayKind` is 1 (Sunday) or 2 (national holiday), unless a special event takes the day (core spec sections 3.4 and 3.5). The TEL scene also returns here (`main_state_set(0x46)`) when a girl does not answer.

Exits: `day_event_next(0)` after a finished activity (back to the day screen, next record); `main_state_set(0x50)` (DATE), `0x45` (TEL), `0x5D` (special event, GEKO), `0x60` (club scene, KANGEI), `0x73` (practice match, RENSYU).

## 3. State

### 3.1 GameState fields used (core names)

| field | role here |
|---|---|
| `weekday` (6 = Sunday), `month`, `day`, `year`, `day_count` | Sunday tests, the third-Sunday test `(day + 6) / 7 == 3` (days 15-21), season and warm-month tests, the 730-day limit of the club scene, date of the appointment lookup |
| `player_flags` (`PlayerFlagBits`) | `health` (3 = `HEALTH_SICK`), `club` (club id, 11 = none); bits 12-15 of the halfword |
| `player_flags` bit 16 = `player_flags_hi` bit 0 (`remedial_week`, m-scheduler name; the C reads it as `unk_0F6.h & 1`) | forced remedial week, section 4.1 |
| `player_birthday`, `girls[0].birthday` | the shared-birthday check (section 10) |
| `last_command` (`LastCommandBits`) | low nibble `weekday_command`, high nibble `holiday_command`: written by `holiday_menu_click` and `holiday_activity_load` |
| `day_events[day_event_index]` | the record of today: type 0x10; `girl` byte = chosen command, `scene` byte = poor-result flag (0 good, 1 poor) once a command is chosen; rewritten as `{0x80, girl, place}` by `appointment_launch` |
| `appointments[8]`, `next_appointment` (slot 0) | date promises, section 7 |
| `appt_place_0..3` (core names of `unk_724..727`) | chosen place, true place and decoys, section 7.4 |
| `appointment_flag`, `date_girl`, `current_girl` | set from slot 0 by the launch; `current_girl` is also the phone target |
| `clubs[11]` (`ClubRecord`): `exp`, flag byte +3 | `exp += 6` per club activity; flag `quit` (word bit 24, m-hud name); `club_scene_blocked` (bit 29, HYPOTHESIS) |
| `girls[i].flags` (`GirlFlagBits`) | `met` (b1), `acquainted` (b2, m-scheduler: set with b3, b6, b7 by the TEL profile page), `phone_known` (b3), `club` (f20), `club_scene_done` (b18, m-scheduler name) |
| `girls[i].tokimeki` word, `girls[i].affection` | +`rand() & 0x7FFF` on a button dial; +1 on a club command, section 4.5 and 8.2 |
| `param_*`, `param_stress` | `parameter_up_down` effects; stress +0x20 on a button dial in the remedial week |
| `progress_state[0x15 + command % 9]` | command use counters (+1 per chosen command) |
| `quarter_flags`, `progress_state` | magazine read marks, section 9 |
| `icon_mode`, `icon_count`, `sd_anim_fast`, `menu_cursor_0`, `menu_cursor_1`, `pad_pressed`, `pad_held`, `step_timer`, `substep` | input and UI state; `menu_cursor_0` = icon (or phone-book item) under the pointer, -1 none |

### 3.2 Data this unit owns (`.data` 0x800B6990-0x800B6D30)

| name (old) | type | content |
|---|---|---|
| `holiday_dial_keys` (`D_800B6990`) | u8[8] | last 7 dial keys, 0xFF empty |
| `holiday_dial_key_pos` (`D_800B6998`) | u8 | write position, wraps at 7 |
| `holiday_activity_bg_sectors` (`D_800B699C`) | s32[8] | background file per command: 0x3FFB (commands 0-2), 0x4003 (3), unused (4), 0x400C (5), 0x3F95 (6), 0x3F9B (7) |
| `appointment_choice_suffix` (`D_800B69BC`) | string | tail text of each place option |
| `holiday_sd_set_summer` (`D_800B69D0`) | u8[8] | SD set per command, months 6-9: 0,2,4,0xC,(0xC),0xA,8,6 |
| `holiday_sd_set_other` (`D_800B69D8`) | u8[8] | same, other months: 1,3,5,0xC,(0xC),0xB,9,7 |
| `holiday_sd_set_sectors` (`D_800B69E0`) | s32[13] | CD sector of SD set 0-12: 0xA1E6 + 0x29 n (n < 12), 0xA79E (set 12) |
| `magazine_hint_*` (`D_800B6A14`-`D_800B6CE0`) | 14 strings | the hints, section 9 |
| `magazine_hints` (`D_800B6CF8`) | u8 *[14] | pointer table in index order |

### 3.3 Data of neighbouring units that this unit reads

These live in the data piece 0x800B5950-0x800B6740 and next to `phone_numbers`; the m-weekday, m-flow and core tables name them, and the names are used here.

| placeholder | content |
|---|---|
| `room_bg_sectors_holiday` (`D_800B5960[4]`) | holiday room background per season (0 Mar-May, 1 Jun-Aug, 2 Sep-Nov, 3 Dec-Feb): 0x3FBE, 0x3FC6, 0x3FAD, 0x3FB6; the weekday twin is `D_800B5950` (0x3FE9, 0x3FF2, 0x3FD7, 0x3FE0) |
| `club_bg_sectors` (`D_800B6780[11]`) | club activity background per club id 0-10: 0x3F57, 0x3F60, 0x3F6A, 0x3F74, 0x3F7C, 0x3F84, 0x3F28, 0x3F31, 0x3F4E, 0x3F45, 0x3F3B |
| `club_sd_set_sectors` (`D_800B6810[32]`) | CD sector of the 32 club SD sets: 0x6CBA + 0x29 n |
| `club_sd_set_summer` (`D_800B6890[11][2]`), `club_sd_set_other` (`D_800B68A8[11][2]`) | club SD set id per club and column (months 6-9 / other months), section 5.3 |
| `D_800B3778[13][7]` (`phone_numbers`, core) | the number rows, section 8.2 |
| `D_800B342C[21]` (`place_name_table`, core) | place names, indexed by the raw place field of an appointment word |
| `D_800B3538[16]` (`girl_full_name_table`, core) | names shown in the phone book |
| `pad_code_entered` (`D_800B3CFC`, m-flow) | the secret-code latch written by `pad_code_update` (`func_80043448`, section 9) |
| `scene_work` (`D_801230D0`, m-weekday) | first word of the shared 0x40-byte work area: here the SD set id of the activity and the phone slot handed to TEL |
| `D_80125CA4..CB0` | SD sprite address block written by `addr_init_*_sd` (palette, frame table, sequence table, texture table) |

## 4. The holiday screen (stages 0 and 1)

### 4.1 Opening: `holiday_open_init`

In this order:

1. Clear the dial key ring (`holiday_dial_keys` = 0xFF x 7, position 0) and run `hizuke_init`.
2. If `holiday_birthday_event_due()` (section 10): set `dialogue_work` scene 8 and girl 0, `parameter_show_init` twice, `main_state_set(0x5D)`, and leave.
3. Clear sprite tables, switch the background clear on, clear the texture cache and the text system, set the title, `icon_count = 14`, create the icon actors and the 14-item icon menu, run `holiday_icons_set_available` (4.3), set the speaker label to the player.
4. Pick the monologue and the icon limits, first match wins:
   - `remedial_week`: re-create the icons; every command, phone, date and magazine icon is off and then icons 0, 1, 2 are turned on again; icon 13 stays off. System and calendar stay on. Message: a forced remedial week has to be done.
   - `health == HEALTH_SICK` (3): re-create the icons; all commands and phone, date and magazine are off, icon 7 (rest) is turned on, the cursor `menu_cursor_0` starts on 7. Message: too ill to move.
   - otherwise (`holiday_icons_set_available` stays in force): if `appointment_find_today` > 0, line 1 is "a date with <girl of slot 0> today" and the cursor starts on icon 9; if the player is in a sports club (ids 6-10, `player_in_sports_club`, `func_8005352C`) and it is the third Sunday (`(day + 6) / 7 == 3`, `weekday == 6`) a second line says it is the club's practice match day (`month % 3 == 0`) or practice day (other months) and the cursor moves to icon 4; with neither, the line asks what to do today.
5. Start the text, clear the upper actors, init the cursor actor, `parameter_show_init` twice, `message_window_init`, and read the season background `D_800B5960[season]` with `dec_bg_cd_read` (season index: months 3-5 give 0, 6-8 give 1, 9-11 give 2, 12, 1 and 2 give 3). A cached slot (result 0/1) shows it at once, sets brightness 0x80 and does `main_stage_next` (stage 1); result -1 (read started) does `main_step_next` to `holiday_open_wait_bg`, which waits for `cd_read_sync_status & 1`, decodes (`dec_bg_decode_to_vram`), re-reads on decode result -1, then does the stage change.

Emu: after forcing mode 0x46 on 1995-04-04 (a Tuesday, no appointment, no club) the init calls `icon_can_use_set(9, 0)`, `(0xD, 0)` and `(4, 0)` in that order. With a promise in slot 0 for that day the calls are `(0xD, 0)` and `(4, 0)` only and the cursor starts on icon 9. Sick (health 3): after those three, `(10, 0)`, `(8, 0)`, `(9, 0)`, `(4, 0)`, `(0..7, 0)`, `(0xD, 0)` and `(7, 1)`, cursor on 7. Remedial week: the same up to `(0xD, 0)`, then `(0, 1)`, `(1, 1)`, `(2, 1)`, cursor unchanged.

### 4.2 Per-frame handling: `holiday_menu_frame`

`k_disp_inc` (text), `cursor_update`, then the handlers in priority order, the first that returns nonzero ends the frame:

1. `holiday_menu_click`: confirm on an icon (4.5);
2. `holiday_phone_dial_shortcut`: button dialing (8.2);
3. `room_click_radio_holiday` (`func_8005B0F4`, m-sysmenu name; HYPOTHESIS there: the object is a radio): a confirm press inside the screen rectangle x -0x27..7, y 0x19..0x23 plays a voice clip: `snd_command(SND_CMD_FADE_BGM_SLOW)` (0xD1), `bgm_stop(0)`, then the clip pair `room_radio_clips_holiday[day_count % 8]` (speaker 0xE) and `SND_CLASS_VOICE` (0x300) starts it. It has no other caller;
4. `room_click_mascot_holiday` (`func_8005B1A8`, m-sysmenu name): a confirm press on the mascot actor (actor 27: pointer x within 12 of the mascot's x, y from the mascot's y to 0x30 below it; at least 0xF1 frames after the stamp `room_mascot_holiday_stamp`, which is never written) sets `current_girl` to the mascot's girl and plays that girl's clip (`girl_voice_clips_by_state[girl][variant]`, variant 1 when `girl_state(girl) & 0x80`, i.e. affection at least 80) plus `SND_CLASS_VOICE`.

The picture behind the icons is the player's room (m-weekday name `room_bg_sectors_holiday`); the weekday screen has the same picture with its own tables and the same click spots.

### 4.3 Which icons are usable (`holiday_icons_set_available`)

`icon_can_use[i]` starts at 1 for every icon when the icons are created. Rules, in the normal case:

| icon | off when |
|---|---|
| 9 appointment | `appointment_find_today(year, month, day) <= 0` (no promise today, or one of the days it answers -1 for: December 24, January 1, from March 1998) |
| 13 | always on this screen (it belongs to the weekday screen) |
| 14 | `icon_mode == ICON_MODE_FREE` (free layout has an extra slot that this screen does not use) |
| 4 club | on a Sunday: all 11 club records have the `quit` flag set (nothing to join, no club to leave); other holidays: `player_flags.club == 11` |

The remedial-week and sick cases of 4.1 replace this list. Everything not named is on; the system (11) and calendar (12) icons are never switched off by this screen.

### 4.4 Commands and what makes a command

Icons 0 to 7 are the commands, the same eight as on the school-day screen (`WeekdayCommand`, m-scheduler names `COMMAND_*`): 0 liberal arts, 1 science, 2 art, 3 athletics, 4 club, 5 trivia, 6 looks, 7 rest. The names follow the row of the `command_deltas` table (each row raises the parameter of the same name; row 7 raises condition and lowers stress; row 4 is replaced by the row of the player's club in `club_activity_deltas`).

### 4.5 Choosing an icon: `holiday_menu_click`

Preconditions: not (`icon_mode == FREE` and `pad_held & 1`; that case returns 1 and leaves the input to the icon layout editor of `icon_bar_update`). Acts on `pad_pressed & 0x20` (confirm) with `menu_cursor_0 >= 0`:

1. Common to any hit: `cd_audio_command(9)`, `snd_command(SND_CMD_XA_FORGET)` (3: stop the voice clip and clear its state, m-dates-a name), and when no BGM is playing `snd_bank_start(0, 0)` (restarts the BGM that a voice-clip click faded out; the `bgm_is_playing` test is the evidence). The handler returns 1.
2. By icon (the icon must be on, `get_icon_can_use`, except 11 and 12):
   - 0-3 and 5-7: `holiday_command = icon`; `snd_command(0x500)`; record `girl = icon`, `scene = command_roll_poor_result(1)` (the poor-result roll, 4.6); `main_stage_set(5)`. Emu: icon 0 gives sounds 3 and 0x500 then `main_stage_set(5)`.
   - 4, not a Sunday (a national holiday with a club): `girls[girl_find_by_club(club)].affection += 1` when that girl is `acquainted`; then as above with stage 5.
   - 4, Sunday, `club == 11`: record as above; `main_stage_set(7)` (join list). The icon's own affection bonus is skipped.
   - 4, Sunday, in a club: `affection += 1` as above, record as above, `main_stage_set(6)` (club menu). Emu: confirmed.
   - 9: needs `appointment_find_today > 0`; `holiday_command = 9`; `snd 0x500`; `main_stage_set(0xFE)`.
   - 8 phone: `holiday_command = 8`; `snd 0x50D`; `main_stage_set(0x10)`.
   - 10 magazine: `snd 0x50D`; `main_stage_set(0x20)`.
   - 12 calendar: `snd 0x50D`; `main_stage_set(0x30)`. 11 system: `snd 0x50D`; `main_stage_set(0x40)`.

`girl_find_by_club(club)` (`func_80053564`, m-menutext name) returns the first heroine 0-10 whose club field equals the club, or -1.

### 4.6 The poor-result roll (`command_roll_poor_result`, `func_8006BDA8`, m-hud)

Run once, when the command is chosen, with day offset 1 (tomorrow's biorhythm). Summary for the port: `score = biorhythm(selected waves, scale 16) + parameter / 16` (club: club exp / 16 instead of the parameter; rest: fixed 16), `+ 6`; `score = rand() % (score + 4)`; stress factor `f = 4` when stress >= 97 else `stress / 32`; `score = (8 - f) * score / 8`; if the three waves at scale 3 are all bad, `score /= 4`; result 0 (good) when `score >= 4`, else 1 (poor). The three waves are sines with periods of about 23, 28 and 33 days over `day_count` plus a per-player offset (`func_8006C700`). The result (`COMMAND_RESULT_NORMAL` 0 or `COMMAND_RESULT_POOR` 1, m-weekday names) is stored in the record and drives the animation frame, the sound and the size of the gains.

## 5. Playing a command (stage 5)

### 5.1 Load (`holiday_activity_load`, step 0, by substep)

Always: `text_system_reset`, hide the message-window title.

- Substep 0: icons: only the chosen one stays on (`icon_can_use_set(i, i == command)` for i < 14). `holiday_command` is rewritten from the record. `progress_state[0x15 + command % 9] += 1`. For command 4 with a club (`club < 11`): `clubs[club].exp += 6` (`CLUB_ACTIVITY_EXP`). Then `holiday_activity_bg_read`: season background when sick, else `holiday_activity_bg_sector()`. Result -1: substep 1; result 0/1: show it, substep 2.
- Substep 1: wait for the read, decode, retry on -1 (as 4.1), then substep 2.
- Substep 2: `holiday_activity_pick_sd_set` (5.3), then the SD animation file: sick and not `remedial_week`: 9 sectors from 0x7D8A (`SD_SICK_BED_PACK_SECTORS`, `SD_SICK_BED_PACK_LBA`, m-weekday names); command other than 4: 0x29 sectors from `holiday_sd_set_sectors[set]`; command 4: 0x29 sectors (`SD_PACK_SECTORS`) from `club_sd_set_sectors[set]`; to 0x801E0000. Substep 3.
- Substep 3: wait for the CD, `main_step_next`.

Emu (command 0, April, not sick): `dec_bg_cd_read(0x3FFB, 0)`, `holiday_activity_pick_sd_set`, `cd_read_sectors(0x29, 0x801E0000, 0xA20F)` (set 1).

The background of the activity (`holiday_activity_bg_sector`, `func_80075FA0`): `holiday_activity_bg_sectors[command]` for commands other than 4, `club_bg_sectors[club]` for command 4.

### 5.2 Set-up (`holiday_activity_sd_setup`, step 1)

Clears actor 96, then, with `poor = record.scene`:

- sick and not `remedial_week`: `addr_init_else_sd(2)` (a set for the sick-in-bed animation) and `poor = 0`;
- command 4: `addr_init_club_sd(set)`; when `command_girl_select(4) != COMPANION_NONE` (0xF, m-weekday name; some girl takes part, see 5.3) a second figure is put on actor 97 at (-0x18, -0x40) with frame `poor + 2`;
- otherwise: sets 6 and 7 (rest) force `record.scene = 0` and `poor = 0`; `addr_init_holiday_sd(set)`.

Then `load_palette`, and the main figure on actor 96 at (-0x50, -0x40), frame = `poor`, animation flag 0x82 instead of the default when `sd_anim_fast` is set. `main_step_next`. Emu: `addr_init_holiday_sd(1)`.

### 5.3 Choosing the SD set (`holiday_activity_pick_sd_set`)

`scene_work = 0xFF` (`D_801230D0`), then by command:

- not command 4: `set = holiday_sd_set_summer[command]` when `in_summer_months()` (`func_80066A84`, months 6-9) else `holiday_sd_set_other[command]`: commands 0, 1, 2 give pairs (0,1), (2,3), (4,5), command 3 gives 12 in every month, 5 gives (0xA, 0xB), 6 gives (8, 9), 7 gives (6, 7); named by `HolidaySdSet`.
- command 4: `col = 0` when `command_girl_select(4) == 0`, else `1`; `set = club_sd_set_summer[club][col]` in months 6-9, `club_sd_set_other[club][col]` otherwise. Per club (summer column 0 / 1 | other-months column 0 / 1): literature 0,2 | 1,3; drama 0x1E,0x1F | same; science 4,6 | 5,7; computer 8,0xA | 9,0xB; art 0x1A,0x1C | 0x1B,0x1D; brass band 0x16,0x18 | 0x17,0x19; baseball 0xC,0xD; soccer 0xE,0xF; tennis 0x14,0x15; swimming 0x12,0x13; basketball 0x10,0x11 (the last five are the same in both seasons).

`command_girl_select(4)` (`func_8006F524`, m-scheduler) returns which girl takes part in the club activity: girl 0 when she and the player share the club and she is acquainted, another heroine id when she is the best candidate by the point rule, 0xF for nobody. Column 0 therefore means "girl 0 is there"; any partner (value other than 0xF) also adds the second figure of 5.2.

### 5.4 Running (`holiday_activity_play`, step 2)

`step_timer` starts at 1 on the first call (the dispatcher adds 1 before the step).

- At `step_timer == 0x20`: if the main figure's frame bit 0 is 0, `snd_command(0x501)` (`UI_SE_ACTIVITY_GOOD`), else `0x502` (`UI_SE_ACTIVITY_POOR`); then `parameter_up_down(command, 1, record.scene)`; `parameter_show_init`. Return.
- While `step_timer <= 0x40`: nothing.
- After: clear actors 96 and 97. If `weekday == 6` and `record.girl == 4` and `club != 11` and `clubs[club].exp == 6` (this was the first club activity of that club): `girl = girl_find_by_club(club)`; `date_girl = girl` (assigned before the tests, whatever they give); with `girl != -1` and `day_count < 0x2DA` (730) and `girl.met` and `girl.acquainted` and not `club.club_scene_blocked` and not `girl.club_scene_done`: set `girl.club_scene_done`, `main_state_set(0x60, club)` and return. (The test for club 10 runs twice, once under `girl == 0`; both copies are identical.) Emu (Sunday, club 0, girl 0 met, acquainted and in club 0, exp 0 before): the activity ends with `main_state_set(0x60, 0)` and flag bit 18 set on the girl; with no girl in the club the same activity ends in `day_event_next` instead.
- Otherwise `day_event_next(0)`.

Emu timing: sound 0x501 at +32 frames, `parameter_up_down(0, 1, 0)` in the same frame, `day_event_next` at +65.

### 5.5 Effects (`parameter_up_down`, weekday unit)

`parameter_up_down(command, holiday = 1, poor)` gives each parameter a delta row (`command_deltas[command]`, or `club_activity_deltas[club]` for 4), in 1/16 point units. Positive deltas are multiplied by `4 / (poor ? 2 : 1) / (sick ? 2 : 1) / (injured or neurotic and command not in {5, 6, 7} ? 2 : 1)`; negative deltas are applied as they are (1x). Stress is the exception in sign: its negative deltas are the good ones and are scaled like gains; a poor result adds one stress point. So a holiday command pays four school-day gains for one school-day cost. Commands 5, 6 and 7 are not reduced by the injured or neurotic penalty, and the roll of command 7 is forced good by 5.2.

## 6. Sunday club flow

### 6.1 Join list (stage 7)

Sunday and `player_flags.club == 11` (no club), icon 4. `join_club_init` (`club_list_menu`) shows the list of 11 clubs plus cancel; a club whose `quit` flag is set is hidden (its item has width 0) and the cursor starts on the first club still shown; `join_club_select`: cross returns to stage 0; confirm on cancel does the same; confirm on a club sets `player_flags.club = club` and goes on; `join_club_message` prints the joined message, and for clubs 6-10 (the sports clubs) a reminder that the third Sunday of every month is club day; `holiday_club_join_exit` clears the upper actors, shows the icons and does `main_stage_set(5)` (the activity with the command recorded at click time). Emu: weekday 6 with no club: icon 4 gives `main_stage_set(7)`; choosing club 3 stores 3 in the club nibble, `join_club_message` runs substeps 0, 1, 2 and 0xFF (no reminder below club 6), then `main_stage_set(5)`.

### 6.2 Club menu (stage 6)

Sunday, in a club, icon 4. Two choices (`x_taku` menu 1): do the club activity, leave the club. Cross returns to stage 0. Choice 0: step 2 `holiday_club_exit`: if the player is in a sports club (`player_in_sports_club`, ids 6-10) and it is the third Sunday and `month % 3 == 0`: `main_state_set(0x73)` (practice match, RENSYU); else `main_stage_set(5)` (the activity). Choice 1: step 3, the message "left the club", `clubs[club].quit = 1`, `player_flags.club = 11`, the text restarts; a press of confirm or cancel (`pad_pressed & 0x60`, `holiday_taibu_club_exit`) returns to stage 0. Emu: on 17 June (a Sunday, third week) with club 6 the menu cursor starts on icon 4, icon 4 gives `main_stage_set(6)`, choice 0 gives `main_state_set(0x73)`.

### 6.3 Club rules in one place

| rule | where |
|---|---|
| club exp +6 per club command | `holiday_activity_load` |
| first activity of a club (exp 6 after the add), on a Sunday, with the girl of that club met and acquainted: scene mode 0x60 | 5.4 |
| third Sunday (days 15-21) is club day for sports clubs; the opening line and the cursor point at icon 4 | 4.1 |
| practice match on the club day of March, June, September, December | 6.2 |
| skipping the club day: the day-end check of the queue reader (core spec) opens TEL stage 8, where the club captain expels the player (`quit` set, club 11) when the last holiday command is not 4 | `day_event_dispatch` (not in this unit) |
| `quit` on all 11 clubs: icon 4 off on Sundays | 4.3 |
| a club command raises the affection of the club's girl by 1 when she is acquainted | 4.5 |

## 7. Appointments (stage 0xFE)

### 7.1 The appointment word

`appointments[8]` are 32-bit words, sorted by date (the sort is `appointment_list_sort`, `func_8006B0C8`), filled from slot 0, an inactive word ends the list. Layout, from the TEL and GEKO writers and the readers here: bits 0-4 place (0-based `Place` id, 5 bits; the name table `place_name_table` is indexed by it directly, as TEL's place pages store it), bits 5-11 year (calendar year mod 100), bits 12-15 month (1-12), bits 16-20 day, bits 21-23 a value TEL stores in `D_80140560` and GEKO sets to 6 (meaning unknown), bit 24 `appointment_flag` (0 when the player arranged the date by phone in TEL, 1 when a girl invited the player in GEKO), bits 25-28 girl, bit 29 active, bits 30-31 zero.

`appointment_find_today(year, month, day)` returns 0 for no promise, -1 on December 24, January 1 and from March 1998 (the dates the game handles itself), otherwise the 1-based index of the last slot with equal month and day. Every user here takes slot 0 as today's promise.

### 7.2 Step 0: `appointment_start`

1. `appt_place_0..3 = 0xFF`, `appointment_prompt_noop()` (`func_8006B640`, an empty function).
2. `appointment_place_candidates(out)`: if slot 0's place is 16-20 (pool, sea, shrine, skating rink, ski resort) return 1; else count distinct places among the active slots (up to the first inactive one), with the places 16-20 left out; fewer than 2 distinct places: `out[0] = slot0.active` (1) and return 1; otherwise `out[]` = the first 3 distinct places in slot order and the count.
3. Count >= 2: `k_reset`, speaker label the player, the question "which place was the meeting?", `main_step_next` (step 1).
4. Count 1: `appt_place_0 = appt_place_1 = slot0.place`, `appt_place_2 = appt_place_3 = 0xFF`; record `{0x80, slot0.girl, slot0.place}` at the queue index; `current_girl = date_girl = slot0.girl`; `appointment_flag = slot0.flag`; `main_step_set(2)`.

### 7.3 Steps 1 and 2: the question

Step 1, by substep: 0 waits for the text (`xa_wait`); 1 builds the menu (`appointment_place_menu_build`); 2 reads menu 1.

`appointment_place_menu_build`: with 3 or more candidates: `r = rand() % 3`; options `c[r % 3]`, `c[(r+1) % 3]`, `c[(r+2) % 3]` (a random rotation of the first three), `appt_place_0 = 2`, a 3-item menu; with 2: `r = rand() & 1`, options `c[r]`, `c[1 - r]`, `appt_place_0 = 1`, a 2-item menu. Each item text is an opening quote mark, the place name and `appointment_choice_suffix`. The options are stored in `appt_place_1..3` (third 0xFF when 2). Emu: slots with places 3 and 5 gave `1,3,5,0xFF`.

Choice (confirm in menu 1): item 0 or 1: `appt_place_0 = appt_place_[1 + item]`; item 2: only when `appt_place_0 == 2`, `appt_place_0 = appt_place_3`; then `main_step_next`. Cross does not cancel this question. Emu: item 1 gave `5,3,5,0xFF`.

Step 2: a monologue naming the chosen place (`place_name_table[appt_place_0]`) and the girl of slot 0 (`get_g_name`), wait for a press, `main_step_next`.

### 7.4 Step 3: `appointment_launch`

`arr = (appt_place_1, appt_place_2, appt_place_3)`; `appt_place_1 = slot0.place` (the true place); the two other entries are refilled from the non-true entries of `arr` into `appt_place_2` and `appt_place_3` in order (so with two options the decoy lands in place 2 and 3 stays 0xFF); `appt_place_0` stays the place the player chose. Then the queue record `{0x80, slot0.girl, slot0.place}`, `current_girl = date_girl = slot0.girl`, `appointment_flag = slot0.flag`, `main_state_set(0x50)`.

Final layout (what DATE and the day-end check read): `appt_place_0` = chosen place, `appt_place_1` = true place, `appt_place_2/3` = decoys or 0xFF. DATE compares 0 with 1: equal means the player named the right place, different means the wrong place, and DATE then looks at 2 and 3 (the decoy places, 0xFF = none; HYPOTHESIS: the wrong-place scenes use them to say where the other girls wait). The queue reader's day-end check also compares the two. When only one place is pending, 0 = 1 and 2 = 3 = 0xFF.

The question is a memory test: the player must remember which place goes with the girl whose promise is due; the other options are the places of the other pending promises. It is skipped when only one place is pending or when the true place is one of the five seasonal places 16-20.

## 8. Telephone

### 8.1 Phone book (steps 0 and 1)

`holiday_tel_init`: clears text and the upper actors, hides the title and the icon bar, `telephone_class_init`, a monologue asking whom to call, `main_step_next`.

`telephone_class_init` builds menu 0 with 14 items in 3 columns of 5 (item `i`: x `(i / 5) * 0x58 - 0x88`, y `(i % 5) * 0x10 - 0x38`, size 0x48 x 0xE) under a heading, and the highlight actor 96:

| item | entry | shown when |
|---|---|---|
| 0 | `girl_full_name_table[13]` (Yoshio) | always |
| 1-11 | `girl_full_name_table[i - 1]` (girls 0-10) | `girls[i - 1].phone_known`, else its width is 0 (not selectable) |
| 12 | `girl_full_name_table[12]` (the rival, Rei) | always |
| 13 | cancel text, one row lower | always |

`holiday_tel_call` (step 1, per frame): `menu_check(0)`, bar, cursor, `k_disp_inc`. Cross: `main_stage_set(0)` (menu rebuilt). Confirm on an item with `menu_cursor_0 != -1`: item 13: `main_stage_set(0)`; otherwise `snd 0x50A` and `current_girl = 13` for item 0, `11` for item 12, `item - 1` for 1-11; `main_step_next`. Emu: item 12 gave `current_girl = 0xB`.

### 8.2 Numbers and the button dial shortcut

`phone_numbers[13][7]`: seven digits of 0, 1, 2 per row; the row is the girl id (rows 0-10), row 11 and row 12 are the same number. Digit 0 is the circle button, 1 cross, 2 triangle. Rows: 0 `0121020`, 1 `1120201`, 2 `2220010`, 3 `2101010`, 4 `2102211`, 5 `0001221`, 6 `2110121`, 7 `2210112`, 8 `0201202`, 9 `0122100`, 10 `0002210` (also used for Yoshio: `current_girl == 13` reads row 10), 11 and 12 `0210210`.

`holiday_phone_dial_shortcut` (every frame of the menu): only while the pointer is in the hot spot x -0x4F..-0x41, y -0xF..0x17 (the picture of the telephone, HYPOTHESIS). A new press of exactly one of circle, cross, triangle (`pad_pressed & 0x70` equal to 0x20, 0x40 or 0x10) appends key 0, 1 or 2 to `holiday_dial_keys` (position `pos`, then `pos++`, wrap at 7) and plays `UI_SE_DIAL_TONE_0 + key`. Then, on any of the three buttons, for each row `i` 0-12 the last 7 keys (oldest = `keys[pos]`) are compared with the row; on a full match and `girls[i].phone_known`:

- `i == 12`: `current_girl = 11`, `menu_cursor_0 = 12`, `scene_work = 11`; `i == 11`: nothing is set (the number is the same as row 12, so this only happens if girl 11 has `phone_known`); other `i`: `current_girl = scene_work = i`, `menu_cursor_0 = i + 1`;
- `girls[i].tokimeki word += rand() & 0x7FFF` (the record of the row, so Rei's own record for 12);
- during `remedial_week`: `param_stress += 0x20`;
- `main_stage_set(0x10)`, `main_step_set(2)`, `substep = 11` (the dial tones are skipped), then `text_system_reset`, `icon_disp_switch(0)`, title hidden, `telephone_class_init`, `k_sub_disp_start(0)`; return 1.

Emu: keys circle, cross, triangle, cross, circle, triangle, circle filled the ring as `0,1,2,1,0,2,0`, then `main_stage_set(0x10)`, `main_step_set(2)`, `main_state_set(0x45)`, `main_stage_set(1)`; the tokimeki word went from 0 to 0x2BCC; `current_girl = 0`, `menu_cursor_0 = 1`.

### 8.3 Dial tones and hand-over (`holiday_tel_exit`, step 2)

`step_timer` counts up. While `substep != 11`: only on frames with `(step_timer & 7) == 7`: substep 0 plays `UI_SE_DIAL_START` (0x504); substeps 4-10 play `UI_SE_DIAL_TONE_0 + digit k` (0x505 + k) of the row of `current_girl` (row 10 for Yoshio, 13), `k = substep - 4`; the substep increases by one on every such frame. So the number "dials" over about 88 frames.

At substep 11 (`HOLIDAY_DIAL_DONE_SUBSTEP`):

- `current_girl == 13` (Yoshio): `menu_cursor_0 = 0`, `k_sub_reset`, `restore_bgm`, `main_state_set(0x45)`, `main_stage_set(0)`;
- `current_girl == 11` (the rival's slot): `menu_cursor_0 = 12`, `scene_work = 11`, `main_state_set(0x45)`, `main_stage_set(1)`;
- other girl: `menu_cursor_0 = current_girl + 1`, `scene_work = current_girl`, `main_state_set(0x45)`, `main_stage_set(1)`.

Emu (item 12): sounds 0x504, then 0x505 0x507 0x506 0x505 0x507 0x506 0x505 at 8-frame spacing, then `main_state_set(0x45)`, `main_stage_set(1)` with `scene_work = 0xB`; TEL starts ringing (0x508 every 128 frames).

### 8.4 The slot alias of the rival

The value 11 in `current_girl` during the phone flow stands for girl 12 (Rei): the phone-book entry 12 shows entry 12 of the name table but stores 11; TEL's ringing step turns 0xB into its own girl value 0xC; TEL's list prints the name of girl 0xC for its item 11. Girl 11 (Miharu) has no phone-book item; her number row is a copy of the rival's. A port can keep the alias or give the rival her own id; the key point is that the TEL scene of stage 1 receives 11 and 12 as one person.

## 9. Magazine, calendar, system

`holiday_stage_magazine` shows the information magazine, a quarterly page (m-hud spec, section 14: each issue unlocks one new date place; eight of the thirteen issues carry a place).

- Step 0 `magazine_init`: `magazine_mark_read()` (`func_8006A044`, m-hud) sets the read bit of the current quarter in `quarter_flags` (bit `4 * (year - 95) + quarter`, quarters Feb-Apr, May-Jul, Aug-Oct, Nov-Jan with January counted in the previous year's last quarter; bit 15 also in May-July; bit 13 in the first month of a quarter, bit 14 in the others); the text is cleared, the title is set, `magazine_page_build()` (`func_8006A2CC`, m-hud) lays out the page of that issue and sets the issue's date-place flag when it applies, the icons and the date panel are hidden, `main_step_next`.
- Step 1 `magazine_show`: draws the page frame (`magazine_base_show`); substep 0 waits for a press (`xa_wait`); substep 1 continues only while the secret-code latch `pad_code_entered` is set, otherwise `main_step_next`; substep 2 prints hint `magazine_hint_index()` as the player's monologue; substep 3 waits for a press; substep 4 ends the step.
- Step 2 `magazine_exit`: `progress_flags_update()` (`func_8006D138`, m-scheduler: turns the read marks into the date-spot entries of `progress_state`, 1 = given, 0x80 = still pending), shows the date panel, `main_stage_set(0)`.

HYPOTHESIS: the hint is an easter egg that appears only while the secret code is active; nothing else in the unit shows it. The latch `pad_code_entered` (`D_800B3CFC`) is set by `pad_code_update` (`func_80043448`, m-flow names) when the pad history shows up, up, down, down, left, right, left, right, cross, circle (18 pad states including releases, at most 16 frames between steps) and clears itself after about 240 frames; the title screen uses it too (core spec).

`magazine_hint_index` (`func_80077CA8`) maps the calendar to a hint 0-13: 1995 (year 95): months 3-4 0, 5-7 1, 8-10 2, 11-12 3; 1996: month 1 3, 2-4 4, 5-7 5, 8-10 6, 11-12 7; 1997: month 8 13, month 1 7, 2-4 8, 5-7 9, 8-10 10, 11-12 11; 1998: month 1 11, later 12. The 14 hints are monologues of the protagonist about the game itself (topics: how to read the biorhythm graph, calling girls by phone, mascot dolls and their controls, pad shortcuts, menu mode, SD animation speed, a joke about the radio and the manual, one about the RPG battle); the text is in `magazine_hints` and is not copied here.

Calendar (0x30) and system (0x40): the same functions as mode 0x34 stages 2 and 4; they return to stage 0 and so rebuild the holiday menu.

## 10. The shared-birthday check (`holiday_birthday_event_due`)

Evaluated in `holiday_open_init` before the screen is built. It returns 1 when all hold: the player's birthday (month, day) equals `girls[0].birthday` (NAME_ENT lets the player set both; the default birthday of girl 0 is 14 February); the calendar month is 3, or month 4 and day below 4; `appointment_find_today(...)` is 0 or -1; today is the player's birthday (`birth_day_check(0xF)`); and `girl_state(0) & 0x80` (affection at least 80). Then `dialogue_work.scene = 8`, `.girl = 0`, mode 0x5D (`MODE_SPECIAL_EVENT`, GEKO): special event 8 of girl 0. It needs the player to give both birthdays the same date in March or the first days of April, so it is unreachable with default birthdays. The content of the scene belongs to the GEKO unit.

## 11. Hardware layer used (module level)

| layer | calls from this unit |
|---|---|
| CD and background pictures | `dec_bg_cd_read(sector, slot)`, `dec_bg_show_set`, `dec_bg_decode_to_vram`, `dec_bg_reset`, `set_dec_bri`, `cd_read_sectors(n, address, sector)`, `cd_read_sync_status`, `cd_audio_command(9)` |
| SD sprite sets | `addr_init_holiday_sd`, `addr_init_club_sd`, `addr_init_else_sd`, `load_palette`, actors 96 and 97 of the actor pool |
| sound | `snd_command(id)` (ids in section 12), `snd_bank_start`, `bgm_is_playing`, `restore_bgm`, XA clips through `xa_clip_request_set` |
| text and menus | `sndisp`/`sndi`, `gnsx` (speaker label), `set_kanji_string`, `k_disp_*`, `k_sub_*`, `xa_wait` (wait for text and a press), `x_taku_string_set`/`x_taku_menu_set`, `menu_set`, `menu_check`, `menu_bar_show`, `menu_cursor_update` |
| HUD | `icon_can_use_set`, `get_icon_can_use`, `icon_disp_switch`, `icon_bar_update`, `parameter_show*`, `hizuke_*`, `message_window_*`, `cal_base_show`, `magazine_base_show` (frame and clip) |

## 12. Sound ids used (`UiSe` and the m-dates-a names)

0x003 `SND_CMD_XA_FORGET` (all icon clicks), 0xD1 `SND_CMD_FADE_BGM_SLOW` and 0x300 `SND_CLASS_VOICE` (the two click-for-a-voice handlers), 0x500 command or date chosen, 0x501 good activity, 0x502 poor activity, 0x504 dial start, 0x505-0x507 dial digit tones, 0x50A phone-book pick, 0x50D phone, magazine, calendar or system chosen, 0x300 XA voice clip start, 0xD1 click on the player spot.

## 13. Functions of the unit

| old | new | stage / role |
|---|---|---|
| `func_80075320` | `mode_holiday` (core) | stage switch |
| `func_80075468` | `holiday_stage_open` | stage 0 |
| `func_800754C0` | `holiday_birthday_event_due` | section 10 |
| `func_800755B4` | `holiday_open_init` | 4.1 |
| `func_80075A64` | `holiday_open_wait_bg` | 4.1 |
| `func_80075AF8` | `holiday_icons_set_available` | 4.3 |
| `func_80075BE8` | `holiday_stage_menu` | stage 1 |
| `func_80075C24` | `holiday_menu_frame` | 4.2 |
| `func_80075C84` | `holiday_phone_dial_shortcut` | 8.2 |
| `func_80075FA0` | `holiday_activity_bg_sector` | 5.1 |
| `func_80076000` | `holiday_menu_click` | 4.5 |
| `func_800764AC` | `appointment_place_candidates` | 7.2 |
| `func_80076630` | `appointment_start` | 7.2 |
| `func_800767A4` | `appointment_place_menu_build` | 7.3 |
| `func_80076B48` | `appointment_place_choose` | 7.3 |
| `func_80076C7C` | `appointment_place_confirm` | 7.3 |
| `appointment_launch` | (kept, patch name) | 7.4 |
| `func_80076EC0` | `holiday_stage_appointment` | stage 0xFE |
| `func_80076F48` | `holiday_stage_activity` | stage 5 |
| `func_80076FDC` | `holiday_activity_pick_sd_set` | 5.3 |
| `func_80077330` | `holiday_activity_bg_read` | 5.1 |
| `func_8007739C` | `holiday_activity_load` | 5.1 |
| `func_80077694` | `holiday_activity_sd_setup` | 5.2 |
| `func_80077900` | `holiday_activity_play` | 5.4 |
| `func_80077BE0` | `holiday_stage_magazine` | stage 0x20 |
| `func_80077C50` | `magazine_init` | section 9 |
| `func_80077CA8` | `magazine_hint_index` | section 9 |
| `func_80077E30` | `magazine_show` | section 9 |
| `magazine_exit`, `holiday_tel`, `holiday_tel_init`, `holiday_tel_call`, `holiday_tel_exit`, `telephone_class_init`, `holiday_club_init`, `holiday_club_exit`, `holiday_taibu_club_exit`, `holiday_club`, `holiday_club_join_exit` | (O.BIN names kept, 11 comment rows) | stages 0x20, 0x10, 0x06, 0x07 |
| `func_8007866C` | `holiday_club_select` | 6.2 |
| `func_8007894C` | `holiday_stage_club_join` | stage 7 |

Helpers in other files, with the names the other units gave them (core, m-hud, m-scheduler, m-weekday, m-flow, m-menutext, m-sysmenu): `func_8006A044` `magazine_mark_read`, `func_8006A2CC` `magazine_page_build`, `func_8006D138` `progress_flags_update`, `func_8006B640` `appointment_prompt_noop`, `func_8006BDA8` `command_roll_poor_result`, `func_8006F524` `command_girl_select`, `func_80066A2C` `season_index`, `func_80066A84` `in_summer_months`, `func_8006B0C8` `appointment_list_sort`, `func_8005AC70` `command_screen_icons_reset`, `func_8005352C` `player_in_sports_club`, `func_80053564` `girl_find_by_club`, `func_8005B0F4` `room_click_radio_holiday`, `func_8005B1A8` `room_click_mascot_holiday`, `D_80125C90` `icon_enabled`, `D_800B3CFC` `pad_code_entered`, `D_801230D0` `scene_work`.

## 14. Quirks a faithful port keeps

- Phone-book item 12 shows the rival's name but stores 11; rows 11 and 12 of the number table are equal and row 11 never sets the girl (8.2, 8.4).
- The button dial shortcut adds its tokimeki bonus to `girls[row]`, even though the slot handed on is 11 for row 12.
- The club scene sets `date_girl` before it tests anything, so a failed test still leaves `date_girl` changed.
- `parameter_show_init` is called twice in the birthday branch and in the normal open path.
- In the sick case with `remedial_week` also set, the background is the season one but the SD set is the normal command set (the two tests differ).
- `appointment_place_candidates` returns 1 and writes the active bit into `out[0]` when fewer than two places exist; the value is never read.
- The place question has no cancel; confirm on item 2 of a 2-item menu does nothing.
- The secret-code latch gates the magazine hint (section 9).

## 15. Open questions

Each is also in the return list of the analyst pass; items 1 to 4 change rows of other tables.

1. **Place base of the appointment word.** `core.tsv` (the `PLACE_*` rows) and `spec/core` say place values in appointment words are place + 1. The code shows the raw 0-based `Place` id: TEL's first place page stores the item index 0-9 and prints `place_name_table[value]` for the same value (second page: items 0-5 store 10-15, the seasonal items store 16-20), and `appointment_place_menu_build` and `appointment_place_confirm` index the name table with the raw field. The notes of the `PLACE_*` rows and line 519 of `spec/core` need the fix.
2. **`appt_place_0..3` (core, HYPOTHESIS rows).** Section 7 shows their roles: chosen place, true place, two decoys. Suggested names `appt_place_chosen`, `appt_place_true`, `appt_place_decoy_a`, `appt_place_decoy_b`. Not renamed here.
3. **`appointment_flag` and `AppointmentBits.b24` (`flag`).** 0 when the player arranged the date by phone (TEL), 1 when a girl invited him (GEKO): suggested `invited_by_girl` and `appointment_invited`.
4. **Mode 0x60 (`MODE_CLUB_CAMP_2`, core HYPOTHESIS).** This unit starts it at the first club activity of a club (section 5.4, emu-confirmed); the overlay name KANGEI suggests a welcome scene. Suggested `MODE_CLUB_FIRST_ACTIVITY` (HYPOTHESIS for the content), with 0x5F staying the camp.
5. **`D_800B3CFC`** is `pad_code_entered` in the m-flow table (core calls it idle in the title); section 9 follows that name. Nothing to change here.
6. **`D_801230D0`** is `scene_work` in the m-weekday table; this unit adds two uses (the holiday SD set id and the phone slot for TEL), which are worth a note in its row.
7. **Phone slot 11.** Phone-book item 12 shows the rival but stores 11; TEL maps 11 to 12 in its ringing step only. The TEL analyst should confirm that every TEL branch treats 11 as the rival, and the port may drop the alias (section 8.4).
8. **Appointment word bits 21-23.** TEL stores `D_80140560`, GEKO stores 6; the reader of the field was not found.
9. **Shared-birthday scene** (special event 8 of girl 0): the content belongs to the GEKO unit; it was not reached in the probes because it needs equal player and girl-0 birthdays in March or early April.
10. **Magazine hint.** Whether the secret-code gate of the hint is a deliberate easter egg is a guess (section 9).
11. **`ClubFlagBits.b29` (`club_scene_blocked`, HYPOTHESIS):** no writer found in C; KANGEI or a raw byte write may set it.
12. **Click spots.** m-sysmenu names the two handlers `room_click_radio_holiday` and `room_click_mascot_holiday`; the rectangle of the first one is a radio there only as a HYPOTHESIS, so the hint about the radio reception is the only support (section 9).
13. **Names shared with other units.** `LastCommandBits.low/icon` (`weekday_command`, `holiday_command`) are rows here; no merged table names these two fields, so the weekday and scheduler owners should confirm the pair.
14. **Day-end expulsion** on a skipped club day lives in `day_event_dispatch` / TEL stage 8 (section 6.3); this unit only sets the cursor and message. The exact calendar day the test sees is for the scheduler spec.
