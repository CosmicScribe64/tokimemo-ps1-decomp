#ifndef OVL_TT_H
#define OVL_TT_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_cd_read_sectors_nowait /* implicit declaration, as matched; main_api.h has a prototype with narrow parameters */
#define MAIN_API_OVERRIDE_func_800AD950 /* implicit int declaration: a void prototype changes the register allocation of tt_sound_bank_stage_reload */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"
extern u8 tt_cd_read_count_previous;
extern u8 tt_cd_read_count_now;
extern u8 tt_vram_palette_row[];
extern u8 *tt_player;
void tt_boss1_rope_update(void);
typedef struct TtRec2 {
    /* 0x0 */ u8 flag;
    /* 0x1 */ u8 pad;
    /* 0x2 */ s16 val;
} TtRec2; /* size 0x4 */
extern u8 *tt_run;
extern u16 tt_menu_text_ids[];
extern u16 D_80155D58;
extern u16 D_80155D5A;
extern u16 D_80155D5C;
extern u16 D_80155D5E;
extern u8 *tt_sprites;
extern u8 *tt_texts;
extern u8 *tt_boss;
extern u8 *tt_bombs;
extern u8 *tt_options;
extern u8 *tt_clouds;
extern u8 *tt_air_enemies;
s32 func_8014D260(s32 arg0, s32 arg1);
extern u8 *tt_sys;
extern u8 *tt_frames;
void tt_state_clear_all(void);
extern s32 tt_animation_lists;
extern s32 tt_animation_list_bell_hit; /* T-11810 */
extern s32 tt_animation_list_bell_fall; /* T-11810 */
extern u8 tt_bell_kind_sequence[]; /* T-11810 */

extern u8 *tt_core;
extern s32 *tt_stage_script;
void tt_bell_popup_spawn(u8 *arg0);
extern u8 *tt_text_strings[];
void tt_boss_part_animation_step(void *arg0, s32 arg1);
extern u8 tt_boss1_intro_script[];
extern u8 tt_boss1_arm_frame_by_angle[];
extern u8 tt_boss1_arm_flip_by_angle[];
extern u8 tt_boss1_arm_frame_cycle[];
extern u8 tt_boss3_part_death_script[]; /* T-11000 */
extern u8 tt_boss1_hitbox_body[];
extern u8 tt_boss4_intro_script[];
extern u8 tt_boss4_part_0b_script[]; /* T-11810 */
extern u8 tt_boss4_hitbox_slot_218[];
void tt_boss3_attack_form0(void);
void tt_boss3_attack_form1(void);
extern u8 tt_boss3_part3_palette[];
extern s16 tt_boss3_tip3_y[];
extern u8 tt_boss3_part2_palette[];
extern s16 tt_boss3_tip2_y[];
extern u8 *tt_text_strings_tail[];
extern u8 tt_boss3_part1_sprite[];
extern u8 tt_boss3_part1_frame[];
extern s16 tt_boss3_part1_x[];
extern s16 tt_boss3_part1_y[];
extern u8 tt_boss3_part1_palette[];
extern u8 tt_boss3_part0_sprite[];
extern u8 tt_boss3_part0_frame[];
extern s16 tt_boss3_part0_x[];
extern s16 tt_boss3_part0_y[];
extern u8 tt_boss3_part0_palette[];
extern u8 *tt_boss_rope_buf;
extern u8 *tt_video;
extern u8 *tt_transforms;
extern u8 tt_boss1_core_script_1[];
extern u8 tt_boss1_core_script_2[];
extern u8 tt_boss1_core_script_3[];
extern u8 tt_sprite_set_table[];
extern s32 tt_boss4_aim_angle;
extern u8 tt_boss1_phase2_script[];
extern s16 tt_boss1_fan_x_0[];
extern s16 tt_boss1_fan_y_0[];
extern s16 tt_boss1_fan_angle_0[];
extern s16 tt_boss1_fan_x_1[];
extern s16 tt_boss1_fan_y_1[];
extern s16 tt_boss1_fan_angle_1[];
extern s16 tt_boss1_fan_x_2[];
extern s16 tt_boss1_fan_y_2[];
extern s16 tt_boss1_fan_angle_2[];
extern s16 tt_boss1_fan_x_3[];
extern s16 tt_boss1_fan_y_3[];
extern s16 tt_boss1_fan_angle_3[];
extern s16 tt_boss1_death_timeline[];
extern s16 tt_boss1_expand_timeline[];
extern u8 tt_boss1_ring_palette_cycle[];
void tt_frame_begin(void);
void tt_frame_end(void);
void tt_pause_update(void);
extern void (*tt_state_table[])(void);
extern s32 tt_boss4_shot_x_a[];
extern s32 tt_boss4_shot_y_a[];
extern s32 tt_boss4_shot_x_b[];
extern s32 tt_boss4_shot_y_b[];
extern s32 tt_boss4_shot_y_b_end[];
typedef struct TtPartAnimationView {
    /* 0x00 */ u8 *sets;
    /* 0x04 */ u8 flip;
    /* 0x05 */ u8 set;
    /* 0x06 */ u8 pad_06[2];
    /* 0x08 */ u8 flags;
    /* 0x09 */ u8 pad_09;
    /* 0x0A */ u8 unk_0A;
    /* 0x0B */ u8 pad_0B[5];
    /* 0x10 */ u8 transform_enable;
    /* 0x11 */ u8 pad_11[3];
} TtPartAnimationView; /* size 0x14 */
extern s16 tt_boss3_death_timeline[];
extern u8 tt_boss_death_part_script[];
extern u8 tt_boss2_orbit_script[];
extern u8 tt_boss4_part_script[];
extern s16 tt_boss4_death_timeline[];
extern u8 tt_air_enemy_kind44_pose[];
extern u8 tt_boss2_part4_script[];
extern u8 tt_boss2_part5_script[];
extern u8 tt_boss2_hitbox_slot_230[];
extern u8 tt_boss2_part_set[]; /* T-11810 */
extern u8 tt_boss2_part_frame[]; /* T-11810 */
extern u8 tt_boss2_hitbox_slot_228[]; /* T-11810 */
extern u8 tt_boss2_hitbox_slot_22c[]; /* T-11810 */
extern u8 tt_boss2_hitbox_slot_238[]; /* T-11810 */
extern u8 tt_boss2_hitbox_face1[];
extern u8 tt_boss2_hitbox_face2[];
extern u8 tt_boss2_hitbox_face3[];
extern u8 tt_boss2_hitbox_face31[];
extern u8 tt_boss2_hitbox_face30[];
extern u8 tt_boss2_hitbox_face29[];
extern s16 tt_sprite_set_3_clut;
extern s16 tt_sprite_set_4_clut;
extern s16 tt_boss2_death_timeline[];
extern u8 tt_boss2_intro_script[];
extern u8 tt_boss2_hitbox_core[];
void tt_boss3_move_form0(void);
extern u8 *tt_bells;
extern u8 tt_boss1_side_part_script_alive[];
extern u8 tt_boss1_side_part_script_broken[];

