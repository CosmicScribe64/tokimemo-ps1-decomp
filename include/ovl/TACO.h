#ifndef OVL_TACO_H
#define OVL_TACO_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_func_8009B430 /* implicit declaration: called with 3 and with 4 arguments (main_api.h: 4) */

#include "common.h"
#include "psx_types.h"
#include "game.h"

void shooter_animate_enemy(s32 arg0);
extern s16 shooter_enemy_keys_a[], shooter_enemy_keys_b[], shooter_enemy_keys_c[], shooter_enemy_keys_d[], shooter_enemy_keys_e[];

/* state handlers of the loading screen driver shooter_stage_load (T-16562) */
s32 shooter_load_step_boot(void);
void shooter_load_step_sound_banks(void);
void shooter_load_step_textures(void);
s32 shooter_load_step_models(void);
s32 shooter_load_step_finish(void);

void shooter_draw_reticle(void);
void shooter_draw_bomb_stock(void);
void shooter_draw_bomb_panel(void);
void shooter_draw_shield_gauge(void);

extern s32 shooter_script_timer;
extern s32 shooter_script_phase;
extern s32 shooter_script_sub_timer;
extern s32 shooter_script_counter;
extern s32 shooter_circler_pairing;
extern s32 shooter_settings_cursor;
extern s32 shooter_settings_changing;
extern s32 shooter_settings_old_value;
extern s32 shooter_settings_animation_frame;
extern s32 shooter_name_entry_position;
extern s32 shooter_name_ring_index;
extern s32 shooter_name_ring_offset;
extern s32 shooter_name_ring_speed;
extern s32 shooter_name_entry_intro;

/* TACO tables; field names after the first user, no semantics known. */
typedef struct ShooterPosition {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 w;
} ShooterPosition; /* size 0x08 */

/* Object handle (T-16560): GsDOBJ2 layout; unk0 attribute (0x80000000 hides it), unk4[0] the
 * ShooterCoordinate coordinate object, unk4[1] the model, unk4[2] the id. func_8009B430 (GsLinkObject4) fills it. */
typedef struct ShooterModelHandle {
    /* 0x00 */ s32 attribute;
    /* 0x04 */ s32 links[3];
} ShooterModelHandle; /* size 0x10 */

/*
 * Coordinate object (T-16560). Same layout as libgs GsCOORDINATE2: a flag word (0 marks the local
 * matrix as changed), the local matrix (rotation unk4, translation unk18..unk20), the world matrix
 * the library computes, and the param/super/sub links. func_80099E30 (GsInitCoordinate2) takes
 * a pointer to one. The translation stays three scalars because matched code reads them as such.
 */
typedef struct ShooterCoordinate {
    /* 0x00 */ s32 flag;
    /* 0x04 */ s16 rotation[3][3];
    /* 0x16 */ s16 alignment_pad;
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ s32 z;
    /* 0x24 */ MATRIX world;
    /* 0x44 */ s32 param;
    /* 0x48 */ struct ShooterCoordinate *parent;
    /* 0x4C */ struct ShooterCoordinate *child;
} ShooterCoordinate; /* size 0x50 */

extern ShooterPosition D_80128880[];
extern ShooterModelHandle D_80127080[];
extern ShooterCoordinate D_80127480[];
/* coordinate objects 1-3 of D_80127480, named separately because shooter_boss2_update_part compares against their addresses (T-19019) */
extern ShooterCoordinate D_801274D0;
extern ShooterCoordinate D_80127520;
extern ShooterCoordinate D_80127570;

/*
 * Actor record, 79 of them at shooter_actors (loops stop at byte offset 0x29F8). T-16560: unk0[1] holds
 * flag bits (0x80, 0x40); +0x04 is the actor's object handle and +0x14 its coordinate object
 * (shooter_game_start_step_init passes &actor->unk14 to func_80099E30). unk74..unk7A and unk7C..unk82 are copied
 * as two eight-byte ShooterPosition values (lwl/lwr pairs) into by-value ShooterPosition arguments.
 */
typedef struct ShooterActor {
    /* 0x00 */ u8 flag_bytes[2];
    /* 0x02 */ u8 mode;
    /* 0x03 */ u8 phase;
    /* 0x04 */ ShooterModelHandle model;
    /* 0x14 */ ShooterCoordinate coordinate;
    /* 0x64 */ s16 rotation_x;
    /* 0x66 */ s16 rotation_y;
    /* 0x68 */ s16 rotation_z;
    /* 0x6A */ s16 param;
    /* 0x6C */ s16 scale_x;
    /* 0x6E */ s16 scale_y;
    /* 0x70 */ s16 scale_z;
    /* 0x72 */ s16 weapon;
    /* 0x74 */ s16 x;
    /* 0x76 */ s16 y;
    /* 0x78 */ s16 z;
    /* 0x7A */ s16 counter;
    /* 0x7C */ s16 argument_x;
    /* 0x7E */ s16 argument_y;
    /* 0x80 */ s16 argument_z;
    /* 0x82 */ s16 auxiliary;
    /* 0x84 */ u8 stats[4];
} ShooterActor; /* size 0x88 */

