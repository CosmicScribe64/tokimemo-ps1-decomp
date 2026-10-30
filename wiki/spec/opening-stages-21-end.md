---
type: spec
updated: 2026-10-29
sources: ["src/ovl/name_entry/prologue.c", "src/main/home_screens.c", "src/main/frame_render_audio.c", "src/main/cd_queue.c", "src/main/game_mode.c", "src/main/background_images.c", "src/main/hud_panels.c", "src/main/actor_sprites.c", "src/main/sound_driver.c", "asm/data/data_800B5950.data.s", "disc/track1.bin (ISO directory)"]
---

# Opening stage 21 to the command screen: porting spec

Ticket: [[tickets/T-39017-opening-stage-specs]]. This page covers the end of mode 0x15 (`GAME_MODE_PROLOGUE`): stage 21 `prologue_mode_stage_finish`, the START skip that reaches it from any stage, and the handoff into mode 0x34 (`GAME_MODE_COMMAND_SCREEN`) stage 0xFF through stage 1, where the player first selects a command. The other opening stages have their own pages under T-39017. Background: [[spec/o-nameent]] section 5 (prologue), [[spec/m-sysmenu]] section 3 (command screen), [[spec/m-flow]] section 3 (mode setters, overlay loader), [[spec/m-hud]] section 10 (day music).

Line numbers refer to the current checkout. Disc file names come from the ISO 9660 directory of `disc/track1.bin`, read in Docker (`tokimemo-recovery`, read-only mount); sector counts are rounded-up file lengths.

## 1. Entry conditions

Stage 21 is entered in exactly two ways:

1. **Normal end.** Stage 20 (`prologue_closing_run`, `prologue.c:2686-2706`) substep 2 counts `prologue_scene_counter` (post-increment, `>= 0x3D`). It then sends `audio_command(5)` and `audio_command(0xD1)` and calls `mode_stage_next()`, which sets stage 21 with step, timers, `mode_stage_scratch` and substep all 0 (`game_mode.c:156-167`).
2. **START skip.** This is the tail of `prologue_mode_handler` (`prologue.c:2812-2821`), run every frame after the stage function. If `(mode_step != 0 || mode_stage != 0) && (pad_pressed & PAD_START)` it calls `mode_stage_set(0x15)`; 0x15 is decimal 21. `pad_pressed` is the edge word `~pad_prev & pad_held` (`pad_input.c:179`, `:232`). The skip is available from stage 0 step 1 onward and also on stages 1-20. Nothing is cleaned up at the moment of the skip. Actors, tpage reservations, an XA line in progress, queued CD reads and any half-finished stage state stay in place until stage 21 substep 0 runs on the next frame.

The handler tail runs on **every** prologue frame, including both stage 21 frames. Its order is fixed:

| order | call | effect |
|---|---|---|
| 1 | stage function (`switch mode_stage`) | |
| 2 | `message_window_title_update()` | |
| 3 | `hud_screen_frame_draw(mode_stage == 0 ? 0 : 2)` | at stage 21 and after the switch to stage 0xFF the mask is 2 (lower two rows plus edge cells). It consumes `rand()` only when the player's health state is 4 (`hud_panels.c:1398-1400`); a new game has health 0, so there are no RNG draws |
| 4 | `message_window_show()` | |
| 5 | START test as above | |

## 2. Stage 21: `prologue_finish_run` (`prologue.c:2714-2741`)

`prologue_mode_stage_finish` runs the body only when `mode_step == 0`, which `mode_stage_set` and `mode_stage_next` always guarantee. The stage has no `dec_bg_draw` call, so on both stage 21 frames the still background is **not submitted** and the frame shows the clear colour, actors and HUD only. Stage 20, in contrast, ends each frame with `dec_bg_draw(1)` (`prologue.c:2705`).

### Substep 0 (one frame)

Statement order:

1. `cd_command_reset()` (`cd_queue.c:58-68`): `cd_queue_head = cd_queue_tail = 0`, `cd_read_status = 2`, `cd_queue_phase = -1`, `cd_audio_position = 0`, `cd_state = 0`, `cd_read_count = 0`, `cd_read_checked = 0`, `cd_last_read_lba = 0`. The function sends no CD-ROM command and no XA stop; it only forgets queued and in-flight requests. Whether a hardware read or XA stream that was already running (on the skip path) still lands or plays is **unproven** and needs an emulator check (section 6).
2. `D_800B5BD4 = 0xF`: the ordering-table slot of the checker screen frame (`hud_panels.c:1457`, `:1461`; called `checker_ot_slot` in [[spec/m-hud]] section 11). It takes effect in the same frame's tail call `hud_screen_frame_draw(2)`.
3. `game_state.screen_brightness = 0x80`, which is the grey tint of the checker sprites.
4. `audio_bank_state_reset()` (`frame_render_audio.c:482-495`): `D_800B3D54[0..1] = 0`, `audio_bgm_set = 0xFF`, the bank-cache LBAs `D_800B3D30/34/38/3C = 0`, the loaded flags `D_800B3D28/2C = 0`, the poll counter `D_800B3D4C = 0`, `D_800B3D50 = 0`, `audio_bgm_sequence = 0`. Because the cache LBAs are cleared, step 7 always issues all five reads.
5. `audio_command(5)`, which sets `audio_state.state_flags |= 0x100000` (`sound_driver.c:312`).
6. `audio_command(0xD1)`, the BGM fade with `bgm_fade_rate = 0x80`, `bgm_fade_step = 1` (`sound_driver.c:717-720`). On the normal path, commands 5 and 0xD1 are therefore sent twice, once in stage 20 and once here.
7. `audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD3D5, 0xD387, 0xD37D)` (`frame_render_audio.c:497-546`, `a == 0` branch). It sets `audio_bgm_set = 0xFF`, `D_800B3D24 = 0`, `D_800B3D50 = 1` and `D_800B3D20 = cd_done_count()`, which is 0 straight after the reset. It sets the CD load base to `0x801B8000` and pushes five non-waiting reads in this order:

| # | arg | sectors | destination | LBA | disc file (file sectors) | over-read past the file end |
|---|---|---|---|---|---|---|
| 1 | b (SE header) | 3 | 0x80010000 | 0xBF98 | `CDROM/VAB_SE/COM_MIX.VH` (2) | 1 sector of `VAB_SE/EFF1.BAK` (0xBF9A) |
| 2 | c (SE body) | 0x20 | 0x801B8000 | 0xBF79 | `CDROM/VAB_SE/COM_MIX.VB` (31) | 1 sector = `COM_MIX.VH` (0xBF98) |
| 3 | f (sequence) | 0x25 | 0x8001C000 | 0xD37D | `CDROM/VAB_SEQ/SPRING.SEQ` (10) | 27 sectors = the first 27 of `SPRING.VB`; the region ends exactly at 0x8002E800, the bank-1 sequence base |
| 4 | e (BGM body) | 0x50 | 0x801C8000 | 0xD387 | `CDROM/VAB_SEQ/SPRING.VB` (78) | 2 sectors = `SPRING.VH` (0xD3D5..0xD3D9, 5 sectors, so only the first 2) |
| 5 | d (BGM header) | 6 | 0x80012000 | 0xD3D5 | `CDROM/VAB_SEQ/SPRING.VH` (5) | 1 sector of `STUDIUM.SEQ` (0xD3DA) |

`D_800B3D30 = 0xBF98` and `D_800B3D34 = 0xD3D5` are set (the second write repeats the first). `D_800B3D28` and `D_800B3D2C` stay 0. This is the spring day-music set: [[spec/m-hud]] section 10 lists `season_bgm_lba_d/e/f[0]` = 0xD3D5/0xD387/0xD37D. It is hard-coded here, so stage 21 does **not** call `hud_day_bgm_load`. The other route into stage 0xFF, `case 0xFF` at `home_screens.c:1453-1457`, does call it.