extern u8 *tt_ground_targets;

extern u8 *tt_shots;
extern u8 *tt_bomb_targets;
extern u8 *tt_arms;
extern u8 *tt_enemy_shots;

extern u8 tt_boss3_hitboxes[];
void tt_font_glyph_draw(s16 arg0, s16 arg1, u8 arg2);
void tt_font_string_draw();

extern u8 tt_boss3_hitbox_slot_228[];
extern u8 tt_boss3_hitbox_slot_22c[];
extern s16 tt_sprite_set_9_clut;
void tt_bomb_targets_clear(void);

void tt_boss3_part_hit(void *arg0, u8 *arg1, s32 arg2);
extern u8 tt_boss3_tip_hitboxes;

void tt_player_power_preset(void);
typedef struct TtOtTag {
    /* 0x00 */ u32 addr : 24;
    /* 0x03 */ u32 len : 8;
} TtOtTag; /* size 0x04 */
void tt_air_enemy_setup(u8 *arg0, u16 arg1, s32 arg2);
#endif /* OVL_TT_H */
void tt_vsync_tick(void);
void tt_sprite_animation_step();

#ifndef OVL_TT_W8_3_TYPES_H
#define OVL_TT_W8_3_TYPES_H
/* State-machine records at tt_shots, walked by tt_shots_update. T-9530. */
typedef struct TtShot {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 pad_01[3];
    /* 0x04 */ u16 state;
    /* 0x06 */ s16 phase;
    /* 0x08 */ s16 step;
    /* 0x0A */ s16 step2;
    /* 0x0C */ u8 pad_0C;
    /* 0x0D */ u8 tick0;
    /* 0x0E */ u8 tick1;
    /* 0x0F */ u8 tick2;
    /* 0x10 */ u8 pad_10[0x18];
    /* 0x28 */ s32 velocity_x;
    /* 0x2C */ s32 velocity_y;
    /* 0x30 */ u8 pad_30[0x12];
    /* 0x42 */ u8 u;
    /* 0x43 */ u8 v;
    /* 0x44 */ u16 hit_timer;
    /* 0x46 */ u8 pad_46[0xE];
} TtShot; /* size 0x54 */
#endif /* OVL_TT_W8_3_TYPES_H */

