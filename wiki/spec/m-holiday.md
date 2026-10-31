---
type: spec
updated: 2026-10-24
sources: ["src/main/80075320.c", "config/renames/proposals/m-holiday-apply.tsv", "config/renames/core.txt", "config/renames/scheduler.txt", "config/renames/school-day.txt", "wiki/spec/core.md", "wiki/ref-english-patch.md", "wiki/data-types.md", "wiki/emulator.md", "asm/data/main/80075320.data.s"]
---

# Port spec: holiday screen, appointments and telephone (unit m-holiday)

Unit `m-holiday` of phase 2 ([[tickets/T-27013-p2-m-holiday]], parent [[tickets/T-27000-phase-2]]). It covers `src/main/80075320.c` (41 function definitions, one original object): the handler of main mode 0x46 `holiday_mode_handler`, the screen used on Sundays and other no-school days. The executable rename batch is `config/renames/proposals/m-holiday-apply.tsv`: 49 rows (28 functions and 21 data symbols). The Docker dry run with `--ledger m-holiday` resolved every row with zero problems. It covers all 28 placeholder function definitions in the object. The 13 definitions already named in the current tree are retained: `holiday_mode_handler` and `appointment_launch`, plus 11 O.BIN names. `D_800B6CE0`, the shared `LastCommandBits` field requests and other constants or comments are explicit follow-up work below, not rows in that executable batch. No game text is quoted; string roles are described without copying their contents.

Scope is the port-first one of 2026-10-24: the game logic, state and flow in full; the hardware layer (CD reads, MDEC backgrounds, SD sprite sets, sound driver, text system) only by its entry points (section 11).

Evidence for this prepared batch: the current C bodies and their callers/callees, current applied ledgers, the other owners' current proposal tables where their changes are still pending, and the Docker planner result. No emulator run was performed for T-27050. The `Emu:` paragraphs below are inherited observations from the earlier draft; they are historical notes, not new verification from this batch. Recheck them before using them as port acceptance evidence. Save states and logs stay in the gitignored `build/emu/`.

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

`holiday_mode_handler` (`func_80075320`, already named in the core ledger) switches on `mode_stage`; each handler switches on `mode_step`, and uses `substep` where noted. After the mode-stage handler returns it always runs the common tail: `hud_mascot_update` when actor 27 is visible (flag 0x80), `message_window_show`, `dec_bg_draw(0)`, `hizuke_show`, `parameter_show`, `cursor_update`, `hud_screen_frame_draw(1)`.

| stage (`HolidayModeStage`) | function | steps | what |
|---|---|---|---|
| 0x00 `HOLIDAY_MODE_STAGE_OPEN` | `holiday_mode_stage_open` (proposed for `func_80075468`) | 0 `holiday_open_init`, 1 `holiday_open_wait_bg` | build the screen, read the season background; then `mode_stage_next` to mode-stage 1 |
| 0x01 `HOLIDAY_MODE_STAGE_MENU` | `holiday_mode_stage_menu` (proposed for `func_80075BE8`) | 0 `holiday_menu_frame` | per-frame input |
| 0x05 `HOLIDAY_MODE_STAGE_ACTIVITY` | `holiday_mode_stage_activity` (proposed for `func_80076F48`) | 0 `holiday_activity_load` (substeps 0-3), 1 `holiday_activity_sd_setup`, 2 `holiday_activity_play` | play the chosen command |
| 0x06 `HOLIDAY_MODE_STAGE_CLUB_MENU` | `holiday_club` (O.BIN) | 0 `holiday_club_init`, 1 `holiday_club_select`, 2 `holiday_club_exit`, 3 `holiday_taibu_club_exit` | Sunday, in a club |
| 0x07 `HOLIDAY_MODE_STAGE_CLUB_JOIN` | `holiday_mode_stage_club_join` (proposed for `func_8007894C`) | 0 `join_club_init`, 1 `join_club_select`, 2 `join_club_message`, 3 `holiday_club_join_exit` | Sunday, no club (the join steps are shared with mode 0x34 mode-stage 5) |
| 0x10 `HOLIDAY_MODE_STAGE_TELEPHONE` | `holiday_tel` (O.BIN) | 0 `holiday_tel_init`, 1 `holiday_tel_call`, 2 `holiday_tel_exit` | phone book, dial tones, hand-over |
| 0x20 `HOLIDAY_MODE_STAGE_MAGAZINE` | `holiday_mode_stage_magazine` (proposed for `func_80077BE0`) | 0 `holiday_magazine_init`, 1 `holiday_magazine_show`, 2 `magazine_exit` (O.BIN) | information magazine page |
| 0x30 `HOLIDAY_MODE_STAGE_CALENDAR` | `settings_command_screen_mode_stage_calendar` (`func_8005B798`, pending m-sysmenu name) | 0 init, 1 wait for cross, 2 exit | this-month/next-month calendar page with the biorhythm graph, shared with mode 0x34 mode-stage 2 |
| 0x40 `HOLIDAY_MODE_STAGE_SYSTEM` | `settings_mode_stage_dispatch` (`func_8005D9B4`, pending m-sysmenu name) | 0 root init, 1 root menu, 0x10.. one range per setting | system settings, shared with mode 0x34 mode-stage 4 |
| 0xFE `HOLIDAY_MODE_STAGE_APPOINTMENT` | `holiday_mode_stage_appointment` (proposed for `func_80076EC0`) | 0 `appointment_start`, 1 `appointment_place_choose`, 2 `appointment_place_confirm`, 3 `appointment_launch` | the date promise of today |

Stages 0x30 and 0x40 return with `mode_stage_set(0)` and `mode_step_set(2)`; `holiday_mode_stage_open` runs `holiday_open_init` for every step except 1, so the menu is rebuilt each time. Stage 5 uses `step_timer` as a frame counter that its dispatcher increments itself (`step_timer += 1` at the top of `holiday_mode_stage_activity` and `holiday_tel`).

Entry: the day-event reader `day_event_dispatch` turns a queue record of type 0x10 (`DAY_EVENT_HOLIDAY`) into `game_mode_set(0x46)`; the queue builder writes that record for every day whose `DayKind` is 1 (Sunday) or 2 (national holiday), unless a special event takes the day (core spec sections 3.4 and 3.5). The TEL scene also returns here (`game_mode_set(0x46)`) when a girl does not answer.

Exits: `day_event_next(0)` after a finished activity (back to the day screen, next record); `game_mode_set(0x50)` (DATE), `0x45` (TEL), `0x5D` (special event, GEKO), `0x60` (club scene, KANGEI), `0x73` (practice match, RENSYU).

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
| `appointment_place_0..3` (core names of `unk_724..727`) | chosen place, true place and decoys, section 7.4 |
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
| `holiday_magazine_hint_*` (`D_800B6A14`-`D_800B6CB8`) | 13 named strings | indexes 0-12, section 9; index 13 at `D_800B6CE0` is deferred because the specific topic is not supported in this pass |
| `holiday_magazine_hints` (`D_800B6CF8`) | u8 *[14] | pointer table in index order |

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
2. If `holiday_shared_birthday_scene_due()` (section 10): set `talk_work` scene 8 and girl 0, `parameter_show_init` twice, `game_mode_set(0x5D)`, and leave.
3. Clear sprite tables, switch the background clear on, clear the texture cache and the text system, set the title, `icon_count = 14`, create the icon actors and the 14-item icon menu, run `holiday_icons_set_available` (4.3), set the speaker label to the player.
4. Pick the monologue and the icon limits, first match wins:
   - `remedial_week`: re-create the icons; every command, phone, date and magazine icon is off and then icons 0, 1, 2 are turned on again; icon 13 stays off. System and calendar stay on. Message: a forced remedial week has to be done.
   - `health == HEALTH_SICK` (3): re-create the icons; all commands and phone, date and magazine are off, icon 7 (rest) is turned on, the cursor `menu_cursor_0` starts on 7. Message: too ill to move.
   - otherwise (`holiday_icons_set_available` stays in force): if `hud_appointment_find_today` > 0, line 1 is "a date with <girl of slot 0> today" and the cursor starts on icon 9; if the player is in a sports club (ids 6-10, `girl_player_sports_club_check`, `func_8005352C`) and it is the third Sunday (`(day + 6) / 7 == 3`, `weekday == 6`) a second line says it is the club's practice match day (`month % 3 == 0`) or practice day (other months) and the cursor moves to icon 4; with neither, the line asks what to do today.
