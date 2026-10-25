---
type: spec
updated: 2026-10-24
unit: RPG_BAT
ticket: T-27027
sources: ["src/ovl/RPG_BAT/", "include/ovl/RPG_BAT.h", "config/renames/proposals/o-rpgbat-apply.tsv", "config/renames/proposals/o-rpgbat-essential.tsv", "src/ovl/DATE/80132000.c", "src/ovl/GEKO/8013AF80.c", "src/ovl/SHUGAKU/"]
---

# Overlay RPG_BAT: the RPG battle scene (port spec)

Phase 2 spec for the Godot port ([[tickets/T-27027-p2-o-rpgbat]], parent [[tickets/T-27000-phase-2]]). Every statement is read from the matched C; "HYPOTHESIS" marks a reading of game meaning the code does not prove. The primary and follow-up proposal files and essential inventories below are preparation history. Current names follow applied ledgers and the final owner closure; do not read old pending-plan language as the current state or assume every candidate applied. The `rpg_` prefix (ADR-0001) and `Rpg` type names remain current. No game text is quoted: enemy kinds, move and speaker roles are described in English.

Naming rules used here. Girls are named by given name, as in the applied core `GirlId`: the girl the strings call by her family name, Himoo, is Yuina (girl id 2, `GIRL_YUINA`); the spec and table say Yuina and `yuina` only. Current `GameState` fields, main functions and pad bits follow the applied core ledgers; the historical placeholder crosswalk in section 7 remains for address lookup.

## 1. What it is

A comic mini game in the style of a console RPG, run as main mode 0x81 on top of the dating sim. The player (the hero) fights with a girl beside him against one of six encounters. The hero has three commands (attack, guard, special) that fill and spend a gauge. The special move is the hero's club move, one of twelve, chosen by the club he belongs to. The girl is a second actor who acts on her own after a per-girl delay. The enemy acts on a timer. Only the hero's HP and the enemy HP pools are tracked; there is no experience, level or item system (the "experience", "gold" and "treasure" lines in the victory text are fixed or random text with no state behind them).

Unit size: 61 C files, 328 functions, `.data` 0x8015E4F0..0x80162000. The scene is mostly choreography; the rules are in about 25 functions (section 5).

## 2. Entry, launch codes, exit

- Entry: main loads RPG_BAT when `game_mode_set(0x81)` runs; the per-frame entry is `rpg_entry` (0x8013B5B0). The O.BIN overlay table (`ovl_entry`) maps mode 0x81 to it ([[overlays]]).
- Launch code: the caller stores a code in main word `D_80122CE0` and saves its own mode, stage and step in `GameState.saved_game_mode`, `saved_mode_stage` and `saved_mode_step` before `game_mode_set(0x81)`.

| Code | Caller | Encounter flag (`rpg_encounter`) | Enemy | Partner rule |
| --- | --- | --- | --- | --- |
| 1 | SHUGAKU, destination value 1 | `RPG_ENCOUNTER_SNAKE` 0x10000 | snake enemy, mongoose rescuer | id 13 (`GIRL_YOSHIO`) kept, else id 14 (`RPG_PARTNER_NONE`, behaviour: no partner) |
| 2 | SHUGAKU, destination value 0 | `RPG_ENCOUNTER_DEER` 0x20000 | deer enemy, monk rescuer | same |
| 3 | SHUGAKU, destination value 2 | `RPG_ENCOUNTER_BEAR` 0x40000 | bear enemy, hunter rescuer | same |
| 4 | DATE, `func_8013C7D0` (the two latest entries of `date_place_history` both equal the chosen place `D_800CA2F8`; that place is not 0; `year` at least 0x60; flag b20 of `player_flags` clear; the launch sets flag b19 and clears the history) | `RPG_ENCOUNTER_DELINQUENTS` 0x100000 | three delinquents | the current girl; ids 13 and 14 become 9 |
| 5 | DATE, `func_8013C7D0` (`year` == 0x61 and `month` == 9, flag b20 clear, and one of: a club slot 0..10 whose club flag field (bits 25..26 of the club word, `ClubFlagBits.unk_25`, not named in core) is 3; flag b19 set, which the launch of code 4 leaves behind; no club points at all in slots 0..10, which also forces the club flag of slot 11 to 3; the launch sets b20 so it fires once and clears the place history) | `RPG_ENCOUNTER_DELINQUENTS` and `rpg_boss_event` | delinquents, then the gang boss | as 4 |
| 6 | GEKO | `RPG_ENCOUNTER_ROBOT` 0x8000000 | the robot of girl 2 (yuina) | id 14 (`RPG_PARTNER_NONE`) |
| 7 or other | SHUGAKU | `RPG_ENCOUNTER_ALIENS` 0x80000000 | aliens | id 2 (`GIRL_YUINA`) |

`D_800CA2F8` is the dating place chosen in DATE (21 places, see the DATE spec), not a girl; the girl is `GameState.current_girl`. The day is not tested by `func_8013C7D0`; whichever DATE step calls it decides when code 5 can happen. The function tests code 5 first (the club or b19 branch, then the no-club branch) and code 4 last, so code 5 wins when both match; when neither fires it just advances the DATE step.

The three SHUGAKU destination values are read from SHUGAKU's own word `D_800CA2CC` (0, 1 or other); which real place each value is stays with the SHUGAKU spec (the enemies suggest nara/deer, okinawa/snake, hokkaido/bear; HYPOTHESIS).

