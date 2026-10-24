---
type: spec
unit: m-scheduler
updated: 2026-10-24
sources: ["src/main/8006CB30.c", "include/game.h", "include/main_api.h", "config/renames/scheduler.txt", "config/renames/core.txt", "config/renames/proposals/m-scheduler.tsv"]
---

# Port spec: scheduler (m-scheduler)

Roadmap phase 2, unit `m-scheduler` ([[tickets/T-27011-p2-m-scheduler]], parent [[tickets/T-27000-phase-2]]). The object `src/main/8006CB30.c` (0x8006CB30-0x800737A0) has 56 function definitions and implements the weekly day-event cycle. Applied shared names come from the committed ledgers and current headers. [[tickets/T-27041-scheduler-readable-batch]] applies 34 function and six data names; its evidence is in `config/renames/scheduler.txt`. Other field/constant/file suggestions remain proposals. Historical emulator observations below are inherited from the original analysis, not new runs in this continuation.

Suggested future file name: `src/main/scheduler.c`; prototypes currently remain in `include/main_api.h`. O.BIN names stay unchanged, including `schedule_init`, `_schedule_init`, `last_date_spot_timer_dec`, `restore_bgm`, `geko_judge`, `date_sasoi_judge`, `schedule_girls_gakko`, `yuukou_down`, `syoushin_up` and `schdeule_girls_param`. The non-O.BIN name `special_event_select` is now `schedule_special_scene_select`. New symbols owned by this object use `schedule_` or `day_event_`, per ADR-0001. Original analysis table: `config/renames/proposals/m-scheduler.tsv`; reconciliation decisions: [[spec/reconcile]].

## 1. What the system does

The game advances in weeks. The player picks a weekday command on the command screen (mode 0x34, other unit); that hands over to this unit, which runs five steps once per week and then lets the day screen play the week one queue record at a time:

1. `MODE_SCHEDULE` (0x30, `schedule_init`): builds the queue of events for tomorrow through Sunday, resolves random placeholders, applies the weekly girl decay, advances the calendar by one day, goes to 0x31.
2. `MODE_DAY_SCREEN` (0x31, `schedule_day_screen_handler`, stage 0 step 0 is `day_event_dispatch`): reads `day_events[day_event_index]`, applies fixed-date rules and phone/appointment checks, and starts the mode for the record. Every scene ends through `day_event_next`, which moves the index on and returns to 0x31. At a day-end record (0xFB) the reader advances the calendar itself.
3. When the queue shows 0xFF (end of the week) the reader goes to the command screen (0x34) and the cycle repeats.

Modes owned here: 0x30 (schedule), 0x31 (day screen; its only stage, 0, is the queue reader, and the HUD draw set runs every frame), 0x32 (`schedule_week_end_mode_handler`: weekly health check, queue type 0xFC), 0x38 (`schedule_month_start_mode_handler`, type 0xFE), 0x39 (`schedule_day_notice_mode_handler`, type 0xFD).

The builder always starts from tomorrow. The queue of a week ends with the Sunday holiday record, which has no day-end record, so the calendar still shows that Sunday when the reader reaches the end marker and the command screen opens; the next build therefore starts on Monday and covers Monday to Sunday. The first week of the game starts on Tuesday 1995-04-04 and is shorter. Probe (mode 0x30 poked from the title-menu state, command 0): the queue was `00 00 00 0f | fb | 00 00 00 0f | fb | 00 00 01 0f | fb | 00 00 01 0f | fc | fb | 10 | ff...` = Wednesday to Saturday school days, the week-end record 0xFC after Saturday, then Sunday as a holiday record (0x10, no 0xFB for Sunday). A second probe on Sunday 1995-04-23 with command 3 gave five school days (Monday to Friday), then 0x10 for Saturday (national holiday of April 29 in the month table), `fc fb`, `10`.

## 2. Function map

| address | name | role |
|---|---|---|
| 8006CB30 | `schedule_init` | mode 0x30 handler (stage 0 runs the step switch, then the HUD draw set) |
| 8006CB6C | `_schedule_init` | step switch 0-4 |
| 8006CD14 | `schedule_step_prepare` | step 0 |
| 8006CDD4 | `schedule_memory_card_save_poll` | step 0 substep 1: wait for the memory-card save job |
| 8006D7EC | `day_event_queue_build_week` (core) | step 1: the queue builder |
| 80070AD4 | `schedule_girls_gakko` | step 2: resolve encounter placeholders |
| 80070E90 | `schdeule_girls_param` | step 3: weekly girl decay |
| 80070ECC | `schedule_step_advance_day` | step 4 |
| 8006CC28 | `last_date_spot_timer_dec` | repeat-date-spot timers |
| 8006CCE4 | `restore_bgm` | start the default BGM when none plays |
| 8006CE84 | `schedule_command_icons_lock_to_record` | enable only the command icon of the current record |
| 8006CF38 | `day_event_queue_clear` (core) | fill the queue with 0xFF |
| 8006CF80 | `girl_flags_bit0_clear` (core) | clear flag bit 0 of girls 0-10 |
| 8006D00C | `schedule_amusement_park_flag_reset` | clear player flag bit 18 in September |
| 8006D038 | `schedule_girl_spots_expire` | quarter-month cleanup of date spots |
| 8006D138 | `schedule_progress_state_update` | seasonal updates of the 21 `progress_state` flag bytes |
| 8006D4A0 | `schedule_club_flag0_clear` | clear club record flag bit 0 |
| 8006D52C | `day_event_queue_push_birthdays` | append birthday records for a date |
| 8006D6E0 | `day_event_queue_resync_after_master` | queue repair after a club tournament scene (called by MASTER) |
| 8006EA40 | `schedule_miharu_scene4_ready` | gate of the 1998-02-15 special event |
| 8006EB64 / 8006EEC4 / 8006F218 | `schedule_meeting_select_catchup` / `schedule_meeting_select_normal` / `schedule_meeting_select_club` | choose a girl for a type-0x48 meeting record |
| 8006F524 | `schedule_command_girl_select` | the girl shown with the week's command |
| 8006FB4C | `schedule_special_scene_select` | pick a one-shot special event |
| 80070694 / 80070850 | `geko_judge` / `date_sasoi_judge` | walk-home encounter girl / invitation girl |
| 80070C44 / 80070E10 | `yuukou_down` / `syoushin_up` | affection decay / syoushin growth |
| 80070F14 | `schedule_day_screen_handler` (core) | mode 0x31 handler |
| 80070F80 | `schedule_day_screen_step_dispatch` | stage 0 step switch of mode 0x31 |
| 80070FD0 | `schedule_ending_start` (core) | step 1: start the ending |
| 80071038 / 80071280 | `girl_counters_clamp` / `girl_flag4_may_clear` (core) | housekeeping at the top of each reader pass |
| 80071110 | `schedule_yuina_highest_points_check` | gate of the 1998-02-28 special event |
| 8007132C | `day_event_dispatch` (core) | the queue reader |
| 800722C4 | `schedule_quarter_flags_rotate` (core) | month-change step of the calendar |
| 80072338 | `calendar_advance_day` (core) | one day forward |
| 8007259C | `day_event_today_clear` (core) | clear the per-day bytes |
| 800725B0 / 800726F0 | `schedule_day_notice_mode_handler` / `schedule_vacation_notice_step` | mode 0x39 |
| 800728A4 / 80072944 / 80072998 / 80072B20 | `schedule_month_start_mode_handler` / `schedule_month_start_step_dispatch` / `schedule_month_data_load_step` / `schedule_month_start_music_wait` | mode 0x38 |
| 80072B5C | `day_event_next` (core) | return to the day screen after a scene |
| 80072BC0 / 80072C68 / 80072CA0 | `schedule_week_end_mode_handler` / `schedule_week_end_hud_init` / `schedule_health_step_dispatch` | mode 0x32 |
| 80072D70 | `schedule_health_check_roll` | decide injury, neurosis, sickness or recovery |
| 80073198, 800732F8, 800733D8 | `schedule_health_injury_start`, `schedule_health_neurosis_start`, `schedule_health_sick_start` | start a condition |
| 800734C4, 800735B8, 800736AC | `schedule_health_injury_recover`, `schedule_health_neurosis_recover`, `schedule_health_sick_recover` | end a condition |

