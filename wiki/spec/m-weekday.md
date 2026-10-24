---
type: spec
unit: m-weekday
updated: 2026-10-24
sources: ["src/main/800737A0.c", "config/renames/proposals/m-weekday.tsv", "config/renames/proposals/core.tsv (p2a-core)"]
---

# Port spec: school-day command loop and vacation days (m-weekday)

Roadmap phase 2, unit `m-weekday` ([[tickets/T-27012-p2-m-weekday]], parent [[tickets/T-27000-phase-2]]).
The unit is `src/main/800737A0.c`, 19 functions. It plays one school day: the main-mode handler `week_day`
(mode 0x40, `MODE_SCHOOL_DAY` in the core table), the rule that turns the day's command into parameter changes
(`parameter_up_down`), the same scene for vacation days (`vacation_day_init`), and the last stage of the
weekly health check (`week_end_exit`, mode 0x32). Facts only, from the matched C and from runs of the game in
the emulator; guesses are marked HYPOTHESIS. Proposed names are in `config/renames/proposals/m-weekday.tsv`;
core names (`game_state`, `day_events`, `main_stage_next`, ...) are used as in `p2a-core`. Names of functions
owned by other units (`command_girl_select`, `command_roll_poor_result`, `icon_bar_init`, `season_index`,
`in_summer_months`, `dec_bg_*`, `holiday_activity_bg_sector`, `holiday_activity_pick_sd_set`, `holiday_sd_set_*`) are
the proposals of the sibling drafts of the same day (m-scheduler, m-hud, h-cdio, m-holiday) and must be reconciled at
review.

Naming: O.BIN developer names (`week_day`, `week_day_init`, `week_day_main`, `week_day_exit`, `week_day_main0`,
`week_day_exit0`, `get_weekly_bg_sector`, `parameter_up_down`, `vacation_day_init`) are kept with an English
comment row. New names extend their stems: `week_day_*` for the school day, `vacation_day_*` for the vacation
setup, `week_end_exit` for the week-end check. `week_day` means "school day" here, not the day of the week
(glossary trap). SD = super-deformed (chibi) sprite, the small animated figure of the player. An SD set id picks
one SD pack, a 41-sector disc file with the figures, frames and animation sequences of one scene.

## What the scene does

The player picks one weekday command for the whole week on the command screen (mode 0x34). When the week's queue
is built, every school day gets a record that carries that command. When the queue reader reaches such a record
it opens mode 0x40, and the screen does this, with no player input:

1. The HUD is built: parameter panel on top, calendar at the right, message window at the bottom, the command
   icon bar at the left with only the day's command enabled.
2. The background of the place that goes with the command is read from disc and shown (library, classroom,
   park, street, classroom with friends, wash room, an outdoor field for rest; a club room for the club command).
3. A small disc file (the SD pack: palette, sprite sheet, frame table, animation sequences) with the animation of
   the player doing the command is read and its animation starts (it plays once and stops on its last frame). On a
   poor day (see below) another sequence of the same pack plays (the player slumps).
4. When the animation has stopped: one sound effect (good or poor), the command is applied to the nine
   parameters, the panel refreshes (changed digits are coloured), a counter of club practice is increased for
   the club command.
5. 14 frames later the scene ends: the queue index moves on and the game returns to the day screen (mode 0x31),
   whose queue reader opens the next record (normally the end-of-day record, then the next day).

In pseudo-code (names of this spec; `holiday setup` is the vacation variant described below):

```
play_school_day(rec):                      # rec = day_events[day_event_index], type 0x00
    cmd, poor, companion = rec.girl, rec.scene, rec.extra
    setup = holiday_setup if vacation != 0 else school_setup
    sick_rest = health == SICK and cmd_chosen == REST and not remedial_week       # last_command, not rec
    bg   = room[season] if sick_rest else setup.bg(cmd, club)       # club only for cmd == CLUB; the vacation bg test skips remedial_week
    pack = SICK_BED if sick_rest else setup.sd_pack(cmd, club, summer, variant(companion, cmd))
    show HUD, bg; spawn SD actor A (sequence = poor); spawn SD actor B if companion is a girl, cmd != REST, plain school day
    wait until A's animation has stopped
    play sound (poor ? 0x502 : 0x501)
    apply_command(cmd, holiday = False, poor)                                      # parameter_up_down
    if cmd == CLUB: clubs[club].exp += 1
    refresh the parameter panel; hold 14 frames
    day_event_index += 1; return to the day screen
```