8. `substep += 1`, `return`.

### Substep 1, also `default:` (one frame)

1. `D_800B3DB0 = 0`, which disables the `move_255_line` column copy in `screen_layers.c:225-227`.
2. `k_speed_set(TEXT_SPEED_SLOW)` sets `text_state.speed = 3` (`text_system.c:200-205`; enum at `include/main/text.h:83`).
3. `game_state.menu_mode = MENU_MODE_A` (1, `include/game_enums.h:263`).
4. `game_mode_set(0x34)` (`game_mode.c:186-206`) sets mode 0x34 and clears stage, step, `game_mode_timer`, `mode_stage_timer`, `mode_step_timer`, both scratch bytes and substep. It then calls `game_mode_request_overlay()`. Mode 0x34 is resident in main and not one of the 48 overlay modes ([[spec/m-flow]] 3.3), so the mode becomes 0x34 at once, no loader frames run, and NAME_ENT stays resident. `talk_voice_wait_frames = 0`. `ovl_target_game_mode` still holds 0x15 from the name-entry request, which is neither 0x5E nor 0x50, so the call ends with `message_window_title_show(0)` (window title hidden).
5. `mode_stage_set(0xFF)` sets stage 0xFF with step, timers, scratch and substep 0.

The same frame's tail then runs `message_window_title_update()`, `hud_screen_frame_draw(2)` (stage 0xFF is not 0), `message_window_show()` and the START test (section 5).

Stage 21 makes **no** RNG calls, text-line calls, actor setup or tpage changes, and no memcpy. Actors 96-159 and the stage's `tpage_buf` reservations from the last stage that ran stay live, and the main-loop `actor_draw_all` (`frame_render_audio.c:61-62`) keeps drawing and stepping them. They are cleared only by `actor_clear_upper` in command-screen step 0 (next frame), and the tpage table only by `tpage_buf_clear` in the build step (section 3). A port must keep drawing the retained prologue actors for these frames, using the slot assignment of the free-order model (for example `STAGE10_FREE_TPAGE_SLOTS` in `first_arrival_board_store.gd:92`), and must not blank them early.

## 3. Mode 0x34 stage 0xFF: `settings_command_screen_mode_stage_reload` (`home_screens.c:76-104`)

The dispatcher is `game_mode_dispatch` case 0x34, which calls `settings_command_screen_mode_handler` (`home_screens.c:46-73`). Each frame it runs the stage function, then `hud_mascot_update()` if the mascot actor's visible bit is set (`*D_8011F3FF & 0x80`), then `hud_screen_frame_draw(0)` (all 80 cells, `checker_ot_slot` 0xF).

The reload stage per frame: `mode_step_timer += 1`, the step function, then `hizuke_show()`, `message_window_show()`, `parameter_show()`, `hud_icon_bar_update(0)` and `hud_cursor_update()`. If `mode_step >= 2` **after** the step function, it also runs `message_window_title_update()` and adds 4 to the u8 `screen_brightness` while it is below 0x80. Stage 21 already set the brightness to 0x80, so on this route the ramp never fires. **This stage has no `dec_bg_draw` call either**; the room picture first appears in stage 1.

