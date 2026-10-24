---
type: spec
updated: 2026-10-24
unit: DATE
ticket: T-27022
sources: ["src/ovl/DATE", "include/ovl/DATE.h", "config/renames/proposals/o-date.tsv", "config/renames/proposals/o-date-main-requests.tsv", "wiki/spec/core.md", "wiki/spec/m-dates-b.md", "wiki/spec/o-rpgbat.md", "wiki/families/date-family-results.md", "wiki/emulator.md"]
---

# Overlay DATE: dates (port spec)

Phase 2 spec for [[tickets/T-27022-p2-o-date]] ([[tickets/T-27000-phase-2]]). It describes what the DATE overlay (668 functions, `src/ovl/DATE`, load address 0x80132000) does, as the matched C implements it, with the names proposed in `config/renames/proposals/o-date.tsv`. Old names are given in backticks where a reader needs the link to the current code. Facts only, in this project's words, no game text. A claim of game meaning that the code does not prove is marked HYPOTHESIS. Scope (user decision of 2026-10-24): game logic, state, flow and data tables are described fully; the PS1 hardware layer (GPU primitives, sprites, VRAM, sound driver, CD/XA streaming, MDEC) only at module level (section 12).

Prefix: `date_` (the established prefix of the overlay: the patch names `date_fill_sections_<girl>` and `date_place_names`; the two patch names with the trap word `event`, `date_event_trigger` and `date_event_choose`, become `date_scene_trigger` and `date_scene_choose` because GLOSSARY Traps maps a scripted scene with a girl to `scene`). Girls are named by given name as in `core` (`GIRL_*`), places by the `core` `Place` enum in snake case. Folder proposal: `src/ovl/date/` (table rows of kind `file`). The core names are used for the shared state (`current_girl`, `date_girl`, `girls[]`, `event_counters`, `event_flags`, `appt_place_*`, `day_event_next`, ...). Where this unit sees a different role in a core field, section 14 says so.

How facts were obtained: the matched C, the step-table data in the asm, the proposals of `core` and `m-dates-b` (the main scene library: `normal_date_*`, `dialogue_run`, `bustup_*`), and about twenty short emulator runs (section 13) from a boot-to-title save state with memory writes that enter mode 0x50 with chosen girl, place, calendar and parameter values. The runs confirm the stage and step order, the meeting-place question, the score formula, the end-of-date bookkeeping, both birthday stages and one special scene trigger.

Table size: 1,103 rows in `o-date.tsv` (658 `func`, 192 `data`, 4 `field`, 60 `type`, 20 `file`, 158 `const`, 11 `comment`), 23 of them marked HYPOTHESIS; 11 functions keep their names (the `date_fill_sections_<girl>` patch names, with `comment` rows); `date_event_trigger` and `date_event_choose` are renamed (`date_scene_trigger`, `date_scene_choose`), `date_shiori_dispatch` to `date_scene_shiori_dispatch` for symmetry with the other eleven girls, and the alias label `func_80132004` inside `date_choice_tables_init` (the address `date_load_choice_tables` calls) has its own row. The 18 main-exe variables that DATE names are not rows of this table: an overlay table may not rename main symbols, so they are in `config/renames/proposals/o-date-main-requests.tsv` (18 rows, 2 marked HYPOTHESIS; section 3 lists them).

## 1. What the overlay is

DATE is the overlay of the dating system. It hosts two main-exe modes:

| mode | name (core) | entry | what it is |
|---|---|---|---|
| 0x50 | `MODE_DATE` | `date_entry` (`func_801448D4`) | a date at a place chosen by the player: meeting, conversation with one answer, reaction, farewell, optional birthday scenes |
| 0x5E | `MODE_DATE_SCENE` | `date_scene_entry` (`func_8014C08C`) | one of 48 scripted special date scenes (46 of the girls 0-10 at a place and season, and two intrusions of Miharu); replaces the rest of a date |

Both are started and finished by main code, not by DATE itself:

- A date starts from `appointment_launch` (main, `src/main/80075320.c`): it copies the first appointment slot into the queue record `DAY_EVENT_APPOINTMENT` (0x80: girl, place), stores the girl in `date_girl` and `current_girl`, the "appointment flag" in `appointment_flag` (`unk_75C`) and calls `main_state_set(0x50)`. The debug menu of OPTION also enters 0x50 and 0x5E (items 1 and 14).
- A date ends with `day_event_next(1)` in `date_end` / `date_scene_end`: the queue reader goes on with the next day event (mode 0x31, or 0x22 while the debug flag is set).
- Other overlays are launched from inside a date and return into it: RPG_BAT (mode 0x81, the street-gang ambush, section 4.6), EN_NICHI (mode 0x82, the festival stalls at the shrine, section 7) and DATE2 (mode 0x5C, the karaoke box, section 5.9). The caller state is saved in `saved_mode`, `saved_stage`, `saved_step` and restored by main.

The overlay is script-driven: the lines of every conversation are not in the overlay. A conversation is a "section" of a script file on disc (text, command and voice tables, run by the main dialogue engine `dialogue_run`, see `spec/m-dates-b` (unit m-dates-b, merged separately)); DATE decides which group and line to play, which face to show, and what the answer is worth. The tables that give the worth of the three answers (response classes) are also disc data (the choice file, section 10).

## 2. Entry and stage machine of mode 0x50

Flow in one picture:

```
appointment_launch -> mode 0x50
  stage 0  load music/scripts/choices -> place setup -> [meeting-place question] -> portrait in, greetings -> ambush check
           (girl refuses, wrong place, bad looks: short branch -> date_end)
  stage 1  talk -> (special scene? mode 0x5E) -> walk to the spot -> conversation -> ANSWER -> score -> reaction
           -> back -> farewell -> [girl's birthday -> stage 2] [player's birthday -> stage 3] -> closing -> date_end
  stage 2  gift menu -> reward -> reply -> [player's birthday -> stage 3] -> closing -> date_end
  stage 3  greeting -> closing -> date_end
date_end -> day_event_next(1)
```


`date_entry` runs once per frame. It calls `bustup_wink_update` (main), switches on `GameState.stage` and runs the shared screen updates (`check_k_scroll`, `k_disp_inc2`, the screen frame, message window, title slide, calendar HUD, `scene_frame_update`, screen shake). Any other stage value goes to `game_reset_to_title`.

| stage | name | handler | steps | purpose | next |
|---:|---|---|---:|---|---|
| 0 | `DATE_STAGE_ARRIVAL` | `date_stage_arrival` | 76 | meeting at the place: loads, arrival, greeting, ambush check | stage 1 (`main_stage_next` at step 0x30) or end |
| 1 | `DATE_STAGE_PLACE` | `date_stage_place` | 18-104 by place | the date: conversation, answer, reaction, farewell | `date_end`, or stage 2 / stage 3 from `date_birthday_check` |
| 2 | `DATE_STAGE_GIRL_BIRTHDAY` | `date_stage_girl_birthday` | 28 | gift choice on the girl's birthday | `date_end` (or stage 3 on the player's birthday too) |
| 3 | `DATE_STAGE_PLAYER_BIRTHDAY` | `date_stage_player_birthday` | 15 | the girl's greeting on the player's birthday | `date_end` |

Every stage handler copies its step table (an array of function pointers) to the stack and calls `table[step](0x80)` once per frame. A step function does its work and calls `main_step_next` when it is done (the `0x80` argument is only read by `wait_sub_sub`). Several steps jump by `main_step_set` or `step += n`; the jumps are listed with the steps. `step_timer` is advanced by the step functions that need it, not by the loop (`spec/m-dates-b`). `date_stage_arrival` and `date_stage_place` add the hooks described in sections 4 and 5; both also draw a "light effect" layer (`light_effect_draw`, HYPOTHESIS: glare or sparkle) while bit 0 of `scene_effect_flags` (main word `D_80122D44`, named by `m-flow`: bit 0 light effect on, bit 1 title banner on) is set. Bit 0 is set at places with sun or water (central park variant 0 in months 3-8, the pool, the sea, the ski resort, each only for some step ranges) and on the three summer Sundays 1995-07-23, 1996-07-28 and 1997-07-27 (HYPOTHESIS: the summer festival day; the same dates select group 1 of the special-day greeting).

Mode 0x5E (`date_scene_entry`): while stage 0 step 0 shows the title, runs `bustup_wink_update`, switches on `dialogue_work.girl` (`D_80122CD4`, the girl id 0-11; other values reset to title) to the girl's dispatcher `date_scene_<girl>_dispatch`, which switches on `dialogue_work.scene` (`D_80122CD0`, 2-6) to the handler of one season (`date_scene_<girl>_<season>`), a step-table runner like the stage handlers. Section 9.

## 3. State

New names; GameState fields keep the `core` names. Rows marked (R) are main-exe variables that DATE names. An overlay table may not rename main symbols, so they are not rows of `o-date.tsv`: they are requests in `config/renames/proposals/o-date-main-requests.tsv` for the merge step and the main owner. Two main words that DATE uses are already named elsewhere and have no request: `date_spot` (`D_80122D08`, `DialogueWork.unk_38`, `m-dates-b`) and `scene_effect_flags` (`D_80122D44`, `m-flow`).