Vacation days (summer, winter and spring breaks, `vacation` != 0) use the same mode and the same record, but a
different setup (stage 10): home or camp backgrounds, holiday SD packs. The week-end health check (mode 0x32) is a
different mode; only its last stage, `week_end_exit`, lives in this file.

## The record and who builds it

`day_events[day_event_index]` is a `DayEvent` (type, girl, scene, extra; core names). For type 0x00 records
(school day or break day, built by `day_events_build_week` for day kinds 0 and 3) the fields mean:

| field (core name) | meaning for type 0x00 | set by |
|---|---|---|
| `type` | 0x00 | queue builder |
| `girl` (byte 1) | the command, `WeekdayCommand` 0-7 (the low nibble of `last_command`; the same for every school day of the week) | queue builder |
| `scene` (byte 2) | the poor flag: 0 normal, 1 poor day. Always 0 for rest | queue builder via `command_roll_poor_result` |
| `extra` (byte 3) | the companion: a girl id, 0 for girl 0, or 0xF for nobody; one lookup per week (`command_girl_select(command)`, same value in every school-day record of the week) | queue builder |

The poor flag and the companion are fixed when the week is built (at the end of the command screen), not when
the day is played. Core names the fields `girl`, `scene`, `extra`; for these records `command`, `poor` and
`companion` read better; see Open questions.

Poor flag, as the scheduler computes it (`command_roll_poor_result`, rule owned by the m-hud/m-scheduler units,
summarised because it decides the effect of a day). Three sine waves with periods of 23, 28 and 33 days (steps of 0xB2,
0x92 and 0x7C per day in a 4096-unit sine, phase from the player's birthday plus `day_count`; HYPOTHESIS: the classic
biorhythm) are the base. Each command uses one of them (athletics the 23-day wave; art, trivia and looks the 28-day
wave; liberal arts and science the 33-day wave; the club command by club; rest all three). The wave term is 0 on a
critical day (the wave within 10% of its zero crossing), else up to 16 in the positive phase and up to 8 in the
negative phase (averaged over the waves in use). The score is that term + the command's parameter / 16 (club: club
practice / 16; rest: a fixed 16 instead) + 6; the roll is `rand() % (score + 4)`; it is multiplied by (8 - f) / 8 with
f = stress / 32 (f = 4 from stress 97, so at most half); it is divided by 4 when all three waves are weak on that day
(a wave is weak when its term at scale 3 is 0: a critical day, a positive phase under a third of the amplitude or a
negative phase under two thirds). The day is poor when the result is below 4. So practice, higher parameters and
low stress make poor days rarer.

## Mode 0x40: stages and steps

`week_day` is called once per frame by `main_state_dispatch`. It switches on `stage` (`unk_1109`):

| stage | name | handler | notes |
|---|---|---|---|
| 0 | `WEEK_DAY_STAGE_INIT` | `week_day_init` | `step_timer` + 1 per call, then switch on `step`: 0 `week_day_init_screen`, 1 `week_day_init_sd_load`, 2 `week_day_init_sd_actors`; each ends with `main_step_next`, the last with `main_stage_next` |
| 1 | `WEEK_DAY_STAGE_MAIN` | `week_day_main` | `step_timer` + 1, step 0 runs `week_day_main0` |
| 2 | `WEEK_DAY_STAGE_EXIT` | `week_day_exit` | `step_timer` + 1, step 0 runs `week_day_exit0` |
| 10 | `WEEK_DAY_STAGE_VACATION` | `vacation_day_init` | same three-step shape; ends in stage 1 |
| other | | `game_reset_to_title` | unexpected stage: reset to the title (never happens in normal play) |

After the stage handler, every frame of the mode draws the day HUD in this order: `parameter_show`,
`message_window_show`, `hizuke_show`, `icon_bar_update(1)`, `dec_bg_draw(0)`, `screen_frame_draw(1)`, and
`mascot_update` when the mascot actor (actor 27) is visible (`D_8011F3FF & 0x80`, flag bit 7 of its
`draw_flags`). These are HUD calls owned by the m-hud unit; the port draws its HUD itself.

