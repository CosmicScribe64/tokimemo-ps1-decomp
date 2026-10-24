---
type: spec
updated: 2026-10-24
sources: ["src/ovl/TACO", "config/renames/proposals/o-taco.tsv", "include/ovl/TACO.h"]
---

# Overlay TACO: the shooter mini game (port spec)

Phase 2 spec for [[tickets/T-27028-p2-o-taco]] ([[tickets/T-27000-phase-2]]). Facts as the matched C implements them. Names are the proposals in `config/renames/proposals/o-taco.tsv` (prefix `shooter_`, stated as the knowledge base proposes it: the glossary seed lists the mini game as "shooter"; types use `Shooter*`, constants `SHOOTER_*`, boss code and tables `shooter_boss1_`..`shooter_boss3_`). The old address is given in brackets where it helps; the overlay is still called TACO on disc and in the splat segment. Everything marked HYPOTHESIS in the table is a guess about game meaning the code does not prove. No game text, assets or screenshots are reproduced; labels are described by role.

## 1. What it is

A three-stage 3D rail shooter reached from the extras (overlay mode 0x90, entered from OPTION when the unlock flag is set, and from MASTER, [[overlays]]). The player flies a ship into the screen, with a second computer-controlled ship (the wingman, actor slot 8) beside it. Each of the three stages is a scripted wave sequence followed by a boss. There are three weapon families (primary, secondary, bombs), pickups dropped by enemies, a 5 minute timer for the whole run, a score with three saved high scores and three-character names, and settings for auto fire and button assignment that persist in the system save. The name TACO is unexplained (HYPOTHESIS, no evidence in code).

Frame rate 60. All timers below are in frames. The screen is 320x240; HUD positions are offsets from the display origin kept in `GameState.unk_014/unk_018[buffer]`.

## 2. Entry and flow

`shooter_entry` (func_80132000) runs once per frame. It adds 1 to `GameState.unk_1100` (frames in this stage; reset by every stage change), switches on `unk_1109` (the stage), then runs two hooks: `shooter_frame_update` (background, camera, load requests, SE cooldowns) and `shooter_draw_models`. Inside a stage `unk_110A` is the step, `unk_110D` a sub-step, `unk_1104` the frame count of the step. Main helpers (names not yet proposed in this unit): func_80042908(n) sets stage n and clears step, sub-step and both counters; func_8004284C advances the step; func_80042940(n) jumps to step n. Unknown stage numbers call func_80046500.

| Stage | Name | Steps | What it does | Next |
| ---: | --- | ---: | --- | --- |
| 0 | `SHOOTER_STAGE_LOAD` (shooter_stage_load) | 5 | Reads about 40 file regions from the disc into the overlay's RAM and VRAM areas (textures, palettes, models, and the sound banks 0 and 1 through the main sound-bank loader), builds the 64 coordinate objects and model handles, loads settings and scores. A loading label blinks every 16 frames and a countdown from 1024 is drawn for the first 1023 frames. Sets the actor pool pointer to D_8011ED58. | 1 |
| 1 | `SHOOTER_STAGE_OPENING` (shooter_stage_opening) | 5 | Opening animation: two models fly through pose keys over an animated sky gradient and star dots. START, circle or cross jumps to step 4, which hides the models and goes to stage 2. Scene sounds play at fixed frames of steps 1 to 3 (bank 6 ids 0x605, 0x606, 0x607; section 11). | 2 |
| 2 | `SHOOTER_STAGE_TITLE` (shooter_stage_title) | 6 | Title animation with a growing star field: two title models run a pose script (frame limits 0x20, 0x54, 0x68, 0x72, 0x7C, 0x9A, 0xAE, 0xEA), lights ramp, four scene sounds at stage frames 0x40 (0x605), 0x80 (0x606), 0xA0 (0x604) and 0xC0 (0x607). Step 1 waits 209 frames (any of START, circle, cross skips to step 5), step 2 reveals a palette randomly, step 3 holds 255 frames, step 5 reloads the palette. | 3 |
| 3 | `SHOOTER_STAGE_MENU` (shooter_stage_menu) | 3 | Five-item menu (start, settings, scores, help, exit). Up, down or select moves (wraps), circle or START confirms with a 65 frame delay. Idle for 901 frames in the stage starts the demo. | 10, 4, 5, 7, 46 by item, or 6 on idle |
| 4 | `SHOOTER_STAGE_SETTINGS` (shooter_stage_settings) | 3 | Settings page, section 6. | 2 |
| 5 | `SHOOTER_STAGE_SCORES` (shooter_stage_scores) | 8 | Ranking page with stars and a zoom in and out. | 2 |
| 6 | `SHOOTER_STAGE_DEMO` (shooter_stage_demo) | 2 | Demo play with scripted input, section 12. | 2 |
| 7 | `SHOOTER_STAGE_HOW_TO_PLAY` (shooter_stage_how_to_play) (HYPOTHESIS: help pages) | 4 | Two text pages with voice lines; text scrolls. Cross returns at any time. | 2 |
| 8 | `SHOOTER_STAGE_RESULT` (shooter_stage_result) | 3 | Score tally, section 9. | 9 or 5 |
| 9 | `SHOOTER_STAGE_NAME_ENTRY` (shooter_stage_name_entry) | 6 | Name entry for a new top-3 score, section 9. | 5 |
| 10 | `SHOOTER_STAGE_GAME_START` (shooter_stage_game_start) | 3 | New game set-up: clears score and stage count, sets timer 0x4650, links the 79 actors, resets ships, waits for the load request to finish, loads the cursor and font banks, sets player HP 0xFF. | 11 |
| 11 | `SHOOTER_STAGE_STAGE1` (shooter_stage_stage1) | 3 | Stage 1 play. | 12 |
| 12 | `SHOOTER_STAGE_BOSS1` (shooter_stage_boss1) | 4 | Boss 1 (init, entrance, fight, defeat). | 21 |
| 21 | `SHOOTER_STAGE_STAGE2` (shooter_stage_stage2) | 3 | Stage 2 play. | 22 |
| 22 | `SHOOTER_STAGE_BOSS2` (shooter_stage_boss2) | 4 | Boss 2. | 31 |
| 31 | `SHOOTER_STAGE_STAGE3` (shooter_stage_stage3) | 3 | Stage 3 play. | 32 |
| 32 | `SHOOTER_STAGE_BOSS3` (shooter_stage_boss3) | 5 | Boss 3 (init, descent, fight, defeat, collapse). | 45 |
| 45 | `SHOOTER_STAGE_GAME_END` (shooter_stage_game_end) | 3 | End banner (clear when 3 or more stages are counted, time-out when the timer is 0, else defeat), the screen wipes in over a snapshot of the last frame; a button press then a sound-bank-ready check go on. | 8 |
| 46 | `SHOOTER_STAGE_EXIT` (shooter_stage_exit) | 8 | Leave: write settings and scores back into the work area, stop sound, write the system file (HYPOTHESIS: memory card), reload main data (steps shared with other overlays), load day data, return to the calling mode. | main |

The three regular stages and the bosses are 11, 12, 21, 22, 31, 32. A stage label (first, second, third stage; boss labels) is shown during the first 96 frames in the stage in blinking form. The playing stages all run the same frame function inside their step 1: `shooter_update_stageN_script` (the spawner), then in every playing step `shooter_draw_hud`, `shooter_update_battle` and `shooter_check_battle_end`.

