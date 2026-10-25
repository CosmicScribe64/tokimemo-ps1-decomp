#ifndef OVL_FESTIVAL_STALLS_H
#define OVL_FESTIVAL_STALLS_H

#include "common.h"
#include "psx_types.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "libgte.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/layer.h"
#include "main/message.h"
#include "main/pad.h"
#include "main/prim.h"
#include "main/scene.h"
#include "main/settings.h"
#include "main/state.h"
#include "main/tpage.h"
#include "main/vram.h"
#include "main/vsync.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "libgte.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/layer.h"
#include "main/message.h"
#include "main/pad.h"
#include "main/prim.h"
#include "main/scene.h"
#include "main/settings.h"
#include "main/state.h"
#include "main/tpage.h"
#include "main/vram.h"
#include "main/vsync.h"
/* Actor view of actor_pool (T-18200) */

typedef struct EnObj {
    /* 0x00 */ s32 kind_flags;
    /* 0x04 */ s32 steering_profile;
    /* 0x08 */ s32 speed_profile;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ Fix32 x; /* 16.16 x: words in stalls_object_motion_update, the integer half .h[1] elsewhere (T-20920) */
    /* 0x18 */ Fix32 y; /* 16.16 y */
    /* 0x1C */ s32 heading;
    /* 0x20 */ s32 movement_step; /* 16.16 step of the motion of stalls_object_motion_update (T-20920) */
    /* 0x24 */ s32 steering_frames;
    /* 0x28 */ s32 speed_class;
    /* 0x2C */ s32 steering_flags;
} EnObj; /* size 0x30, stalls_objects[10] */

/* The ten objects; splat labels D_80139862..D_801398AC are fields of records 0..2 (T-18200). */
extern EnObj stalls_objects[10];

/* EN_NICHI data (overlay rodata/data) */
extern s32 stalls_audio_command_cooldown;
extern s32 stalls_saved_cursor_speed;
extern s32 D_80139210;
/* T-18200: asset addresses read by stalls_actor_graphics_setup and stalls_title_fade_input_dispatch; D_80139204 points at three tpage addresses. */
extern s32 D_80139200;
extern s32 *D_80139204;
extern s32 D_80139208;
extern s32 D_8013920C;
extern s32 D_80139214;
extern s32 D_80139218;

/* EN_NICHI functions */
void stalls_first_year_mode_stage_dispatch(void);
void stalls_wave_grid_cell_draw(s32 arg0, s32 arg1);
void stalls_later_year_callback_dispatch(void);
/* Entries of the 80136B10.c scene table D_80139C5C (defined in that file, T-25201). */
void stalls_presentation_intro_dispatch(void);
void stalls_game_actor_setup(void);
void stalls_game_state_dispatch(void);
void stalls_object_out_of_bounds_clear(void);
void stalls_random_object_spawn(s32 edge_spawn);
void stalls_object_spawn_position_init(s32 object_index, s32 edge_spawn);
void stalls_object_type_count(s32 *regular_count, s32 *kind4_count);
void stalls_object_collision_actor_setup(); /* K&R definition */
void stalls_object_ai_update_dispatch(void);
void stalls_object_actor_render_update(void);
void stalls_cursor_position_update(void);
s32 stalls_wave_start(s32 center_x, s32 center_y); /* implicit int, no return value */
void stalls_particle_grid_position_update(void);
void stalls_active_wave_update(void);
void stalls_wave_grid_draw(void);
void stalls_audio_command_debounce(s32 audio_command_id);
void stalls_score_panel_fade_draw(void);
void stalls_counter_digit_set(s32 arg0, s32 arg1);
s32 stalls_brightness_ramp_up(void);
void stalls_object_mode_substate_dispatch(void);
void stalls_cursor_and_selection_input(void);
void stalls_particle_field_update(void);
void stalls_wave_response_update(void);
void stalls_object_scene_update(void);
void stalls_light_animation_update(void);

extern s32 D_801391E0;
extern s32 D_801391E4;
extern s32 D_801391E8;
extern s32 D_801391EC;
extern s16 D_801391F0;
extern s32 D_801391F4;
extern s32 D_801391F8;
extern s32 D_801391FC;

extern s32 stalls_object_substate;
s32 stalls_target_selection_score_update();
void stalls_cursor_object_proximity_classify(void);

s32 stalls_target_release_score_update();
void stalls_score_completion_update();
void stalls_idle_timeout_update();

extern s32 stalls_substate_frames;
extern s32 stalls_wave_ages[10];
void stalls_wave_grid_displacement(s32 wave_index);
void stalls_object_special_ai_update();
void stalls_object_motion_update(s32 object_index);
void stalls_object_random_steering_update(s32 object_index);
extern s32 D_8013921C;

void stalls_intro_scene_reset(void);
void stalls_intro_sector_load(void);
void stalls_palette_load(void);
extern s32 D_80139220;
extern s32 D_80139224;
extern s32 D_801392A4;
extern s32 D_801392A8;

extern s32 D_80139B20;

s32 stalls_cursor_distance_squared();
void stalls_object_steer_handler_00();
void stalls_object_steer_handler_01();
void stalls_object_steer_handler_02();
void stalls_object_steer_handler_03();
s32 stalls_cursor_relative_direction_sector(s32 object_index);