#ifndef OVL_TT_MATCHED_VIEWS_T9520
#define OVL_TT_MATCHED_VIEWS_T9520
typedef struct TtBomb {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 hidden;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 state;
    /* 0x06 */ s16 phase;
    /* 0x08 */ s16 step;
    /* 0x0A */ s16 step2;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 tick0;
    /* 0x0E */ u8 tick1;
    /* 0x0F */ u8 tick2;
    /* 0x10 */ u8 pad10[0x48];
} TtBomb; /* size 0x58; tt_bombs walks eight records, T-9520 */
typedef struct TtBossView {
    /* 0x000 */ u8 pad0[6];
    /* 0x006 */ u16 phase;
    /* 0x008 */ u16 attack_step;
    /* 0x00A */ u8 padA[0xA];
    /* 0x014 */ s16 x;
    /* 0x016 */ s16 y;
    /* 0x018 */ u8 pad18[0x34E];
    /* 0x366 */ u16 phase_timer;
    /* 0x368 */ u8 pad368[0x3C];
    /* 0x3A4 */ u16 angle_offset;
    /* 0x3A6 */ s16 angle_sign;
} TtBossView; /* minimum accessed extent 0x3A8; T-9520 */
typedef struct TtPositionView {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
} TtPositionView; /* size 0x14; tt_angle_between reads these coordinates */
#endif


#ifndef OVL_TT_T9510_ANIM_TYPES
#define OVL_TT_T9510_ANIM_TYPES
/* T-9510: tt_sprite_set_table is a table of pointers to frame-array pointers. */
typedef struct TtAnimFrame {
    /* 0x00 */ u16 tile;
    /* 0x02 */ u8 hold;
    /* 0x03 */ u8 loop_back;
} TtAnimFrame; /* size 0x04 */

/* The animation fields used by tt_sprite_animation_step; remaining extent unknown. */
typedef struct TtAnimationView {
    /* 0x00 */ u8 pad0[5];
    /* 0x05 */ u8 set;
    /* 0x06 */ u8 frame;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u8 flags;
    /* 0x09 */ u8 hold;
} TtAnimationView; /* accessed prefix size 0x0A */
#endif

#ifndef OVL_TT_SLOT68_T9780
#define OVL_TT_SLOT68_T9780
/* Six 0x68-byte slots walked by tt_clouds_update. */
typedef struct TtCloud {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u16 kind;
    /* 0x06 */ u8 pad6[0x62];
} TtCloud; /* size 0x68 */
void tt_clouds_update(void);
#endif

#ifndef OVL_TT_RENDER_RECORDS_T9780
#define OVL_TT_RENDER_RECORDS_T9780
/* Game frame view; SDK rectangles retain their existing RECT type. */
typedef struct TtFrame {
    /* 0x0000 */ void *next;
    /* 0x0004 */ u8 pad4[0x8044];
    /* 0x8048 */ RECT draw_clip;
    /* 0x8050 */ s16 draw_ofs[2];
    /* 0x8054 */ u8 pad8054[0xA];
    /* 0x805E */ u8 unk_805E;
    /* 0x805F */ u8 unk_805F;
    /* 0x8060 */ u8 unk_8060;
    /* 0x8061 */ u8 pad8061[0x43];
    /* 0x80A4 */ RECT disp_rect;
    /* 0x80AC */ RECT disp_screen;
    /* 0x80B4 */ u8 unk_80B4;
    /* 0x80B5 */ u8 unk_80B5;
    /* 0x80B6 */ u8 pad80B6[2];
} TtFrame; /* size 0x80B8 */
void tt_video_init(void);
#endif