Typical run: 0, 1, 2, 3, 10, 11, 12, 21, 22, 31, 32, 45, 8, 9 (if the score beats rank 3), 5, 2. START during play (pressed edge, mask 0x800) ends the run into stage 45.

## 3. Data model

### 3.1 Actor pool (`shooter_actors`, D_8015EDB4, `ShooterActor` x 79, 0x88 bytes each)

Slots: 0 player ship, 8 wingman, 16 to 62 enemies and boss parts (spawn and hit loops use 16..62; some clear loops run to 63). Slots 1 to 7 and 9 to 15 are not used by the code; slots 63 to 78 only appear in the pool-wide loops (the pool ends at byte 0x29F8). Fields (proposed names in the table):

| Offset | Name | Meaning |
| ---: | --- | --- |
| 0x00 | flag_bytes | byte 1: 0x80 active and shootable, 0x40 contact damage only (cannot be hit), 0x10 set while dying |
| 0x02 | mode | player: bit0 roll left, bit1 roll right, bit2 hit flash, bit6 bomb running (controls frozen), bit7 invulnerable; enemy: behavior id; boss controllers: step or key index |
| 0x03 | phase | player: roll speed counter; enemy: 1 alive, 2 dying; boss controllers: pace counter |
| 0x04 | model | object handle (graphics library object) |
| 0x14 | coord | coordinate object (0x50 bytes, local matrix and world matrix) |
| 0x64, 0x66, 0x68 | rot_x, rot_y, rot_z | rotation, 4096 per turn; rot_z is the player's roll angle |
| 0x6A | param | spin axis, formation index or ring angle |
| 0x6C, 0x6E, 0x70 | scale_x, scale_y, scale_z | 0x1000 is 1.0; the player uses 0x400 |
| 0x72 | weapon | primary weapon: -1 vulcan, 0 wide shot, 1 beam |
| 0x74, 0x76, 0x78 | x, y, z | world position |
| 0x7A | counter | one 16-bit counter with three jobs, so the name is neutral. Enemy: frames since spawn, 0x7FFF marks a new enemy; player: hit timer (starts at 0x10); boss part: number of HP bars (set from the part table when the part is attached; `shooter_boss_damage_percent` multiplies the table HP by it, and the bars-used counter at 0x82 is compared against it) |
| 0x7C, 0x7E, 0x80 | arg_x, arg_y, arg_z | player: bomb slots 1, 2, 3; enemy: spawn argument (path centre, speed, phase) |
| 0x82 | aux | player: secondary weapon (0 homing, 1 spread, -1 none); boss part: HP bars used; enemy: key or pose index |
| 0x84 | stats[4] | radius in units of 16, HP, kind (enemy type; also the score class), hit flags (bit0 hittable, high nibble drop code) |

World axes (derived from the pad handling): +x is screen left (pad left raises x), +y is down, the ship sits near z 0x800 and enemies come from z -0x8000 toward +z; things are removed past z 0x1000 (enemies) or 10001 (enemy bullets). Ship limits: x -0x1000..0x1000, y -0x800..0x180.

### 3.2 Particle tables (`ShooterParticle`, 0x14 bytes, 16 records each)

Fields: x, y, z (s16), w (s16 spare), vx, vy, vz (s16), a spare s16, spare byte, arg byte, age byte, flags byte (0x80 active, 0x20 beam anchor, 0x10 player side, low nibble kind). The hit spark and explosion tables are declared with a second view of the same 0x14 bytes (`ShooterParticleView`: position and velocity as two `ShooterPos`); the two views stay separate types until a code pass unifies them (section 14).

| Table | Address | Use |
| --- | --- | --- |
| shooter_player_primary_shots | D_8015E270 | vulcan, wide shot or beam of the player |
| shooter_wingman_primary_shots | D_8015E3B0 | same for the wingman |
| shooter_player_secondary_shots | D_8015E4F0 | homing or spreading shots |
| shooter_wingman_secondary_shots | D_8015E630 | same for the wingman |
| shooter_bomb_shots | D_8015E770 | blast bomb shots; record 0 is the blast centre |
| shooter_hit_sparks | D_8015E8B0 | spark images; element 0's w holds a ring index |
| shooter_explosions | D_8015E9F0 | explosions with velocity; ring index in element 0's w |
| shooter_enemy_bullets | D_8015EB30 | enemy bullets |
| shooter_items | D_8015EC70 | pickups (also the end marker of the previous table walk) |

`shooter_clear_particle_tables` zeroes the nine tables at stage start (0x140 bytes each).

### 3.3 Other state

| Name | Address | Meaning |
| --- | --- | --- |
| shooter_score | D_8015EDF0 | run score; the HUD shows score + 1,000,000 x shooter_stages_cleared; the last decimal digit is forced to 1 on every frame where `shooter_run_collisions` reaches its score tick (score = score/10*10+1, also capped at 9,999,999); the tick is skipped while a boss is already counted as beaten (stage 12 with cleared != 0, stage 22 with cleared >= 2, stage 32 with cleared >= 3), so the digit is not forced then |
| shooter_time_left | D_8015EDEC | frames left, starts at 18000 (5:00); counts down only while shooter_stages_cleared is below 3 |
| shooter_stages_cleared | D_8015EDCC | +1 when each of boss 1 and boss 2 reaches its defeat condition, +2 for boss 3 (once when its fight ends, once when the collapse ends) |
| settings | D_8015EDD8, EDDC, EDE0, EDE4 | auto shot (0/1), roll button (0..2), shot button (0..2), bomb button (0..2) |
| high scores | D_8015EDF8, EDFC, EE00 and names D_8015EE04, EE08, EE0C | defaults 3,057,300 / 2,057,300 / 1,057,300 with three default initial strings |
| shooter_load_request | D_8015EDBC | asset load request code, section 11 |
| shooter_background_flags | D_8015EDB0 | background layers, section 11 |
| stage script | D_801604C0 (timer), C4 (phase), C8 (sub timer), CC (counter), D0 (shooter_circler_pairing) | spawner state |
| bomb state | D_80160460 (pulse bomb active), D_80160464 (pulse timer) | section 5.3 |
| enemy tables | D_80161D9C (coordinate pool owners, 64 bytes), D_80161DDC, E20, E60, EA4, EC8 | per enemy type: model address, joint count, joint pose table, initial scale, radius |

Main objects the scene uses (not renamed here, they belong to the main unit): D_80127080 (64 model handles), D_80127480 (64 coordinate objects), D_80128880 (rotation angles), D_80122740.. (camera) and D_80122760.. (three lights), D_80129C40 (reference matrix copied into 13 draw functions), D_800E8CA0 (ordering table), D_8011ECA0 (current frame buffer index), D_800E6280.unk_163C (shared scratch: star field, title star dots), unk_036/037/038 (background clear colour RGB).

## 4. Controls and the per-frame update

Pad words: `GameState.unk_F80` is the held mask and `unk_F88` the newly pressed mask. Bit values are the standard PlayStation pad bits (L2 1, R2 2, L1 4, R1 8, triangle 0x10, circle 0x20, cross 0x40, square 0x80, select 0x100, start 0x800, up 0x1000, right 0x2000, down 0x4000, left 0x8000). The word is built in main as (~byte2 << 8) | ~byte3 of the pad buffer, which puts the shoulder and face buttons in the low byte. The skip mask 0x860 in the opening and title is circle, cross and START.

