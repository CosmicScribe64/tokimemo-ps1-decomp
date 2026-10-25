---
type: spec
updated: 2026-10-24
unit: DATE
ticket: T-27022
sources: ["src/ovl/DATE", "include/ovl/DATE.h", "config/renames/proposals/o-date.tsv", "config/renames/proposals/o-date-main-requests.tsv", "wiki/spec/core.md", "wiki/spec/m-dates-b.md", "wiki/spec/o-rpgbat.md", "wiki/families/date-family-results.md", "wiki/emulator.md"]
---

# Overlay DATE: dates (port spec)

Phase 2 spec for [[tickets/T-27022-p2-o-date]] ([[tickets/T-27000-phase-2]]). It describes the DATE overlay as the matched C implements it. The source paths are now under `src/ovl/date/`; current names follow the applied ledgers, while the original `config/renames/proposals/o-date.tsv` is historical evidence and does not imply that every proposal row was applied. Old names are given in backticks where a reader needs the link to the current code. Facts only, in this project's words, no game text. A claim of game meaning that the code does not prove is marked HYPOTHESIS. Scope (user decision of 2026-10-24): game logic, state, flow and data tables are described fully; the PS1 hardware layer (GPU primitives, sprites, VRAM, sound driver, CD/XA streaming, MDEC) only at module level (section 12).

Prefix: `date_` (the established prefix of the overlay: the patch names `date_fill_sections_<girl>` and `date_place_names`; the two patch names with the trap word `event`, `date_event_trigger` and `date_event_choose`, become `date_scene_trigger` and `date_scene_choose` because GLOSSARY Traps maps a scripted scene with a girl to `scene`). Girls are named by given name as in `core` (`GIRL_*`), places by the `core` `Place` enum in snake case. The accepted current folder is `src/ovl/date/`; the former file/header layout table later in this spec is a historical proposal, superseded by the applied T-27074 map and current owner registry. The core names are used for shared state (`current_girl`, `date_girl`, `girls[]`, `scene_counters`, `scene_flags`, `appointment_place_*`, `day_event_next`, ...). Where this unit sees a different role in a core field, section 14 says so.

How facts were obtained: the matched C, the step-table data in the asm, the proposals of `core` and `m-dates-b` (the main scene library: `normal_date_*`, `talk_run`, `bustup_*`), and about twenty short emulator runs (section 13) from a boot-to-title save state with memory writes that enter mode 0x50 with chosen girl, place, calendar and parameter values. Those historical runs were recorded in the earlier spec work; this batch did not rerun the emulator.

Table size: 1,103 rows in `o-date.tsv` (658 `func`, 192 `data`, 4 `field`, 60 `type`, 20 `file`, 158 `const`, 11 `comment`), 23 of them marked HYPOTHESIS; 11 functions keep their names (the `date_fill_sections_<girl>` patch names, with `comment` rows); `date_event_trigger` and `date_event_choose` are renamed (`date_scene_trigger`, `date_scene_choose`), `date_shiori_dispatch` to `date_scene_shiori_dispatch` for symmetry with the other eleven girls, and the alias label `func_80132004` inside `date_choice_tables_init` (the address `date_load_choice_tables` calls) has its own row. The 18 main-exe variables that DATE names are not rows of this table: an overlay table may not rename main symbols, so they are in `config/renames/proposals/o-date-main-requests.tsv` (18 rows, 2 marked HYPOTHESIS; section 3 lists them).


Historical DATE symbol-batch record: `config/renames/proposals/o-date-apply.tsv` contained 850 DATE-scope rows (658 functions, 192 data), excluding core/main/shared symbols and all type, field, constant, file, directory and comment rows. Its independent review and Docker plan (850 rows, 25 files, zero problems) document preparation evidence only. Current symbol status follows applied ledgers and source; remaining constant, file/header and other proposal rows must be checked separately and are not implied applied. Runtime observations above are historical evidence; the reconciliation did not rerun the emulator. The final matching proof passed; final documentation rereview remains in progress.
## 1. What the overlay is

DATE is the overlay of the dating system. It hosts two main-exe modes:

| mode | name (core) | entry | what it is |
|---|---|---|---|
| 0x50 | `GAME_MODE_DATE` | `date_mode_handler` (`func_801448D4`) | a date at a place chosen by the player: meeting, conversation with one answer, reaction, farewell, optional birthday scenes |
| 0x5E | `GAME_MODE_DATE_SCENE` | `date_scene_mode_handler` (`func_8014C08C`) | one of 48 scripted special date scenes (46 of the girls 0-10 at a place and season, and two intrusions of Miharu); replaces the rest of a date |

Both are started and finished by main code, not by DATE itself:

- A date starts from `appointment_launch` (main, `src/main/80075320.c`): it copies the first appointment slot into the queue record `DAY_EVENT_APPOINTMENT` (0x80: girl, place), stores the girl in `date_girl` and `current_girl`, the "appointment flag" in `appointment_flag` (`unk_75C`) and calls `game_mode_set(0x50)`. The debug menu of OPTION also enters 0x50 and 0x5E (items 1 and 14).
- A date ends with `day_event_next(1)` in `date_end` / `date_scene_end`: the queue reader goes on with the next day event (mode 0x31, or 0x22 while the debug flag is set).
- Other overlays are launched from inside a date and return into it: RPG_BAT (mode 0x81, the street-gang ambush, section 4.6), EN_NICHI (mode 0x82, the festival stalls at the shrine, section 7) and DATE2 (mode 0x5C, the karaoke box, section 5.9). The caller state is saved in `saved_mode`, `saved_stage`, `saved_step` and restored by main.

The overlay is script-driven: the lines of every conversation are not in the overlay. A conversation is a "section" of a script file on disc (text, command and voice tables, run by the main talk engine `talk_run`, see `spec/m-dates-b` (unit m-dates-b, merged separately)); DATE decides which group and line to play, which face to show, and what the answer is worth. The tables that give the worth of the three answers (response classes) are also disc data (the choice file, section 10).

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


`date_mode_handler` runs once per frame. It calls `bustup_wink_update` (main), switches on `GameState.mode_stage` and runs the shared screen updates (`check_k_scroll`, `k_disp_inc2`, the screen frame, message window, title slide, calendar HUD, `scene_frame_update`, screen shake). Any other mode stage value goes to `game_mode_reset_to_title`.

| stage | name | handler | steps | purpose | next |
|---:|---|---|---:|---|---|
| 0 | `DATE_MODE_STAGE_ARRIVAL` | `date_mode_stage_arrival` | 76 | meeting at the place: loads, arrival, greeting, ambush check | stage 1 (`mode_stage_next` at step 0x30) or end |
| 1 | `DATE_MODE_STAGE_PLACE` | `date_mode_stage_place` | 18-104 by place | the date: conversation, answer, reaction, farewell | `date_end`, or stage 2 / stage 3 from `date_birthday_check` |
| 2 | `DATE_MODE_STAGE_GIRL_BIRTHDAY` | `date_mode_stage_girl_birthday` | 28 | gift choice on the girl's birthday | `date_end` (or stage 3 on the player's birthday too) |
| 3 | `DATE_MODE_STAGE_PLAYER_BIRTHDAY` | `date_mode_stage_player_birthday` | 15 | the girl's greeting on the player's birthday | `date_end` |

Every mode-stage handler copies its step table (an array of function pointers) to the stack and calls `table[step](0x80)` once per frame. A step function does its work and calls `mode_step_next` when it is done (the `0x80` argument is only read by `wait_sub_sub`). Several steps jump by `mode_step_set` or `step += n`; the jumps are listed with the steps. `step_timer` is advanced by the step functions that need it, not by the loop (`spec/m-dates-b`). `date_mode_stage_arrival` and `date_mode_stage_place` add the hooks described in sections 4 and 5; both also draw a "light effect" layer (`light_effect_draw`, HYPOTHESIS: glare or sparkle) while bit 0 of `scene_effect_flags` (main word `D_80122D44`, named by `m-flow`: bit 0 light effect on, bit 1 title banner on) is set. Bit 0 is set at places with sun or water (central park variant 0 in months 3-8, the pool, the sea, the ski resort, each only for some step ranges) and on the three summer Sundays 1995-07-23, 1996-07-28 and 1997-07-27 (HYPOTHESIS: the summer festival day; the same dates select group 1 of the special-day greeting).

Mode 0x5E (`date_scene_mode_handler`): while mode stage 0 step 0 shows the title, runs `bustup_wink_update`, switches on `talk_work.girl` (the girl id 0-11; other values reset to title) to the girl's dispatcher `date_scene_<girl>_dispatch`, which switches on `talk_work.scene` (2-6) to the handler of one season (`date_scene_<girl>_<season>`), a step-table runner like the mode-stage handlers. Section 9.

## 3. State

New names; GameState fields keep the `core` names. Rows marked (R) are main-exe variables that DATE names. An overlay table may not rename main symbols, so they are not rows of `o-date.tsv`: they are requests in `config/renames/proposals/o-date-main-requests.tsv` for the merge step and the main owner. Two main words that DATE uses are already named elsewhere and have no request: `date_spot` (`D_80122D08`, `TalkWork.unk_38`, `m-dates-b`) and `scene_effect_flags` (`D_80122D44`, `m-flow`).

| name | old | meaning |
|---|---|---|
| `date_place` (R) | `D_800CA2F8` (u8) | place id 0-20 (`Place`): copied from `appt_place_0` by `date_init`; changed by the meeting-place question |
| `date_spot_variant` (R) | `D_800CA2FC` (`DatePlaceView.option`) | variant of this visit (0, 1, ... 5; the art museum calls it the option), set by the place setup (section 5.3) |
| `date_spot` | `D_80122D08` (`talk_work` +0x38, `m-dates-b`) | spot id (`DateSpot`) = index of `GirlRecord.scene_counters`; stored as `girl_last_date_spot` |
| `date_arrival` | `date_arrival` | 0 girl late, 1 on time, 2 player late (section 4.2) |
| `talk_group`, `talk_line` | `D_800CA148/14C` (core) | live group and line cursor of the place conversation; advanced by `talk_run` when a group ends |
| `date_groups_per_variant` | `D_8015B658` | group stride per variant (section 5.3) |
| `date_choice_class[3]` (R) | `D_800CA22C` | response class (0 best .. 4 worst) of the three answers |
| `date_choice_face[3]` (R) | `D_800CA21C` | face the girl shows for each answer: the argument of `bustup_layout_set`, an expression index 0 (best mood) .. 4 (worst), see `spec/m-dates-b` |
| `date_choice_mood_face[3]` (R) | `D_800CA224` | face after the answer: class clamped to 1..3 (0 -> 1, 4 -> 3) |
| `date_choice_tier[3]` (R) | `D_800CA234` | class - 1 clamped to 0..2; offsets the farewell line inside a mood tier |
| `talk_choice` | `D_80122CDC` (core/`m-dates-b`) | original index (0-2) of the picked answer |
| `date_special_scene_active` / `date_special_scene_flag_index` (R) | `D_800CA364` / `D_800CA368` | a special scene replaced the date / the `scene_flags` index it set |
| `talk_work.scene`, `talk_work.girl` | `D_80122CD0/CD4` (core) | which special scene runs (mode 0x5E): scene 2-6 and girl 0-11 |
| `talk_work.launch_code` (R, field `TalkWork.unk_10`, alias label `dialogue_launch_code`) | `D_80122CE0` | RPG_BAT launch code (4 gang ambush, 5 boss ambush, set by DATE) and, after the battle, its result (0 won); `o-rpgbat` uses the same word under its `D_` name. It is word 4 of the dialogue work, which `dialogue_clear` (words 5-30) leaves alone |
| `scene_effect_flags` (`m-flow`) | `D_80122D44` | bit 0 the light effect is drawn (section 2), bit 1 the title banner is on (what the loader screen draws, `m-flow`). DATE sets bit 1 in `date_scene_trigger`, in Miharu's two intrusions (spring: bit 1; summer, at the sea: bits 0 and 1) and, with the whole word set to 2, in `date_scene_choose` just before it hands the karaoke place over to DATE2 (mode 0x5C); it also sets bit 0 for the sun and water places (section 2). `date_end` and `date_scene_end` clear the word. GEKO writes it too. Bit 1 does not mean that a special scene runs: `date_special_scene_active` does |

The other requested main variables, which the notes of `o-date.tsv` mention (`date_fireworks_active`, `date_parade_sign_visible`, `date_ambush_talk_active`, `date_scene_group`, `date_scene_line`, `date_farewell_group_base`, `date_farewell_line`, `festival_stall_result`) are in the same request table; `date_fireworks_active` (+0x44) and `date_parade_sign_visible` (+0x64) lie inside `TalkWork`, so the main owner may prefer field rows for them.

Girl and player state read or written (core names): `girls[g].tokimeki`, `.affection`, `.syoushin` (16.16 words; score updates add fixed-point deltas to the full words, not just the integer parts), `.flags` (bits `met`, 4 = Golden Week flag, 5 = reached the best mood, 6 = birthday known, 12/13 = invitation flags, 14 = a per-girl flag used by the Mio and Yukari winter scenes, `story_stage` bits 9-11), `.scene_counters[spot]`, `.birthday`; `girl_state_rank(g)` (0 best .. 4 worst) and `get_g_zyotai_h` (the "h" rank); `param_trivia`, `param_looks`, `param_athletics`, `param_science`, `param_condition`, `param_stress`; `scene_flags[]`, `day_event_done_bits`, `girl_last_date_spot[]`, `girl_last_date_spot_timer[]`, `date_place_history[4]`, `event_log`, `appointment_slots[0]` (active bit), `player_flags` (bits 17, 19, 20 and the club nibble), calendar bytes, `frame_count`.

## 4. Stage 0: meeting at the place

### 4.1 The 76-step table

`date_init` (step 0) runs `draw2d3d`, `actor_draw_enable`, sprite and actor clears, resets the text system, sets brightness 0x80, sets mask `0x08` (bit 3) in `day_event_done_bits`, copies `appointment_place_0` into `date_place`, `date_girl` into `talk_work.girl` and `current_girl`, calls `talk_names_set`, and sets `date_arrival`: 1, but 0 when `(frame_count & 3) == 0` and the girl is not Yuina (a 25% chance by frame count that she arrives late). Then step 1.

The table (the setup function in step 9 and step 0x16 is replaced at run time by the setup of the current place, section 5.3):

| step | handler | what it does |
|---:|---|---|
| 0x00 | `date_init` | init the scene (see section 4.1) |
| 0x01 | `date_place_bgm_load_start` | start the place music bank load |
| 0x02 | `date_place_bgm_load_wait` | wait for the bank, start the place sequence |
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
| 0x1A | `date_place_bgm_load_start` | place music bank load |
| 0x1B | `date_place_bgm_load_wait` | wait for the bank |
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
| 0x42 | `date_ambush_audio_bank_load_start` | bank load start (battle music) |
| 0x43 | `date_ambush_audio_bank_load_wait` | wait for the bank |
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
- any other answer (move to another candidate): `date_meeting_group` += answer, `date_arrival` = 2, the greeting group is recomputed for "player late", `date_place` becomes the candidate the answer names (answer 1: the true place if the first decoy was the chosen place, else the first decoy; answer 2: the true place if the second decoy was the chosen place, else the second decoy), `talk_group` = 0, the event-log entry of the arrival is dropped, then steps 0x14..0x1D redo the place setup (step 0x16), the bank, the three file reads and the fade-in;
- `date_wrong_place_outcome` (step 0x1E): if `date_place` is still not the true place she is not there (group 7, jump to 0x32); otherwise she waited: syoushin +8.

Emulator: appointment place 3 (aquarium), chosen place 5 with one decoy 5: step 0x10 -> 0x11 -> 0x12 -> 0x13 (question and menu); answer 0 went to step 0x31 and the date ended at once; answer 1 went to 0x14, ran the place setup of place 3 again and reloaded, reached 0x2D with `date_arrival` 2 and then stage 1. With chosen == true place the step went 0x10 -> 0x21.

### 4.3 Arrival greeting

`date_arrival_init` (step 0xD) clears the per-date state (about twenty variables), adds `date_spot_variant` x `date_groups_per_variant` to `talk_group` (the karaoke box, place 11 variant 2, adds the stride once), clears the greeting, opening, film, concert and scene selectors and the park flags, and computes the greeting group:

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

`date_scene_trigger(scene, record_flag, event_index)`: stores `talk_work.scene`, sets `scene_flags[event_index] = 1` when `record_flag` is nonzero, stores `date_special_scene_flag_index`, sets `date_special_scene_active` and bit 1 of `scene_effect_flags` (title banner on), and calls `game_mode_set(0x5E)`. `date_scene_choose` is a chain of conditions evaluated every time it is called by a step; the table of section 9.2 lists them all.

### 4.6 The street-gang ambush

`date_ambush_check` (step 0x2E, runs for every date on arrival):

- The boss test runs first and returns immediately when it fires, so it takes precedence if both kinds are eligible. Kind 5 requires `year == 0x61`, `month == 9`, player flag bit 20 clear, and (some club record 0-10 has club flag field 3, or player flag bit 19 is set, or no club record 0-10 has practice points, in which case record 11 gets flag 3); it sets `scene_flags[0x55]` = 2, player flag bit 20, clears the date-place history, and launches the boss ambush.
- Otherwise kind 4 (first ambush) requires `date_place_history[0] == date_place == date_place_history[1]` (the third date in a row at the same place), place != 0, `year >= 0x60`, and player flag bit 20 clear; it sets `scene_flags[0x55]` = 1, player flag bit 19, clears the history, and launches the first ambush.
- Both kinds set `talk_work.launch_code`, clear `day_event_done_bits` except bit 3 (`&= 8`), save mode 0x50, mode stage and resume step 0x3E, then jump to step 0x40. `date_ambush_prepare_battle` loads the ambush talk data (1 sector at LBA 0xAFCA to 0x801B0000), steps 0x41-0x45 load the battle bank, shake the screen and speak (`current_girl` = 14 with the short-name buffer set to a gang label), and step 0x4A starts RPG_BAT (mode 0x81). The battle returns to step 0x3E (`date_ambush_after_battle`): on a win, kind 4 gives +2 tokimeki to the date girl; kind 5 gives +3 to every met girl and then +2 to the date girl regardless of her met bit. On a loss, stress +5; the screen is rebuilt and step 0x3F runs `date_end`. See [[spec/o-rpgbat]] for the battle.

## 5. Stage 1: the date at a place

### 5.1 The template

Each of the 21 places has its own step table (18-104 steps) chosen by `date_mode_stage_place` from `date_place`; the tables share one skeleton. Step numbers are those of the aquarium (32 steps; emulator-confirmed order):

| step | handler | role |
|---:|---|---|
| 0x00 | `bustup_expression_mood` | face from the girl's mood |
| 0x01 | `date_scene_choose` | special scene check |
| 0x02 | `normal_date_speak` | the girl's talk on arrival at the place (group `talk_group`, section of the place) |
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
| 11 | karaoke | 0x13 | counter % 3: 0 and 1 as variants 0 and 1, 2 hands over to DATE2 (the karaoke box, `game_mode_set(0x5C)` at step 9) | 3 | |
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

The setup step of a place (`date_place_setup_<place>`, step 9 of stage 0) points the section triple (`talk_text_table` etc.) at the place's section of the girl's script, fills the place title and the sub-spot title (text buffers `scene_place_title` and `_2`), sets `date_groups_per_variant`, chooses the variant, sets `date_spot`, copies three class bytes and three face bytes from the choice file rows `[girl][variant][answer]` into `date_choice_class` and `date_choice_face`, starts the background read of the place (the picture id is a constant per place) and appends the arrival to the event log (`event_log_add`).

Variant rule: the girl's visit counter for the spot, `GirlRecord.scene_counters[spot]`, is incremented once at the end of every counted date (section 5.10). Most places use its low bit: even counter -> variant 0, odd -> variant 1, so two successive dates at the same place with the same girl show different scenes. Seasonal and one-off variants override it (table above). The talk group offset is `variant x date_groups_per_variant`.

`talk_group` is a live cursor: each `normal_date_speak` plays the current group of the place section and `talk_run` advances the cursor when the group ends. A visit with variant `v` therefore starts at group `v x stride` and consumes the next groups in order: the arrival talk, the conversation that ends with the question, the reaction to the answer (emulator, aquarium variant 0: the cursor was 0, 1, 2 at steps 2, 0xB and 0xE). Place-specific steps add mood tiers or answer indices to the cursor to jump to the mood or answer variants of a group, and set it by hand where the stride is 0. Emulator: the aquarium counter was 0 before the run (variant 0) and 1 after; `girl_last_date_spot` was set to 5.

### 5.4 The answer

`normal_date_three_select` (main) shows the three answers in a shuffled order (`talk_choice_order`) and stores the original index in `talk_choice`. All tables are indexed by the original index. The neighborhood park and the girl-specific special cases use a two-way menu. The shopping street first runs a three-way menu to pick the shop (step 2; `normal_date_speak_012` keeps its order 0, 1, 2 unshuffled; emulator: step 1 speak, step 2 menu, step 8 `date_shopping_street_enter_shop`), then, after the shop talk, the scored answer menu (step 0x18). The answers themselves are script text; DATE only knows their class and face.

### 5.5 Scoring (`date_choice_score`)

