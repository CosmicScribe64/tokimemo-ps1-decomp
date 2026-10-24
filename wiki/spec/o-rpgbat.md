---
type: spec
updated: 2026-10-24
unit: RPG_BAT
ticket: T-27027
sources: ["src/ovl/RPG_BAT/", "include/ovl/RPG_BAT.h", "config/renames/proposals/o-rpgbat.tsv", "src/ovl/DATE/80132000.c", "src/ovl/GEKO/8013AF80.c", "src/ovl/SHUGAKU/"]
---

# Overlay RPG_BAT: the RPG battle scene (port spec)

Phase 2 spec for the Godot port ([[tickets/T-27027-p2-o-rpgbat]], parent [[tickets/T-27000-phase-2]]). Every statement is read from the matched C; "HYPOTHESIS" marks a reading of game meaning the code does not prove. New names are in `config/renames/proposals/o-rpgbat.tsv` (prefix `rpg_battle_`, the English form of the overlay name; the audit's shorter `rpg_bat_` is the alternative, types keep the existing `Rpg` prefix). No game text is quoted: enemy kinds, move and speaker roles are described in English.

## 1. What it is

A comic mini game in the style of a console RPG, run as main mode 0x81 on top of the dating sim. The player (the hero) fights with a girl beside him against one of six encounters. The hero has three commands (attack, guard, special) that fill and spend a gauge. The special move is the hero's club move, one of twelve, chosen by the club he belongs to. The girl is a second actor who acts on her own after a per-girl delay. The enemy acts on a timer. Only the hero's HP and the enemy HP pools are tracked; there is no experience, level or item system (the "experience", "gold" and "treasure" lines in the victory text are fixed or random text with no state behind them).

Unit size: 61 C files, 328 functions, `.data` 0x8015E4F0..0x80162000. The scene is mostly choreography; the rules are in about 25 functions (section 5).

## 2. Entry, launch codes, exit

- Entry: main loads RPG_BAT when `main_state_set(0x81)` runs; the per-frame entry is `rpg_battle_entry` (0x8013B5B0). The overlay table maps mode 0x81 to it ([[overlays]]).
- Launch code: the caller stores a code in main word `D_80122CE0` and saves its own mode, stage and step in `GameState.unk_720/721/722` before `main_state_set(0x81)`.

| Code | Caller | Encounter flag (`rpg_battle_encounter`) | Enemy | Partner rule |
| --- | --- | --- | --- | --- |
| 1 | SHUGAKU, destination value 1 | `RPG_ENC_SNAKE` 0x10000 | snake enemy, mongoose rescuer | id 13 (`GIRL_YOSHIO`) kept, else id 14 (`RPG_PARTNER_NONE`, behaviour: no partner) |
| 2 | SHUGAKU, destination value 0 | `RPG_ENC_DEER` 0x20000 | deer enemy, monk rescuer | same |
| 3 | SHUGAKU, destination value 2 | `RPG_ENC_BEAR` 0x40000 | bear enemy, hunter rescuer | same |
| 4 | DATE, `func_8013C7D0` (the two latest entries of the place history `unk_728` both equal the chosen place `D_800CA2F8`; that place is not 0; year byte `unk_03E` at least 0x60; flag b20 of `unk_0F4` clear; the launch sets flag b19 and clears the history) | `RPG_ENC_DELINQUENTS` 0x100000 | three delinquents | the current girl; ids 13 and 14 become 9 |
| 5 | DATE, `func_8013C7D0` (year byte `unk_03E` == 0x61 and month byte `unk_03F` == 9, flag b20 clear, and one of: a club slot 0..10 whose club flag `unk_25` is 3; flag b19 set, which the launch of code 4 leaves behind; no club points at all in slots 0..10, which also forces the club flag of slot 11 to 3; the launch sets b20 so it fires once and clears the place history) | 0x100000 and `rpg_battle_boss_event` | delinquents, then the gang boss | as 4 |
| 6 | GEKO | `RPG_ENC_HIMOO_ROBOT` 0x8000000 | Himoo's robot | id 14 (`RPG_PARTNER_NONE`) |
| 7 or other | SHUGAKU | `RPG_ENC_ALIENS` 0x80000000 | aliens | id 2 (Himoo) |

`D_800CA2F8` is the dating place chosen in DATE (21 places, see the DATE spec), not a girl; the girl is `GameState.unk_F5F`. The day is not tested by `func_8013C7D0`; whichever DATE step calls it decides when code 5 can happen. The function tests code 5 first (the club or b19 branch, then the no-club branch) and code 4 last, so code 5 wins when both match; when neither fires it just advances the DATE step.

The three SHUGAKU destination values are read from SHUGAKU's own word `D_800CA2CC` (0, 1 or other); which real place each value is stays with the SHUGAKU spec (the enemies suggest nara/deer, okinawa/snake, hokkaido/bear; HYPOTHESIS).

- Exit: `rpg_battle_exit` (stage 15) puts `rpg_battle_result_code` back into `D_80122CE0`, restores `GameState.unk_F5F` from `rpg_battle_saved_girl`, and calls `main_state_set` / stage set / step set with the saved `unk_720/721/722`. The result code is cleared to 0 at the start and stays 0 on every path that ends with the player standing (delinquent trio or boss beaten, robot beaten, trip rescue; the aliens' rescue script writes 0 explicitly). 1 is written when the player is knocked out (the delinquent and boss defeat paths) and when the aliens retreat because the player reached 1 HP. DATE reads it afterwards: with 0 the current girl gets +2 on her field `unk_02` (with launch code 5 the girls whose flag b1 is set get +3 and the current girl +2); otherwise the stress parameter (`unk_11C`) rises by 5. Other callers' use is in their specs.

## 3. Stages (`GameState.unk_1109`, one switch in `rpg_battle_entry`)

Before the switch: stages 6..9 run `rpg_battle_pause_update`; stages other than 9 call main `func_8006BA40` unless menu mode bit0; stages 5..11 draw a three-sided frame (left, bottom, right bars) around the battle view. After it: `func_8006BD6C(1 or 0)` by menu mode bit0.

| Stage | Constant | Does |
| --- | --- | --- |
| 0 | `RPG_STAGE_INIT` | `rpg_battle_stage_init` clears display state, maps the launch code to the encounter flag, `rpg_battle_init_player_state` sets start values |
| 1 | `RPG_STAGE_LOAD_1` | CD sound-data requests through main `func_80044890` (the encounter's block, chosen by girl group for the delinquent fight, then the club's block); substep `unk_110D` |
| 2 | `RPG_STAGE_LOAD_2` | CD graphics packs: encounter pack to 0x80180000, boss pack to 0x801B1000 (delinquents only), club pack to 0x801D8000 |
| 3 | `RPG_STAGE_ASSETS` | `rpg_battle_stage_asset_addresses` (RAM addresses of the loaded packs per encounter), `rpg_battle_stage_palettes_load` |
| 4 | `RPG_STAGE_SETUP` | `rpg_battle_stage_sprites_setup`, HUD setup, `rpg_battle_stage_labels_init` (text labels for the names), HUD digits |
| 5 | `RPG_STAGE_WAVE` | ripple transition (screen copy in VRAM, sine strips) |
| 6 | `RPG_STAGE_INTRO` | aliens: wipe and a Himoo line (`rpg_battle_stage_intro_aliens`); others: "enemy attacked" text and slide-in; music `RPG_BGM_BATTLE` |
| 7 | `RPG_STAGE_START_SHOUT` | start voice, wait for it |
| 8 | `RPG_STAGE_START_MUSIC` | aliens only: music call; then next stage |
| 9 | `RPG_STAGE_FIGHT` | `rpg_battle_stage_fight` (section 4) |
| 10 | `RPG_STAGE_END_SOUND` | sound command 0xC1, next stage |
| 11 | `RPG_STAGE_FADE_OUT` | fade level `unk_03A` and record levels from 0x80 down by 2 per frame |
| 12 | `RPG_STAGE_STOP_AUDIO` | main CD-state reset and stop |
| 13 | `RPG_STAGE_SCREEN_RESTORE` | main `func_800674B0`, next stage |
| 14 | `RPG_STAGE_WAIT_AUDIO` | next stage when the sound-bank poll (main `snd_bank_load_poll`, `func_80044E8C`) returns 1; counter `unk_1100` counts frames and past 0x400 the audio is stopped and the stage is set back to 13 (a retry loop through `func_800674B0`), not forward |
| 15 | `RPG_STAGE_EXIT` | `rpg_battle_exit` |

Stage 9 ends when a victory or defeat script calls main `main_stage_next` (`func_80042808`: stage + 1, clears step and counters); `main_step_next` (`func_8004284C`) is step + 1, `main_stage_set` (`func_80042908`) and `main_step_set` (`func_80042940`) set stage and step. These are the names the core table proposes; main owns them.

## 4. The fight loop (stage 9) and its state

Per frame, in order (`rpg_battle_stage_fight`): wave background; in code-entry mode (pause state 3) only the girl timer, girl update and message run; skip-message pad bit 0x40; label switching for the robot fight; the three idle counters (below); girl update; defeat check; player update; defeat check; enemy update; defeat check; victory dispatch (`rpg_battle_start_victory_sequence`); HUD digits; message window; the four damage popups; alien background flicker (aliens only).

Idle counters (each +1 per frame while the relevant action words are idle): `rpg_battle_player_ready_timer` (D_8015ED9C; while the player action word has none of the bits 0xFF0111FF), `rpg_battle_enemy_timer` (D_8015EDC8; while the low bytes of the player, enemy and girl words are 0 and the enemy word lacks 0x1000), `rpg_battle_girl_timer` (D_8015EDF4; same, and the girl word lacks 0x1000 and 0x80000000).

### Script channels

All motion is hand-written step scripts. Five independent channels each hold step, substep and timers and use the same helper set (next step, next substep, wait n frames, wait for an actor animation to end, wait for the message to end):

| Channel | Step / substep / timers | Helpers (new names) |
| --- | --- | --- |
| player | `rpg_battle_player_step` D_8015EDAC, `rpg_battle_player_channel` D_8015EDB0 (substep, timer, timer 2) | `rpg_battle_player_step_next`, `_substep_next`, `_wait_step`, `_wait_substep`, `_wait_anim_*`, `_wait_msg_*`, `rpg_battle_player_action_end` |
| enemy | `rpg_battle_enemy_step` D_8015EDD8, substep D_8015EDDC, timers D_8015EDE0/E4 | `rpg_battle_enemy_*` |
| girl | `rpg_battle_girl_channel` D_8015EE00 (frame counter, step, substep, timer), timer 2 D_8015EE10 | `rpg_battle_girl_*` |
| finale (victory script) | `rpg_battle_finale_channel` D_8015EE30 (step, substep, timers) | `rpg_battle_finale_*` |
| voiced dialog | `rpg_battle_dialog_step` D_8015ED8C[0] (step), [1] (waiting flag) | `rpg_battle_voice_play_once`, `_wait`, `_reset` |

HUD: three number slots (player HP, the partner's value, the guest/rescuer value) drawn as four digit sprites each by `rpg_battle_hud_numbers_update`, with a name label beside each; a slot is hidden when its value is above 0x10000000. Other state machines: the message window (`rpg_battle_message` D_8015EE44: state, mode, frame counter; `rpg_battle_message_show` sets the line pointers, line count and frames per line), the command menu (`rpg_battle_menu` D_8015EE50), four damage popups (`rpg_battle_popup_0..3`).

Actors are rows of the shared 0x44-byte display-record table `D_8011ECD0`; scripts address them as "actor i" = record 96 + i (`rpg_battle_actor_set_anim`, `_set_pos`, `_is_idle`; positions are screen coordinates minus (0xA0, 0x78)). Fixed ids: girl 0x1D, player 0x30, hit effect on the player 0x38, enemy actors 0x3B..0x3D, girl effect 0x32; during a special move the hero is shown by actor 0x34 (with 0x33 as a second sprite) instead of 0x30. Record 64..159 field `unk_07` is the fade level (`rpg_battle_fade_set_level`). Field names of that record type belong to the graphics unit.

### Action codes

- Player (`rpg_battle_player_action`, D_8015ED98): low bits 1 attack, 2 special, 0x100 guard, 0x104 guard during the heal sequence (set after 151 guard frames; the guard bit 0x100 stays set), 0x1000 down, 0x10000 command-ready; request bits 0x01000000 attack, 0x02000000 guard, 0x04000000 special set by the menu and consumed by `rpg_battle_player_update`.
- Enemy (`rpg_battle_enemy_action`, D_8015EDC4): high half = encounter group (0x10000..0x40000 trips, 0x100000 delinquents, 0x200000 and 0x400000 boss rounds, 0x8000000 robot, 0x80000000 aliens), low byte = action number; `rpg_battle_enemy_update` runs the matching function. Action numbers per group are in section 5.5.
- Girl (`rpg_battle_girl_action`, D_8015EDF0): 0 none, 1..3 her action number, 0x1000 done for good (girl 1), 0x40000000 reacted (girl 9), 0x80000000 pending (girl 1), 0x10001000 and 0x10001001 the alien survival timer and rescue.

## 5. Rules

### 5.1 Start values (`rpg_battle_init_player_state`)

- Club: scan the 12 club words `GameState.unk_66C`; the club whose bit field `unk_25` equals 3 is the player's club (`rpg_battle_player_club`); slot 11 (no club) is kept only if all twelve words have an empty `unk_00` part, else the club becomes 0xFF. A club of 0xFF disables the special; so do all trip encounters and the aliens (mask 0x800F0000). With a club move: gauge starts at 100 and the special is ready.
- Partner id (`rpg_battle_girl_id`, from `unk_F5F`): trips keep 13, everything else there becomes 14; non-trips remap 13 and 14 to 9; aliens use 2; the robot fight uses 14. Id 14 means no partner here (no HP shown, no actions; `RPG_PARTNER_NONE`, HYPOTHESIS that this reuses the id of another person: the shared `GirlId` table names id 14 `GIRL_SOTOI`, the butler).
- Player HP and max HP: parameter 0 (`unk_0FC.unk_02`) times 10, at least 10.
- Partner HP value (`rpg_battle_girl_hp`, shown in the HUD; only girl 13 changes it): ids 0..10: 256, 99, 312, 304, 189, 298, 1878, 277, 365, 173, 265; id 13: 573; id 14: 0x7FFFFFFF.
- Enemy pool: trips 1/1; robot 5000; aliens 0x7FFFFFFF (scale 1000); delinquents 0x10000 (boss rounds). Three delinquent gauges: 5000 each with `rpg_battle_enemy_damage_scale` 1000 when launch code 5, else 9999 each with scale 1500.
- Enemy action word starts as the encounter flag; the robot starts with action 2 (arrival script). Ready timer starts at 0xFFFF (ready at once) except the robot fight, where the player starts down (0x1000, timer 0) until the arrival ends.

### 5.2 Player commands

- The command menu opens when the ready flag 0x10000 is set. The flag is raised when `rpg_battle_player_ready_timer` exceeds 120 (`rpg_battle_player_ready_delay(0)`) and the timer restarts at 0, so the normal wait is 120 frames of no action; ending a guard instead sets the timer to a random 60..128 (30..88 in the aliens fight), which shortens the next wait. Items shown: attack and guard, plus special when `rpg_battle_special_ready`. The choice toggles a request bit; the update turns it into the action word.
- Gauge `rpg_battle_special_gauge` (0..170): attack +30 + rand(15); guard +20 + rand(15); special costs 100. The special becomes ready when the gauge is at least 100 and a club move exists; it is spent on use. Aliens: every attack also adds 600 frames to the survival counter (`D_8015EE00` word 0).
- Guard (action 0x100): enemy damage is reduced (divisors in 5.5); in the aliens fight each guarded hit also raises `rpg_battle_enemy_damage_scale` by 100. After 151 frames of guard with no enemy or girl action the action word becomes 0x104 and the player heals 5 percent of max HP (`rpg_battle_player_guard_heal`, at least 1) while the heal message and effect play; then the action ends.
- Attack (action 1): target slot, damage roll, then `rpg_battle_player_attack` with a flurry of hit flashes (1 to 10 hits by damage tier: <200, <500, <800, <1100, <1300, 9999 gives 10, else 6).

### 5.3 Target slot (`rpg_battle_pick_target`)

Trips: slot 16/17/18 by encounter (numbers only, no HP). Delinquents: attack action: slot 1 with 1/15, slot 2 with 1/15, else slot 0 (13/15); other actions: slots 1 and 2 with 1/4 each, else 0. Boss rounds 3; robot 4; aliens 5. The slot gives the target rectangle for effects (`rpg_battle_set_target_pos`).

### 5.4 Player damage

Basic attack (`rpg_battle_player_basic_damage`): lv = min(athletics, 300) (`unk_10C.unk_02`); base = rand(100) + rand(100) + lv x 9 / 2; roll r = rand(4096): r == 0 gives 9999, r < 64 gives 0, r < 270 gives base + rand(500) + rand(1000), else base; capped at 9999. In the boss rounds with no club move any non-zero hit also removes 1500 from the boss pool.

Special (`rpg_battle_player_damage_roll`): club points c = word `unk_66C[club].unk_00` clamped to 50..550; base = c x 10 - 500 (0..5000). "Trio" means the delinquent encounter (three damage values, the target slot listed gets the stated bonus); `r(n)` is rand(n); `r2(a,b)` is rand(a) + rand(b). All values are capped at 9999.

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
| none (11) | tiered by `rpg_battle_escalation_counter` n: n 0..1 r2(500,500); 2..3 r2(1000,1000); 4..5 r2(1500,1500); 6..7 r2(2000,2000); 8..15 r2(2500,2500); 16 or more 9999 (and, outside the trio, 30000 off the pool). The roll also writes the tier index back into the counter (0, 1, 2, 3, 4, 5 for the six ranges); the move's animation and sound variant are chosen from that index, and the move's last step (step 9) sets the counter to 0. Enemy attacks then count it up again |

A special in the trips or the aliens fight is not possible; in a non-special action the basic roll is used. Each special has a fixed sequence (approach, banner, 3 to 36 animation steps, damage popups, return) with the girl's own comment inserted at steps 6..18 (section 5.6).

### 5.5 Enemy side

Common: the enemy waits `rpg_battle_enemy_action_threshold` frames: trips 330; robot 360; the others 150 + `rpg_battle_enemy_delay_extra` (re-rolled to rand(20) + rand(40) at every choice). When the timer passes it and nobody acts, an action is chosen (weighted, below); a result with low byte 0 means "wait", the timer is then set to a random head start rand(60) + rand(90). The guts factor (225 - min(guts, 150)) / 150 (`unk_118.unk_02`, 1.5 at guts 0, 0.5 at 150) and the scale `rpg_battle_enemy_damage_scale` / 1000 are applied only where stated below; trips, the robot and the cover attack use plain fractions of HP.

Trips (actions chosen by `rpg_battle_enemy_pick_trip`): attacks 1 and 2 are the enemy's named basic attack, damage = 10 percent of current HP (1 percent when guarding); the third and later choice is action 2, the finale: the enemy bites for HP - 3, the rescuer appears and wins, victory text, result 0. The player cannot be knocked out and cannot hurt the enemy.

Delinquents (`rpg_battle_enemy_pick_delinquent`, weights in `RpgBattleState` slots 0..3, each used one -5): start 15/15/5/30 with the boss event, else 10/10/0/40, refilled when their sum drops below 21. A roll in [0, sum + miss weight): slot 0 gives action 1 (taunt line, no damage); slot 1 gives 0 (wait); slot 2 plus `rpg_battle_enemy_miss_weight` gives action 3 (punch misses; miss weight -20) or 4 when the chosen attacker is charmed; slot 3 gives the real attack (weight -5, and with launch code 4 the scale rises by 25 each time): charmed attacker: action 4; else 5 percent (unless the girl is in state 0x40000000): action 6 (the player covers the girl, damage = girl HP value / 10, divided by 10 more when guarding); else 5 percent (not when the player action word is exactly 0x100, so it can still happen in the heal state 0x104): action 5 (small hit r2(10, 10) x scale / 1000, then the player is down for a moment); else action 2 (damage r2(125, 75) x guts factor x scale / 1000, divided by 10 when guarding). Each real attack adds 1 (action 6: 2) to `rpg_battle_escalation_counter`. The attacker is a uniformly random slot 0..2. Action 7 is entered when an action-2 punch leaves the player at 0 HP: down, defeat taunt, result 1. Slots whose gauge is 0 are not excluded; the fight ends as soon as any one of the three gauges is 0 (`rpg_battle_check_enemy_defeated`). Charmed (girl 7): a slot flag that makes that delinquent's real attack hit another delinquent (r2(100, 100)); it is cleared when the slot takes damage; if all three are charmed the girl's chooser returns 0.

Boss rounds (launch code 5, group 0x200000 then 0x400000): the four weights are zeroed when round 1 starts, so the first choice refills them. Round 1 (`rpg_battle_enemy_pick_bancho_1`): if the weights sum to under 11 they are refilled to wait 20, taunt 15, attack 20; the roll gives action 0 wait (slot 0), 1 taunt (slot 1) or 2 attack (slot 2), the picked slot loses 5 and an attack adds 2 to `rpg_battle_escalation_counter`. Before that, when the boss pool is below one third of its maximum, the weights are set to 0 / 5 / 15 and action 3 (turns serious, starts round 2) is returned. Round 2 (`rpg_battle_enemy_pick_bancho_2`): same scheme, refill at a sum under 11 to wait 25, taunt 15, powered attack 15; actions 0, 1 and 4 (powered attack, counter +2). Damage by `rpg_battle_enemy_damage_roll`, with M the player's max HP: attack 2: r(M x 25 / 100 - 500) + 500, attack 4: r(M x 50 / 100 - 1000) + 1000, both x guts factor x scale / 1000, guarding divides by 3 and 10. The taunt line index is `pool x 6 / max` (six HP bands). Player at 0 HP: action 5, result 1 and defeat. Boss pool 0 ends it with the hand-over scene, loot text and a random title, result 0.

Robot (Himoo, group 0x8000000): action 1 is always chosen (`rpg_battle_escalation_counter` +2): three announced attacks in a row, then one damage roll: 20 percent of the current HP (2 percent guarding, 0 below 20 HP; 50 percent when the player is already down), so the player cannot die. The pool is 5000; the robot dies to damage; the arrival script is action 2 at the start.

Aliens (group 0x80000000, chooser `rpg_battle_enemy_pick_alien`: weight slots 0..3 map to wait, chatter (4), attack 1, attack 2; start 15/5/10/10 when their sum is under 6): attack 1: r2(M/10, M x 5 / 100) + 100, attack 2: r2(M/10, M/10) + 150 (M = the player's max HP), both with the guts factor and the scale; guarding halves the hit but raises the scale by 100; every hit is clamped to leave the player 1 HP; at 1 HP the enemy action becomes 3 (aliens flee, result 1). The pool is reduced by damage like the others, but the defeat check does not cover this encounter, so it never ends the fight. The girl channel counts frames: after about 10800 frames (3 minutes; each attack adds 600) the girl action becomes the rescue script (Himoo destroys the aliens with 9999, a closing line, result 0).

### 5.6 Girl (partner) behavior

Cooldown before she may act (`rpg_battle_girl_action_delay`): id 0 200; 1 10; 7 240; 9, 6, 8, 10: 260; 4, 2, 3, 5: 300; 13: 110 when her HP value is under 10, else 170; 14 never. The choice (`rpg_battle_girl_pick_action`): weights wait/act both 50 (100/100 for ids 1, 13, 14), each choice -5, refilled when the sum is under 20; when the result is 0 the girl timer is set to a random head start rand(100) + rand(50) (so the next try comes sooner). Her action runs only while nothing else acts.

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

During the hero's special move (steps 6..18) the girl's own "assist" function (`rpg_battle_move_assist_dispatch`, ids 0..10) makes her run in, speak a line, and run out; this is presentation only.

### 5.7 End conditions

- Delinquents: any gauge 0 -> `rpg_battle_victory_delinquents` (text, loot of 2 or 3 names weighted 4:3:2:1 over four names, result 0). Launch code 5 then continues: unseen voice, boss entrance and speech, the player is dropped to 0 HP, a voice restores HP to the old max and raises max HP by 2000 (cap 9999), round 1 starts with the pool at 0x10000.
- Trips: after the finale script. Robot: pool 0 -> `rpg_battle_victory_robot` (fade to white), result 0. Aliens: see 5.5.
- The player cannot lose in trips, the robot fight or the alien fight; he can lose to the delinquents and the boss (result 1).

### 5.8 Pause and hidden code entry

Pad bit 0x800 (START in the PsyQ pad layout, where PADstart is bit 11; see section 8 for the conflict with core's `PadBits`) toggles a pause (`rpg_battle_pause_state` 0 run, 1 paused, 3 code entry). In pause and in state 3 two ten-button sequences are matched against the pad word (`rpg_battle_code_sequence_a`: up, up, down, down, left, right, left, right, cross, circle; `_b`: down, down, up, up, right, left, right, left, circle, cross). State 3 is entered from stage 9 only when the partner id is 9 and everything is idle. Sequence a: trips set HP and max HP to 0x1662 when HP >= 4 (the robot fight also sets the enemy pool to 1); the delinquent fight sets HP and max to about 5730 when HP is 1..9, and for HP >= 10 adds 1 to both (cap 9999); the aliens do the same from HP 2. Sequence b: the delinquent fight sets HP to 1; the aliens to 2. A debugging-style code; the port may keep or drop it (HYPOTHESIS: developer feature).

## 6. Data tables

| Address | New name | Content |
| --- | --- | --- |
| 0x8015E4F0 | `rpg_battle_spark_dirs` | eight (dx, dy) steps at 45 degrees, 0x100 / 0xB5 units |
| 0x8015E530, EFA0, EFB0, EFC0, EFE0 | `rpg_battle_loot_names_*` | four string pointers each (decor text only) |
| 0x8015E550, E578 | `rpg_battle_code_sequence_a/b` | ten pad words each |
| 0x8015E5A0 | `rpg_battle_popup_bounce_table` | 16 rows of 12 s16 offsets, row = popup frame / 2 |
| 0x8015E720 | `rpg_battle_menu_item_positions` | six (x, y) pairs for the three menu items |
| 0x8015E74C.. | `rpg_battle_text_player/enemy/girl/result_*` | 40-byte line buffers in four sets (5, 5, 5, 10) with pointer tables E814, E8F0, E9CC, EB70 |
| 0x8015EE8C..EEEC | `rpg_battle_asset_*` | pointers filled at stage 3: sheets (6), frame tables (6), animation tables (6), palettes (7) |
| 0x8015EEF0 | `rpg_battle_drama_lines` | 41 line pointers (the drama move shows three different ones) |
| per-girl constants | in code | girl HP values, delays, damage ranges (sections 5.1 and 5.6) |

## 7. Connections

- Reads `GameState`: `unk_0FC.unk_02` (param 0, HP), `unk_10C.unk_02` (athletics), `unk_118.unk_02` (guts), `unk_66C[12]` (club words), `unk_F5F` (girl), `unk_0D4` (player name string), `unk_F88` (pad edge word), `unk_1092/1093` (menu mode and choice), `unk_1109/110A/110D/1100/1104` (stage, step, substep, counters), `unk_720..722` (return mode), `unk_03A` (fade level). Writes `unk_F5F` (temporarily 14 for voice calls, restored at exit) and the stage counters. Reads girl state through `get_g_zyotai_s(6)`.
- Main helpers used (names owned by main): `rand`, `strcpy/strcat`, `rsin`, `cd_read_sectors`, voice and sound commands (`func_80044750`, `func_80046290`, `func_800460EC` status 4 playing / 2 done), music (`func_8004500C`), text labels (`set_kanji_string`, `k_disp_*`, `k_sub_reset`), menu (`menu_set`, `menu_check`), palette and texture loads, GPU fill helpers (`func_80049A40`, `dtd_on_tpage`).
- Hardware layer (module level only): sprites are display records; CLUT/texture loads (`load_palette`, `load_tpage_buf_lock`); VRAM copies for the ripple (`func_8009C93C`); sound-bank CD loads (stage 1); voiced lines through the CD voice path (`rpg_battle_voice_*`; 44 `rpg_battle_dialog_*` scripts pair one text line with one voice clip); GPU quads for the wave and wipes (`rpg_battle_wave_*`). The port replaces these; the logic above does not depend on them except through the status values stated.
- Disc: `BATTLE/` holds one pack per encounter, club and girl (names of the form `VS_<club|girl|enemy>`), loaded at stages 1..2 and 4; the SERIFU voice file for this scene is read through the voiced-dialog path.

## 8. Open questions

- Which real SHUGAKU destination each launch code 1..3 stands for (needs the SHUGAKU spec).
- Whether partner id 14 (`RPG_PARTNER_NONE`) is meant as "no partner" or another person; core names id 14 `GIRL_SOTOI`. The HUD hides its HP only through the 0x7FFFFFFF sentinel.
- `rpg_battle_guest_value` (D_8015EE18): the number shown for the rescuer slot (trips) is 0x10000000-flagged and revealed by an XOR in the finale; its exact HUD role is not traced.
- Result code polarity is inferred from the paths and DATE's use; GEKO and SHUGAKU callers were not re-read.
- Word `D_8015EBD8` (enemy damage scale) has an effect when guarding raises it; whether this is an intended "turtle penalty" is a guess.
- The BGM ids 0x200..0x204 are named from call context only. The OPTION sound test lists one RPG music name (`RPG_BTL`) and the effect banks `RPG_CLB1..4`, `RPG_GAL`, `RPG_GAL1..3`, `RPG_HOK`, `RPG_NAR`, `RPG_OKI`, `RPG_ROBO`, `RPG_SYU`, `RPG_UFO`; if 0x200..0x204 are sequences of that one music bank, the five guesses collapse into one developer name plus an index. Not traced. The bank names HOK, NAR and OKI also back the guess that the three trips are Hokkaido, Nara and Okinawa.
- Pad layout: this unit's two code tables use the PsyQ layout (up 0x1000, right 0x2000, down 0x4000, left 0x8000, triangle 0x10, circle 0x20, cross 0x40, square 0x80; checked against the data words) and the pause test reads 0x800. Core's `PadBits` has `PAD_START` 0x0008 and `PAD_R1` 0x0800 next to the same direction bits, which mixes the raw hardware order with the PsyQ order. PsyQ defines PADstart as bit 11, so core's table is probably the wrong one; this could not be confirmed inside the repo (the 0x800 meaning here stays HYPOTHESIS: START). To be reconciled before names are applied.
- Emulator: the scene was not run; reaching it needs a save at a DATE or SHUGAKU trigger point (tools/emu has no memory poke).