extern ShooterActor *shooter_actors;

void shooter_update_secondary_shots_of(s32 arg0);
s32 shooter_find_target(s32 *arg0, ShooterPosition arg1);
void shooter_update_primary_shots_of(s32 arg0);
void shooter_reset_enemies(void);
void shooter_spawn_homing_mines(s32 arg0);
s32 shooter_spawn_stage1_mid_boss(void);
void shooter_spawn_debris(void);
void shooter_spawn_swoop_wave(void);
void shooter_spawn_spiral_pair(void);
void shooter_play_help_voice(s32 arg0);
extern ShooterModelHandle D_80127090;
extern ShooterModelHandle D_801270A0;
extern s32 shooter_background_flags;
extern u8 shooter_load_request;
extern u8 shooter_sound_bank_wait_state;

void shooter_settings_draw_changing(void);
void shooter_settings_input(void);
s32 shooter_settings_draw(void);
s32 shooter_title_reveal_step(void);
void shooter_setup_text_layer(void);
s32 shooter_boss3_update_part(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, s32 arg5);
s32 shooter_boss3_collapse_motion(void);
void shooter_init_stars(void);
void shooter_update_stars(s32 arg0, s32 arg1);
void shooter_draw_star(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 shooter_demo_step_init();
void shooter_demo_step_run();
void shooter_boss3_descend_motion(void);
void shooter_boss3_fire_pattern(void);
s32 shooter_play_se();
void shooter_clear_beam_flags(s32 arg0);
s32 shooter_title_animate();
void shooter_title_step_init(void);
void shooter_draw_title_stars();
void shooter_title_step_wait(void);
void shooter_title_step_reveal(void);
void shooter_title_step_hold(void);
void shooter_title_step_advance(void);
void shooter_title_step_finish(void);
/* shooter_boss2_step_entrance: a 12-byte id table and three tables passed by address */
extern u8 shooter_boss2_part_objects[];
extern u8 shooter_boss2_part_radius[];
extern u8 shooter_boss2_part_hit_points[];
extern u8 shooter_boss2_part_offsets[];
extern u8 shooter_boss3_part_objects_1;
extern u8 shooter_boss3_part_radius[];
extern u8 shooter_boss3_part_hit_points[];
extern u8 shooter_boss3_part_offsets[];
extern s32 shooter_score;
extern s32 shooter_high_score_3;

void shooter_draw_ranking(s32 arg0, s32 arg1);
void shooter_update_stage1_script(void);
void shooter_update_stage2_script(void);
void shooter_update_stage3_script(void);
void shooter_remove_enemy();
void shooter_draw_glyph_ring(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void shooter_draw_entered_name(s32 arg0);
void shooter_reset_ship_state(void);
void shooter_clear_player_shots(void);
void shooter_clear_enemy_bullets(void);
s32 shooter_bomb_damage_all(void);
s32 shooter_bomb_damage_by_distance(void);
s32 shooter_update_pulse_bomb(void);
s32 shooter_try_start_bomb(s32 arg0);
s16 shooter_consume_bomb();
s32 shooter_update_blast_shots(void);
extern s32 shooter_scores_zoom;
extern u8 shooter_sound_bank_poll_result;

void shooter_set_white_lights(void);
void shooter_draw_bomb_icon(s32 arg0, s32 arg1, s32 arg2);
s32 shooter_scroll_background(void);
void shooter_process_load_request(void);
void shooter_tick_se_cooldowns(void);
void shooter_draw_floor_grid(void);
void shooter_draw_horizon_bands(void);
void shooter_nop_background_layer(void);
void shooter_update_camera(void);
extern u8 shooter_stages_cleared;
extern s16 shooter_time_left;

void shooter_draw_sky_gradient(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5, s32 arg6);
void shooter_settings_animation_auto_shot(void);
void shooter_settings_animation_roll(void);
void shooter_settings_animation_shot(void);
void shooter_settings_animation_bomb(void);
void shooter_boss3_sway_motion(s32 arg0);

void shooter_move_model(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void shooter_boss3_pose_idle(s32 arg0);
void shooter_boss3_set_part_visibility(s32 arg0, s32 arg1);
void shooter_boss3_update_sway(u32 arg0);
void shooter_fade_white(s32 arg0);
void shooter_boss2_step_entrance(void);
void shooter_boss2_step_defeat(void);
void shooter_boss3_reset_parts(void);
void shooter_boss1_run_attack_mode(void);
s32 shooter_boss_damage_percent(s32 arg0, void *arg1);
void shooter_boss1_morph_to_form2(s32 arg0);
void shooter_boss1_morph_to_form1(s32 arg0);
void shooter_boss1_form2_group_e_motion(s32 arg0);
void shooter_boss1_form2_group_f_motion(s32 arg0);
void shooter_boss1_pick_attack_mode(void);
void shooter_init_object_hierarchy(s32 arg0);
#ifndef TACO_OVERRIDE_shooter_link_model_objects /* 80146D60.c defines it with a pointer parameter (T-19015) */
void shooter_link_model_objects(s32 arg0);
#endif
void shooter_apply_keyframe(s32 arg0, s16 *arg1, s32 arg2);
extern s32 shooter_title_pose_tables[2]; /* two model slots, T-19011 */
extern s16 *shooter_opening_pose_tables[2]; /* model data pointers for shooter_apply_keyframe, T-19002 */
extern s16 shooter_title_frame;

void shooter_init_scrolling_stars(u8 *arg0);
void shooter_set_dark_lights(void);
void shooter_select_sky_palette(s32 arg0);
void shooter_boss1_update_group_e(s32 arg0);
void shooter_boss1_rest_group_e(void);

void shooter_boss1_update_group_f(s32 arg0);
void shooter_boss1_rest_group_f(void);

/* T-1321 */
void shooter_boss1_scatter_motion(s32 arg0);
typedef struct ShooterParticle {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 w;
    /* 0x08 */ s16 velocity_x;
    /* 0x0A */ s16 velocity_y;
    /* 0x0C */ s16 velocity_z;
    /* 0x0E */ s16 unused_word;
    /* 0x10 */ u8 unused_byte;
    /* 0x11 */ u8 argument;
    /* 0x12 */ u8 age;
    /* 0x13 */ u8 flags;
} ShooterParticle; /* size 0x14; same record as ShooterParticleView (two ShooterPosition, four bytes), T-16560 */
void shooter_draw_secondary_shot(ShooterParticle *arg0, s32 arg1);
void shooter_boss2_update_arm_attack_a(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4);
void shooter_boss2_update_arm_attack_c(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3);
void shooter_boss2_update_all_arms_attack(s32 arg0);
void shooter_boss2_update_arm_attack_b(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 shooter_star_brightness;
extern s32 shooter_pulse_bomb_active;
extern s32 shooter_pulse_bomb_timer;
extern s32 shooter_stage1_mid_boss_slot;
extern u16 *D_801FAA84;
extern s32 shooter_result_values[5];
extern s32 shooter_count_target;
extern s32 shooter_count_current;
void shooter_draw_circle_flash(s32 arg0, s32 arg1, s32 arg2);
void shooter_draw_flash_overlay(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
extern ShooterParticle shooter_player_primary_shots[];
/* result screen (T-19006): counter and bonus drawers of 8013B990.c */
s32 shooter_hit_ships_with_bullets(void);
s32 shooter_hit_ships_with_bodies(void);
s32 shooter_collect_items(void);
s32 shooter_hit_enemies_with_shots(ShooterParticle *arg0, s32 arg1, s32 arg2);
extern ShooterParticle shooter_wingman_primary_shots[];
/* shooter_draw_item: per-kind frame count, CLUT row, U and V tables (T-25110) */
extern u8 shooter_item_frame_count[8], shooter_item_clut_row[8], shooter_item_u[8], shooter_item_v[8];
extern u16 shooter_vulcan_frame_u;
extern u16 shooter_wide_shot_frame_u;
void shooter_draw_vulcan_shot(ShooterParticle *arg0, s32 arg1);
void shooter_draw_wide_shot(ShooterParticle *arg0, s32 arg1);
void shooter_draw_beam(ShooterParticle *arg0, s32 arg1);
extern u8 shooter_boss1_part_hit_points[];
void shooter_fire_from_joint(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6);
extern s32 shooter_scores_fade;
void shooter_release_actor_model();
extern u8 shooter_coordinate_owner[];
void shooter_set_enemy_visibility();

void shooter_load_texture_page();

extern u8 shooter_se_cooldown;

extern u8 shooter_se_cooldown_1;

extern u8 shooter_se_cooldown_2[];

extern s32 shooter_model_polygon_ranges[];

extern s16 *shooter_boss1_group_c_poses[];
extern s16 *shooter_boss2_arm_poses[];
void shooter_boss1_rest_group_c(void);
void shooter_boss1_update_group_c(s32 arg0);
extern u8 shooter_boss3_part_objects[];
void shooter_boss3_defeat_motion();
void shooter_apply_keyframe_tween(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4);
s32 shooter_boss3_pose_group_attack_a(s32 arg0);
void shooter_boss1_pose_group_c(s32 arg0);
void shooter_attach_boss_part(s32 arg0, s32 arg1, s32 arg2, void *arg3, void *arg4, s32 arg5);
void shooter_boss3_rest_group_attack(s32 arg0, s32 arg1);
extern s16 *shooter_boss1_group_e_pose_a;
extern s16 *shooter_boss1_group_e_poses_b[];
extern s16 *shooter_boss1_group_e_pose_c;
extern s16 *shooter_boss1_group_e_poses_d[];
extern s16 *shooter_boss1_group_d_poses_a[];
extern s16 *shooter_boss1_group_d_poses_b[];
extern s16 *shooter_boss1_group_d_poses_c[];
extern s16 *shooter_boss1_group_d_poses_d[];
void shooter_boss1_rest_group_d(void);
extern s32 shooter_boss1_morph_pose_pointers[];
extern u8 shooter_boss1_morph_pose_flags[];
s32 shooter_set_pose_from_tables(); /* Returns -1 for unaligned input buffers; T-9610. */
void shooter_boss1_update_ai(s32 arg0);
s32 shooter_boss1_pose_group_pair(s32 arg0, s32 arg1, s32 arg2);
void shooter_boss1_update_group_pair_short(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, u32 arg4);
void shooter_boss1_group_pair_motion(s32 arg0);
s32 shooter_boss1_form2_group_d_motion(s32 arg0);
s32 shooter_boss1_lunge_motion(s32 arg0);
void shooter_boss1_mode_lunge(void);
extern s16 *shooter_boss1_group_f_poses_a[];
extern s16 *shooter_boss1_group_f_poses_b[];
extern s16 *shooter_boss1_group_f_poses_c[];
extern s16 *shooter_boss1_group_f_pose_d;
extern s16 *shooter_boss1_group_f_poses_e[];
void shooter_boss1_pose_group_d_form1(s32 arg0);
void shooter_boss1_pose_group_d_form2(s32 arg0);
void shooter_boss3_update_damage_lights(void);
void shooter_boss3_update_ai(void);
void shooter_name_entry_step_init();
void shooter_draw_glyph();
void shooter_name_entry_step_intro();
void shooter_name_entry_step_input();
void shooter_name_entry_step_outro();
void shooter_name_entry_step_fade();
void shooter_name_entry_step_done();
void shooter_boss1_mode_sway(s32 arg0);
void shooter_boss1_mode_group_pair();
void shooter_boss1_mode_scatter();
void shooter_boss1_mode_form2_group_d();
void shooter_boss1_mode_form2_group_e();
void shooter_boss1_mode_rain();
void shooter_boss1_mode_form2_group_f();
void shooter_menu_step_init(void);
void shooter_game_start_step_wait(void);
void shooter_init_actor_model();
extern s8 shooter_bonus_popup_timer;
extern s32 shooter_bonus_popup_value;
void shooter_boss2_update_arms();
void shooter_boss2_update_idle_sway();

void shooter_boss2_pick_arm_attack(s32 arg0);

void shooter_set_white_lights();
void shooter_load_settings_and_scores();
void shooter_init_coordinate_objects();
void shooter_link_model_handles(s32 *arg0);
extern s32 shooter_help_voice_table[];
s32 shooter_help_step_init(void);
void shooter_help_step_page1(void);
void shooter_help_step_page2(void);
void shooter_help_step_exit(void);
s32 shooter_settings_step_init(void);
void shooter_settings_step_run(void);
void shooter_settings_step_exit(void);
s32 shooter_stage_settings(void);

void shooter_stage2_step_init(void);
void shooter_stage2_step_run(void);
void shooter_stage2_step_end(void);
s32 shooter_spawn_enemy(s32 a, s32 b, ShooterPosition c, ShooterPosition d, ShooterPosition e, s32 f, s32 g);
extern ShooterPosition shooter_zero_position_a;
extern ShooterPosition shooter_zero_position_b;

extern ShooterPosition shooter_circler_start_position;

void shooter_stage1_step_init(void);
void shooter_stage1_step_run(void);
void shooter_stage1_step_end(void);
extern s32 shooter_boss1_group_pair_poses[];

void shooter_boss1_rain_group_c(s32 arg0);
void shooter_boss1_update_group_pair(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4);
void shooter_clear_text_strip(void);
extern s32 shooter_high_score_1;
extern s32 shooter_high_score_2;
extern u8 shooter_high_score_name_1[];
extern u8 shooter_high_score_name_2[];
extern u8 shooter_high_score_name_3[];
extern s32 shooter_name_entry_rank;
extern u8 shooter_name_glyph_table[];
void shooter_name_entry_reset(void);
typedef struct ShooterStar {
    s32 angle;
    s32 radius;
} ShooterStar;
extern ShooterStar *shooter_stars;
extern u8 shooter_setting_auto_shot;
s32 shooter_draw_label();
extern s8 shooter_loader_flag;

void shooter_stage3_step_init(void);
void shooter_stage3_step_run(void);
void shooter_stage3_step_end(void);
extern ShooterParticle shooter_player_secondary_shots[];
extern ShooterParticle shooter_wingman_secondary_shots[];
extern ShooterParticle shooter_bomb_shots[];

void shooter_boss2_step_fight(void);
void shooter_boss2_update_damage_lights(void);
void shooter_boss2_update_ai();

void shooter_set_rotation_from_pose();
void shooter_boss2_rest_arm();
void shooter_read_model_polygon_ranges();
extern s32 shooter_boss2_core_pose;
extern s32 shooter_boss2_tail_poses[];
typedef struct ShooterPosePointers {
    s32 translation;
    s32 rotation;
} ShooterPosePointers;
extern ShooterPosePointers shooter_boss2_rest_pose_pointers[];
extern s32 shooter_triangle_voice_table[]; /* word pairs, read as a flat table by shooter_play_triangle_voice (T-16561) */
typedef struct ShooterPoseFlags {
    u8 translation_fixed;
    u8 rotation_fixed;
} ShooterPoseFlags;
extern ShooterPoseFlags shooter_boss2_rest_pose_flags[];
extern ShooterPosePointers shooter_boss2_part_pose_pointers[]; /* read at i - 1, T-16563 */
extern ShooterPoseFlags shooter_boss2_part_pose_flags[]; /* read at i - 1, T-16563 */
extern ShooterPosePointers shooter_boss1_rest_pose_pointers[]; /* read at i - 1, T-19013 */
extern ShooterPoseFlags shooter_boss1_rest_pose_flags[]; /* read at i - 1, T-19013 */
/* FAKE: not data of its own. shooter_boss2_tail_rest_poses + 4 * i is shooter_boss2_core_tail_poses + 4 * (i - 0x1C); IDO keeps the symbol as the
 * induction base, so only this base gives shooter_boss2_update_tail its zero-offset pointer walk (D_80160448). T-16563 */
extern s16 *shooter_boss2_tail_rest_poses[];
void shooter_boss2_step_init(void);

void shooter_boss2_reset_parts(void);

/* T-9520: globals used by the matched wave-8 functions. */
extern u8 shooter_setting_roll_button;
extern u8 shooter_setting_shot_button;
extern u8 shooter_setting_bomb_button;

void shooter_draw_hud_panels(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void shooter_spawn_guardian_ring(s32 arg0);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void shooter_upload_texture_banks(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void shooter_opening_step_init(void);
void shooter_opening_step_end(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void shooter_scores_step_init(void);
void shooter_scores_step_stars_in(void);
void shooter_scores_step_zoom_in(void);
void shooter_scores_step_show(void);
void shooter_scores_step_zoom_out(void);
s32 shooter_scores_step_stars_out(void);
void shooter_scores_step_exit(void);

#endif

typedef struct ShooterBackgroundScroll {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 scroll_z;
    /* 0x6 */ s16 tile_phase;
} ShooterBackgroundScroll; /* size 0x8 */
extern ShooterBackgroundScroll shooter_background_scroll;
typedef struct ShooterTextureCoordinates {
    /* 0x0 */ u8 b[4];
} ShooterTextureCoordinates; /* size 0x4 */
extern ShooterTextureCoordinates shooter_floor_tile_u;
extern ShooterTextureCoordinates shooter_floor_tile_v;
void shooter_result_step_next(void);
void shooter_spawn_turret_a(s16 arg0, s16 arg1, s16 arg2);
void shooter_spawn_turret_b(s16 arg0, s16 arg1, s16 arg2);
extern s8 shooter_menu_cursor;
void shooter_boss1_step_init(void);
void shooter_boss1_step_entrance(void);
s32 shooter_boss1_step_defeat();
void shooter_boss1_set_lights(void);
void shooter_boss1_update_part(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4);
void shooter_boss1_entrance_script();
s32 shooter_boss1_pose_group_f(s32 arg0);
void shooter_boss1_rest_group_pair(s32 arg0, s32 arg1);
void shooter_boss1_reset_parts();
extern u8 shooter_score_text_1[];
extern u8 shooter_score_text_2[];
extern u8 shooter_score_text_3[];
void shooter_format_score(u8 *arg0, s32 arg1);
typedef struct ShooterScoreTemplate {
    /* 0x00 */ u8 characters[11];
} ShooterScoreTemplate; /* size 0xB: "0123456789" */
extern ShooterScoreTemplate shooter_score_text_template;
void shooter_draw_ranking_text(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void shooter_boss2_pose_arm_a(s32 arg0, s32 arg1, s32 arg2);
void shooter_boss2_pose_arm_b();
extern u8 shooter_se_cooldown_range[];
void shooter_boss3_step_init(void);
void shooter_boss3_step_descend(void);
void shooter_boss3_step_fight(void);
void shooter_boss3_step_defeat(void);
void shooter_boss3_step_collapse(void);
extern s32 shooter_boss3_idle_pose_pointers_joint_7[];
extern u8 shooter_boss3_idle_pose_flags_joint_7[];
extern s32 shooter_boss3_idle_pose_pointers_joint_27;
extern s32 shooter_boss3_idle_pose_pointers_joint_27_rotation;
extern u8 shooter_boss3_idle_pose_flags_joint_27;
extern u8 shooter_boss3_idle_pose_flags_joint_27_rotation;
extern s32 D_8015FEF8[];
extern u8 D_8015FFE2[];
extern s32 shooter_boss3_idle_pose_pointers[];
extern s32 shooter_boss3_idle_pose_pointers_joint_13[];
extern s32 shooter_boss3_idle_pose_pointers_joint_28[];
extern u8 shooter_boss3_idle_pose_flags[];
extern u8 shooter_boss3_idle_pose_flags_joint_13[];
extern u8 shooter_boss3_idle_pose_flags_joint_28[];
extern s16 *shooter_boss3_group_attack_b_poses[];
extern s16 *shooter_boss3_rest_rotation_references_13;
void shooter_boss3_pose_group_attack_c(s32 arg0);
void shooter_boss1_pose_group_e(s32 arg0);
void shooter_boss3_pose_group_attack_b(s32 arg0);
extern s32 shooter_boss3_rest_pose_pointers[];
extern u8 shooter_boss3_rest_pose_flags[];
void shooter_boss3_set_lights(void);
extern s32 shooter_boss3_idle_pose_pointers_joint_26;
extern s32 shooter_boss3_idle_pose_pointers_joint_26_rotation;
extern u8 shooter_boss3_idle_pose_flags_joint_26;
extern u8 shooter_boss3_idle_pose_flags_joint_26_rotation;
void shooter_boss3_update_damage_stage(void);
void shooter_boss3_update_group_attack_a(void);
void shooter_boss3_update_group_attack_b(void);
void shooter_boss3_update_group_attack_c(void);
void shooter_boss1_sway_motion(s32 arg0, s32 arg1);
void shooter_boss2_sway_motion_alt(s32 arg0);
void shooter_behavior_leave_forward(s32 arg0);
void shooter_behavior_leave_back(s32 arg0);

#ifndef OVL_TACO_MATCHED_VIEWS_T9520
#define OVL_TACO_MATCHED_VIEWS_T9520
/* Coordinate object: MATRIX payload at +4, total stride 0x50. T-9610. */
typedef struct ShooterCoordinateView {
    /* 0x00 */ s32 flag;
    /* 0x04 */ MATRIX local;
    /* 0x24 */ u8 pad24[0x2C];
} ShooterCoordinateView; /* size 0x50, full word view of ShooterCoordinate */
void shooter_game_end_step_init(void);
void shooter_game_end_step_wait(void);
void shooter_game_end_step_leave(void);
void shooter_snapshot_screen(void);
#endif

/* Position and velocity copies/loads use two existing eight-byte ShooterPosition records. */
typedef struct ShooterParticleView {
    /* 0x00 */ ShooterPosition position;
    /* 0x08 */ ShooterPosition velocity;
    /* 0x10 */ u8 unused_byte;
    /* 0x11 */ u8 image_index;
    /* 0x12 */ u8 age;
    /* 0x13 */ u8 flags;
} ShooterParticleView; /* size 0x14 */
extern ShooterParticleView shooter_explosions[];

extern ShooterParticleView shooter_hit_sparks[];

#ifndef OVL_TACO_INITIALIZATION_T9780
#define OVL_TACO_INITIALIZATION_T9780
void shooter_game_start_step_init(void);
void shooter_spawn_formation_positive_x(void);
void shooter_spawn_formation_negative_x(void);
void shooter_spawn_formation_v(void);
void shooter_spawn_gate_pair(void);
void shooter_spawn_circling_pair(void);
void shooter_spawn_carrier(void);
void shooter_dismiss_circlers(void);
#endif

#ifndef OVL_TACO_DIRECTIONS_T9780
#define OVL_TACO_DIRECTIONS_T9780
typedef struct ShooterPlanarVelocity {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
} ShooterPlanarVelocity; /* size 0x08 */
typedef struct ShooterFormationVelocities {
    /* 0x00 */ ShooterPlanarVelocity vectors[5];
} ShooterFormationVelocities; /* size 0x28 */
typedef struct ShooterFormationSpins {
    /* 0x00 */ s32 spins[5];
} ShooterFormationSpins; /* size 0x14 */
extern ShooterFormationVelocities shooter_formation_velocities;
extern ShooterFormationSpins shooter_formation_spins;
#endif

#ifndef OVL_TACO_ACTOR_INIT_T10020
#define OVL_TACO_ACTOR_INIT_T10020
extern u8 shooter_boss1_part_objects[];
extern u8 shooter_boss1_part_radius[];
#endif

#ifndef OVL_TACO_MENU_RESOURCE_T10020
#define OVL_TACO_MENU_RESOURCE_T10020
extern s32 shooter_boss2_core_pose_pointers;
extern s32 shooter_boss2_core_pose_pointers_rotation;
extern u8 shooter_boss2_core_pose_flags;
extern u8 shooter_boss2_core_pose_flags_rotation;
void shooter_boss2_update_core(u32 arg0);
void shooter_boss2_pose_core(s32 arg0);
void shooter_boss2_pose_tail(s32 arg0, s32 arg1);
void shooter_boss2_pose_all_arms(s32 arg0, s32 arg1, s32 arg2);
void shooter_boss2_pose_arm_c(s32 arg0, s32 arg1, s32 arg2);
extern s16 *shooter_boss2_core_tail_poses[]; /* six pointers: [0] 0x23's model, [1..4] the walk of shooter_boss2_pose_tail; T-16563 */
#endif

#ifndef OVL_TACO_T10010_SELECTION
#define OVL_TACO_T10010_SELECTION
void shooter_menu_step_select(void);
#endif

#ifndef OVL_TACO_T10010_SCORE_TABLE
#define OVL_TACO_T10010_SCORE_TABLE
extern s32 shooter_hit_sound_played; /* T-19006 */
extern s32 shooter_score_by_class[]; /* Word score table indexed modulo 17. T-10010. */
/* Damage per particle kind (low nibble of ShooterParticle.flags), copied whole into a local by
 * shooter_hit_enemies_with_shots. T-25111. */
typedef struct ShooterShotDamage {
    u8 by_kind[9];
} ShooterShotDamage;
extern ShooterShotDamage shooter_shot_damage_by_kind;
/* Defined in 8013D1E0.c; the s16 parameters give the callers their argument narrowing. T-25111. */
s32 shooter_spawn_hit_spark(s16 x, s16 y, s16 z);

extern ShooterParticle shooter_enemy_bullets[16];
extern ShooterParticle shooter_items[16]; /* the table after shooter_enemy_bullets; also its end address (T-16560) */
s32 shooter_draw_enemy_bullet();
extern u8 shooter_boss1_part_offsets[];
void shooter_boss1_step_fight(void);
void shooter_boss1_update_damage_lights(void);
void shooter_draw_item_weapon();
void shooter_draw_item();
void shooter_exit_step_save_settings(void);
u8 shooter_exit_step_stop_sound(void);
void shooter_exit_step_write_memory_card(void);
void shooter_exit_step_reload_main_data_1(void);
void shooter_exit_step_reload_main_data_2(void);
void shooter_exit_step_reload_main_data_3(void);
u8 shooter_exit_step_load_day_data(void);
void shooter_exit_step_return(void);
void shooter_stage_exit(void);

extern u16 shooter_blast_shot_frame_u;
void shooter_draw_blast_shot(s32 arg0, s32 arg1);
/* Only body-backed dispatch/state/score/timer roles. Stage 7 remains unresolved. Positive legacy requests have no metadata replay. */
enum ShooterStage {
    SHOOTER_STAGE_LOAD = 0, /* dispatch case in shooter_entry */
    SHOOTER_STAGE_OPENING = 1, /* dispatch case in shooter_entry */
    SHOOTER_STAGE_TITLE = 2, /* dispatch case in shooter_entry */
    SHOOTER_STAGE_MENU = 3, /* dispatch case in shooter_entry */
    SHOOTER_STAGE_SETTINGS = 4, /* dispatch case in shooter_entry */
    SHOOTER_STAGE_SCORES = 5, /* ranking dispatch */
    SHOOTER_STAGE_DEMO = 6, /* demo dispatch */
    SHOOTER_STAGE_RESULT = 8, /* score tally dispatch */
    SHOOTER_STAGE_NAME_ENTRY = 9, /* name-entry dispatch */
    SHOOTER_STAGE_GAME_START = 10, /* setup dispatch */
    SHOOTER_STAGE_STAGE1 = 11, /* stage 1 dispatch */
    SHOOTER_STAGE_BOSS1 = 12, /* boss 1 dispatch */
    SHOOTER_STAGE_STAGE2 = 21, /* stage 2 dispatch */
    SHOOTER_STAGE_BOSS2 = 22, /* boss 2 dispatch */
    SHOOTER_STAGE_STAGE3 = 31, /* stage 3 dispatch */
    SHOOTER_STAGE_BOSS3 = 32, /* boss 3 dispatch */
    SHOOTER_STAGE_GAME_END = 45, /* defeat, time-out or clear banner dispatch */
    SHOOTER_STAGE_EXIT = 46 /* overlay exit dispatch */
};

/* Only body-backed dispatch/state/score/timer roles. Stage 7 remains unresolved. Positive legacy requests have no metadata replay. */
enum ShooterLoadRequest {
    SHOOTER_LOAD_GAME_START_BANKS = 0x10, /* load-state dispatch */
    SHOOTER_LOAD_SOUND_BANK_STAGE1 = 0x14, /* Calls audio_bank_load_start for this stage/result request; bank contents are not identified as music. */
    SHOOTER_LOAD_BOSS1 = 0x18, /* boss 1 asset request */
    SHOOTER_LOAD_STAGE2 = 0x20, /* stage 2 asset request */
    SHOOTER_LOAD_SOUND_BANK_STAGE2 = 0x24, /* Calls audio_bank_load_start for this stage/result request; bank contents are not identified as music. */
    SHOOTER_LOAD_BOSS2 = 0x28, /* boss 2 asset request */
    SHOOTER_LOAD_STAGE3 = 0x30, /* stage 3 asset request */
    SHOOTER_LOAD_SOUND_BANK_STAGE3 = 0x34, /* Calls audio_bank_load_start for this stage/result request; bank contents are not identified as music. */
    SHOOTER_LOAD_BOSS3 = 0x38, /* boss 3 asset request */
    SHOOTER_LOAD_SOUND_BANK_RESULT = 0x44 /* Calls audio_bank_load_start for this stage/result request; bank contents are not identified as music. */
};

/* Only body-backed dispatch/state/score/timer roles. Stage 7 remains unresolved. Positive legacy requests have no metadata replay. */
#define SHOOTER_SCORE_MAXIMUM_EXCLUSIVE 10000000 /* comparison threshold */
#define SHOOTER_SCORE_MAXIMUM 9999999 /* clamp assignment after threshold */

/* Only body-backed dispatch/state/score/timer roles. Stage 7 remains unresolved. Positive legacy requests have no metadata replay. */
#define SHOOTER_START_TIME_FRAMES 18000 /* initial timer value, 18000 frames */

/* One exact weapon caller, bank argument 1; source literal is preserved. Other callers remain unnamed; no audible identity claimed. */
#define SHOOTER_SE_FIRE_VULCAN 0x500 /* Raw second argument in this weapon firing path; first argument is exactly 1. */

/* One exact weapon caller, bank argument 1; source literal is preserved. Other callers remain unnamed; no audible identity claimed. */
#define SHOOTER_SE_FIRE_BEAM 0x501 /* Raw second argument in this weapon firing path; first argument is exactly 1. */

/* One exact weapon caller, bank argument 1; source literal is preserved. Other callers remain unnamed; no audible identity claimed. */
#define SHOOTER_SE_FIRE_WIDE 0x502 /* Raw second argument in this weapon firing path; first argument is exactly 1. */

/* One exact weapon caller, bank argument 1; source literal is preserved. Other callers remain unnamed; no audible identity claimed. */
#define SHOOTER_SE_FIRE_SECONDARY 0x503 /* Raw second argument in this weapon firing path; first argument is exactly 1. */

/* Every flags member access in this exact caller is base[i].flags with ShooterParticle *base. Wildcard is caller-bound; no actor flags are selected. Other particle consumers remain unnamed. */
#define SHOOTER_PARTICLE_ACTIVE 0x80 /* Active particle slot is occupied; constructor scans for a clear flag before writing its shot. */

/* Known item producer values at exact callee argument site; positive legacy request, no metadata replay. */
enum ShooterItemKind {
    SHOOTER_ITEM_WEAPON_WIDE = 0, /* item pickup sets primary weapon 0; fire paths identify 0 as wide shot */
    SHOOTER_ITEM_WEAPON_BEAM = 1, /* item pickup sets primary weapon 1; fire paths identify 1 as beam */
    SHOOTER_ITEM_SECONDARY_TOGGLE = 2, /* item pickup toggles the secondary-weapon field */
    SHOOTER_ITEM_BOMB_PULSE = 3, /* item pickup fills one bomb slot; bomb kind 3 starts pulse */
    SHOOTER_ITEM_BOMB_BLAST = 4, /* item pickup fills one bomb slot; bomb kind 4 starts blast */
    SHOOTER_ITEM_HEAL = 5, /* item pickup adds 16 to player HP, capped at 255 */
    SHOOTER_ITEM_BONUS = 6, /* drop mapping emits 6; collector default gives a random score bonus */
    SHOOTER_ITEM_WEAPON_VULCAN = 7 /* item pickup selects the vulcan fallback fire path */
};

/* Player slot zero secondary dispatch only; other actor auxiliary fields excluded by exact caller. */
enum ShooterSecondary {
    SHOOTER_SECONDARY_HOMING = 0, /* player actor auxiliary value 0 dispatches to homing fire */
    SHOOTER_SECONDARY_SPREAD = 1 /* player actor auxiliary value 1 dispatches to spread fire */
};

#endif

/* Font UV pairs (u, v), T-16562: digits 0-9, letters A-Z, then '.', '-', ' ' and a fallback glyph. */
extern u8 shooter_digit_uv[20];
extern u8 shooter_letter_uv[52];
extern u8 shooter_glyph_period_u, shooter_glyph_period_v, shooter_glyph_hyphen_u, shooter_glyph_hyphen_v, shooter_glyph_space_u, shooter_glyph_space_v, shooter_glyph_other_u, shooter_glyph_other_v;

extern u8 shooter_enemy_radius_5;
extern s32 shooter_boss3_rest_rotation_references[];
extern s32 shooter_boss3_rest_rotation_references_6;
extern u8 shooter_enemy_joint_count[]; /* trigger count per kind (T-10810) */
extern s32 *shooter_enemy_joint_poses[]; /* per kind: table of (s32, s32) pairs (T-10810) */
extern s32 shooter_enemy_model_base[]; /* per kind: base handle of the kind's objects (T-19026) */
extern s16 shooter_enemy_scale[]; /* per kind: initial value of actor unk6C..unk70 (T-19026) */
extern u8 shooter_enemy_radius[]; /* per kind: initial value of actor unk84[0] (T-19026) */
void shooter_set_pose_from_table();
void shooter_apply_initial_pose(s32 object_index, s16 *pose_record);