#ifndef OVL_TT_CORE_PREFIX_T9780
#define OVL_TT_CORE_PREFIX_T9780
/* Accessed prefix of the state at tt_core; the complete allocation is unknown. */
typedef struct TtCore {
    /* 0x00 */ s16 state;
    /* 0x02 */ u16 sub_state;
    /* 0x04 */ u16 sub_step;
    /* 0x06 */ s16 phase;
    /* 0x08 */ u8 pad08;
    /* 0x09 */ u8 request_next;
    /* 0x0A */ u8 flag_a;
    /* 0x0B */ u8 flag_b;
    /* 0x0C */ u8 pad0C[5];
    /* 0x11 */ u8 pause_allowed;
    /* 0x12 */ u8 pad12[0xA];
    /* 0x1C */ u8 bg_mode;
    /* 0x1D */ u8 fade_r;
    /* 0x1E */ u8 fade_g;
    /* 0x1F */ u8 fade_b;
    /* 0x20 */ u8 pad20[2];
    /* 0x22 */ u8 autoplay;
    /* 0x23 */ u8 pad23[3];
    /* 0x26 */ u16 timer;
    /* 0x28 */ u8 pad28[6];
    /* 0x2E */ s16 fade_level;
} TtCore; /* accessed prefix size 0x30 */

typedef struct TtPadView {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 pressed;
} TtPadView; /* accessed prefix size 0x06 */
void tt_state_ranking_view(void);
#endif


#ifndef OVL_TT_ACTOR_T9760
#define OVL_TT_ACTOR_T9760
typedef struct TtEnemyShot {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u16 state;
    /* 0x06 */ s16 phase;
    /* 0x08 */ s16 step;
    /* 0x0A */ s16 step2;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 tick0;
    /* 0x0E */ u8 tick1;
    /* 0x0F */ u8 tick2;
    /* 0x10 */ u16 layer;
    /* 0x12 */ u8 pad_12[2];
    /* 0x14 */ s16 x;
    /* 0x16 */ s16 y;
    /* 0x18 */ u8 pad_18[8];
    /* 0x20 */ s32 position_x;
    /* 0x24 */ s32 position_y;
    /* 0x28 */ s32 velocity_x;
    /* 0x2C */ s32 velocity_y;
    /* 0x30 */ u8 pad_30[8];
    /* 0x38 */ void *animation_data;
    /* 0x3C */ u8 flip;
    /* 0x3D */ u8 animation_set;
    /* 0x3E */ u8 animation_frame;
    /* 0x3F */ u8 unk_3F;
    /* 0x40 */ u8 animation_flags;
    /* 0x41 */ u8 animation_hold;
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
    /* 0x44 */ u8 pad_44[8];
    /* 0x4C */ u8 *hitbox;
    /* 0x50 */ u8 pad_50[8];
    /* 0x58 */ u16 homing_age;
    /* 0x5A */ u8 pad_5A[6];
    /* 0x60 */ u16 heading;
    /* 0x62 */ u16 beam_length;
    /* 0x64 */ u8 pad_64[4];
} TtEnemyShot; /* size 0x68; object pool step in TT/enemy_shots_and_sound. T-9760 */
#endif

#ifndef OVL_TT_T9760_FUNC_80148460
#define OVL_TT_T9760_FUNC_80148460
#include "psx_types.h"
typedef struct TtText {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 limited;
    /* 0x02 */ u8 palette;
    /* 0x03 */ u8 rgb[3];
    /* 0x06 */ u16 flags;
    /* 0x08 */ u8 pad_08[2];
    /* 0x0A */ s16 x;
    /* 0x0C */ s16 y;
    /* 0x0E */ u16 text;
    /* 0x10 */ u16 layer;
    /* 0x12 */ u16 count;
} TtText; /* size 0x14; draw pool stride */
/* Sized sprite packet (code 0x65) written by tt_bombs_draw. T-11000. */
typedef struct TtSpritePacket {
    /* 0x00 */ TtOtTag tag;
    /* 0x04 */ u8 r;
    /* 0x05 */ u8 g;
    /* 0x06 */ u8 b;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
    /* 0x0C */ u8 u;
    /* 0x0D */ u8 v;
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 w;
    /* 0x12 */ s16 h;
} TtSpritePacket;
/* Timeline stream at tt_stage_script: current frame (high half) and the next six-byte command. T-11000. */
typedef struct TtStageScript {
    /* 0x00 */ u16 progress_frac;
    /* 0x02 */ u16 progress;
    /* 0x04 */ u8 *air_script_cursor;
} TtStageScript;
/* Textured rectangle packet written by tt_text_draw. T-11000. */
typedef struct TtRectPacket {
    /* 0x00 */ TtOtTag tag;
    /* 0x04 */ u8 r;
    /* 0x05 */ u8 g;
    /* 0x06 */ u8 b;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
    /* 0x0C */ u8 u;
    /* 0x0D */ u8 v;
    /* 0x0E */ u16 clut;
} TtRectPacket;
typedef struct TtDrawBuffer {
    /* 0x0000 */ u32 unk_0000;
    /* 0x0004 */ DR_MODE *cursor;
    /* 0x0008 */ u8 pad_0008[0x8000];
    /* 0x8008 */ TtOtTag ordering[44];
} TtDrawBuffer; /* size 0x80B8; render buffer stride */
typedef struct TtVideoView {
    /* 0x00 */ u8 pad_00[8];
    /* 0x08 */ s32 index;
} TtVideoView; /* accessed prefix size 0x0C */
void tt_texts_draw(void);