- Exit: `rpg_exit` (stage 15) puts `rpg_result_code` back into `D_80122CE0`, restores `GameState.current_girl` from `rpg_saved_girl`, and calls `game_mode_set` / `mode_stage_set` / `mode_step_set` with the saved `saved_game_mode`, `saved_mode_stage` and `saved_mode_step`. The result code (`RPG_RESULT_GOOD` 0, `RPG_RESULT_BAD` 1) is cleared to 0 at the start and stays 0 on every path that ends with the player standing (delinquent trio or boss beaten, robot beaten, trip rescue; the aliens' rescue script writes 0 explicitly). 1 is written on the two defeat paths (delinquents and boss, where the player is at 0 HP) and when the aliens retreat because the player was held at 1 HP (`rpg_enemy_alien_retreat`): in that last case nobody is knocked out, so the names say only which branch the caller takes, not why. The polarity is proven by the callers' branches: DATE (`func_8013CBF4`) tests the word == 0 and then adds to the girl field (`girls[].tokimeki`: +2 for the date girl, `date_girl`, copied from `current_girl` at launch; when `event_flags[85]` is 2, which launch code 5 sets, the girls whose flag b1 is set get +3 and the date girl +2), otherwise it raises `param_stress` by 5; SHUGAKU (`func_80135430` and followers) saves the word in `D_8013C2EC` and tests == 0 for its trip reward branch (a sound command and different girl-record increments than the other branch); GEKO sets code 6 and does not read the result. So 0 is the better branch for every caller, and the names do not claim more.

## 3. Stages (`GameState.mode_stage`, one switch in `rpg_entry`)

Before the switch: stages 6..9 run `rpg_pause_update`; stages other than 9 call main `cursor_update` unless `menu_mode` bit 0 is set; stages 5..11 draw a three-sided frame (left, bottom, right bars) around the battle view. After it: `cursor_show(1 or 0)` by `menu_mode` bit 0.

| Stage | Constant | Does |
| --- | --- | --- |
| 0 | `RPG_STAGE_INIT` | `rpg_stage_init` clears display state, maps the launch code to the encounter flag, `rpg_init_player_state` sets start values |
| 1 | `RPG_STAGE_LOAD_1` | CD sound-data requests through main `audio_bank_load_start` (the encounter's block, chosen by girl group for the delinquent fight, then the club's block); substep `GameState.substep` |
| 2 | `RPG_STAGE_LOAD_2` | CD graphics packs: encounter pack to 0x80180000, boss pack to 0x801B1000 (delinquents only), club pack to 0x801D8000 |
| 3 | `RPG_STAGE_ASSETS` | `rpg_stage_asset_addresses` (RAM addresses of the loaded packs per encounter), `rpg_stage_palettes_load` |
| 4 | `RPG_STAGE_SETUP` | `rpg_stage_sprites_setup`, HUD setup, `rpg_stage_labels_init` (text labels for the names), HUD digits |
| 5 | `RPG_STAGE_WAVE` | ripple transition (screen copy in VRAM, sine strips) |
| 6 | `RPG_STAGE_INTRO` | aliens: wipe and a Yuina line (`rpg_stage_intro_aliens`); others: "enemy attacked" text and slide-in; music `RPG_BGM_BATTLE` |
| 7 | `RPG_STAGE_START_SHOUT` | start voice, wait for it |
| 8 | `RPG_STAGE_START_MUSIC` | aliens only: music call; then next stage |
| 9 | `RPG_STAGE_FIGHT` | `rpg_stage_fight` (section 4) |
| 10 | `RPG_STAGE_END_SOUND` | sound command 0xC1, next stage |
| 11 | `RPG_STAGE_FADE_OUT` | `screen_brightness` and record levels from 0x80 down by 2 per frame |
| 12 | `RPG_STAGE_RESET_AUDIO` | clears the sound-bank load state (`audio_bank_state_reset`) and the CD command ring (`cd_command_reset`), then next stage |
| 13 | `RPG_STAGE_BANK_RELOAD` | main `func_800674B0` (not named in core): queues sound commands 5 and 0x71 and starts the dating sim sound-bank load; the set is chosen by `GameState.vacation`, with one fixed date overriding it; then next stage |
| 14 | `RPG_STAGE_WAIT_AUDIO` | next stage when the sound-bank poll (main `audio_bank_load_poll`) returns 1; `mode_stage_timer` counts frames and past 0x400 the audio is stopped and the stage is set back to 13 (a retry loop through the bank reload), not forward |
| 15 | `RPG_STAGE_EXIT` | `rpg_exit` |

Stage 9 ends when a victory or defeat script calls main `mode_stage_next` (stage + 1, clears step and counters); `mode_step_next` is step + 1, `mode_stage_set` and `mode_step_set` set stage and step. These are core's names; main owns them.

## 4. The fight loop (stage 9) and its state

Per frame, in order (`rpg_stage_fight`): wave background (only for the delinquent encounter and only once the boss part of the finale script has enabled it, see "Shared words"); in code-entry mode (pause state 3) only the girl timer, girl update and message run; skip-message on cross (`PAD_CROSS`); label switching for the robot fight; the three idle counters (below); girl update; defeat check; player update; defeat check; enemy update; defeat check; victory dispatch (`rpg_start_victory_sequence`); HUD digits; message window; the four damage popups; alien background flicker (aliens only).

Idle counters (each +1 per frame while the relevant action words are idle): `rpg_player_ready_timer` (D_8015ED9C; while the player action word has none of the bits 0xFF0111FF), `rpg_enemy_timer` (D_8015EDC8; while the low bytes of the player, enemy and girl words are 0 and the enemy word lacks 0x1000), `rpg_girl_timer` (D_8015EDF4; same, and the girl word lacks 0x1000 and 0x80000000).

### Script channels

All motion is hand-written step scripts. Five independent channels each hold step, substep and timers and use the same helper set (next step, next substep, wait n frames, wait for an actor animation to end, wait for the message to end):

| Channel | Step / substep / timers | Helpers (new names) |
| --- | --- | --- |
| player | `rpg_player_step` D_8015EDAC, `rpg_player_channel` D_8015EDB0 (substep, timer, timer 2) | `rpg_player_step_next`, `_substep_next`, `_wait_step`, `_wait_substep`, `_wait_animation_*`, `_wait_msg_*`, `rpg_player_action_end` |
| enemy | `rpg_enemy_step` D_8015EDD8, substep D_8015EDDC, timers D_8015EDE0/E4 | `rpg_enemy_*` |
| girl | `rpg_girl_channel` D_8015EE00 (frame counter, step, substep, timer), timer 2 D_8015EE10 | `rpg_girl_*` |
| finale (victory script) | `rpg_finale_channel` D_8015EE30 (step, substep, timers) | `rpg_finale_*` |
| voiced dialog | `rpg_dialog_step` D_8015ED8C[0] (step), [1] (waiting flag); these two words are the last two of the voice-started block (see "Shared words") | `rpg_voice_play_once`, `_wait`, `_reset` |

HUD: three number slots (player HP, the partner's value, the guest/rescuer value) drawn as four digit sprites each by `rpg_hud_numbers_update`, with a name label beside each; a slot is hidden when its value is above 0x10000000. Other state machines: the message window (`rpg_message` D_8015EE44: state, mode, frame counter; `rpg_message_show` sets the line pointers, line count and frames per line), the command menu (`rpg_menu` D_8015EE50), four damage popups (`rpg_popup_0..3`).

Actors are rows of the shared 0x44-byte display-record table `D_8011ECD0`; scripts address them as "actor i" = record 96 + i (`rpg_actor_set_animation`, `rpg_actor_set_position`, `rpg_actor_is_idle`; positions are screen coordinates minus (0xA0, 0x78)). Fixed ids: girl 0x1D, player 0x30, hit effect on the player 0x38, enemy actors 0x3B..0x3D, girl effect 0x32; during a special move the hero is shown by actor 0x34 (with 0x33 as a second sprite) instead of 0x30. Record 64..159 field `unk_07` is the fade level (`rpg_fade_set_level`). Field names of that record type belong to the graphics unit.

### Shared words (aliasing a port must split)

Several globals serve more than one purpose in the matched C; the table names and notes say which, and a port that rewrites them as separate state must keep the uses apart:

- Voice block: `rpg_voice_started` (D_8015ED64) is a 3x4 word view of D_8015ED64..ED93, but the slot index passed to `rpg_voice_play_once` is 0..9 (row 0 only). The last two words of the view are `rpg_dialog_step` (D_8015ED8C) and its waiting flag (D_8015ED90). `rpg_voice_reset` zeroes all twelve, so it also rewinds the dialog step and clears the waiting flag; the dialog scripts end with that call (for example the last step of `rpg_dialog_yuina_stop_nonsense`), which is how they return to step 0.
- Background words: `rpg_bg_word_0..5` (D_8015EC38..EC4C) are, in the wave (stage 9 for the delinquent encounter, stage 5 ripple), the phase, amplitude, half-cycle divisor, wavelength, phase step and enable flag. The enable flag is written 1 only by step 31 of `rpg_victory_delinquents` (the boss part of the finale script), so the stage 9 wave does not run in the plain trio fight. In the aliens scene the same six words are the x and y of the background record group (set in `rpg_stage_intro_aliens`, also by `rpg_alien_departure`) and four on/off flags for the flickering records 0x2A..0x2D (`rpg_alien_scene_flicker`, which clears a flag when the record is switched off and sets it again with probability 1/100 per frame; the intro clears all four).
- Ripple second set: D_8015ED28..ED38 is a second parameter block of the stage 5 ripple that sits inside `rpg_scratch` (D_8015EC74..ED3B) and ends with `rpg_voice_playing_seen` (D_8015ED38), which the ripple uses as a speed word set to 1.
- Move parameters: `rpg_move_params` (D_8015EC58, six words; also `RpgMotion20.rect`) is the girl run (home x, y; target x, y; effect offset x, y), the basketball dunk rectangle (target x, y; a second pair that is probably its size, HYPOTHESIS; two derived points) and the art move offsets and radius. The girl run is one use of it, not the meaning of the words.

### Action codes

- Player (`rpg_player_action`, D_8015ED98): low bits 1 attack, 2 special, 0x100 guard, 0x104 guard during the heal sequence (set after 151 guard frames; the guard bit 0x100 stays set), 0x1000 down, 0x10000 command-ready; request bits 0x01000000 attack, 0x02000000 guard, 0x04000000 special set by the menu and consumed by `rpg_player_update`.
- Enemy (`rpg_enemy_action`, D_8015EDC4): high half = encounter group (0x10000..0x40000 trips, 0x100000 delinquents, 0x200000 and 0x400000 boss rounds, 0x8000000 robot, 0x80000000 aliens), low byte = action number; `rpg_enemy_update` runs the matching function. Action numbers per group are in section 5.5.
- Girl (`rpg_girl_action`, D_8015EDF0): 0 none, 1..3 her action number, 0x1000 done for good (girl 1), 0x40000000 reacted (girl 9), 0x80000000 pending (girl 1), 0x10001000 and 0x10001001 the alien survival timer and rescue.

## 5. Rules

### 5.1 Start values (`rpg_init_player_state`)

- Club: scan the 12 club words `GameState.clubs`; the club whose flag field (bits 25..26 of the word) equals 3 is the player's club (`rpg_player_club`); slot 11 (no club) is kept only if all twelve words have an empty `unk_00` part, else the club becomes 0xFF. A club of 0xFF disables the special; so do all trip encounters and the aliens (mask 0x800F0000). With a club move: gauge starts at 100 and the special is ready.
- Partner id (`rpg_girl_id`, from `current_girl`): trips keep 13, everything else there becomes 14; non-trips remap 13 and 14 to 9; aliens use 2; the robot fight uses 14. Id 14 means no partner here (no HP shown, no actions; `RPG_PARTNER_NONE`, HYPOTHESIS that this reuses the id of another person: the shared `GirlId` table names id 14 `GIRL_SOTOI`, the butler).
- Player HP and max HP: parameter 0 (`param_condition.value`) times 10, at least 10.
- Partner HP value (`rpg_girl_hit_points`, shown in the HUD; only girl 13 changes it): ids 0..10: 256, 99, 312, 304, 189, 298, 1878, 277, 365, 173, 265; id 13: 573; id 14: 0x7FFFFFFF.
- Enemy pool: trips 1/1; robot 5000; aliens 0x7FFFFFFF (scale 1000); delinquents 0x10000 (boss rounds). Three delinquent gauges: 5000 each with `rpg_enemy_damage_scale` 1000 when launch code 5, else 9999 each with scale 1500.
- Enemy action word starts as the encounter flag; the robot starts with action 2 (arrival script). Ready timer starts at 0xFFFF (ready at once) except the robot fight, where the player starts down (0x1000, timer 0) until the arrival ends.

### 5.2 Player commands

- The command menu opens when the ready flag 0x10000 is set. The flag is raised when `rpg_player_ready_timer` exceeds 120 (`rpg_player_ready_delay(0)`) and the timer restarts at 0, so the normal wait is 120 frames of no action; ending a guard instead sets the timer to a random 60..128 (30..88 in the aliens fight), which shortens the next wait. Items shown: attack and guard, plus special when `rpg_special_ready`. The choice toggles a request bit; the update turns it into the action word.
- Gauge `rpg_special_gauge` (0..170): attack +30 + rand(15); guard +20 + rand(15); special costs 100. The special becomes ready when the gauge is at least 100 and a club move exists; it is spent on use. Aliens: every attack also adds 600 frames to the survival counter (`D_8015EE00` word 0).
- Guard (action 0x100): enemy damage is reduced (divisors in 5.5); in the aliens fight each guarded hit also raises `rpg_enemy_damage_scale` by 100. After 151 frames of guard with no enemy or girl action the action word becomes 0x104 and the player heals 5 percent of max HP (`rpg_player_guard_heal`, at least 1) while the heal message and effect play; then the action ends.
- Attack (action 1): target slot, damage roll, then `rpg_player_attack` with a flurry of hit flashes (1 to 10 hits by damage tier: <200, <500, <800, <1100, <1300, 9999 gives 10, else 6).

### 5.3 Target slot (`rpg_pick_target`)

Trips: slot 16/17/18 by encounter (numbers only, no HP). Delinquents: attack action: slot 1 with 1/15, slot 2 with 1/15, else slot 0 (13/15); other actions: slots 1 and 2 with 1/4 each, else 0. Boss rounds 3; robot 4; aliens 5. The slot gives the target rectangle for effects (`rpg_set_target_position`).

### 5.4 Player damage

Basic attack (`rpg_player_basic_damage`): lv = min(athletics, 300) (`param_athletics.value`); base = rand(100) + rand(100) + lv x 9 / 2; roll r = rand(4096): r == 0 gives 9999, r < 64 gives 0, r < 270 gives base + rand(500) + rand(1000), else base; capped at 9999. In the boss rounds with no club move any non-zero hit also removes 1500 from the boss pool.

Special (`rpg_player_damage_roll`): club points c = `clubs[club].exp` (core's HYPOTHESIS name for the low field of the club word) clamped to 50..550; base = c x 10 - 500 (0..5000). "Trio" means the delinquent encounter (three damage values, the target slot listed gets the stated bonus); `r(n)` is rand(n); `r2(a,b)` is rand(a) + rand(b). All values are capped at 9999.

| Club (core `Club` id) | Damage |
| --- | --- |
| literature (0) | 5000 + 1000 x rand(base/1000 + 1) |
| drama (1) | r2(6000, base) for each of the three (trio) or the single target |
| science (2) | trio: three values r2(2500, 2500) + 2500, the target slot 9999; single: 9999 |
| computer (3) | r2(1500, base) + 3000 |
| art (4) | trio: 20 percent chance 1000, else r2(base/2, 3500) + 2500; single: the latter |
| brass band (5) | trio: three values r2(2500, 1000) + 2000, +2000 more on the target slot; single: r2(2500, 1000) + 4000 |
| baseball (6) | trio: slot 0 rand(50) x 100 + 2000, slots 1 and 2 rand(50) x 100 + 1000; single: rand(50) x 100 + 3000 |
| soccer (7) | r2(base, base + 2000) + 1000 |
| tennis (8) | h = (base + 2000)/2; r2(h, h) + 4000 |
| swimming (9) | h = base/2; r2(h, h) + 6000 |
| basketball (10) | r2(base, base + 1000) + 2000 |
| none (11) | tiered by `rpg_escalation_counter` n: n 0..1 r2(500,500); 2..3 r2(1000,1000); 4..5 r2(1500,1500); 6..7 r2(2000,2000); 8..15 r2(2500,2500); 16 or more 9999 (and, outside the trio, 30000 off the pool). The roll also writes the tier index back into the counter (0, 1, 2, 3, 4, 5 for the six ranges); the move's animation and sound variant are chosen from that index, and the move's last step (step 9) sets the counter to 0. Enemy attacks then count it up again |

A special in the trips or the aliens fight is not possible; in a non-special action the basic roll is used. Each special has a fixed sequence (approach, banner, 3 to 36 animation steps, damage popups, return) with the girl's own comment inserted at steps 6..18 (section 5.6).

### 5.5 Enemy side

Common: the enemy waits `rpg_enemy_action_threshold` frames: trips 330; robot 360; the others 150 + `rpg_enemy_delay_extra` (re-rolled to rand(20) + rand(40) at every choice). When the timer passes it and nobody acts, an action is chosen (weighted, below); a result with low byte 0 means "wait", the timer is then set to a random head start rand(60) + rand(90). The guts factor (225 - min(guts, 150)) / 150 (`param_guts.value`, 1.5 at guts 0, 0.5 at 150) and the scale `rpg_enemy_damage_scale` / 1000 are applied only where stated below; trips, the robot and the cover attack use plain fractions of HP.

Trips (actions chosen by `rpg_enemy_pick_trip`): attacks 1 and 2 are the enemy's named basic attack, damage = 10 percent of current HP (1 percent when guarding); the third and later choice is action 2, the finale: the enemy bites for HP - 3, the rescuer appears and wins, victory text, result 0. The player cannot be knocked out and cannot hurt the enemy.

Delinquents (`rpg_enemy_pick_delinquent`, weights in `RpgBattleState` slots 0..3, each used one -5): start 15/15/5/30 with the boss event, else 10/10/0/40, refilled when their sum drops below 21. A roll in [0, sum + miss weight): slot 0 gives action 1 (taunt line, no damage); slot 1 gives 0 (wait); slot 2 plus `rpg_enemy_miss_weight` gives action 3 (punch misses; miss weight -20) or 4 when the chosen attacker is charmed; slot 3 gives the real attack (weight -5, and with launch code 4 the scale rises by 25 each time): charmed attacker: action 4; else 5 percent (unless the girl is in state 0x40000000): action 6 (the player covers the girl, damage = girl HP value / 10, divided by 10 more when guarding); else 5 percent (not when the player action word is exactly 0x100, so it can still happen in the heal state 0x104): action 5 (small hit r2(10, 10) x scale / 1000, then the player is down for a moment); else action 2 (damage r2(125, 75) x guts factor x scale / 1000, divided by 10 when guarding). Each real attack adds 1 (action 6: 2) to `rpg_escalation_counter`. The attacker is a uniformly random slot 0..2. Action 7 is entered when an action-2 punch leaves the player at 0 HP: down, defeat taunt, result 1. Slots whose gauge is 0 are not excluded; the fight ends as soon as any one of the three gauges is 0 (`rpg_check_enemy_defeated`). Charmed (girl 7): a slot flag that makes that delinquent's real attack hit another delinquent (r2(100, 100)); it is cleared when the slot takes damage; if all three are charmed the girl's chooser returns 0.

Boss rounds (launch code 5, group 0x200000 then 0x400000): the four weights are zeroed when round 1 starts, so the first choice refills them. Round 1 (`rpg_enemy_pick_bancho_1`): if the weights sum to under 11 they are refilled to wait 20, taunt 15, attack 20; the roll gives action 0 wait (slot 0), 1 taunt (slot 1) or 2 attack (slot 2), the picked slot loses 5 and an attack adds 2 to `rpg_escalation_counter`. Before that, when the boss pool is below one third of its maximum, the weights are set to 0 / 5 / 15 and action 3 (turns serious, starts round 2) is returned. Round 2 (`rpg_enemy_pick_bancho_2`): same scheme, refill at a sum under 11 to wait 25, taunt 15, powered attack 15; actions 0, 1 and 4 (powered attack, counter +2). Damage by `rpg_enemy_damage_roll`, with M the player's max HP: attack 2: r(M x 25 / 100 - 500) + 500, attack 4: r(M x 50 / 100 - 1000) + 1000, both x guts factor x scale / 1000, guarding divides by 3 and 10. The taunt line index is `pool x 6 / max` (six HP bands). Player at 0 HP: action 5, result 1 and defeat. Boss pool 0 ends it with the hand-over scene, loot text and a random title, result 0.

Robot (the robot of girl 2, group 0x8000000): action 1 is always chosen (`rpg_escalation_counter` +2): three announced attacks in a row, then one damage roll: 20 percent of the current HP (2 percent guarding, 0 below 20 HP; 50 percent when the player is already down), so the player cannot die. The pool is 5000; the robot dies to damage; the arrival script is action 2 at the start.

Aliens (group 0x80000000, chooser `rpg_enemy_pick_alien`: weight slots 0..3 map to wait, chatter (4), attack 1, attack 2; start 15/5/10/10 when their sum is under 6): attack 1: r2(M/10, M x 5 / 100) + 100, attack 2: r2(M/10, M/10) + 150 (M = the player's max HP), both with the guts factor and the scale; guarding halves the hit but raises the scale by 100; every hit is clamped to leave the player 1 HP; at 1 HP the enemy action becomes 3 (aliens flee, result 1). The pool is reduced by damage like the others, but the defeat check does not cover this encounter, so it never ends the fight. The girl channel counts frames: after about 10800 frames (3 minutes; each attack adds 600) the girl action becomes the rescue script (Yuina destroys the aliens with 9999, a closing line, result 0).

### 5.6 Girl (partner) behavior

Cooldown before she may act (`rpg_girl_action_delay`): id 0 200; 1 10; 7 240; 9, 6, 8, 10: 260; 4, 2, 3, 5: 300; 13: 110 when her HP value is under 10, else 170; 14 never. The choice (`rpg_girl_pick_action`): weights wait/act both 50 (100/100 for ids 1, 13, 14), each choice -5, refilled when the sum is under 20; when the result is 0 the girl timer is set to a random head start rand(100) + rand(50) (so the next try comes sooner). Her action runs only while nothing else acts.

| Id | Name | Action (when she acts) |
| --- | --- | --- |
| 0 | shiori | slides in, calls out, slides out; the enemy timer is reset to 0 |
| 1 | mio | only once the player has lost more than 10 percent of max HP: says she is dizzy, then idle for good |
| 2 | yuina | aims at the chosen target: damage r(1250) + r(750) + 1 (she does nothing in the aliens fight, where she is only the partner id) |
| 3 | ayako | burst of shots: three targets r(600) + r(300) + 1 each (boss rounds one target r(1000) + r(500) + 1); also the player loses r(m) + r(m), m = min(HP/4, 100) |
| 4 | saki | cheers: heals the player r(75) + r(125) + 1 |
| 5 | yukari | calls her father, who hits the target for r(2000) + r(1000) + 1 |
| 6 | nozomi | shouts; polite or rough by her mood byte; enemy damage scale +10 |
| 7 | mira | trio: 50 percent she charms one uncharmed delinquent (sets its flag), else a snub line; other fights: always the snub line |
| 8 | yuko | talks to the enemies: the extra enemy delay becomes 360 (only if it is under 101) |
| 9 | megumi | action 1 flinch, then state 0x40000000; later 80 percent shout (3), 20 percent pause (2) |
| 10 | yumi | fake crying; the miss weight of the delinquent AI becomes 50, the boss or delinquents react |
| 13 | yoshio | three actions: punch (misses, his HP value drops to 7), kick (same), potion (HP value back to full); picks potion when under 8, else punch or kick at random |

During the hero's special move (steps 6..18) the girl's own "assist" function (`rpg_move_assist_dispatch`, ids 0..10) makes her run in, speak a line, and run out; this is presentation only.

### 5.7 End conditions

- Delinquents: any gauge 0 -> `rpg_victory_delinquents` (text, loot: 2 draws, 3 with probability 1/5, each over four lines with weights 4:3:2:1 out of rand(10), with replacement and one flag per line, so repeats collapse and 1 to 3 distinct lines show; result 0). Launch code 5 then continues: unseen voice, boss entrance and speech, the player is dropped to 0 HP, a voice restores HP to the old max and raises max HP by 2000 (cap 9999), round 1 starts with the pool at 0x10000.
- Trips: after the finale script. Robot: pool 0 -> `rpg_victory_robot` (fade to white), result 0. Aliens: see 5.5.
- The player cannot lose in trips, the robot fight or the alien fight; he can lose to the delinquents and the boss (result 1).

### 5.8 Pause and hidden code entry

Pad bit 0x800 (`PAD_START` in core's `PadBits`, tested in `GameState.pad_pressed` with a 2-frame debounce) toggles a pause (`rpg_pause_state` 0 run, 1 paused, 3 code entry). In pause and in state 3 two ten-button sequences are matched against the pad word (`rpg_code_sequence_a`: `PAD_UP`, `PAD_UP`, `PAD_DOWN`, `PAD_DOWN`, `PAD_LEFT`, `PAD_RIGHT`, `PAD_LEFT`, `PAD_RIGHT`, `PAD_CROSS`, `PAD_CIRCLE`; `_b`: `PAD_DOWN`, `PAD_DOWN`, `PAD_UP`, `PAD_UP`, `PAD_RIGHT`, `PAD_LEFT`, `PAD_RIGHT`, `PAD_LEFT`, `PAD_CIRCLE`, `PAD_CROSS`). State 3 is entered from stage 9 only when the partner id is 9 and everything is idle. Sequence a: trips set HP and max HP to 0x1662 when HP >= 4 (the robot fight also sets the enemy pool to 1); the delinquent fight sets HP and max to about 5730 when HP is 1..9, and for HP >= 10 adds 1 to both (cap 9999); the aliens do the same from HP 2. Sequence b: the delinquent fight sets HP to 1; the aliens to 2. The pad layout of the two tables and of the pause test is the PsyQ layout core's `PadBits` uses (checked against the data words). A debugging-style code; the port may keep or drop it (HYPOTHESIS: developer feature).

## 6. Data tables

| Address | New name | Content |
| --- | --- | --- |
| 0x8015E4F0 | `rpg_spark_directions` | eight (dx, dy) steps at 45 degrees, 0x100 / 0xB5 units |
| 0x8015E530, EFA0, EFB0, EFC0, EFE0 | `rpg_loot_names_*` | four string pointers each (decor text only) |
| 0x8015E550, E578 | `rpg_code_sequence_a/b` | ten pad words each |
| 0x8015E5A0 | `rpg_popup_bounce_table` | 16 rows of 12 s16 offsets, row = popup frame / 2 |
| 0x8015E720 | `rpg_menu_item_positions` | six (x, y) pairs for the three menu items |
| 0x8015E74C.. | `rpg_text_player/enemy/girl/result_*` | 0x28-byte line buffers in four arrays (5, 5, 5, 10) with pointer tables E814, E8F0, E9CC, EB70; only the buffers the code addresses directly are named (player 0..2, enemy 0..3, girl 0..1, result 0..5), the others are reached through the pointer tables and have no table row |
| 0x8015EE8C..EEEC | `rpg_asset_*` | pointers filled at stage 3: sheets (6), frame tables (6), animation tables (6), palettes (7) |
| 0x8015EEF0 | `rpg_drama_lines` | 41 line pointers sampled by the drama move; its collision adjustments do not guarantee three different lines |
| 0x8015EC38..EC4C | `rpg_bg_word_0..5` | shared words: wave parameters and the aliens scene position and flicker flags (section 4, "Shared words") |
| 0x8015EC58 | `rpg_move_params` | six words, per-use meaning (section 4, "Shared words") |
| 0x8015ED64, ED8C | `rpg_voice_started`, `rpg_dialog_step` | one 12-word block; the dialog step and flag are its last two words (section 4) |
| per-girl constants | in code | girl HP values, delays, damage ranges (sections 5.1 and 5.6) |

`func_80150F84` samples indices `a`, `b` and `c` independently from 0..40. It then increments `b` modulo 41 if `b == a`, increments `c` modulo 41 if `c == a`, and increments `c` modulo 41 if `c == b`. The final adjustment can make `c == a` because the function does not check `a` again: for example, `a=2, b=1, c=1` ends with `c=2`; `a=0, b=40, c=40` ends with `c=0`. The routine attempts to avoid collisions but can copy the same line more than once.

## 7. Connections

- Reads `GameState` (core names): `param_condition.value` (parameter 0, HP), `param_athletics.value`, `param_guts.value`, `clubs[12]`, `current_girl`, `player_family_name` (the hero's name string used in the lines), `pad_pressed`, `menu_mode` and `menu_cursor_0`, `mode_stage`, `mode_step`, `substep`, `mode_stage_timer`, `mode_step_timer`, `saved_game_mode`, `saved_mode_stage`, `saved_mode_step`, `screen_brightness`, `vacation`. Writes `current_girl` (temporarily 14 for voice calls, restored at exit) and the stage counters. Reads girl state through `get_g_zyotai_s(6)`. The callers' fields (DATE: `year`, `month`, `date_place_history`, `player_flags`, `girls[].tokimeki`, `param_stress`) are listed in section 2.
- Main helpers used (names owned by main, core's names where it has one): `rand`, `strcpy` and `strcat`, `rsin`, `cd_read_sectors`, voice and sound commands (`audio_command`, `cd_audio_request_set`, `cd_audio_state` with status 4 playing and 2 done), music (`audio_bank_start`), bank loading (`audio_bank_load_start`, `audio_bank_load_poll`, `audio_bank_state_reset`, `cd_command_reset`), text labels (`set_kanji_string`, `k_disp_*`, `k_sub_reset`), menu (`menu_set`, `menu_check`, `cursor_update`, `cursor_show`), palette and texture loads, GPU fill helpers (`prim_gradient_rect`, `dtd_on_tpage`), stage control (`mode_stage_next`, `mode_stage_set`, `mode_step_set`, `game_mode_set`).
- Hardware layer (module level only): sprites are display records; CLUT/texture loads (`load_palette`, `load_tpage_buf_lock`); VRAM copies for the ripple (`func_8009C93C`, not named in core); sound-bank CD loads (stage 1); voiced lines through the CD voice path (`rpg_voice_*`; 44 `rpg_dialog_*` scripts pair one text line with one voice clip); GPU quads for the wave and wipes (`rpg_wave_*`). The port replaces these; the logic above does not depend on them except through the status values stated.
- Disc: `BATTLE/` holds one pack per encounter, club and girl (names of the form `VS_<club|girl|enemy>`), loaded at stages 1..2 and 4; the SERIFU voice file for this scene is read through the voiced-dialog path.

### Main and core names used by this spec

The matched C still shows placeholders; this is the map used in this spec (source: core table).

| Placeholder | Core name | Placeholder | Core name |
| --- | --- | --- | --- |
| `GameState.unk_F88` | `pad_pressed` | `GameState.unk_1109` | `mode_stage` |
| `unk_F5F` | `current_girl` | `unk_110A` | `mode_step` |
| `unk_66C` | `clubs` | `unk_110D` | `substep` |
| `unk_720`, `721`, `722` | `saved_game_mode`, `saved_mode_stage`, `saved_mode_step` | `unk_1100`, `unk_1104` | `mode_stage_timer`, `mode_step_timer` |
| `unk_03A` | `screen_brightness` | `unk_0FC`, `unk_10C`, `unk_118`, `unk_11C` | `param_condition`, `param_athletics`, `param_guts`, `param_stress` (field `unk_02` = `value`) |
| `unk_1092`, `unk_1093` | `menu_mode`, `menu_cursor_0` | `unk_0D4` | `player_family_name` |
| `unk_728` | `date_place_history` | `unk_03E`, `unk_03F` | `year`, `month` |
| `unk_0F4` | `player_flags` | `unk_1BC[i].unk_02` | `girls[i].tokimeki` |

## 8. Open questions

- Which real SHUGAKU destination each launch code 1..3 stands for (needs the SHUGAKU spec).
- Whether partner id 14 (`RPG_PARTNER_NONE`) is meant as "no partner" or another person; core names id 14 `GIRL_SOTOI`. The HUD hides its HP only through the 0x7FFFFFFF sentinel.
- `rpg_guest_value` (D_8015EE18): the number shown for the rescuer slot (trips) is 0x10000000-flagged and revealed by an XOR in the finale; its exact HUD role is not traced.
- Result code: the polarity (0 is the better branch for the callers) is read from the branches in DATE and SHUGAKU (section 2); GEKO does not read it. What stays open is only whether the game thinks of 1 as "failed" in the aliens retreat, where the code sets it without a knock-out.
- Word `D_8015EBD8` (enemy damage scale) rises when the player guards in the aliens encounter and scales enemy damage; whether this is an intended defensive penalty is a hypothesis.
- The BGM ids 0x200..0x204 are named from call context only. The OPTION sound test lists one RPG music name (`RPG_BTL`) and the effect banks `RPG_CLB1..4`, `RPG_GAL`, `RPG_GAL1..3`, `RPG_HOK`, `RPG_NAR`, `RPG_OKI`, `RPG_ROBO`, `RPG_SYU`, `RPG_UFO`; if 0x200..0x204 are sequences of that one music bank, the five guesses collapse into one developer name plus an index. Not traced. The bank names HOK, NAR and OKI also back the guess that the three trips are Hokkaido, Nara and Okinawa.
- Emulator: the scene was not run; reaching it needs a save at a DATE or SHUGAKU trigger point (tools/emu has no memory poke).

## 9. Symbol coverage and open naming work

The address-owned overlay has 61 original C objects in `config/objects/RPG_BAT.txt`. The 597-row primary packet (328 function, 168 data, 11 type, 31 field, 58 parameter and 1 local) and its 73 constant candidates/five type leads are historical packet counts. Independent review checked that preparation's corrections and selected source evidence, not every semantic claim. The earlier plan attempt exited 137; final owner closure and applied ledgers now govern current source names. The constant inventory remains candidate evidence rather than proof that all sites applied.

The local `var_a0` in `func_801477E0` is named `player_extra_damage_bound`: it is `min(player HP / 4, 100)` and bounds extra player damage written to D_8015EBBC for target 33. That value is consumed by the player hit-points damage path (`func_80151C10` to `func_8013EA60`, target 33). Parameter `arg3` of `func_8013EA60` is `damage_effect_mode`: the flag chooses player damage versus healing and is also passed as the popup kind.

The table below records the primary packet's unresolved-identifier snapshot after subtracting its parameter/local proposal rows. Consult the final owner closure and current ledgers before treating any row as still unresolved; compiler temporaries and stack spills remain technical placeholders where no distinct game-state role is established.

| Definition | Unresolved decompiler identifiers | Status / reason |
| --- | --- | --- |
| `func_80132000` | `temp_v1` | temp_v1 (compiler temporary; no distinct game-state role established) |
| `func_80132EE0` | `temp_v0_2` | temp_v0_2 (compiler temporary; no distinct game-state role established) |
| `func_80134A70` | `temp_v1`, `temp_v1_2` | temp_v1 (compiler temporary; no distinct game-state role established); temp_v1_2 (compiler temporary; no distinct game-state role established) |
| `func_80135358` | `sp34` | sp34 (stack spill; no source-level variable established) |
| `func_80136530` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80137204` | `var_s0`, `var_s1` | var_s0 (unresolved local intermediate; role not established); var_s1 (unresolved local intermediate; role not established) |
| `func_80138324` | `sp34` | sp34 (stack spill; no source-level variable established) |
| `func_801387F0` | `sp34` | sp34 (stack spill; no source-level variable established) |
| `func_80138B9C` | `sp34` | sp34 (stack spill; no source-level variable established) |
| `func_8013A480` | `temp_v1` | temp_v1 (compiler temporary; no distinct game-state role established) |
| `func_8013A650` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8013ACA4` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8013BBF8` | `sp30`, `sp58` | sp30 (stack spill; no source-level variable established); sp58 (stack spill; no source-level variable established) |
| `func_8013C5D0` | `temp_v1`, `temp_v1_2` | temp_v1 (compiler temporary; no distinct game-state role established); temp_v1_2 (compiler temporary; no distinct game-state role established) |
| `func_8013E300` | `temp_v1` | temp_v1 (compiler temporary; no distinct game-state role established) |
| `func_8013E54C` | `arg0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_8013E7C0` | `arg0`, `arg1`, `arg2`, `arg3`, `temp_v0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg3 (unresolved parameter; body/caller evidence not yet sufficient for a name); temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8013E810` | `arg0`, `arg1`, `arg2`, `arg3`, `temp_v0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg3 (unresolved parameter; body/caller evidence not yet sufficient for a name); temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8013E864` | `arg0`, `arg1`, `arg2`, `temp_v0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8013E8B8` | `arg0`, `arg1`, `arg2`, `temp_v0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8013E934` | `arg0`, `arg1`, `arg2` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_8013EA30` | `arg0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_8013F350` | `arg0`, `arg1` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_8013F3F0` | `arg0`, `arg1` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_8013FF28` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80140A58` | `temp_v0`, `temp_v0_2` | temp_v0 (compiler temporary; no distinct game-state role established); temp_v0_2 (compiler temporary; no distinct game-state role established) |
| `func_80140CC8` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80141C70` | `temp_v1` | temp_v1 (compiler temporary; no distinct game-state role established) |
| `func_80141E38` | `temp_v1` | temp_v1 (compiler temporary; no distinct game-state role established) |
| `func_801421E0` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_801427E4` | `temp_v0`, `temp_v0_2`, `temp_v0_3` | temp_v0 (compiler temporary; no distinct game-state role established); temp_v0_2 (compiler temporary; no distinct game-state role established); temp_v0_3 (compiler temporary; no distinct game-state role established) |
| `func_80143830` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80143C28` | `arg0`, `arg1`, `temp_v0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80143CE8` | `arg0`, `arg1`, `temp_v0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80143DA8` | `arg0`, `arg1`, `arg2`, `arg3`, `arg4`, `arg5` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg3 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg4 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg5 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_801477E0` | `temp_t4` | temp_t4 (compiler temporary; no distinct game-state role established) |
| `func_80147930` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80147F10` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_801496F0` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014A660` | `arg0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_8014ADD0` | `arg0`, `arg1` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_8014BB80` | `arg0`, `sp34` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); sp34 (stack spill; no source-level variable established) |
| `func_8014E64C` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014ED44` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014ED80` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014F080` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014F0BC` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014F350` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014F38C` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014F5DC` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014F618` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_8014F88C` | `sp28`, `var_a0` | sp28 (stack spill; no source-level variable established); var_a0 (unresolved local intermediate; role not established) |
| `func_80151380` | `arg0`, `temp_v0` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80151F38` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80152080` | `sp34` | sp34 (stack spill; no source-level variable established) |
| `func_80152384` | `sp64`, `sp80`, `sp90`, `spA0` | sp64 (stack spill; no source-level variable established); sp80 (stack spill; no source-level variable established); sp90 (stack spill; no source-level variable established); spA0 (stack spill; no source-level variable established) |
| `func_801525B0` | `sp28`, `var_a3` | sp28 (stack spill; no source-level variable established); var_a3 (unresolved local intermediate; role not established) |
| `func_80153B08` | `temp_s1`, `temp_s3`, `var_s0` | temp_s1 (compiler temporary; no distinct game-state role established); temp_s3 (compiler temporary; no distinct game-state role established); var_s0 (unresolved local intermediate; role not established) |
| `func_80153D20` | `temp_s1`, `temp_s3`, `var_s0` | temp_s1 (compiler temporary; no distinct game-state role established); temp_s3 (compiler temporary; no distinct game-state role established); var_s0 (unresolved local intermediate; role not established) |
| `func_80153F3C` | `arg0`, `arg1`, `arg2`, `arg3`, `arg4`, `arg5` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg3 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg4 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg5 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_80154220` | `arg0`, `arg1`, `arg2`, `arg3`, `arg4` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg3 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg4 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_8015528C` | `arg0`, `arg1`, `arg2`, `arg3`, `arg4`, `arg5`, `arg6`, `arg7`, `arg8` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg3 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg4 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg5 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg6 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg7 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg8 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_80155868` | `arg0`, `arg1`, `arg2`, `arg3`, `arg4`, `arg5`, `arg6`, `arg7`, `arg8` | arg0 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg1 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg2 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg3 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg4 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg5 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg6 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg7 (unresolved parameter; body/caller evidence not yet sufficient for a name); arg8 (unresolved parameter; body/caller evidence not yet sufficient for a name) |
| `func_80156D78` | `sp38` | sp38 (stack spill; no source-level variable established) |
| `func_80157158` | `sp34` | sp34 (stack spill; no source-level variable established) |
| `func_801596E8` | `temp_v0` | temp_v0 (compiler temporary; no distinct game-state role established) |
| `func_80159B40` | `sp5C`, `sp78`, `sp88` | sp5C (stack spill; no source-level variable established); sp78 (stack spill; no source-level variable established); sp88 (stack spill; no source-level variable established) |

The unresolved inventory covers 67 definitions and 140 identifier entries: 67 arguments, 48 compiler temporaries, 6 local intermediates, 19 stack spill slots. An argument was removed from this list only when the proposal contains its parameter rename and body-backed rationale. The follow-up packet `o-rpgbat-followup-apply.tsv` renames three role-proven array types (`RpgStrTbl4`, `RpgWordTbl4`, `RpgWordTbl10`) and the source-local 41-entry drama string table. It also names the `RpgBattleView` fields at 0x0C, 0x10 and 0x14 as aliases of the primary data symbols `rpg_enemy_damage_scale`, `rpg_enemy_delay_extra` and `rpg_enemy_miss_weight`. `RpgBatWords3` and `RpgBatWords4` remain unresolved because their shared field offsets have different roles across instances; `RpgActorMotion.unk_08` overlaps another global, and `RpgTransitionState.unk_04` has no observed read or write. The exact type and field disposition is recorded in `o-rpgbat-followup-essential.tsv`.

Final convention correction to the historical candidate packet: it contained 168 data, 31 field, 328 func, 1 local, 58 param and 11 type rows. Remaining size-derived aliases were explicitly withheld; maximum/minimum and argument words were expanded. Source integration has since occurred; final matching/reviewer status is tracked separately. Genuine narrative/audio hypotheses and unresolved aggregates remain open.


Residual completeness audit: after excluding comments, 83 additional in-range data identifiers remain outside the named-data packet and are explicitly inventoried as unresolved references/declarations. They are not assumed to be independent storage definitions; aggregate alias ownership must be proved before separate renames. Seven original RpgLaunch constant leads are also explicitly unresolved alongside 73 site candidates.

Type completeness audit: the actual C/header census includes 18 named type definitions, including source-local views. The primary essential inventory listed 12 unresolved members; the follow-up names the three `RpgBattleView` alias fields, leaving nine unresolved members: the seven fields of the heterogeneous `RpgBatWords3`/`RpgBatWords4` views, `RpgActorMotion.unk_08` (overlapping alias), and `RpgTransitionState.unk_04` (no observed access). The primary inventory's five withheld type leads are resolved as follows: `RpgStrTbl4`, `RpgWordTbl4`, and `RpgWordTbl10` receive body-backed role names in the follow-up; the two `RpgBatWords` aggregate types remain unresolved with per-instance evidence. `StrTbl41` receives `RpgDramaLineTable`; `RpgBattleView` remains a partial view because its aggregate base/extent is explicitly unresolved. No unresolved member is treated as a hardware exception.