| name | old | meaning |
|---|---|---|
| `date_place` (R) | `D_800CA2F8` (u8) | place id 0-20 (`Place`): copied from `appt_place_0` by `date_init`; changed by the meeting-place question |
| `date_spot_variant` (R) | `D_800CA2FC` (`DatePlaceView.option`) | variant of this visit (0, 1, ... 5; the art museum calls it the option), set by the place setup (section 5.3) |
| `date_spot` | `D_80122D08` (`dialogue_work` +0x38, `m-dates-b`) | spot id (`DateSpot`) = index of `GirlRecord.event_counters`; stored as `girl_last_date_spot` |
| `date_arrival` | `date_arrival` | 0 girl late, 1 on time, 2 player late (section 4.2) |
| `dialogue_group`, `dialogue_line` | `D_800CA148/14C` (core) | live group and line cursor of the place conversation; advanced by `dialogue_run` when a group ends |
| `date_groups_per_variant` | `D_8015B658` | group stride per variant (section 5.3) |
| `date_choice_class[3]` (R) | `D_800CA22C` | response class (0 best .. 4 worst) of the three answers |
| `date_choice_face[3]` (R) | `D_800CA21C` | face the girl shows for each answer: the argument of `bustup_layout_set`, an expression index 0 (best mood) .. 4 (worst), see `spec/m-dates-b` |
| `date_choice_mood_face[3]` (R) | `D_800CA224` | face after the answer: class clamped to 1..3 (0 -> 1, 4 -> 3) |
| `date_choice_tier[3]` (R) | `D_800CA234` | class - 1 clamped to 0..2; offsets the farewell line inside a mood tier |
| `dialogue_choice` | `D_80122CDC` (core/`m-dates-b`) | original index (0-2) of the picked answer |
| `date_special_scene_active` / `date_special_scene_flag_index` (R) | `D_800CA364` / `D_800CA368` | a special scene replaced the date / the `event_flags` index it set |
| `dialogue_work.scene`, `dialogue_work.girl` | `D_80122CD0/CD4` (core) | which special scene runs (mode 0x5E): scene 2-6 and girl 0-11 |
| `dialogue_work.launch_code` (R, field `DialogueWork.unk_10`, alias label `dialogue_launch_code`) | `D_80122CE0` | RPG_BAT launch code (4 gang ambush, 5 boss ambush, set by DATE) and, after the battle, its result (0 won); `o-rpgbat` uses the same word under its `D_` name. It is word 4 of the dialogue work, which `dialogue_clear` (words 5-30) leaves alone |
| `scene_effect_flags` (`m-flow`) | `D_80122D44` | bit 0 the light effect is drawn (section 2), bit 1 the title banner is on (what the loader screen draws, `m-flow`). DATE sets bit 1 in `date_scene_trigger`, in Miharu's two intrusions (spring: bit 1; summer, at the sea: bits 0 and 1) and, with the whole word set to 2, in `date_scene_choose` just before it hands the karaoke place over to DATE2 (mode 0x5C); it also sets bit 0 for the sun and water places (section 2). `date_end` and `date_scene_end` clear the word. GEKO writes it too. Bit 1 does not mean that a special scene runs: `date_special_scene_active` does |

The other requested main variables, which the notes of `o-date.tsv` mention (`date_fireworks_active`, `date_parade_sign_visible`, `date_ambush_talk_active`, `date_scene_group`, `date_scene_line`, `date_farewell_group_base`, `date_farewell_line`, `festival_stall_result`) are in the same request table; `date_fireworks_active` (+0x44) and `date_parade_sign_visible` (+0x64) lie inside `DialogueWork`, so the main owner may prefer field rows for them.

Girl and player state read or written (core names): `girls[g].tokimeki`, `.affection`, `.syoushin` (16.16 words; DATE changes only the integer part), `.flags` (bits `met`, 4 = Golden Week flag, 5 = reached the best mood, 6 = birthday known, 12/13 = invitation flags, 14 = a per-girl flag used by the Mio and Yukari winter scenes, `story_stage` bits 9-11), `.event_counters[spot]`, `.birthday`; `girl_state_rank(g)` (0 best .. 4 worst) and `get_g_zyotai_h` (the "h" rank); `param_trivia`, `param_looks`, `param_athletics`, `param_science`, `param_condition`, `param_stress`; `event_flags[]`, `girl_last_date_spot[]`, `girl_last_date_spot_timer[]`, `date_place_history[4]`, `event_log`, `appointment_slots[0]` (active bit), `player_flags` (bits 17, 19, 20 and the club nibble), `daily_event_bits`, calendar bytes, `frame_count`.

## 4. Stage 0: meeting at the place

### 4.1 The 76-step table

`date_init` (step 0) runs `draw2d3d`, `actor_draw_enable`, sprite and actor clears, resets the text system, sets brightness 0x80, sets bit 8 of `daily_event_bits` (a scene done-today mark, core open question 16), copies `appt_place_0` into `date_place`, `date_girl` into `dialogue_work.girl` and `current_girl`, calls `dialogue_names_set`, and sets `date_arrival`: 1, but 0 when `(frame_count & 3) == 0` and the girl is not Yuina (a 25% chance by frame count that she arrives late). Then step 1.

The table (the setup function in step 9 and step 0x16 is replaced at run time by the setup of the current place, section 5.3):

