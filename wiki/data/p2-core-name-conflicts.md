---
type: data
updated: 2026-10-24
ticket: T-27010
sources: ["config/renames/proposals/core.tsv", "p2a-h-cdio config/renames/proposals/h-cdio.tsv", "p2a-h-graphics config/renames/proposals/h-graphics.tsv"]
---

# Phase 2 core: names that differ from the hardware units

Generated on 2026-10-24 from the proposal tables of the `core`, `h-cdio` and `h-graphics` worktrees (same kind, scope and old name, different new name). 96 rows. Default winner when the orchestrator makes no other decision: the hardware table, except the rows marked `core`, which are game logic and not hardware. The tables are still being reviewed, so regenerate the list before the apply step. Rule and the sound prefix question: [[spec/core]] section 17.

| kind | scope | old | core name | other name | unit | winner |
| --- | --- | --- | --- | --- | --- | --- |
| data | main | `D_800B3D70` | `cd_audio_request` | `xa_clip_request` | h-cdio | hardware |
| data | main | `D_800B5938` | `dec_bg_show` | `dec_bg_shown` | h-cdio | hardware |
| data | main | `D_800B5948` | `last_screen_mode` | `dec_bg_last_layer` | h-cdio | hardware |
| data | main | `D_8011ECA0` | `draw_buffer` | `draw_buffer_index` | h-graphics | hardware |
| data | main | `D_801217D0` | `sprite_quads` | `sprite_table` | h-graphics | hardware |
| data | main | `D_801220D0` | `bg_layers` | `bg_slots` | h-graphics | hardware |
| data | main | `D_80125130` | `cd_cmd_ring` | `cd_queue` | h-cdio | hardware |
| field | DisplayActor44 | `unk_02` | `flags` | `anim_flags` | h-graphics | hardware |
| field | DisplayActor44 | `unk_06` | `blink_period` | `anim_speed` | h-graphics | hardware |
| field | DisplayActor44 | `unk_0C` | `frame_table` | `image_table` | h-graphics | hardware |
| field | DisplayActor44 | `unk_16` | `sequence_slot` | `sequence_index` | h-graphics | hardware |
| field | DisplayActor44 | `unk_18` | `frame` | `frame_index` | h-graphics | hardware |
| field | DisplayActor44 | `unk_24` | `x_frac` | `x` | h-graphics | hardware |
| field | DisplayActor44 | `unk_28` | `y_frac` | `y` | h-graphics | hardware |
| field | DisplayActor44 | `unk_2C` | `dx_frac` | `velocity_x` | h-graphics | hardware |
| field | DisplayActor44 | `unk_30` | `dy_frac` | `velocity_y` | h-graphics | hardware |
| field | DisplayActor44 | `unk_34` | `tpage_table` | `texture_blocks` | h-graphics | hardware |
| field | DisplayActor44 | `unk_38` | `quad_flags` | `sprite_attribute` | h-graphics | hardware |
| field | DisplayFixActor44 | `unk_02` | `flags` | `anim_flags` | h-graphics | hardware |
| field | DisplayFixActor44 | `unk_2C` | `dx` | `velocity_x` | h-graphics | hardware |
| field | DisplayFixActor44 | `unk_30` | `dy` | `velocity_y` | h-graphics | hardware |
| field | GameState | `unk_000` | `video_width` | `screen_width` | h-cdio | hardware |
| field | GameState | `unk_002` | `video_height` | `screen_height` | h-cdio | hardware |
| field | GameState | `unk_004` | `video_interlace` | `screen_interlace` | h-cdio | hardware |
| field | GameState | `unk_014` | `screen_center_x` | `draw_origin_x` | h-cdio | hardware |
| field | GameState | `unk_014` | `screen_center_x` | `draw_origin_x` | h-graphics | hardware |
| field | GameState | `unk_018` | `screen_center_y` | `draw_origin_y` | h-cdio | hardware |
| field | GameState | `unk_018` | `screen_center_y` | `draw_origin_y` | h-graphics | hardware |
| field | GameState | `unk_01C` | `prim_arena` | `prim_work` | h-graphics | hardware |
| field | GameState | `unk_036` | `clear_color_r` | `back_clear_r` | h-graphics | hardware |
| field | GameState | `unk_037` | `clear_color_g` | `back_clear_g` | h-graphics | hardware |
| field | GameState | `unk_038` | `clear_color_b` | `back_clear_b` | h-graphics | hardware |
| field | GameState | `unk_039` | `back_clear` | `back_clear_enabled` | h-graphics | hardware |
| field | GameState | `unk_03B` | `brightness_step` | `screen_brightness_rate` | h-graphics | hardware |
| field | GameState | `unk_10A2` | `tile_layer_enabled` | `screen_layer_enabled` | h-graphics | hardware |
| field | GameState | `unk_10A5` | `quad_layer` | `sprite_priority` | h-graphics | hardware |
| field | GameState | `unk_10E8` | `quads_enabled` | `display_pass_enabled` | h-graphics | hardware |
| field | GameState | `unk_10EC` | `bg_layer_order` | `bg_slot_priority` | h-graphics | hardware |
| field | GameState | `unk_10F0` | `frame_hcount_limit` | `frame_min_hcount` | h-graphics | hardware |
| field | GameState | `unk_10F4` | `timer_ticks` | `vblank_count` | h-graphics | hardware |
| field | GameState | `unk_1112` | `actors_draw_enabled` | `actor_draw_enabled` | h-graphics | hardware |
| field | GameState | `unk_1128` | `tpage_req` | `tpage_frame_map` | h-graphics | hardware |
| field | GameState | `unk_1228` | `tpage_buf` | `tpage_resident` | h-graphics | hardware |
| field | GameState | `unk_1429` | `csr_queue_count` | `vram_upload_count` | h-graphics | hardware |
| field | GameState | `unk_142C` | `csr_queue` | `vram_upload_queue` | h-graphics | hardware |
| field | Rec24 | `unk_00` | `flags` | `attribute` | h-graphics | hardware |
| field | Rec24 | `unk_08` | `w` | `width` | h-graphics | hardware |
| field | Rec24 | `unk_0A` | `h` | `height` | h-graphics | hardware |
| field | Rec24 | `unk_14` | `tint_r` | `r` | h-graphics | hardware |
| field | Rec24 | `unk_15` | `tint_g` | `g` | h-graphics | hardware |
| field | Rec24 | `unk_16` | `tint_b` | `b` | h-graphics | hardware |
| field | Rec24 | `unk_18` | `anchor_x` | `pivot_x` | h-graphics | hardware |
| field | Rec24 | `unk_1A` | `anchor_y` | `pivot_y` | h-graphics | hardware |
| func | main | `func_80043510` | `frame_render` | `render_frame` | h-graphics | hardware |
| func | main | `func_80044750` | `snd_command` | `sound_request` | h-graphics | hardware |
| func | main | `func_8004482C` | `snd_bank_state_reset` | `sound_bank_state_reset` | h-graphics | hardware |
| func | main | `func_80044890` | `snd_bank_load_start` | `sound_bank_load_start` | h-graphics | hardware |
| func | main | `func_80044E8C` | `snd_bank_load_poll` | `sound_bank_wait_ready` | h-graphics | hardware |
| func | main | `func_8004500C` | `snd_bank_start` | `bgm_play` | h-graphics | hardware |
| func | main | `func_800452C4` | `cd_cmd_reset` | `cd_queue_reset` | h-cdio | hardware |
| func | main | `func_80045318` | `cd_cmd_push` | `cd_queue_push` | h-cdio | hardware |
| func | main | `func_80045414` | `cd_audio_command` | `cd_command` | h-cdio | hardware |
| func | main | `func_800455C4` | `cd_cmd_pump` | `cd_queue_update` | h-cdio | hardware |
| func | main | `func_800460DC` | `cd_done_count` | `cd_read_count_get` | h-cdio | hardware |
| func | main | `func_800460EC` | `cd_audio_state` | `cd_state_get` | h-cdio | hardware |
| func | main | `func_80046274` | `cd_audio_ticks` | `xa_position_get` | h-cdio | hardware |
| func | main | `func_80046290` | `cd_audio_request_set` | `xa_clip_request_set` | h-cdio | hardware |
| func | main | `func_800462C8` | `cd_read_sectors_nowait` | `cd_read_sectors_unchecked` | h-cdio | hardware |
| func | main | `func_800476C0` | `actors_draw` | `actor_pool_draw` | h-graphics | hardware |
| func | main | `func_80048DAC` | `actors_draw_enable` | `actor_draw_enable` | h-graphics | hardware |
| func | main | `func_80048E78` | `actor_clear_upper` | `actor_reset_scene_range` | h-graphics | hardware |
| func | main | `func_80048EB8` | `actor_clear_range` | `actor_reset_range` | h-graphics | hardware |
| func | main | `func_80048F64` | `actor_clear` | `actor_reset` | h-graphics | hardware |
| func | main | `func_80049A40` | `prim_gradient_rect` | `prim_box_gouraud` | h-graphics | hardware |
| func | main | `func_80049FF0` | `prim_gradient_line` | `prim_line_gradient` | h-graphics | hardware |
| func | main | `func_8004AE28` | `debug_number_show` | `debug_print_int` | h-graphics | hardware |
| func | main | `func_8005751C` | `dec_bg_decode_to_vram` | `dec_bg_decode` | h-cdio | hardware |
| func | main | `func_8005A06C` | `mode_boot` | `boot_mode_run` | h-cdio | core |
| func | main | `func_80085E30` | `scene_screen_reset` | `scene_gfx_init` | h-graphics | hardware |
| func | main | `func_8008667C` | `girl_state_rank` | `girl_state_code` | h-graphics | core |
| func | main | `func_800866A0` | `bustup_read_step` | `overlay_load_verify_step` | h-graphics | hardware |
| type | main | `CdRingView` | `CdRingState` | `CdQueueState` | h-cdio | hardware |
| type | main | `DisplayActor44` | `Actor` | `DisplayActor` | h-graphics | hardware |
| type | main | `DisplayFixActor44` | `ActorFixed` | `DisplayActorFixed` | h-graphics | hardware |
| type | main | `DisplayImage78` | `SpriteFrame` | `ActorImage` | h-graphics | hardware |
| type | main | `DisplayImagePart0E` | `SpritePart` | `ActorImagePart` | h-graphics | hardware |
| type | main | `GsRec01C` | `PrimArena` | `PrimWorkArea` | h-graphics | hardware |
| type | main | `GsRec1328` | `PaletteLoad` | `PaletteUpload` | h-graphics | hardware |
| type | main | `GsRec142C` | `CsrLoad` | `VramUpload` | h-graphics | hardware |
| type | main | `ModRec14` | `ModTileSet` | `TileSet` | h-graphics | hardware |
| type | main | `MouseOut` | `MouseSample` | `MouseReading` | h-cdio | hardware |
| type | main | `Rec24` | `SpriteQuad` | `GsSprite` | h-graphics | hardware |
| type | main | `SpriteCell8` | `BgCell` | `GsCell` | h-graphics | hardware |
| type | main | `SpriteMap10` | `BgMap` | `GsMap` | h-graphics | hardware |
| type | main | `SpriteSlot24` | `BgLayer` | `GsBg` | h-graphics | hardware |
| type | main | `SrnRec28` | `SrnLayer` | `ScreenLayer` | h-graphics | hardware |