### Stage 0, step 0: `week_day_init_screen`

Runs every frame of the step; the part before the switch runs each time.

1. `daily_flags_clear` (clears `daily_event_bits` and `events_today`).
2. If `vacation` != 0: clear the sprite tables (`sprite_tables_clear`), `main_stage_set(10)` and return (the
   vacation setup takes over with step 0).
3. Otherwise a three-state switch on `substep` (`unk_110D`):
   - substep 0 (one frame): clear actors 96-159, background brightness 0x80, clear-screen on, texture-block
     cache cleared, sprite tables cleared; build the calendar actors (`hizuke_init`), the icon bar
     (`icon_bar_init`, 14 icons, 15 in the free layout), `icon_disp_switch(1)`, lock the icons to the record's
     command (`command_icons_lock_to_record`: only the icon with the command id is usable; in free layout the
     15th icon too when a command stands), parameter panel (`parameter_show_init`), the finger cursor actor
     (`cursor_actor_init(0)`), `parameter_disp_switch(1)`, message window; load the sick-overlay graphics
     (`func_8005AD1C`); if the player is in the remedial week (player_flags bit 16) sprite quad 0 is hidden, else
     `func_8005AD70` sets quad 0 up when `health` == sick (the sick overlay: the command screen loads the same graphics in
     its sick branch);
     reset the text system, hide the window title, start the typewriter at entry 0 (no text is queued, the
     window stays empty); then `week_day_bg_read`. Its result: 0 or 1 = the background is already resident in that
     buffer (show it, substep 2); -1 = a read was started (substep 1).
   - substep 1: wait until the CD read finishes (`cd_read_sync_status() & 1`), decode it into VRAM
     (`dec_bg_decode`, h-cdio name), show the decoded buffer; if the decode reports -1 (bad data) reset the
     background cache and start the read again (stay in substep 1); else substep 2.
   - substep 2: `main_step_next`.

### `week_day_bg_read` and `get_weekly_bg_sector`

- Sick-rest special: `health` == 3 (sick) and not the remedial week (bit 16 of `player_flags`) and the last chosen
  command (`last_command` low nibble) == rest (7): the background is `room_bg_sectors_weekday[season]`, the
  player's room (the same file the weekday command screen and the phone screen use). `season` = `season_index`
  (months 3-5 spring 0, 6-8 summer 1, 9-11 autumn 2, 12-2 winter 3). Emulator, player state poked to sick and command rest:
  the player sleeps in a bed in his room at night; with the remedial-week flag also set the ordinary outdoor rest scene
  plays, so the flag switches the special off.
- Otherwise `get_weekly_bg_sector()`: record command != 4: `week_day_bg_sectors[command]`; command 4 (club):
  `club_bg_sectors[club]` with `club` = `player_flags` bits 12-15.
- Both go to `dec_bg_cd_read(sector, 0)`: background layer 0, two cached buffers, the cache skips a read when the
  sector is already resident.

Files behind the sectors (directory `BS` of the disc): school-day command 0 to 3 use files `C_01_27` to `C_01_30`,
command 5 to 7 use `C_01_42` to `C_01_44`; club 0-10 use `C_01_36` to `C_01_41` (literature, drama, science,
computer, art, brass band) and `C_01_31`, `C_01_32`, `C_01_35`, `C_01_34`, `C_01_33` (baseball, soccer, tennis,
swimming, basketball); the room uses `C_02_02` plus the season letter H (spring), N (summer), A (autumn), F
(winter). Scenes seen in the emulator on 1995-04-05 to 04-10: command 0 library, 1 classroom,
2 park (sketching), 3 street (jogging), 5 classroom with several friends, 6 wash room, 7 outdoors (lying asleep).

### Stage 0, step 1: `week_day_init_sd_load` (with `week_day_pick_sd_set`)

Substep 0: `week_day_pick_sd_set` chooses the SD set id (the index of an SD pack file) and stores it in `scene_work[0]` (`D_801230D0`); then the
read is queued:

- sick-rest special (same test as above): read 9 sectors at sector 32138 (`SD_SICK_BED_PACK_LBA`, the sick-bed pack of
  the `ETC_SD` directory) to 0x801E0000;
- else command != club: read 41 sectors at `week_day_sd_set_sectors[id]` (directory `MAIN_SD`, 25 files, id = position
  in the directory, sector 42418 + 41 * id);
- else (club): 41 sectors at `club_sd_set_sectors[id]` (directory `CLUB_SD`, 32 files, 27834 + 41 * id).

Then substep 1. Substep 1: when the read has finished, check the file checksum (`file_checksum_ok`: 16-bit byte sum
equals the value stored at the end of the file); on success `main_step_next`, else substep 0 again (reselect, reread).
`week_day_pick_sd_set` details:

- `variant` = 0 when the record's `extra` is 0; 2 when the command is athletics and `extra` is 5; else 1 (this
  includes 0xF, nobody).
- Summer flag = `in_summer_months()` (months 6-9). Command != club, != rest: `id = table[season][2 * command + variant]`
  (commands 0-3), `table[9 + variant]` (command 5), `table[11 + variant]` (6), `table[13]` (rest). Tables
  `week_day_sd_set_summer` / `week_day_sd_set_other`:

| command | summer ids by variant | other months |
|---|---|---|
| 0 liberal arts | 0, 2 | 1, 3 |
| 1 science | 4, 6 | 5, 7 |
| 2 art | 8, 10 | 9, 11 |
| 3 athletics | 12, 13, 14 | 12, 13, 14 |
| 5 trivia | 21, 23 | 22, 24 |
| 6 looks | 17, 19 | 18, 20 |
| 7 rest | 15 | 16 |

  Ids come in (summer, other months) pairs for commands 0-2 (0/1 ... 10/11, even = summer) and for 15/16, 17/18, 19/20,
  21/22, 23/24; the second file of a pair has a `W` in its name. Ids 12-14 (athletics) have no `W` file and no seasonal
  variant. On 4/5 (April, a `W` id) the figure wears the school jacket and in the July vacation scene (a holiday summer
  id) the white shirt, so the `W` files are the jacket uniform of the cold months. Emulator: command 0 chose id 3, command 1 id 7, command 2 id 11,
  command 3 id 13, command 5 id 24, command 6 id 20, command 7 id 16, all in April with companion 0xF.
- Club command: `id = club_sd_set_summer[club][variant]` or `club_sd_set_other[club][variant]` with `variant` 0 when
  `extra` is 0, else 1 (club 0 literature 0/2 summer, 1/3 other; club 1 drama 30/31; club 2 science 4/6, 5/7; club 3
  computer 8/10, 9/11; club 4 art 26/28, 27/29; club 5 brass band 22/24, 23/25; baseball 12/13, soccer 14/15,
  swimming 18/19, tennis 20/21, basketball 16/17, the same in both seasons).

### Stage 0, step 2: `week_day_init_sd_actors`

1. Clear actors 96 and 97.
2. Pick the pack tables: sick-rest special: `addr_init_else_sd(2)`, poor flag read as 0 for the actor; club command:
   `addr_init_club_sd(id)`; else `addr_init_weekly_sd(id)`, and for the two rest ids (15 and 16) the record's poor flag is
   cleared (so rest never plays the poor sequence). These helpers (module 8007C030) set four pointers
   from a table of three 16-bit offsets per id and the pack base 0x801E0000: `sd_palette` (the base),
   `sd_texture_blocks`, `sd_image_table` (frame table), `sd_sequence_table`. The pack file is
   palette, texture blocks, frame table, sequences; offsets per id are data of `addr_init_*_sd`. Pointers inside a pack
   are absolute RAM addresses for the load address 0x801E0000. Example, school-day pack 0 (84 KB): palette at +0
   (0x200 bytes), image records from +0x200, four sequences at +0x3FC to +0x473, the texture-block pointer table at +0x474
   (its first entries point at +0x4E0 and +0x44E0; blocks are 0x4000 bytes, 128 x 128 pixels at 8 bits; the rest of the
   file up to 0x14800 is more texture data), the image pointer table at +0x47C, the sequence pointer table at +0x4B8 with four entries: sequence 0 and 1 for figure A (normal, poor), 2 and 3 for figure B.