Core already names 12 of the 56 (the rows marked core); this table does not repeat them in the TSV.

## 3. State read and written

Names are the core proposals. Offsets are from `game_state` (0x800E6280).

| field | offset | use here |
|---|---|---|
| `vacation` | 0x3D | 0 term, 1 summer, 2 winter, 3 spring; set by `schedule_vacation_notice_step`; the weekly girl decay and the angry-call rule run only in term |
| `year`, `month`, `day`, `weekday` | 0x3E-0x41 | calendar; year 95 = 1995 (the game ends in March 1998, year 98) |
| `day_count` | 0x42 | days since the start; 0x2D6 (726) and 365 are the two-year cutoffs |
| `month_calendar[2]` | 0x44 | this month and next: `day_kind[d]` (0 school day, 1 Sunday, 2 holiday, 3 break day; `[0]` = days in the month) and `marker[d]` (`[0]` = weekday of the 1st) |
| `player_flags` | 0xF4 | bits 8-11 health, 12-15 club (11 = none), 16 remedial week (HYPOTHESIS), 18 amusement park done (HYPOTHESIS), 21-23 `recovery_days` |
| `progress_state` | 0x120 | 0x9C bytes: flag bytes 0-0x14 (HYPOTHESIS: date-spot tip entries, section 3.1), command counters at +0x15 (nine bytes, one per `command % 9`, incremented once per queue build), snapshot at +0x20 |
| `girls[i]` | 0x1BC + 0x38 i | `tokimeki`, `affection`, `syoushin` (16.16 words, value halves at +2/+6/+0xA), `flags`, `birthday`, `event_counters[0x24]` |
| `appointment_slots[8]` | 0x54C | bit 29 active, bits 25-28 girl; slot 0 is the next one |
| `event_flags[0xFC]` | 0x56C | one-shot event bytes (section 10) |
| `clubs[12]` | 0x66C | u16 exp, level byte (low nibble is the team level, HYPOTHESIS), flag byte |
| `day_events[32]`, `day_event_index` | 0x69C, 0x71C | the queue: type, girl (command id for school days), scene (1 = poor day for school days), extra (the shown girl) |
| `events_today`, `daily_event_bits`, `daily_check_done` | 0x71D-0x71F | per-day marks; `daily_check_done` stops the call checks from repeating when the reader returns to the same record |
| `girl_last_date_spot[16]`, `girl_last_date_spot_timer[16]`, `girl_event_bits[16]` | 0x72C, 0x73C, 0x74C | per-girl bytes touched by `last_date_spot_timer_dec` and `schedule_girl_spots_expire` |
| `current_girl`, `date_girl` | 0xF5F, 0x75D | `date_girl` carries the girl to the sick-visit scene 0x62 |
| `quarter_flags` | 0xF64 | read through the girl-flag bit view in `schedule_progress_state_update` (bits 1-8, set by TEL when it hands out a pending `progress_state` entry; section 3.1) |
| `last_command` | 0xF68 | low nibble = weekday command (`WeekdayCommand`), bits 4-7 = holiday command (4 = club activity) |
| `mode_step_timer`, `frame_count`, `game_mode`, `mode_stage`, `mode_step`, `substep` | 0x1104, 0x10F8, 0x1108-0x110A, 0x110D | machine state |
| `card_job`, `card_timer`, `card_result` | 0x111B, 0x1120, 0x1115 | memory-card save started by the command screen, polled by `schedule_memory_card_save_poll` |
| `system_data.seed` | 0x800E7D10 | word stepped by the timer; used as a cheap random start index (`% 11`) and for the neurosis roll (`& 0xFF`) |

Main-exe globals used: `schedule_last_picked_girl` (0x800B6740, s8, owned by this object), the five words `schedule_command_rank_bar_liberal_arts_athletics`, `schedule_command_rank_bar_art`, `schedule_command_rank_bar_trivia`, `schedule_command_rank_bar_shiori_athletics`, `schedule_command_rank_bar_science_looks` (0x800B6748-0x800B6758, owned), `talk_work` / `D_80122CD4` (0x80122CD0 / 0x80122CD4, core names; the scene and girl handed to a special-event scene), `D_80122DD0` (proposed `overlay_scratch`, 0x80122DD0, 0x100 bytes, cleared before the sports and culture festivals; users: TAIIKU as camera and 16 records, BUNKASAI and BUNKAKEN as a word array, OPTION as a buffer), `D_800B3C6C` (proposed `debug_menu_active`, HYPOTHESIS), `D_800B5BC8` (season-picture fade counter of the HUD, set to 0xEF by `schedule_month_data_load_step`).

### 3.1 `schedule_progress_state_update` windows (HYPOTHESIS: date-spot tips)

`schedule_progress_state_update` (`schedule_progress_state_update`) runs once per reader pass and from `magazine_exit` (O.BIN name; the exit of the holiday magazine step in `src/main/80075320.c`). It reads only `year` and `month` and the `quarter_flags` word (the `GirlFlagBits` bit view), and writes `progress_state` entries. Evidence for the meaning: TEL's date-spot page treats an entry below 20 as listed when its low seven bits are nonzero, and TEL's pending-entry step turns the last (highest-index) entry with bit 7 (0x80) into 1 and sets `quarter_flags` bit 1 for entry 1, 2 for entry 7, 3 for entry 3, 4 for entry 0xD, 5 for entry 5, 6 for entry 0xA, 7 for entry 6 and 8 for entry 0xB. So 0x80 reads as "a new tip is waiting" and 1 as "given" (HYPOTHESIS). The entry number is not proven to be a girl id: entry 0xE would be girl 14, and the girls are 0-13.