#endif


#ifndef OVL_TT_W9_2_COLLISION_H
#define OVL_TT_W9_2_COLLISION_H
/* tt_box_overlap reads signed x/y offsets and byte half extents. T-9720. */
typedef struct TtHitbox {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 half_w;
    /* 0x05 */ u8 half_h;
} TtHitbox; /* size 0x06 */
extern TtHitbox tt_hitboxes[];
extern TtHitbox tt_boss1_hitbox_rope[];
#endif

/* Halfword masks indexed by the byte phase at work+0x21, T-9720. */
extern u16 tt_pause_code_masks[];
/* Halfword button masks indexed by the phase at work+0x21 (tt_title_code_check). T-11800 */
extern u16 tt_title_code_masks[];

#ifndef OVL_TT_T9720_COORDINATES
#define OVL_TT_T9720_COORDINATES
/* Signed halfword coordinates consumed by tt_boss1_side_parts_init. */
extern s16 tt_boss1_core_part_y[];
#endif


#ifndef OVL_TT_W9_RENDER_TYPES
#define OVL_TT_W9_RENDER_TYPES
typedef union TtFixed16 {
    /* 0x00 */ s32 w;
    /* 0x00 */ struct { s16 lo; s16 hi; } h;
} TtFixed16; /* size 0x04 */
typedef union TtHalfwordBytes {
    /* 0x00 */ u16 h;
    /* 0x00 */ struct { u8 lo; u8 hi; } b;
} TtHalfwordBytes; /* size 0x02; actor +0x40 has both lhu and lbu accesses. T-9710. */
typedef struct TtObjectView {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 pad_01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u16 state;
    /* 0x06 */ s16 phase;
    /* 0x08 */ s16 step;
    /* 0x0A */ s16 step2;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 tick0;
    /* 0x0E */ u8 tick1;
    /* 0x0F */ u8 tick2;
    /* 0x10 */ u16 layer;
    /* 0x12 */ u8 pad_12[2];
    /* 0x14 */ s16 x;
    /* 0x16 */ s16 y;
    /* 0x18 */ u8 pad_18[8];
    /* 0x20 */ TtFixed16 position_x;
    /* 0x24 */ TtFixed16 position_y;
    /* 0x28 */ u8 pad_28[0x10];
    /* 0x38 */ u8 unk_38;
    /* 0x39 */ u8 unk_39;
    /* 0x3A */ u8 unk_3A;
    /* 0x3B */ u8 unk_3B;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ TtHalfwordBytes unk_40;
    /* 0x42 */ u16 unk_42;
    /* 0x44 */ u16 unk_44;
    /* 0x46 */ u8 unk_46;
    /* 0x47 */ u8 unk_47;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ u8 unk_4C;
    /* 0x4D */ u8 unk_4D;
    /* 0x4E */ u8 unk_4E;
    /* 0x4F */ u8 unk_4F;
    /* 0x50 */ u8 unk_50;
    /* 0x51 */ u8 unk_51;
    /* 0x52 */ u8 pad_52[6];
    /* 0x58 */ u16 unk_58;
    /* 0x5A */ u16 unk_5A;
    /* 0x5C */ u8 unk_5C;
    /* 0x5D */ u8 unk_5D;
    /* 0x5E */ u8 pad_5E[0x10];
    /* 0x6E */ s16 unk_6E;
    /* 0x70 */ u8 pad_70[8];
} TtObjectView; /* size 0x78; allocator walks 64 slots in 0x1E00 bytes. T-9710. */

#endif

#ifndef W9_TT_tt_air_enemy_kind40_move
#define W9_TT_tt_air_enemy_kind40_move
void tt_air_enemy_kind40_move(u8 *arg0);
#endif

#ifndef W9_TT_tt_air_enemy_kind41_move
#define W9_TT_tt_air_enemy_kind41_move
void tt_air_enemy_kind41_move(u8 *arg0);
#endif

#ifndef W9_TT_tt_air_enemy_kind43_move
#define W9_TT_tt_air_enemy_kind43_move
void tt_air_enemy_kind43_move(u8 *arg0);
#endif