`shooter_update_battle` (func_80132FE8), once per frame in stages 10, 11, 12, 21, 22, 31, 32 and in the demo:

1. If the player's mode has bit 6 (a bomb is running), steps 2 to 4 are skipped: movement, wingman and shot updates are frozen and shots in flight stop (the bomb call then receives an unset fire flag, which it does not read in that state).
2. `shooter_update_player`, `shooter_update_wingman`, `shooter_update_secondary_shots`, `shooter_update_primary_shots`.
3. Fire flags: bit 0x20 = fire, bit 0x40 = bomb. With auto shot on, the held shot button sets fire on frames where `unk_10F8 & 3` is 0 (every 4th frame); with it off, a newly pressed shot button fires once. The bomb button always uses the pressed edge. Shot button by setting: 0 circle, 1 cross, 2 square. Bomb button by setting: 0 cross, 1 square, 2 circle.
4. Per ship: primary weapon function by `weapon` (0 wide, 1 beam, else vulcan), secondary by `aux` (0 homing, 1 spread, -1 none), each given the fire flag.
5. `shooter_try_start_bomb` / bomb update (`shooter_update_bomb`, section 5.3), `shooter_run_collisions`, `shooter_update_hit_sparks`, `shooter_update_explosions`, `shooter_update_enemy_bullets`, `shooter_collect_items`, `shooter_update_items`.

### 4.1 Player movement (`shooter_update_player`)

- Speed per frame: 32; 45 when a horizontal and a vertical direction are held together.
- Left (0x8000): if x > 0x800, x += speed * (0x1000 - x) / 2048, else x += speed. Right (0x2000): mirror with -0x800 and -0x1000. Up (0x1000): if y < -0x700, y += speed * (-0x800 - y) / 256, else y -= speed. Down (0x4000): if y > 0x80, y += speed * (0x180 - y) / 256, else y += speed. Then clamp x to +-0x1000 and y to -0x800..0x180.
- Roll buttons by setting: 0 left shoulder buttons L1 or L2 roll left, R1 or R2 roll right (masks 5 and 0xA); 1 L1 and R1 (4 and 8); 2 L2 and R2 (1 and 2). A roll starts only when roll speed (phase) is 0: mode becomes 0x81 (left) or 0x82 (right), phase 0x7F, and the wingman's mode is increased by 1.
- While rolling: roll angle += phase (left; -= for right), x += 8 (left) or -= 8 (right). While phase is 0xF or more it falls by 1 per frame and the invulnerable bit (0x80) clears when phase drops below 0x40; below 0xF the roll finishes when the angle reaches the end of the turn (low 12 bits at least 0xFE1, or at most 0x1E for the right roll), then mode, phase and angle reset.
- Hit flash: bit 2 of mode, started by any hit; the hit timer counts 0x10 down to 0 and clears the bit. On timer values 15, 14, 10 and 2 the screen flashes red (`shooter_flash_red`).

### 4.2 Wingman (`shooter_update_wingman`, slot 8)