3. `load_palette(sd_palette, row 0x10, ...)`: queue the palette upload to CLUT row 0x10.
4. SD actor A = actor 96: draw priority 1, `anim_flags` 5 (running, stop on the last frame), sprite attribute
   0x01000000 (8-bit pixels), `draw_flags` 0x84 (visible, position not shifted by the image origin), frame table,
   sequence table and texture blocks from the pack, palette mode 2, palette row 0x10, sequence index = poor flag (0
   or 1), position (-80, -64) (origin at screen centre), animation speed byte set to 0x82 (two ticks per frame)
   when `sd_anim_fast` (`unk_F6E`, toggled by pad 2 START) is nonzero.
5. SD actor B = actor 97, only when all of these hold: the record command is not rest, `day_kind[today]` of the
   current month record is not 1, 2 or 3 (a plain school day: not Sunday, holiday or break day), and `extra` is not
   0xF. It is built like A with sequence index = poor flag + 2 and position (-24, -64). Actor B is the companion girl's SD
   figure, and the girl drawn is baked into the pack: variant 0 packs show girl 0, variant 1 packs the heroine that
   `command_girl_select` can return for that command (emulator, records poked in a loaded state: command 0 with `extra` 0
   shows girl 0, with `extra` 1, 2 or 5 the same girl 1; command 1 girl 2; command 2 girl 3; command 3 variant 1 girl 6,
   variant 2 girl 5; command 5 girl 8 beside the friend; command 6 girl 7). So `extra` acts only as 0 / nonzero (plus the
   value 5 for athletics) and 0xF = nobody; the port can pick the figure by (command, variant).
6. `main_stage_next` (stage 1).

### Stage 1: `week_day_main0` (applies the command)

`week_day_main` adds 1 to `step_timer` each frame and at step 0 calls `week_day_main0`:

- If actor 96 `anim_flags` bit 0 (running) is clear, i.e. the animation has stopped:
  - On the first such frame (actor 96 `anim_timer` == 0): play sound command 0x501 when actor 96's sequence index
    is even, 0x502 when odd (sequence index = poor flag; 0x502 is the id the card code uses for errors); then
    `parameter_up_down(command, 0, poor)` with the record's `girl` and `scene` bytes; then `parameter_show_init()`
    (re-reads the nine parameters for the panel; changed digits are drawn in another colour); and for the club
    command only: `clubs[club % 11].exp += 1` (u16 club practice points).
  - Every such frame: if the timer was 13 or more before the increment, `main_stage_next`; the timer (the actor's
    own `anim_timer` field, idle after the animation stopped) is incremented each frame, so the result stays on screen 14 frames.
- If `step_timer` >= 601 (0x259; counted since stage 1 began), reset the CD command ring and `main_stage_next` even
  if the animation never stops (a safety valve).

Emulator timeline of the first school day (1995-04-05, command 0, healthy; frame 1 is the second frame of the mode):
step 0 ends at frame 21 (about 22 frames in all, the first background read), step 1 frames 22-53, step 2 frame 54,
stage 1 from frame 55, `parameter_up_down` at frame 87 (the 32 frames are the animation), `week_day_exit0` at
frame 101 (14 later); consecutive school days are 83 frames apart (day screen and day-end record included). Vacation
day on 1995-07-24: step 2 at frame 33, `parameter_up_down` at frame 66, exit at 80, days 83 frames apart. Most of
steps 0 and 1 is CD read time and need not be reproduced; the animation length comes from the pack's sequence table.

### Stage 2: `week_day_exit0`

`day_event_index += 1`; clear actors 96 and 97; `main_state_set(0x31)` (day screen). Unlike `day_event_next` it does not
test the debug switch `D_800B3C6C` (which would go to mode 0x22).

## `parameter_up_down(command, holiday, poor)`

Arguments in the C: `arg0` the command id (0-7; the records of holiday days carry ids that fit the same table), `arg1`
nonzero for a holiday command (called with 1 by the holiday module), `arg2` nonzero for a poor day. Stored values:
nine parameters (core `param_*`, order condition, liberal arts, science, art, athletics, trivia, looks, guts, stress)
are 16.16 numbers; the table values are in sixteenths of a point (`PARAM_DELTA_UNIT` 0x1000 = 1/16).