For the picked answer `a = talk_choice` and `r = date_choice_class[a]` (0 best .. 4 worst; `r` is used as it is, so class 4 weighs tokimeki -1 and affection -2):

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
- planetarium: when science < 120 (0x78) and option != 0, the scene overrides the response class and face. For most girls this overrides answer 0: class 4/face 4 for Shiori and Ayako, class 4/face 3 for Yuina, Nozomi and Yuko, and class 3/face 3 for Mio, Saki, Yukari, Megumi and Mira. Yumi instead has answer 2 overridden to class 1/face 1. The low-science branch also adds 2 to the group; other answers remain table-provided.
- game center, medal corner: opens from September 1996;
- art museum: option 5 (exhibition already seen) scores at once and skips nine steps;
- Mira, Shiori, Yuko and others have extra lines in the library, zoo, game center and karaoke by variant and answer;
- several steps (movie theater, shrine, amusement park, Saki and Mio scenes) set `current_girl` to 14 for a line and then back to the date girl (the library branch of Yumi, the planetarium talk of Yumi, the Yuina talk in the amusement park, the movie theater, the ambush talk of section 4.6, and the Mio winter, Megumi spring (its setup and one speak step) and Saki winter scenes). HYPOTHESIS: id 14 is a narration speaker. What the code shows: `core` names id 14 `GIRL_SOTOI` (name-table entry 14), the mouth lock is set for these lines (so the portrait's mouth does not move), and the gang ambush (section 4.6) uses the same id with a gang label in the short-name buffer, so 14 is at least a generic extra speaker. All table names that say `narration` carry the HYPOTHESIS mark.

### 5.7 Farewell, invitation day and closing

`date_farewell_speak` (step 0x16 of the aquarium) plays section 22. The group is `3 x mood tier + date_choice_tier[a]` where the mood tier is `(rank >= 2) + (rank >= 3) + (rank >= 4)` (0..3), so the farewell has four tiers of three lines (rank 4 also forces face 4 and choice tier 0). Overrides: group 0xA after an appointment (`appointment_flag` set), 0x12 for Nozomi in rank 0-1 after an appointment, and 0xB (0x13 for Nozomi in rank 0-1) when the visit repeats the girl's last date spot: `date_spot == girl_last_date_spot[girl]` with `girl_last_date_spot_timer != 0`, the mode is 0x50, `date_parade_offered` is 0, there is no appointment flag, and the place is neither 0 nor 18. That branch also applies the repeat-spot penalty on its first frame: face 3, tokimeki -1, affection -2, syoushin +5. In `core` the spot memory is `girl_last_date_spot[girl]` with a 12-week timer lowered by `last_date_spot_timer_dec` (spec core, 5.4). Mio and Yukari in rank 0-1 with line 3 get face 1.

`date_invited_day_check` (step 0x18): on 1995-10-15 and on 1996-10-13, if the girl's flag bit 12 (1995) or 13 (1996) is set, both flags are cleared and the extra talk of step 0x19 (`date_invited_day_talk`, section 22 with group 3 x tier) is played unless her mood is rank 4. HYPOTHESIS: the two flags are festival invitations that a date on that day replaces.

`date_closing_speak` (step 0x1B): section 26, group = mood tier 0..3 (`(rank >= 2) + (rank >= 3) + (rank >= 4)`).

### 5.8 Neighborhood park opening

The park's flow is short: `date_neighborhood_park_start` sets the group from the mood tier (2 x tier), tokimeki +1, affection +1; a two-way question (step 3) selects the opening talk (`date_neighborhood_park_pick_greeting`, step 4): for the first answer: group 4 (once, when the girl first reaches rank 0 and h-rank 0: flag bit 5 is set), else 2 (her birthday is within a month, `date_birthday_near`; line = school year), else 3 (`date_hint_roll`: a 50% roll when ((year == 0x61 and month >= 3) or year == 0x62)); in all other cases group = the answer (0 or 1) with a random line modulo `date_opening_count_a/b[girl]`.

### 5.9 Movie theater, concert hall, karaoke

Film index (`date_movie_index`): 1995: Jun-Aug 0, Sep-Nov 1, else 2; 1996: Jan-Feb 2, Mar-May 3, Jun-Aug 4, Sep-Nov 5, Dec 6; 1997: Jan-Feb 6, Mar-May 7, Jun-Aug 8, Sep-Nov 9, Dec 10; later 10. Concert programme (`date_concert_index`): 1995: Apr-May 0, Jun-Aug 1, Sep-Nov 2, else 3; 1996: Jan-Feb 3, Mar-May 4, Jun-Aug 5, Sep-Nov 6, Dec 7; 1997: Jan-Feb 7, Mar-May 8, Jun-Aug 9, Sep-Nov 10, Dec 11; later 11. Each index selects a poster/stage background, an audio clip (tables `date_movie_audio_pairs` and `date_concert_audio_pairs`: 11 and 12 pairs of request words) and the group of the reaction section (`DATE_SHARED_FILM_REACTION`, `DATE_SHARED_CONCERT_REACTION`); the reaction plays 128 frames after the clip starts. Karaoke variant 2 (every third visit: counter % 3 == 2; also forced when the main flag `D_800B3CFC` is 1) calls `game_mode_set(0x5C)` (DATE2, the karaoke box; section 14 on the `core` mode name) from `date_scene_choose` at step 9.

### 5.10 `date_end`

`date_end` (the last step of stage 0, 1, 2 and 3) is the bookkeeping of a date:

1. loads the main data (`func_800674B0`) and waits for the sound bank (retry after 0x400 frames);
2. only if the appointment slot's active bit is clear (it was cleared when the girl appeared, section 4.3; a refused or failed meeting leaves it set and skips this block):
   - if `date_spot > 0` and `date_special_scene_active` is 0: `girls[current_girl].scene_counters[date_spot] += 1`;
   - last-spot memory also requires `date_spot > 0`, then has the looks gate `param_looks >= 35`, or `>= 30` when `current_girl != GIRL_MIRA`; when both conditions pass, the value is stored in `girl_last_date_spot[current_girl]` and its timer is set to 12 (spot 8 is stored as 7, spot 6 as 5);
   - the remaining rewards use a separate looks gate: `param_looks >= 35`, or `>= 30` when `date_girl != GIRL_MIRA`. Under this gate, place 18 (shrine) gives tokimeki +1; on May 4-5 with the current girl's flag 4 set tokimeki +2; if `talk_work` (launch code) is 0 and `appointment_place_1` equals the place: syoushin -40, and on May 3-4 flag 4 is set (the Golden Week mark: a date on May 3 or 4 earns +2 for a date on May 4 or 5);
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

`date_birthday_check` (step 0x1A of the aquarium; every place table has it just before the closing talk): if today equals the girl's `birthday` (month bits 0-3, day bits 4-8): `game_mode_set(0x50)` and `mode_stage_set(2)`; else if it equals the player's birthday: stage 3; else continue.

Stage 2, the girl's birthday: `date_gift_start` needs her flag bit 6 (birthday known), else it jumps to step 0x13. It sets mask `0x01` in `day_event_done_bits` for Shiori or `0x02` for every other girl, logs scene 0xA, loads her gift section triple (`date_gift_sections_init`, a 0x16-sector file at LBA 0xAF0D) and computes the school year index `date_school_year` (0-2: April-March). The three gift names for the answer menu come from `date_gift_names`: 33-byte records, index `girl x 9 + school_year x 3 + answer` (93 of 99 slots are filled; the six empty ones are all three gifts of Megumi and of Yumi in the first school year, when they have no gift scene). The menu returns `talk_choice` (original index, 0 the best gift) and `date_gift_apply_reward` gives:

| gift answer | tokimeki | affection | syoushin | face |
|---|---:|---:|---:|---:|
| 0 | +3 | +2 | -15 | 1 |
| 1 | +2 | +1 | -10 | 2 |
| 2 | 0 | +1 | +10 | 3 |

(integer points, then `check_para_limit`). The reply group is `2 x answer + 13`, the mood group of the reaction is 2/4/6/8 by rank. After the closing talk `date_end` runs; if it is also the player's birthday, `date_gift_player_birthday_check` first starts stage 3. Emulator: with the girl's birthday poked to today, stage 2 started after the invitation-day check and reached the reward step.

Stage 3, the player's birthday: `date_player_birthday_start` always sets mask `0x08` (bit 3) in `day_event_done_bits`; a girl in rank 2 or worse then skips the greeting (jump to step 9). Otherwise it also sets mask `0x04` (bit 2) and logs scene 0xD; the greeting is 3 sectors at LBA 0xAF43 (`date_player_birthday_load_data`), group 1 of the girl's table (`date_player_birthday_pick_sections`), no reward is applied; then the closing talk and `date_end`. Emulator: with the player's birthday poked to today stage 3 started after the check.

## 9. Special date events (mode 0x5E)

### 9.1 Structure

`date_scene_choose` (every call in a place table at steps 1, 9 and some others, and at mode stage 0 steps 0x29 / 0xF) starts a special scene when its conditions hold (9.2). The scene replaces the rest of the date. date_scene_sections_init makes exactly 237 distinct script-section pointer assignments (948 bytes) across the scene scripts: `date_scene_trigger` sets mode 0x5E; the girl's dispatcher runs the season handler, whose table (17-67 steps) does: reset choices (`date_scene_reset_choices`), load data (`..._load_data`), the scene setup (`..._setup`: points `date_scene_text/command/voice` at the scene's sections, applies the fixed reward, sets the event still (main's `event_still` term), palettes and face/mouth animation), load the girl's scene script (`date_scene_load_script`, LBA 0xAE21-0xAE5A), play it with `date_scene_speak` (one group per step, `date_scene_group`/`date_scene_line`, main variables, requests), change backgrounds (`bg_read_sub2` steps), show the event stills with fades, and finish with the shared date tail (`date_load_girl_script`, `place_init`, portrait in, farewell talk, closing talk) and `date_scene_end`. Only five scenes contain an answer menu: Mio spring and winter, Ayako winter, Mira spring and autumn. The per-scene step tables use different callback arguments; for example, Yumi's winter table passes `0x3C` while most other scene tables pass `0x80`. Each scene table also holds its own sound, background and visual-effect helpers; the port can treat them as presentation.

`date_scene_end` shares the tail bookkeeping but has a different last-spot gate: when the looks threshold passes, it stores `date_spot` when it is **0 or greater** (ordinary `date_end` only stores it when `date_spot > 0`); spot 8 becomes 7 and spot 6 becomes 5. It then logs the scene (`event_log_add(2, date_special_scene_flag_index)` after dropping the previous entry when its flag byte is 0 or 1; not for Mira's special scene and not for Miharu's; Mira's spring scene sets the log rank override to its palette index + 1), applies syoushin -40 and the Golden Week bonus and flag, calls `check_para_limit`, stores the place, shifts the history, clears `scene_effect_flags` and calls `day_event_next(1)`.

### 9.2 Trigger table

Years are the year byte (0x5F = 1995, 0x60 = 1996, 0x61 = 1997). Season = months 3-5 spring, 6-8 summer, 9-11 autumn, 12/1/2 winter. "State" means `girl_state_rank == 0` with the affection bit (>= 80) and `get_g_zyotai_h == 0` (best mood); "good" is rank < 2 and h-rank 0; "fair" is rank < 3 and h-rank < 2. "Step" is the mode stage 1 step of the place table at which `date_scene_choose` runs. The flag index is the `scene_flags` index; unless noted, it must be zero for the trigger to pass. Mio autumn accepts flag 0, or flag 1 on an even `frame_count`; Yuina winter, Mira summer and Yuko December have no prior-flag gate. "Sets" means the trigger records the flag, and "scene" means the scene sets it itself.

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
| yuina | winter (5) | shopping_street | December, junk shop; no prior-flag gate | 0xF | 18 sets |
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
| mira | summer (3) | sea | no prior-flag gate (repeats every visit) | 0xA | 48 sets |
| mira | autumn (4) | amusement_park | state, ride choice = second (the ghost house) | 0x23 | 49 scene |
| mira | winter (5) | ski_resort | state | 8 | 50 sets |
| mira | special (6) | any | state; stage 0 step 0x29; clears the appointment active bit | stage 0 | 51 sets |
| yuko | spring (2) | game_center | medals closed or first answer | 0xC | 55 sets |
| yuko | summer (3) | amusement_park | night parade offered, state | 0x3C | 56 sets |
| yuko | autumn (4) | concert_hall | programme 10 | 0x11 | 57 sets |
| yuko | winter (5) | skating_rink | | 8 | 58 sets |
| yuko | december (6) | library | 1995-12-10, 1996-12-08 or 1997-12-07, rank < 3, player flag 17; no prior-flag gate | 0 | 59 sets |
| megumi | spring (3) | amusement_park | first Sunday of the month | 0x1B | 62 sets |
| megumi | summer (4) | central_park | state; stage 0 step 0x29; clears the appointment active bit | stage 0 | 63 sets |
| megumi | autumn (5) | zoo | | 8 | 64 sets |
| megumi | winter (6) | skating_rink | state, athletics >= 80 | 8 | 65 sets |
| yumi | spring (3) | amusement_park | | 0 | 69 sets |
| yumi | summer (4) | central_park | state | 1 | 70 sets |
| yumi | autumn (5) | stadium | variant >= 2 (game day) | 8 | 71 sets |
| yumi | winter (6) | ski_resort | state | 0 | 72 sets |

Miharu's two scenes are intrusions into a date of any girl, tested after the per-girl blocks at mode stage 0 step 0xF: spring scene (3): months 3-5, `girls[11].story_stage >= 2`, `scene_flags[76]` clear; summer scene (5): sea, months 6-8, `story_stage >= 2`, `scene_flags[78]` clear. They save mode 0x50 / stage 0 / step + 1, set `talk_work.girl` = 11 and return to the date with `return_step`; no reward. Their speaker is girl 11 with the short-name buffer set to a "mysterious woman" label (a string in the code), so she is introduced as a stranger.

### 9.3 Rewards

Integer points added by the scene setup (the standard end adds syoushin -40 on top). "Answer" rows depend on the menu of the scene.

| girl | season | tokimeki | affection | syoushin | note |
|---|---|---:|---:|---:|---|
| shiori | autumn | +2 | 0 | -28 | |
| shiori | spring | +3 | +2 | -20 | |
| shiori | winter | +1 | +1 | -20 | |
| shiori | summer | +1 | +2 | -20 | |
| mio | spring | 0 | 0 | 0 | answer 0: tokimeki -2, affection -1, syoushin +20 and `scene_flags[9]` set; answer 1: face 2 |
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
| mira | autumn | 0 | 0 | 0 | answer 0: affection -1, syoushin +10, `scene_flags[49]` set; other answers none |
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

Function families: place setups date_place_setup_<place> (21), spot entries date_<place>_enter_spot, answer scoring (date_choice_score, date_score_multipliers), shared tail (date_farewell_speak, date_closing_speak, date_birthday_check, date_end), birthday stages (date_gift_*, date_player_birthday_*), rides (date_ride_*, date_virtual_ship_*), and scene families date_scene_<girl>_<season>[_setup|_data_init|_load_data|_background_<k>|_bgm_<k>|_audio_command_<k>|...] (385 functions and 48 step-table labels). The table has the full list with notes. The file proposal covers 19 C objects plus the umbrella header. The mixed source object 8015A190.c contains seven Yumi continuation definitions (func_8015A190, 8015A1C8, 8015A250, 8015A288, 8015A328, 8015A350, 8015A378) before Miharu begins at func_8015A3B0; preserve the object as scene_yumi_miharu.c.

## 12. Hardware layer coverage

Named only at module level: falling-particle layer (`date_central_park_particles_*`), fireworks (`date_shrine_fireworks_*`, `date_firework_targets`), neon sign palette cycling (`date_parade_sign_palette_cycle`), light beams and stripe effects of the scenes (`date_scene_light_beams`, `date_scene_*_effect*`, `date_gradient_quad_draw`, `date_gradient_triangle_draw`), the ride animations (`date_ride_*`, `date_virtual_ship_*`, three GTE helper functions at 0x8015A640-0x8015A980 in libgte files that are SDK code and keep their names), background and sound loads (`bg_read_sub2`, bank loads in `date_place_bgm_load_*`).

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
8. After the aquarium run: `scene_counters[5]` = 1, `girl_last_date_spot[0]` = 5, timer 12, history[0] = 3 (section 5.10).
9. Repeat spot (aquarium with `girl_last_date_spot[0]` = 5 and timer 12 poked): on the first frame of the farewell step tokimeki 1.5 -> 0.5, affection 50.75 -> 48.75, syoushin 0 -> 5 (section 5.7).
10. Third date in a row (history 3, 3, year 1996): `date_ambush_check` jumped step 0x2E -> 0x40, steps 0x41..0x4A ran and RPG_BAT (mode 0x81) loaded at frame 1137 (section 4.6).
11. Shiori spring scene end to end (central park, April): tokimeki 0 -> 3, affection 50 -> 52, syoushin 0 (-20 and -40 clamped); `date_scene_end` ran at frame 4058 and the queue reader took over at frame 4191 (section 9).
12. Amusement park (girl 0, first answers, not a first Sunday): steps 0, 1, ..., 0x0A -> 0x1E (show skipped), ride menu 0x22, 0x25 ride choice (roller coaster), 0x30 ride, 0x36 menu, 0x37 score, 0x3A reaction, 0x3B -> 0x4C (no August parade), 0x4D..0x5C, end (section 6).
13. Shopping street (girl 0, first answers): step 1 speaks the shop question, step 2 is the unshuffled shop menu, step 8 `date_shopping_street_enter_shop`, step 0x18 is the scored menu, `date_choice_score` at 0x19, end through the shared tail (section 5.4).

14. Girl-late arrival: after the `frame_count` write of 2 `date_arrival` was 0 (so the low bits were 0 when `date_init` ran) and stage 0 ran step 0x2A (the waiting talk), a 128-frame wait at 0x2B and the portrait at 0x2C before the greeting (section 4.3); after the write of 3 it was 1.
15. Talk cursor: a write watch on `talk_group` in the aquarium run showed 0 (reset), then +1 after each finished group at steps 2, 0xB and 0xE (section 5.3).
16. Neighborhood park (girl 0): at the opening step tokimeki 0 -> 1 and affection 50 -> 51, opening group 0 with a random line (2) for the first answer (section 5.8).
17. Disc check (reading the image, nothing kept): the choice file at LBA 0xAFA9 (3 sectors) holds only the values 0-4 in the class area (0x000-0xAB0) and in the face area (0xAB0-0x1560) and is zero after 0x1560, as the layout of section 10.1 says; the gift table has six empty 33-byte records (section 8).
18. Mio spring scene (girl 1, amusement park, April 1997, first ride choice): `date_scene_trigger(2, 0, 9)` was called from `date_scene_choose` at the ride-choice step with `talk_choice` 0, as the trigger table says (section 9.2).

Not run: the three-way menu shuffle (main), the other rides, the hero show and the night parade, the shrine festival and fireworks, the ambush battle itself, Miharu's intrusions, and every special scene except the Shiori spring one (item 11); the Mio spring one (item 18) was only run up to its trigger.

## 14. Notes for the port and disagreements with `core`

- Model the date as four stages with explicit step lists per place; the 21 per-place tables share one template (5.1), so a data-driven engine with per-place steps (enter spot, extra talks, special steps) is enough. The 70 girl/place handlers of section 5.6 are conditions on (girl, answer, variant) that add a line, a face or a group offset.
- The scoring needs five inputs: the response class of the answer (disc table, section 10), `param_trivia`, `date_place_affinity[girl][place]` (overlay table), the girl's mood rank and the calendar. Keep the 16.16 representation of the three girl counters.
- Randomness: arrival late (1/4 by frame count), the park opening line, the September 22/23 greeting parity, the hint roll (50% after the eligible calendar gate: ((year == 0x61 and month >= 3) or year == 0x62)), and the replay of Mio's autumn library scene when its flag is already set (even frame count). A port may use any RNG with the same probabilities.
- `core` names `unk_724..unk_727` `appt_place_0..3` ("recent appointment places"). DATE reads them as: `unk_724` the place the player chose, `unk_725` the true appointment place (`appointment_launch` stores the slot's place there), `unk_726/727` the places of the player's other pending appointments or 0xFF; `unk_728[4]` is the place history. Suggest `chosen_place`, `appointment_place`, `other_place_a/b`. No row was written for these fields because they are `core` names.
- `core` calls mode 0x5E `GAME_MODE_DATE_SCENE` ("girl-specific scenes"); it hosts 48 special date scenes (46 of the girls 0-10 and two of Miharu). The name stays; the note could say "special date scenes".
- `PLACE_SHRINE` (core): the scene at place 18 is a festival with stalls and fireworks; the name stays, the place name table says shrine.
- Mode 0x5C: `core` names 0x58 and 0x5C `MODE_AMUSEMENT_PARK` and `MODE_AMUSEMENT_PARK_2` (both DATE2), and GLOSSARY.md and the ADR describe DATE2 as the amusement park date with rides and karaoke. DATE starts mode 0x5C only from the karaoke place (place 11 variant 2, `date_scene_choose` at step 9), while its own amusement park place (12) with the rides runs inside DATE (section 6). So the `_2` name does not tell a reader that 0x5C is the karaoke box of a date. The review of this spec found ride and karaoke text in the DATE2 overlay's strings, which fits both names; that was not re-read here and no rename is proposed. Which mode launches 0x58 was not traced.
- `core` names GirlId 14 `GIRL_SOTOI` (name-table entry 14, "butler"). DATE sets `current_girl` to 14 for single lines (section 5.6); the narration reading is a HYPOTHESIS and the table names with `narration` carry the mark.
- `D_80122D44`: an earlier draft of the DATE table named it `date_light_effect_flags` with bit 1 "special scene running". The code does not show that (the loader screen gates the title banner on bit 1 and GEKO writes the word too; DATE sets bit 1 for the karaoke hand-off and Miharu's intrusions as well). `m-flow` (open question 11) and this spec now agree on `scene_effect_flags`.
- Main variables: the 18 requests of `o-date-main-requests.tsv` are `TalkWork` words (+0x10, +0x44, +0x64, +0x7C) and plain globals in `D_800CA134..D_800CA368`; the main owner decides prefix and form (`date_` is the overlay prefix, these live in the main exe).

## 15. Open questions

1. The meeting-place mechanism (section 4.2) is read as a double-booking test: main lists the places of the player's other pending appointments as candidates and asks where the meeting place was; the code does not say how the player is meant to remember it.
2. Stage 0 steps 0x37-0x3D are reached from the looks check (confirmed by the emulator); 0x3E-0x3F are the ambush return point. No other jump into them was found.
3. The meaning of the `girl_side` toggle (`D_8015BF44/48`) at place changes (HYPOTHESIS name).
4. `D_800B3CFC` is a shared input-sequence latch, established by the continuation trace below. Its exact player-facing effect differs by consumer; the meaning of the input sequence itself is not inferred. Any main-symbol name remains a main-owner request.
5. `D_800E7D34 |= 1` in Ayako's spring scene (album or gallery flags?) and the flags `GirlFlags` bits 12/13/14 are only seen at the DATE sites.
6. The 48 scenes were not each run; their reward rows come from the matched C. Nozomi winter and Ayako winter apply two reward variants by rank or answer.
7. The three libgte clones at 0x8015A640-0x8015A980 are SDK code (not named here).
8. Speaker 14 (section 5.6): narration voice, a hidden extra speaker, or something else; the text of the lines was not read.
9. The night-parade talk (script section 9, section 6): only the step positions and cursors were read; whether the talk is the girl's invitation, the parade itself, or both is not known.
10. Names that rest on the calendar only: `DATE_SPECIAL_DAY_EQUINOX_*` (March 20/21, September 22/23) carry HYPOTHESIS; the three late-July Sundays are named by the weekday and month only (`DATE_SPECIAL_DAY_SUMMER_SUNDAY`), the festival-day reading of section 2 is a guess.



## 16. Remaining DATE placeholders after the function/data batch

The source/header census found 1,161 owned D labels; 192 are in the apply batch, leaving 969. The earlier 567-label body-use subset omitted 402 labels in explicit DATE resource/address initializers; all 402 are now listed below with their C source line. There are also four function-like leftovers and 107 labels referenced only by DATE assembly within the DATE overlay address range. The asm-only list is a separate overlay category, not part of the 969 source/header data labels. No data bytes are copied here.

The four function-like leftovers are func_8013272C (unused duplicate prototype for the already named date_event_choose, alias debt; include/ovl/DATE.h:69) and func_8015A640, func_8015A7E0, func_8015A980 (linked GTE trigonometry helpers called from src/ovl/DATE/8014A1B0.c:321-326; optional SDK/hardware naming).

Source/header deferral categories: 120 hardware presentation/resource, 471 aggregate/type/field/table, 63 game-state/table, 315 resource/address initializer; type/layout deferred. These categories defer naming until resource ownership or aggregate types/fields/table shape are established.

| Identifier | Deferred evidence category | Source use / initializer |
|---|---|---|
| D_8015B5C0 | hardware presentation/resource | src/ovl/DATE/80132000.c:4674: p[96].tpage_table = D_8015B5C0; |
| D_8015B5C4 | aggregate/type/field/table | src/ovl/DATE/80132000.c:4672: p[96].frame_table = D_8015B5C4; |
| D_8015B5C8 | aggregate/type/field/table | src/ovl/DATE/80132000.c:4673: p[96].sequence_table = D_8015B5C8; |
| D_8015B5CC | aggregate/type/field/table | src/ovl/DATE/80132000.c:4675: p[96].unk_14 = D_8015B5CC; |
| D_8015B5D0 | hardware presentation/resource | src/ovl/DATE/80132000.c:4660: load_palette(D_8015B5D0, i & 0xFF, 1, 0, i - 16); |
| D_8015B5D4 | hardware presentation/resource | src/ovl/DATE/80132000.c:4662: load_palette(D_8015B5D4, 14, 1, 0, 0); |
| D_8015B5D8 | hardware presentation/resource | src/ovl/DATE/80132000.c:4663: load_palette(D_8015B5D8, 13, 1, 0, 0); |
| D_8015B5DC | hardware presentation/resource | src/ovl/DATE/80132000.c:4689: scene_still_show(D_8015B5DC, D_8015B5E0, D_8015B5E4, D_8015B5E8, D_8015B5EC, D_8015B5F0); |
| D_8015B5E0 | hardware presentation/resource | src/ovl/DATE/80132000.c:4689: scene_still_show(D_8015B5DC, D_8015B5E0, D_8015B5E4, D_8015B5E8, D_8015B5EC, D_8015B5F0); |
| D_8015B5E4 | hardware presentation/resource | src/ovl/DATE/80132000.c:4689: scene_still_show(D_8015B5DC, D_8015B5E0, D_8015B5E4, D_8015B5E8, D_8015B5EC, D_8015B5F0); |
| D_8015B5E8 | hardware presentation/resource | src/ovl/DATE/80132000.c:4689: scene_still_show(D_8015B5DC, D_8015B5E0, D_8015B5E4, D_8015B5E8, D_8015B5EC, D_8015B5F0); |
| D_8015B5EC | hardware presentation/resource | src/ovl/DATE/80132000.c:4689: scene_still_show(D_8015B5DC, D_8015B5E0, D_8015B5E4, D_8015B5E8, D_8015B5EC, D_8015B5F0); |
| D_8015B5F0 | hardware presentation/resource | src/ovl/DATE/80132000.c:4689: scene_still_show(D_8015B5DC, D_8015B5E0, D_8015B5E4, D_8015B5E8, D_8015B5EC, D_8015B5F0); |
| D_8015B5F4 | aggregate/type/field/table | src/ovl/DATE/80132000.c:301: D_8015B5F4 = 0x801CA200; |
| D_8015B5F8 | aggregate/type/field/table | src/ovl/DATE/80132000.c:302: D_8015B5F8 = 0x801CE200; |
| D_8015B5FC | aggregate/type/field/table | src/ovl/DATE/80132000.c:303: D_8015B5FC = 0x801D2200; |
| D_8015B600 | aggregate/type/field/table | src/ovl/DATE/80132000.c:304: D_8015B600 = 0x801D6200; |
| D_8015B604 | aggregate/type/field/table | src/ovl/DATE/80132000.c:305: D_8015B604 = 0x801DA200; |
| D_8015B608 | aggregate/type/field/table | src/ovl/DATE/80132000.c:306: D_8015B608 = 0x801DE200; |
| D_8015B60C | hardware presentation/resource | src/ovl/DATE/80132000.c:4890: ((ActorFixed *) actor_pool)[i + 98].tpage_table = (u8 *)D_8015B60C; |
| D_8015B610 | hardware presentation/resource | src/ovl/DATE/80132000.c:4894: ((ActorFixed *) actor_pool)[i + 98].tpage_table = (u8 *)D_8015B610; |
| D_8015B614 | hardware presentation/resource | src/ovl/DATE/80132000.c:4888: ((ActorFixed *) actor_pool)[i + 98].frame_table = (u8 *)D_8015B614; |
| D_8015B618 | hardware presentation/resource | src/ovl/DATE/80132000.c:4892: ((ActorFixed *) actor_pool)[i + 98].frame_table = (u8 *)D_8015B618; |
| D_8015B61C | hardware presentation/resource | src/ovl/DATE/80132000.c:4889: ((ActorFixed *) actor_pool)[i + 98].sequence_table = (u8 *)D_8015B61C; |
| D_8015B620 | hardware presentation/resource | src/ovl/DATE/80132000.c:4893: ((ActorFixed *) actor_pool)[i + 98].sequence_table = (u8 *)D_8015B620; |
| D_8015B624 | aggregate/type/field/table | src/ovl/DATE/80132000.c:316: D_8015B624 = *(s16 *)0x801F0D14; |
| D_8015B628 | aggregate/type/field/table | src/ovl/DATE/80132000.c:317: D_8015B628 = *(s16 *)0x801F0D18; |
| D_8015B62C | hardware presentation/resource | src/ovl/DATE/80132000.c:4877: load_palette(D_8015B62C, 0x12, 1, (s32)two, 0); |
| D_8015B630 | hardware presentation/resource | src/ovl/DATE/80132000.c:4879: load_palette(D_8015B630, 0x12, 1, (s32)two, 0); |
| D_8015B634 | aggregate/type/field/table | src/ovl/DATE/80132000.c:2590: D_80120684 = D_8015B634; |
| D_8015B638 | aggregate/type/field/table | src/ovl/DATE/80132000.c:2588: D_8012065C = D_8015B638; |
| D_8015B63C | aggregate/type/field/table | src/ovl/DATE/80132000.c:2589: D_80120660 = D_8015B63C; |
| D_8015B640 | aggregate/type/field/table | src/ovl/DATE/80132000.c:2591: D_80120664 = D_8015B640; |
| D_8015B644 | hardware presentation/resource | src/ovl/DATE/80132000.c:2582: load_palette((s32) D_8015B644, 0x11, 1, 3, 0); |
| D_8015B648 | game-state/table | src/ovl/DATE/80132000.c:3610: talk_text_table = (s32)D_8015B648; |
| D_8015B64C | game-state/table | src/ovl/DATE/80132000.c:3611: talk_command_table = (s32)D_8015B64C; |
| D_8015B650 | game-state/table | src/ovl/DATE/80132000.c:3612: talk_voice_table = (s32)D_8015B650; |
| D_8015BD34 | aggregate/type/field/table | src/ovl/DATE/80132000.c:4769: D_8015BD34 += 1; |
| D_8015BD38 | hardware presentation/resource | src/ovl/DATE/80132000.c:4773: ((ActorFixed *)actor_pool)[D_8015BD38 + 95].sequence_slot = 3; |
| D_8015BD3C | hardware presentation/resource | src/ovl/DATE/80132000.c:4717: ((ActorFixed *)actor_pool)[31].unk_07 = D_8015BD3C; |
| D_8015BD94 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3269: D_800CA13C = D_8015BD94; |
| D_8015BD98 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3054: D_800CA13C = D_8015BD98; |
| D_8015BD9C | aggregate/type/field/table | src/ovl/DATE/80132000.c:3202: D_800CA13C = D_8015BD9C; |
| D_8015BDA4 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3270: D_800CA140 = D_8015BDA4; |
| D_8015BDA8 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3055: D_800CA140 = D_8015BDA8; |
| D_8015BDAC | aggregate/type/field/table | src/ovl/DATE/80132000.c:3203: D_800CA140 = D_8015BDAC; |
| D_8015BDB4 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3271: D_800CA144 = D_8015BDB4; |
| D_8015BDB8 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3057: D_800CA144 = D_8015BDB8; |
| D_8015BDBC | aggregate/type/field/table | src/ovl/DATE/80132000.c:3204: D_800CA144 = D_8015BDBC; |
| D_8015BDC4 | game-state/table | src/ovl/DATE/80132000.c:1092: talk_text_table = D_8015BDC4; |
| D_8015BDC8 | game-state/table | src/ovl/DATE/80132000.c:1358: talk_text_table = D_8015BDC8; |
| D_8015BDCC | game-state/table | src/ovl/DATE/80132000.c:772: talk_text_table = D_8015BDCC; |
| D_8015BDD0 | game-state/table | src/ovl/DATE/80132000.c:1485: talk_text_table = D_8015BDD0; |
| D_8015BDD4 | game-state/table | src/ovl/DATE/80132000.c:1056: talk_text_table = D_8015BDD4; |
| D_8015BDD8 | game-state/table | src/ovl/DATE/80132000.c:1123: talk_text_table = D_8015BDD8; |
| D_8015BDDC | game-state/table | src/ovl/DATE/80132000.c:658: talk_text_table = D_8015BDDC; |
| D_8015BDE0 | game-state/table | src/ovl/DATE/80132000.c:1248: talk_text_table = D_8015BDE0; |
| D_8015BDE4 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3259: D_800CA13C = D_8015BDE4; |
| D_8015BDE8 | game-state/table | src/ovl/DATE/80132000.c:1423: talk_text_table = D_8015BDE8; |
| D_8015BDEC | game-state/table | src/ovl/DATE/80132000.c:853: talk_text_table = D_8015BDEC; |
| D_8015BDF0 | game-state/table | src/ovl/DATE/80132000.c:1453: talk_text_table = D_8015BDF0; |
| D_8015BDF4 | game-state/table | src/ovl/DATE/80132000.c:820: talk_text_table = D_8015BDF4; |
| D_8015BDF8 | game-state/table | src/ovl/DATE/80132000.c:1177: talk_text_table = D_8015BDF8; |
| D_8015BDFC | game-state/table | src/ovl/DATE/80132000.c:1509: talk_text_table = D_8015BDFC; |
| D_8015BE00 | game-state/table | src/ovl/DATE/80132000.c:1540: talk_text_table = D_8015BE00; |
| D_8015BE04 | game-state/table | src/ovl/DATE/80132000.c:1202: talk_text_table = D_8015BE04; |
| D_8015BE08 | game-state/table | src/ovl/DATE/80132000.c:735: talk_text_table = D_8015BE08; |
| D_8015BE0C | game-state/table | src/ovl/DATE/80132000.c:1016: talk_text_table = D_8015BE0C; |
| D_8015BE10 | game-state/table | src/ovl/DATE/80132000.c:1156: talk_text_table = D_8015BE10; |
| D_8015BE14 | game-state/table | src/ovl/DATE/80132000.c:683: talk_text_table = D_8015BE14; |
| D_8015BE18 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3158: D_800CA13C = D_8015BE18; |
| D_8015BE1C | aggregate/type/field/table | src/ovl/DATE/80132000.c:2991: D_800CA13C = D_8015BE1C; |
| D_8015BE20 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3043: D_800CA13C = D_8015BE20; |
| D_8015BE24 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3184: D_800CA13C = D_8015BE24; |
| D_8015BE28 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3175: D_800CA13C = D_8015BE28; |
| D_8015BE30 | game-state/table | src/ovl/DATE/80132000.c:1093: talk_command_table = D_8015BE30; |
| D_8015BE34 | game-state/table | src/ovl/DATE/80132000.c:1359: talk_command_table = D_8015BE34; |
| D_8015BE38 | game-state/table | src/ovl/DATE/80132000.c:773: talk_command_table = D_8015BE38; |
| D_8015BE3C | game-state/table | src/ovl/DATE/80132000.c:1486: talk_command_table = D_8015BE3C; |
| D_8015BE40 | game-state/table | src/ovl/DATE/80132000.c:1057: talk_command_table = D_8015BE40; |
| D_8015BE44 | game-state/table | src/ovl/DATE/80132000.c:1124: talk_command_table = D_8015BE44; |
| D_8015BE48 | game-state/table | src/ovl/DATE/80132000.c:659: talk_command_table = D_8015BE48; |
| D_8015BE4C | game-state/table | src/ovl/DATE/80132000.c:1249: talk_command_table = D_8015BE4C; |
| D_8015BE50 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3260: D_800CA140 = D_8015BE50; |
| D_8015BE54 | game-state/table | src/ovl/DATE/80132000.c:1424: talk_command_table = D_8015BE54; |
| D_8015BE58 | game-state/table | src/ovl/DATE/80132000.c:854: talk_command_table = D_8015BE58; |
| D_8015BE5C | game-state/table | src/ovl/DATE/80132000.c:1454: talk_command_table = D_8015BE5C; |
| D_8015BE60 | game-state/table | src/ovl/DATE/80132000.c:821: talk_command_table = D_8015BE60; |
| D_8015BE64 | game-state/table | src/ovl/DATE/80132000.c:1178: talk_command_table = D_8015BE64; |
| D_8015BE68 | game-state/table | src/ovl/DATE/80132000.c:1510: talk_command_table = D_8015BE68; |
| D_8015BE6C | game-state/table | src/ovl/DATE/80132000.c:1541: talk_command_table = D_8015BE6C; |
| D_8015BE70 | game-state/table | src/ovl/DATE/80132000.c:1203: talk_command_table = D_8015BE70; |
| D_8015BE74 | game-state/table | src/ovl/DATE/80132000.c:736: talk_command_table = D_8015BE74; |
| D_8015BE78 | game-state/table | src/ovl/DATE/80132000.c:1017: talk_command_table = D_8015BE78; |
| D_8015BE7C | game-state/table | src/ovl/DATE/80132000.c:1157: talk_command_table = D_8015BE7C; |
| D_8015BE80 | game-state/table | src/ovl/DATE/80132000.c:684: talk_command_table = D_8015BE80; |
| D_8015BE84 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3159: D_800CA140 = D_8015BE84; |
| D_8015BE88 | aggregate/type/field/table | src/ovl/DATE/80132000.c:2992: D_800CA140 = D_8015BE88; |
| D_8015BE8C | aggregate/type/field/table | src/ovl/DATE/80132000.c:3044: D_800CA140 = D_8015BE8C; |
| D_8015BE90 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3185: D_800CA140 = D_8015BE90; |
| D_8015BE94 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3176: D_800CA140 = D_8015BE94; |
| D_8015BE9C | game-state/table | src/ovl/DATE/80132000.c:1094: talk_voice_table = D_8015BE9C; |
| D_8015BEA0 | game-state/table | src/ovl/DATE/80132000.c:1360: talk_voice_table = D_8015BEA0; |
| D_8015BEA4 | game-state/table | src/ovl/DATE/80132000.c:774: talk_voice_table = D_8015BEA4; |
| D_8015BEA8 | game-state/table | src/ovl/DATE/80132000.c:1487: talk_voice_table = D_8015BEA8; |
| D_8015BEAC | game-state/table | src/ovl/DATE/80132000.c:1058: talk_voice_table = D_8015BEAC; |
| D_8015BEB0 | game-state/table | src/ovl/DATE/80132000.c:1125: talk_voice_table = D_8015BEB0; |
| D_8015BEB4 | game-state/table | src/ovl/DATE/80132000.c:660: talk_voice_table = D_8015BEB4; |
| D_8015BEB8 | game-state/table | src/ovl/DATE/80132000.c:1250: talk_voice_table = D_8015BEB8; |
| D_8015BEBC | aggregate/type/field/table | src/ovl/DATE/80132000.c:3261: D_800CA144 = D_8015BEBC; |
| D_8015BEC0 | game-state/table | src/ovl/DATE/80132000.c:1425: talk_voice_table = D_8015BEC0; |
| D_8015BEC4 | game-state/table | src/ovl/DATE/80132000.c:855: talk_voice_table = D_8015BEC4; |
| D_8015BEC8 | game-state/table | src/ovl/DATE/80132000.c:1455: talk_voice_table = D_8015BEC8; |
| D_8015BECC | game-state/table | src/ovl/DATE/80132000.c:822: talk_voice_table = D_8015BECC; |
| D_8015BED0 | game-state/table | src/ovl/DATE/80132000.c:1179: talk_voice_table = D_8015BED0; |
| D_8015BED4 | game-state/table | src/ovl/DATE/80132000.c:1511: talk_voice_table = D_8015BED4; |
| D_8015BED8 | game-state/table | src/ovl/DATE/80132000.c:1542: talk_voice_table = D_8015BED8; |
| D_8015BEDC | game-state/table | src/ovl/DATE/80132000.c:1204: talk_voice_table = D_8015BEDC; |
| D_8015BEE0 | game-state/table | src/ovl/DATE/80132000.c:737: talk_voice_table = D_8015BEE0; |
| D_8015BEE4 | game-state/table | src/ovl/DATE/80132000.c:1018: talk_voice_table = D_8015BEE4; |
| D_8015BEE8 | game-state/table | src/ovl/DATE/80132000.c:1158: talk_voice_table = D_8015BEE8; |
| D_8015BEEC | game-state/table | src/ovl/DATE/80132000.c:685: talk_voice_table = D_8015BEEC; |
| D_8015BEF0 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3160: D_800CA144 = D_8015BEF0; |
| D_8015BEF4 | aggregate/type/field/table | src/ovl/DATE/80132000.c:2993: D_800CA144 = D_8015BEF4; |
| D_8015BEF8 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3045: D_800CA144 = D_8015BEF8; |
| D_8015BEFC | aggregate/type/field/table | src/ovl/DATE/80132000.c:3186: D_800CA144 = D_8015BEFC; |
| D_8015BF00 | aggregate/type/field/table | src/ovl/DATE/80132000.c:3177: D_800CA144 = D_8015BF00; |
| D_8015CE80 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:130: D_8015CF1C = D_8015CE80; |
| D_8015CE84 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:90: D_8015CF1C = D_8015CE84; |
| D_8015CE88 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:7: D_8015CE88 = 0x801B03C8; |
| D_8015CE8C | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:100: D_8015CF1C = D_8015CE8C; |
| D_8015CE90 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:105: D_8015CF1C = D_8015CE90; |
| D_8015CE94 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:125: D_8015CF1C = D_8015CE94; |
| D_8015CE98 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:95: D_8015CF1C = D_8015CE98; |
| D_8015CE9C | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:115: D_8015CF1C = D_8015CE9C; |
| D_8015CEA0 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:120: D_8015CF1C = D_8015CEA0; |
| D_8015CEA4 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:135: D_8015CF1C = D_8015CEA4; |
| D_8015CEA8 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:110: D_8015CF1C = D_8015CEA8; |
| D_8015CEAC | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:16: D_8015CEAC = 0x801B12D8; |
| D_8015CEB0 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:140: D_8015CF1C = D_8015CEB0; |
| D_8015CEB4 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:131: D_8015CF20 = D_8015CEB4; |
| D_8015CEB8 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:91: D_8015CF20 = D_8015CEB8; |
| D_8015CEBC | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:20: D_8015CEBC = 0x801B03D0; |
| D_8015CEC0 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:101: D_8015CF20 = D_8015CEC0; |
| D_8015CEC4 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:106: D_8015CF20 = D_8015CEC4; |
| D_8015CEC8 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:126: D_8015CF20 = D_8015CEC8; |
| D_8015CECC | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:96: D_8015CF20 = D_8015CECC; |
| D_8015CED0 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:116: D_8015CF20 = D_8015CED0; |
| D_8015CED4 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:121: D_8015CF20 = D_8015CED4; |
| D_8015CED8 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:136: D_8015CF20 = D_8015CED8; |
| D_8015CEDC | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:111: D_8015CF20 = D_8015CEDC; |
| D_8015CEE0 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:29: D_8015CEE0 = 0x801B12E0; |
| D_8015CEE4 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:141: D_8015CF20 = D_8015CEE4; |
| D_8015CEE8 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:132: D_8015CF24 = D_8015CEE8; |
| D_8015CEEC | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:92: D_8015CF24 = D_8015CEEC; |
| D_8015CEF0 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:33: D_8015CEF0 = 0x801B03F0; |
| D_8015CEF4 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:102: D_8015CF24 = D_8015CEF4; |
| D_8015CEF8 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:107: D_8015CF24 = D_8015CEF8; |
| D_8015CEFC | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:127: D_8015CF24 = D_8015CEFC; |
| D_8015CF00 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:97: D_8015CF24 = D_8015CF00; |
| D_8015CF04 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:117: D_8015CF24 = D_8015CF04; |
| D_8015CF08 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:122: D_8015CF24 = D_8015CF08; |
| D_8015CF0C | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:137: D_8015CF24 = D_8015CF0C; |
| D_8015CF10 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:112: D_8015CF24 = D_8015CF10; |
| D_8015CF14 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:42: D_8015CF14 = 0x801B1338; |
| D_8015CF18 | aggregate/type/field/table | src/ovl/DATE/80145AA0.c:142: D_8015CF24 = D_8015CF18; |
| D_8015CF80 | hardware presentation/resource | src/ovl/DATE/801461B0.c:478: load_palette(D_8015CF80, 0x11, 1, 0, 0); |
| D_8015CF84 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:479: func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98); |
| D_8015CF88 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:479: func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98); |
| D_8015CF8C | aggregate/type/field/table | src/ovl/DATE/801461B0.c:479: func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98); |
| D_8015CF90 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:479: func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98); |
| D_8015CF94 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:479: func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98); |
| D_8015CF98 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:479: func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98); |
| D_8015CFA0 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:317: v += D_8015CFA0; |
| D_8015CFA4 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:14: s16 D_8015CFA4[3] = { 0, 0, 0 }; |
| D_8015CFAC | aggregate/type/field/table | src/ovl/DATE/801461B0.c:15: s16 D_8015CFAC[3] = { 0, 0, 0 }; |
| D_8015CFB4 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:16: s16 D_8015CFB4[3] = { 0, 0, 0 }; |
| D_8015CFBC | aggregate/type/field/table | src/ovl/DATE/801461B0.c:17: s16 D_8015CFBC[3] = { 0, 0, 0 }; |
| D_8015CFC4 | aggregate/type/field/table | src/ovl/DATE/801461B0.c:1179: if (D_8015CFC4++ == 0x14) { |
| D_8015CFD0 | aggregate/type/field/table | src/ovl/DATE/80149310.c:201: D_8015D06C = D_8015CFD0; |
| D_8015CFD4 | aggregate/type/field/table | src/ovl/DATE/80149310.c:155: D_8015D06C = D_8015CFD4; |
| D_8015CFD8 | aggregate/type/field/table | src/ovl/DATE/80149310.c:165: D_8015D06C = D_8015CFD8; |
| D_8015CFDC | aggregate/type/field/table | src/ovl/DATE/80149310.c:170: D_8015D06C = D_8015CFDC; |
| D_8015CFE0 | aggregate/type/field/table | src/ovl/DATE/80149310.c:196: D_8015D06C = D_8015CFE0; |
| D_8015CFE4 | aggregate/type/field/table | src/ovl/DATE/80149310.c:160: D_8015D06C = D_8015CFE4; |
| D_8015CFE8 | aggregate/type/field/table | src/ovl/DATE/80149310.c:180: D_8015D06C = D_8015CFE8; |
| D_8015CFEC | aggregate/type/field/table | src/ovl/DATE/80149310.c:190: D_8015D06C = D_8015CFEC; |
| D_8015CFF0 | aggregate/type/field/table | src/ovl/DATE/80149310.c:185: D_8015D06C = D_8015CFF0; |
| D_8015CFF4 | aggregate/type/field/table | src/ovl/DATE/80149310.c:206: D_8015D06C = D_8015CFF4; |
| D_8015CFF8 | aggregate/type/field/table | src/ovl/DATE/80149310.c:175: D_8015D06C = D_8015CFF8; |
| D_8015CFFC | aggregate/type/field/table | src/ovl/DATE/80149310.c:321: D_8015D06C = D_8015CFFC; |
| D_8015D000 | aggregate/type/field/table | src/ovl/DATE/80149310.c:211: D_8015D06C = D_8015D000; |
| D_8015D004 | aggregate/type/field/table | src/ovl/DATE/80149310.c:202: D_8015D070 = D_8015D004; |
| D_8015D008 | aggregate/type/field/table | src/ovl/DATE/80149310.c:156: D_8015D070 = D_8015D008; |
| D_8015D00C | aggregate/type/field/table | src/ovl/DATE/80149310.c:166: D_8015D070 = D_8015D00C; |
| D_8015D010 | aggregate/type/field/table | src/ovl/DATE/80149310.c:171: D_8015D070 = D_8015D010; |
| D_8015D014 | aggregate/type/field/table | src/ovl/DATE/80149310.c:197: D_8015D070 = D_8015D014; |
| D_8015D018 | aggregate/type/field/table | src/ovl/DATE/80149310.c:161: D_8015D070 = D_8015D018; |
| D_8015D01C | aggregate/type/field/table | src/ovl/DATE/80149310.c:181: D_8015D070 = D_8015D01C; |
| D_8015D020 | aggregate/type/field/table | src/ovl/DATE/80149310.c:191: D_8015D070 = D_8015D020; |
| D_8015D024 | aggregate/type/field/table | src/ovl/DATE/80149310.c:186: D_8015D070 = D_8015D024; |
| D_8015D028 | aggregate/type/field/table | src/ovl/DATE/80149310.c:207: D_8015D070 = D_8015D028; |
| D_8015D02C | aggregate/type/field/table | src/ovl/DATE/80149310.c:176: D_8015D070 = D_8015D02C; |
| D_8015D030 | aggregate/type/field/table | src/ovl/DATE/80149310.c:322: D_8015D070 = D_8015D030; |
| D_8015D034 | aggregate/type/field/table | src/ovl/DATE/80149310.c:212: D_8015D070 = D_8015D034; |
| D_8015D038 | aggregate/type/field/table | src/ovl/DATE/80149310.c:203: D_8015D074 = D_8015D038; |
| D_8015D03C | aggregate/type/field/table | src/ovl/DATE/80149310.c:157: D_8015D074 = D_8015D03C; |
| D_8015D040 | aggregate/type/field/table | src/ovl/DATE/80149310.c:167: D_8015D074 = D_8015D040; |
| D_8015D044 | aggregate/type/field/table | src/ovl/DATE/80149310.c:172: D_8015D074 = D_8015D044; |
| D_8015D048 | aggregate/type/field/table | src/ovl/DATE/80149310.c:198: D_8015D074 = D_8015D048; |
| D_8015D04C | aggregate/type/field/table | src/ovl/DATE/80149310.c:162: D_8015D074 = D_8015D04C; |
| D_8015D050 | aggregate/type/field/table | src/ovl/DATE/80149310.c:182: D_8015D074 = D_8015D050; |
| D_8015D054 | aggregate/type/field/table | src/ovl/DATE/80149310.c:192: D_8015D074 = D_8015D054; |
| D_8015D058 | aggregate/type/field/table | src/ovl/DATE/80149310.c:187: D_8015D074 = D_8015D058; |
| D_8015D05C | aggregate/type/field/table | src/ovl/DATE/80149310.c:208: D_8015D074 = D_8015D05C; |
| D_8015D060 | aggregate/type/field/table | src/ovl/DATE/80149310.c:177: D_8015D074 = D_8015D060; |
| D_8015D064 | aggregate/type/field/table | src/ovl/DATE/80149310.c:323: D_8015D074 = D_8015D064; |
| D_8015D068 | aggregate/type/field/table | src/ovl/DATE/80149310.c:213: D_8015D074 = D_8015D068; |
| D_8015DDD0 | aggregate/type/field/table | src/ovl/DATE/8014A1B0.c:18: s16 D_8015DDD0[0x2C] = { 0 }; |
| D_8015DE28 | aggregate/type/field/table | src/ovl/DATE/8014A1B0.c:114: D_8015DE34[0] = D_8015DE28; |
| D_8015DE2C | aggregate/type/field/table | src/ovl/DATE/8014A1B0.c:115: D_8015DE34[1] = D_8015DE2C; |
| D_8015DE30 | aggregate/type/field/table | src/ovl/DATE/8014A1B0.c:116: D_8015DE34[2] = D_8015DE30; |
| D_8015DE34 | aggregate/type/field/table | src/ovl/DATE/8014A1B0.c:22: s32 D_8015DE34[3] = { 0, 0, 0 }; |
| D_8015DE38 | aggregate/type/field/table | src/ovl/DATE/8014A1B0.c:208: func_8014AD9C(1, D_8015DE38, 0); |
| D_8015DE3C | aggregate/type/field/table | src/ovl/DATE/8014A1B0.c:214: func_8014AD9C(i, D_8015DE3C, j); |
| D_8015DE4C | aggregate/type/field/table | src/ovl/DATE/8014A1B0.c:231: frame = D_8015DE4C; |
| D_8015DE50 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:7: D_8015DE50 = 0x80198304; |
| D_8015DE54 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:175: D_8015E208 = D_8015DE54; |
| D_8015DE58 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:215: D_8015E208 = D_8015DE58; |
| D_8015DE5C | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:252: D_8015E208 = D_8015DE5C; |
| D_8015DE60 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:268: D_8015E208 = D_8015DE60; |
| D_8015DE64 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:290: D_8015E208 = D_8015DE64; |
| D_8015DE68 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:19: D_8015DE68 = 0x80198390; |
| D_8015DE6C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:21: D_8015DE6C = 0x801986F4; |
| D_8015DE70 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:80: D_8015E208 = D_8015DE70; |
| D_8015DE74 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:93: D_8015E208 = D_8015DE74; |
| D_8015DE78 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:108: D_8015E208 = D_8015DE78; |
| D_8015DE7C | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:141: D_8015E208 = D_8015DE7C; |
| D_8015DE80 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:31: D_8015DE80 = 0x80199D78; |
| D_8015DE84 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:33: D_8015DE84 = 0x8019A584; |
| D_8015DE88 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:35: D_8015DE88 = 0x801982E8; |
| D_8015DE8C | aggregate/type/field/table | src/ovl/DATE/80150010.c:82: D_8015E208 = D_8015DE8C; |
| D_8015DE90 | aggregate/type/field/table | src/ovl/DATE/80150010.c:101: D_8015E208 = D_8015DE90; |
| D_8015DE94 | aggregate/type/field/table | src/ovl/DATE/80150010.c:117: D_8015E208 = D_8015DE94; |
| D_8015DE98 | aggregate/type/field/table | src/ovl/DATE/80150010.c:171: D_8015E208 = D_8015DE98; |
| D_8015DE9C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:45: D_8015DE9C = 0x80199604; |
| D_8015DEA0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:47: D_8015DEA0 = 0x80199B18; |
| D_8015DEA4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:49: D_8015DEA4 = 0x8019894C; |
| D_8015DEA8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:51: D_8015DEA8 = 0x80199068; |
| D_8015DEAC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:53: D_8015DEAC = 0x801994E4; |
| D_8015DEB0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:55: D_8015DEB0 = 0x801983D0; |
| D_8015DEB4 | aggregate/type/field/table | src/ovl/DATE/80151460.c:93: D_8015E208 = D_8015DEB4; |
| D_8015DEB8 | aggregate/type/field/table | src/ovl/DATE/80151460.c:124: D_8015E208 = D_8015DEB8; |
| D_8015DEBC | aggregate/type/field/table | src/ovl/DATE/80151460.c:164: D_8015E208 = D_8015DEBC; |
| D_8015DEC0 | aggregate/type/field/table | src/ovl/DATE/80151460.c:179: D_8015E208 = D_8015DEC0; |
| D_8015DEC4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:65: D_8015DEC4 = 0x80199D8C; |
| D_8015DEC8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:67: D_8015DEC8 = 0x8019829C; |
| D_8015DECC | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:84: D_8015E208 = D_8015DECC; |
| D_8015DED0 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:107: D_8015E208 = D_8015DED0; |
| D_8015DED4 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:129: D_8015E208 = D_8015DED4; |
| D_8015DED8 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:161: D_8015E208 = D_8015DED8; |
| D_8015DEDC | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:183: D_8015E208 = D_8015DEDC; |
| D_8015DEE0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:79: D_8015DEE0 = 0x80199D60; |
| D_8015DEE4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:81: D_8015DEE4 = 0x80198308; |
| D_8015DEE8 | aggregate/type/field/table | src/ovl/DATE/80154260.c:85: D_8015E208 = D_8015DEE8; |
| D_8015DEEC | aggregate/type/field/table | src/ovl/DATE/80154260.c:101: D_8015E208 = D_8015DEEC; |
| D_8015DEF0 | aggregate/type/field/table | src/ovl/DATE/80154260.c:128: D_8015E208 = D_8015DEF0; |
| D_8015DEF4 | aggregate/type/field/table | src/ovl/DATE/80154260.c:145: D_8015E208 = D_8015DEF4; |
| D_8015DEF8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:91: D_8015DEF8 = 0x801983B8; |
| D_8015DEFC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:93: D_8015DEFC = 0x8019881C; |
| D_8015DF00 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:80: D_8015E208 = D_8015DF00; |
| D_8015DF04 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:102: D_8015E208 = D_8015DF04; |
| D_8015DF08 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:129: D_8015E208 = D_8015DF08; |
| D_8015DF0C | aggregate/type/field/table | src/ovl/DATE/801552B0.c:144: D_8015E208 = D_8015DF0C; |
| D_8015DF10 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:103: D_8015DF10 = 0x8019A30C; |
| D_8015DF14 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:105: D_8015DF14 = 0x80198374; |
| D_8015DF18 | aggregate/type/field/table | src/ovl/DATE/80156600.c:84: D_8015E208 = D_8015DF18; |
| D_8015DF1C | aggregate/type/field/table | src/ovl/DATE/80156600.c:112: D_8015E208 = D_8015DF1C; |
| D_8015DF20 | aggregate/type/field/table | src/ovl/DATE/80156600.c:128: D_8015E208 = D_8015DF20; |
| D_8015DF24 | aggregate/type/field/table | src/ovl/DATE/80156600.c:144: D_8015E208 = D_8015DF24; |
| D_8015DF28 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:115: D_8015DF28 = 0x80198418; |
| D_8015DF2C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:117: D_8015DF2C = 0x801989A4; |
| D_8015DF30 | aggregate/type/field/table | src/ovl/DATE/80157880.c:84: D_8015E208 = D_8015DF30; |
| D_8015DF34 | aggregate/type/field/table | src/ovl/DATE/80157880.c:109: D_8015E208 = D_8015DF34; |
| D_8015DF38 | aggregate/type/field/table | src/ovl/DATE/80157880.c:125: D_8015E208 = D_8015DF38; |
| D_8015DF3C | aggregate/type/field/table | src/ovl/DATE/80157880.c:146: D_8015E208 = D_8015DF3C; |
| D_8015DF40 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:127: D_8015DF40 = 0x8019A37C; |
| D_8015DF44 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:129: D_8015DF44 = 0x801987E8; |
| D_8015DF48 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:84: D_8015E208 = D_8015DF48; |
| D_8015DF4C | aggregate/type/field/table | src/ovl/DATE/801586A0.c:100: D_8015E208 = D_8015DF4C; |
| D_8015DF50 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:123: D_8015E208 = D_8015DF50; |
| D_8015DF54 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:138: D_8015E208 = D_8015DF54; |
| D_8015DF58 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:139: D_8015DF58 = 0x8019ABDC; |
| D_8015DF5C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:141: D_8015DF5C = 0x80198784; |
| D_8015DF60 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:143: D_8015DF60 = 0x80198FB4; |
| D_8015DF64 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:84: D_8015E208 = D_8015DF64; |
| D_8015DF68 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:106: D_8015E208 = D_8015DF68; |
| D_8015DF6C | aggregate/type/field/table | src/ovl/DATE/801594B0.c:122: D_8015E208 = D_8015DF6C; |
| D_8015DF70 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:137: D_8015E208 = D_8015DF70; |
| D_8015DF74 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:153: D_8015DF74 = 0x8019AB0C; |
| D_8015DF78 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:155: D_8015DF78 = 0x80199B14; |
| D_8015DF7C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:157: D_8015DF7C = 0x80199F30; |
| D_8015DF80 | aggregate/type/field/table | src/ovl/DATE/8015A190.c:74: D_8015E208 = D_8015DF80; |
| D_8015DF84 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:161: D_8015DF84 = 0x8019A628; |
| D_8015DF88 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:163: D_8015DF88 = 0x8019AFF8; |
| D_8015DF8C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:8: D_8015DF8C = 0x80198340; |
| D_8015DF90 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:176: D_8015E20C = D_8015DF90; |
| D_8015DF94 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:216: D_8015E20C = D_8015DF94; |
| D_8015DF98 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:253: D_8015E20C = D_8015DF98; |
| D_8015DF9C | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:269: D_8015E20C = D_8015DF9C; |
| D_8015DFA0 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:291: D_8015E20C = D_8015DFA0; |
| D_8015DFA4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:20: D_8015DFA4 = 0x801983C0; |
| D_8015DFA8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:22: D_8015DFA8 = 0x8019870C; |
| D_8015DFAC | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:81: D_8015E20C = D_8015DFAC; |
| D_8015DFB0 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:94: D_8015E20C = D_8015DFB0; |
| D_8015DFB4 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:109: D_8015E20C = D_8015DFB4; |
| D_8015DFB8 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:142: D_8015E20C = D_8015DFB8; |
| D_8015DFBC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:32: D_8015DFBC = 0x80199DB8; |
| D_8015DFC0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:34: D_8015DFC0 = 0x8019A5D8; |
| D_8015DFC4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:36: D_8015DFC4 = 0x80198314; |
| D_8015DFC8 | aggregate/type/field/table | src/ovl/DATE/80150010.c:83: D_8015E20C = D_8015DFC8; |
| D_8015DFCC | aggregate/type/field/table | src/ovl/DATE/80150010.c:102: D_8015E20C = D_8015DFCC; |
| D_8015DFD0 | aggregate/type/field/table | src/ovl/DATE/80150010.c:118: D_8015E20C = D_8015DFD0; |
| D_8015DFD4 | aggregate/type/field/table | src/ovl/DATE/80150010.c:172: D_8015E20C = D_8015DFD4; |
| D_8015DFD8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:46: D_8015DFD8 = 0x80199644; |
| D_8015DFDC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:48: D_8015DFDC = 0x80199B38; |
| D_8015DFE0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:50: D_8015DFE0 = 0x801989C4; |
| D_8015DFE4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:52: D_8015DFE4 = 0x80199098; |
| D_8015DFE8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:54: D_8015DFE8 = 0x80199524; |
| D_8015DFEC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:56: D_8015DFEC = 0x801983FC; |
| D_8015DFF0 | aggregate/type/field/table | src/ovl/DATE/80151460.c:94: D_8015E20C = D_8015DFF0; |
| D_8015DFF4 | aggregate/type/field/table | src/ovl/DATE/80151460.c:125: D_8015E20C = D_8015DFF4; |
| D_8015DFF8 | aggregate/type/field/table | src/ovl/DATE/80151460.c:165: D_8015E20C = D_8015DFF8; |
| D_8015DFFC | aggregate/type/field/table | src/ovl/DATE/80151460.c:180: D_8015E20C = D_8015DFFC; |
| D_8015E000 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:66: D_8015E000 = 0x80199DF0; |
| D_8015E004 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:68: D_8015E004 = 0x801982D0; |
| D_8015E008 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:85: D_8015E20C = D_8015E008; |
| D_8015E00C | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:108: D_8015E20C = D_8015E00C; |
| D_8015E010 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:130: D_8015E20C = D_8015E010; |
| D_8015E014 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:162: D_8015E20C = D_8015E014; |
| D_8015E018 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:184: D_8015E20C = D_8015E018; |
| D_8015E01C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:80: D_8015E01C = 0x80199DB4; |
| D_8015E020 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:82: D_8015E020 = 0x80198330; |
| D_8015E024 | aggregate/type/field/table | src/ovl/DATE/80154260.c:86: D_8015E20C = D_8015E024; |
| D_8015E028 | aggregate/type/field/table | src/ovl/DATE/80154260.c:102: D_8015E20C = D_8015E028; |
| D_8015E02C | aggregate/type/field/table | src/ovl/DATE/80154260.c:129: D_8015E20C = D_8015E02C; |
| D_8015E030 | aggregate/type/field/table | src/ovl/DATE/80154260.c:146: D_8015E20C = D_8015E030; |
| D_8015E034 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:92: D_8015E034 = 0x801983E0; |
| D_8015E038 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:94: D_8015E038 = 0x8019883C; |
| D_8015E03C | aggregate/type/field/table | src/ovl/DATE/801552B0.c:81: D_8015E20C = D_8015E03C; |
| D_8015E040 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:103: D_8015E20C = D_8015E040; |
| D_8015E044 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:130: D_8015E20C = D_8015E044; |
| D_8015E048 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:145: D_8015E20C = D_8015E048; |
| D_8015E04C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:104: D_8015E04C = 0x8019A344; |
| D_8015E050 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:106: D_8015E050 = 0x801983A0; |
| D_8015E054 | aggregate/type/field/table | src/ovl/DATE/80156600.c:85: D_8015E20C = D_8015E054; |
| D_8015E058 | aggregate/type/field/table | src/ovl/DATE/80156600.c:113: D_8015E20C = D_8015E058; |
| D_8015E05C | aggregate/type/field/table | src/ovl/DATE/80156600.c:129: D_8015E20C = D_8015E05C; |
| D_8015E060 | aggregate/type/field/table | src/ovl/DATE/80156600.c:145: D_8015E20C = D_8015E060; |
| D_8015E064 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:116: D_8015E064 = 0x8019845C; |
| D_8015E068 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:118: D_8015E068 = 0x801989BC; |
| D_8015E06C | aggregate/type/field/table | src/ovl/DATE/80157880.c:85: D_8015E20C = D_8015E06C; |
| D_8015E070 | aggregate/type/field/table | src/ovl/DATE/80157880.c:110: D_8015E20C = D_8015E070; |
| D_8015E074 | aggregate/type/field/table | src/ovl/DATE/80157880.c:126: D_8015E20C = D_8015E074; |
| D_8015E078 | aggregate/type/field/table | src/ovl/DATE/80157880.c:147: D_8015E20C = D_8015E078; |
| D_8015E07C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:128: D_8015E07C = 0x8019A3B8; |
| D_8015E080 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:130: D_8015E080 = 0x80198850; |
| D_8015E084 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:85: D_8015E20C = D_8015E084; |
| D_8015E088 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:101: D_8015E20C = D_8015E088; |
| D_8015E08C | aggregate/type/field/table | src/ovl/DATE/801586A0.c:124: D_8015E20C = D_8015E08C; |
| D_8015E090 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:139: D_8015E20C = D_8015E090; |
| D_8015E094 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:140: D_8015E094 = 0x8019AC40; |
| D_8015E098 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:142: D_8015E098 = 0x801987F8; |
| D_8015E09C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:144: D_8015E09C = 0x80198FE0; |
| D_8015E0A0 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:85: D_8015E20C = D_8015E0A0; |
| D_8015E0A4 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:107: D_8015E20C = D_8015E0A4; |
| D_8015E0A8 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:123: D_8015E20C = D_8015E0A8; |
| D_8015E0AC | aggregate/type/field/table | src/ovl/DATE/801594B0.c:138: D_8015E20C = D_8015E0AC; |
| D_8015E0B0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:154: D_8015E0B0 = 0x8019AB4C; |
| D_8015E0B4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:156: D_8015E0B4 = 0x80199B6C; |
| D_8015E0B8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:158: D_8015E0B8 = 0x80199F50; |
| D_8015E0BC | aggregate/type/field/table | src/ovl/DATE/8015A190.c:75: D_8015E20C = D_8015E0BC; |
| D_8015E0C0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:162: D_8015E0C0 = 0x8019A690; |
| D_8015E0C4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:164: D_8015E0C4 = 0x8019B06C; |
| D_8015E0C8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:165: D_8015E0C8 = 0x801985A4; |
| D_8015E0CC | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:177: D_8015E210 = D_8015E0CC; |
| D_8015E0D0 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:217: D_8015E210 = D_8015E0D0; |
| D_8015E0D4 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:254: D_8015E210 = D_8015E0D4; |
| D_8015E0D8 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:270: D_8015E210 = D_8015E0D8; |
| D_8015E0DC | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:292: D_8015E210 = D_8015E0DC; |
| D_8015E0E0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:171: D_8015E0E0 = 0x8019861C; |
| D_8015E0E4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:172: D_8015E0E4 = 0x801987A4; |
| D_8015E0E8 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:82: D_8015E210 = D_8015E0E8; |
| D_8015E0EC | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:95: D_8015E210 = D_8015E0EC; |
| D_8015E0F0 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:110: D_8015E210 = D_8015E0F0; |
| D_8015E0F4 | aggregate/type/field/table | src/ovl/DATE/8014F0F0.c:143: D_8015E210 = D_8015E0F4; |
| D_8015E0F8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:177: D_8015E0F8 = 0x8019A020; |
| D_8015E0FC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:178: D_8015E0FC = 0x8019AA8C; |
| D_8015E100 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:179: D_8015E100 = 0x801984E8; |
| D_8015E104 | aggregate/type/field/table | src/ovl/DATE/80150010.c:84: D_8015E210 = D_8015E104; |
| D_8015E108 | aggregate/type/field/table | src/ovl/DATE/80150010.c:103: D_8015E210 = D_8015E108; |
| D_8015E10C | aggregate/type/field/table | src/ovl/DATE/80150010.c:119: D_8015E210 = D_8015E10C; |
| D_8015E110 | aggregate/type/field/table | src/ovl/DATE/80150010.c:173: D_8015E210 = D_8015E110; |
| D_8015E114 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:184: D_8015E114 = 0x80199890; |
| D_8015E118 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:185: D_8015E118 = 0x80199CCC; |
| D_8015E11C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:186: D_8015E11C = 0x80198F14; |
| D_8015E120 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:187: D_8015E120 = 0x80199190; |
| D_8015E124 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:188: D_8015E124 = 0x801997F8; |
| D_8015E128 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:189: D_8015E128 = 0x8019863C; |
| D_8015E12C | aggregate/type/field/table | src/ovl/DATE/80151460.c:95: D_8015E210 = D_8015E12C; |
| D_8015E130 | aggregate/type/field/table | src/ovl/DATE/80151460.c:126: D_8015E210 = D_8015E130; |
| D_8015E134 | aggregate/type/field/table | src/ovl/DATE/80151460.c:166: D_8015E210 = D_8015E134; |
| D_8015E138 | aggregate/type/field/table | src/ovl/DATE/80151460.c:181: D_8015E210 = D_8015E138; |
| D_8015E13C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:194: D_8015E13C = 0x8019A050; |
| D_8015E140 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:195: D_8015E140 = 0x80198470; |
| D_8015E144 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:86: D_8015E210 = D_8015E144; |
| D_8015E148 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:109: D_8015E210 = D_8015E148; |
| D_8015E14C | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:131: D_8015E210 = D_8015E14C; |
| D_8015E150 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:163: D_8015E210 = D_8015E150; |
| D_8015E154 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:185: D_8015E210 = D_8015E154; |
| D_8015E158 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:201: D_8015E158 = 0x8019A044; |
| D_8015E15C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:202: D_8015E15C = 0x8019851C; |
| D_8015E160 | aggregate/type/field/table | src/ovl/DATE/80154260.c:87: D_8015E210 = D_8015E160; |
| D_8015E164 | aggregate/type/field/table | src/ovl/DATE/80154260.c:103: D_8015E210 = D_8015E164; |
| D_8015E168 | aggregate/type/field/table | src/ovl/DATE/80154260.c:130: D_8015E210 = D_8015E168; |
| D_8015E16C | aggregate/type/field/table | src/ovl/DATE/80154260.c:147: D_8015E210 = D_8015E16C; |
| D_8015E170 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:207: D_8015E170 = 0x801985E4; |
| D_8015E174 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:208: D_8015E174 = 0x80198998; |
| D_8015E178 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:82: D_8015E210 = D_8015E178; |
| D_8015E17C | aggregate/type/field/table | src/ovl/DATE/801552B0.c:104: D_8015E210 = D_8015E17C; |
| D_8015E180 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:131: D_8015E210 = D_8015E180; |
| D_8015E184 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:146: D_8015E210 = D_8015E184; |
| D_8015E188 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:213: D_8015E188 = 0x8019A500; |
| D_8015E18C | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:214: D_8015E18C = 0x801985A8; |
| D_8015E190 | aggregate/type/field/table | src/ovl/DATE/80156600.c:86: D_8015E210 = D_8015E190; |
| D_8015E194 | aggregate/type/field/table | src/ovl/DATE/80156600.c:114: D_8015E210 = D_8015E194; |
| D_8015E198 | aggregate/type/field/table | src/ovl/DATE/80156600.c:130: D_8015E210 = D_8015E198; |
| D_8015E19C | aggregate/type/field/table | src/ovl/DATE/80156600.c:146: D_8015E210 = D_8015E19C; |
| D_8015E1A0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:219: D_8015E1A0 = 0x80198784; |
| D_8015E1A4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:220: D_8015E1A4 = 0x80198B14; |
| D_8015E1A8 | aggregate/type/field/table | src/ovl/DATE/80157880.c:86: D_8015E210 = D_8015E1A8; |
| D_8015E1AC | aggregate/type/field/table | src/ovl/DATE/80157880.c:111: D_8015E210 = D_8015E1AC; |
| D_8015E1B0 | aggregate/type/field/table | src/ovl/DATE/80157880.c:127: D_8015E210 = D_8015E1B0; |
| D_8015E1B4 | aggregate/type/field/table | src/ovl/DATE/80157880.c:148: D_8015E210 = D_8015E1B4; |
| D_8015E1B8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:225: D_8015E1B8 = 0x8019A5CC; |
| D_8015E1BC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:226: D_8015E1BC = 0x80198E04; |
| D_8015E1C0 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:86: D_8015E210 = D_8015E1C0; |
| D_8015E1C4 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:102: D_8015E210 = D_8015E1C4; |
| D_8015E1C8 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:125: D_8015E210 = D_8015E1C8; |
| D_8015E1CC | aggregate/type/field/table | src/ovl/DATE/801586A0.c:140: D_8015E210 = D_8015E1CC; |
| D_8015E1D0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:231: D_8015E1D0 = 0x8019AFE8; |
| D_8015E1D4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:232: D_8015E1D4 = 0x80198CF4; |
| D_8015E1D8 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:233: D_8015E1D8 = 0x801991E8; |
| D_8015E1DC | aggregate/type/field/table | src/ovl/DATE/801594B0.c:86: D_8015E210 = D_8015E1DC; |
| D_8015E1E0 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:108: D_8015E210 = D_8015E1E0; |
| D_8015E1E4 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:124: D_8015E210 = D_8015E1E4; |
| D_8015E1E8 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:139: D_8015E210 = D_8015E1E8; |
| D_8015E1EC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:238: D_8015E1EC = 0x8019ADD0; |
| D_8015E1F0 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:239: D_8015E1F0 = 0x80199DE4; |
| D_8015E1F4 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:240: D_8015E1F4 = 0x8019A05C; |
| D_8015E1F8 | aggregate/type/field/table | src/ovl/DATE/8015A190.c:76: D_8015E210 = D_8015E1F8; |
| D_8015E1FC | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:242: D_8015E1FC = 0x8019AB34; |
| D_8015E200 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:243: D_8015E200 = 0x8019B494; |
| D_8015E204 | aggregate/type/field/table | src/ovl/DATE/8014B1F0.c:343: D_80122D48 = (u8) D_8015E204 + 1; |
| D_8015E220 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:184: scene_anim_show(D_8015E224, D_8015E228, D_8015E220, (s32) D_8015E22C); |
| D_8015E224 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:184: scene_anim_show(D_8015E224, D_8015E228, D_8015E220, (s32) D_8015E22C); |
| D_8015E228 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:184: scene_anim_show(D_8015E224, D_8015E228, D_8015E220, (s32) D_8015E22C); |
| D_8015E22C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:184: scene_anim_show(D_8015E224, D_8015E228, D_8015E220, (s32) D_8015E22C); |
| D_8015E230 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:182: load_palette(D_8015E230, 0x11, 1, 2, 0); |
| D_8015E234 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:183: scene_still_show(D_8015E234, D_8015E238, D_8015E23C, D_8015E240, D_8015E244, D_8015E248); |
| D_8015E238 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:183: scene_still_show(D_8015E234, D_8015E238, D_8015E23C, D_8015E240, D_8015E244, D_8015E248); |
| D_8015E23C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:183: scene_still_show(D_8015E234, D_8015E238, D_8015E23C, D_8015E240, D_8015E244, D_8015E248); |
| D_8015E240 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:183: scene_still_show(D_8015E234, D_8015E238, D_8015E23C, D_8015E240, D_8015E244, D_8015E248); |
| D_8015E244 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:183: scene_still_show(D_8015E234, D_8015E238, D_8015E23C, D_8015E240, D_8015E244, D_8015E248); |
| D_8015E248 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:183: scene_still_show(D_8015E234, D_8015E238, D_8015E23C, D_8015E240, D_8015E244, D_8015E248); |
| D_8015E24C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:229: scene_anim_show(D_8015E250, D_8015E254, D_8015E24C, (s32) D_8015E258); |
| D_8015E250 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:229: scene_anim_show(D_8015E250, D_8015E254, D_8015E24C, (s32) D_8015E258); |
| D_8015E254 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:229: scene_anim_show(D_8015E250, D_8015E254, D_8015E24C, (s32) D_8015E258); |
| D_8015E258 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:229: scene_anim_show(D_8015E250, D_8015E254, D_8015E24C, (s32) D_8015E258); |
| D_8015E25C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:225: load_palette(D_8015E25C, 0x11, 1, 2, 0); |
| D_8015E260 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:228: scene_still_show(D_8015E260, D_8015E264, D_8015E268, D_8015E26C, D_8015E270, D_8015E274); |
| D_8015E264 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:228: scene_still_show(D_8015E260, D_8015E264, D_8015E268, D_8015E26C, D_8015E270, D_8015E274); |
| D_8015E268 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:228: scene_still_show(D_8015E260, D_8015E264, D_8015E268, D_8015E26C, D_8015E270, D_8015E274); |
| D_8015E26C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:228: scene_still_show(D_8015E260, D_8015E264, D_8015E268, D_8015E26C, D_8015E270, D_8015E274); |
| D_8015E270 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:228: scene_still_show(D_8015E260, D_8015E264, D_8015E268, D_8015E26C, D_8015E270, D_8015E274); |
| D_8015E274 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:228: scene_still_show(D_8015E260, D_8015E264, D_8015E268, D_8015E26C, D_8015E270, D_8015E274); |
| D_8015E278 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:262: scene_anim_show(D_8015E27C, D_8015E280, D_8015E278, (s32) D_8015E284); |
| D_8015E27C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:262: scene_anim_show(D_8015E27C, D_8015E280, D_8015E278, (s32) D_8015E284); |
| D_8015E280 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:262: scene_anim_show(D_8015E27C, D_8015E280, D_8015E278, (s32) D_8015E284); |
| D_8015E284 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:262: scene_anim_show(D_8015E27C, D_8015E280, D_8015E278, (s32) D_8015E284); |
| D_8015E288 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:260: load_palette(D_8015E288, 0x11, 1, 2, 0); |
| D_8015E28C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:261: scene_still_show(D_8015E28C, D_8015E290, D_8015E294, D_8015E298, D_8015E29C, D_8015E2A0); |
| D_8015E290 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:261: scene_still_show(D_8015E28C, D_8015E290, D_8015E294, D_8015E298, D_8015E29C, D_8015E2A0); |
| D_8015E294 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:261: scene_still_show(D_8015E28C, D_8015E290, D_8015E294, D_8015E298, D_8015E29C, D_8015E2A0); |
| D_8015E298 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:261: scene_still_show(D_8015E28C, D_8015E290, D_8015E294, D_8015E298, D_8015E29C, D_8015E2A0); |
| D_8015E29C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:261: scene_still_show(D_8015E28C, D_8015E290, D_8015E294, D_8015E298, D_8015E29C, D_8015E2A0); |
| D_8015E2A0 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:261: scene_still_show(D_8015E28C, D_8015E290, D_8015E294, D_8015E298, D_8015E29C, D_8015E2A0); |
| D_8015E2A4 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:278: scene_anim_show(D_8015E2A8, D_8015E2AC, D_8015E2A4, D_8015E2B0); |
| D_8015E2A8 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:278: scene_anim_show(D_8015E2A8, D_8015E2AC, D_8015E2A4, D_8015E2B0); |
| D_8015E2AC | hardware presentation/resource | src/ovl/DATE/8014CC40.c:278: scene_anim_show(D_8015E2A8, D_8015E2AC, D_8015E2A4, D_8015E2B0); |
| D_8015E2B0 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:278: scene_anim_show(D_8015E2A8, D_8015E2AC, D_8015E2A4, D_8015E2B0); |
| D_8015E2B4 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:276: load_palette(D_8015E2B4, 0x11, 1, 2, 0); |
| D_8015E2B8 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:277: scene_still_show(D_8015E2B8, D_8015E2BC, D_8015E2C0, D_8015E2C4, D_8015E2C8, D_8015E2CC); |
| D_8015E2BC | hardware presentation/resource | src/ovl/DATE/8014CC40.c:277: scene_still_show(D_8015E2B8, D_8015E2BC, D_8015E2C0, D_8015E2C4, D_8015E2C8, D_8015E2CC); |
| D_8015E2C0 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:277: scene_still_show(D_8015E2B8, D_8015E2BC, D_8015E2C0, D_8015E2C4, D_8015E2C8, D_8015E2CC); |
| D_8015E2C4 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:277: scene_still_show(D_8015E2B8, D_8015E2BC, D_8015E2C0, D_8015E2C4, D_8015E2C8, D_8015E2CC); |
| D_8015E2C8 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:277: scene_still_show(D_8015E2B8, D_8015E2BC, D_8015E2C0, D_8015E2C4, D_8015E2C8, D_8015E2CC); |
| D_8015E2CC | hardware presentation/resource | src/ovl/DATE/8014CC40.c:277: scene_still_show(D_8015E2B8, D_8015E2BC, D_8015E2C0, D_8015E2C4, D_8015E2C8, D_8015E2CC); |
| D_8015E2D0 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:296: scene_anim_show(D_8015E2D4, D_8015E2D8, D_8015E2D0, D_8015E2DC); |
| D_8015E2D4 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:296: scene_anim_show(D_8015E2D4, D_8015E2D8, D_8015E2D0, D_8015E2DC); |
| D_8015E2D8 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:296: scene_anim_show(D_8015E2D4, D_8015E2D8, D_8015E2D0, D_8015E2DC); |
| D_8015E2DC | hardware presentation/resource | src/ovl/DATE/8014CC40.c:296: scene_anim_show(D_8015E2D4, D_8015E2D8, D_8015E2D0, D_8015E2DC); |
| D_8015E2E0 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:294: load_palette(D_8015E2E0, 0x11, 1, 2, 0); |
| D_8015E2E4 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:295: scene_still_show(D_8015E2E4, D_8015E2E8, D_8015E2EC, D_8015E2F0, D_8015E2F4, D_8015E2F8); |
| D_8015E2E8 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:295: scene_still_show(D_8015E2E4, D_8015E2E8, D_8015E2EC, D_8015E2F0, D_8015E2F4, D_8015E2F8); |
| D_8015E2EC | hardware presentation/resource | src/ovl/DATE/8014CC40.c:295: scene_still_show(D_8015E2E4, D_8015E2E8, D_8015E2EC, D_8015E2F0, D_8015E2F4, D_8015E2F8); |
| D_8015E2F0 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:295: scene_still_show(D_8015E2E4, D_8015E2E8, D_8015E2EC, D_8015E2F0, D_8015E2F4, D_8015E2F8); |
| D_8015E2F4 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:295: scene_still_show(D_8015E2E4, D_8015E2E8, D_8015E2EC, D_8015E2F0, D_8015E2F4, D_8015E2F8); |
| D_8015E2F8 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:295: scene_still_show(D_8015E2E4, D_8015E2E8, D_8015E2EC, D_8015E2F0, D_8015E2F4, D_8015E2F8); |
| D_8015E2FC | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:194: D_8012070C = D_8015E2FC; |
| D_8015E300 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:193: D_801206E4 = D_8015E300; |
| D_8015E304 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:195: D_801206E8 = D_8015E304; |
| D_8015E308 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:196: D_801206EC = D_8015E308; |
| D_8015E30C | hardware presentation/resource | src/ovl/DATE/8014CC40.c:187: load_palette(D_8015E30C, 0x12, 1, 2, 0); |
| D_8015E310 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:385: load_palette(D_8015E30C, 0x12, 1, 2, D_8015E310); |
| D_8015E314 | hardware presentation/resource | src/ovl/DATE/8014CC40.c:496: load_palette(D_8015E25C, 0x12, 1, 2, D_8015E314 + 4); |
| D_8015E318 | aggregate/type/field/table | src/ovl/DATE/8014CC40.c:488: D_8015E314 += D_8015E318; |
| D_8015E690 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:85: scene_anim_show(D_8015E694, D_8015E698, D_8015E690, D_8015E69C); |
| D_8015E694 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:85: scene_anim_show(D_8015E694, D_8015E698, D_8015E690, D_8015E69C); |
| D_8015E698 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:85: scene_anim_show(D_8015E694, D_8015E698, D_8015E690, D_8015E69C); |
| D_8015E69C | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:85: scene_anim_show(D_8015E694, D_8015E698, D_8015E690, D_8015E69C); |
| D_8015E6A0 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:83: load_palette(D_8015E6A0, 0x11, 1, 2, 0); |
| D_8015E6A4 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:84: scene_still_show(D_8015E6A4, D_8015E6A8, D_8015E6AC, D_8015E6B0, D_8015E6B4, D_8015E6B8); |
| D_8015E6A8 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:84: scene_still_show(D_8015E6A4, D_8015E6A8, D_8015E6AC, D_8015E6B0, D_8015E6B4, D_8015E6B8); |
| D_8015E6AC | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:84: scene_still_show(D_8015E6A4, D_8015E6A8, D_8015E6AC, D_8015E6B0, D_8015E6B4, D_8015E6B8); |
| D_8015E6B0 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:84: scene_still_show(D_8015E6A4, D_8015E6A8, D_8015E6AC, D_8015E6B0, D_8015E6B4, D_8015E6B8); |
| D_8015E6B4 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:84: scene_still_show(D_8015E6A4, D_8015E6A8, D_8015E6AC, D_8015E6B0, D_8015E6B4, D_8015E6B8); |
| D_8015E6B8 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:84: scene_still_show(D_8015E6A4, D_8015E6A8, D_8015E6AC, D_8015E6B0, D_8015E6B4, D_8015E6B8); |
| D_8015E6BC | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:98: scene_anim_show(D_8015E6C0, D_8015E6C4, D_8015E6BC, D_8015E6C8); |
| D_8015E6C0 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:98: scene_anim_show(D_8015E6C0, D_8015E6C4, D_8015E6BC, D_8015E6C8); |
| D_8015E6C4 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:98: scene_anim_show(D_8015E6C0, D_8015E6C4, D_8015E6BC, D_8015E6C8); |
| D_8015E6C8 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:98: scene_anim_show(D_8015E6C0, D_8015E6C4, D_8015E6BC, D_8015E6C8); |
| D_8015E6CC | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:96: load_palette(D_8015E6CC, 0x11, 1, 2, 0); |
| D_8015E6D0 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:97: scene_still_show(D_8015E6D0, D_8015E6D4, D_8015E6D8, D_8015E6DC, D_8015E6E0, D_8015E6E4); |
| D_8015E6D4 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:97: scene_still_show(D_8015E6D0, D_8015E6D4, D_8015E6D8, D_8015E6DC, D_8015E6E0, D_8015E6E4); |
| D_8015E6D8 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:97: scene_still_show(D_8015E6D0, D_8015E6D4, D_8015E6D8, D_8015E6DC, D_8015E6E0, D_8015E6E4); |
| D_8015E6DC | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:97: scene_still_show(D_8015E6D0, D_8015E6D4, D_8015E6D8, D_8015E6DC, D_8015E6E0, D_8015E6E4); |
| D_8015E6E0 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:97: scene_still_show(D_8015E6D0, D_8015E6D4, D_8015E6D8, D_8015E6DC, D_8015E6E0, D_8015E6E4); |
| D_8015E6E4 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:97: scene_still_show(D_8015E6D0, D_8015E6D4, D_8015E6D8, D_8015E6DC, D_8015E6E0, D_8015E6E4); |
| D_8015E6E8 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:135: scene_anim_show(D_8015E6EC, D_8015E6F0, D_8015E6E8, D_8015E6F4); |
| D_8015E6EC | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:135: scene_anim_show(D_8015E6EC, D_8015E6F0, D_8015E6E8, D_8015E6F4); |
| D_8015E6F0 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:135: scene_anim_show(D_8015E6EC, D_8015E6F0, D_8015E6E8, D_8015E6F4); |
| D_8015E6F4 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:135: scene_anim_show(D_8015E6EC, D_8015E6F0, D_8015E6E8, D_8015E6F4); |
| D_8015E6F8 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:133: load_palette(D_8015E6F8, 0x11, 1, 2, 0); |
| D_8015E6FC | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:134: scene_still_show(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710); |
| D_8015E700 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:134: scene_still_show(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710); |
| D_8015E704 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:134: scene_still_show(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710); |
| D_8015E708 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:134: scene_still_show(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710); |
| D_8015E70C | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:134: scene_still_show(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710); |
| D_8015E710 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:134: scene_still_show(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710); |
| D_8015E714 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:144: load_palette(D_8015E714, 0x11, 1, 2, 0); |
| D_8015E718 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:145: scene_still_show(D_8015E718, D_8015E71C, D_8015E720, D_8015E724, D_8015E728, D_8015E72C); |
| D_8015E71C | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:145: scene_still_show(D_8015E718, D_8015E71C, D_8015E720, D_8015E724, D_8015E728, D_8015E72C); |
| D_8015E720 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:145: scene_still_show(D_8015E718, D_8015E71C, D_8015E720, D_8015E724, D_8015E728, D_8015E72C); |
| D_8015E724 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:145: scene_still_show(D_8015E718, D_8015E71C, D_8015E720, D_8015E724, D_8015E728, D_8015E72C); |
| D_8015E728 | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:145: scene_still_show(D_8015E718, D_8015E71C, D_8015E720, D_8015E724, D_8015E728, D_8015E72C); |
| D_8015E72C | hardware presentation/resource | src/ovl/DATE/8014F0F0.c:145: scene_still_show(D_8015E718, D_8015E71C, D_8015E720, D_8015E724, D_8015E728, D_8015E72C); |
| D_8015EA40 | resource/address initializer; type/layout deferred | src/ovl/DATE/80150010.c:8: D_8015EA40 = 0x801B0000; |
| D_8015EA44 | resource/address initializer; type/layout deferred | src/ovl/DATE/80150010.c:9: D_8015EA44 = 0x801B2000; |
| D_8015EA48 | resource/address initializer; type/layout deferred | src/ovl/DATE/80150010.c:10: D_8015EA48 = 0x801B6000; |
| D_8015EA4C | resource/address initializer; type/layout deferred | src/ovl/DATE/80150010.c:11: D_8015EA4C = 0x801BA000; |
| D_8015EA50 | resource/address initializer; type/layout deferred | src/ovl/DATE/80150010.c:12: D_8015EA50 = 0x801BE000; |
| D_8015EA54 | resource/address initializer; type/layout deferred | src/ovl/DATE/80150010.c:13: D_8015EA54 = 0x801C2000; |
| D_8015EA58 | resource/address initializer; type/layout deferred | src/ovl/DATE/80150010.c:14: D_8015EA58 = 0x801C6000; |
| D_8015EA5C | aggregate/type/field/table | src/ovl/DATE/80150010.c:18: D_8015EA5C = (u8 *)0x801CE1A4; |
| D_8015EA60 | aggregate/type/field/table | src/ovl/DATE/80150010.c:19: D_8015EA60 = (u8 *)0x801CE1A8; |
| D_8015EA64 | aggregate/type/field/table | src/ovl/DATE/80150010.c:20: D_8015EA64 = (u8 *)0x801CE1E0; |
| D_8015EA68 | aggregate/type/field/table | src/ovl/DATE/80150010.c:21: D_8015EA68 = *(s16 *)0x801CE1FC; |
| D_8015EA6C | aggregate/type/field/table | src/ovl/DATE/80150010.c:22: D_8015EA6C = (u8 *)0x801B0000; |
| D_8015EA70 | aggregate/type/field/table | src/ovl/DATE/80150010.c:23: D_8015EA70 = (u8 *)0x801B2000; |
| D_8015EA74 | aggregate/type/field/table | src/ovl/DATE/80150010.c:24: D_8015EA74 = (u8 *)0x801B6000; |
| D_8015EA78 | aggregate/type/field/table | src/ovl/DATE/80150010.c:25: D_8015EA78 = (u8 *)0x801BA000; |
| D_8015EA7C | aggregate/type/field/table | src/ovl/DATE/80150010.c:26: D_8015EA7C = (u8 *)0x801BE000; |
| D_8015EA80 | aggregate/type/field/table | src/ovl/DATE/80150010.c:27: D_8015EA80 = (u8 *)0x801C2000; |
| D_8015EA84 | aggregate/type/field/table | src/ovl/DATE/80150010.c:28: D_8015EA84 = (u8 *)0x801C6000; |
| D_8015EA88 | aggregate/type/field/table | src/ovl/DATE/80150010.c:32: D_8015EA88 = (u8 *)0x801D2370; |
| D_8015EA8C | aggregate/type/field/table | src/ovl/DATE/80150010.c:33: D_8015EA8C = (u8 *)0x801D2374; |
| D_8015EA90 | aggregate/type/field/table | src/ovl/DATE/80150010.c:34: D_8015EA90 = (u8 *)0x801D23A4; |
| D_8015EA94 | aggregate/type/field/table | src/ovl/DATE/80150010.c:35: D_8015EA94 = *(s16 *)0x801D23C0; |
| D_8015EA98 | aggregate/type/field/table | src/ovl/DATE/80150010.c:36: D_8015EA98 = (u8 *)0x801B0000; |
| D_8015EA9C | aggregate/type/field/table | src/ovl/DATE/80150010.c:37: D_8015EA9C = (u8 *)0x801D2000; |
| D_8015EAA0 | aggregate/type/field/table | src/ovl/DATE/80150010.c:38: D_8015EAA0 = (u8 *)0x801B2000; |
| D_8015EAA4 | aggregate/type/field/table | src/ovl/DATE/80150010.c:39: D_8015EAA4 = (u8 *)0x801B6000; |
| D_8015EAA8 | aggregate/type/field/table | src/ovl/DATE/80150010.c:40: D_8015EAA8 = (u8 *)0x801BA000; |
| D_8015EAAC | aggregate/type/field/table | src/ovl/DATE/80150010.c:41: D_8015EAAC = (u8 *)0x801BE000; |
| D_8015EAB0 | aggregate/type/field/table | src/ovl/DATE/80150010.c:42: D_8015EAB0 = (u8 *)0x801C2000; |
| D_8015EAB4 | aggregate/type/field/table | src/ovl/DATE/80150010.c:43: D_8015EAB4 = (u8 *)0x801C6000; |
| D_8015EAB8 | aggregate/type/field/table | src/ovl/DATE/80150010.c:44: D_8015EAB8 = (u8 *)0x801CE000; |
| D_8015EABC | aggregate/type/field/table | src/ovl/DATE/80150010.c:48: D_8015EABC = (u8 *)0x801D2174; |
| D_8015EAC0 | aggregate/type/field/table | src/ovl/DATE/80150010.c:49: D_8015EAC0 = (u8 *)0x801D217C; |
| D_8015EAC4 | aggregate/type/field/table | src/ovl/DATE/80150010.c:50: D_8015EAC4 = (u8 *)0x801D21A8; |
| D_8015EAC8 | aggregate/type/field/table | src/ovl/DATE/80150010.c:51: D_8015EAC8 = *(s16 *)0x801D21C4; |
| D_8015EACC | aggregate/type/field/table | src/ovl/DATE/80150010.c:52: D_8015EACC = (u8 *)0x801B0000; |
| D_8015EAD0 | aggregate/type/field/table | src/ovl/DATE/80150010.c:53: D_8015EAD0 = (u8 *)0x801B2000; |
| D_8015EAD4 | aggregate/type/field/table | src/ovl/DATE/80150010.c:54: D_8015EAD4 = (u8 *)0x801B6000; |
| D_8015EAD8 | aggregate/type/field/table | src/ovl/DATE/80150010.c:55: D_8015EAD8 = (u8 *)0x801BA000; |
| D_8015EADC | aggregate/type/field/table | src/ovl/DATE/80150010.c:56: D_8015EADC = (u8 *)0x801BE000; |
| D_8015EAE0 | aggregate/type/field/table | src/ovl/DATE/80150010.c:57: D_8015EAE0 = (u8 *)0x801C2000; |
| D_8015EAE4 | aggregate/type/field/table | src/ovl/DATE/80150010.c:58: D_8015EAE4 = (u8 *)0x801C6000; |
| D_8015ED70 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:9: D_8015ED70 = 0x801E2118; |
| D_8015ED74 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:10: D_8015ED74 = 0x801E2130; |
| D_8015ED78 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:11: D_8015ED78 = 0x801E2154; |
| D_8015ED7C | aggregate/type/field/table | src/ovl/DATE/80151460.c:12: D_8015ED7C = *(s16 *)0x801E216C; |
| D_8015ED80 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:13: D_8015ED80 = 0x801B0000; |
| D_8015ED84 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:14: D_8015ED84 = 0x801B2000; |
| D_8015ED88 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:15: D_8015ED88 = 0x801B6000; |
| D_8015ED8C | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:16: D_8015ED8C = 0x801BA000; |
| D_8015ED90 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:17: D_8015ED90 = 0x801BE000; |
| D_8015ED94 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:18: D_8015ED94 = 0x801C2000; |
| D_8015ED98 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:19: D_8015ED98 = 0x801C6000; |
| D_8015ED9C | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:23: D_8015ED9C = 0x801D2160; |
| D_8015EDA0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:24: D_8015EDA0 = 0x801D2168; |
| D_8015EDA4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:25: D_8015EDA4 = 0x801D2188; |
| D_8015EDA8 | aggregate/type/field/table | src/ovl/DATE/80151460.c:26: D_8015EDA8 = *(s16 *)0x801D21A4; |
| D_8015EDAC | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:27: D_8015EDAC = 0x801B0000; |
| D_8015EDB0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:28: D_8015EDB0 = 0x801B2000; |
| D_8015EDB4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:29: D_8015EDB4 = 0x801B6000; |
| D_8015EDB8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:30: D_8015EDB8 = 0x801BA000; |
| D_8015EDBC | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:31: D_8015EDBC = 0x801BE000; |
| D_8015EDC0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:32: D_8015EDC0 = 0x801C2000; |
| D_8015EDC4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:33: D_8015EDC4 = 0x801C6000; |
| D_8015EDC8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:37: D_8015EDC8 = 0x801CE124; |
| D_8015EDCC | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:38: D_8015EDCC = 0x801CE128; |
| D_8015EDD0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:39: D_8015EDD0 = 0x801CE148; |
| D_8015EDD4 | aggregate/type/field/table | src/ovl/DATE/80151460.c:40: D_8015EDD4 = *(s16 *)0x801CE15C; |
| D_8015EDD8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:41: D_8015EDD8 = 0x801B0000; |
| D_8015EDDC | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:42: D_8015EDDC = 0x801B2000; |
| D_8015EDE0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:43: D_8015EDE0 = 0x801B6000; |
| D_8015EDE4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:44: D_8015EDE4 = 0x801BA000; |
| D_8015EDE8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:45: D_8015EDE8 = 0x801BE000; |
| D_8015EDEC | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:46: D_8015EDEC = 0x801C2000; |
| D_8015EDF0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:47: D_8015EDF0 = 0x801C6000; |
| D_8015EDF4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:51: D_8015EDF4 = 0x801CE080; |
| D_8015EDF8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:52: D_8015EDF8 = 0x801CE084; |
| D_8015EDFC | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:53: D_8015EDFC = 0x801CE094; |
| D_8015EE00 | aggregate/type/field/table | src/ovl/DATE/80151460.c:54: D_8015EE00 = *(s16 *)0x801CE0A8; |
| D_8015EE04 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:55: D_8015EE04 = 0x801B0000; |
| D_8015EE08 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:56: D_8015EE08 = 0x801B2000; |
| D_8015EE0C | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:57: D_8015EE0C = 0x801B6000; |
| D_8015EE10 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:58: D_8015EE10 = 0x801BA000; |
| D_8015EE14 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:59: D_8015EE14 = 0x801BE000; |
| D_8015EE18 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:60: D_8015EE18 = 0x801C2000; |
| D_8015EE1C | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:61: D_8015EE1C = 0x801C6000; |
| D_8015EE20 | resource/address initializer; type/layout deferred | src/ovl/DATE/80151460.c:65: D_8015EE20 = 0x801E4000; |
| D_8015EE24 | aggregate/type/field/table | src/ovl/DATE/80151460.c:91: D_8015EE24[i] = rand() % 4; |
| D_8015EF0C | aggregate/type/field/table | src/ovl/DATE/80151460.c:255: tbl = D_8015EF0C; |
| D_8015EF14 | aggregate/type/field/table | src/ovl/DATE/80151460.c:369: extern DtTbl D_8015EF14; |
| D_8015F160 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:5: D_8015F160 = 0x801CE124; |
| D_8015F164 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:6: D_8015F164 = 0x801CE128; |
| D_8015F168 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:7: D_8015F168 = 0x801CE148; |
| D_8015F16C | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:8: D_8015F16C = *(s16 *)0x801CE15C; |
| D_8015F170 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:9: D_8015F170 = 0x801B0000; |
| D_8015F174 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:10: D_8015F174 = 0x801B2000; |
| D_8015F178 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:11: D_8015F178 = 0x801B6000; |
| D_8015F17C | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:12: D_8015F17C = 0x801BA000; |
| D_8015F180 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:13: D_8015F180 = 0x801BE000; |
| D_8015F184 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:14: D_8015F184 = 0x801C2000; |
| D_8015F188 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:15: D_8015F188 = 0x801C6000; |
| D_8015F18C | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:19: D_8015F18C = 0x801CE0F4; |
| D_8015F190 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:20: D_8015F190 = 0x801CE0F8; |
| D_8015F194 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:21: D_8015F194 = 0x801CE118; |
| D_8015F198 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:22: D_8015F198 = *(s16 *)0x801CE12C; |
| D_8015F19C | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:23: D_8015F19C = 0x801B0000; |
| D_8015F1A0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:24: D_8015F1A0 = 0x801B2000; |
| D_8015F1A4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:25: D_8015F1A4 = 0x801B6000; |
| D_8015F1A8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:26: D_8015F1A8 = 0x801BA000; |
| D_8015F1AC | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:27: D_8015F1AC = 0x801BE000; |
| D_8015F1B0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:28: D_8015F1B0 = 0x801C2000; |
| D_8015F1B4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:29: D_8015F1B4 = 0x801C6000; |
| D_8015F1B8 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:33: D_8015F1B8 = (u8 *)0x801CE088; |
| D_8015F1BC | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:34: D_8015F1BC = (u8 *)0x801CE08C; |
| D_8015F1C0 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:35: D_8015F1C0 = (u8 *)0x801CE0A0; |
| D_8015F1C4 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:36: D_8015F1C4 = *(s16 *)0x801CE0B4; |
| D_8015F1C8 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:37: D_8015F1C8 = (u8 *)0x801B0000; |
| D_8015F1CC | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:38: D_8015F1CC = (u8 *)0x801B2000; |
| D_8015F1D0 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:39: D_8015F1D0 = (u8 *)0x801B6000; |
| D_8015F1D4 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:40: D_8015F1D4 = (u8 *)0x801BA000; |
| D_8015F1D8 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:41: D_8015F1D8 = (u8 *)0x801BE000; |
| D_8015F1DC | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:42: D_8015F1DC = (u8 *)0x801C2000; |
| D_8015F1E0 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:43: D_8015F1E0 = (u8 *)0x801C6000; |
| D_8015F1E4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:47: D_8015F1E4 = 0x801CE0F4; |
| D_8015F1E8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:48: D_8015F1E8 = 0x801CE0F8; |
| D_8015F1EC | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:49: D_8015F1EC = 0x801CE118; |
| D_8015F1F0 | aggregate/type/field/table | src/ovl/DATE/80152FC0.c:50: D_8015F1F0 = *(s16 *)0x801CE12C; |
| D_8015F1F4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:51: D_8015F1F4 = 0x801B0000; |
| D_8015F1F8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:52: D_8015F1F8 = 0x801B2000; |
| D_8015F1FC | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:53: D_8015F1FC = 0x801B6000; |
| D_8015F200 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:54: D_8015F200 = 0x801BA000; |
| D_8015F204 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:55: D_8015F204 = 0x801BE000; |
| D_8015F208 | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:56: D_8015F208 = 0x801C2000; |
| D_8015F20C | resource/address initializer; type/layout deferred | src/ovl/DATE/80152FC0.c:57: D_8015F20C = 0x801C6000; |
| D_8015F520 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:8: D_8015F520 = 0x801D2240; |
| D_8015F524 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:9: D_8015F524 = 0x801D2248; |
| D_8015F528 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:10: D_8015F528 = 0x801D2288; |
| D_8015F52C | aggregate/type/field/table | src/ovl/DATE/80154260.c:11: D_8015F52C = *(s16 *)0x801D22A0; |
| D_8015F530 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:12: D_8015F530 = 0x801B0000; |
| D_8015F534 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:13: D_8015F534 = 0x801B2000; |
| D_8015F538 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:14: D_8015F538 = 0x801B6000; |
| D_8015F53C | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:15: D_8015F53C = 0x801BA000; |
| D_8015F540 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:16: D_8015F540 = 0x801BE000; |
| D_8015F544 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:17: D_8015F544 = 0x801C2000; |
| D_8015F548 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:18: D_8015F548 = 0x801C6000; |
| D_8015F54C | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:22: D_8015F54C = 0x801D21E0; |
| D_8015F550 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:23: D_8015F550 = 0x801D21E8; |
| D_8015F554 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:24: D_8015F554 = 0x801D2228; |
| D_8015F558 | aggregate/type/field/table | src/ovl/DATE/80154260.c:25: D_8015F558 = *(s16 *)0x801D2240; |
| D_8015F55C | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:26: D_8015F55C = 0x801B0000; |
| D_8015F560 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:27: D_8015F560 = 0x801B2000; |
| D_8015F564 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:28: D_8015F564 = 0x801B6000; |
| D_8015F568 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:29: D_8015F568 = 0x801BA000; |
| D_8015F56C | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:30: D_8015F56C = 0x801BE000; |
| D_8015F570 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:31: D_8015F570 = 0x801C2000; |
| D_8015F574 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:32: D_8015F574 = 0x801C6000; |
| D_8015F578 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:36: D_8015F578 = 0x801CE078; |
| D_8015F57C | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:37: D_8015F57C = 0x801CE07C; |
| D_8015F580 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:38: D_8015F580 = 0x801CE08C; |
| D_8015F584 | aggregate/type/field/table | src/ovl/DATE/80154260.c:39: D_8015F584 = *(s16 *)0x801CE098; |
| D_8015F588 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:40: D_8015F588 = 0x801B0000; |
| D_8015F58C | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:41: D_8015F58C = 0x801B2000; |
| D_8015F590 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:42: D_8015F590 = 0x801B6000; |
| D_8015F594 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:43: D_8015F594 = 0x801BA000; |
| D_8015F598 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:44: D_8015F598 = 0x801BE000; |
| D_8015F59C | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:45: D_8015F59C = 0x801C2000; |
| D_8015F5A0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:46: D_8015F5A0 = 0x801C6000; |
| D_8015F5A4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:50: D_8015F5A4 = 0x801CE148; |
| D_8015F5A8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:51: D_8015F5A8 = 0x801CE14C; |
| D_8015F5AC | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:52: D_8015F5AC = 0x801CE170; |
| D_8015F5B0 | aggregate/type/field/table | src/ovl/DATE/80154260.c:53: D_8015F5B0 = *(s16 *)0x801CE184; |
| D_8015F5B4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:54: D_8015F5B4 = 0x801B0000; |
| D_8015F5B8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:55: D_8015F5B8 = 0x801B2000; |
| D_8015F5BC | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:56: D_8015F5BC = 0x801B6000; |
| D_8015F5C0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:57: D_8015F5C0 = 0x801BA000; |
| D_8015F5C4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:58: D_8015F5C4 = 0x801BE000; |
| D_8015F5C8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:59: D_8015F5C8 = 0x801C2000; |
| D_8015F5CC | resource/address initializer; type/layout deferred | src/ovl/DATE/80154260.c:60: D_8015F5CC = 0x801C6000; |
| D_8015F5D0 | aggregate/type/field/table | src/ovl/DATE/80154260.c:156: D_8015F5D0 = 0; |
| D_8015F900 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:8: D_8015F900 = 0x801CE080; |
| D_8015F904 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:9: D_8015F904 = 0x801CE084; |
| D_8015F908 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:10: D_8015F908 = 0x801CE094; |
| D_8015F90C | aggregate/type/field/table | src/ovl/DATE/801552B0.c:11: D_8015F90C = *(s16 *)0x801CE0A8; |
| D_8015F910 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:12: D_8015F910 = 0x801B0000; |
| D_8015F914 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:13: D_8015F914 = 0x801B2000; |
| D_8015F918 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:14: D_8015F918 = 0x801B6000; |
| D_8015F91C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:15: D_8015F91C = 0x801BA000; |
| D_8015F920 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:16: D_8015F920 = 0x801BE000; |
| D_8015F924 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:17: D_8015F924 = 0x801C2000; |
| D_8015F928 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:18: D_8015F928 = 0x801C6000; |
| D_8015F92C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:22: D_8015F92C = 0x801CE31C; |
| D_8015F930 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:23: D_8015F930 = 0x801CE320; |
| D_8015F934 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:24: D_8015F934 = 0x801CE39C; |
| D_8015F938 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:25: D_8015F938 = *(s16 *)0x801CE3BC; |
| D_8015F93C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:26: D_8015F93C = 0x801B0000; |
| D_8015F940 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:27: D_8015F940 = 0x801B2000; |
| D_8015F944 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:28: D_8015F944 = 0x801B6000; |
| D_8015F948 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:29: D_8015F948 = 0x801BA000; |
| D_8015F94C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:30: D_8015F94C = 0x801BE000; |
| D_8015F950 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:31: D_8015F950 = 0x801C2000; |
| D_8015F954 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:32: D_8015F954 = 0x801C6000; |
| D_8015F958 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:36: D_8015F958 = 0x801CE124; |
| D_8015F95C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:37: D_8015F95C = 0x801CE128; |
| D_8015F960 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:38: D_8015F960 = 0x801CE148; |
| D_8015F964 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:39: D_8015F964 = *(s16 *)0x801CE15C; |
| D_8015F968 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:40: D_8015F968 = 0x801B0000; |
| D_8015F96C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:41: D_8015F96C = 0x801B2000; |
| D_8015F970 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:42: D_8015F970 = 0x801B6000; |
| D_8015F974 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:43: D_8015F974 = 0x801BA000; |
| D_8015F978 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:44: D_8015F978 = 0x801BE000; |
| D_8015F97C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:45: D_8015F97C = 0x801C2000; |
| D_8015F980 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:46: D_8015F980 = 0x801C6000; |
| D_8015F984 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:50: D_8015F984 = 0x801B0000; |
| D_8015F988 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:51: D_8015F988 = 0x801B2000; |
| D_8015F98C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:52: D_8015F98C = 0x801B6000; |
| D_8015F990 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:53: D_8015F990 = 0x801BA000; |
| D_8015F994 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:54: D_8015F994 = 0x801BE000; |
| D_8015F998 | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:55: D_8015F998 = 0x801C2000; |
| D_8015F99C | resource/address initializer; type/layout deferred | src/ovl/DATE/801552B0.c:56: D_8015F99C = 0x801C6000; |
| D_8015F9A0 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:115: D_8015F9A0[i] = 0; |
| D_8015F9A8 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:122: D_8015F9A8[i] = rand() % 120; |
| D_8015F9B0 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:100: sp68 = D_8015F9B0; |
| D_8015F9C0 | aggregate/type/field/table | src/ovl/DATE/801552B0.c:101: sp58 = D_8015F9C0; |
| D_8015FB1C | aggregate/type/field/table | src/ovl/DATE/801552B0.c:280: sp98 = D_8015FB1C; |
| D_8015FB2C | aggregate/type/field/table | src/ovl/DATE/801552B0.c:281: sp88 = D_8015FB2C; |
| D_8015FB3C | aggregate/type/field/table | src/ovl/DATE/801552B0.c:282: sp68 = D_8015FB3C; |
| D_8015FCE0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:8: D_8015FCE0 = 0x801CE0DC; |
| D_8015FCE4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:9: D_8015FCE4 = 0x801CE0E0; |
| D_8015FCE8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:10: D_8015FCE8 = 0x801CE0F8; |
| D_8015FCEC | aggregate/type/field/table | src/ovl/DATE/80156600.c:11: D_8015FCEC = *(s16 *)0x801CE10C; |
| D_8015FCF0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:12: D_8015FCF0 = 0x801B0000; |
| D_8015FCF4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:13: D_8015FCF4 = 0x801B2000; |
| D_8015FCF8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:14: D_8015FCF8 = 0x801B6000; |
| D_8015FCFC | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:15: D_8015FCFC = 0x801BA000; |
| D_8015FD00 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:16: D_8015FD00 = 0x801BE000; |
| D_8015FD04 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:17: D_8015FD04 = 0x801C2000; |
| D_8015FD08 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:18: D_8015FD08 = 0x801C6000; |
| D_8015FD0C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:22: D_8015FD0C = 0x801D2100; |
| D_8015FD10 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:23: D_8015FD10 = 0x801D2108; |
| D_8015FD14 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:24: D_8015FD14 = 0x801D2128; |
| D_8015FD18 | aggregate/type/field/table | src/ovl/DATE/80156600.c:25: D_8015FD18 = *(s16 *)0x801D2144; |
| D_8015FD1C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:26: D_8015FD1C = 0x801B0000; |
| D_8015FD20 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:27: D_8015FD20 = 0x801B2000; |
| D_8015FD24 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:28: D_8015FD24 = 0x801B6000; |
| D_8015FD28 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:29: D_8015FD28 = 0x801BA000; |
| D_8015FD2C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:30: D_8015FD2C = 0x801BE000; |
| D_8015FD30 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:31: D_8015FD30 = 0x801C2000; |
| D_8015FD34 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:32: D_8015FD34 = 0x801C6000; |
| D_8015FD38 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:36: D_8015FD38 = 0x801D2240; |
| D_8015FD3C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:37: D_8015FD3C = 0x801D2248; |
| D_8015FD40 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:38: D_8015FD40 = 0x801D2288; |
| D_8015FD44 | aggregate/type/field/table | src/ovl/DATE/80156600.c:39: D_8015FD44 = *(s16 *)0x801D22A0; |
| D_8015FD48 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:40: D_8015FD48 = 0x801B0000; |
| D_8015FD4C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:41: D_8015FD4C = 0x801B2000; |
| D_8015FD50 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:42: D_8015FD50 = 0x801B6000; |
| D_8015FD54 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:43: D_8015FD54 = 0x801BA000; |
| D_8015FD58 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:44: D_8015FD58 = 0x801BE000; |
| D_8015FD5C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:45: D_8015FD5C = 0x801C2000; |
| D_8015FD60 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:46: D_8015FD60 = 0x801C6000; |
| D_8015FD64 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:50: D_8015FD64 = 0x801CE080; |
| D_8015FD68 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:51: D_8015FD68 = 0x801CE084; |
| D_8015FD6C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:52: D_8015FD6C = 0x801CE094; |
| D_8015FD70 | aggregate/type/field/table | src/ovl/DATE/80156600.c:53: D_8015FD70 = *(s16 *)0x801CE0A8; |
| D_8015FD74 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:54: D_8015FD74 = 0x801B0000; |
| D_8015FD78 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:55: D_8015FD78 = 0x801B2000; |
| D_8015FD7C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:56: D_8015FD7C = 0x801B6000; |
| D_8015FD80 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:57: D_8015FD80 = 0x801BA000; |
| D_8015FD84 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:58: D_8015FD84 = 0x801BE000; |
| D_8015FD88 | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:59: D_8015FD88 = 0x801C2000; |
| D_8015FD8C | resource/address initializer; type/layout deferred | src/ovl/DATE/80156600.c:60: D_8015FD8C = 0x801C6000; |
| D_801600A0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:8: D_801600A0 = 0x801CE090; |
| D_801600A4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:9: D_801600A4 = 0x801CE094; |
| D_801600A8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:10: D_801600A8 = 0x801CE0AC; |
| D_801600AC | aggregate/type/field/table | src/ovl/DATE/80157880.c:11: D_801600AC = *(s16 *)0x801CE0C0; |
| D_801600B0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:12: D_801600B0 = 0x801B0000; |
| D_801600B4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:13: D_801600B4 = 0x801B2000; |
| D_801600B8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:14: D_801600B8 = 0x801B6000; |
| D_801600BC | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:15: D_801600BC = 0x801BA000; |
| D_801600C0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:16: D_801600C0 = 0x801BE000; |
| D_801600C4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:17: D_801600C4 = 0x801C2000; |
| D_801600C8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:18: D_801600C8 = 0x801C6000; |
| D_801600CC | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:22: D_801600CC = 0x801D2240; |
| D_801600D0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:23: D_801600D0 = 0x801D2248; |
| D_801600D4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:24: D_801600D4 = 0x801D2288; |
| D_801600D8 | aggregate/type/field/table | src/ovl/DATE/80157880.c:25: D_801600D8 = *(s16 *)0x801D22A0; |
| D_801600DC | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:26: D_801600DC = 0x801B0000; |
| D_801600E0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:27: D_801600E0 = 0x801B2000; |
| D_801600E4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:28: D_801600E4 = 0x801B6000; |
| D_801600E8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:29: D_801600E8 = 0x801BA000; |
| D_801600EC | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:30: D_801600EC = 0x801BE000; |
| D_801600F0 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:31: D_801600F0 = 0x801C2000; |
| D_801600F4 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:32: D_801600F4 = 0x801C6000; |
| D_801600F8 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:36: D_801600F8 = 0x801CE078; |
| D_801600FC | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:37: D_801600FC = 0x801CE07C; |
| D_80160100 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:38: D_80160100 = 0x801CE08C; |
| D_80160104 | aggregate/type/field/table | src/ovl/DATE/80157880.c:39: D_80160104 = *(s16 *)0x801CE098; |
| D_80160108 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:40: D_80160108 = 0x801B0000; |
| D_8016010C | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:41: D_8016010C = 0x801B2000; |
| D_80160110 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:42: D_80160110 = 0x801B6000; |
| D_80160114 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:43: D_80160114 = 0x801BA000; |
| D_80160118 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:44: D_80160118 = 0x801BE000; |
| D_8016011C | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:45: D_8016011C = 0x801C2000; |
| D_80160120 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:46: D_80160120 = 0x801C6000; |
| D_80160124 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:50: D_80160124 = 0x801CE0F4; |
| D_80160128 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:51: D_80160128 = 0x801CE0F8; |
| D_8016012C | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:52: D_8016012C = 0x801CE118; |
| D_80160130 | aggregate/type/field/table | src/ovl/DATE/80157880.c:53: D_80160130 = *(s16 *)0x801CE12C; |
| D_80160134 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:54: D_80160134 = 0x801B0000; |
| D_80160138 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:55: D_80160138 = 0x801B2000; |
| D_8016013C | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:56: D_8016013C = 0x801B6000; |
| D_80160140 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:57: D_80160140 = 0x801BA000; |
| D_80160144 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:58: D_80160144 = 0x801BE000; |
| D_80160148 | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:59: D_80160148 = 0x801C2000; |
| D_8016014C | resource/address initializer; type/layout deferred | src/ovl/DATE/80157880.c:60: D_8016014C = 0x801C6000; |
| D_80160440 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:8: D_80160440 = 0x801CE124; |
| D_80160444 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:9: D_80160444 = 0x801CE128; |
| D_80160448 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:10: D_80160448 = 0x801CE148; |
| D_8016044C | aggregate/type/field/table | src/ovl/DATE/801586A0.c:11: D_8016044C = *(s16 *)0x801CE15C; |
| D_80160450 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:12: D_80160450 = 0x801B0000; |
| D_80160454 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:13: D_80160454 = 0x801B2000; |
| D_80160458 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:14: D_80160458 = 0x801B6000; |
| D_8016045C | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:15: D_8016045C = 0x801BA000; |
| D_80160460 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:16: D_80160460 = 0x801BE000; |
| D_80160464 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:17: D_80160464 = 0x801C2000; |
| D_80160468 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:18: D_80160468 = 0x801C6000; |
| D_8016046C | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:22: D_8016046C = 0x801D2240; |
| D_80160470 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:23: D_80160470 = 0x801D2248; |
| D_80160474 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:24: D_80160474 = 0x801D2288; |
| D_80160478 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:25: D_80160478 = *(s16 *)0x801D22A0; |
| D_8016047C | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:26: D_8016047C = 0x801B0000; |
| D_80160480 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:27: D_80160480 = 0x801B2000; |
| D_80160484 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:28: D_80160484 = 0x801B6000; |
| D_80160488 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:29: D_80160488 = 0x801BA000; |
| D_8016048C | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:30: D_8016048C = 0x801BE000; |
| D_80160490 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:31: D_80160490 = 0x801C2000; |
| D_80160494 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:32: D_80160494 = 0x801C6000; |
| D_80160498 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:36: D_80160498 = 0x801CE0F4; |
| D_8016049C | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:37: D_8016049C = 0x801CE0F8; |
| D_801604A0 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:38: D_801604A0 = 0x801CE118; |
| D_801604A4 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:39: D_801604A4 = *(s16 *)0x801CE12C; |
| D_801604A8 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:40: D_801604A8 = 0x801B0000; |
| D_801604AC | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:41: D_801604AC = 0x801B2000; |
| D_801604B0 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:42: D_801604B0 = 0x801B6000; |
| D_801604B4 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:43: D_801604B4 = 0x801BA000; |
| D_801604B8 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:44: D_801604B8 = 0x801BE000; |
| D_801604BC | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:45: D_801604BC = 0x801C2000; |
| D_801604C0 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:46: D_801604C0 = 0x801C6000; |
| D_801604C4 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:50: D_801604C4 = 0x801CE1A0; |
| D_801604C8 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:51: D_801604C8 = 0x801CE1A4; |
| D_801604CC | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:52: D_801604CC = 0x801CE1D4; |
| D_801604D0 | aggregate/type/field/table | src/ovl/DATE/801586A0.c:53: D_801604D0 = *(s16 *)0x801CE1F0; |
| D_801604D4 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:54: D_801604D4 = 0x801B0000; |
| D_801604D8 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:55: D_801604D8 = 0x801B2000; |
| D_801604DC | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:56: D_801604DC = 0x801B6000; |
| D_801604E0 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:57: D_801604E0 = 0x801BA000; |
| D_801604E4 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:58: D_801604E4 = 0x801BE000; |
| D_801604E8 | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:59: D_801604E8 = 0x801C2000; |
| D_801604EC | resource/address initializer; type/layout deferred | src/ovl/DATE/801586A0.c:60: D_801604EC = 0x801C6000; |
| D_801607E0 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:8: D_801607E0 = 0x801CE0F4; |
| D_801607E4 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:9: D_801607E4 = 0x801CE0F8; |
| D_801607E8 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:10: D_801607E8 = 0x801CE118; |
| D_801607EC | aggregate/type/field/table | src/ovl/DATE/801594B0.c:11: D_801607EC = *(s16 *)0x801CE12C; |
| D_801607F0 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:12: D_801607F0 = 0x801B0000; |
| D_801607F4 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:13: D_801607F4 = 0x801B2000; |
| D_801607F8 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:14: D_801607F8 = 0x801B6000; |
| D_801607FC | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:15: D_801607FC = 0x801BA000; |
| D_80160800 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:16: D_80160800 = 0x801BE000; |
| D_80160804 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:17: D_80160804 = 0x801C2000; |
| D_80160808 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:18: D_80160808 = 0x801C6000; |
| D_8016080C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:22: D_8016080C = 0x801CE0F4; |
| D_80160810 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:23: D_80160810 = 0x801CE0F8; |
| D_80160814 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:24: D_80160814 = 0x801CE118; |
| D_80160818 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:25: D_80160818 = *(s16 *)0x801CE12C; |
| D_8016081C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:26: D_8016081C = 0x801B0000; |
| D_80160820 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:27: D_80160820 = 0x801B2000; |
| D_80160824 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:28: D_80160824 = 0x801B6000; |
| D_80160828 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:29: D_80160828 = 0x801BA000; |
| D_8016082C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:30: D_8016082C = 0x801BE000; |
| D_80160830 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:31: D_80160830 = 0x801C2000; |
| D_80160834 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:32: D_80160834 = 0x801C6000; |
| D_80160838 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:36: D_80160838 = 0x801CE0F4; |
| D_8016083C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:37: D_8016083C = 0x801CE0F8; |
| D_80160840 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:38: D_80160840 = 0x801CE118; |
| D_80160844 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:39: D_80160844 = *(s16 *)0x801CE12C; |
| D_80160848 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:40: D_80160848 = 0x801B0000; |
| D_8016084C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:41: D_8016084C = 0x801B2000; |
| D_80160850 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:42: D_80160850 = 0x801B6000; |
| D_80160854 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:43: D_80160854 = 0x801BA000; |
| D_80160858 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:44: D_80160858 = 0x801BE000; |
| D_8016085C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:45: D_8016085C = 0x801C2000; |
| D_80160860 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:46: D_80160860 = 0x801C6000; |
| D_80160864 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:50: D_80160864 = 0x801D2470; |
| D_80160868 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:51: D_80160868 = 0x801D2478; |
| D_8016086C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:52: D_8016086C = 0x801D24B8; |
| D_80160870 | aggregate/type/field/table | src/ovl/DATE/801594B0.c:53: D_80160870 = *(s16 *)0x801D24D0; |
| D_80160874 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:54: D_80160874 = 0x801B0000; |
| D_80160878 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:55: D_80160878 = 0x801B2000; |
| D_8016087C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:56: D_8016087C = 0x801B6000; |
| D_80160880 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:57: D_80160880 = 0x801BA000; |
| D_80160884 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:58: D_80160884 = 0x801BE000; |
| D_80160888 | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:59: D_80160888 = 0x801C2000; |
| D_8016088C | resource/address initializer; type/layout deferred | src/ovl/DATE/801594B0.c:60: D_8016088C = 0x801C6000; |

### DATE assembly-only label inventory

These 107 D labels appear in DATE assembly but not in DATE C/header references. The source evidence names the assembly file and line; their values remain out of this spec.

| Identifier | Assembly evidence |
|---|---|
| D_8015AB20 | asm/ovl/DATE/matchings/DATE/80132000/func_80133DE8.s:22 |
| D_8015AB2C | asm/ovl/DATE/matchings/DATE/80132000/func_80133EA0.s:20 |
| D_8015AB40 | asm/ovl/DATE/matchings/DATE/80132000/func_80133EA0.s:26 |
| D_8015AB48 | asm/ovl/DATE/matchings/DATE/80132000/func_80133EA0.s:83 |
| D_8015AB50 | asm/ovl/DATE/matchings/DATE/80132000/func_80133EA0.s:122 |
| D_8015AB54 | asm/ovl/DATE/matchings/DATE/80132000/func_80133EA0.s:169 |
| D_8015AB58 | asm/ovl/DATE/matchings/DATE/80132000/func_8013426C.s:18 |
| D_8015AB60 | asm/ovl/DATE/matchings/DATE/80132000/func_8013426C.s:52 |
| D_8015AB70 | asm/ovl/DATE/matchings/DATE/80132000/func_8013426C.s:69 |
| D_8015AB78 | asm/ovl/DATE/matchings/DATE/80132000/func_8013426C.s:80 |
| D_8015AB80 | asm/ovl/DATE/matchings/DATE/80132000/func_801344C8.s:18 |
| D_8015AB88 | asm/ovl/DATE/matchings/DATE/80132000/func_801344C8.s:52 |
| D_8015AB90 | asm/ovl/DATE/matchings/DATE/80132000/func_801344C8.s:69 |
| D_8015AB98 | asm/ovl/DATE/matchings/DATE/80132000/func_801344C8.s:80 |
| D_8015ABA0 | asm/ovl/DATE/matchings/DATE/80132000/func_801344C8.s:96 |
| D_8015ABA8 | asm/ovl/DATE/matchings/DATE/80132000/func_801347B8.s:18 |
| D_8015ABB0 | asm/ovl/DATE/matchings/DATE/80132000/func_801347B8.s:24 |
| D_8015ABB8 | asm/ovl/DATE/matchings/DATE/80132000/func_801349A0.s:18 |
| D_8015ABC8 | asm/ovl/DATE/matchings/DATE/80132000/func_801349A0.s:38 |
| D_8015ABD0 | asm/ovl/DATE/matchings/DATE/80132000/func_801349A0.s:49 |
| D_8015ABD8 | asm/ovl/DATE/matchings/DATE/80132000/func_80134C64.s:18 |
| D_8015ABE0 | asm/ovl/DATE/matchings/DATE/80132000/func_80134C64.s:49 |
| D_8015ABEC | asm/ovl/DATE/matchings/DATE/80132000/func_80134C64.s:94 |
| D_8015ABF4 | asm/ovl/DATE/matchings/DATE/80132000/func_80134C64.s:137 |
| D_8015AC00 | asm/ovl/DATE/matchings/DATE/80132000/func_80134C64.s:180 |
| D_8015AC08 | asm/ovl/DATE/matchings/DATE/80132000/func_80134C64.s:192 |
| D_8015AC10 | asm/ovl/DATE/matchings/DATE/80132000/func_8013518C.s:18 |
| D_8015AC18 | asm/ovl/DATE/matchings/DATE/80132000/func_8013518C.s:38 |
| D_8015AC24 | asm/ovl/DATE/matchings/DATE/80132000/func_8013518C.s:51 |
| D_8015AC30 | asm/ovl/DATE/matchings/DATE/80132000/func_8013518C.s:86 |
| D_8015AC3C | asm/ovl/DATE/matchings/DATE/80132000/func_801353F8.s:18 |
| D_8015AC4C | asm/ovl/DATE/matchings/DATE/80132000/func_801353F8.s:37 |
| D_8015AC64 | asm/ovl/DATE/matchings/DATE/80132000/func_801353F8.s:48 |
| D_8015AC7C | asm/ovl/DATE/matchings/DATE/80132000/func_80135620.s:18 |
| D_8015AC8C | asm/ovl/DATE/matchings/DATE/80132000/func_80135620.s:38 |
| D_8015AC9C | asm/ovl/DATE/matchings/DATE/80132000/func_80135620.s:49 |
| D_8015ACAC | asm/ovl/DATE/matchings/DATE/80132000/func_80135804.s:18 |
| D_8015ACB8 | asm/ovl/DATE/matchings/DATE/80132000/func_80135804.s:47 |
| D_8015ACC4 | asm/ovl/DATE/matchings/DATE/80132000/func_80135804.s:37 |
| D_8015ACD0 | asm/ovl/DATE/matchings/DATE/80132000/func_80135A34.s:64 |
| D_8015ACD8 | asm/ovl/DATE/matchings/DATE/80132000/func_80135B4C.s:20 |
| D_8015ACE8 | asm/ovl/DATE/matchings/DATE/80132000/func_80135C14.s:18 |
| D_8015ACF4 | asm/ovl/DATE/matchings/DATE/80132000/func_80135C14.s:24 |
| D_8015AD04 | asm/ovl/DATE/matchings/DATE/80132000/func_80135E7C.s:147 |
| D_8015AD0C | asm/ovl/DATE/matchings/DATE/80132000/func_80135E7C.s:153 |
| D_8015AD14 | asm/ovl/DATE/matchings/DATE/80132000/func_80136340.s:156 |
| D_8015AD24 | asm/ovl/DATE/matchings/DATE/80132000/func_80136340.s:162 |
| D_8015AD2C | asm/ovl/DATE/matchings/DATE/80132000/func_801366A4.s:21 |
| D_8015AD34 | asm/ovl/DATE/matchings/DATE/80132000/func_801366A4.s:27 |
| D_8015AD40 | asm/ovl/DATE/matchings/DATE/80132000/func_80136870.s:29 |
| D_8015AD48 | asm/ovl/DATE/matchings/DATE/80132000/func_80136870.s:34 |
| D_8015AD50 | asm/ovl/DATE/matchings/DATE/80132000/func_80136A74.s:23 |
| D_8015AD58 | asm/ovl/DATE/matchings/DATE/80132000/func_80136A74.s:29 |
| D_8015AD64 | asm/ovl/DATE/matchings/DATE/80132000/func_80136B70.s:21 |
| D_8015AD70 | asm/ovl/DATE/matchings/DATE/80132000/func_80136B70.s:27 |
| D_8015AD80 | asm/ovl/DATE/matchings/DATE/80132000/func_80136D3C.s:21 |
| D_8015AD8C | asm/ovl/DATE/matchings/DATE/80132000/func_80136D3C.s:27 |
| D_8015AD98 | asm/ovl/DATE/matchings/DATE/80132000/func_80137154.s:12 |
| D_8015ADA4 | asm/ovl/DATE/matchings/DATE/80132000/func_80137154.s:145 |
| D_8015ADB8 | asm/ovl/DATE/matchings/DATE/80132000/func_80137154.s:316 |
| D_8015ADC4 | asm/ovl/DATE/matchings/DATE/80132000/func_80137D1C.s:9 |
| D_8015ADD4 | asm/ovl/DATE/matchings/DATE/80132000/func_80137FAC.s:7 |
| D_8015ADE4 | asm/ovl/DATE/matchings/DATE/80132000/func_8013808C.s:17 |
| D_8015ADEC | asm/ovl/DATE/matchings/DATE/80132000/func_801387B8.s:7 |
| D_8015ADF4 | asm/ovl/DATE/matchings/DATE/80132000/func_80138934.s:7 |
| D_8015ADFC | asm/ovl/DATE/matchings/DATE/80132000/func_80138934.s:16 |
| D_8015AE10 | asm/ovl/DATE/matchings/DATE/80132000/func_80138934.s:103 |
| D_8015AE28 | asm/ovl/DATE/matchings/DATE/80132000/func_80138934.s:191 |
| D_8015AE38 | asm/ovl/DATE/matchings/DATE/80132000/func_80138934.s:280 |
| D_8015AE4C | asm/ovl/DATE/matchings/DATE/80132000/func_80138934.s:365 |
| D_8015AE54 | asm/ovl/DATE/matchings/DATE/80132000/func_801393DC.s:24 |
| D_8015AE6C | asm/ovl/DATE/matchings/DATE/80132000/func_801393DC.s:33 |
| D_8015AE84 | asm/ovl/DATE/matchings/DATE/80132000/func_80139D9C.s:7 |
| D_8015AE90 | asm/ovl/DATE/matchings/DATE/80132000/func_8013C190.s:61 |
| D_8015AEA0 | asm/ovl/DATE/matchings/DATE/80132000/func_8013CADC.s:46 |
| D_8015AEA8 | asm/ovl/DATE/matchings/DATE/80132000/func_80140ECC.s:8 |
| D_8015B140 | asm/ovl/DATE/matchings/DATE/801461B0/func_801462B0.s:573 |
| D_8015B158 | asm/ovl/DATE/matchings/DATE/801461B0/func_8014718C.s:350 |
| D_8015B170 | asm/ovl/DATE/matchings/DATE/801461B0/func_80147958.s:980 |
| D_8015B188 | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:176 |
| D_8015B1A0 | asm/ovl/DATE/matchings/DATE/801461B0/func_801491E4.s:15 |
| D_8015B1A8 | asm/ovl/DATE/matchings/DATE/801461B0/func_801491E4.s:26 |
| D_8015B360 | asm/ovl/DATE/matchings/DATE/80149310/func_80149B78.s:5 |
| D_8015B440 | asm/ovl/DATE/matchings/DATE/8014CC40/func_8014D298.s:7 |
| D_8015B470 | asm/ovl/DATE/matchings/DATE/8014F0F0/func_8014FE5C.s:7 |
| D_8015B480 | asm/ovl/DATE/matchings/DATE/80150010/func_80150344.s:21 |
| D_8015B490 | asm/ovl/DATE/matchings/DATE/80152FC0/func_801535D8.s:21 |
| D_8015B498 | asm/ovl/DATE/matchings/DATE/80152FC0/func_801535D8.s:27 |
| D_8015B4C0 | asm/ovl/DATE/matchings/DATE/80154260/func_80154E14.s:7 |
| D_8015B4E0 | asm/ovl/DATE/matchings/DATE/80156600/func_80156F10.s:7 |
| D_8015B4F0 | asm/ovl/DATE/matchings/DATE/80157880/func_80157BD4.s:18 |
| D_8015B500 | asm/ovl/DATE/matchings/DATE/801594B0/func_8015996C.s:18 |
| D_8015B510 | asm/ovl/DATE/matchings/DATE/8015A190/func_8015A414.s:11 |
| D_8015CFA6 | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:42 |
| D_8015CFA8 | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:43 |
| D_8015CFAE | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:48 |
| D_8015CFB0 | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:50 |
| D_8015CFB6 | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:87 |
| D_8015CFB8 | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:104 |
| D_8015CFBE | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:138 |
| D_8015CFC0 | asm/ovl/DATE/matchings/DATE/801461B0/func_801489D0.s:155 |
| D_8015DDD2 | asm/ovl/DATE/matchings/DATE/8014A1B0/func_8014A810.s:25 |
| D_8015DDD4 | asm/ovl/DATE/matchings/DATE/8014A1B0/func_8014A810.s:24 |
| D_8015DDD8 | asm/ovl/DATE/matchings/DATE/8014A1B0/func_8014A810.s:31 |
| D_8015DDDA | asm/ovl/DATE/matchings/DATE/8014A1B0/func_8014A810.s:38 |
| D_8015DDDC | asm/ovl/DATE/matchings/DATE/8014A1B0/func_8014A810.s:37 |
| D_8015E510 | asm/ovl/DATE/matchings/DATE/8014CC40/func_8014E3E8.s:5 |

## 17. DATE logical file and topic map

Historical DATE file/header layout proposal, superseded by the accepted applied T-27074 path map and `config/header_owners.json`. The source units now use their logical current paths; retain this table only as the earlier topic-evidence inventory.

| Current source | Proposed logical file | Topic / source context |
|---|---|---|
| src/ovl/DATE/80132000.c | src/ovl/date/place_scenes.c | stage 0 and 1 handlers of all 21 places: place setups, arrival, choice scoring, special-scene chooser, date end |
| src/ovl/DATE/80141EE0.c | src/ovl/date/entry.c | 11 per-girl script section fillers, mode 0x50 entry and the stage 0 and stage 1 dispatchers |
| src/ovl/DATE/80145AA0.c | src/ovl/date/player_birthday.c | stage 3: the girl greets the player on his birthday |
| src/ovl/DATE/801461B0.c | src/ovl/date/rides.c | amusement park rides: roller coaster, ghost house, Ferris wheel, scream machine (virtual ship is separate) |
| src/ovl/DATE/80149310.c | src/ovl/date/girl_birthday.c | stage 2: gift choice on the girl's birthday |
| src/ovl/DATE/8014A1B0.c | src/ovl/date/virtual_ship.c | virtual ship ride (3D, GTE) |
| src/ovl/DATE/8014B1F0.c | src/ovl/date/scene_common.c | scene section pointer init, mode 0x5E entry, shared scene steps and the scene end |
| src/ovl/DATE/8014CC40.c | src/ovl/date/scene_yuko.c | mode 0x5E scenes of Yuko (girl 8) with the shared gradient quad drawer |
| src/ovl/DATE/8014F0F0.c | src/ovl/date/scene_shiori.c | mode 0x5E scenes of Shiori (girl 0) |
| src/ovl/DATE/80150010.c | src/ovl/date/scene_yuina.c | mode 0x5E scenes of Yuina (girl 2) |
| src/ovl/DATE/80151460.c | src/ovl/date/scene_ayako.c | mode 0x5E scenes of Ayako (girl 3) with the gradient triangle drawer |
| src/ovl/DATE/80152FC0.c | src/ovl/date/scene_mira.c | mode 0x5E scenes of Mira (girl 7) |
| src/ovl/DATE/80154260.c | src/ovl/date/scene_mio.c | mode 0x5E scenes of Mio (girl 1) |
| src/ovl/DATE/801552B0.c | src/ovl/date/scene_yukari.c | mode 0x5E scenes of Yukari (girl 5) |
| src/ovl/DATE/80156600.c | src/ovl/date/scene_nozomi.c | mode 0x5E scenes of Nozomi (girl 6) |
| src/ovl/DATE/80157880.c | src/ovl/date/scene_megumi.c | mode 0x5E scenes of Megumi (girl 9) |
| src/ovl/DATE/801586A0.c | src/ovl/date/scene_saki.c | mode 0x5E scenes of Saki (girl 4) |
| src/ovl/DATE/801594B0.c | src/ovl/date/scene_yumi.c | main Yumi mode 0x5E scenes (girl 10); seven continuation definitions remain in the next object |
| src/ovl/DATE/8015A190.c | src/ovl/date/scene_yumi_miharu.c | seven Yumi continuation definitions at lines 7-49, then Miharu starts at line 55; keep this mixed object boundary |
| include/ovl/DATE.h | include/ovl/date.h | overlay header: types, externs and prototypes; new name follows the folder |

The proposed topic header is include/ovl/date.h, from include/ovl/DATE.h; it is a compatibility umbrella over the 17 topic owners below. The file map has 19 C objects and this one umbrella. No source/header move is included in the function/data batch.


### Proposed topic-header owners

The compatibility umbrella include/ovl/date.h includes the owners below and does not redeclare them. Each overlay declaration has one topic owner; shared GameState, GirlRecord, TalkWork, main services, and main-owned variables stay in their existing shared headers. Keep function tables and local callback types in their owning C objects unless header closure requires cross-object visibility.

| Proposed topic header | Declaration owner and consumers | Source evidence |
|---|---|---|
| include/ovl/date/place_scenes.h | Common arrival/place/spot/scoring/event-trigger/date-end prototypes and DATE-local table/resource externs used across place handlers. | src/ovl/DATE/80132000.c:244-5168 |
| include/ovl/date/entry.h | Mode 0x50 entry, stage dispatchers, and per-girl script-section fillers. | src/ovl/DATE/80141EE0.c:4-850 |
| include/ovl/date/rides.h | Amusement-park ride handlers and ride callback prototypes. | src/ovl/DATE/801461B0.c:20-1242 |
| include/ovl/date/virtual_ship.h | Virtual-ship ride callbacks and GTE-backed state. | src/ovl/DATE/8014A1B0.c:24-395 |
| include/ovl/date/birthdays.h | Girl-birthday and player-birthday stage handlers, loaders, and callback declarations. | src/ovl/DATE/80149310.c and src/ovl/DATE/80145AA0.c |
| include/ovl/date/scene_common.h | Mode 0x5E handler, scene-section pointers, shared flow/tail callbacks, and shared scene state. | src/ovl/DATE/8014B1F0.c:6-480 |
| include/ovl/date/scenes/yuko.h | Yuko scene entry points and private callbacks/resources; consumed by the common scene dispatcher. | src/ovl/DATE/8014CC40.c |
| include/ovl/date/scenes/shiori.h | Shiori scene entry points and private callbacks/resources. | src/ovl/DATE/8014F0F0.c |
| include/ovl/date/scenes/yuina.h | Yuina scene entry points and private callbacks/resources. | src/ovl/DATE/80150010.c |
| include/ovl/date/scenes/ayako.h | Ayako scene entry points and private callbacks/resources. | src/ovl/DATE/80151460.c |
| include/ovl/date/scenes/mira.h | Mira scene entry points and private callbacks/resources. | src/ovl/DATE/80152FC0.c |
| include/ovl/date/scenes/mio.h | Mio scene entry points and private callbacks/resources. | src/ovl/DATE/80154260.c |
| include/ovl/date/scenes/yukari.h | Yukari scene entry points and private callbacks/resources. | src/ovl/DATE/801552B0.c |
| include/ovl/date/scenes/nozomi.h | Nozomi scene entry points and private callbacks/resources. | src/ovl/DATE/80156600.c |
| include/ovl/date/scenes/megumi.h | Megumi scene entry points and private callbacks/resources. | src/ovl/DATE/80157880.c |
| include/ovl/date/scenes/saki.h | Saki scene entry points and private callbacks/resources. | src/ovl/DATE/801586A0.c |
| include/ovl/date/scenes/yumi_miharu.h | Yumi continuation and Miharu intrusion entry points and private callbacks/resources from their shared C object. | src/ovl/DATE/801594B0.c and src/ovl/DATE/8015A190.c:7-112 |

The previous proposal counted 17 topic headers plus a compatibility umbrella. It is not the current declaration map: use `config/header_owners.json` and current `include/ovl/date.h` ownership. The file/header migration is already applied; this historical table does not authorize a new split.

## 18. DATE constants inventory

The 158 proposed constants cover stage and arrival IDs, greeting and meeting groups, response classes, ride/shop/spot IDs, special-day IDs, script sections, and mode-0x5E scene IDs. Their numeric values are code IDs or table indices as described; they do not change consumer field widths or signedness. Keep unclear semantics marked HYPOTHESIS, and leave field widths to existing source declarations and consumers. Context columns identify source sites that consume each family.

| Family | Value | Proposed name | Meaning / source context |
|---|---:|---|---|
| DateModeStage | 0 | DATE_MODE_STAGE_ARRIVAL | meeting at the place: loads, arrival, greeting; ends with main_stage_next; src/ovl/DATE/80141EE0.c:717-850, mode/stage dispatch and handlers |
| DateModeStage | 1 | DATE_MODE_STAGE_PLACE | the date at the place: talk, choice, reaction, farewell; ends with date_end; src/ovl/DATE/80141EE0.c:717-850, mode/stage dispatch and handlers |
| DateModeStage | 2 | DATE_MODE_STAGE_GIRL_BIRTHDAY | the girl's birthday: gift choice (entered from date_birthday_check); src/ovl/DATE/80141EE0.c:717-850, mode/stage dispatch and handlers |
| DateModeStage | 3 | DATE_MODE_STAGE_PLAYER_BIRTHDAY | the player's birthday: the girl's greeting; src/ovl/DATE/80141EE0.c:717-850, mode/stage dispatch and handlers |
| DateArrival | 0 | DATE_ARRIVAL_GIRL_LATE | HYPOTHESIS: late = she appears only after a waiting talk; set at the start with chance 1/4 (frame_count & 3 == 0), never for Yuina; src/ovl/DATE/80132000.c:336, arrival initialization |
| DateArrival | 1 | DATE_ARRIVAL_ON_TIME | default; src/ovl/DATE/80132000.c:336, arrival initialization |
| DateArrival | 2 | DATE_ARRIVAL_PLAYER_LATE | HYPOTHESIS: late = the player first went to another place and corrected it (the code only shows the correction); src/ovl/DATE/80132000.c:336, arrival initialization |
| DateResponseClass | 0 | DATE_CLASS_BEST | tokimeki +3, affection +2 (times multipliers / 16), syoushin -10 weight; src/ovl/DATE/80132000.c:3425, choice scoring; line 376, class derivation |
| DateResponseClass | 1 | DATE_CLASS_GOOD | tokimeki +2, affection +1, syoushin weight -8; src/ovl/DATE/80132000.c:3425, choice scoring; line 376, class derivation |
| DateResponseClass | 2 | DATE_CLASS_FAIR | tokimeki +1, affection 0, syoushin weight -6; src/ovl/DATE/80132000.c:3425, choice scoring; line 376, class derivation |
| DateResponseClass | 3 | DATE_CLASS_POOR | tokimeki 0, affection -1, syoushin weight +8; src/ovl/DATE/80132000.c:3425, choice scoring; line 376, class derivation |
| DateResponseClass | 4 | DATE_CLASS_WORST | tokimeki -1, affection -2, syoushin weight +10; src/ovl/DATE/80132000.c:3425, choice scoring; line 376, class derivation |
| DateRide | 0 | DATE_RIDE_ROLLER_COASTER | amusement park ride id (date_ride_index); src/ovl/DATE/80132000.c:1570-2000, park ride selection and handlers |
| DateRide | 1 | DATE_RIDE_GHOST_HOUSE | amusement park ride id (date_ride_index); src/ovl/DATE/80132000.c:1570-2000, park ride selection and handlers |
| DateRide | 2 | DATE_RIDE_FERRIS_WHEEL | amusement park ride id (date_ride_index); src/ovl/DATE/80132000.c:1570-2000, park ride selection and handlers |
| DateRide | 3 | DATE_RIDE_SCREAM_MACHINE | amusement park ride id (date_ride_index); src/ovl/DATE/80132000.c:1570-2000, park ride selection and handlers |
| DateRide | 4 | DATE_RIDE_VIRTUAL_SHIP | amusement park ride id (date_ride_index); src/ovl/DATE/80132000.c:1570-2000, park ride selection and handlers |
| DateShop | 0 | DATE_SHOP_BOUTIQUE | shopping street shop id (date_shop_index); src/ovl/DATE/80132000.c:1000-1300, shopping street setup and choices |
| DateShop | 1 | DATE_SHOP_FANCY_SHOP | shopping street shop id (date_shop_index); src/ovl/DATE/80132000.c:1000-1300, shopping street setup and choices |
| DateShop | 2 | DATE_SHOP_JUNK_SHOP | shopping street shop id (date_shop_index); src/ovl/DATE/80132000.c:1000-1300, shopping street setup and choices |
| DateSpot | 1 | DATE_SPOT_CENTRAL_PARK | date_spot value; scene_counters index and spot id of the park (sub-spots are logged as 0x25-0x27); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 2 | DATE_SPOT_BOUTIQUE | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 3 | DATE_SPOT_FANCY_SHOP | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 4 | DATE_SPOT_JUNK_SHOP | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 5 | DATE_SPOT_AQUARIUM | date_spot value; inside the aquarium; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 6 | DATE_SPOT_DOLPHIN_SHOW | date_spot value; aquarium special, from March 1996, once; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 7 | DATE_SPOT_ZOO | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 8 | DATE_SPOT_ZOO_SPECIAL | HYPOTHESIS: name; zoo special variant, once, from December 1995; stored as spot 7 in girl_last_date_spot (date_spot value); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 9 | DATE_SPOT_BOTANICAL_GARDEN | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 10 | DATE_SPOT_PLANETARIUM | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 11 | DATE_SPOT_ART_MUSEUM_HALL | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 12 | DATE_SPOT_EXHIBIT_ARTIST | date_spot value; autumn 1997 exhibition; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 13 | DATE_SPOT_EXHIBIT_SCULPTURE | date_spot value; spring 1997 exhibition; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 14 | DATE_SPOT_EXHIBIT_PAINTERS | date_spot value; autumn 1995 exhibition; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 15 | DATE_SPOT_LIBRARY | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 16 | DATE_SPOT_VIDEO_GAMES | date_spot value; game center, video corner; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 17 | DATE_SPOT_MEDAL_GAMES | date_spot value; game center, medal corner; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 18 | DATE_SPOT_BOWLING_ALLEY | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 19 | DATE_SPOT_KARAOKE | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 20 | DATE_SPOT_HERO_SHOW | date_spot value; amusement park; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 21 | DATE_SPOT_ROLLER_COASTER | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 22 | DATE_SPOT_GHOST_HOUSE | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 23 | DATE_SPOT_FERRIS_WHEEL | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 24 | DATE_SPOT_SCREAM_MACHINE | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 25 | DATE_SPOT_VIRTUAL_SHIP | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 26 | DATE_SPOT_NIGHT_PARADE | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 27 | DATE_SPOT_STADIUM | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 28 | DATE_SPOT_STADIUM_GAME | date_spot value; months 1, 3, 6, 11; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 29 | DATE_SPOT_POOL | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 30 | DATE_SPOT_BEACH | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 31 | DATE_SPOT_SKATING_RINK | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 32 | DATE_SPOT_SKI_SLOPE | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 33 | DATE_SPOT_NEIGHBORHOOD_PARK | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 34 | DATE_SPOT_SHRINE_FESTIVAL | date_spot value; festival stalls and fireworks; src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 35 | DATE_SPOT_MOVIE_THEATER | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 36 | DATE_SPOT_CONCERT_HALL | date_spot value (index of GirlRecord.scene_counters); src/ovl/DATE/80132000.c:510-2000, place setup and spot selection |
| DateSpot | 37 | DATE_SPOT_CENTRAL_PARK_PATH | event_log_add scene argument for central park path (variant 0); not date_spot or scene_counters; src/ovl/DATE/80132000.c event_log_add call sites |
| DateSpot | 38 | DATE_SPOT_CENTRAL_PARK_POND | event_log_add scene argument for central park pond (variant 1); not date_spot or scene_counters; src/ovl/DATE/80132000.c event_log_add call sites |
| DateSpot | 39 | DATE_SPOT_CENTRAL_PARK_BLOSSOM_PATH | event_log_add scene argument for central park blossom path (variant 2); not date_spot or scene_counters; src/ovl/DATE/80132000.c event_log_add call sites |
| DateSpot | 40 | DATE_SPOT_DESERTED_ISLAND | event_log_add scene argument for the sea special; not date_spot or scene_counters; src/ovl/DATE/80132000.c event_log_add call sites |
| DateSection | 0 | DATE_SECTION_ART_MUSEUM | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 1 | DATE_SECTION_BOWLING_ALLEY | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 2 | DATE_SECTION_CONCERT_HALL | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 3 | DATE_SECTION_ZOO | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 4 | DATE_SECTION_SHRINE | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 5 | DATE_SECTION_GAME_CENTER | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 6 | DATE_SECTION_KARAOKE | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 7 | DATE_SECTION_NEIGHBORHOOD_PARK | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 8 | DATE_SECTION_MOVIE_THEATER | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 9 | DATE_SECTION_AMUSEMENT_PARK_PARADE | index into date_section_text/command/voice of the girl's script file; spoken by date_amusement_park_talk_unshuffled at the steps around the night parade (0x3D, 0x48, 0x4A), cursors date_park_parade_group/line; the text was not read; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 10 | DATE_SECTION_POOL | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 11 | DATE_SECTION_PLANETARIUM | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 12 | DATE_SECTION_SEA | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 13 | DATE_SECTION_BOTANICAL_GARDEN | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 14 | DATE_SECTION_SHOPPING_STREET | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 15 | DATE_SECTION_SKATING_RINK | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 16 | DATE_SECTION_SKI_RESORT | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 17 | DATE_SECTION_STADIUM | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 18 | DATE_SECTION_AQUARIUM | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 19 | DATE_SECTION_LIBRARY | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 20 | DATE_SECTION_AMUSEMENT_PARK | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 21 | DATE_SECTION_CENTRAL_PARK | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 22 | DATE_SECTION_FAREWELL | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 23 | DATE_SECTION_ARRIVAL_GREETING | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 24 | DATE_SECTION_SPECIAL_DAY_GREETING | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 25 | DATE_SECTION_NEIGHBORHOOD_PARK_OPENING | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSection | 26 | DATE_SECTION_CLOSING | index into date_section_text/command/voice of the girl's script file; src/ovl/DATE/80141EE0.c, per-girl section setup and date entry |
| DateSharedSection | 0 | DATE_SHARED_CONCERT_REACTION | group = programme index; src/ovl/DATE/80141EE0.c, shared section fillers |
| DateSharedSection | 1 | DATE_SHARED_MEETING_PLACE | groups 0-8, date_meeting_group; src/ovl/DATE/80141EE0.c, shared section fillers |
| DateSharedSection | 2 | DATE_SHARED_CHOICE_REACTION | line = response class; src/ovl/DATE/80141EE0.c, shared section fillers |
| DateSharedSection | 3 | DATE_SHARED_FILM_REACTION | group = film index; src/ovl/DATE/80141EE0.c, shared section fillers |
| DateGreeting | 0 | DATE_GREETING_LATE_TIER_0 | girl late, mood tier 0 (rank 0-1) (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 2 | DATE_GREETING_LATE_TIER_1 | girl late, mood tier 1 (rank 2) (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 4 | DATE_GREETING_LATE_TIER_2 | girl late, mood tier 2 (rank 3-4) (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 6 | DATE_GREETING_ON_TIME_TIER_0 | on time, tier 0 (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 7 | DATE_GREETING_ON_TIME_TIER_1 | on time, tier 1 (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 8 | DATE_GREETING_ON_TIME_TIER_2 | on time, tier 2 (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 9 | DATE_GREETING_PLAYER_LATE_TIER_0 | player late, tier 0 (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 10 | DATE_GREETING_PLAYER_LATE_TIER_1 | player late, tier 1 (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 11 | DATE_GREETING_PLAYER_LATE_TIER_2 | player late, tier 2 (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateGreeting | 12 | DATE_GREETING_BAD_LOOKS | looks below the threshold (group of DATE_SECTION_ARRIVAL_GREETING, variable date_arrival_group); src/ovl/DATE/80141EE0.c, stage-0 greeting/section fillers |
| DateMeetingGroup | 0 | DATE_MEETING_ASK_TWO | is it here? with two candidate places; src/ovl/DATE/80132000.c:336, arrival initialization |
| DateMeetingGroup | 3 | DATE_MEETING_ASK_THREE | same question with three candidates; src/ovl/DATE/80132000.c:336, arrival initialization |
| DateMeetingGroup | 7 | DATE_MEETING_NOT_HERE | she is not at this place; src/ovl/DATE/80132000.c:336, arrival initialization |
| DateMeetingGroup | 8 | DATE_MEETING_REFUSES | mood rank 4: she does not come; src/ovl/DATE/80132000.c:336, arrival initialization |
| DateSpecialDay | 0 | DATE_SPECIAL_DAY_EQUINOX_SPRING | HYPOTHESIS: equinox days (the code only compares the date); March 20/21; src/ovl/DATE/80132000.c:336-400 and place setup seasonal branches |
| DateSpecialDay | 1 | DATE_SPECIAL_DAY_SUMMER_SUNDAY | 1995-07-23, 1996-07-28, 1997-07-27 (Sundays in late July; the same dates turn on the light effect; spec section 2 has the festival-day guess); src/ovl/DATE/80132000.c:336-400 and place setup seasonal branches |
| DateSpecialDay | 2 | DATE_SPECIAL_DAY_EQUINOX_AUTUMN_A | HYPOTHESIS: equinox days (the code only compares the date); September 22/23, frame count odd; src/ovl/DATE/80132000.c:336-400 and place setup seasonal branches |
| DateSpecialDay | 3 | DATE_SPECIAL_DAY_EQUINOX_AUTUMN_B | HYPOTHESIS: equinox days (the code only compares the date); September 22/23, frame count even; src/ovl/DATE/80132000.c:336-400 and place setup seasonal branches |
| DateSpecialDay | 4 | DATE_SPECIAL_DAY_DECEMBER_23 | December 23; src/ovl/DATE/80132000.c:336-400 and place setup seasonal branches |
| DateSceneShiori | 3 | DATE_SCENE_SHIORI_AUTUMN | talk_work.scene value of the autumn special date scene of shiori; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014F0F0.c per-girl season scene table |
| DateSceneShiori | 4 | DATE_SCENE_SHIORI_SPRING | talk_work.scene value of the spring special date scene of shiori; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014F0F0.c per-girl season scene table |
| DateSceneShiori | 5 | DATE_SCENE_SHIORI_WINTER | talk_work.scene value of the winter special date scene of shiori; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014F0F0.c per-girl season scene table |
| DateSceneShiori | 6 | DATE_SCENE_SHIORI_SUMMER | talk_work.scene value of the summer special date scene of shiori; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014F0F0.c per-girl season scene table |
| DateSceneMio | 2 | DATE_SCENE_MIO_SPRING | talk_work.scene value of the spring special date scene of mio; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80154260.c per-girl season scene table |
| DateSceneMio | 3 | DATE_SCENE_MIO_SUMMER | talk_work.scene value of the summer special date scene of mio; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80154260.c per-girl season scene table |
| DateSceneMio | 4 | DATE_SCENE_MIO_AUTUMN | talk_work.scene value of the autumn special date scene of mio; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80154260.c per-girl season scene table |
| DateSceneMio | 5 | DATE_SCENE_MIO_WINTER | talk_work.scene value of the winter special date scene of mio; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80154260.c per-girl season scene table |
| DateSceneYuina | 2 | DATE_SCENE_YUINA_SPRING | talk_work.scene value of the spring special date scene of yuina; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80150010.c per-girl season scene table |
| DateSceneYuina | 3 | DATE_SCENE_YUINA_SUMMER | talk_work.scene value of the summer special date scene of yuina; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80150010.c per-girl season scene table |
| DateSceneYuina | 4 | DATE_SCENE_YUINA_AUTUMN | talk_work.scene value of the autumn special date scene of yuina; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80150010.c per-girl season scene table |
| DateSceneYuina | 5 | DATE_SCENE_YUINA_WINTER | talk_work.scene value of the winter special date scene of yuina; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80150010.c per-girl season scene table |
| DateSceneAyako | 2 | DATE_SCENE_AYAKO_SPRING | talk_work.scene value of the spring special date scene of ayako; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80151460.c per-girl season scene table |
| DateSceneAyako | 3 | DATE_SCENE_AYAKO_SUMMER | talk_work.scene value of the summer special date scene of ayako; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80151460.c per-girl season scene table |
| DateSceneAyako | 4 | DATE_SCENE_AYAKO_AUTUMN | talk_work.scene value of the autumn special date scene of ayako; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80151460.c per-girl season scene table |
| DateSceneAyako | 5 | DATE_SCENE_AYAKO_WINTER | talk_work.scene value of the winter special date scene of ayako; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80151460.c per-girl season scene table |
| DateSceneSaki | 2 | DATE_SCENE_SAKI_SPRING | talk_work.scene value of the spring special date scene of saki; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801586A0.c per-girl season scene table |
| DateSceneSaki | 3 | DATE_SCENE_SAKI_SUMMER | talk_work.scene value of the summer special date scene of saki; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801586A0.c per-girl season scene table |
| DateSceneSaki | 4 | DATE_SCENE_SAKI_AUTUMN | talk_work.scene value of the autumn special date scene of saki; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801586A0.c per-girl season scene table |
| DateSceneSaki | 5 | DATE_SCENE_SAKI_WINTER | talk_work.scene value of the winter special date scene of saki; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801586A0.c per-girl season scene table |
| DateSceneYukari | 3 | DATE_SCENE_YUKARI_SPRING | talk_work.scene value of the spring special date scene of yukari; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801552B0.c per-girl season scene table |
| DateSceneYukari | 4 | DATE_SCENE_YUKARI_SUMMER | talk_work.scene value of the summer special date scene of yukari; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801552B0.c per-girl season scene table |
| DateSceneYukari | 5 | DATE_SCENE_YUKARI_AUTUMN | talk_work.scene value of the autumn special date scene of yukari; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801552B0.c per-girl season scene table |
| DateSceneYukari | 6 | DATE_SCENE_YUKARI_WINTER | talk_work.scene value of the winter special date scene of yukari; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801552B0.c per-girl season scene table |
| DateSceneNozomi | 2 | DATE_SCENE_NOZOMI_SPRING | talk_work.scene value of the spring special date scene of nozomi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80156600.c per-girl season scene table |
| DateSceneNozomi | 3 | DATE_SCENE_NOZOMI_SUMMER | talk_work.scene value of the summer special date scene of nozomi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80156600.c per-girl season scene table |
| DateSceneNozomi | 4 | DATE_SCENE_NOZOMI_AUTUMN | talk_work.scene value of the autumn special date scene of nozomi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80156600.c per-girl season scene table |
| DateSceneNozomi | 5 | DATE_SCENE_NOZOMI_WINTER | talk_work.scene value of the winter special date scene of nozomi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80156600.c per-girl season scene table |
| DateSceneMira | 2 | DATE_SCENE_MIRA_SPRING | talk_work.scene value of the spring special date scene of mira; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80152FC0.c per-girl season scene table |
| DateSceneMira | 3 | DATE_SCENE_MIRA_SUMMER | talk_work.scene value of the summer special date scene of mira; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80152FC0.c per-girl season scene table |
| DateSceneMira | 4 | DATE_SCENE_MIRA_AUTUMN | talk_work.scene value of the autumn special date scene of mira; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80152FC0.c per-girl season scene table |
| DateSceneMira | 5 | DATE_SCENE_MIRA_WINTER | talk_work.scene value of the winter special date scene of mira; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80152FC0.c per-girl season scene table |
| DateSceneMira | 6 | DATE_SCENE_MIRA_SPECIAL | talk_work.scene value of the special special date scene of mira; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80152FC0.c per-girl season scene table |
| DateSceneYuko | 2 | DATE_SCENE_YUKO_SPRING | talk_work.scene value of the spring special date scene of yuko; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014CC40.c per-girl season scene table |
| DateSceneYuko | 3 | DATE_SCENE_YUKO_SUMMER | talk_work.scene value of the summer special date scene of yuko; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014CC40.c per-girl season scene table |
| DateSceneYuko | 4 | DATE_SCENE_YUKO_AUTUMN | talk_work.scene value of the autumn special date scene of yuko; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014CC40.c per-girl season scene table |
| DateSceneYuko | 5 | DATE_SCENE_YUKO_WINTER | talk_work.scene value of the winter special date scene of yuko; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014CC40.c per-girl season scene table |
| DateSceneYuko | 6 | DATE_SCENE_YUKO_DECEMBER | talk_work.scene value of the december special date scene of yuko; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8014CC40.c per-girl season scene table |
| DateSceneMegumi | 3 | DATE_SCENE_MEGUMI_SPRING | talk_work.scene value of the spring special date scene of megumi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80157880.c per-girl season scene table |
| DateSceneMegumi | 4 | DATE_SCENE_MEGUMI_SUMMER | talk_work.scene value of the summer special date scene of megumi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80157880.c per-girl season scene table |
| DateSceneMegumi | 5 | DATE_SCENE_MEGUMI_AUTUMN | talk_work.scene value of the autumn special date scene of megumi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80157880.c per-girl season scene table |
| DateSceneMegumi | 6 | DATE_SCENE_MEGUMI_WINTER | talk_work.scene value of the winter special date scene of megumi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/80157880.c per-girl season scene table |
| DateSceneYumi | 3 | DATE_SCENE_YUMI_SPRING | talk_work.scene value of the spring special date scene of yumi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801594B0.c per-girl season scene table |
| DateSceneYumi | 4 | DATE_SCENE_YUMI_SUMMER | talk_work.scene value of the summer special date scene of yumi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801594B0.c per-girl season scene table |
| DateSceneYumi | 5 | DATE_SCENE_YUMI_AUTUMN | talk_work.scene value of the autumn special date scene of yumi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801594B0.c per-girl season scene table |
| DateSceneYumi | 6 | DATE_SCENE_YUMI_WINTER | talk_work.scene value of the winter special date scene of yumi; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/801594B0.c per-girl season scene table |
| DateSceneMiharu | 3 | DATE_SCENE_MIHARU_SPRING | talk_work.scene value of the spring special date scene of miharu; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8015A190.c per-girl season scene table |
| DateSceneMiharu | 5 | DATE_SCENE_MIHARU_SUMMER | talk_work.scene value of the summer special date scene of miharu; src/ovl/DATE/8014B1F0.c, scene dispatcher; src/ovl/DATE/8015A190.c per-girl season scene table |

## 19. Essential semantic continuation (T-27051)

The 850 frozen function/data rows are unchanged. Supplementary parameters, gameplay locals, proven
owned types/fields and literal sites are being reconciled against all 668 definitions in the 19
original objects. Filename coverage is not semantic coverage. Preparation, application and runtime
confirmation remain separate claims.

Supplementary evidence uses the captured original-object paths and function names. Primary is
applying the filename-only map during this work; resolve a moved object through
`config/renames/proposals/layout-DATE-files.tsv`, its YAML or original address-range comment.
Those moves do not remove the referenced function. Dry-plan proof will name its captured tree.

### Frozen identifier supplement

`config/renames/proposals/o-date-essential.tsv` has 378 five-column rows: 6 parameters, 287 locals,
38 types and 47 fields. SHA-256:
`0b6cbda9b0da25e5389acafcd31c63dac0857eb04b7b780ee8f8313ac213612d`.
Independent Standards review required American `Program` spelling and technical girl-counter
weight names instead of new unapproved `syoushin` derivatives; exactly three targets changed.
The earlier 378-row plan passed at hash
`089d0786d0d2767298e7e6d5e857d22e2b2c5b0bdd5e07d956331c8cac07f29e`, but does not prove those
three revised targets. The corrected full plan now also passes: 378 rows, 19 changed paths,
exit 0, zero problems/advisories, at the current `0b6cbda9...` hash. Authoritative replacement proof
is `.claude/orchestrator/state/T-27051-essential/date/identifier-plan-reviewed.json` and `.log`.
It uses the coherent source tree with manifest
`5ddf8ff3d455b94fda3184ddf62eb342abdd79cc5a0a595a6a8c1d6614e31db2` and the recorded current rename
tool; `.claude/orchestrator/state/T-27051-essential/date/identifier-plan.json` and `.log` are proof.
Independent Standards and Spec preparation reviews pass; no source application is claimed.

All 668 definitions were included in the declaration census, including old-style parameters and
the function-pointer local. A second exact retained-use audit caught false exemptions: five
explicitly unused FAKE slots, seven log-count snapshots, a step cursor, an elapsed timer and a
background resource. `o-date-essential-retained-locals.tsv` prepares those 15 locals separately,
preserving the first packet's membership. Its SHA-256 is
`321ef7347316b159d7278b4fed414aaa650120e1916cff601fd1a017afab5119`; full verification-enabled
plan: exit 0, 15 rows, 6 changed paths, zero problems. Combined declaration classification is
308 prepared roles, 26 retained meaningful names, 28 actual loop counters, 126 optional hardware/
presentation declarations and one unused declaration without game logic, totaling 489.
The 38 unique callback members `f` receive `callbacks`, even where a
shared capacity type's own role name cannot yet be resolved without representation changes.

Literal-site preparation required revision. Independent Spec review found comment matches
masquerading as exact operands in the legacy-family expansion and known gameplay deltas without
proposed symbols. Those preparation gaps and the subsequent immediate-domain/alias/count-kind
findings are resolved in the final inventory. Real code-token/AST coordinates distinguish repeated
equal-valued operands, direct selectors, return statuses, groups/lines, strides and arithmetic masks.

The final `o-date-essential-constants.tsv` has **3,268 exact sites / 1,187 names / 110 families**,
SHA-256 `c9b68eccdcd9d283e9c3b295db8743efe4dc0a835076ce4ff0c625e7c0efb3b8`.
Declaration kinds are 1,617 defines, 1,335 enum sites and 316 existing-enum reuse sites. Source
counts, masks, offsets and limits do not become enum storage types. The first-week Sunday cutoff
is explicitly `DATE_FIRST_WEEK_DAY_EXCLUSIVE_LIMIT=8` for `day < 8`; the last accepted day is 7.

All 9,857 numeric code tokens have a disposition: 3,268 prepared; 5,899 optional hardware/resource/
presentation; 630 neutral; 8 explicit matching artifacts; 42 ignored callback payloads; 3 precisely
unproved `ClubFlagBits.unk_25` operands; one mixed-role local; and six fixed-layout operands.
All 668 definitions participate in coverage, of which 645 contain numeric tokens and 23 contain
none. The old 158 conceptual constants reconcile as 117 real role-specific C sites and 41
external/computed-schema entries; no FAKE-comment text is represented as a literal operand.

Actual pointer-table words and owned/shared target definitions prove six dispatcher arguments
reach `wait_sub_sub` and 42 are ignored. The latter receive no invented wait duration. The full
cursor/stride audit covers 164 operands, including the former 117 zero-cursor family; group, line,
disabled variant stride and scalar alias views remain distinct.

Five positive-family dry runs preserve exactly 44 actual edits, with proof JSON SHA-256
`13d4b57c2c7d215e5f249a66217e83799bc6ab37f76a7d4ad3a9b11f25bc535d`.
They prove their selected occurrences only. Independent Standards and Spec reviews pass for the
complete preparation packet, with no unresolved findings. Exact proof and residue are recorded in
`.claude/orchestrator/state/T-27051-essential/date/producer-report.md`, `packet-audit-final.json`,
`semantic-literals-final.json`, `callback-argument-closures.json` and `cursor-role-audit.json`.
The counts, families and five-plan 44-site result above describe preparation packets, not the final applied-site count. Current source integration and ledgers supersede the old “application outstanding” status. Individual candidate sites remain un-applied unless present in the relevant applied ledger. The final matching proof passed; final documentation rereview remains in progress.

### Shared input-sequence latch

The earlier open question about `D_800B3CFC` now has a producer trace. Main
`src/main/pad_input.c:288-306`, `func_80043448`, compares `game_state.pad_held` against the
20-word table `D_800B3CA8`, advances byte index `D_800B3CF8`, and sets the byte latch after index
reaches 18. Byte timer `D_800B3D00` resets the sequence on expiry and clears an active latch on
wrap. The table's game-facing interpretation is not inferred from those mechanics.

DATE reads that latch in `func_801449D4` and `func_80144E20` (`src/ovl/DATE/80141EE0.c:842,1032`)
to enter the karaoke branch. DATE2's `func_801388C0` (`80137D50.c:337-354`) consumes a latch edge,
cycles its selection value through 1 and 2, and clears latch/index; EN_NICHI's `func_80136A2C`
(`80132000.c:1786-1793`) consumes it to increment its variant. These prove a shared input-sequence
latch rather than an idle timer or an unexplained karaoke-only flag. The main `core.txt` historical
note on `mode_title` calls it idle; that wording is superseded by this writer trace. Main symbol
naming and the DATE2/EN_NICHI rules belong to their owners; this DATE packet does not rename them.

### Constant application limits

The family inventory above is not an executable all-site plan. The older selector and signed-literal
limitations described in the earlier preparation notes are historical: T-27070 added exact caller/line
selectors, typed signed-operand replacement and existing-enum reuse in `tools/rename.py`, with
negative and mixed-type coverage in its tests. This support does not mean every candidate site was
applied. Each family still needs exact values/sites and a corresponding applied-ledger entry; an
observed value alone is not permission for a broad substitution.

### Current executable constant packet v2 (2026-10-24)

Fresh Docker planning against the applied identifiers proves 44 DATE replacements with zero
unnamed selections. The ignored packet is `.claude/orchestrator/state/T-27051-constant-v2/`;
its combined plan reconstructs the exact changed C files before any flush. The DATE header has
19 users, all DATE sources. The original 44-site proof remains immutable historical evidence.

Arrival uses six strict one-caller requests for nine sites, preserving the `2U` comparison in
`src/ovl/DATE/place_scenes.c:2998` as `((unsigned int)DATE_ARRIVAL_CORRECTED_PLACE)`.
The neighboring arrival-line and girl-state operands stay untouched. A subsequent legacy positive
request reuses that full enum with a validated 0/1 subset and names only the two remaining
`date_init` assignments in `src/ovl/DATE/entry.c`. This avoids changing the tool's unsupported
whole-function numeric-token guard for the initialization caller. Ride 14, shop 2, meeting 6 and
special-day 11 use private-header positive requests. DATE totals are nine strict and 35 legacy
replacements; all 44 have fresh baseline-coordinate evidence.

The combined DATE/NAME_ENT/GEKO preflight passed at 204 replacements, with the source/header/tool
pins unchanged. Legacy const rows are metadata-free and have no signed-site replay; the strict
requests retain the tool's versioned metadata and inverse/GCC/prototype-view proofs. Primary owns
application, five DATE enum-domain declaration comments, ledger checks and matching verification.
No live constants or headers were applied here, and this does not close the wider literal inventory.