#ifndef W9_TT_tt_air_enemy_kind46_move
#define W9_TT_tt_air_enemy_kind46_move
void tt_air_enemy_kind46_move(u8 *arg0);
#endif

#ifndef W9_TT_tt_air_enemy_kind47_move
#define W9_TT_tt_air_enemy_kind47_move
void tt_air_enemy_kind47_move(void *arg0);
#endif

#ifndef W9_TT_tt_air_enemy_kind48_move
#define W9_TT_tt_air_enemy_kind48_move
void tt_air_enemy_kind48_move(u8 *arg0);
#endif

#ifndef W9_TT_tt_air_enemy_move
#define W9_TT_tt_air_enemy_move
void tt_air_enemy_move(u8 *arg0);
#endif

#ifndef W9_TT_tt_air_enemy_explode_init
#define W9_TT_tt_air_enemy_explode_init
void tt_air_enemy_explode_init(u8 *arg0);
#endif

#ifndef OVL_TT_T9940_ACTOR_TAIL
#define OVL_TT_T9940_ACTOR_TAIL
typedef struct TtFieldEnemyTail {
    /* 0x00 */ u16 shot_timer;
    /* 0x02 */ u8 shot_fired;
    /* 0x03 */ u8 pad_03[13];
} TtFieldEnemyTail; /* size 0x10; view of TtObjectView at +0x5E */
#endif

#ifndef TT_STATE96_DEFINED
#define TT_STATE96_DEFINED
/* Accessed prefix of tt_sys, T-9940. */
typedef struct TtSysView {
    /* 0x00 */ u8 pad_00[0x56];
    /* 0x56 */ u8 se_flags[0x40];
} TtSysView; /* size 0x96 */
#endif

#ifndef OVL_TT_GRID_T10010
#define OVL_TT_GRID_T10010
/* Grid records stepped by tt_ranking_grid_scroll. */
typedef struct TtTransformView {
    /* 0x00 */ u8 pad00[4];
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ u8 pad08[8];
} TtTransformView; /* size 0x10 */
typedef struct TtSpriteView {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ s16 x;
    /* 0x16 */ s16 y;
    /* 0x18 */ u8 pad18[0x4C];
} TtSpriteView; /* size 0x64 */
/* The grid scroll value is unsigned here (lhu, including its increment). */
typedef struct TtCoreGridView {
    /* 0x00 */ u8 pad00[0x2E];
    /* 0x2E */ u16 scroll_offset;
} TtCoreGridView; /* accessed prefix size 0x30 */
#endif

#ifndef OVL_TT_SCENE_DRAW_T10010
#define OVL_TT_SCENE_DRAW_T10010
/* Scene coordinates and draw selectors used by the paired drawing helpers. */
typedef struct TtBossDrawView {
    /* 0x000 */ u8 pad00[0x14];
    /* 0x014 */ s16 x;
    /* 0x016 */ s16 y;
    /* 0x018 */ u8 pad18[0x24];
    /* 0x03C */ u8 unk_3C;
    /* 0x03D */ u8 pad3D;
    /* 0x03E */ u8 unk_3E;
    /* 0x03F */ u8 pad3F[0x25];
    /* 0x064 */ u8 unk_64;
    /* 0x065 */ u8 pad65;
    /* 0x066 */ u8 unk_66;
    /* 0x067 */ u8 pad67[0x1E1];
    /* 0x248 */ s16 xs[24];
    /* 0x278 */ s16 ys[24];
    /* 0x2A8 */ u8 pad2A8[0xFC];
    /* 0x3A4 */ u16 unk_3A4[4];
} TtBossDrawView; /* accessed prefix size 0x3AC */
extern TtHitbox tt_hitbox_shot;
extern TtHitbox tt_boss1_hitbox_core_shot; /* T-11000 */
#endif

extern u8 tt_demo_input_attract[];
extern u8 tt_demo_input_stage4[]; /* T-11000 */
extern u8 tt_demo_input_stage2[];
extern u8 tt_demo_input_stage1[];
extern u8 tt_demo_input_stage3[];