extern u16 stalls_launch_charge;
extern u8 stalls_remaining_target_count;
extern u8 stalls_target_motion_active[8];
extern u8 stalls_target_motion_speeds[8];
extern u8 stalls_target_motion_frames[8];
extern s16 stalls_collected_result_codes[6];
void stalls_target_collision_update(void);
void stalls_progress_meter_draw(void);
s32 stalls_target_collision_resolve();
void stalls_eligible_girl_frame_select(void);
#endif /* OVL_FESTIVAL_STALLS_H */
extern s32 D_80139B24;

#ifndef OVL_EN_NICHI_T9720_TABLES
#define OVL_EN_NICHI_T9720_TABLES
/* Local copies in stalls_target_collision_resolve: twelve halfwords and 33 callbacks. */
typedef struct EnHalf12 {
    /* 0x00 */ s16 value[12];
} EnHalf12; /* size 0x18 */
typedef struct EnFn33 {
    /* 0x00 */ s32 (*f[33])();
} EnFn33; /* size 0x84 */
extern EnHalf12 D_80139D04;
extern EnFn33 D_80139D1C;
#endif


#ifndef W9_EN_RESET_TABLE
#define W9_EN_RESET_TABLE
/* A 16.16 point (x, y): stalls_wave_grid_displacement reads and writes the words, the other users the integer
 * halves .h[1] (T-20920; T-9710 had four halfwords). */
typedef struct EnFixPos {
    /* 0x00 */ Fix32 x;
    /* 0x04 */ Fix32 y;
} EnFixPos; /* size 0x08 */
extern EnFixPos D_801392AC[15][12]; /* the point grid: 15 rows of 12 (stalls_particle_grid_reset, stalls_particle_grid_position_update, stalls_wave_grid_displacement, stalls_wave_grid_cell_draw) */
#endif

#ifndef OVL_EN_NICHI_DISTANCE_T10020
#define OVL_EN_NICHI_DISTANCE_T10020
extern s32 stalls_regular_object_count;
extern u8 stalls_object_proximity_classes[12];
/* Exclusive end of the ten status bytes beginning at stalls_object_proximity_classes. */
extern u8 D_80139B12[];
#endif

#ifndef OVL_EN_NICHI_RESET_T10020
#define OVL_EN_NICHI_RESET_T10020
extern s32 stalls_wave_ring_index;
extern s32 stalls_selection_meter;
extern s32 stalls_kind4_object_count;
extern s32 stalls_pad_code_stage;
void stalls_particle_grid_reset(void);
#endif

#ifndef OVL_EN_NICHI_TABLES_T10020
#define OVL_EN_NICHI_TABLES_T10020
void stalls_intro_substep_dispatch(void);
void stalls_saved_caller_state_restore(void);
void stalls_game_state_reset(void);
void stalls_game_frame_update(void);
extern u8 D_80139A2C[8]; /* follows the stalls_objects records, read as their end (T-10810); object kind -> sprite flag (T-18200) */
extern EnFixPos D_80139228[10]; /* pop-up centres (T-10810), ten up to stalls_wave_ages */
#endif

#ifndef OVL_EN_NICHI_T10850_TYPES
#define OVL_EN_NICHI_T10850_TYPES
/* T-10850: 64 words copied to the local motion lookup table. */
typedef union EnWords64 {
    /* 0x000 */ s32 words[64];
    /* 0x000 */ s32 value[8][8]; /* matrix access in stalls_particle_grid_position_update, T-14000 */
} EnWords64; /* size 0x100 */
extern EnWords64 D_80139B28;
#endif

#ifndef OVL_EN_NICHI_T18200_TYPES
#define OVL_EN_NICHI_T18200_TYPES
/* T-18200: sprite frames of the objects, six halfwords per row; stalls_object_actor_render_update reads row
 * stalls_object_direction_sector_select(i) for kinds 1..3 and row 8 + stalls_object_direction_sector_select(i) for kind 4. Ends at stalls_selection_meter. */
extern s16 D_80139A34[16][6];

/* T-18200: tables that stalls_game_actor_setup and stalls_eligible_girl_frame_select copy to the stack as one block
 * (lw/sw runs, then an lhu/sh tail for the odd sizes); element type from the drafts' reads. */
typedef struct EnHalf22 {
    /* 0x00 */ s16 value[22];
} EnHalf22; /* size 0x2C */
typedef struct EnBytes22 {
    /* 0x00 */ u8 value[22];
} EnBytes22; /* size 0x16 */
typedef struct EnHalf64 {
    /* 0x00 */ s16 value[64];
} EnHalf64; /* size 0x80 */
typedef struct EnHalf13 {
    /* 0x00 */ s16 value[13];
} EnHalf13; /* size 0x1A */
extern EnHalf22 D_80139C68; /* stalls_game_actor_setup: 22 display records, +0x14 */
extern EnHalf22 D_80139C94; /* stalls_game_actor_setup: +0x22 */
extern EnHalf22 D_80139CC0; /* stalls_game_actor_setup: +0x26 */
extern EnBytes22 D_80139CEC; /* stalls_game_actor_setup: +0x01 */
extern EnHalf64 D_80139DA0; /* stalls_eligible_girl_frame_select: picked by rand() & 0x3F */
extern EnHalf13 D_80139E20; /* stalls_eligible_girl_frame_select: one value per girl, 13 */
#endif