| step | function | substep and effect | next |
|---|---|---|---|
| 0 | `settings_command_screen_setup_reset` (`:131-142`) | `draw2d3d(1,0)`, `actor_draw_enable(1)`, `sprite_tables_clear()`, `actor_clear_upper()` (actors 0x60..0x9F, `actor_sprites.c:641-647`), `tile_layer_enabled = 0`, `boot_hold = 0`, `set_dec_bri(0)` (background brightness only; `screen_brightness` stays 0x80), `text_system_reset()`, `k_disp_start(2)`, `mode_step_next()` | step 1, next frame |
| 1 | `settings_command_screen_reload_calendar` (`:206-227`) | substep 0: `audio_bank_load_poll()` (`frame_render_audio.c:652-676`); when it returns 1, `audio_bank_start(0,0)`, then substep 1. Substep 1: `cd_read_sectors(0x95, 0x801A0000, 0x9B94)` = `CDROM/FUKUTEST/CALROM.NBN` (149 sectors, exact), then substep 2. Substep 2: when `cd_read_sync_status() & 1`, run `calendar_month_load(year, month, 0)`, `menu_mode &= 3` (stays 1), `D_800B5BC8 = 0xFE` (season-picture fade counter), and if `hud_mascot_choice < 0xE` call `hud_mascot_set(choice)` and `hud_mascot_display_switch(1)`; then `mode_step_next()` | step 2 |
| 2 | `settings_command_screen_build` (`:231-332`) | see section 3.2 | step 3 (CD read) or stage+1 (cached) |
| 3 | `settings_room_bg_load_finish` (`:352-364`) | when `cd_read_sync_status() & 1`: `dec_bg_show_set(0, dec_bg_decode_to_vram(0))`. On `bg_decode_result == -1`: `dec_bg_reset()` and the read again. Otherwise: `set_dec_bri(0x80)`, `screen_brightness = 0x80`, `mode_stage_set(1)` | stage 1 |

### 3.1 Bank poll timing (step 1 substep 0)

`audio_bank_load_poll` does nothing until `((cd_done_count() - D_800B3D20) + 0x100) & 0xFF >= 5`, which means all five stage 21 reads have finished (the counter started at 0). On the first frame that passes, `D_800B3D4C == 0`, so `audio_bank_transfer_start()` runs (`:615-650`): bank 0 with both loaded flags 0 gives `cd_load_destination_base_set(0x801B8000)` and `audio_command(0x11)`, and the counter becomes 1. On each later frame the counter is pre-incremented, and the poll succeeds when the counter passes 0xC0 or `audio_bank_transfer_done(0)` (`audio_bgm_bank_ready(0) && audio_bgm_bank_ready(1)`). On success it sends commands `0x22`, `0x21`, `0x2F` (0x2F is `audio_sequence_pack_index(0x8001C000, 0)`, `sound_driver.c:285-287`), sets `D_800B3D54[0] = 0` and returns 1. The caller then runs `audio_bank_start(0, 0)` (`:691-715`). It checks `*(u32*)0x8001C000 & 0xFFFF == 0x40` and returns -1 without effect otherwise; the C ignores this result. On a pass it sends `0x22`, `0x21`, `0x2F` **again**, then `0x200` (sequence 0), and sets `audio_bgm_set = 0`, `audio_bgm_sequence = 0`, `audio_bgm_playing = 1`. Preserve the duplicated command triple and the 0xC0-frame timeout path; the timeout counts as success.

### 3.2 Build step on the new-game route (`settings_command_screen_build`)

The general rule table is in [[spec/m-sysmenu]] 3.2. The order that matters for the port:

1. `back_clear_switch(0)`, `actor_clear_upper()`, `sprite_tables_clear()`, clear colour 0/0/0, `text_system_reset()`, window title text `平日コマンド画面` (hidden), `gnsx(&player_family_name)` (speaker label), `parameter_show_init()` twice, `hizuke_init()`, `message_window_init()`, `parameter_disp_switch(1)`.
2. **`tpage_buf_clear()`** (`actor_sprites.c:403-414`): every slot with `(i & 0xF) < 5` becomes 0, the rest -1, then `_sys_default_tpage_set` reserves 11-15, 21-25, 43-47 and 53-57 with 0x77. This is the point where all prologue texture-page state (including the stage 10 free-order slots 5, 6, 7, 37, 38, 39, 58) is released.
3. Row selection from `month_calendar[0].marker[day - 1 + 2]` ("tomorrow") and `marker[day - 1 + 7]`, plus `player_flags`. On 1995-04-04 the row depends on the April 1995 markers in CALROM, as loaded by step 1. The port must evaluate the original markers and must not assume row 7 (the ordinary row). Rows 1-6 call `settings_command_screen_icons_reset(1)`; row 7 calls it with 0, sets `menu_cursor_0 = last_command & 0xF` and queues the monologue `明日から何をしようかな？）` with `sndisp(..., 0, 0x1F)`.
4. `settings_command_screen_icons_reset` (`:366-383`): icons 0xA, 8 and 9 are **always** off; icon 0xE is off when `icon_mode == 2`; icon 4 is off when the club nibble `player_flags.h >> 12 == 0xB`; with a nonzero argument, commands 0-7 are off.
5. `k_disp_start(4)`, `hud_cursor_actor_init(0)`; on `year == 0x5F && month == 4 && day == 4`, `icon_can_use_set(0xD, 0)` (the first day cannot open the data/card icon); `menu_bar_color(0x10, 0, 0, 8)`.
6. `settings_room_bg_load_start()` (`:334-350`): `dec_bg_cd_read(D_800B5950[hud_season_index()], 0)`. April gives season 0. `D_800B5950 = {0x3FE9, 0x3FF2, 0x3FD7, 0x3FE0}` (`asm/data/data_800B5950.data.s:10-13`); 0x3FE9 = `CDROM/BS/C_02_02H.BS` (9 sectors). `dec_bg_cd_read` (`background_images.c:62-83`) compares the LBA with `bg_lba_cache[0..1]`. On a miss it reads **10** sectors (one past the file end, into `C_02_02N.BS`) to 0x80162000, writes `bg_lba_cache[1 - dec_bg_show[0]] = 0x3FE9` and returns -1, which leads to `mode_step_next()` (step 3). A hit returns 0 or 1: `dec_bg_show_set(0, r)`, brightness 0x80, `mode_stage_next()`, so stage 0xFF wraps the u8 to **stage 0**, and the full setup (reset, wait, build) runs again ([[spec/m-sysmenu]] 5.4).
   - On this route it always misses. Prologue stage 0 substep 0 calls `dec_bg_reset()` (`prologue.c:218`), which zeroes all four cache LBAs and `dec_bg_show[0..2]`, and every prologue background read uses slot 1 (`dec_bg_cd_read(x, 1)`; there is no slot 0 read or show in `prologue.c`). START cannot skip before that reset, because the skip needs stage 0 step 1 or later. So: cache miss, step 3, `dec_bg_decode_to_vram(0)` with `dec_bg_show[0] == 0`, giving case 1, decode 320×88 to VRAM (0x140, 0x1A0), and `dec_bg_show_set(0, 1)`.

Stage 1 (`settings_command_screen_mode_stage_select`) is the first frame where the player can choose; this is the command-ready boundary. From there the documented [[spec/m-sysmenu]] 3.3 rules apply, and icon 0 leads to `game_mode_set(0x30)`.

### 3.3 Frame budget (normal path, source order)

The frame numbers are logical and relative to the frame where stage 20 calls `mode_stage_next`. Stalls inside CD and audio hardware stretch them.

| frame | what runs |
|---|---|
| F | stage 20 sends 5 and 0xD1, then stage 21 |
| F+1 | stage 21 substep 0: reset, five bank reads queued |
| F+2 | stage 21 substep 1: mode 0x34, stage 0xFF |
| F+3 | 0xFF step 0: reset (actors cleared) |
| F+4… | step 1 substep 0: wait for 5 reads, then 1 frame of transfer start (0x11), then transfer wait (at most 0xC0 frames) |
| +1 | substep 1: CALROM read issued |
| +n | substep 2: CALROM done, then step 2 |
| +1 | step 2 build: room read issued, then step 3 |
| +n | step 3: room decode, then stage 1 (command ready) |

## 4. RNG, text, lip sync and XA