#ifndef OVL_TT_T11800_ANIM14
#define OVL_TT_T11800_ANIM14
/* Animation records of the tt_boss work, 0x14 bytes from work+0x38 (tt_boss1_side_parts_init). T-11800. */
typedef struct TtPartAnimation {
    /* 0x00 */ u8 *sets;
    /* 0x04 */ u8 flip;
    /* 0x05 */ u8 set;
    /* 0x06 */ u8 frame;
    /* 0x07 */ u8 pad_07;
    /* 0x08 */ u8 flags;
    /* 0x09 */ u8 hold;
    /* 0x0A */ u8 pad_0A;
    /* 0x0B */ u8 clut;
    /* 0x0C */ u8 pad_0C[4];
    /* 0x10 */ u8 transform_enable;
    /* 0x11 */ u8 pad_11[3];
} TtPartAnimation; /* size 0x14 */
#endif

#ifndef OVL_TT_T11810_SCENE_TABLES
#define OVL_TT_T11810_SCENE_TABLES
/* Scene tables and update handlers installed by tt_stage_script_init. T-11810 */
extern u8 tt_stage1_air_script[];
extern u8 tt_stage1_ground_script[];
extern u8 tt_stage2_air_script[];
extern u8 tt_stage2_ground_script[];
extern u8 tt_stage3_air_script[];
extern u8 tt_stage3_ground_script[];
extern u8 tt_stage4_air_script[];
extern u8 tt_stage4_ground_script[];
void tt_boss1_update(void);
void tt_boss2_update(s32 arg0);
void tt_boss3_update(s32 arg0);
void tt_boss4_update(void);
#endif

#ifndef OVL_TT_T11820_TABLES
#define OVL_TT_T11820_TABLES
/* x and image tables of the two menu record rows set up by tt_title_menu_init. T-11820 */
extern s16 tt_title_menu_x[];
extern u16 tt_title_menu_text[];
extern s16 tt_title_hint_x[];
extern u16 tt_title_hint_text[];
/* Record tables of tt_boss4_init (indices 0..10) and its three work pointers. T-11820 */
extern u8 tt_boss4_part_flip[];
extern u8 tt_boss4_part_frame[];
extern u8 tt_boss4_hitbox_slot_228[];
extern u8 tt_boss4_hitbox_slot_22c[];
extern u8 tt_boss4_hitbox_slot_230[];
#endif

#ifndef OVL_TT_T11840_80146348
#define OVL_TT_T11840_80146348
/* Six-byte rows at tt_sys + 0x30 (three rows shifted by tt_ranking_try_insert; tt_name_entry_update edits bytes 2..4). T-11840 */
typedef struct TtRankingEntry {
    /* 0x00 */ u16 time;
    /* 0x02 */ u8 name[4];
} TtRankingEntry; /* size 0x6 */
#endif

#ifndef OVL_TT_T11840_8014A2B4
#define OVL_TT_T11840_8014A2B4
/* Pointer table stored at tt_core + 0x18 by tt_state_title. T-11840 */
extern u8 *tt_title_tile_map[];
/* Defined after its caller tt_state_title in 80148DB0.c. T-11840 */
void tt_shots_clear(void);
#endif

#ifndef OVL_TT_T11840_801441BC
#define OVL_TT_T11840_801441BC
/* Per-step frame limits indexed by tt_player + 0x5A / + 0x5B (tt_player_update). T-11840 */
extern u8 tt_arm_swing_frames[];
/* Defined after its caller tt_player_update in 80142D40.c. T-11840 */
void tt_lives_icons_update(void);
#endif

#ifndef OVL_TT_T11840_80137C20
#define OVL_TT_T11840_80137C20
/* Per-record animation bytes and the update script installed by tt_boss1_init. T-11840 */
extern u8 tt_boss1_part_set[];
extern u8 tt_boss1_part_frame[];
extern u8 tt_boss1_hitbox_core[];
#endif

#ifndef OVL_TT_T11840_80141450
#define OVL_TT_T11840_80141450
/* Scale/rotate parameters read by tt_sprite_packet_transform (second argument). T-11840 */
typedef struct TtSpriteTransform {
    /* 0x00 */ s16 sx;
    /* 0x02 */ s16 sy;
    /* 0x04 */ s16 cx;
    /* 0x06 */ s16 cy;
    /* 0x08 */ s16 rx;
    /* 0x0A */ s16 ry;
    /* 0x0C */ s32 angle;
} TtSpriteTransform; /* size 0x10 */
#endif

#ifndef OVL_TT_T11840_8013D988
#define OVL_TT_T11840_8013D988
/* tt_core viewed up to the two fields tt_bg_tilemap_warp_draw reads (0x32: angle step, 0x34: divisor of the grid). T-11840 */
typedef struct TtCoreWarpView {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 warp_phase;
    /* 0x34 */ s16 warp_divisor;
} TtCoreWarpView; /* accessed prefix size 0x36 */
#endif