```
health  = player_flags.health                       # 0 ok, 1 injured, 2 neurotic, 3 sick
div_a   = 2 if health == SICK else 1
div_b   = 2 if health in (INJURED, NEUROTIC) and command not in (TRIVIA, LOOKS, REST) else 1
k       = (4 if holiday else 1) * 0x1000 / (2 if poor else 1) / div_a / div_b      # integer divisions, in this order
row     = club_activity_deltas[club] if command == CLUB else command_deltas[command]  # s8[9]
for i in 0..7:   change(i, row[i] * k       if row[i] > 0 else row[i] * 0x1000)      # gains scaled, costs not
d8 = row[8] * k if row[8] < 0 else row[8] * 0x1000      # stress: relief (negative) scaled, a rise is not
change(8, d8 + (0x10000 if poor else 0))                 # one call: a poor day adds one more full point of stress
change(i, d):  v = param[i] + d; if v < 0: v = 0; if int(v) >= 1000: int(v) = 999 (fraction kept)
```

Facts the port needs:

- Only beneficial changes are scaled: gains of parameters 0-7 and the relief (negative) of stress. Losses,
  condition costs and stress rises are never divided or multiplied, and a holiday command multiplies gains by 4 but not
  costs. Poor, sick and injured/neurotic each halve the gains and they stack (a poor day while sick gives a quarter).
  Commands 5 (trivia), 6 (looks) and 7 (rest) are exempt from the injured/neurotic halving only; the sick halving
  applies to every command.
- `command_deltas` is 8 rows of 9 signed bytes (row 4 is all zero because the club command uses its own table);
  `club_activity_deltas` is 11 rows (club ids 0-10; the club 11, no club, row does not exist, and the command screen
  disables the club icon for a player without club). The headline of each command row: 0 liberal arts +15, condition
  -12, stress +12; 1 science +15; 2 art +15; 3 athletics +52, guts +22, condition -38, looks -7; 5 trivia +24, looks
  +12, condition -19, stress -14; 6 looks +33, trivia +8, condition -10; 7 condition +49, stress -47, looks -12,
  trivia -10; clubs: literature/drama/science/computer/art/brass band raise their field by 12-15 and stress by
  14-26, sports clubs raise athletics by 13-15 and guts by 9-12. The full tables are read from the executable
  (data, not repeated here).
- Worked example, checked in the emulator on 1995-04-05 (command 0, healthy, normal): condition 100 -> 99.25
  (-12/16), liberal arts 40 -> 40.9375 (+15/16), science and art +1/16, athletics -6/16, trivia +4/16, looks
  -4/16, guts -1/16, stress +12/16. On 1995-04-07 the poor day (flag 1) the gains are halved (liberal arts +15/32)
  and stress rises 12/16 + 1 point. A club day (player state poked to club 0, command 4, healthy, normal) gave
  condition -12/16, liberal arts +12/16, science +4/16, art +2/16, trivia +1/16, looks -4/16, guts -1/16, stress
  +18/16, and `clubs[0].exp` went from 0 to 1: exactly row 0 of `club_activity_deltas`.
- The holiday module calls it with `holiday` = 1 about 32 frames into the holiday command scene (and plays the same
  two sounds); the KANGEI overlay calls it with 0 for club-camp days after adding its own bonus (KANGEI unit).
- The stress change is one `change` call with the extra point already added, so the floor at 0 is applied once to the
  sum. `parameter_change` (HUD module, O.BIN name) is the only writer of the values here.

## Stage 10: `vacation_day_init` (vacation days and club camp days)

Entered when school-day step 0 sees `vacation` != 0 (summer, winter or spring break; the queue builder emits normal
type 0x00 records for break-day kind 3 days), and called directly by the KANGEI overlay (club camp, queue type 0x42) at
six of its own stages (3, 7, 13, 18, 23 and 27 of its day dispatcher). The three steps mirror the school-day setup:

- Step 0 `vacation_day_init_screen`: HUD setup as school-day step 0 minus the daily flag clear, the cursor actor, the sick
  overlay (quad 0 is always hidden); background by `vacation_day_bg_read`; no decode retry. Background rule: when the
  first record of the queue has type 0x25 or 0x42 (tournament or camp week) or the player is not (sick and resting) it is
  the holiday background table (`holiday_activity_bg_sector`, `func_80075FA0`; per command: 0-2 the home-room file `C_02_03`, 3 `C_29_02`, 5 `C_29_10`, 6 and 7 the
  school files `C_01_43` and `C_01_44`; the club command `club_bg_sectors[club]`); when the player is sick and the last
  command is rest (and it is no camp or tournament week) `room_bg_sectors_holiday[season]` (emulator, sick and resting on 1995-07-24: the player sleeps in his room by day). Unlike the school-day
  version this test does not look at the remedial-week flag, while steps 1 and 2 of the vacation setup do. Emulator:
  command 0 on 1995-07-24 shows the player at a desk in a room with bookshelves, white shirt.
- Step 1 `vacation_day_init_sd_load`: the SD set id comes from the holiday module (`holiday_activity_pick_sd_set`,
  `func_80076FDC`: summer table `holiday_sd_set_summer` = ids 0, 2, 4, 12, -, 10, 8, 6 per command, other months
  `holiday_sd_set_other` = 1, 3, 5, 12, -, 11, 9, 7; the club
  command goes through `club_sd_set_*`, variant 1 unless `command_girl_select(4)` returns 0; this lookup is made live,
  the record's `extra` is not used here); read 41 sectors at `holiday_sd_set_sectors[id]`
  (directory `HOLI_SD`, 13 files from sector 41446; id 12 is a `MAIN_SD` file) or `club_sd_set_sectors[id]`, or the 9-sector
  sick-bed pack. It only waits for the read (no checksum).
- Step 2 `vacation_day_init_sd_actors`: as school-day step 2 with `addr_init_holiday_sd` (13 ids), the ids 6 and 7
  (rest, summer and winter) clear the poor flag, actor B only for the club command when `command_girl_select(4)` is not
  0xF. Then `main_stage_set(1)` for a school-day record (play and exit as above), or `main_stage_next` when the first
  record of the queue is a camp record (KANGEI continues its own stage machine).

## Interfaces and shared state

State the unit reads or writes (core names; offsets in `game_state`):

| field | offset, type | use here |
|---|---|---|
| `vacation` | 0x03D u8 | 0 term, 1 summer, 2 winter, 3 spring; nonzero sends the school day to stage 10 |
| `month`, `day` | 0x03F, 0x040 u8 | summer flag (months 6-9), season index, day-kind lookup |
| `month_calendar[0].day_kind[day]` | 0x044 + day, u8 | not 1, 2 or 3 (plain school day) for SD actor B |
| `player_flags` | 0x0F4 u32 (`PlayerFlagBits`) | health = bits 8-11, club = bits 12-15, bit 16 remedial week (set by the exam result scene; while it is set the command screen enables only commands 0-2, so the week's school days carry a study command) |
| `param_*` | 0x0FC-0x11C, 9 x `ParamValue` (u16 frac, u16 value) | changed only through `parameter_change` |
| `clubs[club].exp` | 0x66C + 4 * club, u16 | +1 per club-command day |
| `day_events[]`, `day_event_index` | 0x69C 32 x 4 bytes, 0x71C u8 | the record of the day; the index moves on at the exit |
| `events_today`, `daily_event_bits` | 0x71D, 0x71E u8 | cleared by `daily_flags_clear` in step 0 |
| `last_command` | 0xF68 u32 (low nibble) | sick-rest test (rest = 7) |
| `sd_anim_fast` | 0xF6E s8 | animation speed byte 0x82 instead of 1 when nonzero |
| `step_timer`, `mode`, `stage`, `step`, `substep` | 0x1104 u32, 0x1108-0x110A, 0x110D u8 | the four-level state machine |

Types: `DayEvent` (4 bytes), `ParamValue` (4), `ClubRecord` (4), `MonthCalendar` (0x40), `Actor` (0x44, actors 96 and 97),
`SpriteQuad` (0x24, quad 0 for the sick overlay).

Reads: `day_events[day_event_index]`, `last_command`, `player_flags` (health, club, bit 16 remedial week),
`vacation`, `month`, `day`, `month_calendar[0].day_kind`, `sd_anim_fast`, the nine parameters, `clubs`. Writes:
the nine parameters (through `parameter_change`), `clubs[club].exp`, `day_event_index`, `daily_event_bits` and
`events_today` (cleared), `scene_work[0]` (the SD id), the SD pointer globals, actors 96 and 97, mode.
Callers: `main_state_dispatch` (mode 0x40 -> `week_day`), the day-screen queue reader (type 0x00 records ->
`main_state_set(0x40)` after enabling only the command's icon), KANGEI (`vacation_day_init`, `parameter_up_down`),
the holiday module (`parameter_up_down`), `func_80072BC0` (mode 0x32 default stage -> `week_end_exit`).

`week_end_exit` (mode 0x32, stage 0xFF): `day_event_next(0)` (queue index + 1, `main_state_set(0x31)`, or mode 0x22 when the
debug switch is set), then the day HUD calls (`parameter_show`, `message_window_show`, `hizuke_show`, `icon_bar_update(0)`,
`mascot_update`, `screen_frame_draw(1)`, `dec_bg_draw(get_last_gamen_mode())`). The weekly health check
(injury, neurosis, illness, recovery messages; core spec section 4.2) sets stage 0xFF when it is done. Emulator: runs
once on each Saturday night with mode 0x32, stage 0xFF.

## Port notes

- The whole mode is a fixed pipeline per day: load assets for (command, season, club, health), play the animation once,
  apply the effects, hold, advance the queue. The port can load assets instantly; keep the animation length (from the
  sequence table of the SD pack) and the 14-frame hold if the feel of the pacing matters.
- Effects apply once, after the animation, not at the start. Sound first, parameter change, panel refresh, club
  practice point, in that order.
- The poor flag and the companion are decided at queue build time from the state of the week's start (and for the poor
  flag the day offset); the port should compute them there, not when the day plays.
- The sick-rest special (health sick, not in the remedial week, command rest) replaces the background by the player's
  room of the current season and the SD pack by the sick-bed pack. It is the normal case for a sick player: on the
  command screen (mode 0x34, outside this unit) a sick player can only enable the rest icon, and the remedial week
  (three study icons only) takes precedence over sickness, which is why the special excludes the remedial week. Other
  commands of a sick player (possible in the remedial week) play normally with halved gains.
- Backgrounds and SD packs are assets selected by tables of sectors (a CD address): the port should key them by file,
  the files are listed above. `W` files are the jacket uniform; the summer switch is months 6-9, not the season index.
- The hardware layer is replaced: the dec_bg read/decode/show, actors 96/97, palette row 0x10 and sprite attribute bits
  are the PS1 way of showing a background and an animated sprite.

## Open questions

1. The club command's SD variants (which girl is baked into each club pack) and command 3 variant 0 were not looked at;
   club 0 was played once (club room scene, deltas and practice point confirmed) and the variant rule is from the C.
2. The vacation-day companion (live lookup for the club command only) and the vacation club scene were not played.
3. Core's `DayEvent` field names (`girl`, `scene`, `extra`) fit event records; for type 0x00 and 0x10 they hold command,
   poor flag and companion. Suggest a union view `SchoolDayEvent { type, command, poor, companion }` or comments; no
   rows are proposed here because the field rows belong to core.
4. Actor 97 is `ACTOR_CALENDAR_MARK_FIRST` in the m-hud draft and SD character B here; suggest alias constants `ACTOR_SD_A` 96,
   `ACTOR_SD_B` 97 (both units use them).
5. Sound ids 0x501 / 0x502 (good / poor here; 0x502 is also the card error sound) need names from the sound unit.
6. The sick overlay (`func_8005AD1C` / `func_8005AD70`, sprite quad 0 when sick): what it looks like was not identified in the
   scenes seen (it sits where the sleeping figure is drawn); it needs a name from the graphics side.
7. `room_bg_sectors_*`: the weekday table shows the player's room at night (command screen, sick-rest school day), the
   holiday table the same room by day (sick-rest vacation day, emulator, 1995-07-24); other holiday uses are the holiday
   unit's.
8. `D_800B6928` has 11 club rows; a command 4 with club id 11 would read padding bytes (cannot happen: the icon is disabled).