The first switch runs on every day of both months of a window and sets the entry to 1 when its bit is set; the second switch runs on every day of the month after the window and sets the entry to 0x80 when the bit is still clear. The year tests are exact (one year per row); every other year of those months does nothing.

| window (first switch) | entry | quarter_flags bit | month after (second switch) |
|---|---|---|---|
| June-July 1995 | 1; also entry 0xE set to 1 with no bit test | 1 | August 1995 |
| September-October 1995 | 7 | 2 | November 1995 |
| December 1995 and January 1996 | 3 | 3 | February 1996 |
| March-April 1996 | 0xD | 4 | May 1996 |
| June-July 1996 | 5 | 5 | August 1996 |
| September-October 1996 | 0xA | 6 | November 1996 |
| December 1996 and January 1997 | 6 | 7 | February 1997 |
| March-April 1997 | 0xB | 8 (byte +1 bit 0) | May 1997 |

Every November (any year) the function also clears `quarter_flags` bit 15 (byte +1 bit 7); no reader of that bit was found in this unit.

## 4. The five schedule steps (mode 0x30)

`schedule_init` runs the step switch only while `stage == 0`; the HUD draw set (`parameter_show`, `message_window_show`, `hizuke_show`, `hud_icon_bar_update(0)`, `hud_mascot_update`, `hud_screen_frame_draw(1)`) runs every frame.

| step | function | work |
|---|---|---|
| 0 | `schedule_step_prepare` | substep 0: `restore_bgm` unless the record at the queue cursor (`day_events[day_event_index]`) is an exam (0x40); step 1 has not rebuilt the queue yet, so this reads last week's queue, normally its 0xFF end marker, and the test is effectively always true (the builder's own `day_events[0] == 0x40` test is a different check); hide the window title; `last_date_spot_timer_dec`. Substep 1: `schedule_memory_card_save_poll` until it returns 1. Substep 2: `mode_step_next` |
| 1 | `day_event_queue_build_week` | section 5 |
| 2 | `schedule_girls_gakko` | section 8 |
| 3 | `schdeule_girls_param` | section 13 |
| 4 | `schedule_step_advance_day` | `calendar_advance_day`; stop CD audio if it is playing; `game_mode_set(0x31)` |

`schedule_memory_card_save_poll` (nothing pending returns 1 at once and clears `card_busy`): while `card_job == 1` it runs the card step machine; result 0x13 = finished, play sound 0x50F; `card_timer` above 0x200 or result 0xF0 = error, play sound 0x502; both stop the card job.

## 5. The week queue builder (`day_event_queue_build_week`)

Input: the calendar (today is the day the command was given), `last_command`, the flags and scores. Output: `day_events[0..]` and `day_event_index = 0`. Record layout: type, girl, scene, extra (bytes 0-3).

Setup, in order:
1. `progress_state[0x15 + (command % 9)] += 1`; `girls[11].event_counters[0]` -1 if above 0 (a 4-week cool-down set by the girl 11 special event); `sel = schedule_command_girl_select(command) & 0xF`; `cnt` = number of met girls among 1-9; `schedule_last_picked_girl = -1`; `recovery_days` -1 if above 0; `day_event_queue_clear`.
2. `wd` = tomorrow's weekday; `day` = today's day; if today is the last day of the month: record 0 = 0xFE and the table index for tomorrow switches to next month (`monthOff = 1`).
3. Tomorrow's marker 11 (school trip): push 0x41 then 0x10, birthdays for `day + 6`, `mode_step_next`, return. The trip replaces the whole week. (This test and the marker 8 test read `month_calendar[0]` even when tomorrow is in the next month; a trip or camp starting on the 1st would be missed. No such date exists in the data seen.)
4. Tomorrow's marker 8 (club season): if the player has a club and the command is 4 (club): when the club is 6 (baseball) and its level nibble is at least 8, push six tournament days `0x25, birthdays, 0xFB` (the last three with girl byte 7), then 0x10 and birthdays for `day + 6`; any other club gets six camp days (type 0x42, girl byte 4, scene = poor-day check, extra = `sel`), then 0x10 and birthdays. Both return. A player with no club instead bumps a 2-bit counter in `clubs[11]` (HYPOTHESIS: seasons skipped) and continues.
5. Loop over the days `wd..6` (Monday = 0), `monthOff` and `day` stepping; if `day` passes the month length push 0xFE and switch month. For each day, in this order:
   - markers 6, 0xE, 0x14, 0xA, 0x10 and 2 each push a notice record 0xFD (core: day-notice; the notice shows the vacation message, section 11).
   - the festival chain (first match wins): marker 4 push 0x20 (sports festival); 0xC push 0x24 (culture festival); 0xD push 0x28 (Christmas preparation); 0xF push 0x2C (New Year preparation); 0x11 push 0x31 (Valentine); 0x13 push 0x30 (seasonal party); marker 2 in year 96: set girl 10 syoushin 0 and affection 0x32, push 0x48 (girl 10, scene 0) and `schedule_club_flag0_clear`; marker 2 in year 97: push 0x43 (career guidance) and `schedule_club_flag0_clear`; day kind 1 or 2 (Sunday, holiday): push 0x45 (girl 11, scene 4) and increment `event_flags[77]` when `schedule_miharu_scene4_ready`, else push 0x10; January 3, 1998 (year 97, month 13, day 3): 0x26 when the player is in a sports club other than baseball (clubs 7-10: `player_in_sports_club` is true for clubs 6-10 and the code also excludes club 6) and the club level nibble is at least 0xA, else 0x27 when the club is 0-5 and its exp is at least 0x17C, else an ordinary school-day record (so baseball players, players with no club and players below the thresholds get the school-day record); marker 0x12 in year 98: push 0x4A (career choice); day kind 0 or 3: the school-day branch below.
   - school-day branch: if marker 5 or the first record is already an exam, push 0x40; else push a school-day record `00, command, poor_day, sel` where `poor_day = command_check(wd)` (zero for command 7 whatever the check says). Count good days (`nNo`) and poor days (`nYes`).
   - then, still on that day: when the record just written is a poor day, it is the sixth counted school day (`nYes + nNo == 6`, normally Saturday) and the day kind is 0, `(rand & 0xF) < 2` (1 in 8) queues 0x47 with girl 12. A break day (kind 3): in winter break (`vacation == 2`) with `event_flags[73]` below `day_count / 365`, girl 10 met, her `get_g_zyotai_h` rank 0 and `(rand & 0xF) == 0` queue 0x45 (girl 10, scene 7); in summer break on a Wednesday of month 8 with `rand % 100 < 20`, at least one of girls 1-9 met and player flag bit 18 clear queue 0x49 and set bit 18.
   - on a plain school day (kind 0) of a non-exam week: outside the very first month, while `day_count < 0x2D6` and no meeting was queued this week: `schedule_meeting_select_catchup` when `cnt < 2`, else `schedule_meeting_select_normal`, each pushes 0x48 (girl, scene) and counts `nSpecial`; with command 4 `schedule_meeting_select_club` can add another. Then, at most twice per build, each school day pushes an encounter placeholder 0x44 with probability 1/4 (`(rand & 3) == 3`).
   - Saturday (`wd == 5`, `day_count >= 0x21`): in an exam week, with `rand & 1`, girl 9 met, `rand % 100 < 50`, `get_g_zyotai_h(9) < 3` and art + liberal arts + science of at least 0x12D, set the command nibble to 3 and push 0x47 with girl 9. Otherwise, when the day kind is 0, the command is not 7, `(rand & 3) != 0` and all six school days were good days: push 0x47 with girl `sel` if `sel` is neither 0 nor 0xF; if `sel` is 0 or 0xF: for commands 0-3 with `rand % 100 < 7`, girl 9 met and `get_g_zyotai_h(9) < 3`, push 0x47 with girl 9, else when `sel == 0` and the command is not 5 push 0x47 with girl 0.
   - birthdays: type 0x32 for girl 0 when it is her birthday, for girls 1-10 that are met with flag bit 6, and type 0x33 for the player unless health is 3.
   - `wd == 5` push 0xFC; `wd != 6` push 0xFB.