#ifndef OVL_TT_T11840_80136C64
#define OVL_TT_T11840_80136C64
/* Table stored at tt_core + 0x18 by tt_state_stage. T-11840 */
extern u8 tt_stage_tile_map[];
#endif

#ifndef OVL_TT_T11850_801478D0
#define OVL_TT_T11850_801478D0
/* Sprite animation data drawn by tt_part_sprite_draw. T-11850 */
typedef struct TtSpritePart {
    /* 0x00 */ u16 tile;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ u8 clut;
    /* 0x07 */ u8 flags;
} TtSpritePart; /* size 0x08 */
typedef struct TtSpriteSequence {
    /* 0x00 */ u16 count;
    /* 0x02 */ u16 start;
} TtSpriteSequence; /* size 0x04 */
typedef struct TtSpriteFrame {
    /* 0x00 */ u16 seq;
    /* 0x02 */ u8 pad2[2];
} TtSpriteFrame; /* size 0x04 */
typedef struct TtSpriteSet {
    /* 0x00 */ TtSpriteFrame *frames;
    /* 0x04 */ TtSpriteSequence *seqs;
    /* 0x08 */ TtSpritePart *parts;
    /* 0x0C */ u8 *attr;
    /* 0x10 */ u16 clut;
} TtSpriteSet; /* accessed prefix size 0x12 */
#endif

#ifndef OVL_TT_T11860_8013F040
#define OVL_TT_T11860_8013F040
/* Hit box passed to tt_box_overlap by tt_boss4_collision (12 bytes of data; first record used). T-11860 */
extern TtHitbox tt_boss4_hitbox_body;
#endif

#ifndef OVL_TT_T11860_G4
#define OVL_TT_T11860_G4
/* Settings word at system_data + 0x3C (D_800E7D4C), read by tt_config_load through a pointer to system_data.
 * b0 and b8/b9 are written back by tt_main from the TT state bytes 0x45, 0x43 and 0x49. T-11860 */
typedef struct TtConfigBits {
    u32 bomb_button : 8;
    u32 autofire : 1;
    u32 narrow_display : 1;
    u32 b10 : 15;
    u32 direct_mode : 1;
    u32 stage_select : 3;
    u32 use_pad2 : 1;
    u32 b30 : 2;
} TtConfigBits; /* size 0x04 */
#endif

#ifndef OVL_TT_T11860_801374C0
#define OVL_TT_T11860_801374C0
/* Second hit box of the tt_boss1_hitbox_rope table, labelled on its own; passed to tt_box_overlap by tt_boss1_side_parts_shot_damage. T-11860 */
extern TtHitbox tt_boss1_hitbox_side_part;
#endif

#ifndef OVL_TT_T11860_G9
#define OVL_TT_T11860_G9
/* Scene reset in 80148DB0.c, called by tt_title_menu_update before its definition. T-11860 */
void tt_title_logo_hide(void);
#endif

#ifndef OVL_TT_T11860_G10
#define OVL_TT_T11860_G10
/* s16 pairs passed to tt_boss_part_animation_step for the 0x14-byte records 2, 3 and 8..22 set up by tt_boss1_rope_break; its first value is copied to byte 6 of records 2 and 3. T-11860 */
extern s16 tt_boss1_break_script[];
#endif

#ifndef OVL_TT_T11860_801478D0
#define OVL_TT_T11860_801478D0
/* Sprite animation state at object + 0x38 (and + 0x98), drawn by tt_part_sprite_draw. T-11860 */
typedef struct TtSpriteAnimation {
    /* 0x00 */ TtSpriteSet **sets;
    /* 0x04 */ u8 flip; /* XORed into each part's flags; bit 0 flips y, bit 1 flips x */
    /* 0x05 */ u8 set;
    /* 0x06 */ u8 frame;
    /* 0x07 */ u8 reserved[3];
    /* 0x0A */ u8 clut_override;
    /* 0x0B */ u8 clut;
    /* 0x0C */ s16 x;
    /* 0x0E */ s16 y;
    /* 0x10 */ u8 transform_enable;
} TtSpriteAnimation; /* accessed prefix size 0x11 */
#endif

#ifndef OVL_TT_T24086_8014D260
#define OVL_TT_T24086_8014D260
/* PsyQ LIBGTE ratan_tbl (1025 s16 entries) indexed by func_8014D260 (ratan2). T-24086 */
extern s16 D_80159B20[];
#endif