| step | handler | what it does |
|---:|---|---|
| 0x00 | `date_init` | init the scene (see section 4.1) |
| 0x01 | `date_place_music_load_start` | start the place music bank load |
| 0x02 | `date_place_music_load_wait` | wait for the bank, start the place sequence |
| 0x03 | `date_load_shared_script` | read the shared script file (2 sectors) |
| 0x04 | `scene_wait_cd_read` | wait for the CD |
| 0x05 | `date_load_girl_script` | read the girl's script file, fill her 27 section pointers |
| 0x06 | `scene_wait_cd_read` | wait |
| 0x07 | `date_load_choice_tables` | read the choice file (3 sectors), wire its table pointers |
| 0x08 | `scene_wait_cd_read` | wait |
| 0x09 | `date_place_setup_neighborhood_park` | place setup (replaced by the place's own setup function) |
| 0x0A | `scene_wait_cd_read` | wait for the place background read |
| 0x0B | `dec_init` | check and decode the background |
| 0x0C | `change_dec_bg` | show it |
| 0x0D | `date_arrival_init` | arrival init: counters, greeting group |
| 0x0E | `date_scene_reveal_start` | reveal wipe |
| 0x0F | `date_place_enter_check` | title and special-scene check |
| 0x10 | `date_meeting_place_check` | meeting-place check: right place or alone jumps to 0x21, mood 4 jumps to 0x34, else builds the question |
| 0x11 | `date_wrong_place_speak` | speak the question |
| 0x12 | `date_meeting_place_select` | two- or three-way answer |
| 0x13 | `date_meeting_place_resolve` | resolve the answer: stay -> 0x1F or 0x31, move -> 0x14 |
| 0x14 | `date_wrong_place_speak` | speak (she asks to go to the other place) |
| 0x15 | `normal_date_bg_fadeout` | fade the background out |
| 0x16 | `date_place_setup_neighborhood_park` | place setup again (for the corrected place) |
| 0x17 | `scene_wait_cd_read` | wait |
| 0x18 | `dec_init` | decode |
| 0x19 | `change_dec_bg` | show |
| 0x1A | `date_place_music_load_start` | place music bank load |
| 0x1B | `date_place_music_load_wait` | wait for the bank |
| 0x1C | `date_place_reload_steps` | reload script files (3 reads) |
| 0x1D | `date_place_init_with_cost` | place_init, group offset |
| 0x1E | `date_wrong_place_outcome` | outcome of the correction: not the appointment place -> 0x32, else syoushin +8 |
| 0x1F | `date_wrong_place_speak` | speak the meeting talk |
| 0x20 | `date_mira_late_skip` | Mira special skip |
| 0x21 | `date_load_special_portrait` | special portrait for shrine and ski |
| 0x22 | `read_bustup` | start the girl's portrait read |
| 0x23 | `scene_wait_cd_read` | wait |
| 0x24 | `date_portrait_address_init_step` | portrait sprite addresses |
| 0x25 | `bustup_show_checked_hidden` | build the portrait (checked) |
| 0x26 | `date_choice_classes_derive` | derive classes |
| 0x27 | `date_empty_step` | no-op |
| 0x28 | `date_looks_check` | looks check (below 30, Mira 35: bad greeting, -1/-1/+5, jump to 0x37) |
| 0x29 | `date_scene_choose` | special-scene check |
| 0x2A | `date_arrival_late_gate` | late-arrival wait talk (only date_arrival 0) |
| 0x2B | `wait_sub_sub` | wait |
| 0x2C | `date_girl_in_clear_appointment` | portrait in; clear appointment active bit |
| 0x2D | `date_arrival_greeting_speak` | arrival greeting talk |
| 0x2E | `date_ambush_check` | ambush check |
| 0x2F | `date_special_day_greeting` | special-day greeting |
| 0x30 | `main_stage_next` | next stage (stage 1) |
| 0x31 | `date_wrong_place_speak` | speak: she is not here |
| 0x32 | `date_meeting_set_not_here_group` | meeting group 7 |
| 0x33 | `wait_sub_sub` | wait |
| 0x34 | `date_wrong_place_speak` | speak |
| 0x35 | `normal_date_bg_out` | cover wipe |
| 0x36 | `date_end` | date end (failed meeting) |
| 0x37 | `bustup_expression_3` | face 3 (looks branch) |
| 0x38 | `date_girl_in_clear_appointment` | portrait in; clear appointment active bit |
| 0x39 | `date_arrival_greeting_speak` | speak (greeting group 12) |
| 0x3A | `normal_date_girl_out` | portrait out |
| 0x3B | `date_arrival_greeting_speak` | speak |
| 0x3C | `normal_date_bg_out` | cover wipe |
| 0x3D | `date_end` | date end |
| 0x3E | `date_ambush_after_battle` | resume point after the ambush battle: rewards, rebuild the screen |
| 0x3F | `date_end` | date end |
| 0x40 | `date_ambush_prepare_battle` | prepare the ambush talk |
| 0x41 | `scene_wait_cd_read` | wait |
| 0x42 | `date_ambush_bank_load_start` | bank load start (battle music) |
| 0x43 | `date_ambush_bank_load_wait` | wait for the bank |
| 0x44 | `normal_date_girl_suddenout` | portrait out at once |
| 0x45 | `date_ambush_bang` | bang text and sounds |
| 0x46 | `don_init2` | screen shake start |
| 0x47 | `don_wait` | wait |
| 0x48 | `normal_date_speak` | speak the ambush talk |
| 0x49 | `normal_date_bg_fadeout` | fade out |
| 0x4A | `date_ambush_start_battle` | start RPG_BAT |
| 0x4B | `game_reset_to_title` | guard: reset to title |

Observed order (emulator, aquarium, girl 0, right place, on time): steps 0..0x10 in 240 frames, then the jump to 0x21, steps 0x21..0x30 in 245 frames, stage 1.

### 4.2 The meeting-place question and `date_arrival`

Main (the holiday command, `src/main/80075320.c`) sets four bytes before launching: `appt_place_0` (`unk_724`) = the place the player picked, `appt_place_1` (`unk_725`) = the true place of the appointment being launched (first slot), `unk_726/727` = up to two other places or 0xFF. `func_800764AC` builds the candidate list from the places of all pending appointment slots (up to 8 slots, at most 3 distinct places; the seasonal places 16-20 are ignored, and when the first slot is one of them no question is asked). With fewer than two distinct places the player is not asked and all four bytes are the true place. Otherwise main asks where the meeting place was (a string of the main code) and shows the candidates as a menu in random order; the pick becomes `appt_place_0`; `appointment_launch` then moves the true place into `appt_place_1` and the remaining candidates into `unk_726/727`. So the other candidates are places of the player's other pending appointments (a double-booking test). The place field of an appointment slot (`unk_00`, bits 0-4) is the 0-based place id: main indexes its place name table with it. In `core` these four bytes are named `appt_place_0..3`; DATE reads them as chosen place, true place and two other places (section 14).

`date_meeting_place_check` (step 0x10):

- if the girl's `girl_state_rank` is 4 (worst mood): `date_meeting_group` = 8 and jump +0x24 to step 0x34: she does not come; the date ends with the cover wipe and `date_end` (its counting block is skipped because the appointment stays active);
- else if the chosen place equals the true place and `date_arrival` is 1, or there is no decoy (`unk_726` == 0xFF), or the decoys equal the true place: jump +0x11 to step 0x21 (no question);
- else it builds the question (it is also asked when the chosen place is right but the girl is late and decoys exist: staying then means waiting for her): the name of the other candidate in text buffer A, and of the second decoy in buffer M if there is one (`date_meeting_group` = 3 for three candidates, 0 for two).

Steps 0x11-0x13 speak the question (shared section `DATE_SHARED_MEETING_PLACE`), show a two- or three-way menu (`date_meeting_place_select`; answer 0 stays, the others move) and resolve it (`date_meeting_place_resolve`):

- chosen place == true place and answer 0 ("stay"): speak, then if the step moved set `date_player_waited` = 1, `date_arrival_group` + 1 and jump +0xC to step 0x1F;
- chosen place != true place and answer 0: jump +0x1E to step 0x31: she is not here (group 7), wait, speak, cover wipe, `date_end`;
- any other answer (move to another candidate): `date_meeting_group` += answer, `date_arrival` = 2, the greeting group is recomputed for "player late", `date_place` becomes the candidate the answer names (answer 1: the true place if the first decoy was the chosen place, else the first decoy; answer 2: the true place if the second decoy was the chosen place, else the second decoy), `dialogue_group` = 0, the event-log entry of the arrival is dropped, then steps 0x14..0x1D redo the place setup (step 0x16), the bank, the three file reads and the fade-in;
- `date_wrong_place_outcome` (step 0x1E): if `date_place` is still not the true place she is not there (group 7, jump to 0x32); otherwise she waited: syoushin +8.

Emulator: appointment place 3 (aquarium), chosen place 5 with one decoy 5: step 0x10 -> 0x11 -> 0x12 -> 0x13 (question and menu); answer 0 went to step 0x31 and the date ended at once; answer 1 went to 0x14, ran the place setup of place 3 again and reloaded, reached 0x2D with `date_arrival` 2 and then stage 1. With chosen == true place the step went 0x10 -> 0x21.

### 4.3 Arrival greeting

`date_arrival_init` (step 0xD) clears the per-date state (about twenty variables), adds `date_spot_variant` x `date_groups_per_variant` to `dialogue_group` (the karaoke box, place 11 variant 2, adds the stride once), clears the greeting, opening, film, concert and scene selectors and the park flags, and computes the greeting group:

```
tier  = (rank >= 2) + (rank >= 3)            rank = girl_state_rank(current_girl)
group = tier * (arrival == 0 ? 2 : 1) + arrival * 6 - (arrival == 2 ? 3 : 0)
```

so girl late -> 0, 2, 4; on time -> 6, 7, 8; player late -> 9, 10, 11 (`DateGreeting`). It also sets `date_first_sunday` (weekday 6 and day < 8). The greeting is spoken from `DATE_SECTION_ARRIVAL_GREETING` by `date_arrival_greeting_speak` (step 0x2D; on its third line the face becomes 3 when the player is late and the girl is in rank 2 or worse). If the girl is late (`date_arrival` 0) steps 0x2A/0x2B first speak a waiting talk before her portrait appears (`date_arrival_late_gate`); otherwise they are skipped. `date_girl_in_clear_appointment` (step 0x2C) shows the portrait and clears the active bit of `appointment_slots[0]`; that bit decides in section 5.10 whether the date counts.

`date_looks_check` (step 0x28): if `param_looks` < 30 (< 35 for Mira) the greeting group becomes 12 (bad looks), tokimeki -1, affection -1, syoushin +5, and the step jumps +0xF to 0x37: face 3, portrait in, speak, portrait out, speak, cover wipe, `date_end`. Emulator: looks 10 took 0x28 -> 0x37 -> ... -> `date_end` (the date still returns through `day_event_next`).

`date_special_day_greeting` (step 0x2F, section 24): a short extra greeting on March 20/21 (group 0), the three summer Sundays of section 2 (1), September 22/23 (2 or 3 by frame parity), December 23 (4); no greeting on other days.

### 4.4 Special event check at arrival

Step 0x29 runs `date_scene_choose`; two girls have scenes that start here instead of in stage 1 (Mira's special scene and Megumi's summer scene, section 9.2), and Miharu's spring and summer intrusions start at step 0xF.

### 4.5 `date_scene_trigger` and `date_scene_choose`

`date_scene_trigger(scene, record_flag, event_index)`: stores `dialogue_work.scene`, sets `event_flags[event_index] = 1` when `record_flag` is nonzero, stores `date_special_scene_flag_index`, sets `date_special_scene_active` and bit 1 of `scene_effect_flags` (title banner on), and calls `main_state_set(0x5E)`. `date_scene_choose` is a chain of conditions evaluated every time it is called by a step; the table of section 9.2 lists them all.

### 4.6 The street-gang ambush

`date_ambush_check` (step 0x2E, runs for every date on arrival):

- kind 4 (first ambush): `date_place_history[0] == date_place == date_place_history[1]` (the third date in a row at the same place), the place is not 0, `year >= 0x60`, `player_flags` bit 20 clear; sets `event_flags[0x55]` = 1, `player_flags` bit 19, clears the history;
- kind 5 (boss ambush): `year == 0x61` and `month == 9`, bit 20 clear, and (some club record 0-10 has club flag field 3, or bit 19 is set, or no club record 0-10 has points, in which case record 11 gets flag 3); sets `event_flags[0x55]` = 2, bit 20, clears the history;
- both set `dialogue_work.launch_code`, `daily_event_bits &= 8`, save mode 0x50, the stage and the resume step 0x3E, and jump to step 0x40: `date_ambush_prepare_battle` loads the ambush talk data (1 sector at LBA 0xAFCA to 0x801B0000), steps 0x41-0x45 load the battle bank, shake the screen and speak (`current_girl` = 14 with the short-name buffer set to a gang label), step 0x4A starts RPG_BAT (mode 0x81). The battle returns to step 0x3E (`date_ambush_after_battle`): `dialogue_work.launch_code` 0 (won): tokimeki +2 for the date girl, or in the boss case +3 for every met girl with `GirlFlags` bit 1 and +2 for the date girl; otherwise the player's stress +5; the screen is rebuilt and step 0x3F runs `date_end`. See [[spec/o-rpgbat]] for the battle.

## 5. Stage 1: the date at a place

### 5.1 The template

Each of the 21 places has its own step table (18-104 steps) chosen by `date_stage_place` from `date_place`; the tables share one skeleton. Step numbers are those of the aquarium (32 steps; emulator-confirmed order):

| step | handler | role |
|---:|---|---|
| 0x00 | `bustup_expression_mood` | face from the girl's mood |
| 0x01 | `date_scene_choose` | special scene check |
| 0x02 | `normal_date_speak` | the girl's talk on arrival at the place (group `dialogue_group`, section of the place) |
| 0x03 | `normal_date_girl_out` | portrait out |
| 0x04 | `date_<place>_enter_spot` | logs the spot in the event log (removing a duplicate entry) and starts the background read |
| 0x05-0x08 | wait, decode, `normal_date_move_place`, `place_init2` | cross-fade to the spot, sub-title |
| 0x09 | `date_scene_choose` | special scene check |
| 0x0A | `normal_date_girl_in` | portrait in |
| 0x0B | `normal_date_speak` | the conversation that ends with a question |
| 0x0C | `normal_date_three_select` | the player picks one of three answers (or two) |
| 0x0D | `date_choice_score` | score the answer (section 5.5) |
| 0x0E-0x0F | `normal_date_speak`, place extras | the girl's reaction; girl-specific extra lines |
| 0x10 | `date_choice_reaction_speak` | shared reaction section 2, line = response class of the answer |
| 0x11-0x15 | portrait out, move place, `place_init`, portrait in, `date_choice_mood_face_step` | back to the first location |
| 0x16 | `date_farewell_speak` | farewell (section 5.7) |
| 0x17-0x19 | `bustup_expression_mood`, `date_invited_day_check`, `date_invited_day_talk` | invitation-day extra (section 5.7) |
| 0x1A | `date_birthday_check` | birthday gate (section 8) |
| 0x1B | `date_closing_speak` | closing talk (section 5.7) |
| 0x1C-0x1E | portrait out, `normal_date_bg_out`, `date_end` | leave |
| 0x1F | `game_reset_to_title` | guard |

Other places insert steps: the shopping street has a shop menu before (section 5.4), the amusement park has rides, the night parade and a second answer (section 6), the sea has the island scene and special cases, the shrine the festival stalls and the fireworks (section 7), movie theater and concert hall play an audio clip (5.9).

### 5.2 The places

Spot ids are `DateSpot`; "classes" are the tables of the choice file (rows per girl, bytes). Months are 1-12; the school year is `year - 0x5F` (April-March, see section 8).

| id | place | spots (log ids) | variant rule | groups/variant | notes |
|---:|---|---|---|---:|---|
| 0 | neighborhood_park | 0x21 | single | 2 | two-way answer; the opening talk is chosen by `date_neighborhood_park_pick_greeting` (section 5.8); tokimeki +1, affection +1 at once |
| 1 | central_park | 1 (0x25 path, 0x26 pond, 0x27 blossom path) | 0 path on even visits, 1 pond on odd ones; 2 blossom path from March 15 to April 30 unless Shiori's or Saki's spring scene is due (then 0); a due Yumi summer scene forces the pond; the falling-particle layer runs on variant 2 and on variant 0 in autumn | 3 | classes 9 bytes per girl |
| 2 | shopping_street | 2 boutique, 3 fancy shop, 4 junk shop | the shop is the player's first answer (menu at step 2); variant = parity of the shop's counter | 0; the shop sets a base group (boutique from 0, fancy shop 4-10, junk shop 8-14 by girl) plus 2 per variant (6 for Shiori and Mira in the boutique) | classes 18 bytes per girl (3 shops x 2 variants x 3); music 0x200-0x202 by shop |
| 3 | aquarium | 5, 6 | 0/1 by parity of counter[5]; 2 = dolphin show once, from March 1996 | 3 | |
| 4 | zoo | 7, 8 | 0/1 by parity of counter[7]; special variant 2 once from December 1995; Mira cycles 3 variants by counter % 3 | 3 | Mio has fixed summer groups |
| 5 | botanical_garden | 9 | parity | 3 (Mira 5) | |
| 6 | planetarium | 0xA | option 0/1 by parity | 7 | science < 120 lowers the classes of option 1 for most girls |
| 7 | art_museum | 0xB hall, 0xC / 0xD / 0xE exhibitions | option 0 hall (first visit), 1 hall (repeat), 2 artist exhibition (autumn 1997), 3 sculpture (spring 1997), 4 painters (autumn 1995), 5 exhibition already seen | 3 | 15 bytes per girl |
| 8 | library | 0xF | parity | 3 | |
| 9 | game_center | 0x10 video, 0x11 medals | parity; the medal corner is a second answer (variants 2/3) and opens from September 1996 | 0 (set by hand) | 12 bytes per girl |
| 10 | bowling_alley | 0x12 | parity | 3 | |
| 11 | karaoke | 0x13 | counter % 3: 0 and 1 as variants 0 and 1, 2 hands over to DATE2 (the karaoke box, `main_state_set(0x5C)` at step 9) | 3 | |
| 12 | amusement_park | 0x14 hero show, 0x15-0x19 rides, 0x1A night parade | see section 6 | 0 | 104 steps |
| 13 | stadium | 0x1B, 0x1C | 0/1 by parity; 2/3 (spot 0x1C) in months 1, 3, 6, 11 | 3 | |
| 14 | movie_theater | 0x23 | film index 0-10 by date (section 5.9) | 0 | audio clip per film |
| 15 | concert_hall | 0x24 | programme index 0-11 by date | 0 | audio clip per programme |
| 16 | pool | 0x1D | parity | 3 | athletics < 40 check (5.6) |
| 17 | sea | 0x1E, 0x28 | parity; Nozomi with athletics >= 150 goes to the deserted island route (spot 0x28, own menu and score, steps 0x2D-0x4A) | 3 | |
| 18 | shrine | 0x22 | by school year: festival stalls (1995, 1996) or fireworks (1997) | 5 | section 7 |
| 19 | skating_rink | 0x1F | parity | 3 | athletics < 40 check |
| 20 | ski_resort | 0x20 | parity (Yukari forced to 1 after any visit) | 3 | athletics < 100 check |

### 5.3 Place setup and the variant rule

The setup step of a place (`date_place_setup_<place>`, step 9 of stage 0) points the section triple (`dialogue_text_table` etc.) at the place's section of the girl's script, fills the place title and the sub-spot title (text buffers `scene_place_title` and `_2`), sets `date_groups_per_variant`, chooses the variant, sets `date_spot`, copies three class bytes and three face bytes from the choice file rows `[girl][variant][answer]` into `date_choice_class` and `date_choice_face`, starts the background read of the place (the picture id is a constant per place) and appends the arrival to the event log (`event_log_add`).

Variant rule: the girl's visit counter for the spot, `GirlRecord.event_counters[spot]`, is incremented once at the end of every counted date (section 5.10). Most places use its low bit: even counter -> variant 0, odd -> variant 1, so two successive dates at the same place with the same girl show different scenes. Seasonal and one-off variants override it (table above). The dialogue group offset is `variant x date_groups_per_variant`.

`dialogue_group` is a live cursor: each `normal_date_speak` plays the current group of the place section and `dialogue_run` advances the cursor when the group ends. A visit with variant `v` therefore starts at group `v x stride` and consumes the next groups in order: the arrival talk, the conversation that ends with the question, the reaction to the answer (emulator, aquarium variant 0: the cursor was 0, 1, 2 at steps 2, 0xB and 0xE). Place-specific steps add mood tiers or answer indices to the cursor to jump to the mood or answer variants of a group, and set it by hand where the stride is 0. Emulator: the aquarium counter was 0 before the run (variant 0) and 1 after; `girl_last_date_spot` was set to 5.

### 5.4 The answer

`normal_date_three_select` (main) shows the three answers in a shuffled order (`dialogue_choice_order`) and stores the original index in `dialogue_choice`. All tables are indexed by the original index. The neighborhood park and the girl-specific special cases use a two-way menu. The shopping street first runs a three-way menu to pick the shop (step 2; `normal_date_speak_012` keeps its order 0, 1, 2 unshuffled; emulator: step 1 speak, step 2 menu, step 8 `date_shopping_street_enter_shop`), then, after the shop talk, the scored answer menu (step 0x18). The answers themselves are script text; DATE only knows their class and face.

### 5.5 Scoring (`date_choice_score`)

For the picked answer `a = dialogue_choice` and `r = date_choice_class[a]` (0 best .. 4 worst; `r` is used as it is, so class 4 weighs tokimeki -1 and affection -2):

```
face(date_choice_face[a])
tokimeki  += ((3 - r) * zatu * basho << 16) / 16        zatu, basho from date_score_multipliers(3 - r)
affection += ((2 - r) * zatu * basho << 16) / 16        multipliers from (2 - r)
syoushin  += (W[r]  * zatu * basho << 16) / 16          W = {-10, -8, -6, 8, 10}, multipliers from (2 - r)
check_para_limit()
```

All three are 16.16 words, so the effect on the integer part is `n * zatu * basho / 16` (fractions accumulate). `date_score_multipliers(n)`:

- trivia tier: for `n > 0`, `zatu = 5 - i` where `i` is the first of 0..4 with `param_trivia >= 100 - 25 i` (so 5 at trivia 100 or more, 4 at 75, 3 at 50, 2 at 25, 1 below), and 0 when trivia is 0. For `n <= 0`, `zatu = i + 3` (3 at 100 or more, 4, 5, 6, 7 below 25), and 8 when trivia is 0: a bad answer hurts more with low trivia;
- place affinity `v = date_place_affinity[girl][place]` (0 = she loves the place .. 4): `basho` = 5, 4, 4, 3, 2 for `n > 0` and 3, 4, 4, 5, 6 for `n <= 0`.

Worked example (checked in the emulator): girl 0, aquarium (affinity row value 1), trivia 60, answer class 1: tokimeki weight n = 2, zatu = 3, basho = 4 -> +24/16 = +1.5 (the run showed 0 -> 1.5); affection n = 1 -> +0.75 (50 -> 50.75); syoushin weight -8 with n = 1 -> -6, clamped at 0. The best case (class 0, trivia >= 100, loved place) is +3 x 5 x 5 / 16 = +4.7 tokimeki and +3.1 affection per answer.

`check_para_limit` clamps tokimeki and affection of girls 0-10 to 0..0x80 and syoushin to 0..0xFF. The face of the answer (`date_choice_face`) shows during the reaction; `date_choice_mood_face` shows later at the farewell.

### 5.6 Girl- and place-specific adjustments

About 70 small handlers inside the place tables change the group, the face, or add a line for one girl at one place and variant ("extra talk", "extra face" in the names). They are not generated by a rule; the port should take them from the functions. The checks with numeric conditions:

- `param_looks` threshold 30 (35 for Mira) at arrival (section 4.3) and again at the end (5.10);
- pool, skating rink and ski resort (`date_choice_special_reactions`; the sea has no such check): the first answer is forced to class 3 (POOR) with face 3 and a special reaction line when `param_athletics` is below a threshold. Pool (place 16): athletics < 40 (0x28), variant 1, Shiori, Mio, Ayako, Saki, Yukari, Yuko or Megumi, reaction line 5. Skating rink (place 19): athletics < 40, variant 0, Shiori, Mio, Ayako, Nozomi or Mira, line 6. Ski resort (place 20): athletics < 100, variant 0, Shiori, Saki, Nozomi, Mira, Yuko or Yumi, line 6;
- sea, Nozomi: athletics >= 150 goes to the deserted island route (step 0x2D), otherwise her answers 0 and 2 are class 2 with face 3;
- planetarium: science < 120 (0x78): for option 1 the first classes become 4 or 3 by girl, and the group gets +2;
- game center, medal corner: opens from September 1996;
- art museum: option 5 (exhibition already seen) scores at once and skips nine steps;
- Mira, Shiori, Yuko and others have extra lines in the library, zoo, game center and karaoke by variant and answer;
- several steps (movie theater, shrine, amusement park, Saki and Mio scenes) set `current_girl` to 14 for a line and then back to the date girl (the library branch of Yumi, the planetarium talk of Yumi, the Yuina talk in the amusement park, the movie theater, the ambush talk of section 4.6, and the Mio winter, Megumi spring (its setup and one speak step) and Saki winter scenes). HYPOTHESIS: id 14 is a narration speaker. What the code shows: `core` names id 14 `GIRL_SOTOI` (name-table entry 14), the mouth lock is set for these lines (so the portrait's mouth does not move), and the gang ambush (section 4.6) uses the same id with a gang label in the short-name buffer, so 14 is at least a generic extra speaker. All table names that say `narration` carry the HYPOTHESIS mark.

### 5.7 Farewell, invitation day and closing

`date_farewell_speak` (step 0x16 of the aquarium) plays section 22. The group is `3 x mood tier + date_choice_tier[a]` where the mood tier is `(rank >= 2) + (rank >= 3) + (rank >= 4)` (0..3), so the farewell has four tiers of three lines (rank 4 also forces face 4 and choice tier 0). Overrides: group 0xA after an appointment (`appointment_flag` set), 0x12 for Nozomi in rank 0-1 after an appointment, and 0xB (0x13 for Nozomi in rank 0-1) when the visit repeats the girl's last date spot: `date_spot == girl_last_date_spot[girl]` with `girl_last_date_spot_timer != 0`, the mode is 0x50, `date_parade_offered` is 0, there is no appointment flag, and the place is neither 0 nor 18. That branch also applies the repeat-spot penalty on its first frame: face 3, tokimeki -1, affection -2, syoushin +5. In `core` the spot memory is `girl_last_date_spot[girl]` with a 12-week timer lowered by `last_date_spot_timer_dec` (spec core, 5.4). Mio and Yukari in rank 0-1 with line 3 get face 1.

`date_invited_day_check` (step 0x18): on 1995-10-15 and on 1996-10-13, if the girl's flag bit 12 (1995) or 13 (1996) is set, both flags are cleared and the extra talk of step 0x19 (`date_invited_day_talk`, section 22 with group 3 x tier) is played unless her mood is rank 4. HYPOTHESIS: the two flags are festival invitations that a date on that day replaces.

`date_closing_speak` (step 0x1B): section 26, group = mood tier 0..3 (`(rank >= 2) + (rank >= 3) + (rank >= 4)`).

### 5.8 Neighborhood park opening

The park's flow is short: `date_neighborhood_park_start` sets the group from the mood tier (2 x tier), tokimeki +1, affection +1; a two-way question (step 3) selects the opening talk (`date_neighborhood_park_pick_greeting`, step 4): for the first answer: group 4 (once, when the girl first reaches rank 0 and h-rank 0: flag bit 5 is set), else 2 (her birthday is within a month, `date_birthday_near`; line = school year), else 3 (a 50% roll from March 1997, `date_hint_roll`); in all other cases group = the answer (0 or 1) with a random line modulo `date_opening_count_a/b[girl]`.

### 5.9 Movie theater, concert hall, karaoke

Film index (`date_movie_index`): 1995: Jun-Aug 0, Sep-Nov 1, else 2; 1996: Jan-Feb 2, Mar-May 3, Jun-Aug 4, Sep-Nov 5, Dec 6; 1997: Jan-Feb 6, Mar-May 7, Jun-Aug 8, Sep-Nov 9, Dec 10; later 10. Concert programme (`date_concert_index`): 1995: Apr-May 0, Jun-Aug 1, Sep-Nov 2, else 3; 1996: Jan-Feb 3, Mar-May 4, Jun-Aug 5, Sep-Nov 6, Dec 7; 1997: Jan-Feb 7, Mar-May 8, Jun-Aug 9, Sep-Nov 10, Dec 11; later 11. Each index selects a poster/stage background, an audio clip (tables `date_movie_audio_pairs` and `date_concert_audio_pairs`: 11 and 12 pairs of request words) and the group of the reaction section (`DATE_SHARED_FILM_REACTION`, `DATE_SHARED_CONCERT_REACTION`); the reaction plays 128 frames after the clip starts. Karaoke variant 2 (every third visit: counter % 3 == 2; also forced when the main flag `D_800B3CFC` is 1) calls `main_state_set(0x5C)` (DATE2, the karaoke box; section 14 on the `core` mode name) from `date_scene_choose` at step 9.

### 5.10 `date_end`

`date_end` (the last step of stage 0, 1, 2 and 3) is the bookkeeping of a date:

1. loads the main data (`func_800674B0`) and waits for the sound bank (retry after 0x400 frames);
2. only if the appointment slot's active bit is clear (it was cleared when the girl appeared, section 4.3; a refused or failed meeting leaves it set and skips the whole block):
   - if `date_spot > 0` and `date_special_scene_active` is 0: `event_counters[date_spot] += 1`;
   - if `param_looks >= 35`, or `>= 30` and the girl is not Mira: `girl_last_date_spot[g] = date_spot` (spot 8 is stored as 7, spot 6 as 5) and `girl_last_date_spot_timer[g] = 12`;
   - under the same looks test: place 18 (shrine) gives tokimeki +1; on May 4-5 with the girl's flag 4 set tokimeki +2; if `dialogue_work.launch_code` is 0 and `appt_place_1` equals the place: syoushin -40, and on May 3-4 flag 4 is set (the Golden Week mark: a date on May 3 or 4 earns +2 for a date on May 4 or 5);
3. `check_para_limit`; `appt_place_0 = date_place`; shifts `date_place_history` (3 <- 2 <- 1 <- 0 <- place); clears `scene_effect_flags`; sound command 0xCF; `day_event_next(1)`.

Emulator: after the aquarium run, counter[5] = 1, `girl_last_date_spot[0]` = 5, its timer 12, `date_place_history[0]` = 3.

## 6. The amusement park

Place 12 has 104 steps and two answer menus. `date_place_setup_amusement_park` sets `date_park_new_rides` (from March 1997) and `date_park_newest_rides` (from December 1997) and logs spot 0x14.

1. Hero show (first Sunday of a month only): steps 0x0B-0x1D. The girl proposes the show with a yes/no question (step 0x0C) and asks again (0x11). Answer "no" at either question sets the ride-choice group by girl (9; Mira 10; Megumi 8; Saki, Yuko, Yukari, Yumi 7) and jumps to step 0x1F; "yes" continues into the show (`date_amusement_park_enter_hero_show`, spot 0x14, music 0x207) and sets `date_show_taken` for some girls. On any other day `date_amusement_park_skip_show` (step 0x0A) sets that group and jumps directly to step 0x1E. Megumi's spring scene starts at step 0x1B inside the show path.
2. Ride choice (menu at step 0x22, not scored): the first answer is the roller coaster (the scream machine from March 1997), the second the ghost house (the virtual ship from December 1997), the third the Ferris wheel; `date_amusement_park_choose_ride` (step 0x25) adds 0, 5 or 10 to the dialogue group, sets `date_ride_index` (`DateRide`), the ride's title and spot (0x15-0x19) and its class rows (offset 0, 6, 12, 18, 24 in the 36-byte girl row; a sixth offset 30 serves the extra row of `date_amusement_park_ride_extra_tables`).
3. The ride (step 0x30, `date_ride_dispatch`): the ride function runs its animation and reloads the portrait (steps 0x30/0x31 alternate; emulator: roller coaster, about 790 frames); the roller coaster, ghost house, Ferris wheel, scream machine and the 3D virtual ship are separate functions (`date_ride_*`, `date_virtual_ship_*`).
4. After the ride: a talk (group += 2 x variant), the second menu (step 0x36) with the ride's class row, `date_choice_score`, the reaction; in August the girl then offers the night parade (`date_amusement_park_parade_offer`, step 0x3B): accept goes to the parade scenes (spot 0x1A, music 0x204, neon sign with palette cycling), decline restores the saved choice and jumps +0x10 (emulator: outside August the jump went to 0x4C); then the farewell tail at the main area (steps 0x4D-0x5C) and `date_end`. The night-parade talk (script section 9, `DATE_SECTION_AMUSEMENT_PARK_PARADE`, group and line cursors `date_park_parade_group` and `date_park_parade_line`) is spoken at steps 0x3D, 0x48 and 0x4A; the parade steps advance its group (+1 at step 0x3F when the girl has visited spot 0x1A before, +1 at step 0x49 when she has not). The text of the section was not read, so "talk about the parade" is the reading of those step positions.

Girls have ride-specific groups and special classes (Mio on the scream machine, Mira in the ghost house, Yuko on the Ferris wheel, Saki on the scream machine); three special scenes run a ride function from their own tables: Shiori's winter scene (Ferris wheel), Mio's spring scene (scream machine) and Mira's autumn scene (ghost house).

## 7. The shrine festival

Place 18 (steps 0-0x49, spot 0x22). After a two-way question (step 2) and the opening talk, `date_shrine_festival_branch` (step 8) splits by school year:

- 1995: group 1 for Yuina, Yukari, Yuko and Yumi (0 for the others), tokimeki +1, then the festival stalls at step 0xB;
- 1996: group 4 (6 for the others), jump to step 0x14, the festival stalls at step 0x16;
- 1997: group 0xA (0xC for the others), tokimeki +1, jump to step 0x1F: the fireworks scene, the only variant with a scored answer (menu at step 0x29; `date_shrine_choice_tables_by_girl` copies 3 class bytes and 3 face bytes per girl from the table `date_class_table_shrine`).

The stalls are the EN_NICHI overlay: `date_shrine_start_festival_stalls` saves mode, stage and the resume step 0x3A and starts mode 0x82; `festival_stall_result` (0-4) comes back through the resume steps 0x3A-0x48 (reset of the screen, portrait reload, `date_shrine_result_face`), and `date_shrine_after_stalls_jump` (step 0x48) goes to step 0xC in 1995 (talk, then `date_shrine_jump_to_farewell`) and to step 0x17 in 1996: `date_shrine_stall_reward` adds `festival_stall_result` to the group and applies result 0: tokimeki +1, affection +1, stress -10, face 1; 1: affection +1; 2: tokimeki -1; 3: tokimeki +5; 4: none (faces by result, see `date_shrine_result_face`). The fireworks scene (`date_shrine_fireworks_setup`) loads 0x86 sectors, sets up ten firework sprites and the sky tiles and runs `date_shrine_fireworks_update` every frame until `date_shrine_fireworks_fade` fades it out; the date then continues in the shared farewell tail (steps 0x2F-0x39). Several girls have a girl-specific portrait file loaded for the shrine and for the ski resort.

## 8. Birthdays

`date_birthday_check` (step 0x1A of the aquarium; every place table has it just before the closing talk): if today equals the girl's `birthday` (month bits 0-3, day bits 4-8): `main_state_set(0x50)` and `main_stage_set(2)`; else if it equals the player's birthday: stage 3; else continue.

Stage 2, the girl's birthday: `date_gift_start` needs her flag bit 6 (birthday known), else it jumps to step 0x13. It sets `daily_event_bits` bit 1 (Shiori) or 2 (others), logs scene 0xA, loads her gift section triple (`date_gift_sections_init`, a 0x16-sector file at LBA 0xAF0D) and computes the school year index `date_school_year` (0-2: April-March). The three gift names for the answer menu come from `date_gift_names`: 33-byte records, index `girl x 9 + school_year x 3 + answer` (93 of 99 slots are filled; the six empty ones are all three gifts of Megumi and of Yumi in the first school year, when they have no gift scene). The menu returns `dialogue_choice` (original index, 0 the best gift) and `date_gift_apply_reward` gives:

| gift answer | tokimeki | affection | syoushin | face |
|---|---:|---:|---:|---:|
| 0 | +3 | +2 | -15 | 1 |
| 1 | +2 | +1 | -10 | 2 |
| 2 | 0 | +1 | +10 | 3 |

(integer points, then `check_para_limit`). The reply group is `2 x answer + 13`, the mood group of the reaction is 2/4/6/8 by rank. After the closing talk `date_end` runs; if it is also the player's birthday, `date_gift_player_birthday_check` first starts stage 3. Emulator: with the girl's birthday poked to today, stage 2 started after the invitation-day check and reached the reward step.

Stage 3, the player's birthday: `date_player_birthday_start`: a girl in rank 2 or worse skips the greeting (jump to step 9), otherwise sets `daily_event_bits` bit 4 and logs scene 0xD; the greeting is 3 sectors at LBA 0xAF43 (`date_player_birthday_load_data`), group 1 of the girl's table (`date_player_birthday_pick_sections`), no reward is applied; then the closing talk and `date_end`. Emulator: with the player's birthday poked to today stage 3 started after the check.

## 9. Special date events (mode 0x5E)

### 9.1 Structure

`date_scene_choose` (every call in a place table at steps 1, 9 and some others, and at stage 0 steps 0x29 / 0xF) starts a special scene when its conditions hold (9.2). The scene replaces the rest of the date: `date_scene_trigger` sets mode 0x5E; the girl's dispatcher runs the season handler, whose table (17-67 steps) does: reset choices (`date_scene_reset_choices`), load data (`..._load_data`), the scene setup (`..._setup`: points `date_scene_text/command/voice` at the scene's sections, applies the fixed reward, sets the event still (main's `event_still` term), palettes and face/mouth animation), load the girl's scene script (`date_scene_load_script`, LBA 0xAE21-0xAE5A), play it with `date_scene_speak` (one group per step, `date_scene_group`/`date_scene_line`, main variables, requests), change backgrounds (`bg_read_sub2` steps), show the event stills with fades, and finish with the shared date tail (`date_load_girl_script`, `place_init`, portrait in, farewell talk, closing talk) and `date_scene_end`. Only five scenes contain an answer menu: Mio spring and winter, Ayako winter, Mira spring and autumn. Each scene table also holds its own sound, background and visual-effect helpers; the port can treat them as presentation.

`date_scene_end` is `date_end` without the visit counter: it loads the main data, stores the last date spot (same looks test as 5.10, spot 8 -> 7, 6 -> 5), logs the scene (`event_log_add(2, date_special_scene_flag_index)` after dropping the previous entry when its flag byte is 0 or 1; not for Mira's special scene and not for Miharu's; Mira's spring scene sets the log rank override to its palette index + 1), syoushin -40, the Golden Week bonus and flag, `check_para_limit`, `appt_place_0 = date_place`, shifts the history, clears `scene_effect_flags` and `day_event_next(1)`.

### 9.2 Trigger table

Years are the year byte (0x5F = 1995, 0x60 = 1996, 0x61 = 1997). Season = months 3-5 spring, 6-8 summer, 9-11 autumn, 12/1/2 winter. "State" means `girl_state_rank == 0` with the affection bit (>= 80) and `get_g_zyotai_h == 0` (best mood); "good" is rank < 2 and h-rank 0; "fair" is rank < 3 and h-rank < 2. "Step" is the stage 1 step of the place table at which `date_scene_choose` runs. The flag index is the `event_flags` index; "sets" means the trigger records it, "scene" means the scene sets it itself.

| girl | season (scene) | place | extra conditions | step | flag |
|---|---|---|---|---:|---:|
| shiori | spring (4) | central_park | | 0xD | 3 sets |
| shiori | summer (6) | pool | year >= 1996, fair, visited the pool before | 0xA | 5 sets |
| shiori | autumn (3) | neighborhood_park | good | 1 | 2 sets |
| shiori | winter (5) | amusement_park | state, ride choice = Ferris wheel | 0x23 | 4 sets |
| mio | spring (2) | amusement_park | ride choice = first (the scream machine), new rides open | 0x23 | 9 scene |
| mio | summer (3) | sea | | 0xA | 10 sets |
| mio | autumn (4) | library | flag 0, or flag 1 and an even frame count | 0xB | 11 sets |
| mio | winter (5) | movie_theater | film index 6 | 0x11 | 12 sets |
| yuina | spring (2) | zoo | year >= 1996 | 8 | 15 sets |
| yuina | summer (3) | bowling_alley | year 1997 | 9 | 16 sets |
| yuina | autumn (4) | game_center | year >= 1996, medal corner chosen | 0xC | 17 sets |
| yuina | winter (5) | shopping_street | December, junk shop | 0xF | 18 sets |
| ayako | spring (2) | karaoke | year 1997 | 9 | 22 sets |
| ayako | summer (3) | sea | | 0xA | 23 sets |
| ayako | autumn (4) | art_museum | artist exhibition active | 7 | 24 sets |
| ayako | winter (5) | planetarium | good, 1997 or 1998 or December 1996 | 0xB | 25 sets |
| saki | spring (2) | central_park | state | 0xD | 28 sets |
| saki | summer (3) | pool | state, visited the pool before | 0xA | 29 sets |
| saki | autumn (4) | stadium | no game day, variant < 2 | 8 | 30 sets |
| saki | winter (5) | shopping_street | fancy shop | 0xF | 31 sets |
| yukari | spring (3) | botanical_garden | year 1997 | 9 | 35 sets |
| yukari | summer (4) | aquarium | fair, year >= 1997 | 9 | 36 sets |
| yukari | autumn (5) | central_park | state | 0xD | 37 sets |
| yukari | winter (6) | ski_resort | | 0 | 38 sets |
| nozomi | spring (2) | art_museum | sculpture exhibition active | 7 | 41 sets |
| nozomi | summer (3) | sea | state, athletics >= 150 | 0xD | 42 sets |
| nozomi | autumn (4) | botanical_garden | | 9 | 43 sets |
| nozomi | winter (5) | bowling_alley | | 9 | 44 sets |
| mira | spring (2) | shopping_street | boutique | 0xF | 47 sets |
| mira | summer (3) | sea | none (repeats every visit) | 0xA | 48 sets |
| mira | autumn (4) | amusement_park | state, ride choice = second (the ghost house) | 0x23 | 49 scene |
| mira | winter (5) | ski_resort | state | 8 | 50 sets |
| mira | special (6) | any | state; stage 0 step 0x29; clears the appointment active bit | stage 0 | 51 sets |
| yuko | spring (2) | game_center | medals closed or first answer | 0xC | 55 sets |
| yuko | summer (3) | amusement_park | night parade offered, state | 0x3C | 56 sets |
| yuko | autumn (4) | concert_hall | programme 10 | 0x11 | 57 sets |
| yuko | winter (5) | skating_rink | | 8 | 58 sets |
| yuko | december (6) | library | 1995-12-10, 1996-12-08 or 1997-12-07, rank < 3, player flag 17 | 0 | 59 sets |
| megumi | spring (3) | amusement_park | first Sunday of the month | 0x1B | 62 sets |
| megumi | summer (4) | central_park | state; stage 0 step 0x29; clears the appointment active bit | stage 0 | 63 sets |
| megumi | autumn (5) | zoo | | 8 | 64 sets |
| megumi | winter (6) | skating_rink | state, athletics >= 80 | 8 | 65 sets |
| yumi | spring (3) | amusement_park | | 0 | 69 sets |
| yumi | summer (4) | central_park | state | 1 | 70 sets |
| yumi | autumn (5) | stadium | variant >= 2 (game day) | 8 | 71 sets |
| yumi | winter (6) | ski_resort | state | 0 | 72 sets |

Miharu's two scenes are intrusions into a date of any girl, tested after the per-girl blocks at stage 0 step 0xF: spring scene (3): months 3-5, `girls[11].story_stage >= 2`, `event_flags[76]` clear; summer scene (5): sea, months 6-8, `story_stage >= 2`, `event_flags[78]` clear. They save mode 0x50 / stage 0 / step + 1, set `dialogue_work.girl` = 11 and return to the date with `return_step`; no reward. Their speaker is girl 11 with the short-name buffer set to a "mysterious woman" label (a string in the code), so she is introduced as a stranger.

### 9.3 Rewards

Integer points added by the scene setup (the standard end adds syoushin -40 on top). "Answer" rows depend on the menu of the scene.

| girl | season | tokimeki | affection | syoushin | note |
|---|---|---:|---:|---:|---|
| shiori | autumn | +2 | 0 | -28 | |
| shiori | spring | +3 | +2 | -20 | |
| shiori | winter | +1 | +1 | -20 | |
| shiori | summer | +1 | +2 | -20 | |
| mio | spring | 0 | 0 | 0 | answer 0: tokimeki -2, affection -1, syoushin +20 and `event_flags[9]` set; answer 1: face 2 |
| mio | summer | +1 | +2 | -20 | |
| mio | autumn | +2 | +2 | -20 | |
| mio | winter | +3 | +2 | -20 | the answer sets Mio's flag 14 or raises the group |
| yuina | spring | +3 | +2 | -20 | |
| yuina | summer | +2 | 0 | -10 | |
| yuina | autumn | 0 | +2 | -10 | |
| yuina | winter | +3 | +2 | -20 | |
| ayako | spring | +2 | +1 | -20 | |
| ayako | summer | -1 | -1 | +20 | penalty scene |
| ayako | autumn | +2 | +1 | -20 | |
| ayako | winter | answer: +3 / +1 or +2 / +2 | | -20 | answer 0: +3, +1; answer 1: +2, +2 |
| saki | spring | +3 | +2 | -20 | also `param_condition` +10 |
| saki | summer | +3 | +2 | -20 | |
| saki | autumn | 0 | +2 | -20 | |
| saki | winter | +1 | +1 | -20 | |
| yukari | spring | +2 | +1 | -10 | |
| yukari | summer | +2 | +1 | -20 | |
| yukari | autumn | +3 | +2 | -20 | |
| yukari | winter | +1 | +1 | -20 | |
| nozomi | spring | +1 | +1 | -20 | |
| nozomi | summer | +3 | +2 | -20 | |
| nozomi | autumn | +2 | +2 | -10 | |
| nozomi | winter | rank < 2: +3 / else +1 | rank < 2: +2 / else +3 | -20 | |
| mira | spring | 0 | 0 | 0 | the three-way answer is scored with `date_choice_score` with classes 0, 4, 2 for the original answers 0, 1, 2 |
| mira | summer | +1 | +1 | -20 | |
| mira | autumn | 0 | 0 | 0 | answer 0: affection -1, syoushin +10, `event_flags[49]` set; other answers none |
| mira | winter | +3 | +2 | -20 | |
| mira | special | +3 | +2 | 0 | |
| yuko | spring | 0 | +3 | -10 | |
| yuko | summer | +3 | +2 | -20 | |
| yuko | autumn | +1 | +1 | -20 | |
| yuko | winter | +1 | +1 | -20 | |
| yuko | december | 0 | +2 | -20 | |
| megumi | spring | +1 | +1 | -20 | |
| megumi | summer | +2 | +1 | -20 | |
| megumi | autumn | +1 | +1 | -20 | |
| megumi | winter | +3 | +2 | -20 | |
| yumi | spring | +1 | +1 | -10 | |
| yumi | summer | +3 | +2 | -20 | |
| yumi | autumn | 0 | +2 | -10 | |
| yumi | winter | +2 | +1 | -20 | |

## 10. Data and disc files

Overlay data (names in the table): the step tables (72 `*_steps` rows), the 27-section pointer arrays `date_section_text/command/voice` filled by `date_fill_sections_<girl>` (indices `DateSection`: 0-21 one per place in the order art museum, bowling alley, concert hall, zoo, shrine, game center, karaoke, neighborhood park, movie theater, amusement park night parade, pool, planetarium, sea, botanical garden, shopping street, skating rink, ski resort, stadium, aquarium, library, amusement park, central park; then 22 farewell, 23 arrival greeting, 24 special-day greeting, 25 neighborhood park opening, 26 closing; Mio and Yuina lack some sections), the 4 shared sections, the 40 choice-table pointers, `date_syoushin_weight`, `date_place_affinity` (11 x 21 bytes), `date_place_names` (21 x 33-byte records), `date_gift_names` (99 x 33), the two audio pair tables and the sound bank tables of section 4.1 (`date_place_bank_third_argument`, `date_place_bank_first_data_argument`, `date_place_bank_second_data_argument`; `date_place_load_command` and `date_place_sequence_command` hold the sound commands; 21 entries each).

Disc reads (sector counts and LBAs as in the code; the files are game data and are not reproduced here):

| file | sectors | LBA | RAM | read by |
|---|---:|---|---|---|
| shared date script | 2 | 0xAF46 | 0x80197000 | `date_load_shared_script` |
| girl date script | 0x14-0x18 by girl | 0xAD15-0xADF1 | 0x80198000 | `date_load_girl_script` |
| choice file (classes, faces) | 3 | 0xAFA9 | 0x801A4000 | `date_load_choice_tables` |
| girl scene script | 3-7 by girl | 0xAE21-0xAE5A | 0x80198000 | `date_scene_load_script` |
| gift script | 0x16 | 0xAF0D | 0x801B0000 | `date_gift_load_data` |
| player-birthday script | 3 | 0xAF43 | 0x801B0000 | `date_player_birthday_load_data` |
| ambush talk | 1 | 0xAFCA | 0x801B0000 | `date_ambush_prepare_battle` |
| central park particle data | 0x22 | 0xBC13 | 0x801E0000 | `date_central_park_load_particles` |
| fireworks data | 0x86 | 0xBC35 | 0x801B0000 | `date_shrine_load_fireworks_data` |
| parade graphics | 0x1D | 0xBD41 | 0x801B0000 | `date_amusement_park_parade_prepare` |

### 10.1 Layout of the choice file

The choice file (3 sectors, 0x1800 bytes, read to 0x801A4000) holds, per place, a table of response classes indexed `[girl 0-10][variant][answer]` and, at the same offset + 0xAB0, a table of faces with the same shape. The class tables are packed in the first 0xAB0 bytes, the face tables in the next 0xAB0. `date_choice_tables_init` sets the 40 pointers; the offsets are:

| place | class table offset | face table offset | bytes per girl | class table end (11 girls) |
|---|---|---|---:|---|
| central_park | 0x000 | 0xAB0 | 9 | 0x063 |
| shopping_street | 0x064 | 0xB14 | 18 | 0x12A |
| aquarium | 0x12C | 0xBDC | 9 | 0x18F |
| zoo | 0x190 | 0xC40 | 9 | 0x1F3 |
| botanical_garden | 0x1F4 | 0xCA4 | 6 | 0x236 |
| planetarium | 0x238 | 0xCE8 | 6 | 0x27A |
| art_museum | 0x27C | 0xD2C | 15 | 0x321 |
| library | 0x324 | 0xDD4 | 6 | 0x366 |
| game_center | 0x368 | 0xE18 | 12 | 0x3EC |
| bowling_alley | 0x3EC | 0xE9C | 6 | 0x42E |
| karaoke | 0x430 | 0xEE0 | 6 | 0x472 |
| amusement_park | 0x474 | 0xF24 | 36 | 0x600 |
| stadium | 0x600 | 0x10B0 | 12 | 0x684 |
| movie_theater | 0x684 | 0x1134 | 33 | 0x7EF |
| concert_hall | 0x7F0 | 0x12A0 | 36 | 0x97C |
| pool | 0x97C | 0x142C | 6 | 0x9BE |
| sea | 0x9C0 | 0x1470 | 6 | 0xA02 |
| shrine | 0xA04 | 0x14B4 | 3 | 0xA25 |
| skating_rink | 0xA28 | 0x14D8 | 6 | 0xA6A |
| ski_resort | 0xA6C | 0x151C | 6 | 0xAAE |

Bytes per girl are three bytes per variant and answer set (central park 3 variants, shopping street 3 shops x 2 variants, amusement park 12 triples, movie theater 11 films, concert hall 12 programmes); a girl's row is `girl x bytes_per_girl`, the variant adds `3 x variant`, the answer adds its index. The neighborhood park has no table (its classes are 0).

A port needs the script text, command and voice tables of these files (extraction is the job of the dialogue tool noted in `m-dates-b`) and the class and face tables of the choice file (stride in the `date_class_table_<place>` rows).

## 11. Names and files

Function families: place setups `date_place_setup_<place>` (21), spot entries `date_<place>_enter_spot`, answer scoring (`date_choice_score`, `date_score_multipliers`), shared tail (`date_farewell_speak`, `date_closing_speak`, `date_birthday_check`, `date_end`), birthday stages (`date_gift_*`, `date_player_birthday_*`), rides (`date_ride_*`, `date_virtual_ship_*`), and the scene families `date_scene_<girl>_<season>[_setup|_data_init|_load_data|_background_<k>|_sound_<k>|...]` (385 functions and 48 step-table labels). The table has the full list with notes. Proposed files (rows of kind `file`): `place_scenes.c`, `entry.c`, `rides.c`, `virtual_ship.c`, `girl_birthday.c`, `player_birthday.c`, `scene_common.c`, `scene_<girl>.c`; header `include/ovl/date.h`.

## 12. Hardware layer coverage

Named only at module level: falling-particle layer (`date_central_park_particles_*`), fireworks (`date_shrine_fireworks_*`, `date_firework_targets`), neon sign palette cycling (`date_parade_sign_palette_cycle`), light beams and stripe effects of the scenes (`date_scene_light_beams`, `date_scene_*_effect*`, `date_gradient_quad_draw`, `date_gradient_triangle_draw`), the ride animations (`date_ride_*`, `date_virtual_ship_*`, three GTE helper functions at 0x8015A640-0x8015A980 in libgte files that are SDK code and keep their names), background and sound loads (`bg_read_sub2`, bank loads in `date_place_music_load_*`).

## 13. Verification log (emulator, 2026-10-24)

All runs start from a boot-to-title save state (the title menu) and write memory at frame 3 so that the loader enters mode 0x50 at once with chosen values; the pad presses CIRCLE for 4 frames every 40 frames from frame 300 (every 30 in the amusement park run). The write option (`--poke FRAME:ADDRESS:HEXBYTES`, bytes in memory order) exists only in a scratch copy of `tools/emu`, which is not in the repo; the addresses and values below are everything needed to repeat a run with any emulator that can write memory at a frame. Nothing from the runs (states, screenshots) is committed.

Writes at frame 3 in every run (`GameState` is at 0x800E6280; names from `core`):

| address | field | value |
|---|---|---|
| 0x800E7388 | mode (`GameState` +0x1108) | 2 (the loader) |
| 0x800E738E | overlay target mode (+0x110E) | 0x50 |
| 0x800E7389, 0x800E738A | stage, step (+0x1109, +0x110A) | 0, 0 |
| 0x800E69DD, 0x800E71DF | `date_girl`, `current_girl` | girl id (0 unless noted) |
| 0x800E69A4..0x800E69A7 | `appt_place_0..3` | place, place, 0xFF, 0xFF unless noted |
| 0x800E6392 | `param_trivia` value (s16) | 60 (bytes 3C 00) |
| 0x800E6396 | `param_looks` value (s16) | 60 (bytes 3C 00) |

Calendar writes (every run except 1-3): year byte 0x800E62BE = 0x5F (1995), month 0x800E62BF = 4, day 0x800E62C0 = 4; run 5 does not write the day.

Writes that differ by run (numbers as in the list below):

| run | extra or different writes |
|---|---|
| 1, 2, 8, 15 | place 3 (aquarium) |
| 3 | `appt_place_0` = 5, `appt_place_1` = 3, `appt_place_2` = 5, no calendar write; the answer-0 run presses only CIRCLE, the answer-1 run adds a DOWN press (3 frames) at frame 392 |
| 4 | looks 10 (bytes 0A 00) |
| 5, 11 | place 1 (central park) |
| 6 | girl's birthday word at 0x800E644C = 0x44 (April 4, `girls[0].birthday`) |
| 7 | player's birthday word at 0x800E6378 = 0x44 |
| 9 | `girl_last_date_spot[0]` (0x800E69AC) = 5 and its timer (0x800E69BC) = 12 |
| 10 | year byte 0x60 and `date_place_history[0]`, `[1]` (0x800E69A8, 0x800E69A9) = 3 |
| 12 | place 0xC (amusement park) |
| 13 | place 2 (shopping street) |
| 14 | `frame_count` (0x800E7378) written at frame 58 with 2 or 3 (item 14 has the result) |
| 16 | place 0 (neighborhood park) |
| 18 | `date_girl` and `current_girl` = 1 (Mio), place 0xC, year byte 0x61 (1997), month 4 |

1. Aquarium, girl 0, on time, right place: step timeline 0..0x10, 0x21..0x30, stage 1 steps 0..0x1E, `date_end` at frame 1910 (section 4.1 and 5.1). Place setup of place 3 ran as the replacement of step 9.
2. Same run, break at `date_choice_score` and at the next step: tokimeki 0 -> 1.5, affection 50 -> 50.75, syoushin 0 -> 0 (clamped) for class 1, trivia 60, affinity value 1 (section 5.5).
3. Wrong place (chosen 5, true 3, decoy 5): question, answer 0 -> step 0x31 and `date_end`; answer 1 (pad down) -> steps 0x14.., place setup of place 3, reloads, `date_arrival` 2, stage 1 (section 4.2).
4. Looks 10: step 0x28 -> 0x37 ... `date_end` (section 4.3).
5. Central park, April: `date_scene_trigger(4, 1, 3)` called from `date_scene_choose` at stage 1 step 0xD, mode 0x5E, `date_scene_shiori_spring` ran (section 9).
6. Girl's birthday poked to today: `date_birthday_check` -> stage 2, `date_gift_apply_reward` reached with answer 0.
7. Player's birthday poked to today: stage 3 started.
8. After the aquarium run: `event_counters[5]` = 1, `girl_last_date_spot[0]` = 5, timer 12, history[0] = 3 (section 5.10).
9. Repeat spot (aquarium with `girl_last_date_spot[0]` = 5 and timer 12 poked): on the first frame of the farewell step tokimeki 1.5 -> 0.5, affection 50.75 -> 48.75, syoushin 0 -> 5 (section 5.7).
10. Third date in a row (history 3, 3, year 1996): `date_ambush_check` jumped step 0x2E -> 0x40, steps 0x41..0x4A ran and RPG_BAT (mode 0x81) loaded at frame 1137 (section 4.6).
11. Shiori spring scene end to end (central park, April): tokimeki 0 -> 3, affection 50 -> 52, syoushin 0 (-20 and -40 clamped); `date_scene_end` ran at frame 4058 and the queue reader took over at frame 4191 (section 9).
12. Amusement park (girl 0, first answers, not a first Sunday): steps 0, 1, ..., 0x0A -> 0x1E (show skipped), ride menu 0x22, 0x25 ride choice (roller coaster), 0x30 ride, 0x36 menu, 0x37 score, 0x3A reaction, 0x3B -> 0x4C (no August parade), 0x4D..0x5C, end (section 6).
13. Shopping street (girl 0, first answers): step 1 speaks the shop question, step 2 is the unshuffled shop menu, step 8 `date_shopping_street_enter_shop`, step 0x18 is the scored menu, `date_choice_score` at 0x19, end through the shared tail (section 5.4).

14. Girl-late arrival: after the `frame_count` write of 2 `date_arrival` was 0 (so the low bits were 0 when `date_init` ran) and stage 0 ran step 0x2A (the waiting talk), a 128-frame wait at 0x2B and the portrait at 0x2C before the greeting (section 4.3); after the write of 3 it was 1.
15. Dialogue cursor: a write watch on `dialogue_group` in the aquarium run showed 0 (reset), then +1 after each finished group at steps 2, 0xB and 0xE (section 5.3).
16. Neighborhood park (girl 0): at the opening step tokimeki 0 -> 1 and affection 50 -> 51, opening group 0 with a random line (2) for the first answer (section 5.8).
17. Disc check (reading the image, nothing kept): the choice file at LBA 0xAFA9 (3 sectors) holds only the values 0-4 in the class area (0x000-0xAB0) and in the face area (0xAB0-0x1560) and is zero after 0x1560, as the layout of section 10.1 says; the gift table has six empty 33-byte records (section 8).
18. Mio spring scene (girl 1, amusement park, April 1997, first ride choice): `date_scene_trigger(2, 0, 9)` was called from `date_scene_choose` at the ride-choice step with `dialogue_choice` 0, as the trigger table says (section 9.2).

Not run: the three-way menu shuffle (main), the other rides, the hero show and the night parade, the shrine festival and fireworks, the ambush battle itself, Miharu's intrusions, and every special scene except the Shiori spring one (item 11); the Mio spring one (item 18) was only run up to its trigger.

## 14. Notes for the port and disagreements with `core`

- Model the date as four stages with explicit step lists per place; the 21 per-place tables share one template (5.1), so a data-driven engine with per-place steps (enter spot, extra talks, special steps) is enough. The 70 girl/place handlers of section 5.6 are conditions on (girl, answer, variant) that add a line, a face or a group offset.
- The scoring needs five inputs: the response class of the answer (disc table, section 10), `param_trivia`, `date_place_affinity[girl][place]` (overlay table), the girl's mood rank and the calendar. Keep the 16.16 representation of the three girl counters.
- Randomness: arrival late (1/4 by frame count), the park opening line, the September 22/23 greeting parity, the hint roll (50%), and the replay of Mio's autumn library scene when its flag is already set (even frame count). A port may use any RNG with the same probabilities.
- `core` names `unk_724..unk_727` `appt_place_0..3` ("recent appointment places"). DATE reads them as: `unk_724` the place the player chose, `unk_725` the true appointment place (`appointment_launch` stores the slot's place there), `unk_726/727` the places of the player's other pending appointments or 0xFF; `unk_728[4]` is the place history. Suggest `chosen_place`, `appointment_place`, `other_place_a/b`. No row was written for these fields because they are `core` names.
- `core` calls mode 0x5E `MODE_DATE_SCENE` ("girl-specific scenes"); it hosts 48 special date scenes (46 of the girls 0-10 and two of Miharu). The name stays; the note could say "special date scenes".
- `PLACE_SHRINE` (core): the scene at place 18 is a festival with stalls and fireworks; the name stays, the place name table says shrine.
- Mode 0x5C: `core` names 0x58 and 0x5C `MODE_AMUSEMENT_PARK` and `MODE_AMUSEMENT_PARK_2` (both DATE2), and GLOSSARY.md and the ADR describe DATE2 as the amusement park date with rides and karaoke. DATE starts mode 0x5C only from the karaoke place (place 11 variant 2, `date_scene_choose` at step 9), while its own amusement park place (12) with the rides runs inside DATE (section 6). So the `_2` name does not tell a reader that 0x5C is the karaoke box of a date. The review of this spec found ride and karaoke text in the DATE2 overlay's strings, which fits both names; that was not re-read here and no rename is proposed. Which mode launches 0x58 was not traced.
- `core` names GirlId 14 `GIRL_SOTOI` (name-table entry 14, "butler"). DATE sets `current_girl` to 14 for single lines (section 5.6); the narration reading is a HYPOTHESIS and the table names with `narration` carry the mark.
- `D_80122D44`: an earlier draft of the DATE table named it `date_light_effect_flags` with bit 1 "special scene running". The code does not show that (the loader screen gates the title banner on bit 1 and GEKO writes the word too; DATE sets bit 1 for the karaoke hand-off and Miharu's intrusions as well). `m-flow` (open question 11) and this spec now agree on `scene_effect_flags`.
- Main variables: the 18 requests of `o-date-main-requests.tsv` are `DialogueWork` words (+0x10, +0x44, +0x64, +0x7C) and plain globals in `D_800CA134..D_800CA368`; the main owner decides prefix and form (`date_` is the overlay prefix, these live in the main exe).

## 15. Open questions

1. The meeting-place mechanism (section 4.2) is read as a double-booking test: main lists the places of the player's other pending appointments as candidates and asks where the meeting place was; the code does not say how the player is meant to remember it.
2. Stage 0 steps 0x37-0x3D are reached from the looks check (confirmed by the emulator); 0x3E-0x3F are the ambush return point. No other jump into them was found.
3. The meaning of the `girl_side` toggle (`D_8015BF44/48`) at place changes (HYPOTHESIS name).
4. `D_800B3CFC` (a flag shared with DATE2, EN_NICHI and ETC) makes the karaoke place go to the DATE2 box; its writers outside DATE were not traced.
5. `D_800E7D34 |= 1` in Ayako's spring scene (album or gallery flags?) and the flags `GirlFlags` bits 12/13/14 are only seen at the DATE sites.
6. The 48 scenes were not each run; their reward rows come from the matched C. Nozomi winter and Ayako winter apply two reward variants by rank or answer.
7. The three libgte clones at 0x8015A640-0x8015A980 are SDK code (not named here).
8. Speaker 14 (section 5.6): narration voice, a hidden extra speaker, or something else; the text of the lines was not read.
9. The night-parade talk (script section 9, section 6): only the step positions and cursors were read; whether the talk is the girl's invitation, the parade itself, or both is not known.
10. Names that rest on the calendar only: `DATE_SPECIAL_DAY_EQUINOX_*` (March 20/21, September 22/23) carry HYPOTHESIS; the three late-July Sundays are named by the weekday and month only (`DATE_SPECIAL_DAY_SUMMER_SUNDAY`), the festival-day reading of section 2 is a guess.