5. Start the text, clear the upper actors, init the cursor actor, `parameter_show_init` twice, `message_window_init`, and read the season background `D_800B5960[season]` with `dec_bg_cd_read` (season index: months 3-5 give 0, 6-8 give 1, 9-11 give 2, 12, 1 and 2 give 3). A cached slot (result 0/1) shows it at once, sets brightness 0x80 and does `mode_stage_next` (stage 1); result -1 (read started) does `mode_step_next` to `holiday_open_wait_bg`, which waits for `cd_read_sync_status & 1`, decodes (`dec_bg_decode_to_vram`), re-reads on decode result -1, then does the stage change.

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
| 9 appointment | `hud_appointment_find_today(year, month, day) <= 0` (no promise today, or one of the days it answers -1 for: December 24, January 1, from March 1998) |
| 13 | always on this screen (it belongs to the weekday screen) |
| 14 | `icon_mode == ICON_MODE_FREE` (free layout has an extra slot that this screen does not use) |
| 4 club | on a Sunday: all 11 club records have the `quit` flag set (nothing to join, no club to leave); other holidays: `player_flags.club == 11` |

The remedial-week and sick cases of 4.1 replace this list. Everything not named is on; the system (11) and calendar (12) icons are never switched off by this screen.

### 4.4 Commands and what makes a command

