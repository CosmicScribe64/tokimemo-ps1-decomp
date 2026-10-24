---
type: spec
unit: m-hud
updated: 2026-10-24
sources: ["src/main/80062CD0.c", "src/main/800674B0.c", "config/renames/proposals/m-hud.tsv", "config/renames/proposals/core.tsv", "config/renames/proposals/h-graphics.tsv", "wiki/spec/core.md"]
---

# Port spec: HUD, calendar page, parameter panel, magazine, save panel (m-hud)

Roadmap phase 2, unit `m-hud` ([[tickets/T-27016-p2-m-hud]], parent [[tickets/T-27000-phase-2]]). Files: `src/main/80062CD0.c` (proposed name `hud_panels.c`) and `src/main/800674B0.c` (proposed `hud_screens.c`), 81 functions. The rename table is `config/renames/proposals/m-hud.tsv` (columns kind, scope, old, new, note; no header row; 200 rows: 45 func, 15 comment, 57 data, 19 field, 1 type, 60 const, 3 file; 9 rows carry a HYPOTHESIS mark). Names of other units are used as proposed there: the shared core names ([[spec/core]]) for game state, enums and the most used functions, and the hardware tables of [[spec/h-graphics]] and [[spec/h-cdio]] where core and the hardware units disagree (`actor_reset`, `prim_box_gouraud`, `prim_line_gradient`, `sprite_table`, `sound_request`, `sound_bank_load_start`, `bgm_play`, `decimal_digit`, `cd_state_get`). Actor fields are cited by the h-graphics names (`draw_priority`, `anim_flags`, `draw_flags`, `clut_row`, `frame_index`, `x`, `y`, `scale_x`, ...) and by offset where that helps.

Scope (port first): the game rules, state and flow are described in full; drawing, VRAM and sound-bank loading are named at module level. Facts only, in this project's words; guesses are marked HYPOTHESIS. No game text is quoted: labels and titles are paraphrased.

Checks with the emulator ([[emulator]]), from a saved state on the weekday run screen of 1995-04-05 (mode 0x40, the screen that plays out the chosen command): a screenshot gave the layout in section 2; `trace` over 145 frames showed which functions run every frame (section 2); `break` on `command_roll_poor_result` and `biorhythm_value` gave the call chain and arguments of section 12; a write watch on `D_8011ECA4` gave section 18; memory dumps gave the start values of `place_flags`, `icon_positions`, `icon_enabled` and the mascot variables; a second state on the command screen (1995-04-09) was driven with the pad to the calendar check page (section 13). Save states and screenshots are not committed.

## 1. What the unit is

The unit is the heads-up display and a group of small screens that the day, weekday, holiday and telephone modes share. It contains no mode handler of its own. A mode handler calls an `*_init` function once to create actors (sprites) and a `*_show` or `*_update` function every frame. Everything is drawn through the actor pool and the sprite table of the graphics layer ([[spec/h-graphics]]); coordinates are screen coordinates with the origin in the middle of the 320 x 240 screen (x -160..159, y -120..119).

| group | functions (new names; old names in the table) | section |
|---|---|---|
| portrait face | `bustup_read_start`, `bustup_build` (core), `bustup_face_update`, `bustup_expression_select`, `bustup_speech`, `bustup_wink`, `bustup_hide`, `bustup_idle_counter_wrap` | 4 |
| parameter panel | `parameter_show_init`, `parameter_show`, `parameter_disp_switch`, `parameter_change` | 5 |
| date panel | `hizuke_init`, `hizuke_show`, `hizuke_disp_switch` | 6 |
| message window, title banner | `message_window_init/show`, `message_disp_switch`, `message_window_set_title`, `message_window_set_title_hidden`, `message_window_title_show/update` | 7 |
| command icon bar | `icon_bar_init`, `icon_bar_update` (core), `icon_layout_apply`, `icon_move_allowed`, `icon_can_use_set`, `get_icon_can_use`, `icon_disp_switch`, `icon_card_anim_set` | 8 |
| season and holiday helpers | `season_index`, `in_summer_months`, `summer_phase`, `vacation_for_date` | 9 |
| day music | `day_bgm_load`, `season_bgm_load`, `bgm_load_finish_step` | 10 |
| background pattern | `screen_frame_draw` (core) | 11 |
| calendar page | `calendar_screen_init`, `calendar_marks_build`, `calendar_cell_x/y`, `month_calendar_load`, `calendar_view_update`, `biorhythm_graph_draw`, `cal_base_show`, `cal_sprite_init`, `cal_sprite_disp_switch` | 13 |
| biorhythm and command roll | `biorhythm_value`, `biorhythm_birth_offset`, `command_roll_poor_result`, `biorhythm_probe_unused` | 12 |
| information magazine | `magazine_page_build`, `magazine_mark_read`, `magazine_base_show` | 14 |
| appointments | `appointment_find_today`, `appointment_count_for_girl`, `appointment_list_sort`, `appointment_add`, `appointment_prompt_noop` | 15 |
| club list | `club_list_menu` | 16 |
| save slot panel | `save_slot_info_show`, `data_save_load_class_init` | 17 |
| small services | cursor (core), text marker, mascot, SD sprite helpers, corner icon, event log scan, button labels, light effect (core), `ui_vram_init` | 18 |

## 2. The day screen as built by this unit

Layout seen on the weekday run screen (screenshot of the emulator state, all positions in screen coordinates):

- Top: the parameter panel, three rows of three parameters (section 5), rectangle 264 x 42 at (-132, -97).
- Right edge: the date panel, rectangle 20 x 114 at (134, -75) with year, month, day and weekday (section 6).
- Bottom: the message window, rectangle 266 x 52 at (-133, 46) (section 7).
- Left: the command icons in a block from x -138 to -66, y -41 to 31 (section 8).
- Behind everything: a scrolling check pattern drawn by `screen_frame_draw` (section 11); in the middle the scene background (`dec_bg_draw`, other unit).
- The pointer (actor 0, core `cursor_*`) is drawn by the cursor functions.

Per-frame call lists:

- day screen (mode 0x31, `mode_day_screen`): the queue reader or ending start, `parameter_show`, `message_window_show`, `hizuke_show`, `icon_bar_update(0)`, `mascot_update`, `dec_bg_draw`, `screen_frame_draw(1)`.
- weekday run screen (mode 0x40, `week_day`): the stage handler, `parameter_show`, `message_window_show`, `hizuke_show`, `icon_bar_update(1)`, `dec_bg_draw(0)`, `screen_frame_draw(1)` and, when actor 27 is visible, `mascot_update`.
- command screen (mode 0x34, `mode_command_screen`): the stage handler (selection stage: `parameter_show`, `hizuke_show`, `message_window_show`, `icon_bar_update(0)`, cursor update, `message_window_title_update`, `dec_bg_draw`), `mascot_update` when visible, `screen_frame_draw(0)`; stage 2 is the calendar check and calls `calendar_view_update`.

Emulator trace from the weekday run state, 145 frames: `parameter_show`, `hizuke_show`, `message_window_show`, `icon_bar_update`, `season_index` and `screen_frame_draw` ran every frame; `parameter_show_init`, `hizuke_init`, `message_window_init`, `icon_bar_init`, `icon_layout_apply` and `mascot_set` ran once or twice at entry; `mascot_update` ran once; `bustup_face_update` did not run.

Typical init sequence (boot HUD init, holiday handler, weekday init, command screen): `hizuke_init`, `parameter_show_init` twice, `icon_bar_init`, `message_window_init`. The double call has this effect: the second call finds the snapshot equal to the live values and so draws every digit plainly (no highlight, section 5). A single call right after a parameter change (after `parameter_up_down`) shows the changes.

## 3. Actor slots and sprite quads used

Slots of the 160-actor pool (`actor_pool`) that this unit owns. Proposed constants `ActorSlot`.

| slot | use |
|---|---|
| 0 | pointer (cursor) |
| 1 | message window frame |
| 2 | continue marker (frame 1, home (116, 72)) |
| 3 .. 3 + title_banner_length + 1 | title banner plate pieces |
| 15 | corner icon (frame 0xAD at (-134, -92)) |
| 16 .. 23 | date panel: 16 label, 17-18 year digits, 19-20 month, 21-22 day, 23 weekday |
| 27, 28 | mascot (27) and its anchor hand (28) |
| 30, 31 | invisible animation drivers of the portrait eyes (30) and mouth (31) |
| 32 .. 59 | parameter panel: 27 digits (three per parameter) and the label actor 59 |
| 64 .. 78 | icon base plates (icon i uses 64 + i) |
| 80 .. 94 | icon glyphs (icon i uses 80 + i); actor 93 is icon 13 |
| 96, 97 | scene actors: a screen header sprite (calendar, save/load, club list), or SD sprites in dialogue (HYPOTHESIS) |
| 0x61 .. 0x80 (0x9F) | calendar marks (up to 0x3F) and the magazine page layers |

The separate globals `D_8011ED15..D_8011ED90` (actor 1), `D_8011ED59..` (actor 2), `D_8011F0CD..` (actor 15), `D_8011F3FF`, `D_8011F414`, `D_8011F41C`, `D_8011F443` (actors 27, 28), `D_8011F4CA/4CB` (actor 30), `D_8011F50E/50F` (actor 31), `D_8012059A/059C` (actor 93), `D_80120651..D_80120696` (actors 96, 97) and `D_8011ECD3`, `D_8011ECE8` (actor 0) are fields of these actors, reached through a flat symbol; a port addresses them as `actor_pool[slot].field`.

Sprite quads (`sprite_table` entries): 0-5 are the portrait quads (0 mouth, 1 eyes, 2-5 hair and body pieces); 0-3 are reused by the calendar page as month images and month plates.

## 4. Portrait face (bustup)

The portrait of the girl in a scene is built by core's `bustup_build`: five image blocks in texture slots 5-9, a frame and sequence table, two animation drivers (actors 30 and 31) and six sprite quads. This unit animates the face.