- RNG: none in stage 21 or in stage 0xFF on a new game. The only `rand()` on these frames is the health-state-4 jitter in `hud_screen_frame_draw`.
- Text: no prologue line starts after line 42 (stage 20). `text_system_reset` in steps 0 and 2 discards the prologue text list, and the build queues the one monologue line with speaker code 0x1F (`k_disp_start(4)`). No voice clip is requested and there is no lip sync.
- XA: stage 21 issues no voice stop. Line 42's clip normally ended in stage 20, because substep 2 is reached only after `prologue_line_run` finishes. On the skip path a clip may still be streaming when `cd_command_reset` clears `cd_audio_position` and `cd_state`, and what the hardware does next is unproven (section 6).

## 5. Edge cases the port must reproduce or explicitly reject

1. **START on the switch frame.** On stage 21 substep 1 the tail START test runs with mode 0x34 and stage 0xFF; since stage != 0, a START edge calls `mode_stage_set(0x15)`. Mode 0x34 has no stage 0x15 case, so the command screen would only run the mascot and frame draw and never progress (a source soft-lock). This needs two START edges on consecutive logical updates (the skip at F-1 or earlier, release, press at F+1), which a person is very unlikely to manage. The port must either reproduce it or reject it explicitly. It must not silently ignore it.
2. **START during stage 21 substep 0** sets stage 21 substep 0 again. The next frame repeats the whole reset and bank request (all five reads again, `D_800B3D20` re-based).
3. **START on the stage 20 frame that advances**: stage 21 is set twice, with no difference.
4. **Unknown substep**: source `default:` behaves like substep 1. Substep only ever goes 0 to 1, so other values cannot occur.
5. **Cached room picture**: this cannot happen on this route (section 3.2), but the shared reload code must still implement the u8 stage wrap to stage 0.

## 6. Port status (read-only audit of the current checkout)

What exists:

- `port/godot/flow/prologue_arrival.gd:1454-1478` `_stage_21_finish` follows the statement order of section 2 (reset services, the two globals, commands 5 and 0xD1, the `normal_gameplay` bank role, the field reset to 0x34, stage 0xFF). The tail START test is at `:121-123`.
- `port/godot/flow/new_game_command_reload.gd` implements stage 0xFF steps 0-3 with source constants (0x95, 0x801A0000, 0x9B94, 0xFE, mascot < 14, brightness +4 after the step).
- `port/godot/services/opening_audio_rules.gd` `BANK_TRANSITION_CONTRACTS["normal_gameplay"]` holds the five LBAs. `tools/port/prologue_bank_content.py` parses the literal `audio_bank_load_start` arguments of `prologue_finish_run` and binds each LBA to a unique ISO entry (file bytes plus SHA-256).
- `tools/port/hud_content.py:27-28` exports `CALROM.NBN` and `C_02_02H.BS` with hashes. `port/godot/scenes/month_start_controller.gd` already models the CALROM read.

Gaps and divergences, each a defect or open item:

1. **No installed handoff (R21 in [[port-adversarial-review]]).** `app/first_arrival_boot.gd:319` accepts only modes 0x14, 2 and 0x15. `flow/interactive_prologue.gd:230-235` binds 0x34 only for stage 0xFF and rejects later stages. No production owner carries the session into stage 1 selection.
2. **The `PlayableBoot` reload services are logging stubs** (`app/playable_boot.gd:206-222`). `setup_reset` only advances the step; `hud_read_start`, `calendar_upload` and `mascot_set` only log, so no CALROM bytes are read or uploaded; `room_finish` sets `command_ready` and stage 1 with no room read, no decode and no `bg_decode_result` retry. `bank_load_start` (`:68-80`) queues and immediately drains the bank in one phase, so readiness does not depend on the five reads plus the transfer of section 3.1. This is synthetic readiness and must be replaced.
3. **Stage 0 step 1 is mis-modelled.** `new_game_command_reload.gd` maps stage 0 step 1 onto the reload calendar substeps (`hud_substep = substep + 1`). The source runs `settings_command_screen_setup_wait` there (`k_disp_inc`, `mode_step_next`, `home_screens.c:144-147`). Stage 0 also calls `dec_bg_draw(get_last_gamen_mode())` every frame and runs `message_window_title_update` unconditionally.
4. **The build diverges from the source** (`build_command_screen`, `:49-91`). It ties icons 8/9/10 to `vacation` (the source always disables them), treats icon 13 as a phone icon gated by girl flags (the source disables 13 only on 1995-04-04), uses `club_id >= 11` (the source uses `== 0xB`), has no row table (markers, remedial week, sick) and no icon 14. It also omits `tpage_buf_clear`, the monologue `sndisp` and `k_disp_start(4)`, `hud_cursor_actor_init`, `menu_bar_color` and `settings_room_bg_load_start`.
5. **The bank contract also accepts an alternate triple.** `matches_bank_load_args("normal_gameplay", ...)` accepts the alternate 0xCCA6/0xCCA1/0xCC9D. Stage 21 has only the spring triple, so stage 21 validation should pin the primary triple.
6. **Unsourced XA stop.** `OpeningAudioRules.finish_cd_reset` (`opening_audio_rules.gd:244-248`) cancels the voice and emits `native_stop_xa_if_active`; `cd_command_reset` does no such thing. `finish_bank_reset` (`:251-252`) only sets a flag and does not reset the bank cache or loaded-state fields. Either cite emulator evidence for the stop or remove it, and model the reset fields.
7. **Draw gaps on the stage 21 frames.** `_stage_21_finish` calls `outer_draw(2)`, which is right, but it must not draw the prologue background (there is no `dec_bg_draw`), and the retained actors must keep drawing. The current `outer_draw` and `logical_draw` bindings were not checked for this.
8. **The tail START check after the mode switch** writes stage 21 into mode 0x34, which `new_game_command_reload.gd:102` then rejects. That is an explicit rejection of edge case 5.1; record it as such in tests.
9. **`game_mode_set` side effects**: `reset_game_mode_fields` (`core/game_state.gd:33-43`) clears fields only. The window-title hide and `talk_voice_wait_frames = 0` must come from the router or the session; check this.

## 7. Resources the exporter must add or prove

1. **Bank read extents.** The five stage 21 reads copy whole sector ranges (section 2, table), and the current bank metadata pins only file bytes. Either export the sector-exact extents, which include the trailing sectors of `EFF1.BAK`, `COM_MIX.VH`, `SPRING.VB` (27 sectors), `SPRING.VH` and `STUDIUM.SEQ`, or prove from `sound_driver.c` that the VAB and SEQ consumers never read past each file's length. The SEQ case matters most, because its 37-sector read fills 0x8001C000-0x8002E7FF.
2. **Room read extent.** `C_02_02H.BS` is read as 10 sectors, so one sector of `C_02_02N.BS` lands at 0x80162000+0x4800. Prove the BS decoder stops at the stream end, or export the extent.
3. **CALROM April 1995 markers** (already in the HUD pack): expose the decoded `month_calendar[0].marker` entries at indices `day + 1` and `day + 6` (5 and 10 for day 4) for the row decision, from the original file only.
4. **Main-executable strings** for the build: the window title `平日コマンド画面` and the nine monologue strings at `home_screens.c:258-323`, with their speaker code 0x1F. They live in main rodata, not NAME_ENT. Bind them to the executable with an address and hash rather than copying literals into runtime code.
5. **Initial `hud_mascot_choice`** for a new game is already a pack constant (`initial_mascot`). Verify it against `boot_game_data_init` and the name-entry exit, not against a captured context.

## 8. Verification still required

Use a PCSX-Redux (`tokimemo-emu`) trace from the end of stage 20, and separately from a START skip during an XA line. Record per logical frame: mode, stage, step, substep, the `cd_read_count` progression, the order and destinations of the five bank reads, the audio command sequence (5, D1, 0x11, 0x22/0x21/0x2F twice, 0x200), whether the XA stream stops on `cd_command_reset`, the frame where stage 1 is reached, and the actor and tpage state on F+1 and F+2. None of this has been captured for this page; every timing claim above comes from the source alone.