- Every frame: flag byte 0x80, radius 2, hittable bit set; HP and the other fields as set by `shooter_init_ships`.
- `shooter_count_targets` counts live hittable enemy slots 16..63 whose kind is not 0xE and returns the first index. With 8 or more targets the wingman flies toward that first target: each frame x and y move by the unit direction (length 4096, from main's vector-normalise routine func_800AA460) divided by 103, about 40 units. With fewer than 8 it follows its mode (0..9; +1 on every player roll, values from 10 wrap to 0): modes 0 and 1 move toward the player the same way while the squared distance is at least 0x10001 (about 256 units); modes 2 and 3 do so with the x and y components swapped; modes 4 and 5 aim at the player's position shifted by +0x80 in y and stop at squared distance 0x641, also with swapped components; modes 6 to 9 do not steer in x or y. Modes 6 and 7 bounce z between 41 and the player's z + 33 at 32 per frame; modes 8 and 9 give z a random velocity from a random unit vector divided by 103, re-rolled with chance 1/128 per frame.
- Always: z += vz; with chance 1/64 per frame vz = a random value in -16..15; if z is more than 0x200 past the player's z, vz = -(0..15); if z < -0x1000, vz = 0..15. x and y are clamped like the player's.
- It fires (flag 0x20) on frames where `unk_10F8 & 7` is 0 when at least one target exists, and uses the same weapons as the player (`weapon`, `aux` of slot 8, changed by items 0, 1, 2, 7).
- Its HP (0x80 after `shooter_init_ships`, 0xFF after game start) is reduced by hits but never read for any decision: the wingman cannot die. Its body-contact loop scans slots 0..15 instead of 16..62, so it only ever tests the ships themselves.

## 5. Weapons

All shot tables hold 16 records. A shot is removed when its age reaches 255 or z passes -0x7D00.

### 5.1 Primary weapons (`weapon`)

| Weapon | Fire function | Behaviour |
| --- | --- | --- |
| -1 vulcan | `shooter_fire_vulcan` | Free slot found: flags 0x94 (kind 4), x = ship x + 0x40 or -0x40 alternating by slot parity, y = ship y + 0x80 (player) or 0 (wingman), z = ship z, vz -0x200. SE 0x500 for the player. Damage 6. |
| 0 wide | `shooter_fire_wide_shot` | Same layout, flags 0x98 (kind 8), y + 0x40 (player). Damage 2 but hit box width 0x60 instead of 0x30. SE 0x502. |
| 1 beam | `shooter_fire_beam` | Only starts when all 16 slots are free. Flags 0xB0, two anchor records at the ship (+0x40 and -0x40 in x after 3 frames, y + 0x20); the beam lasts 128 frames and follows the ship. SE 0x501. Damage: every other frame each anchor removes 1 HP from every enemy in front of the ship within radius + 0x18 in x and y; score per tick is class score / 20. |

Weapons change through item pickups (section 7.3). The player starts with the vulcan.

### 5.2 Secondary weapons (`aux`)

Start: none (-1). The first secondary item gives homing (0), later ones toggle homing and spread. Fire function with the fire flag; the shot goes into the secondary table.

- Homing (`shooter_fire_secondary_homing`): flags 0x91 (kind 1), spawn at ship x, y + 0x40, z + 0x40, vz -0x100; stores a target index chosen by `shooter_find_target` (scans 48 slots from a start that depends on the frame counter and keeps the nearest target in front). After age 17, every 4th frame the velocity is averaged toward the target by (direction vector / 64), clamped to 0x40; if there is no target one is searched. Damage 3. SE 0x503.
- Spread (`shooter_fire_secondary_spread`): flags 0x92 (kind 2), x velocity = sin(roll angle)/256, y velocity = +-cos(roll angle)/256 alternating by slot parity (y offset -0x60 * (slot & 1) + 0x40), vz -0x40. After age 17 velocity is divided by 16 and vz becomes -0x200. Damage 6.

### 5.3 Bombs

The player has three bomb slots in `arg_x`, `arg_y`, `arg_z` (start: 4, 3, empty). A slot holds a bomb kind: 3 pulse or 4 blast. The top slot is the highest-numbered one holding a bomb (slot 3, then 2, then 1). Bomb pickups fill the first empty slot in order 1, 2, 3 (a full stack overwrites slot 3). Pressing the bomb button (edge) while no bomb is running and the ship is not invulnerable starts the top bomb.

- Both kinds set the player's mode bits 0xC0 (frozen controls and invulnerable) and clear the enemy bullets and the ships' shot tables.
- Pulse bomb (kind 3, `shooter_update_pulse_bomb`): flag D_80160460 = 1, timer D_80160464 runs 260 frames with a circle flash and white overlay. From timer 65 on, every 4th frame all live hittable enemies lose 2 HP (`shooter_bomb_damage_all`, score on kill: class score) and SE 0x505 plays. At 260 the flag clears, one more damage pass runs and the slot is consumed.
- Blast bomb (kind 4): record 0 of the bomb table spawns at the ship with vz -8, SE 0x505. Every 4 frames a further record (index = age of record 0 / 4, up to 16) spawns with vz -0x20, switching to -0x100 after age 16; wingman is pulled to half the distance to the player each such frame (at z minus 0x200). When record 0's age passes 128, every 4th frame `shooter_bomb_damage_by_distance` hits every live hittable enemy for ((0x1000 - |dx|) + (0x1000 - |dy|)) / 256 clamped at 0 per axis (dx, dy from the blast centre), SE 0x504; shots then accelerate to vz -0x400. The blast ends when record 0 reaches age 255; the slot is consumed and its flags cleared.
- While a bomb runs the player cannot move or shoot (section 4).

## 6. Settings, menu, ranking (stages 3 to 5)

Settings (stage 4): five rows: auto shot (off/on), roll button (three choices), shot button (circle/cross/square), bomb button (cross/square/circle), exit. Up/down moves the cursor (wraps over 5), right or circle cycles forward, left cycles back, cross or START leaves; a 31-frame cross-fade animates the changed value. Values live in D_8015EDD8..EDE4 and are stored in the work area at `D_800E7D10 + 0x40` (a 2-byte bit set plus names and scores): byte 0 bit 0 = valid/unlocked flag, bit 2 = auto shot, bits 3 and 4 = roll setting; byte 1 low nibble = bomb setting, high nibble = shot setting; names at +0x42, +0x45, +0x48 (3 bytes each); scores at +0x4C, +0x50, +0x54. `shooter_load_settings_and_scores` restores them at stage 0 when the flag is set, otherwise writes defaults. `shooter_exit_step_save_settings` writes them back at stage 46. Bit 1 of the same word is read at the end of stage 46 (see section 10). The bit 0 flag is also the unlock flag other overlays set (MASTER sets bits 0 and 1).

Ranking page (stage 5): eight steps: stars fade in over 64 frames, ranking zooms from 0 to normal (0x1000) at 60 per frame (120 while cross is held), holds until circle is pressed or cross is held, zooms out at 500 per frame, stars fade out over 43 frames, main sound command 0xC1 (a direct call to the main sound player, not an SE id of banks 5 or 6), back to stage 2. Scores are formatted by `shooter_format_score` as 8 characters, right aligned, capped at 99,999,999.

## 7. Collisions, damage, scoring, items

`shooter_run_collisions` (func_8013B990): enemy bullets vs ships, enemy bodies vs ships, then shot hits for the four shot tables, then the score tick. Shot hits are skipped while a boss is already counted as beaten (stage 12 and stages cleared != 0, stage 22 and cleared >= 2, stage 32 and cleared >= 3).

### 7.1 Enemy side to ships

- Enemy bullet (`shooter_hit_ships_with_bullets`): live bullet within |dx| < 0x80, |dy| < 0x80, |dz| < 0x100 of a ship that is not invulnerable: bullet removed, ship HP -4 (0 if lower). Player only: mode bit 2 set, hit timer 0x10, SE 0x506.
- Enemy body (`shooter_hit_ships_with_bodies`): enemy slot 16..62 with flag byte & 0xC0, HP != 0, radius r = stats[0] * 16: |dz| < 0x100 and |dx|, |dy| < r hits the player for 12 HP (0 if lower), hit flash and SE as above. There is no invulnerability time after a hit, so standing in a body loses 12 HP per frame. The wingman's loop (slots 0..15) never matches an enemy.
- The run ends in stage 45 when player HP is 0.

### 7.2 Player shots to enemies (`shooter_hit_enemies_with_shots`)

Candidate enemies: slots 16..62 with flag 0x80, stats[3] bit 0, HP != 0 and radius != 0. Score class value: the table `shooter_score_by_class` indexed by kind % 17; in boss stages the value is flat (boss 1: 400, boss 2: 200, boss 3: 800 for every part).

Class values (kind 0..16): 200, 600, 2500, 800, 5000, 12000, 5000, 3000, 400, 1000, 3000, 1000, 600, 2000, 150000, 50000, 100000.

- Bullet shots (kinds 1, 2, 4, 8): hit when |z difference| < r + 0x100 and |dx|, |dy| < r + width (width 0x30, 0x60 for kind 8). The shot is removed, HP -= damage (damage per kind: 0:1, 1:3, 2:6, 3:1, 4:6, 5..7:1, 8:2), a spark spawns at the midpoint, score += class / 10 * damage; on kill (HP 0) the hittable bits clear (stats[3] & 0xF0), SE 0x508 plays and score += class. One enemy per bullet per frame. SE 0x507 plays on the first hit of each player shot table per frame (guarded by D_8015F500).
- Beam (records with flag 0x20): on frames with `unk_10F8 & 1`, for both anchors, any enemy within r + 0x18 in x and y and behind the shooter (enemy z < shooter z) loses 1 HP, with a spark; score += class / 20; on kill score += class.

### 7.3 Items (`shooter_spawn_item`, `shooter_collect_items`)

Dropped when an enemy dies: `shooter_update_enemies` checks mode 0xFF has not started and HP 0, sets the enemy dying (mode 0xFF, phase 2) and spawns items by the drop code (high nibble of stats[3]): 0x10 item 6, 0x20 item 7, 0x30 item 0, 0x40 item 1, 0x50 item 2, 0x60 item 3, 0x70 item 4, 0x80 item 5, 0xD0 five times item 6, 0xE0 items 0, 1, 7, 0xF0 items 6, 6, 7, 2, 0, 1, 5, 3, 4.

Item motion: spawn velocity x and y random in -64..63, vz +0x50; per frame the position moves by the velocity and x and y velocity move toward 0 by 1; except in the last age step, positions clamp to x +-0x800, y -0x700..0x80 and z velocity flips when z passes the player's z + 0x10 (outward) or goes below -0x1000 (inward); the age counter advances every 255 frames and the item vanishes after 5 steps (about 21 seconds), or at z >= 0x2711. Max 16 items.

Pickup (player): within 0x200 on each axis, +1000 points and:

| Item | Effect |
| ---: | --- |
| 0 | weapon = wide (SE 0x514) |
| 1 | weapon = beam (SE 0x512) |
| 2 | secondary toggles (first time homing); SE 0x511 or 0x516 |
| 3, 4 | bomb added (first empty slot 1, 2, 3, else slot 3); SE 0x509 |
| 5 | HP +16, capped 255; SE 0x510 |
| 6 | bonus: random value from a 1-in-8 table (100, 200, 400, 800, 1600, 3200, 6400, 57300) added to the score and shown for 32 frames; SE 0x509 |
| 7 | weapon = vulcan (SE 0x500) |

The wingman collects items other than 3 and 4 (same weapon and heal effects, no score).

## 8. Enemies, stage scripts and bosses

Enemy, boss and behavior names in this section and in the table (debris, homing mine, swooper, spiral ship, formation ship, gate, circler, carrier, turret, patroller, wall ship, swarm ship, guardian, orbiting orb, mid boss) are behaviour labels chosen from the movement and attack code. The models are TMD files whose appearance has not been looked at, so a label says nothing about what the enemy looks like (open question 1).

An enemy is created by `shooter_spawn_enemy(type, behavior, arg, position, rotation, HP, flags)`: it takes the first free slot in 16..62, requires enough free coordinate objects (D_80161D9C), links the model, and sets mode = behavior, kind = type, counter = 0x7FFF (first-frame show), the argument vector, position, rotation, scale and radius from the type tables, HP, flags (low bit forced to 1). Return -1 when no slot or objects are free. `shooter_update_enemies` runs the behavior (`shooter_run_enemy_behavior`), then animation and death handling. Generic behaviors: 0xFD leave backward (z -250 per frame), 0xFE leave forward (z +0x100 per frame), 0xFF dying (burst of explosions, spin, removed after 61 frames).

### 8.1 Stage 1 script (`shooter_update_stage1_script`)

Phase (D_801604C4), timer (D_801604C0, +1 per frame):

| Phase | Behavior |
| ---: | --- |
| 0 | frames < 2100: debris every 7 frames; frames < 1200: a homing mine every 50 frames; from 601 to 1800 a wave of five swoopers whenever none are alive; at frame 1801 go on |
| 1 | spawn the mid boss (HP 255, drop code 0xF0, remembered in D_80160470); go on |
| 2 | frames < 600: debris every 7; < 300: swooper wave when none alive; from 481: three homing mines every 110 frames; from 301: a spiral pair every 120 frames; go on when no type 6 enemy is alive or the mid boss is dying |
| 3 | HP 0 for all enemies; go on |
| 4 | white fade in (+2 per frame) to 128; go on |
| 5 | fade out (+3 per frame) to 128, then reset the enemy tables and go to the end step |

Types (kind, behavior, HP, radius/16, drop):

| Type | Behavior | HP | Radius | Notes |
| --- | --- | --- | --- | --- |
| 0 debris | 1 | 4, 8 or 12 by random size class | 3, 4 or 5 | scale and speed by size; spins; every third drops a bonus item (code 0x10); passes toward the camera at z +0x100 per frame |
| 7 homing mine | 7 | 5 (20 in the phase 2 triple) | 3 or 4 | pulled toward the player in x and y by 1/40..1/69 per frame, z +250, removed at z > 2001; drop code 0x50 before frame 240 |
| 4 swooper | 2 | 6 | 4 | five at once, phase offsets 0, 20, 20, 40, 40; moves on a circle in x/z for 2560 angle units; the first drops 0xE0 or 0x50 alternating |
| 5 spiral ship | 3 then 5 / 4 | 4 / 2 | 3 | behavior 3 spirals in for 100 frames (drop 0x80 or 0x10), then turns into behavior 5: fires every 7 frames for 60 frames, turns, flies away; behavior 4 spirals out |
| 6 mid boss | 6 | 255 | 0 until it is close, then 32 | grows in over 500 frames, approaches 0x61A7; drop 0xF0 |

### 8.2 Stage 2 script (`shooter_update_stage2_script`)

Phases 0..13 with a sub-phase loop (D_801604CC):

- Sub-phases (spawned in parallel while the phase is below 12): 0 spawn a carrier on its first frame and wait until none is alive; 1 gate pair; 2 and 3 gate pair when no gate B is alive; 4..8 pass; 9 reset to 0.
- Phase 0 and 6: six formation ships (`shooter_spawn_formation_pos_x`) from the +x side (screen left) at frames 2, 10, .. 42; go on at frame 43 when none of type 8 live. Phases 1 and 7: same from the -x side (`shooter_spawn_formation_neg_x`). Phases 2 and 8: a V of five at frame 5; go on from frame 21 when none live. Phases 3 and 9: ships alternate between the +x and -x side every 8 frames. Phases 4, 5, 10, 11: circling pairs at frames 5, 35, 65 (`shooter_spawn_circling_pair`: two circlers per call, one entering from each x side at x +-6000, one high (y -2200) and one low (y 800); `shooter_circler_pairing` (D_801604D0) picks the diagonal, 1 = (-x, high) + (+x, low), 2 = (-x, low) + (+x, high); it is chosen on the first call from the player's position, 1 when the player is at x < 0 with y < -700 or at x > 0 with y >= -699, else 2, and reused by the later calls), go on at frame 66 when none live. Phase 12: wait until all enemies are dead. Phase 13: wait until no active enemy remains, then reset and end.

| Type | Behavior | HP | Notes |
| --- | --- | --- | --- |
| 8 formation ship | 0x12 | 4 (V of five: 4, 6, 2, 6, 4) | approaches, then fans out by the spin index into one of five directions (velocities (64,0), (48,-48), (0,-64), (-48,-48), (-64,0)), aimed shot every 20 frames |
| 9 and 0xA gate | 0x13, 0x14 | 64 | spawned as a pair at a random position; both follow the player 61 frames; B (drop 0x80) then waits 91 frames, turns and leaves backward; A waits 61 frames and leaves forward |
| 0xC circler | 0x11 | 2 | pairs; flies in, circles with radius 500 firing every 20 frames, leaves; drop 0x10 |
| 0xE carrier | 0x15 | 16 | crosses the screen, drops a turret at x -800, 0 and 800; drop 0xE0; not targetable |
| 0xF turret A | 0x16 | 4 | drop 0xD0; fires every 20 frames while sinking |
| 0x10 turret B | 0x16 | 8 | drop 0x60 or 0x70 |
| 0xB patroller | 0x10 | 6 | bobbing, fires every 60 frames; its spawner has no caller (never spawned) |

### 8.3 Stage 3 script (`shooter_update_stage3_script`)

Every 30 frames a swarm ship (type 2, behavior 0x20, HP 4, wobbles in then dives; drop 0x10 on two of five) is spawned if fewer than 10 are alive (phases 0..3) or fewer than 3 (phases 4 and 5). Phase 0 starts a wall-ship wave and advances; phases 1..3 start their wave when no wall ship is alive and advance; phase 4 waits for no wall ship; phases 5 and 6 advance; phases 7..10 wait for no active enemy, then spawn a guardian ring with (phase - 3) orbs and advance; phase 11 waits until all dead; phase 12 waits until no active enemy, then resets and ends.

- Wall ship (type 3, behavior 0x21): three per wave, from alternating sides (+x, +x, -x in waves 0 and 2; -x, -x, +x in waves 1 and 3), at y -1200, -200 and -700, HP 8 (10 for the -700 ship of wave 0). Each slides toward x 0 (x -= x/50) and every 10 frames fires an aimed bullet (speed 0x180) while |x| < 1000, in a window by height: y -1200 during the first 60 frames of its 181-frame cycle, y -700 from 61 to 119, y -200 from 121. Drop codes 0xE0, 0x50 or 0x80 on single ships by wave.
- Guardian (type 0xD): core behavior 0x23 (HP 40, drop 0xD0, fires every 15 frames for 601 frames) and orbs behavior 0x22 (HP 20, random drop code, orbit the centre for 601 frames), both grow in and shrink out.

### 8.4 Bosses

The boss is a set of parts, each an actor in slot 16 upward bound to a model object (`shooter_attach_boss_part`) and placed every frame from its joint (`shooter_boss1_update_part` and the boss 2 and 3 equivalents): position = object 0 position + the boss rotation applied to the joint position + a per-part offset from a table. Each part has a radius (from a table, in units of 16, with a second value for boss 1's second form), HP per bar and a bar count. A part whose HP is below 21 and whose bars-used counter is below the bar count refills to the full HP per bar and counts one bar. Dead parts blink out and are removed from the model.

The fight ends when the damage percent reaches a threshold. `shooter_boss_damage_percent` computes it in integer arithmetic: `100 - (num * 100) / den`, with `num` = the sum of the parts' current HP and `den` = the sum over the parts of HP per bar x bars. The division truncates, so the result is the real-valued percent rounded up, and the tests (`>= 80`, `>= 85`, `>= 85`) are made on that integer: a threshold T is reached as soon as the real percent exceeds T - 1. Only the current bar of each part is in `num` and a refill puts that bar back to full, so the value is already positive at the start and falls back at every refill; it does not climb with the number of bars used.

| Boss | Parts | Part HP (per bar x bars) | Start percent (integer) | Threshold | Extra condition |
| --- | ---: | --- | ---: | ---: | --- |
| 1 (stage 12) | 11 | 255x2, 255x3, 255x2, 100x1, 100x1, 100x1, 255x1, 100x1, 100x1, 100x1, 255x1 (sum 2895) | 36 | 80 | none |
| 2 (stage 22) | 12 | 200x3, 200x3, 200x3, 255x3, 200x2, 255x1, 150x1, 200x2, 255x1, 150x1, 200x2, 200x2 (sum 4975) | 51 | 85 | actors 16 and 17 have no HP |
| 3 (stage 32) | 11 | 255 x bars 4, 5, 2, 3, 3, 2, 2, 2, 2, 2, 2 (sum 7395) | 63 | 85 | actors 16, 19 and 20 have no HP |

Start percent is the value right after the parts are attached (all parts at full HP of bar 1). The current-HP sums are 1875 of 2895 (boss 1), 2465 of 4975 (boss 2) and 2805 of 7395 (boss 3), which gives 100 - 64 = 36, 100 - 49 = 51 and 100 - 37 = 63 (the real-valued percents are 35.2, 50.5 and 62.1). In terms of current HP, the threshold is reached when the sum is at most 607 (boss 1), 795 (boss 2) or 1183 (boss 3); the extra condition has to hold as well.

Boss 1 radius table per part (form 1, form 2): (0, 15), (0, 20), (0, 10), then 10, 10, 15, 20, 10, 10, 15, 20 for both forms. Boss 2 radii: 10, 10, 10, 30, then 20 for the other eight. Boss 3 radii: 30, 40, 30, 20 and 20 for the rest.

Damage lights: when the percent rises above the value remembered by the damage-light actor, the three lights switch to red for three steps, then back to the boss lighting (each boss has its own light set).

Shots from bosses use `shooter_fire_from_joint(object, joint, dx, dy, dz, speed, flags)`: the joint offset is rotated by the boss rotation and passed to `shooter_spawn_enemy_bullet`. Bullet flags: 0x80 aimed at the player, 0x40 with random spread (each axis 0x7F - (0..255)), low nibble the sprite style 0..6. Bullet velocity: aimed bullets get the unit direction divided by (4096 / speed + 1); unaimed bullets move +z at the speed. Bullets live 255 frames or until z >= 10001.

Boss 1 (`shooter_stage_boss1`): init waits for the load request and builds 30 joints; the entrance is a script of 1150 frames (pose keys, a morph to form 2 starting at frame 649 and back to form 1 at frame 950); the fight runs until the threshold, then 240 frames of defeat: explosions to frame 120 (bank 6 sound 0x608 at defeat frames 0 and 60), white flashes, objects hidden at frame 100, stage 21 at 240. The step that detects the threshold plays 0x605 once, counts one stage cleared and starts the defeat. The entrance script plays 0x600 at its frames 10 and 199, and each morph plays 0x601 once at its first pose key (key 1 going to form 2, key 0x1D coming back). Actor 16 holds the attack mode (0..8) with its step counter in mode and its pace in phase; actor 17 holds the form (pose index 1 or 0x1D) and the morph state; actors 18 to 25 drive the part groups and 20 the damage-light memory. The attack mode is picked at random (`shooter_boss1_pick_attack_mode`) with a percentage roll: form 1: below 20 mode 0, below 40 mode 4, below 55 mode 1 (mode 7 when actors 21 and 25 have no HP), below 70 mode 2, below 90 mode 3, else 7; form 2: below 40 mode 2, below 70 mode 5, below 95 mode 6, else 8. At fight frame 601 the first morph to form 2 starts (when actor 17's index is 1). Modes: 0 and 4 sway sideways in 5-step path pieces (x step of +-0x96, +-0x78 or +-0x50 per piece; mode 0 starts toward +x, screen left, and mode 4 is the same x path negated, starting toward -x, screen right) with shots from joint 0x1B every second step (each mode ends after 60 steps or at random 1 in 200); 1 tilt and a group animation; 2 scatter shots from two joints, 20 steps; 3 a lunge toward the camera over 80 steps in which the group actors become contact-only (flag 0x40, radius 20) for steps 45 to 54; 5, 6, 8 form 2 group animations (80 or 115 steps); 7 rain of random shots from joint 0x1B for 50 steps.

Boss 2 (`shooter_stage_boss2`): three seven-segment chains each pick an attack when idle (`shooter_boss2_pick_arm_attack`: roll below 50 attack 1, below 70 attack 2, else 3; none while actor 17 is busy); attack 1 and 3 are pose sequences with shots from the chain's base joint at keys 30 to 32, attack 2 fires aimed shots on every second step; when all three are idle a combined attack runs for 110 steps. Actor 16 runs a barrage cycle of 30 steps, actor 17 a core cycle of 20, actor 21 a tail cycle of 70, actor 23 an idle sway. When actors 16 to 18 have no HP, shots fire from joint 0x1C. Entrance 300 frames; during the entrance and the fight 0x600 plays when the step frame count is a multiple of 240 and a 1 in 3 roll succeeds; the barrage cycle plays 0x602 every 10 steps and the arm attacks play 0x601. Defeat: flashes from frame 180, bank 6 sound 0x604 at frame 199, fade to frame 440, then stage 31.

Boss 3 (`shooter_stage_boss3`): descends until its y is within 100 of the target (step 1; 0x600 plays every 180 frames in the descent and again in the fight), then fights: group attacks A (actor 18) and B or C (actor 19) start at random (1 in 200 per frame, only while the damage percent is below 75 and the controlling part has HP), an idle sway on actor 16, and at damage 32 and 48 percent the idle pose set and model colours change. Shots come from joint 0x19 depending on which parts live. Defeat: 600 frames of explosions and a white fade (0x602 plays at frame 600 if the step is still running; the fire pattern plays 0x601), then the collapse step: parts fall at random speeds 5..12, explosions run to frame 400, stage 45 after frame 600.

## 9. End of run: results and name entry

Stage 45: the banner depends on the outcome: clear when 3 or more stages are counted; time-out when the timer is 0; else defeat. A button then advances; `shooter_load_request` 0x44 (a bank 0 sound load) is issued at its init and the exit waits for the sound-bank-ready check.

Stage 8 (`shooter_result_step_compute`): base score = score + 1,000,000 x stages cleared (this updates `shooter_score`). Then:

- Time bonus: if stages cleared >= 3: time left (frames) x 100, else 0.
- Shield bonus: if cleared >= 3: 500,000 when player HP is 255, else HP x 1000; otherwise HP x 100.
- Bomb bonus: slot 3 filled 500,000, else slot 2 filled 250,000, else slot 1 filled 100,000, else 0.
- Total = base + time + shield + bomb, capped at 9,999,999.

Step 1 rolls the five numbers one after another (score, time bonus, bomb bonus, shield bonus, total; each rolls up one digit place at a time, `shooter_count_up_number`); circle skips to the total, and circle on the total stores it to `shooter_score` and moves on. Step 2 goes to stage 9 when the total is above the third high score, otherwise to stage 5.

Stage 9: the score is inserted at rank 1, 2 or 3 (the old entries shift down, names too), the new name blanked. Input: left and right rotate a ring of 40 glyphs (26 letters, period, hyphen, 10 digits, space, an end mark) with speed levels -2..2 and an intermediate offset of -50..50; circle takes the front glyph (up to 3 characters; the end mark ends), cross deletes the last character, START ends. Then the ranking page (stage 5) shows the result.

## 10. Connections to the rest of the game

- Entry: mode 0x90 (the overlay loader's default; entry 0x80132000, [[overlays]], [[game-state]]). Two callers: MASTER, when its event reaches case 4 with the condition `unk_0F4` bits 12-15 equal 3 true, sets bits 0 and 1 of the settings flag word at `D_800E7D50`, saves the return state (`unk_720` = current mode, `unk_721` = stage + 1, `unk_722` = 0) and enters; OPTION, from its menu when bit 0 is set, clears bit 1, plays a SE and enters. So bit 0 means the shooter is unlocked and the settings are valid (the settings loader also sets it when it writes defaults), and bit 1 means launched with a saved return state.
- Exit (stage 46): after saving settings and scores into `D_800E7D10 + 0x40`, running the shared exit steps and loading the day data, step 7 restores the caller with `main_state_set(unk_720)`, `func_80042908(unk_721)`, `func_80042940(unk_722)` (and a few resets) when bit 1 is set (the MASTER case), otherwise `main_state_set(0x12)` (the OPTION case; mode 0x12 is resident main-exe code).
- Shared sub-systems from main: pad words, frame counter `unk_10F8`, RNG (`func_800AE0D0`, seed `unk_10F4`), SE and voice players, the text layer, the CD reader (`cd_read_sectors`, `cd_read_sync_status`), GPU packet allocation.

## 11. Presentation layer (module level)

Not part of the game rules; a port replaces it.

- Rendering: models are PlayStation TMD blocks loaded to 0x8018A000.. and linked with the graphics library (GsLinkObject4 style) into 64 handles and 64 coordinate objects (`shooter_link_model_objects`, `shooter_init_object_hierarchy`); the 3D pass is `shooter_draw_models`. Sprites and effects (shots, bullets, items, sparks, explosions, HUD, text) are POLY_FT4/SPRT/POLY_G4 packets built in the per-frame buffer with `GetWorkBase` and linked into the ordering table at slot `dist * 4` (depth) or fixed HUD slots. Billboards are made by transforming four corner points with the reference matrix (D_80129C40) and the GTE.
- Background (`shooter_background_flags`, D_8015EDB0): bit 0 floor grid (13x8 tiles, scroll state `shooter_background_scroll`), bit 1 four horizon bands, bit 2 an empty layer; stage 1 uses only the star field (flag 0x80), stage 2 flags 3 with a light sky colour in GameState unk_036..038 (0x78, 0xA8, 0xF0), stage 3 flag 4 with a dark blue clear colour (8, 0x10, 0x60). The star field is 100 stars (angle, radius*1000) updated by `shooter_update_stars`; brightness factor `shooter_star_brightness`.
- Camera follows the player's x and y with the roll angle.
- Asset loading: `shooter_load_request` codes processed by `shooter_process_load_request`: 0x10 game start banks, 0x14 stage 1 sound bank, 0x18 boss 1 assets, 0x20 stage 2 assets (requested while boss 1 initialises), 0x24 stage 2 sound bank, 0x28 boss 2 assets, 0x30 stage 3 assets, 0x34 stage 3 sound bank, 0x38 boss 3 assets, 0x44 end sound bank. The four sound bank codes call `snd_bank_load_start` (core name of func_80044890) for bank 0, each with its own LBA set, and stage 0 loads banks 0 and 1 the same way; the default branch waits until the load has finished (func_80045288 returns 1) and clears the request. The ready check of the stages is the main poll func_80044C98 (returns -1 while busy, 1 when ready; kept in `shooter_sound_bank_poll_result`). HYPOTHESIS: bank 0 holds the stage music (BGM name SHOOT in the sound test).
- Sound: `shooter_play_se(bank_flag, id)` (func_801335A0) plays main sound id `(id & 0xFF) | 0x500` when the first argument is odd (bank 5, battle sounds) and `(id & 0xFF) | 0x600` when it is even (bank 6, scene sounds). The C source writes the id with a 0x500 prefix in both cases, so a source id is not the played id when the first argument is 0: a call `(0, 0x508)` plays 0x608, `(0, 0x504)` plays 0x604. Every id in this spec is the played id. Every battle call passes 1 and every scene call passes 0. Cooldown: only when the first argument is exactly 1, the low byte of the id is below 10 and the background flags (D_8015EDB0) are non-zero, a call is dropped while that id's cooldown counter is non-zero, otherwise it plays and sets the counter to a random value of 0..2 x range frames (range from a per-id table); the counters fall by one per frame (`shooter_tick_se_cooldowns`). Bank 6 calls never use a cooldown.
  - Bank 5 (battle): 0x500 vulcan, 0x501 beam, 0x502 wide shot, 0x503 secondary, 0x504 blast bomb hit, 0x505 pulse bomb tick or blast bomb start, 0x506 player hit, 0x507 enemy hit, 0x508 enemy destroyed, 0x509 pickup, 0x510 heal, 0x511 secondary set to homing, 0x516 secondary set to spread, 0x512 beam selected, 0x514 wide shot selected (the vulcan item replays 0x500).
  - Bank 6 (scenes), by use: 0x600 cursor move in the main menu and settings, and the boss cue (boss 1 entrance frames 10 and 199; boss 2 every 240 frames at 1 in 3; boss 3 every 180 frames); 0x601 menu confirm or value change, and the boss 1 morph, boss 2 arm attack and boss 3 fire pattern cues; 0x602 leave settings, the boss 2 barrage cue (every 10 steps), the boss 3 defeat cue (frame 600) and boss 1 part group E key 1; 0x603 game start (stage 10 init) and boss 1 part group E key 16; 0x604 title frame 0xA0 and boss 2 defeat frame 199; 0x605, 0x606 and 0x607 title frames 0x40, 0x80 and 0xC0 and the opening script cues (0x605 also marks boss 1's fight end); 0x608 boss 1 defeat explosions (frames 0 and 60). The sounds themselves are not identified (bits 8 to 11 of a sound command are its class; the core table for the main exe notes classes 5 to 8 as not decoded); the names in the table describe where each id is used, and a bank 6 name is not menu-only.
  - Direct commands (not through `shooter_play_se`) to the same main sound-driver function (func_80044750): 0x24, 0x74, 0xB1 and 0xC1 (class 0, effect ids) and 0x300 (class 3, voice); their meaning is not known.
- Voice: a triangle press in step 1 of stages 11, 21 and 31 (the HUD drawer is the only caller) plays one of 6 voice clips through `cd_audio_request_set` (core name of func_80046290; HYPOTHESIS: voice); the help pages play 9 clips.
- Screen effects: red flash on hit, white flashes and fades (`shooter_draw_flash_overlay`, `shooter_fade_white`), a circle flash (`shooter_draw_circle_flash`), the screen wipe of stage 45 (`shooter_draw_screen_wipe`), model CLUT recolouring for boss damage (`model_recolor.c`).

## 12. Demo mode (stage 6)

Set-up identical to a new game, weapon: low 2 bits of the frame counter pick wide (0), beam (1) or vulcan. Each frame the script ORs the fire bit (circle) into both pad words and, by `unk_1100 & 0x1FF` (a 512-frame cycle), holds: right (0x2000) at 0x79..0x8B, down (0x4000) at 0xAB..0xBD, left (0x8000) at 0xDD..0xEF, up (0x1000) at 0x10F..0x121, masks 0x3000 and 0x6000 at 0x191..0x1B7, masks 0xC000 and 0x9000 at 0x1CD..0x1DF. Any real pad input or 1801 frames ends the demo (to stage 2). The battle-end check is not run, the timer and game-over do not apply.

## 13. Open questions

- The meaning of the name TACO and of the player's ship and enemy appearance (models are TMDs on disc; names here are behaviour labels, see section 8).
- Whether the wingman self-hit and the 0..15 scan are original behaviour (HP is never used, so the effect is none); stage 4 and 5 timing is exact, but the 12-per-frame contact damage without invulnerability could only be confirmed by running the scene.
- Boss part names and the exact shot patterns of the boss group animations are described only by structure; a runtime trace per boss would settle which group is which visible part.
- The asset layout of the disc files (LBA table) is kept in the code and not repeated here.
- The emulator was not used: the scene is only reachable with the unlock flag, which has no cheap entry in the current tooling.

## 14. Notes for merging the tables

- Pad bits: the 14 `PadBits` / `PAD_*` rows of `o-taco.tsv` are the same rows as in `core.tsv` (same key, same name, same value; core's values were corrected in the core fix round, so nothing conflicts any more). The merge keeps one copy. The TACO code also reads the roll masks 0x5 and 0xA (L1 or L2, R1 or R2) and the skip mask 0x860 (circle, cross, START).
- Comment rows: the nine `comment` rows follow core's column convention: column 4 is the English one-liner, column 5 is a note (call sites in the overlay). They add one-liners for PsyQ-style names that stay as they are; none of the nine is in `core.tsv`, `o-tt.tsv` or `o-rpgbat.tsv`. Seven rows of an earlier version duplicated core rows (`draw2d3d`, `back_clear_switch`, `load_palette`, `LoadSquare`, `dtd_on_tpage`, `k_reset`, `load_tpage_buf_lock`) and were dropped. What this overlay adds to them: it calls `draw2d3d` with (1, 1) in the opening and title set-up, with (1, 0) in an opening step and with (0, 0) in the exit set-up, and calls `dtd_on_tpage` with five arguments from several drawers, the first two being a texture page position such as (0x340, 0x180) (its meaning stays a HYPOTHESIS, the name comes from the developer's own name).
- Type rows, particle: `Tc14` (scalar fields x, y, z, w, vx, vy, vz, vw, spare, arg, age, flags) becomes `ShooterParticle`; `TacoParticle14` (the same 0x14 bytes as two `ShooterPos` plus four bytes, used for the spark and explosion tables) becomes its own type `ShooterParticleView` with the field names pos, vel, spare, arg, age, flags. Two names, two field sets, no type defined twice. A later code pass can replace the view by the scalar fields.
- Type rows, settings: `ShooterSettingsBlock` and its word, half, byte and flag views describe the same memory as core's `SaveTacoSettings` (main scope `SaveSettings24`) and `TacoSettingsBits` (main scope `WorkSetBits`); core marks both as guesses and says TACO reads them. Decision for the merge: the Shooter prefix wins, so the two core rows are renamed to `ShooterSettingsSave` and `ShooterSettingsBits`; the overlay views and the main views stay separate types until a code pass unifies them, and neither table renames their bit fields.
- Sound-bank naming: core names func_80044890 `snd_bank_load_start` and func_80046290 `cd_audio_request_set`. This table follows core: the loader step, the poll state and the four load request codes say `sound_bank`, not `stream` (`shooter_load_step_sound_banks`, `shooter_sound_bank_wait_state`, `shooter_sound_bank_poll_result`, `SHOOTER_LOAD_SOUND_BANK_STAGE1`..`RESULT`). The only guess left is that bank 0 holds the stage music (HYPOTHESIS in the notes). The poll func_80044C98 is not named in core.
- Sound names: `SHOOTER_SE_*` constants of bank 6 (0x600 to 0x608) are used by the menus, the title, the opening script and the boss scripts, so the menu names are not menu-only. In the C source the id of a scene sound is written with the 0x500 prefix; the constants hold the played ids.
- Direction words: the table and this spec name x directions by sign, because world +x is screen left (pad left raises x). `shooter_spawn_formation_pos_x` / `shooter_spawn_formation_neg_x`, `SHOOTER_BOSS1_MODE_SWAY_POS_X_FIRST` / `SHOOTER_BOSS1_MODE_SWAY_NEG_X_FIRST` and `shooter_circler_pairing` replaced names that said left and right. The names that remain with left or right say a button (`SHOOTER_MODE_ROLL_LEFT` is the roll started by a left shoulder button, which drifts toward +x) or a HUD position in screen coordinates.
- Files: the `file` rows move the C files to `src/ovl/shooter/` and the header to `include/ovl/shooter.h`. The overlay itself stays TACO (disc name, splat segment, `asm/ovl/TACO/`, `build/ovl/TACO.*` and `config/symbols_TACO.txt`, which `config/overlays/TACO.yaml` lists and `tools/check_symbols.py` names as `config/symbols_<OVL>.txt`), so there is no file row for the symbols file. `configure.py` derives the C path from the overlay name (`src/ovl/<NAME>/<addr>.c`), so the apply step has to change that wiring together with the moves.
- Links: the umbrella ticket link `[[tickets/T-27000-phase-2]]` has no file yet; the phase 2 merge step creates it. `wiki/spec/index.md` is created by each unit and will conflict at the merge, and `wiki/index.md` needs one line pointing to `wiki/spec/index.md`. The table file starts with the same header comment as `core.tsv` (`# kind scope old new note`).