- `bustup_read_start(lba)` (core) reads 0x2D sectors to 0x80180000 and clears `D_8011F50F`, `D_8011F4CB` (the drivers' draw flags) and `mouth_frames_reversed`.
- `bustup_expression_select(rank)` (called from `bustup_build`; the callers pass `get_g_zyotai_h(current_girl) & 0x7F`, 0 best to 4 worst, forced to 0 for girl ids 12 and up): the eye and mouth quads (0, 1) take their picture from texture page 7 for rank 0-1, 8 for 2-3, 9 for 4; an odd rank shifts the V origin by 0x40 (the second half of the block). So a girl shows one of five expression sets that follows her relationship rank.
- `bustup_face_update()`, every frame while a portrait is shown: sets driver 31 to brightness 0x80 with its hidden bit cleared and unhides quads 0 and 1, then
  - voice on (`voice_mode != 0`): the CD audio level (`cd_state_get() & 4` means audio is playing; level = `getCDlevel() + 1`, else 0) is stored in the 4-entry ring `cd_level_history[frame_count & 3]`; the mouth frame n (0-3) is the number of thresholds of `bustup_lipsync_thresholds[current_girl]` that are below the level (the largest satisfied threshold wins, max 3); quad 0 V = 16 * n. The mouth frame is recomputed every frame when it is 0 and only on even frames when open. Holding square clears `lipsync_peak_level`; the peak is not read.
  - voice off: quad 0 V = 16 * frame index of driver 31 (the scripted talk animation).
  - quad 1 (eyes) V = 32 * frame index of driver 30 (blink animation).
- `bustup_speech()`: if voice is off and driver 31's animation bit 0 is clear, sets bits 0 and 4 (start the talk loop). `bustup_wink()`: the same for driver 30 (blink loop).
- Lip-sync thresholds (u8 [16][3] per `GIRL_*` id, levels above which the mouth shows frame 1, 2, 3): shiori 16/40/56, mio 24/52/72, yuina 16/40/56, ayako 24/56/80, saki 16/40/64, yukari 14/36/56, nozomi 12/28/36, mira 20/40/64, yuko 16/36/56, megumi 16/32/44, yumi 14/36/64, miharu 16/32/48, rei 16/32/44, yoshio 12/28/40, sotoi 16/32/48, id 15 16/32/48.
- `sd_mouth_update()` runs the same algorithm for actor 96's frame (HYPOTHESIS: the SD sprite in dialogue), and `sd_actor_96_anim_start` / `sd_actor_97_anim_start` start the animation of actors 96 and 97 (bits 0 and 1 of `anim_flags`, when bit 0 is clear).
- `mouth_frames_reversed` (`D_800B5A60`, HYPOTHESIS name): ENDING's lip sync uses `3 - level` while it is set; DATE sets it with `event_face0`; the dialogue code swaps it with `D_800CA360` around event stills.
- Dead code: `bustup_hide(arg)` (hides drivers and quads 0-5 when `arg == 0`, never called), `bustup_idle_counter_wrap` (zeroes `D_800B5A64` when it reaches 0xA5; nothing else touches it).

## 5. Parameter panel

State: the nine `param_*` records of `GameState` (16.16 values, value 0..999, core 4.1) and the 0x7C-byte `profile_snapshot` inside `progress_state` (new struct `ProgressState`: `place_flags[21]` at +0x00, `command_counts[9]` at +0x15, two unused bytes, `profile_snapshot[0x7C]` at +0x20; header change for the apply step). The snapshot holds a copy of `GameState` +0xC4..+0x13F (names, flags, birthday and the nine parameters).

- `parameter_change(i, delta)`: `param[i].word += delta` (16.16), a negative word becomes 0, a value of 1000 or more becomes 999. `parameter_up_down` (weekday unit) calls it.
- `parameter_show_init()`: loads five palette rows to VRAM (0x120, 0x1E0..0x1E4), creates actors 32..59, then for each parameter k (0..8) writes three digit actors: frame `0x44 + digit` (digit from `decimal_digit(value, 3/2/1)`: hundreds, tens, units), spacing 8 px. Grid: column k % 3 at x = -73, 15, 103 (x of the first digit), row k / 3 at y = -88, -74, -60. Actor 59 (frame 0x3D) is the label at (0, -72).
  - If the live value is larger than the snapshot value, the three digits get `clut_row` `HUD_CLUT_UP_SATURDAY` (0x43) and flip phase 1; if smaller, `HUD_CLUT_DOWN_SUNDAY` (0x44) and phase 1; equal values stay plain.
  - At the end the live profile block is copied into `profile_snapshot` (so the next call compares against the values shown now).
- `parameter_show()`, every frame: while actor 32 is visible it draws the panel background (`prim_box_gouraud(-132, -97, 264, 42, slot 12, colour 0x30E00)`) and runs the flip animation of the digit actors: phase 1 shrinks `scale_x` by `0x1000 / step` per frame (and raises `anim_timer`) until it reaches that step, then phase 2 grows it back and the phase returns to 0. `step` is 8, or 4 when `sd_anim_fast` is nonzero. The stress digits (actors 56-58) use `scale_y` instead.
- `parameter_disp_switch(1/0)` shows or hides actors 32-59.

The digits show the new value at once; the flip and the colour only mark the change.

## 6. Date panel

`hizuke_init()` creates actors 16..23 and sets: year digits (frame `0x44 + digit`, year % 100) at x = 140 and 148, y = -65; month at y = -34; day at y = -1; weekday sprite frame `0x52 + weekday` at (146, 31); label actor 16 (frame 0x3C) at (144, -18). A month or day below 10 hides the tens digit actor (`draw_flags = 0`, parked at (255, 255)) and puts the units digit at x = 144. The weekday sprite colour: `clut_row` 0x43 on Saturday, 0x44 on Sunday, and 0x44 on any day whose `day_kind` (this month's record, day index = today) is 1 or 2 (Sunday or national holiday). `hizuke_show()` draws the background rectangle (134, -75, 20 x 114, slot 5, colour 0x30E00) while actor 16 is visible. `hizuke_disp_switch(1/0)` shows or hides the eight actors.

## 7. Message window and title banner

Message window: `message_window_init()` creates actor 1 (frame 0x3E at (0, 75), priority 7), `message_window_show()` draws the rectangle (-133, 46, 266 x 52, slot 8, colour 0x30E00) while it is visible, `message_disp_switch` toggles it. The text itself is the text list (`set_kanji_string`, other unit).

Title banner (top left, a short screen name): `message_window_set_title(text)` (core) queues the text at (-206, -152), sets `title_banner_length = strlen / 2` and builds `length + 2` plate actors from slot 3: the right cap (frame 0x73) in slot 3, `length` tiles (frame 0x72), the left cap (frame 0x71) in the last slot; piece i sits at x = (length - i) * 14 - 135 plus the slide offset and y = -81 plus the slide offset. `message_window_title_update()` runs the timeline kept in `title_banner_timer`:

| timer | what |
|---|---|
| -0x40 .. -1 | slides in from the top left; the text entry follows; the timer rises by 1 per frame |
| 0 .. 0x77 | shown at home (plate at (-144, -90), 20 px high, width `length * 14 + 8`) |
| 0x78 .. 0xB7 | slides out |
| 0xB8 | retired (off screen); while triangle is held the banner is shown at home |

`message_window_set_title_hidden(text)` is `set_title` followed by `title_banner_timer = 0xB8` (the banner starts retired; the original callers pass the string through the register although the C prototype is `(void)`). `message_window_title_show(1/0)` shows or hides the plate actors and the text entry (it also resets the timer to retired when hiding).

## 8. Command icon bar

Fourteen icons (`ICON_COUNT_FIXED`) in the two fixed layouts, fifteen (`ICON_COUNT_FREE`) in the free layout; `icon_mode` (`ICON_MODE_TYPE_A/B/FREE`) and `icon_count` are saved settings. Each icon is two actors: a base plate (slot 64 + i, frame 2) and a glyph (slot 80 + i, sequence i). Icons 0-7 are the weekday commands (`COMMAND_LIBERAL_ARTS` .. `COMMAND_REST`, the scheduler unit's `WeekdayCommand`); the other icons are handled by the command screen (mode 0x34) and the holiday handler (mode 0x46), which own their names. Command screen: icons 8-10 also store their number as the chosen command and go to the schedule step; icon 12 opens the calendar check (stage 2); icon 13 opens the data (memory card) screen (stage 3; its glyph uses sequence 14 while `card_busy` is set and sequence 13 otherwise, `icon_card_anim_set`); icon 11 opens the system settings (stage 4); icon 14 exists only in the free layout, stores the value 0xE ("no command") and goes to the schedule step, with a notice when an exam or the school trip starts the next day. Holiday handler: 0-7 commands, 8 stage 0x10, 10 stage 0x20, 12 stage 0x30, 11 stage 0x40, 9 the appointment of the day (only when one is due today).

State: `icon_enabled[15]` (`icon_can_use_set(i, on)` stores 1 or 0; `get_icon_can_use(i)` returns 1 only when it is 1), `icon_positions[15][2]` (s16 x, y), `icon_selected_prev`, `icon_pulse_timer`.

- `icon_bar_init()`: in free layout `icon_count` becomes 15; `icon_enabled[0 .. icon_count - 1] = 1`; creates the plate actors (frame 2, priority 4) and glyph actors (sequence = icon index, priority 3); copies positions: type A from `icon_layout_a`, type B from `icon_layout_b`, free layout keeps the saved array, but if two or more of its first 15 positions are (0, 0) (never saved) it restores layout A; calls `icon_layout_apply(icon_count)`; loads three palette rows (VRAM 0x1F1, 0x1F2, 0x1F3).
- `icon_layout_apply(n)`: `n` other than 15 becomes 14 (stored back to `icon_count`); registers menu 0 with `n` hit boxes of 20 x 16 at (x - 10, y - 10); moves plate and glyph actors to the positions; actors 78 and 94 (icon 14) are hidden unless `n == 15`.
- `icon_bar_update(a)`, every frame:
  1. If L2 is held and `icon_mode == 2`: the d-pad moves the selected icon (`menu_cursor_0`) and the pointer by `cursor_speed` pixels per frame, only if `icon_move_allowed` accepts the move; `icon_layout_apply` refreshes the boxes and then the stored icon position is clamped to x -138..128, y -92..92 (the actors follow on the next refresh). With L2 held in layouts A and B nothing happens. (In the menu modes that step by pad, `menu_mode` 1 and 3, the d-pad moves `menu_cursor_0` over the enabled icons one per press, wrapping, and the pointer jumps next to the icon: emulator, from icon 0 two presses of left reach icon 13 and then icon 12.) Without L2: `menu_check(0, pointer x, pointer y)` and `menu_cursor_update(0)` (hover detection).
  2. `icon_move_allowed(i, dx, dy)` returns 0 when the moved icon's box leaves x -160..160 or its top edge leaves y -120..120 (the bottom edge is not tested: the code repeats the top test), or when a corner of the 20 x 16 box falls inside another icon's box.
  3. Selection change: when `menu_cursor_0 != icon_selected_prev`, `icon_pulse_timer = 0x20` (it counts down).
  4. Per icon: disabled -> glyph `anim_flags = 0`, plate `clut_row = HUD_CLUT_ICON_DISABLED` (0x51), plate priority 3. Enabled -> plate priority 4, glyph priority 3; hovered (`menu_items[i][0].hover != 0`) -> plate `clut_row` 0x52, glyph priority 2, plate priority 3, and when `a == 0` the glyph scale is `0x1000 + rsin((0x20 - timer) << 6)` on both axes (a pulse after a selection change); otherwise plate `clut_row` 0x53 and scale 0x1000.
  5. Icon 13's animation: sequence 14 (frame reset when >= 4) while `card_busy`, else sequence 13 (frame reset when >= 6).
  `a != 0` (17 call sites) is the display-only form of the bar: no pulse.
- Layouts (x, y in screen coordinates; `icon_layout_a`: 15 entries): A = (-138,-41) (-114,-41) (-90,-41) (-138,-25) (-90,-25) (-138,-9) (-114,-9) (-90,-9) (-138,15) (-114,15) (-90,15) (-138,31) (-114,31) (-90,31) (-66,31). B = (-138,-45) (-114,-45) (-138,-29) (-114,-29) (-138,-13) (-114,-13) (-138,3) (-114,3) (-138,19) (-114,19) (-90,19) (-138,35) (-114,35) (-90,35) (-66,35). Emulator: the weekday run state held layout A in `icon_positions` and only icon 0 enabled; the command screen state of 1995-04-09 (first selection) had icons 0-3, 5-7 and 11-13 enabled (club icon 4 off for a player without a club, 8-10 and 14 off).
- `icon_disp_switch(1/0)` shows or hides the plate and glyph actors of the first `icon_count` icons.

## 9. Season, summer and vacation helpers

| function | rule |
|---|---|
| `season_index()` | month 3-5 -> `SEASON_SPRING` (0), 6-8 -> 1, 9-11 -> 2, 12, 1, 2 -> 3 |
| `in_summer_months()` | 1 for months 6-9, else 0 |
| `summer_phase()` | `in_summer_months()`, but -1 from May 25 on (month 5, day >= 25) and from September 24 on; the random encounter chooser in the queue builder uses == 1 for summer-only encounters and == 0 for the others, so -1 allows neither |
| `vacation_for_date(month, day)` | 1 (summer break) for August and July 24 on; 2 (winter) for December 25 on and January 1-7 (month 13 counts as January); 3 (spring) for March 24 on and April 1-3; else 0. TEL date planning offers the pool and sea only when it returns 1 and the skating rink and ski resort only when it returns 2. The `vacation` field (core 3.3) is set by the notice mode, not by this function. |

Season index selects the BGM bank (section 10), the check-pattern palette (section 11) and a background image table in the weekday unit.

## 10. Day music

- `day_bgm_load()`: `sound_request(5)` and `sound_request(0x71)` stop the current music, then `sound_bank_load_start(0, 0xBF98, 0xBF79, d, e, f)` with the bank files of the day: `vacation` 1, 2 or 3 uses three fixed sector triples (summer, winter, spring break); `vacation == 0` uses `season_bgm_lba_d/e/f[season_index()]` (spring 0xD3D5/0xD387/0xD37D, summer 0xD4E7/0xD49E/0xD488, autumn 0xC70B/0xC6C3/0xC6B6, winter 0xD8CB/0xD87E/0xD874). On 1996-09-01 the summer-break files are still used although `vacation` is already 0. Most scenes call it when they return to the day screen (33 call sites in main and overlays); the sound unit owns the bank protocol.
- `season_bgm_load()`: the same prologue and the season banks only; `ending_start` calls it before it waits for the load with `sound_bank_wait_ready`, starts sequence 0x200 and enters `MODE_ENDING`.
- `bgm_load_finish_step()` (never called) would wait for the load, call `bgm_play(0, 0)` and `main_stage_next`.

## 11. Background check pattern (`screen_frame_draw`)

Core named it. Each frame it advances a scroll offset (`checker_scroll_u/v`) by a rule that depends on the player's health state (`player_flags` bits 8-11): 0 (and anything else) full speed (v - 1 every 2nd frame, u + 1 every frame); 1 about half; 2 about a quarter; 3 about an eighth; 4 random jitter of +-16 per frame; 5 a sine and cosine swirl. Then it draws a 10 x 8 grid of 32 x 32 px sprites (texture page 0x3C0, 0x100, uv offset by the scroll, grey tint = `screen_brightness`, CLUT at VRAM (0xA0, 0x1E1), ordering slot `checker_ot_slot`), skipping cells by the mask argument: 0 draws all 80, 1 leaves out columns 1-8 of rows 1-5 (the screen border only), 2 leaves out rows 0-5 except the two edge cells of row 5 (the lower two rows and the edges above them). Call counts: argument 2 at 38 sites, 1 at 22, 0 at 12.

Seasonal colour: `season_palettes[4]` (one 16-colour row per `season_index`) are blended into the CLUT by a 16-step crossfade (weights `255 - fade` and `fade - 239`, shifted right by 4, one step every 4 frames) that runs only while `season_palette_fade >= 0xEF`; at 255 it loads the palette of the current season and the counter wraps to 0. In the main executable only NAME_ENT writes the counter (0xFE, which loads the current season's palette once). Open question 2.

## 12. Biorhythm and the command roll

`biorhythm_value(physical, emotional, intellectual, scale)` evaluates three sine waves: phase `p = biorhythm_birth_offset() + day_count` (day_count is the u16 `GameState` day counter; callers may add days ahead by temporarily adding to it), waves `rsin(p * 0xB2)`, `rsin(p * 0x92)`, `rsin(p * 0x7C)`; one rsin turn is 4096, so the periods are 4096 / 178 = 23 days, 28 days and 33 days (the classic biorhythm cycles; the labels follow the commands each wave judges, section below). The three flags select which waves count.

- For each selected wave w: count + 1; if |w| < 0x199 (a critical day) total -= 0x1000 and bad + 1; else if w > 0 total += w; else total -= w / 2 (a negative wave adds half its magnitude, as coded).
- `scale > 0`: if total / count < 0 return 0, else `(total * scale / 4096) / count`. `scale < 0`: the first selected wave times scale / 4096 (the graph uses -32). `scale == 0` (no caller): -1 when a selected wave is critical (with all three selected: when at least two are), else 1.
- `biorhythm_birth_offset()`: `5475 - birthday_day_table[birth month] - birth day`; for a birthday on April 1-3: `5506 - birthday_day_table[3] - day`. `birthday_day_table` (u16 [13]): 0, 95, 64, 35, 370, 340, 309, 279, 248, 217, 187, 156, 126 (days from the first of the month to a reference date near April 5 of the next occurrence). It is a phase constant only; the sign of the day term is as coded.

`command_roll_poor_result(days_ahead)` decides whether a weekday command goes poorly. It returns 1 (poor) or 0 (normal); the queue builder stores it in the school-day record's scene field and the holiday handler in the record of the chosen command, and `parameter_up_down` halves the positive parameter gains of a poor result and adds one extra point of stress (its stress term is the scaled row value plus `arg2 - 1`, with `arg2` 1 for a normal and 2 for a poor result). Steps (as coded, `day_count` is temporarily advanced by `days_ahead` and restored):

1. base by `last_command` low nibble: 0 liberal arts: `biorhythm_value(0,0,1,16) + liberal arts / 16`; 1 science: same wave + science / 16; 2 art: `(0,1,0,16) + art / 16`; 3 athletics: `(1,0,0,16) + athletics / 16`; 4 club: wave by the player's club (clubs 0 literature, 2 science, 3 computer: intellectual; 1 drama, 4 art, 5 brass band: emotional; 6-10 sports: physical) `+ club exp / 16` (the club record's 16-bit exp shifted right 4); 5 trivia: `(0,1,0,16) + trivia / 16`; 6 looks: `(0,1,0,16) + looks / 16`; 7 rest: a probe of all three waves is evaluated and dropped, base = 16.
2. `score = base + 6`; `stress factor f` = 4 when stress >= 97, else stress / 32; `score = rand() % (score + 4)`; `score = (8 - f) * score / 8`.
3. Three probes at scale 3: physical, emotional and intellectual wave alone; each probe that returns 0 counts as a failure (a critical wave, or a contribution that rounds to 0). If all three fail, `score /= 4`.
4. Return 1 when `score < 4`, else 0.

Emulator: `break` on `command_roll_poor_result` during the first weekday run showed it called with 1 from the holiday handler (`func_80076000` below `mode_holiday`) and with 0 from `day_events_build_week` (via `schedule_init`); inside it `biorhythm_value(0,0,1,16)` then the three scale-3 probes ran.

The wave labels: command 3 and the sports clubs use the 23-day wave, arts and trivia/looks the 28-day wave, the study commands the 33-day wave (HYPOTHESIS for the words physical, emotional, intellectual; the mapping is code).

`biorhythm_graph_draw()` (calendar check page): a pulsing grey zero line from (-128, -12) to (124, -12), then three polylines of 16 points each, one per wave, for the days `day_count - 8 .. day_count + 7` (the graph calls `biorhythm_value(.., -32)`, so a wave is plotted as `-sin * 32 / 4096` pixels around the zero line, 16 px per day, x from -144); the line colours cycle with `(i + frame_count) & 15`. `calendar_view_update()` calls it first. Emulator screenshot of the calendar check page: the three curves (green, red, blue in the picture) cross both month pages from the left edge of the notebook to about x = 100.

## 13. Calendar page

Used by the command screen (mode 0x34) calendar check (icon 12, stage 2) and by TEL's "choose the day for a date" step. Emulator (command screen state of 1995-04-09, icon 12 chosen with the pad, then circle): mode 0x34 stage 2; the page is an open two-page notebook, this month on the left (April) and next month on the right (May), columns Sunday to Saturday, the day numbers (Sundays and holidays in red) are part of the month images, the three biorhythm curves run across both pages, the backing panel `cal_base_show` hides all of the parameter panel except its top row, a title banner slides in and out, and the small marks grow, hold and shrink on the 256-frame cycle below. Build step: `calendar_screen_init()` (header sprite on actor 96, frame 0x43 at (-16, -19), priority 4, then `calendar_marks_build()`), plus `cal_sprite_init()` (four quads: month images 0x7E x 0x7E at x -136 and 0 from VRAM tile page 0x1E, month number plates 0x18 x 0x10 at x -88 and 48). Per frame: `calendar_view_update()` = `biorhythm_graph_draw`, `dtd_on_tpage(0, 0, 1, 2, 0)`, `cal_base_show()` (panel (-160, -87), 283 x 125, slot 5, colour 0xE0E0E0) and the mark pulse: `t = marks[0].anim_timer++`; for actors 0x61..0x9F, phase `(t >> 6) & 3`: 0 scale = (t & 63) << 6 (grow), 1 scale 0x1000, 2 scale = (63 - (t & 63)) << 6 (shrink), 3 scale 0 (hidden); the cycle is 256 frames.

Month data. `month_calendar[2]` (core) holds this month and next month: `day_kind[0]` = days in the month, `day_kind[d]` = `DayKind`, `marker[0]` = weekday of the 1st (0 Monday .. 6 Sunday), `marker[d]` = `CalendarMarker`. `month_calendar_load(year, month, advance)`: `idx = (year - 95) * 12 + month - 4`; when `idx < 35` it uploads the two 32 x 128 images of blobs `idx` and `idx + 1` of `month_data_table` to VRAM (0x3A0, 0x100) and (0x3A0, 0x180); when `advance != 0` it also copies record 1 to record 0 (0x40 bytes) and loads record 1 from blob `idx + 1` (day count, first weekday, then that many kinds and markers). `advance == 1` is used at month changes and in the OPTION date setup; `advance == 0` reloads the images after an overlay overwrote VRAM (BUNKAKEN, BUNKASAI, TAIIKU, TACO, the command screen).

Cells. `calendar_cell_x(side, d)` = `side * 0x88 + column * 16 - 0x78`, `calendar_cell_y(side, d)` = `row * 16 - 0x30`, where `side` 0 is this month and 1 next month, `c = (first + d + 1)` with `first` = `marker[0]` of the side (a Sunday first day, 6, counts as -1) and `d` the 0-based day; column = `c % 7`, row = `c / 7`. Columns run Sunday to Saturday. TEL uses them minus 16 as 16 x 16 pick boxes for the days of the next 28 days that are Sundays or holidays (`day_kind` 1 or 2) without an appointment.

Marks (`calendar_marks_build`): clears 0x20 actors from slot 0x61 (draw_flags 0x8C, direct frame mode, brightness 0x80, off screen), then in this order, adding one actor per mark (stop when slot 0xA0 is reached):

1. player birthday: `CAL_FRAME_PLAYER_BIRTHDAY` (0x6E) on this month's cell when the birthday month is this month, on next month's cell when it is month + 1 (no December wrap here).
2. girls 0-10 with `birthday_known` (flag bit 6): `CAL_FRAME_GIRL_BIRTHDAY` (0x6D) in the same way, with the December to January wrap.
3. the eight appointment slots, if active: `CAL_FRAME_DATE` (0x62) for this month and for `month % 12 + 1`.
4. sports-club members (club id 6-10, bits 12-15 of `player_flags`): `CAL_FRAME_CLUB_EVENT` (0x6F) on the third Sunday, day `21 - weekday of the 1st` (this month; next month's too, except in February 1998).
5. per day of this month (then next month): when `marker` is not 0 and `calendar_marker_frames[marker]` is not 0xFF the frame is marked, except frame 0x6F which is only marked in August (this month 8 / next month when this month is 7) and only for players who have a club. A frame of the kinds `CAL_FRAME_EXAM` (not when the month is February), `CAL_FRAME_SCHOOL_TRIP` or the August 0x6F repeats on the following days that have no marker of their own for 6 days in total, `CAL_FRAME_WEEK_EVENT` (0x67) for 7 days.
6. past encounters: every `event_log` entry of the current year and month marks its day with 0x62; girl 12 entries only when her `SystemData.flags[12]` or `girl_cleared[12]` is set.

`calendar_marker_frames[marker]` (24 entries, 0xFF = none): 2, 3, 10, 16, 21 -> 0x65 (notice frame); 4 sports festival -> 0x64; 5 exam -> 0x66; 8 club season -> 0x6F; 11 school trip -> 0x6C; 12 culture festival -> 0x63; 13 Christmas preparation -> 0x6B; 15 New Year preparation -> 0x6A; 17 Valentine -> 0x68; 18 career choice -> 0x66; 19 seasonal party -> 0x69; 23 -> 0x67; the others 0xFF (core 3.2 names the markers).

## 14. Information magazine and date places

A quarterly page (the holiday handler titles it the information magazine) that unlocks one new date place per issue. Eight of the thirteen issues carry a place.

- Issue number from the date (`magazine_page_build`, also the bit number of `magazine_mark_read`): months 2-4 -> `4 * (year - 95)`, months 5-7 -> `4 * (year - 95) + 1`, months 8-10 -> `+ 2`, months 11-12 -> `+ 3`, month 1 -> `4 * (year - 96) + 3`; so issue k covers the three months starting at month `(3k + 2) % 12 + 1` of the page title (issue 0: March to May, issue 1: June to August, issue 3: December to February); issue 12 is the spring of 1998.
- `magazine_mark_read()` (the magazine step calls it first): sets `quarter_flags` bit `4 * (year - 95) + quarter` (quarter by months 2-4, 5-7, 8-10, 11-1, field names `spot_info_00..11`), bit 14 except in months 2, 5, 8, 11 where it sets bit 13, bit 15 in months 5-7, and both bits 13 and 14 in April 1995. `quarter_flags_rotate` (core) moves bit 13 to bit 14 at months 3, 6, 9, 12.
- `magazine_page_build()`: lays out the page for the issue: a header actor, up to ten layers chosen by quarter and year (first layer 0x79..0x7D, then variant frames by year), the page title and a second heading (`magazine_title`, `magazine_subtitle`, 13 strings each), three text lines (`magazine_lines`), three month labels `number_strings[(3k + 2) % 12 + 1 ...]` at x = 256 (a second page off the right edge of the screen, HYPOTHESIS: the magazine scrolls), three symbols `0x90 + magazine_spot_marks[k][i]`, and the narration with the player name; issues 0, 9-10 and 11-12 instead show one of three two-line notices.
- Place unlock inside `magazine_page_build` (sets `place_flags[p] = 1`): issue 1 -> place 1 (`PLACE_KIRAMEKI_CENTRAL_PARK`) unless month 5; issue 2 -> 7 (`PLACE_ART_MUSEUM`) unless month 8; issue 3 -> 3 (`PLACE_AQUARIUM`) in December and unconditionally in January; issue 4 -> 13 (`PLACE_STADIUM`) unless month 2; issue 5 -> 5 (`PLACE_BOTANICAL_GARDEN`) unless month 5; issue 6 -> 10 (`PLACE_BOWLING_ALLEY`) unless month 8; issue 7 -> 6 (`PLACE_PLANETARIUM`) in December and in January; issue 8 -> 11 (`PLACE_KARAOKE`) unless month 2; issues 0, 9, 10, 11, 12 none.
- Scheduler side (the scheduler unit's `func_8006D138`, noted here for the port): in the second and third month of a quarter a set issue bit stores 1 in `place_flags`; in the first month after the quarter, an unset bit stores `PLACE_FLAG_PENDING` (0x80); TEL's phone scene then announces one pending place, sets the issue bit (bits 1-8 for places 1, 7, 3, 13, 5, 10, 6, 11) and stores 1. GEKO events also write `place_flags` (places 14, 7, 13, 2, 3, 5, 10, 11, 15, 12, 8). At new-game start places 0, 2, 4, 8, 9, 12 and 15 are set (emulator dump).

Date panels elsewhere read `place_flags[p] & 0x7F` as "place p can be chosen" (TEL).

## 15. Appointments

The eight words of `appointment_slots` (core 3.6) are kept packed and sorted.

- `appointment_find_today(year, month, day)`: returns -1 when `month == 12 && day == 24`, when the month (13 counts as 1) is 1 and the day is 1, and when `month >= 3 && year >= 0x62` (from March 1998); else the highest 1-based slot whose month (bits 12-15) and day (bits 16-20) match, or 0.
- `appointment_count_for_girl(girl)`: number of active slots whose girl field equals `girl`.
- `appointment_list_sort()`: packs the active slots to the front, zeroes the rest, then selection-sorts by `month * 100 + day`; while the current month is 12, January entries count as month 13 so they sort last.
- `appointment_add(word)`: stores into the first inactive slot and calls the sort. With all eight slots active it writes to index 8, i.e. into `event_flags[0..3]`; no capacity test was found at the two callers (TEL after a date is planned, GEKO events), see open question 10.
- `appointment_prompt_noop()`: empty function called once before the holiday handler asks where the appointment is.

## 16. Club list

`club_list_menu()`: header actor 96 (frame 0x3F at (-16, -19), priority 4); the title text at (-128, -82); for clubs 0..10 a 64 x 16 box in columns of five (x = `(i / 5) * 84 - 128`, y = `(i % 5) * 16 - 56`) with the club name; a club whose `ClubRecord` flag byte has bit 0 set (`quit`, set when the player left that club) gets no label and width 0; a "cancel" box at (40, 8); `menu_set(1, 12, ...)`, `menu_cursor_1` on the first available club; the prompt for choosing a club to join is queued as narration; the typewriter starts at entry 1. The join screen (`join_club_*`, other unit) reads the result from `menu_cursor_1`.

## 17. Save slot panel

`save_slot_info_show(arg0, arg1)` (`arg0` unused; `arg1 != 0` puts the panel at x = -140 instead of -8) queues the text lines of one album slot and starts the typewriter at entry 6 (`k_sub_disp_start(6)`):

- slot header: "<number> album" (the number from the full-width table `number_strings`);
- if the slot is the system slot (`SystemData.selected_slot == 0`), a "system" label; if its status byte is 0 "unused", if 0xFF "data error";
- otherwise: the save date (year from two digits `year / 10`, `year % 10`, month, day with a tens digit only when >= 10), the player's family and given name, the nickname, the club (`club_name_table[...]`), and "cleared" = the count of non-zero entries among the 13 per-girl flags of the slot, capped at 99.
- Source: when `slot_info_from_system_data` (`D_800E8BEE`, HYPOTHESIS name) is set, the arrays of `SystemData` for slot `D_800E7D36`; otherwise the card block (`NameRecordF00` of the slot at `unk_1117`, status from `slot_status[save_slot]`), where the club is the top nibble of a 16-bit field.

`data_save_load_class_init()` sets the header sprite of these screens (actor 96, frame 0x43 at (-16, -19), priority 3). Callers: the command screen's data stage (mode 0x34 stage 3) and ETC's system file screen.

## 18. Small services

- Mascot (`mascot_set`, `mascot_update`, `mascot_disp_switch`; the selection `mascot_choice` is core's): a decorative sprite hanging from an anchor like a pendulum. `mascot_choice` 0 or >= 14 hides it and resets the variables (anchor (120, -55), length 15, swing 8). Otherwise actor 27 shows frame `choice - 1` (own image tables `D_800C5FD4`, `D_800C6010`, `D_800C5FCC`) and actor 28 the hand frame (0xD, or 0xE while R1 or R2 is held). Per frame: offset `dx = (rsin((frame_count & 0x7F) << 5) * swing) >> 12`, rotation `-(2 * dx / 3) << 12`, x = anchor x + dx, y = anchor y + length - (15 - sqrt(length^2 - dx^2)); a pink line (`prim_line_gradient`, colour 0xF8B0C8) joins the anchor and the sprite. A button press on pad 2 sets `swing = (length + 1) / 2`; `swing` falls by 1 every 64 frames. With R1 held the d-pad moves the anchor by 4 px (wrapping at +-0x200); with R2 held up and down change `length` by 2 (down while < 0xC4, up while >= 3). HYPOTHESIS: a toy for the player, selected in the settings screen.
- `text_marker_set` (core) places actor 2; `text_marker_bob()` moves it down 1 px every 4 frames from y 0x48 to 0x4F, then snaps back.
- `corner_icon_set(on)`: actor 15 (frame 0xAD at (-134, -92)); used with 1 around STR movies and the NAME_ENT attract scene.
- `cursor_actor_init`, `cursor_show`, `cursor_update` (core): `cursor_update` moves the pad cursor only while `cursor_moved_this_frame` (`D_8011ECA4`) is 0 and then sets it to 1; emulator write watch: the frame-start function `func_800422C8` (+0x100) writes 0 to it every frame, so the pointer moves at most once per frame even when several callers run it. `cursor_style` (`D_800B6708`): square steps it 0 -> 0x9B .. 0xAC -> 0 and writes it to actor 0's frame; 0 or a value outside 0x9B..0xAD shows frame 0. The pad cursor moves `cursor_speed` px per frame (double while triangle, circle, cross or square is held and `menu_mode & 3 == 0`; no movement while L2, R2, L1 or R1 is held), clamped to x -150..140, y -100..100.
- `event_log_scan(*first, *last, _)`: after a load, counts the `event_log` (a zero year byte ends the list; capped at 0xFF; the rest is cleared) and returns the girl of the last entry in `*last` and of the last entry whose scene is 0, 1, 2 or 0xF in `*first` (0xFF if none); `GameState` takes these as `current_girl` and `date_girl`.
- `confirm_button_label()` / `cancel_button_label()`: the text naming the confirm or cancel button: mouse (`input_device == 0`) -> right or left button label by `confirm_button`, pad -> the pad confirm (circle) or cancel (cross) label (system settings screen).
- `ui_vram_init()` (boot HUD init): uploads the 32 x 128 image at `D_800B374C` (month_data_table[1]) to VRAM (0x3E0, 0x180), clears 128 bytes of `scratch_words` and uploads them to (0x3E0, 0x1C0).
- `light_effect_draw` (core) uses `light_effect_param_a/b` (nine entries each).

## 19. Connections to the rest of the game

| consumer | uses |
|---|---|
| every day, weekday, holiday, date and event scene | the HUD show functions each frame; `day_bgm_load` on return |
| queue builder (scheduler unit) | `command_roll_poor_result`, `summer_phase`, `appointment_find_today`, `appointment_count_for_girl`, `month_calendar_load(.., 1)`, `season_bgm_load` (ending) |
| weekday and holiday handlers | `icon_can_use_set`, `get_icon_can_use`, `icon_bar_init`, `icon_disp_switch`, `parameter_show_init`, `parameter_change` (through `parameter_up_down`), `magazine_*`, `club_list_menu`, `appointment_add`, `appointment_list_sort` |
| TEL | `calendar_screen_init`, `cal_sprite_init`, `calendar_cell_x/y`, `calendar_view_update`, `vacation_for_date`, `appointment_*`, `place_flags` |
| NAME_ENT, ETC, OPTION | `save_slot_info_show`, `event_log_scan`, `data_save_load_class_init`, button labels, `season_palette_fade` |
| dialogue and DATE/GEKO/ENDING | `bustup_*`, `sd_*`, lip-sync tables |

## 20. Hardware layer entry points (named only)

`actor_reset`, `prim_box_gouraud`, `prim_line_gradient` (plate and panel rectangles, mascot line, graph lines), `dtd_on`, `dtd_on_tpage`, `goto_tpage`, `LoadSquare`, `load_palette`, `load_tpage_buf_lock`, `GetWorkBase` / `AddPrim` (check pattern), `_sprite_set_light_effect1`, `menu_set` / `menu_check`, `set_kanji_string`, `k_disp_*`, `sound_request`, `sound_bank_load_start`, `getCDlevel`, `rsin`, `SquareRoot0`. The port replaces them with engine drawing; the logic above only needs the sprite positions, frames, colours (CLUT rows) and the order.

## 21. Hypotheses and open questions

1. `season_palette_fade` (`D_800B5BC8`): the crossfade to the next season's palette starts only when the counter is 0xEF or more; only NAME_ENT writes it. Who changes the palette when the season changes during play (maybe a path that sets the counter through a different symbol)? Needs an emulator run across a season border.
2. `mouth_frames_reversed` (`D_800B5A60`): the meaning of the flag (ENDING inverts the mouth level; DATE sets it with `event_face0`).
3. The mascot: what the settings screen shows, and whether the swing amount is ever read for anything else (HYPOTHESIS: a plaything).
4. `place_flags` bit 7 (`PLACE_FLAG_PENDING`): confirmed by the scheduler and TEL code, name is a HYPOTHESIS.
5. `quarter_flags` bits 13-15 (`spot_info_first_month`, `spot_info_later_month`, `spot_info_summer`): HYPOTHESIS names from `magazine_mark_read` and TEL's availability tests.
6. Marker 23 and frame `CAL_FRAME_WEEK_EVENT` (0x67): which event is a 7-day mark.
7. Icons 8-11 and 14 of the command screen (mode 0x34) and 8-12 of the holiday screen: the weekday and holiday units own the names; this unit only fixes the numbering and the stage each icon opens.
8. Core's `bustup_build` layout (quads 2-5 are the hair and body pieces) is not checked here.
9. Dead and unused code: `bustup_hide`, `bustup_idle_counter_wrap`, `bgm_load_finish_step`, `biorhythm_probe_unused`, `cal_sprite_disp_switch`, `appraisal_base_show`, `appointment_prompt_noop`.
10. The bottom edge test of `icon_move_allowed` repeats the top test, `appointment_add` writes past the array when all slots are active, and a negative wave adds to the biorhythm total; these are coded as described (a port should decide whether to keep them).
11. `HudClutRow` names only the values used here; a screenshot of Sunday 1995-04-09 shows the weekday plate in red (row 0x44); the colour of row 0x43 (Saturday, raised digits) was not seen.
12. `slot_info_from_system_data` (`D_800E8BEE`): what NAME_ENT's 20 tests mean (HYPOTHESIS name).
13. Name conflicts with other units: the proposals use the hardware names (`actor_reset`, `prim_box_gouraud`, ...) where core differs; core's own names for `bustup_*`, `message_window_*`, `icon_bar_update`, `mascot_update` and `screen_frame_draw` are kept. The new type `ProgressState` and the three field rows under it need a header change; `GameState.unk_120` keeps core's name `progress_state`.
14. The first two 0x20-byte rows of `D_800C6220` serve both as digit palettes (VRAM rows 0x1E0, 0x1E1) and as icon palettes (0x1F2, 0x1F3); rows 3 and 4 are the up and down colours (`clut_row` 0x43, 0x44); row 2 is not used by this unit.
15. Answers to the core spec: open question 5 (the 21 `progress_state` flag bytes are the date place flags, section 14; the next nine are command counters); open question 6 in part (`sd_anim_fast` halves the flip time of the parameter panel digits, section 5; the animations of actors 96 and 97 were not checked); open question 15 (file names `hud_panels.c` and `hud_screens.c`; the second file mixes music, mascot, calendar, magazine and menus and could be split at function boundaries into `bgm.c`, `mascot.c`, `calendar.c`, `magazine.c` and `biorhythm.c` when the splat segment is cut).