Icons 0 to 7 are the commands, the same eight as on the school-day screen (`WeekdayCommand`, m-scheduler names `COMMAND_*`): 0 liberal arts, 1 science, 2 art, 3 athletics, 4 club, 5 trivia, 6 looks, 7 rest. The names follow the row of the `command_deltas` table (each row raises the parameter of the same name; row 7 raises condition and lowers stress; row 4 is replaced by the row of the player's club in `club_activity_deltas`).

### 4.5 Choosing an icon: `holiday_menu_click`

Preconditions: not (`icon_mode == FREE` and `pad_held & 1`; that case returns 1 and leaves the input to the icon layout editor of `hud_icon_bar_update`). Acts on `pad_pressed & 0x20` (confirm) with `menu_cursor_0 >= 0`:

1. Common to any hit: `cd_audio_command(9)`, `snd_command(SND_CMD_XA_FORGET)` (3: stop the voice clip and clear its state, m-dates-a name), and when no BGM is playing `snd_bank_start(0, 0)` (restarts the BGM that a voice-clip click faded out; the `bgm_is_playing` test is the evidence). The handler returns 1.
2. By icon (the icon must be on, `get_icon_can_use`, except 11 and 12):
   - 0-3 and 5-7: `holiday_command = icon`; `snd_command(0x500)`; record `girl = icon`, `scene = hud_command_roll_poor_result(1)` (the poor-result roll, 4.6); `mode_stage_set(5)`. Emu: icon 0 gives sounds 3 and 0x500 then `mode_stage_set(5)`.
   - 4, not a Sunday (a national holiday with a club): `girls[girl_find_by_club(club)].affection += 1` when that girl is `acquainted`; then as above with stage 5.
   - 4, Sunday, `club == 11`: record as above; `mode_stage_set(7)` (join list). The icon's own affection bonus is skipped.
   - 4, Sunday, in a club: `affection += 1` as above, record as above, `mode_stage_set(6)` (club menu). Emu: confirmed.
   - 9: needs `hud_appointment_find_today > 0`; `holiday_command = 9`; `snd 0x500`; `mode_stage_set(0xFE)`.
   - 8 phone: `holiday_command = 8`; `snd 0x50D`; `mode_stage_set(0x10)`.
   - 10 magazine: `snd 0x50D`; `mode_stage_set(0x20)`.
   - 12 calendar: `snd 0x50D`; `mode_stage_set(0x30)`. 11 system: `snd 0x50D`; `mode_stage_set(0x40)`.

`girl_find_by_club(club)` (`func_80053564`, m-menutext name) returns the first heroine 0-10 whose club field equals the club, or -1.

### 4.6 The poor-result roll (`hud_command_roll_poor_result`, `func_8006BDA8`, m-hud)

Run once, when the command is chosen, with day offset 1 (tomorrow's biorhythm). Summary for the port: `score = biorhythm(selected waves, scale 16) + parameter / 16` (club: club exp / 16 instead of the parameter; rest: fixed 16), `+ 6`; `score = rand() % (score + 4)`; stress factor `f = 4` when stress >= 97 else `stress / 32`; `score = (8 - f) * score / 8`; if the three waves at scale 3 are all bad, `score /= 4`; result 0 (good) when `score >= 4`, else 1 (poor). The three waves are sines with periods of about 23, 28 and 33 days over `day_count` plus a per-player offset (`func_8006C700`). The result (`COMMAND_RESULT_NORMAL` 0 or `COMMAND_RESULT_POOR` 1, m-weekday names) is stored in the record and drives the animation frame, the sound and the size of the gains.

## 5. Playing a command (stage 5)

### 5.1 Load (`holiday_activity_load`, step 0, by substep)

Always: `text_system_reset`, hide the message-window title.

- Substep 0: icons: only the chosen one stays on (`icon_can_use_set(i, i == command)` for i < 14). `holiday_command` is rewritten from the record. `progress_state[0x15 + command % 9] += 1`. For command 4 with a club (`club < 11`): `clubs[club].exp += 6` (`CLUB_ACTIVITY_EXP`). Then `holiday_activity_bg_read`: season background when sick, else `holiday_activity_bg_sector()`. Result -1: substep 1; result 0/1: show it, substep 2.
- Substep 1: wait for the read, decode, retry on -1 (as 4.1), then substep 2.
- Substep 2: `holiday_activity_pick_sd_set` (5.3), then the SD animation file: sick and not `remedial_week`: 9 sectors from 0x7D8A (`SD_SICK_BED_PACK_SECTORS`, `SD_SICK_BED_PACK_LBA`, m-weekday names); command other than 4: 0x29 sectors from `holiday_sd_set_sectors[set]`; command 4: 0x29 sectors (`SD_PACK_SECTORS`) from `club_sd_set_sectors[set]`; to 0x801E0000. Substep 3.
- Substep 3: wait for the CD, `mode_step_next`.

Emu (command 0, April, not sick): `dec_bg_cd_read(0x3FFB, 0)`, `holiday_activity_pick_sd_set`, `cd_read_sectors(0x29, 0x801E0000, 0xA20F)` (set 1).

The background of the activity (`holiday_activity_bg_sector`, `func_80075FA0`): `holiday_activity_bg_sectors[command]` for commands other than 4, `club_bg_sectors[club]` for command 4.

### 5.2 Set-up (`holiday_activity_sd_setup`, step 1)

Clears actor 96, then, with `poor = record.scene`:

- sick and not `remedial_week`: `addr_init_else_sd(2)` (a set for the sick-in-bed animation) and `poor = 0`;
- command 4: `addr_init_club_sd(set)`; when `schedule_command_girl_select(4) != COMPANION_NONE` (0xF, m-weekday name; some girl takes part, see 5.3) a second figure is put on actor 97 at (-0x18, -0x40) with frame `poor + 2`;
- otherwise: sets 6 and 7 (rest) force `record.scene = 0` and `poor = 0`; `addr_init_holiday_sd(set)`.

Then `load_palette`, and the main figure on actor 96 at (-0x50, -0x40), frame = `poor`, animation flag 0x82 instead of the default when `sd_anim_fast` is set. `mode_step_next`. Emu: `addr_init_holiday_sd(1)`.

### 5.3 Choosing the SD set (`holiday_activity_pick_sd_set`)

`scene_work = 0xFF` (`D_801230D0`), then by command:

- not command 4: `set = holiday_sd_set_summer[command]` when `hud_in_summer_months()` (`func_80066A84`, months 6-9) else `holiday_sd_set_other[command]`: commands 0, 1, 2 give pairs (0,1), (2,3), (4,5), command 3 gives 12 in every month, 5 gives (0xA, 0xB), 6 gives (8, 9), 7 gives (6, 7); named by `HolidaySdSet`.
- command 4: `col = 0` when `schedule_command_girl_select(4) == 0`, else `1`; `set = club_sd_set_summer[club][col]` in months 6-9, `club_sd_set_other[club][col]` otherwise. Per club (summer column 0 / 1 | other-months column 0 / 1): literature 0,2 | 1,3; drama 0x1E,0x1F | same; science 4,6 | 5,7; computer 8,0xA | 9,0xB; art 0x1A,0x1C | 0x1B,0x1D; brass band 0x16,0x18 | 0x17,0x19; baseball 0xC,0xD; soccer 0xE,0xF; tennis 0x14,0x15; swimming 0x12,0x13; basketball 0x10,0x11 (the last five are the same in both seasons).

`schedule_command_girl_select(4)` (`func_8006F524`, m-scheduler) returns which girl takes part in the club activity: girl 0 when she and the player share the club and she is acquainted, another heroine id when she is the best candidate by the point rule, 0xF for nobody. Column 0 therefore means "girl 0 is there"; any partner (value other than 0xF) also adds the second figure of 5.2.

### 5.4 Running (`holiday_activity_play`, step 2)

`step_timer` starts at 1 on the first call (the dispatcher adds 1 before the step).

- At `step_timer == 0x20`: if the main figure's frame bit 0 is 0, `snd_command(0x501)` (`UI_SE_ACTIVITY_GOOD`), else `0x502` (`UI_SE_ACTIVITY_POOR`); then `parameter_up_down(command, 1, record.scene)`; `parameter_show_init`. Return.
- While `step_timer <= 0x40`: nothing.
- After: clear actors 96 and 97. If `weekday == 6` and `record.girl == 4` and `club != 11` and `clubs[club].exp == 6` (this was the first club activity of that club): `girl = girl_find_by_club(club)`; `date_girl = girl` (assigned before the tests, whatever they give); with `girl != -1` and `day_count < 0x2DA` (730) and `girl.met` and `girl.acquainted` and not `club.club_scene_blocked` and not `girl.club_scene_done`: set `girl.club_scene_done`, `game_mode_set(0x60, club)` and return. (The test for club 10 runs twice, once under `girl == 0`; both copies are identical.) Emu (Sunday, club 0, girl 0 met, acquainted and in club 0, exp 0 before): the activity ends with `game_mode_set(0x60, 0)` and flag bit 18 set on the girl; with no girl in the club the same activity ends in `day_event_next` instead.
- Otherwise `day_event_next(0)`.

Emu timing: sound 0x501 at +32 frames, `parameter_up_down(0, 1, 0)` in the same frame, `day_event_next` at +65.

### 5.5 Effects (`parameter_up_down`, weekday unit)

`parameter_up_down(command, holiday = 1, poor)` gives each parameter a delta row (`command_deltas[command]`, or `club_activity_deltas[club]` for 4), in 1/16 point units. Positive deltas are multiplied by `4 / (poor ? 2 : 1) / (sick ? 2 : 1) / (injured or neurotic and command not in {5, 6, 7} ? 2 : 1)`; negative deltas are applied as they are (1x). Stress is the exception in sign: its negative deltas are the good ones and are scaled like gains; a poor result adds one stress point. So a holiday command pays four school-day gains for one school-day cost. Commands 5, 6 and 7 are not reduced by the injured or neurotic penalty, and the roll of command 7 is forced good by 5.2.

## 6. Sunday club flow

### 6.1 Join list (stage 7)

Sunday and `player_flags.club == 11` (no club), icon 4. `join_club_init` (`hud_club_list_menu`) shows the list of 11 clubs plus cancel; a club whose `quit` flag is set is hidden (its item has width 0) and the cursor starts on the first club still shown; `join_club_select`: cross returns to stage 0; confirm on cancel does the same; confirm on a club sets `player_flags.club = club` and goes on; `join_club_message` prints the joined message, and for clubs 6-10 (the sports clubs) a reminder that the third Sunday of every month is club day; `holiday_club_join_exit` clears the upper actors, shows the icons and does `mode_stage_set(5)` (the activity with the command recorded at click time). Emu: weekday 6 with no club: icon 4 gives `mode_stage_set(7)`; choosing club 3 stores 3 in the club nibble, `join_club_message` runs substeps 0, 1, 2 and 0xFF (no reminder below club 6), then `mode_stage_set(5)`.

### 6.2 Club menu (stage 6)

Sunday, in a club, icon 4. Two choices (`x_taku` menu 1): do the club activity, leave the club. Cross returns to stage 0. Choice 0: step 2 `holiday_club_exit`: if the player is in a sports club (`girl_player_sports_club_check`, ids 6-10) and it is the third Sunday and `month % 3 == 0`: `game_mode_set(0x73)` (practice match, RENSYU); else `mode_stage_set(5)` (the activity). Choice 1: step 3, the message "left the club", `clubs[club].quit = 1`, `player_flags.club = 11`, the text restarts; a press of confirm or cancel (`pad_pressed & 0x60`, `holiday_taibu_club_exit`) returns to stage 0. Emu: on 17 June (a Sunday, third week) with club 6 the menu cursor starts on icon 4, icon 4 gives `mode_stage_set(6)`, choice 0 gives `game_mode_set(0x73)`.

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

`appointments[8]` are 32-bit words, sorted by date (the sort is proposed as `hud_appointment_list_sort`, `func_8006B0C8` in m-hud's apply table). `appointment_place_candidates` scans up to eight slots and stops at the first inactive word. Layout, from the TEL and GEKO writers and the readers here: bits 0-4 place (raw 0-based `Place` id; the name table `place_name_table` is indexed directly by it), bits 5-11 year (calendar year mod 100), bits 12-15 month (1-12), bits 16-20 day, bits 21-23 a value TEL stores in `D_80140560` and GEKO sets to 6 (meaning unknown), bit 24 `appointment_flag` (0 when the player arranged the date by phone in TEL, 1 when a girl invited the player in GEKO), bits 25-28 girl, bit 29 active, bits 30-31 zero.

`hud_appointment_find_today(year, month, day)` is the pending m-hud name for the current `appointment_find_today` body. That body normalizes month 13 to 1; returns -1 on December 24, January 1, and from March of year 0x62; otherwise scans all eight words for equal month/day and returns the highest matching index plus one, or 0 when there is no match. This loop does not test the active bit. The callers here use a result greater than zero as a promise today and otherwise follow the no-promise/special-date path; `appointment_start` separately reads slot 0.

### 7.2 Step 0: `appointment_start`

1. `appointment_place_0..3 = 0xFF`, `hud_appointment_prompt_noop()` (`func_8006B640`, an empty function).
2. `appointment_place_candidates(out)`: if slot 0's place is 16-20 (pool, sea, shrine, skating rink, ski resort) return 1; else count distinct places among the active slots (up to the first inactive one), with the places 16-20 left out; fewer than 2 distinct places: `out[0] = slot0.active` (1) and return 1; otherwise `out[]` = the first 3 distinct places in slot order and the count.
3. Count >= 2: `k_reset`, speaker label the player, the question "which place was the meeting?", `mode_step_next` (step 1).
4. Count 1: `appointment_place_0 = appointment_place_1 = slot0.place`, `appointment_place_2 = appointment_place_3 = 0xFF`; record `{0x80, slot0.girl, slot0.place}` at the queue index; `current_girl = date_girl = slot0.girl`; `appointment_flag = slot0.flag`; `mode_step_set(2)`.

### 7.3 Steps 1 and 2: the question

Step 1, by substep: 0 waits for the text (`xa_wait`); 1 builds the menu (`appointment_place_menu_build`); 2 reads menu 1.

`appointment_place_menu_build`: with 3 or more candidates: `r = rand() % 3`; options `c[r % 3]`, `c[(r+1) % 3]`, `c[(r+2) % 3]` (a random rotation of the first three), `appointment_place_0 = 2`, a 3-item menu; with 2: `r = rand() & 1`, options `c[r]`, `c[1 - r]`, `appointment_place_0 = 1`, a 2-item menu. Each item text is an opening quote mark, the place name and `appointment_choice_suffix`. The options are stored in `appointment_place_1..3` (third 0xFF when 2). Emu: slots with places 3 and 5 gave `1,3,5,0xFF`.

Choice (confirm in menu 1): item 0 or 1: `appointment_place_0 = appt_place_[1 + item]`; item 2: only when `appointment_place_0 == 2`, `appointment_place_0 = appointment_place_3`; then `mode_step_next`. Cross does not cancel this question. Emu: item 1 gave `5,3,5,0xFF`.

Step 2: a monologue naming the chosen place (`place_name_table[appointment_place_0]`) and the girl of slot 0 (`get_g_name`), wait for a press, `mode_step_next`.

### 7.4 Step 3: `appointment_launch`

`arr = (appointment_place_1, appointment_place_2, appointment_place_3)`; `appointment_place_1 = slot0.place` (the true place); the two other entries are refilled from the non-true entries of `arr` into `appointment_place_2` and `appointment_place_3` in order (so with two options the decoy lands in place 2 and 3 stays 0xFF); `appointment_place_0` stays the place the player chose. Then the queue record `{0x80, slot0.girl, slot0.place}`, `current_girl = date_girl = slot0.girl`, `appointment_flag = slot0.flag`, `game_mode_set(0x50)`.

Final layout (what DATE and the day-end check read): `appointment_place_0` = chosen place, `appointment_place_1` = true place, `appointment_place_2/3` = decoys or 0xFF. DATE compares 0 with 1: equal means the player named the right place, different means the wrong place, and DATE then looks at 2 and 3 (the decoy places, 0xFF = none; HYPOTHESIS: the wrong-place scenes use them to say where the other girls wait). The queue reader's day-end check also compares the two. When only one place is pending, 0 = 1 and 2 = 3 = 0xFF.

The question is a memory test: the player must remember which place goes with the girl whose promise is due; the other options are the places of the other pending promises. It is skipped when only one place is pending or when the true place is one of the five seasonal places 16-20.

## 8. Telephone

### 8.1 Phone book (steps 0 and 1)

`holiday_tel_init`: clears text and the upper actors, hides the title and the icon bar, `telephone_class_init`, a monologue asking whom to call, `mode_step_next`.

`telephone_class_init` builds menu 0 with 14 items in 3 columns of 5 (item `i`: x `(i / 5) * 0x58 - 0x88`, y `(i % 5) * 0x10 - 0x38`, size 0x48 x 0xE) under a heading, and the highlight actor 96:

| item | entry | shown when |
|---|---|---|
| 0 | `girl_full_name_table[13]` (Yoshio) | always |
| 1-11 | `girl_full_name_table[i - 1]` (girls 0-10) | `girls[i - 1].phone_known`, else its width is 0 (not selectable) |
| 12 | `girl_full_name_table[12]` (the rival, Rei) | always |
| 13 | cancel text, one row lower | always |

`holiday_tel_call` (step 1, per frame): `menu_check(0)`, bar, cursor, `k_disp_inc`. Cross: `mode_stage_set(0)` (menu rebuilt). Confirm on an item with `menu_cursor_0 != -1`: item 13: `mode_stage_set(0)`; otherwise `snd 0x50A` and `current_girl = 13` for item 0, `11` for item 12, `item - 1` for 1-11; `mode_step_next`. Emu: item 12 gave `current_girl = 0xB`.

### 8.2 Numbers and the button dial shortcut

`phone_numbers[13][7]`: seven digits of 0, 1, 2 per row; the row is the girl id (rows 0-10), row 11 and row 12 are the same number. Digit 0 is the circle button, 1 cross, 2 triangle. Rows: 0 `0121020`, 1 `1120201`, 2 `2220010`, 3 `2101010`, 4 `2102211`, 5 `0001221`, 6 `2110121`, 7 `2210112`, 8 `0201202`, 9 `0122100`, 10 `0002210` (also used for Yoshio: `current_girl == 13` reads row 10), 11 and 12 `0210210`.

`holiday_phone_dial_shortcut` (every frame of the menu): only while the pointer is in the hot spot x -0x4F..-0x41, y -0xF..0x17 (the picture of the telephone, HYPOTHESIS). A new press of exactly one of circle, cross, triangle (`pad_pressed & 0x70` equal to 0x20, 0x40 or 0x10) appends key 0, 1 or 2 to `holiday_dial_keys` (position `pos`, then `pos++`, wrap at 7) and plays `UI_SE_DIAL_TONE_0 + key`. Then, on any of the three buttons, for each row `i` 0-12 the last 7 keys (oldest = `keys[pos]`) are compared with the row; on a full match and `girls[i].phone_known`:

- `i == 12`: `current_girl = 11`, `menu_cursor_0 = 12`, `scene_work = 11`; `i == 11`: nothing is set (the number is the same as row 12, so this only happens if girl 11 has `phone_known`); other `i`: `current_girl = scene_work = i`, `menu_cursor_0 = i + 1`;
- `girls[i].tokimeki word += rand() & 0x7FFF` (the record of the row, so Rei's own record for 12);
- during `remedial_week`: `param_stress += 0x20`;
- `mode_stage_set(0x10)`, `mode_step_set(2)`, `substep = 11` (the dial tones are skipped), then `text_system_reset`, `icon_disp_switch(0)`, title hidden, `telephone_class_init`, `k_sub_disp_start(0)`; return 1. `telephone_class_init` ends with `menu_cursor_0 = 0`, so on the shortcut frame `menu_cursor_0` is 0, not the value set above; `holiday_tel_exit` sets it again on the next frame.

Emu: keys circle, cross, triangle, cross, circle, triangle, circle filled the ring as `0,1,2,1,0,2,0`, then `mode_stage_set(0x10)`, `mode_step_set(2)`, `game_mode_set(0x45)`, `mode_stage_set(1)`; the tokimeki word went from 0 to 0x2BCC; `current_girl = 0`, `menu_cursor_0 = 1` (after the `holiday_tel_exit` frame).

### 8.3 Dial tones and hand-over (`holiday_tel_exit`, step 2)

`step_timer` counts up. While `substep != 11`: only on frames with `(step_timer & 7) == 7`: substep 0 plays `UI_SE_DIAL_START` (0x504); substeps 4-10 play `UI_SE_DIAL_TONE_0 + digit k` (0x505 + k) of the row of `current_girl` (row 10 for Yoshio, 13), `k = substep - 4`; the substep increases by one on every such frame. So the number "dials" over about 88 frames.

At substep 11 (`HOLIDAY_DIAL_DONE_SUBSTEP`):

- `current_girl == 13` (Yoshio): `menu_cursor_0 = 0`, `k_sub_reset`, `restore_bgm`, `game_mode_set(0x45)`, `mode_stage_set(0)`;
- `current_girl == 11` (the rival's slot): `k_sub_reset`, `menu_cursor_0 = 12`, `scene_work = 11`, `restore_bgm`, `game_mode_set(0x45)`, `mode_stage_set(1)`;
- other girl: `k_sub_reset`, `menu_cursor_0 = current_girl + 1`, `scene_work = current_girl`, `restore_bgm`, `game_mode_set(0x45)`, `mode_stage_set(1)`.

All three branches call `restore_bgm` (`src/main/holiday.c`, `holiday_tel_exit`); an earlier reading that only the Yoshio branch does was wrong.

Phone-book cursor (`holiday_tel_call`, step 1): `menu_cursor_update(0)` with `menu_mode` 1, the only mode the holiday menu enters the phone book in (`src/main/menu_speaker_girl.c`). SELECT does `menu_mode++`, `menu_cursor_0 = -1` and returns, so CIRCLE on that frame selects nothing; later frames run the `menu_mode` 2 pointer control, which the port refuses. A held L1/L2/R1/R2 (`pad_held & 0xF`) blocks movement. Only an exact `pad_pressed & 0xF000` of UP or LEFT moves to the previous item and DOWN or RIGHT to the next; UP+DOWN does not move. The cursor wraps and skips items `menu_set` disabled (width 0: unknown numbers), at most 16 skips.

Golden (2026-10-30, `.asset-cache/emu/dates-phone-golden-v1`, ignored data): from the holiday menu of 1995-05-28 (`route-select-008.sstate`), DOWN puts the pointer on icon 8 and CIRCLE (frame 40) sets stage 0x10; `holiday_tel_init` runs on frame 41. `menu_mode` (GameState+0x1092) is 1 in every sample. In the phone book, DOWN and RIGHT move to the next selectable item and UP and LEFT to the previous one, wrapping and skipping unknown numbers. Calls to Yoshio, Shiori and the rival give 0x50A on the CIRCLE frame, 0x504 seven frames later, then from 32 frames after 0x504 at 8-frame spacing the seven digits of row 10 (Yoshio), row 0 (Shiori) and row 11 (the rival: `current_girl` 11). Row 12 holds the same bytes but only the dial shortcut reads it; `tools/port/test_phone_digits_executable.py` checks both rows against the executable. `scene_work` stays 16 for Yoshio. `.asset-cache/emu/dates-phone-golden-v2` reruns the three calls with a sample every frame and a breakpoint on `game_mode_set`: `holiday_tel_exit` calls `game_mode_set(0x45)` on frames 228, 188 and 198, one frame before the TEL overlay loads (229, 189, 199). The port replays these runs in `port/godot/tests/holiday/run_phone_book_golden.gd` and matches every sound, its frame and the hand-over frame.

Emu (item 12): sounds 0x504, then 0x505 0x507 0x506 0x505 0x507 0x506 0x505 at 8-frame spacing, then `game_mode_set(0x45)`, `mode_stage_set(1)` with `scene_work = 0xB`; TEL starts ringing (0x508 every 128 frames).

### 8.4 The slot alias of the rival

The value 11 in `current_girl` during the phone flow stands for girl 12 (Rei): the phone-book entry 12 shows entry 12 of the name table but stores 11; TEL's ringing step turns 0xB into its own girl value 0xC; TEL's list prints the name of girl 0xC for its item 11. Girl 11 (Miharu) has no phone-book item; her number row is a copy of the rival's. A port can keep the alias or give the rival her own id; the key point is that the TEL scene of stage 1 receives 11 and 12 as one person.

## 9. Magazine, calendar, system

`holiday_mode_stage_magazine` shows the information magazine, a quarterly page (m-hud spec, section 14: each issue unlocks one new date place; eight of the thirteen issues carry a place).

- Step 0 `holiday_magazine_init`: `hud_magazine_mark_read()` (`func_8006A044`, m-hud) sets issue bits 0..11 in `quarter_flags` only for years 95..97 with month groups 2-4, 5-7, 8-10 and 11-13; January 96..98 sets preceding-year bits 3/7/11. It never writes issue bit 12 in spring 1998. It also sets bit 15 in May-July, bit 13 in the first month of a quarter and bit 14 in the others (April 1995 sets both13 and14); the text is cleared, the title is set, `hud_magazine_page_build()` (`func_8006A2CC`, m-hud) lays out the page of that issue and sets the issue's date-place flag when it applies, the icons and the date panel are hidden, `mode_step_next`.
- Step 1 `holiday_magazine_show`: draws the page frame (`magazine_base_show`); substep 0 waits for a press (`xa_wait`); substep 1 continues only while the secret-code latch `pad_code_entered` is set, otherwise `mode_step_next`; substep 2 prints hint `holiday_magazine_hint_index()` as the player's monologue; substep 3 waits for a press; substep 4 ends the step.
- Step 2 `magazine_exit`: `schedule_progress_state_update()` (`func_8006D138`, m-scheduler: turns the read marks into the date-spot entries of `progress_state`, 1 = given, 0x80 = still pending), shows the date panel, `mode_stage_set(0)`.

HYPOTHESIS: the hint is an easter egg that appears only while the secret code is active; nothing else in the unit shows it. The latch `pad_code_entered` (`D_800B3CFC`) is set by `pad_code_update` (`func_80043448`, m-flow names) when the pad history shows up, up, down, down, left, right, left, right, cross, circle (18 pad states including releases, at most 16 frames between steps) and clears itself after about 240 frames; the title screen uses it too (core spec).

`holiday_magazine_hint_index` (`func_80077CA8`) maps the calendar to a hint 0-13: 1995 (year 95): months 3-4 0, 5-7 1, 8-10 2, 11-12 3; 1996: month 1 3, 2-4 4, 5-7 5, 8-10 6, 11-12 7; 1997: month 8 13, month 1 7, 2-4 8, 5-7 9, 8-10 10, 11-12 11; 1998: month 1 11, later 12. The 14 hints are monologues of the protagonist about the game itself; the text is in `holiday_magazine_hints` and is not copied here. The topic of index 13 is not named in this pass (section 3.2).

Calendar (0x30) and system (0x40): the same functions as mode 0x34 stages 2 and 4; they return to stage 0 and so rebuild the holiday menu.

## 10. The shared-birthday check (`holiday_shared_birthday_scene_due`)

Evaluated in `holiday_open_init` before the screen is built. The body returns 0 if the packed month/day fields of the player's and girl 0's birthdays differ; unless month == 3 or (month == 4 and day < 4); if `hud_appointment_find_today(...) > 0`; or if the call-site test `birth_day_check(0xF) != 1` holds. It returns 1 only when those tests pass and `get_g_zyotai_s(GIRL_SHIORI) & 0x80` is nonzero. The O.BIN wrapper `birth_day_check` is declared `void` in the current C definition, while its caller tests the returned register; the underlying `birth_day_check_days` returns 1 on a matching birthday. When the helper succeeds, its `holiday_open_init` caller writes `D_80122CD4 = 0` (girl 0), `talk_work = 8` (scene 8), calls `parameter_show_init` twice, and enters mode 0x5D (`GAME_MODE_SPECIAL_EVENT`). The contents of the scene belong to the GEKO unit.

## 11. Hardware layer used (module level)

| layer | calls from this unit |
|---|---|
| CD and background pictures | `dec_bg_cd_read(sector, slot)`, `dec_bg_show_set`, `dec_bg_decode_to_vram`, `dec_bg_reset`, `set_dec_bri`, `cd_read_sectors(n, address, sector)`, `cd_read_sync_status`, `cd_audio_command(9)` |
| SD sprite sets | `addr_init_holiday_sd`, `addr_init_club_sd`, `addr_init_else_sd`, `load_palette`, actors 96 and 97 of the actor pool |
| sound | `snd_command(id)` (ids in section 12), `snd_bank_start`, `bgm_is_playing`, `restore_bgm`, XA clips through `xa_clip_request_set` |
| text and menus | `sndisp`/`sndi`, `gnsx` (speaker label), `set_kanji_string`, `k_disp_*`, `k_sub_*`, `xa_wait` (wait for text and a press), `x_taku_string_set`/`x_taku_menu_set`, `menu_set`, `menu_check`, `menu_bar_show`, `menu_cursor_update` |
| HUD | `icon_can_use_set`, `get_icon_can_use`, `icon_disp_switch`, `hud_icon_bar_update`, `parameter_show*`, `hizuke_*`, `message_window_*`, `cal_base_show`, `magazine_base_show` (frame and clip) |

## 12. Sound ids used (`UiSe` and the m-dates-a names)

0x003 `SND_CMD_XA_FORGET` (all icon clicks), 0xD1 `SND_CMD_FADE_BGM_SLOW` and 0x300 `SND_CLASS_VOICE` (the two click-for-a-voice handlers), 0x500 command or date chosen, 0x501 good activity, 0x502 poor activity, 0x504 dial start, 0x505-0x507 dial digit tones, 0x50A phone-book pick, 0x50D phone, magazine, calendar or system chosen, 0x300 XA voice clip start, 0xD1 click on the player spot.

## 13. Functions of the unit

| old | new | stage / role |
|---|---|---|
| `func_80075320` | `holiday_mode_handler` (core) | mode-stage switch |
| `func_80075468` | `holiday_mode_stage_open` | stage 0 |
| `func_800754C0` | `holiday_shared_birthday_scene_due` | section 10 |
| `func_800755B4` | `holiday_open_init` | 4.1 |
| `func_80075A64` | `holiday_open_wait_bg` | 4.1 |
| `func_80075AF8` | `holiday_icons_set_available` | 4.3 |
| `func_80075BE8` | `holiday_mode_stage_menu` | stage 1 |
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
| `func_80076EC0` | `holiday_mode_stage_appointment` | stage 0xFE |
| `func_80076F48` | `holiday_mode_stage_activity` | stage 5 |
| `func_80076FDC` | `holiday_activity_pick_sd_set` | 5.3 |
| `func_80077330` | `holiday_activity_bg_read` | 5.1 |
| `func_8007739C` | `holiday_activity_load` | 5.1 |
| `func_80077694` | `holiday_activity_sd_setup` | 5.2 |
| `func_80077900` | `holiday_activity_play` | 5.4 |
| `func_80077BE0` | `holiday_mode_stage_magazine` | stage 0x20 |
| `func_80077C50` | `holiday_magazine_init` | section 9 |
| `func_80077CA8` | `holiday_magazine_hint_index` | section 9 |
| `func_80077E30` | `holiday_magazine_show` | section 9 |
| `magazine_exit`, `holiday_tel`, `holiday_tel_init`, `holiday_tel_call`, `holiday_tel_exit`, `telephone_class_init`, `holiday_club_init`, `holiday_club_exit`, `holiday_taibu_club_exit`, `holiday_club`, `holiday_club_join_exit` | (O.BIN names kept; definition comments are a separate follow-up) | mode-stages 0x20, 0x10, 0x06, 0x07 |
| `func_8007866C` | `holiday_club_select` | 6.2 |
| `func_8007894C` | `holiday_mode_stage_club_join` | stage 7 |

### 13.1 Shared main symbols and owner requests

This object uses the following shared main names. Names marked “pending” are proposals of the owning unit, not applied ledger names. This unit does not rename them.

| symbol used here | owner name / status | use from this object |
|---|---|---|
| `appointment_find_today` | `hud_appointment_find_today`, pending in `config/renames/proposals/m-hud-apply.tsv` | appointment icon, appointment launch and birthday branch |
| `func_8006B640`, `func_8006BDA8`, `func_8006B0C8` | `hud_appointment_prompt_noop`, `hud_command_roll_poor_result`, `hud_appointment_list_sort`, pending in `config/renames/proposals/m-hud-apply.tsv` | empty prompt hook, command result roll and appointment ordering |
| `func_8006A044`, `func_8006A2CC`, `func_80066A2C`, `func_80066A84` | `hud_magazine_mark_read`, `hud_magazine_page_build`, `hud_season_index`, `hud_in_summer_months`, pending in `config/renames/proposals/m-hud-apply.tsv` | magazine page, background season and sprite-set selection |
| `club_list_menu` | `hud_club_list_menu`, pending in `config/renames/proposals/m-hud-apply.tsv` | shared club-join initializer |
| `func_8006D138`, `func_8006F524` | `schedule_progress_state_update` and `schedule_command_girl_select`, applied in `config/renames/scheduler.txt` | magazine exit updates progress; club command selects the companion |
| `func_8005352C`, `func_80053564` | `girl_player_sports_club_check`, `girl_find_by_club`, pending in `config/renames/proposals/m-menutext-apply.tsv` | sports-club tests and club-member lookup |
| `func_8005AC70`, `func_8005B0F4`, `func_8005B1A8` | `command_screen_icons_reset`, `room_click_radio_holiday` (HYPOTHESIS for “radio”), `room_click_mascot_holiday`, pending in `config/renames/proposals/m-sysmenu-apply.tsv` | icon availability reset and the two room click handlers |
| `D_800B5960`, `D_800B6780`, `D_800B6810`, `D_800B6890`, `D_800B68A8`, `D_801230D0` | `room_bg_sectors_holiday`, `club_bg_sectors`, `club_sd_set_sectors`, `club_sd_set_summer`, `club_sd_set_other`, `scene_work`, proposed in `config/renames/proposals/m-weekday.tsv` | shared room background, club backgrounds/sets and activity-set scratch word |
| `D_800B3CFC` | `pad_code_entered`, proposed in `config/renames/proposals/m-flow.tsv` | magazine hint gate |
| `D_80125C90` | `icon_enabled`, pending in `config/renames/proposals/m-hud.tsv` | icon availability reads and writes |

Request to the shared core owner: the low and high nibbles of `LastCommandBits` are read and written as the school-day and holiday commands here; use one coordinated field rename pair (`weekday_command`, `holiday_command`) because both main command-screen objects share the type. The m-holiday executable batch contains no field rows.

Follow-up outside this batch: add one-line English definition comments for the 11 game-side O.BIN functions listed above, based on their bodies; decide the shared `LastCommandBits` field names with the core owner; run `rename.py const` separately for the `HolidayModeStage` enum and constants if retained; defer a separate file rename until the symbol/header work is reviewed. The data label `D_800B6CE0` is also deferred: the code selects index 13 only for year 0x61, month 8, but this pass does not establish a safe descriptive name for that string. `ClubFlagBits.b29` remains a hypothesis because this source only reads it and no writer was found in the inspected code.

Exact deferred draft-row counts: 1 data (`D_800B6CE0`), 4 field, 73 constant, 12 comment and 1 file row. The executable batch covers all 28 placeholder functions; none is deferred. Shared bit-field ownership and executable constant sites require their own reviewed batches, comments must be reconciled with current definitions, and the file move comes separately.

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
2. **`appointment_place_0..3` (core, HYPOTHESIS rows).** Section 7 shows their roles: chosen place, true place, two decoys. Suggested names `appt_place_chosen`, `appt_place_true`, `appt_place_decoy_a`, `appt_place_decoy_b`. Not renamed here.
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
13. **Shared command nibbles.** `LastCommandBits.low/icon` is a request to the shared core owner for coordinated names `weekday_command` and `holiday_command`; these fields are deliberately absent from the m-holiday executable batch.
14. **Day-end expulsion** on a skipped club day lives in `day_event_dispatch` / TEL stage 8 (section 6.3); this unit only sets the cursor and message. The exact calendar day the test sees is for the scheduler spec.

T-27050 independent review: all newly proposed magazine symbols use the owning holiday_magazine_ prefix; the O.BIN `magazine_exit` is preserved. The second-controller hint expands pad2 to second_pad in its proposed identifier.

T-27050 final verification (2026-10-24): independent Sol 6.1 review PASS for this executable table and reconciled spec. Docker `tokimemo-decomp-arm64 --cpus=2`, `python3 tools/rename.py plan config/renames/proposals/m-holiday-apply.tsv --ledger m-holiday` exited 0 with 49 rows, 5 affected files, no PROBLEM. Combined four-table validation found no duplicate source keys or target names. No application or build was performed by this lane; the integration owner must apply and obtain clean 27/27 hashes before source completion.

## 16. Source filename and topic-header layout (recommendation)

The existing `src/main/80075320.c` is one original object, from `0x80075320` up to the next object at `0x800789E0`. The separate proposal `config/renames/proposals/m-holiday-files.tsv` recommends old stem `80075320` to new stem `holiday`. The object contains the days-off handler, appointments, telephone, magazine, and club flows; the module prefixes are `holiday_` and `appointment_` (ADR 0001, `wiki/phase2.md`, and `GLOSSARY.md`). Preserve this as one C/object unit. This file row cannot create or split headers.

The Docker dry-run of that one-row file proposal succeeded: it resolved `src/main/80075320.c` to `src/main/holiday.c`, identified the matching YAML and asm path changes, and reported three files to change. It made no changes.

Recommended declaration layout, as a separate migration after symbol names are applied and reviewed:

| new header | guard | owned declarations | include dependencies and boundary |
|---|---|---|---|
| `include/main/holiday.h` | `MAIN_HOLIDAY_H` | `holiday_mode_handler` (`0x80075320`); holiday mode/open/menu/dial functions `0x80075468`-`0x80076000`; activity/magazine functions `0x80076EC0`-`0x80077E30`; holiday club select/join functions `0x8007866C`-`0x8007894C`; the 20 holiday_ data symbols in `0x800B6990`-`0x800B6D30` (appointment_choice_suffix belongs to appointment.h); retained game-side O.BIN names `holiday_tel*`, `holiday_club*`, `holiday_taibu_club_exit`, `magazine_exit`, `telephone_class_init` | `common.h` only for current fixed-width signatures; no game.h/main_api.h include and no cross-topic include unless a declaration actually needs its type. Keep O.BIN names and comments. |
| `include/main/appointment.h` | `MAIN_APPOINTMENT_H` | `appointment_place_candidates` (`0x800764AC`), `appointment_start` (`0x80076630`), `appointment_place_menu_build` (`0x800767A4`), `appointment_place_choose` (`0x80076B48`), `appointment_place_confirm` (`0x80076C7C`), existing `appointment_launch` (`0x80076DA0`), and data `appointment_choice_suffix` (`0x800B69BC`) | `common.h` only for current fixed-width signatures; no umbrella include. |

The preceding paragraph records the pre-T27061 umbrella transition. Current declarations are resolved through `config/header_owners.json`; `GameState` is in `include/game/game_state.h`, shared appointment records and enums are in their registered topic owners, and `main_api.h`/`game.h` are retired. Preserve `MAIN_API_OVERRIDE_func_80053418`, its matched `void ()` view in the defining C file, all existing override guards and `()` declarations. Any type-only move belongs to the shared owner; neither holiday header duplicates a declaration. The earlier snapshot/compare and umbrella-deletion sequence is historical; header migration and file rename remain separate operations. Address ranges above describe object ownership; the individual listed function boundaries are the authority within each group.

The dependency inventory below was written before topic-header integration. The external main APIs resolve to their registered headers under `include/main/` and must not be copied into either holiday header. Keep the listed dependency groups as ownership referrals; use `config/header_owners.json` for the current declaration path. The statement that this checkout had not created `include/main/` is obsolete.

## 17. Proven constant inventory (separate, site-scoped proposals)

This is an inventory for separately reviewed `rename.py const` rows, not blanket numeric replacement. The reviewed 49-row function/data batch stays frozen. Reuse `game_enums.h` and existing `PAD_*` masks first. Same-valued literals at unrelated sites are not automatically the same constant.

| family | source evidence and exact scope | safe treatment / type cautions |
|---|---|---|
| Girl, club, place, weekday IDs | `holiday_shared_birthday_scene_due` (`0x800754C0`) refers to girl 0; `holiday_open_init` (`0x800755B4`) checks Sunday and sports-club eligibility; `holiday_icons_set_available` (`0x80075AF8`) counts 11 club flags and checks no-club; `holiday_menu_click` (`0x80076000`) dispatches command/icon 4; `appointment_place_candidates` (`0x800764AC`) and `appointment_place_confirm` (`0x80076C7C`) filter seasonal place IDs 16-20 and index a 32-entry count array; `holiday_activity_pick_sd_set` (`0x80076FDC`) indexes club tables; `holiday_activity_play` (`0x80077900`) tests Sunday, command 4, club 10, and the 730-day cutoff. | Reuse `GIRL_SHIORI`, `GIRL_YOSHIO`, `GIRL_MIHARU`, `WEEKDAY_SUNDAY`, `CLUB_NONE`, and `PLACE_POOL`..`PLACE_SKI_RESORT` only where domains match. Command 4 is not automatically the same as icon 4. The array bound 32 is not a Place enumerator. Keep packed values at source width. |
| Command, icon, and phone row indices | `holiday_phone_dial_shortcut` (`0x80075C84`) matches 13 rows of 7 digits, including row 11/12 behavior; `holiday_menu_click` (`0x80076000`) records command choices and dispatches icons 4, 8, 9, 10, 11, 12; `holiday_activity_load` (`0x8007739C`) uses command 0-7 and `0x15 + command % 9`; `telephone_class_init` (`0x800782F0`) lays out 14 entries with special rows 0 and 13; `holiday_tel_call` (`0x80078058`) maps the Yoshio/Miharu special rows. | Keep these domains separate: holiday command IDs 0-7; icon IDs 0-13; phone rows 0-12; seven dial digits. Use girl enums for girl comparisons. Row 11/12 is a telephone-table alias, not a `GirlId`. Keep `menu_cursor_0 == -1` signed. |
| Calendar, seasons, birthdays, club dates | `holiday_shared_birthday_scene_due` (`0x800754C0`) requires matching birthday month/day fields, month 3 or month 4 with `day < 4`, appointment lookup `<= 0`, `birth_day_check(0xF) == 1`, and Shiori state bit `0x80`; `holiday_open_init` (`0x800755B4`) computes third Sunday as `(day+6)/7 == 3`, checks month `% 3 == 0`, and uses season helpers; `holiday_club_exit` (`0x800787B4`) repeats the third-Sunday and quarterly-match condition; `holiday_magazine_hint_index` (`0x80077CA8`) buckets months at 2, 5, 8, and 11 and has a year `0x61`, month 8 case. | Reuse `WEEKDAY_SUNDAY` and existing season helpers. Name the third-Sunday ordinal and quarterly-month condition only at those sites. Month/day fields are unsigned bytes; month is 1-based. Do not treat a literal 3 globally as “March” or “third Sunday.” Keep year `0x61` local absent a shared year convention. |
| Existing GameMode / DayEventType reuse | `holiday_open_init` enters0x5D; appointment paths enter0x50 and queue0x80; holiday telephone flow enters0x45; activity play enters0x60; holiday_club_exit enters0x73. | Reuse `GAME_MODE_SPECIAL_EVENT`, `GAME_MODE_DATE`, `GAME_MODE_TELEPHONE_PROFILE`, `GAME_MODE_CLUB_CAMP_2` (retains core HYPOTHESIS), `GAME_MODE_PRACTICE_MATCH` and `DAY_EVENT_APPOINTMENT`. These are actual mode/queue sites, separate from local stage IDs; preserve the existing GameMode roles0x11OPENING/0x13FILE_SELECT/0x14NAME_ENTRY/0x15PROLOGUE. |
| Mode stages | `holiday_mode_handler` (`0x80075320`) switches on stages `0`, `1`, `5`, `6`, `7`, `0x10`, `0x20`, `0x30`, `0x40`, `0xFE`; the dispatchers `holiday_mode_stage_open/menu/activity/appointment/magazine/club_join` define their local behavior. | A separate `HolidayModeStage` enum is supportable only for these `mode_stage` sites, with `HOLIDAY_MODE_STAGE_` names. These are local stages, not `GameMode`; 0x30/0x40 handlers belong to settings and should use names owned there if supplied. |
| Flags and masks | `holiday_mode_handler` checks actor 27 draw flag `0x80`; birthday helper checks girl state `0x80`; `holiday_open_init` checks player upper flag bit 0 and health low nibble; `holiday_phone_dial_shortcut` masks input by `0x70`, dispatches `0x20`, `0x40`, `0x10`, masks the random bonus by `0x7FFF`, then adds it to tokimeki, and adds stress `0x20`; appointment helpers decode active and low-five-bit place; activity play checks animation bit 0 and club flags; `holiday_club_select` (`0x8007866C`) sets club flag bit 0 and updates the player club nibble with mask `0xFF0F` and value `0xB0`. | Reuse `PAD_CIRCLE`, `PAD_CROSS`, and `PAD_TRIANGLE` for the dial switch values0x20/0x40/0x10; other masks need their own exact field context. Dial key selectors should be scoped to its switch. Equal `0x80` values on actor/girl/packed data are distinct. Club flag bit 29 remains a hypothesis; defer it. Use unsigned masks for packed fields and preserve explicit signed extraction where the body uses sign shifts. |
| Bounds, sentinels, and frame thresholds | `holiday_open_init`: seven-key ring; `holiday_phone_dial_shortcut`: seven digits and 13 rows; `appointment_place_candidates`: 8 slots, 32 counters, stops at first inactive, returns total distinct count while copying at most 3 and writes the slot-0 active bit when fewer than two distinct choices exist; `appointment_place_menu_build`: up to 3 choices; `holiday_activity_play`: timer `0x20` event and waits through `0x40`; `holiday_tel_exit` (`0x80078148`): tests every 8 ticks; `holiday_activity_play`: day count `< 0x2DA` and club exp increment/test of 6; `holiday_taibu_club_exit` (`0x80078858`): pad mask `0x60`. | Candidate names belong only to the listed loops, comparisons, or writes. Preserve signed `-1` and helper return semantics; use `u8/u16/u32` for source-width state and `s32` for signed return/count paths. Counter capacity 32 does not mean 32 valid places. |
| Sound/UI arguments | `holiday_menu_click` (`0x80076000`) issues click and selection sounds; `holiday_activity_play` (`0x80077900`) emits `0x501`/`0x502`; dial/telephone handlers use `0x504` and the `0x505`-series; `telephone_class_init` uses `i/5`, `i%5` and fixed coordinates. | Reuse current `UiSe` values from section 12 only where present in the owning ledger. Scope any new sound IDs to actual calls. Do not name coordinates or arithmetic literals without a clear repeated domain; preserve the call argument width. |

Unknown flags and unverified interpretations stay deferred. In particular, the evidence here supports code conditions and arithmetic, not the meaning of all fields or all equal-valued literals. A constant batch must record its exact owner function and exact use site.

T-27050 second-work-package verification (2026-10-24): independent Sol 6.1 review PASS for the filename/header recommendations and constants inventory, with all findings resolved. The separate m-holiday-files.tsv Docker plan exited0 with 1 rows/3 affected files and no PROBLEM. This review approves recommendations, not implemented headers/constants. Existing symbol batch remains frozen; no source move, header extraction, literal replacement, apply or build was performed.

## 18. Essential local names and constant-site follow-up (T-27062)

The reviewed 49-row `m-holiday-apply.tsv` and one-row `m-holiday-files.tsv` remain frozen. The 36-row supplementary local/parameter packet is `config/renames/proposals/m-holiday-locals.tsv`; it scopes every row to one current definition in `src/main/holiday.c`, using current placeholder function names so the tool can resolve after the pending function-ledger chain. Its names cover the notice text/count and name buffer in `func_800755B4`; Sunday club quit count; dial-ring read index and random value; event queue and player-flag reads; click-handler result and Sunday/weekday club-member lookups; appointment candidate output/counts/IDs and menu strings; choice scratch passed by `func_80076630`; appointment confirmation text; activity command and normalized club-partner column; animation-result flags; club member/id; phone digit index; and club-departure message. `place_choices_out` retains the helper's dual contract; `place_choices_scratch` names the caller's output buffer, not a meaningful count. The Docker planner result is recorded after the final table version below.

The packet does not rename simple `i`/`j` loop counters; CD/background return statuses (`r`); graphics coordinate/geometry arrays (`x`, `y`, `w`, `h`); animation actor/layout temporaries; any FAKE stack-padding or unused locals (`pad*`, `top_gap`, `list_gap`, `temp_hi`, the birthday helper's `rank`/`points`); or generic copies that lack a stable game-logic role. It preserves signed return/result values, unsigned packed views, and caller-visible state. In particular, `appointment_place_candidates` returns 1 immediately for a seasonal slot-0 place without writing the output; otherwise it scans only the contiguous active prefix (up to eight slots), counts distinct non-seasonal place IDs, and, when that count is below two, writes slot-0 active state to output word 0 and returns 1. At two or more distinct choices it copies at most three IDs in appointment order and returns the full distinct count. The caller `func_80076630` uses the count to choose the place menu or launch path; it does not read the below-two output word. The dial's `rand()` result is ANDed with `0x7FFF` before it is added to the matched phone row's full tokimeki-fraction word; preserve the full-width write and the phone row 11/12 alias behavior.

`config/renames/proposals/m-holiday-constant-sites.tsv` is the source-wide site inventory for proposed/reused constants and explicit deferrals. Its header is `function`, `expression`, `value`, `name`, `evidence`. New constants that can be tied to exact locals after the local packet lands are the dial tokimeki mask (`dial_random_value & 0x7FFF`), minimum distinct appointment places (`distinct_place_count < 2`), and maximum copied menu choices (`copied_choice_count >= 3`). Their values are in `config/renames/proposals/m-holiday-constants.txt`; the future const dry-run belongs in the future holiday-owned header once it exists, with `--defines`, those values, and sites `dial_random_value`, `distinct_place_count`, and `copied_choice_count`. The local-site dry-run is not a const dry-run: the local identifiers do not yet exist in source. The ring capacity 7 is body-backed (`D_800B6998 >= 7`), but the data-symbol site also sees three zero reset writes unresolved; club activity points 6 appears in `practice_points += 6` and the first-activity `== 6` gate, but a field-wide site sees 32 unrelated threshold literals. Both are recorded for review and excluded from the immediately safe values file.

The site manifest also records exact-context blockers for each family from the previous inventory: reuse existing `GirlId`, `Club`, `Place`, `Weekday`, `HealthState`, `GameMode`, and `DayEventType` names; reuse pending `WeekdayCommand/COMMAND_*` for commands; coordinate icon and sound IDs with m-hud/shared UiSe owners; keep mode stages separate from game modes; defer shared `mode_stage`, `mode_step`, `substep`, calendar fields, packed appointment bits, health/remedial flags, club flags, timer/day-count fields, magazine year/month indexing, nested progress-state arithmetic, actor layout values, and audio digit arithmetic where current const-site matching is too broad or cannot represent the expression. `ClubFlagBits.b29` remains a hypothesis, not a proven flag constant. No literal replacement or const application is authorized by these proposals.

For the three local-scoped candidates, the eventual review-only command is:

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/rename.py const --header include/main/holiday.h --defines \
  --values config/renames/proposals/m-holiday-constants.txt \
  --site dial_random_value --site distinct_place_count --site copied_choice_count
```

That command is intentionally not run in this lane: `include/main/holiday.h` does not yet exist, and the exact local identifiers are still proposal rows rather than source identifiers. The manifest currently has 43 five-column site records covering the reused ID domains, command/icon/phone rows, calendar/birthday rules, mode stages, modes/queue types, flags and masks, appointment packing, bounds/thresholds, activity/club gates, magazine indexing, actor layout, and sounds. The separate three-value file contains only sites that can be isolated by those unique locals after the local batch; broad field/data sites remain evidence-only, not a proposed command.

T-27062 review resolution: the manifest has 43 five-column records. Phone-book items0/1..11/12/13 are distinct from HUD icons; the random mask stays signed s32 without a U suffix. Distinct/copy count selectors also hit their zero initializations, which remain explicitly unresolved mathematical zeros in the three-value candidate file; threshold-only application is not claimed. The club_partner_column name denotes source return0 versus any nonzero value, including15, rather than existence of a girl partner.

T-27062 final review (2026-10-24): independent Sol review passed the 36-row local/parameter packet and corrected 43-record manifest. The author's local plan passed with exit 0, 36 rows, one affected file and no warnings. The final normalization changed whitespace to five-column TSV and clarified evidence without changing rename operations, so that verified plan is reused under root's resource rule. The frozen 49-row symbol packet was revalidated after main file moves: exit 0, six affected files. No holiday constant dry plan or application is claimed; future local selectors and the missing owner header still gate that step.

T-27062 post-application revalidation (2026-10-24): after root applied `config/renames/main-ui.txt`, a fresh Docker plan of the unchanged 36-row local packet passed with exit 0, one affected file and no `PROBLEM`. Historical function scopes resolve through the applied ledger to current named definitions; independent Sol follow-up review passed their unchanged meanings. Evidence: ignored `.claude/orchestrator/state/T-27062-holiday-post-main-ui-plan.txt`. The new exact nonnegative `--reuse` subset may support individual existing-enum comparison/subscript sites once selectors are prepared and proved; it does not remove the future-local, signed-literal or unsupported-form gates for the three new holiday candidate constants.