6. `mode_step_next`.

A type-0x47 record is a visit scene (overlay OMIMAI, mode 0x61). The queue builder queues it in three situations that are not sickness: a poor Saturday (girl 12), six good days or an exam-week condition (the command's girl, girl 0, girl 9). The core name `DAY_EVENT_SICK_VISIT` is probably too narrow (HYPOTHESIS: visit; the scene reads the last command and the girl's state, so it can be both).

## 6. The command check (`poor_day`)

`func_8006BDA8(day_offset)` (HUD unit, not renamed here) returns 1 for a poor day (the gains of the command are halved by `parameter_up_down`'s half flag, and one stress point is added) and 0 for a normal day. It temporarily adds `day_offset` to `day_count`, then:
- the command picks one biorhythm wave and a parameter: commands 0 and 1 the 33-day wave, command 2 the 28-day wave, command 3 the 23-day wave, command 5 and 6 the 28-day wave, command 4 by club (clubs 0, 2, 3 the 33-day wave; clubs 1, 4, 5 the 28-day; clubs 6-10 the 23-day); command 7 uses all three waves and a fixed base of 16; the base is wave (0..16) plus the matching parameter / 16 (liberal arts, science, art, athletics, trivia, looks) or the club exp / 16;
- the wave is `rsin((player_birth_offset + day_count) * k)` with k = 178, 146, 124 (4096 per turn, so periods of about 23, 28 and 33 days); the player offset (`func_8006C700`) is derived from `player_birthday` (5475 minus a per-month table entry minus the birth day, with a special case for early April);
- score = base + 6; stress factor f = 4 when stress is at least 97, else stress / 32; score = `rand() % (score + 4)`, then score * (8 - f) / 8; if all three single-wave tests (scale 3) are zero, score / 4; poor day when the final score is below 4.

Wave average (`func_8006C334`): each selected wave adds its value when positive and half its absolute value when negative, but a wave whose absolute value is under 0x199 (about 10 percent of the amplitude 4096, near a zero crossing: a critical day) adds -0x1000 instead; the sum is divided by the number of waves and scaled (`total * scale / 4096 / count`, 0 when the average is negative). So a day is bad mainly when a selected wave is near zero. The port can use any RNG but should keep the 23/28/33-day waves, the base, the stress factor and the threshold. 

## 7. `schedule_command_girl_select` and the shown girl

`schedule_command_girl_select(command % 9)` returns a girl id (0-10) or 0xF. For each command it looks up one or two girls and compares `rank - bar` with Shiori's `rank - bar` (the rank bytes are the first array filled by `func_80053418`, whose name `girl_scores` is only suggested in section 16 and is not a table row; a lookup in a 20 x 20 grid of weighted tokimeki / 5 and weighted affection / 5, both capped at 19; owned by `src/main/8004F870.c`):

| command | girl(s) | her bar | Shiori's own bar |
|---|---|---|---|
| 0 liberal arts | 1 | `schedule_command_rank_bar_liberal_arts_athletics` (40) | `schedule_command_rank_bar_liberal_arts_athletics` |
| 1 science | 2 | `schedule_command_rank_bar_science_looks` (55) | `schedule_command_rank_bar_science_looks` |
| 2 art | 3 | `schedule_command_rank_bar_art` (35) | `schedule_command_rank_bar_art` |
| 3 athletics | 5 and 6 | `schedule_command_rank_bar_liberal_arts_athletics` | `schedule_command_rank_bar_shiori_athletics` (45) |
| 4 club | the heroine of the player's club (clubs 0-1 girl 1, 2-3 girl 2, 4-5 girl 3, 6-7 girl 4, 8 girl 5, 9 girl 6, 10 girl 10), who must be met, have flag bit 2 and her own club equal to the player's; when Shiori shares the club her value is rank - 11 (0 without bit 2) and girl 10 is still checked in club 10 | 11 | 11 |
| 5 trivia | 8 | `schedule_command_rank_bar_trivia` (25) | `schedule_command_rank_bar_trivia` |
| 6 looks | 7 | `schedule_command_rank_bar_science_looks` | `schedule_command_rank_bar_science_looks` |
| 7 rest | none | | |

Result: the girl when she is met, her excess is positive and larger than Shiori's; else 0 when Shiori's excess is positive; else 0xF. `0` means Shiori. In the school-day record it is `extra`; the weekday handler maps it to a picture variant (0 default, non-zero another variant; command 3 with girl 5 a third variant; HYPOTHESIS: the girl appears in the picture of the command). The visit rules in section 5 use `sel` as well.

## 8. Resolving encounter placeholders (`schedule_girls_gakko`)

For each of the 32 records that is type 0x44 (placeholder):
- next record is 0x32, 0x33 or 0x47: becomes 0xFA (skip);
- health is 3 (sick): 0xFA;
- `schedule_special_scene_select` succeeds: type 0x45, girl, scene;
- `date_sasoi_judge` returns a girl and no invitation was made this build: type 0x46 with that girl (at most one per build; note the judge has already stored the girl in `schedule_last_picked_girl`);
- `geko_judge` returns a girl: the record stays 0x44 with the girl in the girl byte;
- else 0xFA.
Then `mode_step_next`. Probe: with no eligible girl all placeholders became 0xFA.

`geko_judge` (walk-home encounter): start at `system_data.seed % 11`, look at the 11 girls in rotation order; the first girl who is met, has `syoushin < 0x3C`, has `get_g_zyotai_s != 4`, whose `func_80053418` rank byte (`girl_scores`, section 16) is at least `ENCOUNTER_MIN_CLOSENESS` (0x11), passes `rand % 100 < 50`, and differs from `schedule_last_picked_girl` wins.

`date_sasoi_judge` (invitation): refuses when appointment 0 is active, when the day is in 8-14 (`(day + 6) / 7 == 2`) and the player has a club (club < 11), on December 17 and 24, and from February 1998. Otherwise the same rotation with rank at least `INVITATION_MIN_CLOSENESS` (0x21) and `rand % 100 < 8`.

## 9. Meeting selectors (type 0x48)

A meeting record (girl, scene) sets that girl's affection to 0x32 and syoushin to 0 when the reader starts it and runs mode 0x5B. The three selectors pick at most one unmet girl; all test `(met == 0)`.

| girl | condition (catch-up, used while fewer than 2 of girls 1-9 are met) | normal (2 or more met) | scene |
|---|---|---|---|
| 1 | command 0, liberal arts 0x37 | 0x4B | `frame_count & 1` |
| 2 | command 1, science 0x3C | 0x55 | `frame_count & 3` |
| 3 | command 2, art 0x32 | 0x41 | 0 |
| 5 | command 3, athletics and looks 0x51, and girl 0's club is not 8 | 0x54 | 0 |
| 6 | command 3, athletics and guts 0x1F, and girl 0's club is not 9 | 0x47 | 0 |
| 7 | command 6, looks 0x65 | 0x7E | 0 |
| 8 | by her `story_stage` 0-3: trivia 0x24, 0x38, 0x4C, or 0x5B with command 5 | same | the stage |
| 4 | player's club differs from her club, guts 0x24 | 0x60 | 0 |

The first match in the order above wins. Chance per call: catch-up 1 in 3 (`rand % 3 == 0`), normal 1 in 8 (`rand & 7 == 0`). `schedule_meeting_select_club` (only with command 4, called after the other two and able to add a second record) has a 1 in 3 chance and needs an unmet girl 1-6 in the player's own club: girls 1-5 need the club exp at least 0x15; girl 5 only in club 8 and girl 6 only in club 9, both when girl 0 is not in that club. It sets the girl's flag bit 18 and returns scene 2 + parity (girl 1, club 0) or 4 (girl 1 elsewhere), 4 (girl 2 in club 2) or 5, 1 (girl 3 in club 4) or 2, 1 (girl 4 in club 6) or 2, 1 (girls 5 and 6). Emulator probe: 1995-05-07, command 0, liberal arts 0x40: a week with a `48 01 00 00` record.

HYPOTHESIS: these are the first-meeting events (the meeting scene sets `met`); the code shows only the conditions above.

## 10. Special events (`schedule_special_scene_select`)

Run for a placeholder slot. `counts[g]` = sum of `girls[g].event_counters[1..20]`. For each girl 0-10 in order: she must be met, `get_g_zyotai_h(g) < 2`, `counts[g] != 0`, and a roll `(rand & 0xF) < 5` (5/16); then per girl (first success returns):

| girl | roll | season (`uniform season`: 1 = months 6-9, 0 = other, -1 = changeover weeks May 25+ and Sep 24+ never match) | one-shot flag | result (girl, scene) |
|---|---|---|---|---|
| 0 | 3% | frame parity odd: season 1 and `get_g_zyotai_s & 0x7F` below 2; even: none | flag 0 (odd) or 1 (even) | (0,1) or (0,2) |
| 1 | 3% | 0 | 8 | (1,1) |
| 2 | 3% | 0 | 14 | (2,1) |
| 3 | 7% | 1 | 21 | (3,1) |
| 4 | 3% | 0 | 27 (incremented, no clear check) | (4,1) |
| 5 | 3% | 1 | 33 | (5,1); or 50% with flag 37 == 1 and flag 34 clear, any season: (5,2) |
| 6 | 3% | 1 | 40 | (6,1) |
| 7 | 3% | 1 | 46 | (7,1) |
| 8 | 3% | 0 | 54 | (8,1) |
| 9 | 3% | 0 | 60 | (9,1); or with flag 60 == 1 and her flag bit 14 set, no roll: bit cleared, flag 61 +1, (9,2) |
| 10 | 3% | 1, and flag 27 nonzero | 67 | (10,1); or 10%, September 1997 day 14 or later, flag 68 clear: (10,2) |

After the loop, independent of the 5/16 roll: girl 12's flag byte 3 (HYPOTHESIS: number of telephone calls with her, incremented by TEL) at least 0x15, flag 80 clear and season 0: (12,2); at least 0x29 and flag 79 clear: (12,1); at least 0x41 and flag 81 clear: (12,3). Then girl 11: `rand % 100 < 4`, flag 77 clear, `girls[11].event_counters[0] == 0`, her `story_stage` below 5 and `day_count >= 0x1D`: increment flag 74, set the counter to 4 (four weekly builds), (11,1). Last: flag 53 clear and flag 52 == 1: flag 53 +1, (7,8). Output of a success: `(girl, scene)` for a type-0x45 record (mode 0x5D, scene host in GEKO: the reader copies girl to `D_80122CD4` and scene to `talk_work`).

`event_flags` layout, from the indices above: each heroine owns a block (girl 0 from 0, 1 from 8, 2 from 14, 3 from 21, 4 from 27, 5 from 33, 6 from 40, 7 from 46, 8 from 54, 9 from 60, 10 from 67, 11 from 74, 12 from 79) and the scene number minus one is the offset in the block. The constants are the `EventFlag` rows of the table.

Three flags are more than one-shot markers. Flag 77 (girl 11 scene 4 done) also gates other code: while it is set, `schedule_special_scene_select` stops girl 11 scene 1, `day_event_dispatch` skips the girl 11 telephone call (stage 3) and ENDING skips its girl 11 chance roll; NAME_ENT tests it for nonzero. Flag 79 (girl 12 scene 1) takes the values 0, 1 (picked here) and 2 (GEKO `func_8013C014` writes 1 or 2 from a scene outcome); `schedule_yuina_highest_points_check` and ENDING test `== 2`. Flag 82 does not fit the block pattern (HYPOTHESIS name `EVENT_FLAG_FINAL_YOSHIO`): the reader only tests it `== 0` as the gate of the 1998-02-26 girl 13 scene 0 and the scheduler never writes it, while GEKO `func_80141D74` (`src/ovl/GEKO/80141AF0.c:68`) stores `date_girl` (a girl id) in it, GEKO `func_801420E4` (same file, line 244) adds 1, and ENDING (`src/ovl/ENDING/8013B6A0.c`) passes it to `talk_names_set` as a girl id and tests it nonzero. So it is not a plain done flag, and 'girl 13 from 82' is not a block start.

## 11. The queue reader (`day_event_dispatch`) and the other modes

Emulator probe of the whole table below: a one-record queue of each type was poked into the title-menu state and the reader run; every type reached the mode listed (overlay modes are requested through loader mode 2 and appear in `overlay_target_mode`; type 0x24 was probed with the player's club at 11, giving 0x71), 0xFA and unknown types led to mode 0x34, and a type-0x44 record for an unmet girl was skipped. The branches with a girl or appointment condition (0x44 to mode 0x59, 0x46 skipped by an active appointment) were not run.

The day screen runs `day_event_dispatch` in stage 0 step 0. Each pass:

1. Housekeeping: `girl_flag4_may_clear` (May 6), `girl_counters_clamp` (tokimeki, affection, syoushin capped 0..0x80; when a girl's syoushin is below 0x32 her flag bits 16 and 17 are cleared so the forced encounter and the rumor can fire again).
2. Fixed rules: from March 1998 on (year >= 98, month >= 3, day != 0) `mode_step_next` to start the ending. On 1998-02-26, when `event_flags[82]` is 0 and at least 6 of girls 1-9 are met: `day_event_index -= 1`, special event girl 13 scene 0 (mode 0x5D). On 1998-02-28, when `schedule_yuina_highest_points_check` is true and `event_flags[20]` is 0: flag 20 +1, special event girl 2 scene 7. Then `schedule_progress_state_update` and `schedule_amusement_park_flag_reset`.
3. Forced encounter: not December 24, `events_today == 0`, weekday not Sunday, `day_kind[today] == 0`, the current record is 0xFB, the first record is not an exam and the previous record is type 0: the first met girl 0-10 with syoushin at least 0x4C and flag bit 16 clear gets bit 16 set, `day_event_index -= 1`, `events_today += 1`, the girl id written into the previous record's girl byte and mode 0x59.
4. Skips: when the first and previous record are 0x25 (tournament week) birthday records (0x32, 0x33) are stepped over; a 0x32 record is skipped when `daily_event_bits` has bit 1 (girl 0) or bit 2 (others) set, a 0x33 record when bit 4 is set; type 0xFA is stepped over.
5. Appointments, at a 0xFB or 0xFF record: `appointment_sort` (`func_8006B0C8`); if `appointment_find_today` finds an appointment: its girl in state 4 (`get_g_zyotai_s == 4`) gives telephone stage 2, a changed place history (`appt_place_1 != appt_place_0`) stage 0, an active appointment stage 1; each clears appointment 0's active bit, sets `daily_check_done` and enters mode 0x43 with that stage.
6. At 0xFF only (end of the week): sports-club players (club 6-10) get telephone stage 8 on the third Sunday of the month ((day + 6) / 7 == 3, days 15-21) unless the holiday command was 4; culture-club players (club under 6) get stage 9 on October 8, 1995, October 6, 1996 and October 5, 1997; on 1995-10-15 every met girl with `festival_call_95` and no appointment gets stage 6; on 1996-10-13 girls with `festival_call_96` get stage 7. (Stage numbers are the TEL unit's; names are not proposed here.)
7. At 0xFB (end of a day): every met girl with syoushin at least `SYOUSHIN_RUMOR` (0x3D) and rumor-done clear gets the flag set and, if any did, mode 0x44 (rumor report). Else, when `daily_check_done` is 0: 1997-07-26 telephone stage 5; in term (vacation 0) outside year 98 a met girl with syoushin at least 0x5B plays sound 0x50E and stage 4; girl 11 with `story_stage != 0`, flag 77 clear, her flag byte 3 below 7 (or 7-8 once `day_count >= 0x2DB`) and a 1 in 128 roll gives stage 3. Then `daily_check_done = 0`, `day_event_index += 1`, `calendar_advance_day`.
8. Dispatch on the record type (table below). Before type 0, only the command icon of the record is enabled.

| type | mode | before the mode starts |
|---|---|---|
| 0x00 school day | 0x40 | command icons restricted to the record's command |
| 0x10 holiday | 0x46 | |
| 0x20 sports festival | 0x72 | `girl_flags_bit0_clear`; `overlay_scratch` cleared |
| 0x24 culture festival | 0x70 if the player's club is under 6, else 0x71 | same |
| 0x28 / 0x2C | 0x42 / 0x41 | `girl_flags_bit0_clear` |
| 0x30 / 0x31 | 0x55 / 0x54 | |
| 0x32 / 0x33 | 0x57 / 0x56 | |
| 0x40 | 0x35 | `girl_flags_bit0_clear` |
| 0x41 | 0x51 | same |
| 0x42 | 0x5F | |
| 0x43 | 0x36 | same bit-0 clear |
| 0x44 | 0x59, or `day_event_next` when the player is sick, the previous record is 0x28 or 0x47, or the girl is not met | `events_today += 1` |
| 0x45 | 0x5D | `D_80122CD4 = record girl`, `talk_work = record scene` |
| 0x46 | 0x5A, or `day_event_next` when appointment 0 is active | `events_today += 1` |
| 0x47 | 0x61 | `events_today += 1` |
| 0x48 | 0x5B | `current_girl = girl`, affection 0x32, syoushin 0, `talk_work = scene` |
| 0x49 | 0x58 | |
| 0x4A | 0x37 | |
| 0x25 / 0x26 / 0x27 | 0x74 / 0x75 / 0x76 | |
| 0xFC | 0x32 (`schedule_week_end_mode_handler`) | |
| 0xFD | 0x39 (`schedule_day_notice_mode_handler`) | `girl_flags_bit0_clear` |
| 0xFE | 0x38 (`schedule_month_start_mode_handler`) | |
| 0xFF or unknown | 0x34 | |

`day_event_next(flag)` leaves a scene: when `debug_menu_active` is nonzero it returns to mode 0x22; else `day_event_index += 1` (after restarting BGM if `flag == 1`) and mode 0x31. `debug_menu_active` starts at 1 in the data section and NAME_ENT clears it when a game is started, so every normal game runs with it 0; a probe from the title-menu state, where it is still 1, ended a skipped record in mode 0x22.

### 11.1 Day notice (mode 0x39) and vacations

Stage 0 runs `schedule_vacation_notice_step`. Step 0: message window and speaker setup, then `vacation`: if a break is running, it ends (0, new-term message); else month 7 sets 1, month 12 sets 2, month 3 sets 3 (a break-starts-tomorrow message), any other month leaves 0. The season sound set is reloaded (`func_800674B0`) and the text starts. Step 1 waits for the sound bank, restarts sequence 0 and goes to stage 1, which increments `day_event_index` and returns to 0x31. Probe: months 7, 12, 3 give 1, 2, 3; month 8 with vacation 1 gives 0; months 5 and 9 with vacation 0 stay 0. Six markers (2, 6, 10, 14, 16, 20; core MARKER_NOTICE_*) therefore mean the last school day before each of the three breaks and the day before term resumes (HYPOTHESIS: which marker is which; the code toggles by state, not by marker).

`schedule_day_notice_mode_handler` also tests bit 7 of byte 3 of five HUD actors (the aliases `D_8011F113`, `D_8011ED17`, `D_8011F553`, `D_8011F3FF`, `D_8011ECD3` are actors 16, 1, 32, 27 and 0 of the pool: calendar, message window, parameter panel, mascot and cursor, by the show function each one calls) and calls that actor's show or update function when it is set, before the normal HUD draw set. The aliases belong to the graphics unit's actor pool and get no rows here.

### 11.2 Month start (mode 0x38)

Queue type 0xFE is processed on the first day of the new month (the date has already advanced). Stage 0 step 0 (`schedule_month_data_load_step`): substep 0 reads 0x95 sectors from LBA 0x9B94 to 0x801A0000 (the 36 month blobs, core section 3.2); substep 1, when the read is done, calls `func_80068938(year, month, 1)` to install the next month's calendar into `month_calendar[1]`. In months 3, 6, 9 and 12 `schedule_amusement_park_flag_reset` always runs and the HUD season-picture fade counter is set to 0xEF. On 1996-09-01 nothing else happens: the mode goes straight to stage 1 (no sound, no season reload, no notice). On every other such date sound 0xC1 plays, the season set is reloaded and the mode goes to step 1 (`schedule_month_start_music_wait`), which waits for the sound bank and restarts sequence 0; in September (1995 and 1997) the new-term notice is shown first. Other months go straight to stage 1 (increment the index, back to 0x31).

### 11.3 Calendar day advance (`calendar_advance_day`, core)

Weekday +1 (7 wraps to 0), day +1; when the day passes the month length the day is 1, month +1, and `schedule_quarter_flags_rotate` runs (months 3, 6, 9, 12: `quarter_flags` bit 14 takes bit 13, bit 13 cleared); month 13 becomes 1 and year +1; `day_count` +1; in months 3, 6, 9, 12 `schedule_girl_spots_expire` runs every day; `events_today` and `daily_event_bits` are cleared and `daily_check_done` is set to 1 when the queue index is 0, else 0; when the new weekday is Sunday (6), the remedial-week flag (player bit 16) is cleared unless the first queue record is an exam; the appointment list is re-sorted. Month lengths: 31 for 1, 3, 5, 7, 8, 10, 12; 30 for 4, 6, 9, 11; February 29 in year 96 (leap), else 28.

## 12. Weekly health check (mode 0x32, queue type 0xFC)

Stage 0: HUD initialisers, `mode_stage_next`. Stage 1: `schedule_health_step_dispatch` (step 0 decides, 1 injury, 2 neurosis, 3 sickness, 5-7 recoveries). Other stages (0xFF): the weekday handler `func_800737A0` (not part of this unit; it ends the week).

`schedule_health_check_roll` uses `limit = min(0xFE, 204 + guts / 8)` (guts higher means a lower chance):

| state | transitions |
|---|---|
| healthy (and `recovery_days == 0`) | condition under `HEALTH_CONDITION_LOW` (20) and `limit < (rand & 0xFF)`, or condition 0: injury (step 1). Stress at least `HEALTH_STRESS_HIGH` (0x51) and `limit < (seed & 0xFF)`: neurosis (step 2). Both together: sickness (step 3). Nothing: stage 0xFF |
| injured | stress test as above: sickness; condition at least `HEALTH_CONDITION_RECOVER` (31): recovery (step 5), which wins over the sickness roll |
| neurotic | (condition under 20 and the roll) or condition 0: sickness; stress under `HEALTH_STRESS_RECOVER` (70): recovery (step 6) wins |
| sick | stress under 70 and condition at least 31: recovery (step 7). Otherwise, on a school day (kind 0), start at a random girl and take the first met girl with tokimeki (weighted) at least 0x33 who passes `(rand & 0xFF) < 0x3F`: `date_girl` = that girl, mode 0x62 (visit at home). The state test in that loop uses the random start girl, not the loop girl (looks like a slip in the original; keep) |

Starting a condition shows a message and sets the health nibble (1 injured, 2 neurosis, 3 sick); waits 61 frames, then until 257 frames or a button (injury: a newly pressed button; neurosis, sickness and the recoveries: any held button). Injury only: afterwards, with 25/256 chance (`frame_count & 0xFF < 0x19`), `event_flags[75]` clear and `vacation == 0`: special event girl 11 scene 2 and flag 75 set. Sickness also starts sequence 0x202. A recovery sets `recovery_days = 3`, clears the health nibble and shows a healed message. Probe: condition 0 at the start of stage 1 gives step 1 and health 1. Health effects elsewhere: sick halves parameter gains, birthday records are not queued for the player when sick, placeholders are skipped.

## 13. Weekly girl update (`schdeule_girls_param`)

Only when `vacation == 0`: `yuukou_down` then `syoushin_up`, each over girls 0-10 that are met. `yuukou_down`: with `q = day / 7` of today: when `q == 0` (days 1-6) affection -1 (floor 0) for every met girl; when `q == 2` (days 14-20) only girl 0 loses 1. Because builds happen on Sundays this is one step a month for most girls (none in a month whose first Sunday is the 7th), up to two for girl 0. Probe: on 1995-04-16 girl 0 went from 0x32 to 0x31 and a girl not yet met stayed. `syoushin_up`: syoushin +5 for each met girl whose weighted affection (`get_h_yuukou`) is below 10. Then `mode_step_next`.

## 14. Constants and thresholds (consolidated)

| name | value | where |
|---|---|---|
| `SYOUSHIN_REARM` | 0x32 | flags 16 and 17 cleared below it |
| `SYOUSHIN_BLOCK` | 0x3C | encounter and invitation judges refuse |
| `SYOUSHIN_RUMOR` | 0x3D | rumor report at day end |
| `SYOUSHIN_FORCED_ENCOUNTER` | 0x4C | forced encounter |
| `SYOUSHIN_ANGRY_CALL` | 0x5B | term-time telephone stage 4 |
| `SYOUSHIN_CAP` | 0x80 | working cap of the three girl scores |
| `DAY_COUNT_MEETINGS_END` | 0x2D6 | meeting selectors stop |
| `DAY_COUNT_VISITS_START` | 0x21 | Saturday visits start |

## 15. Notes for the port

- The queue is a plain list of 4-byte records rebuilt every week from the calendar and the player's state. The save routine also copies `day_events` and the index (core section 8), so a mid-week save resumes at the same record; a port that saves only at week boundaries can rebuild instead.
- Keep the order of checks in the reader: appointment calls come before the fixed rules for 0xFB, and the forced encounter rewrites the previous record's girl byte; a different order changes which scene wins.
- Randomness: `rand()` calls are spread through the builder; the numbers in sections 5-10 are the probabilities to keep. Poor days and health rolls use the biorhythm and `system_data.seed`; a port can substitute any RNG.
- The month blobs are disc data (core section 3.2): read from the disc image, not from the executable.
- Many rules key on exact dates of 1995-1998 (festival call dates, 1998-02-15/26/28, 1998-01-03). They are part of the scenario, not general code.
- Flag views that overlap core's rows (the merge must reconcile them): bit 18 of `player_flags` is `PlayerFlagBits.b18` (`amusement_park_done` here) and core's const `PLAYER_FLAG_AMUSEMENT_PARK_DONE`; bit 16 is `PlayerFlagBits.b16` (`remedial_week` here) and core's `PLAYER_FLAG_16`; the 3-bit recovery counter is core's `PlayerFlagBits.recovery_days`, which this table does not repeat for the byte view `RecoveryCounterBits.f` (core renames that type `RecoveryCounterBits`). The C reaches bit 18 through the `ScheduleGirlFlagBits` view both on the player word (amusement park, `day_event_queue_build_week`) and on girl flag words (club scene done, `schedule_meeting_select_club`), so `ScheduleGirlFlagBits.b18` has two meanings and stays unnamed; the girl-word use is `GirlFlagBits.b18`.
- All numbers in this spec are as the C implements them; where the original used an unused or redundant test (for example the dead alternative of the amusement-park rule) it is left out and listed in the open questions.

## 16. Cross-unit names suggested for callees outside this file

These addresses belong to other main units; the rows are not in the table to keep one name per address, but the notes above use these meanings.

| address | suggested | meaning |
|---|---|---|
| 8006BDA8 | `command_check_poor_day` | the poor-day check of section 6 |
| 8006C334 | `biorhythm_value` | three-wave average with critical-day handling |
| 8006C700 | `biorhythm_birth_offset` | day offset from the player's birthday |
| 80066A84 | `uniform_season_summer` | 1 in months 6-9 else 0 |
| 80066ACC | `uniform_season_special` | same, -1 in the changeover weeks (May 25+, Sep 24+) |
| 8005352C | `player_in_sports_club` | player's club is 6-10 |
| 8006B014 | `appointment_count_for_girl` | number of active appointment slots of a girl |
| 8006B0C8 | `appointment_sort` | pack and sort appointment slots by date (January counted as month 13 in December) |
| 80053418 | `girl_scores` | fills 11 rank bytes and 11 points (ending-candidate score) |
| 80052E60, 80052F68 | `girl_points_shiori`, `girl_points_others` | points of girl 0 / girls 1-10 (m-menutext renamed the second from `girl_points_heroines`: the glossary keeps heroine for prose) |
| 80068938 | `month_calendar_install` | install month blob(s) into `month_calendar` |
| 800674B0 | `season_sound_set_load` | reload the season background and sound set |
| 80054388 | `card_job_step` | memory-card step machine returning the result code |

## 17. Open questions

1. `progress_state[0..0x14]` (sections 3 and 3.1): the date-spot tip reading (1 = given, 0x80 = pending) fits TEL's date-spot page and `schedule_progress_state_update` but is not proven, and the entry number is not a girl id; the TEL analyst should confirm and rename the eight `quarter_flags` bits.
2. Type 0x47 (visit): which scene it plays by cause (poor Saturday for girl 12, six good days, exam-week girl 9, sickness in mode 0x62) is the OMIMAI analyst's call; the core name `DAY_EVENT_SICK_VISIT` may need to become a neutral "visit".
3. Girl flag bits 2, 6, 12, 13, 18 are named by their uses in this unit (HYPOTHESIS); the TEL, BUNKASAI and KANGEI analysts can confirm. Bit 2 (`acquainted`) is tested with the met bit by `schedule_command_girl_select` and the holiday club scene (`func_80077900`), alone by the command screen, and not by `schedule_meeting_select_club`; the girl-record init leaves it clear for girl 0. Bit 6 is set only for girl 0 at the start (girl-record init), by TEL's profile function for the others.
4. Girl 12's flag byte 3 (a counter incremented by TEL) and girl 11's flag byte 3 (read here, writer not found) have no field names; `GirlFlagBits.rest` is that byte.
5. TEL stage numbers 0-9 used by the reader (section 11): appointment (0 place changed, 1 normal, 2 girl in state 4), 3 girl 11, 4 angry call, 5 the 1997-07-26 call, 6 and 7 festival follow-ups, 8 sports clubs, 9 culture clubs. Names belong to the TEL unit.
6. `meeting_select_*` and type 0x48 are called "meeting" after the core name `DAY_EVENT_MEETING` (HYPOTHESIS: first meetings). The scene numbers of girl 8 (0-3 by story stage) suggest a multi-part introduction.
7. `date_sasoi_judge` can update `schedule_last_picked_girl` before its invitation result is discarded. The selector rejects only its immediately preceding selected girl: consecutive repeats are blocked, but A,B,A remains possible. This is not a per-week exclusion set.
8. `ClubRecord` flag byte: the 2-bit counter (`ClubFlagBits` unk_25) incremented for the "no club" record during club seasons has no known reader name (HYPOTHESIS: seasons skipped).
9. Marker 0x17 (written by ETC after three failed subjects) has no branch in the builder: how the command screen turns it into the forced study week belongs to the command-screen unit.
10. The `amusement_park_done` alternative branch `year 97 && day == 28` can never differ from the first alternative (both need bit 18 clear); dead code, no behaviour.
11. `debug_menu_active` (0x800B3C6C, initial value 1, cleared when a game starts) belongs to the OPTION debug page; a port without the debug page can drop the test in `day_event_next`.
12. `event_flags[82]` (`EVENT_FLAG_FINAL_YOSHIO`, HYPOTHESIS name): the scheduler reads it only as a zero test for the 1998-02-26 girl 13 scene 0, while GEKO stores a girl id in it and adds 1 to it and ENDING uses it as a girl id (section 10). What it records, and whether girl 13 scene 0 can repeat, belongs to the GEKO and ENDING analysts.
