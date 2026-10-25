#ifndef OVL_RPG_BAT_H
#define OVL_RPG_BAT_H

#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game/schedule_records.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "libgte.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/minigame.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/menu.h"
#include "main/message.h"
#include "main/pad.h"
#include "main/prim.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"

extern s32 rpg_enemy_attack_count;
extern s32 rpg_enemy_step;
extern s32 rpg_enemy_substep;
extern s32 rpg_enemy_timer_a;
extern s32 rpg_enemy_timer_b;

extern s32 rpg_target_x;
extern s32 rpg_target_y;
extern s32 D_8015EC6C;
extern s32 rpg_message_line_frames;
#ifndef RPG_BAT_OVERRIDE_rpg_player_action
extern s32 rpg_player_action;
#endif
extern s32 rpg_enemy_delay_extra;
extern s32 rpg_enemy_damage_scale;
extern s32 rpg_enemy_miss_weight;
extern s32 rpg_enemy_action;
extern s32 rpg_girl_action;
extern s32 rpg_over;
s32 rpg_rand(s32);
void rpg_apply_damage(s32, s32, s32, s32);
extern s32 rpg_enemy_slot;
extern s32 rpg_damage;
void rpg_player_step_next(void);
void rpg_player_substep_next(void);
void rpg_girl_step_next(void);
extern s32 rpg_girl_timer_b;
void rpg_girl_substep_next(void);
void rpg_finale_step_next(void);
void rpg_finale_substep_next(void);

void rpg_robot_palette_cycle(void);
void rpg_move_assist_nozomi_polite(void);
void rpg_move_assist_nozomi_rough(void);
s32 rpg_dialog_delinquent_taunt(void);
void rpg_enemy_action_end(void);

extern s32 rpg_voice_started[][4];
extern s32 rpg_player_hit_points;

extern u8 **rpg_message_lines;
extern s32 rpg_message_line_count;
void rpg_yoshio_punch(void);
void rpg_yoshio_kick(void);
void rpg_yoshio_potion(void);
void rpg_megumi_flinch(void);
void rpg_megumi_pause(void);
void rpg_megumi_shout(void);
extern s32 rpg_player_action_words[2]; /* defined in RPG_BAT/battle_lifecycle_and_channels.c (T-9010) */
/* Counters that functions of RPG_BAT/battle_lifecycle_and_channels.c reset together: one variable each, because the
 * original shares one `lui $at` between the stores (T-9010). Defined in RPG_BAT/battle_lifecycle_and_channels.c. */
typedef struct RpgBatWords3 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} RpgBatWords3; /* size 0x0C */
typedef struct RpgBatWords4 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} RpgBatWords4; /* size 0x10 */
extern RpgBatWords3 rpg_player_channel;
extern RpgBatWords4 rpg_girl_channel;
extern RpgBatWords4 rpg_finale_channel;
extern RpgBatWords3 rpg_message;
extern RpgBatWords3 rpg_menu;
void rpg_victory_delinquents(void);
/* T-24013: callbacks reached before their definitions. */
void rpg_loot_text_delinquents(void);
void rpg_bancho_asset_load(void);
void rpg_bancho_flash(void);
void rpg_victory_robot(void);
void rpg_victory_bancho(void);
extern s32 rpg_encounter;
extern s32 rpg_damage_slot1;
extern s32 rpg_damage_slot2;
extern s32 rpg_result_code;
extern s32 rpg_scratch;
s32 rpg_dialog_delinquent_defeat_taunt(void);
extern s32 rpg_girl_id;
s32 rpg_enemy_pick_delinquent();
s32 rpg_enemy_pick_bancho_2();
s32 rpg_enemy_pick_alien();
extern s32 rpg_damage_to_player_extra;
/* The prototypes below can be hidden by a file that was matched with the implicit int view the
 * original callers had: it defines RPG_BAT_OVERRIDE_<name> before its includes and declares
 * `s32 <name>();` itself (the definitions in 8013E4D0.c are void; T-16210). */
#ifndef RPG_BAT_OVERRIDE_rpg_actor_set_animation
void rpg_actor_set_animation(s32 a, s32 b, s32 c, s32 d);
#endif
#ifndef RPG_BAT_OVERRIDE_rpg_actor_set_animation_alt
void rpg_actor_set_animation_alt(s32 a, s32 b, s32 c, s32 d);
#endif
#ifndef RPG_BAT_OVERRIDE_rpg_actor_set_position
void rpg_actor_set_position(s32 a, s32 b, s32 c);
#endif

void rpg_enemy_wait_step(s32 frames);
void rpg_enemy_step_next(void);

#ifndef RPG_BAT_OVERRIDE_rpg_se_play_latched_6xx
void rpg_se_play_latched_6xx(s32 a, s32 b, s32 c);
#endif
void rpg_basketball_spark_burst(s32 a, s32 b);
s32 rpg_no_club_step_3(void);
/* Same override rule as the pair above (the definition in 8013E4D0.c is void; T-17101). */
#ifndef RPG_BAT_OVERRIDE_rpg_se_play_latched_5xx
void rpg_se_play_latched_5xx(s32 a, s32 b, s32 c);
#endif
s32 rpg_actor_is_idle(s32 i);
s32 rpg_nozomi_is_calm(void);

void rpg_wave_fade_step(void);
void rpg_wave_strips_a(void);
void rpg_wave_strips_b(void);
void rpg_wave_strip_draw(s32, s32, s32, s32, s32, s32);
void rpg_wave_quad_draw(s32 *xy, s32 *uv, s32 arg2, s32 arg3, s32 arg4); /* four (x, y) and four (u, v) words */
extern s32 rpg_bg_word_0;
extern s32 rpg_bg_word_1;
extern s32 rpg_bg_word_2;
extern s32 rpg_bg_word_3;
extern s32 rpg_ripple_phase_2;
extern s32 rpg_ripple_phase_2_step;
extern s32 rpg_ripple_period_2;
extern s32 rpg_ripple_width_2;
extern s32 rpg_bg_word_4;
extern s32 rpg_bg_word_5;
s32 rpg_art_step_7(void);
void rpg_art_draw_shape(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* defined in C in one object, called from another (T-0500) */
s32 rpg_rand2(s32 upper_bound_a, s32 upper_bound_b);

extern s16 rpg_popup_bounce_table[][12];
extern s32 rpg_popup_0;
extern s32 D_8015EE60;
extern s32 rpg_popup_1;
extern s32 rpg_popup_2;
extern s32 rpg_popup_3;
extern s32 D_8015EE64;
void rpg_popup_digit_set_animation(s32 a0, s32 a1, s32 a2);
void rpg_record_set_animation(s32 arg0, s32 arg1, s32 arg2);
void rpg_message_next_state(s32 channel);
extern s32 D_8015EE6C;
extern s32 D_8015EE70;
extern s32 D_8015EE78;
extern s32 D_8015EE7C;
extern s32 D_8015EE84;
extern s32 D_8015EE88;
/* T-5000: six words read and written as one object (the original keeps loads after the stores). */
typedef struct RpgMoveParams {
    /* 0x00 */ s32 param_0;
    /* 0x04 */ s32 param_1;
    /* 0x08 */ s32 param_2;
    /* 0x0C */ s32 param_3;
    /* 0x10 */ s32 param_4;
    /* 0x14 */ s32 param_5;
} RpgMoveParams; /* size 0x18 */
extern RpgMoveParams rpg_move_params;
void rpg_girl_run_set_rect(void);
extern s32 rpg_asset_palette_5;
extern s32 rpg_pause_state;
extern s32 rpg_pause_debounce;
extern s32 rpg_player_ready_timer;
extern s32 rpg_enemy_timer;
extern s32 rpg_label_enemy;
extern s32 rpg_girl_timer;
extern s32 rpg_label_guest;
s32 rpg_check_enemy_defeated(void);
void rpg_start_victory_sequence(void);
void rpg_background_wave(void);
void rpg_pause_update(void);
s32 rpg_stage_fight(void);
#ifndef RPG_BAT_OVERRIDE_rpg_enemy_update
void rpg_enemy_update(void);
#endif
void rpg_player_update(void);
void rpg_girl_update();
void rpg_hud_numbers_update(void);
void rpg_message_update(void);
void rpg_popup_bounce_0(void);
void rpg_popup_bounce_1(void);
void rpg_player_basic_damage(void);
void rpg_popup_bounce_2(void);
void rpg_popup_bounce_3(void);
/* Pad-sequence matcher state of rpg_code_entry_update, one aggregate. The original keeps the store of unk_00 in front
 * of the load of unk_04; four separate scalars let the build sink that store into the branch delay slot (T-17107). */
typedef struct RpgCodeEntry {
    /* 0x00 */ s32 pad_now; /* pad word read this frame (rpg_code_entry) */
    /* 0x04 */ s32 pad_last; /* pad word of the previous change (D_8015EBF0) */
    /* 0x08 */ s32 sequence_b_progress; /* position in the rpg_code_sequence_b sequence (D_8015EBF4) */
    /* 0x0C */ s32 sequence_a_progress; /* position in the rpg_code_sequence_a sequence (D_8015EBF8) */
} RpgCodeEntry; /* size 0x10 */
extern RpgCodeEntry rpg_code_entry;
void rpg_code_entry_update();
/* Battle counters; one struct because as1 then keeps the access order of the original (T-4030, rpg_enemy_pick_bancho_2). */
typedef struct RpgBattleState {
    /* 0x00 */ s32 enemy_weight_0;
    /* 0x04 */ s32 enemy_weight_1;
    /* 0x08 */ s32 enemy_weight_2;
    /* 0x0C */ s32 enemy_weight_3; /* T-17100: the words between are the scalars D_8015EC08..rpg_special_gauge */
    /* 0x10 */ s32 girl_weight_wait;
    /* 0x14 */ s32 girl_weight_act;
    /* 0x18 */ s32 result_code;
    /* 0x1C */ s32 saved_girl;
    /* 0x20 */ s32 boss_event;
    /* 0x24 */ s32 pause_state;
    /* 0x28 */ s32 pause_debounce;
    /* 0x2C */ s32 target_x;
    /* 0x30 */ s32 target_y;
    /* 0x34 */ s32 special_gauge;
    /* 0x38 */ s32 escalation_counter;
} RpgBattleState; /* size 0x3C */
extern RpgBattleState rpg_state;

extern s32 rpg_girl_timer_a;

extern s32 rpg_se_latch[];

extern s32 rpg_voice_wait_frames;

extern s32 rpg_dialog_step[2]; /* [1] is D_8015ED90; one base symbol keeps loads behind stores (T-4070) */

void rpg_move_assist_shiori();
void rpg_move_assist_mio();
void rpg_move_assist_yuina();
void rpg_move_assist_ayako();
void rpg_move_assist_saki();
void rpg_move_assist_yukari();
void rpg_move_assist_nozomi();
void rpg_move_assist_mira();
void rpg_move_assist_yuko();
void rpg_move_assist_megumi();
void rpg_move_assist_yumi();
void rpg_message_show();
void rpg_player_wait_substep();
void rpg_player_wait_msg_substep();
void rpg_drama_pick_lines();
void rpg_ayako_shot_pick(void);
extern u8 rpg_text_player_pointers;
extern s32 rpg_saved_girl;
void rpg_voice_play_once();
extern u8 rpg_text_girl_0[];
extern u8 rpg_text_girl_pointers[];
extern s32 rpg_motion;
extern s32 rpg_motion_dy;

extern s32 rpg_girl_hit_points;
void rpg_enemy_substep_next(void);
void rpg_enemy_wait_msg_substep_unused(s32 line_count);
void rpg_girl_action_end();
void rpg_girl_wait_step();
void rpg_voice_wait();
void rpg_voice_reset();

extern u8 rpg_text_player_0[];
extern s32 rpg_move_timer;
extern s32 rpg_player_timer_2;
/* A file that reads the first word as a scalar (rpg_girl_pick_action, T-17109) defines RPG_BAT_OVERRIDE_rpg_delinquent_charmed_0 and declares `extern s32 rpg_delinquent_charmed_0;` itself. */
#ifndef RPG_BAT_OVERRIDE_rpg_delinquent_charmed_0
extern s32 rpg_delinquent_charmed_0[3];
#endif
extern s32 rpg_asset_palette_4;
extern u8 rpg_text_player_1[];
extern u8 rpg_text_player_2[];
extern u8 rpg_text_enemy_1[];
extern u8 rpg_text_enemy_2[];
extern u8 rpg_text_enemy_3[];
extern u8 rpg_text_result_1[];
extern u8 rpg_text_result_2[];
extern u8 rpg_text_result_3[];
extern u8 rpg_text_result_4[];
extern u8 rpg_text_result_5[];
extern s32 rpg_player_hit_points_maximum;
extern s32 rpg_player_guard_heal;
extern u32 rpg_girl_substep;
void rpg_girl_wait_substep();
extern s32 rpg_enemy_hit_points;
extern s32 rpg_enemy_hit_points_maximum;
extern u32 rpg_girl_step;
void rpg_yukari_call_father(void);
extern s32 rpg_delinquent_charmed_1;
extern s32 rpg_delinquent_charmed_2;
extern u32 rpg_player_club;
extern s32 rpg_special_ready;
void rpg_club_move_basketball(void);
void rpg_club_move_brass_band(void);
void rpg_club_move_literature(void);
void rpg_club_move_computer(void);
void rpg_club_move_science(void);
void rpg_club_move_soccer(void);
void rpg_club_move_no_club(void);
void rpg_club_move_swimming(void);
void rpg_club_move_drama(void);
void rpg_club_move_art(void);
void rpg_club_move_tennis(void);
void rpg_club_move_baseball(void);
extern s32 rpg_finale_timer;
void rpg_player_wait_animation_substep();
void rpg_girl_action_yoshio();
void rpg_girl_action_megumi();
void rpg_girl_action_yumi();
void rpg_girl_action_saki();
void rpg_player_pose_idle();
void rpg_club_move_dispatch(void);
void rpg_girl_action_shiori();
s32 rpg_girl_action_delay();
s32 rpg_has_boss_event(void);
void rpg_move_assist_dispatch(void);
void rpg_drama_step_3(void);
void rpg_drama_overlay_fade(s32 arg0);
s32 rpg_drama_overlay_spawn(void);
s32 rpg_drama_overlay_hide(void);
s32 rpg_girl_pick_action();
void rpg_girl_action_yuina();
void rpg_girl_action_mira();
void rpg_girl_survival_timer();
void rpg_girl_rescue_sequence();
void rpg_girl_action_ayako();
s32 rpg_girl_action_mio();
s32 rpg_player_attack_hit_flurry(void);
void rpg_player_attack_miss_swing(void);
void rpg_girl_action_yuko();
void rpg_girl_action_nozomi();
void rpg_girl_action_yukari();
/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s32 rpg_message_mode;

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s32 rpg_girl_hit_points_maximum;

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s32 rpg_player_timer;
extern s32 rpg_move_limit;
void rpg_soccer_step_8(void);
extern s32 rpg_message_frame;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void rpg_brass_band_step_3(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void rpg_move_assist_yuina_flash(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void rpg_saki_effects_tick(void);
void rpg_saki_effect_step(s32 actor, s32 animation);
s32 rpg_swimming_main_effect(s32 arg0, s32 arg1);
void rpg_alien_scene_flicker(void);

#endif
extern u8 rpg_text_result_0[];
extern u8 rpg_text_result_pointers[];
extern u8 rpg_text_enemy_0[];
extern u8 rpg_text_enemy_pointers[];
extern s32 rpg_delinquent_gauges;
extern s32 rpg_delinquent_hit_points_1;
extern s32 rpg_delinquent_hit_points_2;
#ifndef RPG_BAT_OVERRIDE_rpg_player_step
/* The six words rpg_delinquent_gauges..rpg_delinquent_charmed_2 are one aggregate; rpg_apply_damage only matches when it reads them through
 * one struct (the original loads the counter above the flag-byte store, scalars give the other order; T-17107). */
typedef struct RpgDelinquentGauges {
    /* 0x00 */ s32 hit_points[3];  /* rpg_delinquent_gauges, rpg_delinquent_hit_points_1, rpg_delinquent_hit_points_2 */
    /* 0x0C */ s32 charmed[3]; /* rpg_delinquent_charmed_0, rpg_delinquent_charmed_1, rpg_delinquent_charmed_2 */
} RpgDelinquentGauges; /* size 0x18 */
/* FAKE: interim view. The scalar names above stay for the files that still use them (801504E0.c, 8014DF60.c,
 * 80154430.c, rpg_check_enemy_defeated), so RPG_GAUGE reaches the aggregate through the first symbol until those are
 * migrated to a real `RpgDelinquentGauges rpg_delinquent_gauges`. T-17107 */
#define RPG_GAUGE ((RpgDelinquentGauges *)&rpg_delinquent_gauges)
extern u32 rpg_player_step;
#endif
void rpg_literature_step_7(void);
void rpg_literature_step_8(void);
void rpg_literature_step_9(void);
extern u32 rpg_escalation_counter;
void rpg_fade_set_level();
void rpg_girl_wait_animation_step();
void rpg_dialog_yumi_fake_cry();
void rpg_dialog_enemy_apologizes();
extern u32 rpg_voice_playing_seen;

void rpg_mira_charm_apply();
void rpg_mira_reject_line();

extern s32 rpg_boss_event;
extern s32 D_8015EE28;
extern s32 D_8015EE20;
typedef struct RpgBattleScratch {
    /* 0x00 */ s32 values[50];
} RpgBattleScratch; /* size 0xC8 */


void rpg_soccer_step_3(void);
void rpg_soccer_step_9(void);

/* T-9780: second dialogue text buffer. */
s32 rpg_science_step_7(void);
void rpg_science_step_10(void);
void rpg_science_step_20(void);

#ifndef OVL_RPG_HEALTH_RECORDS_T9780
#define OVL_RPG_HEALTH_RECORDS_T9780
extern s32 rpg_label_player;
extern s32 rpg_label_girl;
extern s32 rpg_result_line_count;
#endif

#ifndef OVL_RPG_BAT_TRANSITION_T9740
#define OVL_RPG_BAT_TRANSITION_T9740

/* Packed shade used by the scene transition renderer. T-9740. */
typedef union RpgPackedColor {
    /* 0x00 */ u32 word;
    /* 0x00 */ struct {
        /* 0x00 */ u8 r;
        /* 0x01 */ u8 g;
        /* 0x02 */ u8 b;
        /* 0x03 */ u8 code;
    } rgb;
} RpgPackedColor; /* size 0x04 */

/* Three words rooted at rpg_girl_substep; the last advances by two per frame. */
typedef struct RpgTransitionState {
    /* 0x00 */ u32 step;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ s32 frame_offset;
} RpgTransitionState; /* size 0x0C */

void rpg_alien_departure(void);

#endif


#ifndef OVL_RPG_ASSET_WORDS_T9720
#define OVL_RPG_ASSET_WORDS_T9720
/* Word values copied into 0x44-byte draw records by rpg_effects_select_asset_set. */
#ifndef RPG_BAT_OVERRIDE_rpg_asset_sheet_0
extern s32 rpg_asset_sheet_0;
#endif
extern s32 rpg_asset_sheet_1;
extern s32 rpg_asset_sheet_4;
extern s32 rpg_asset_sheet_5;
extern s32 rpg_asset_frames_0;
extern s32 rpg_asset_frames_1;
extern s32 rpg_asset_frames_4;
extern s32 rpg_asset_frames_5;
extern s32 rpg_asset_animation_0;
extern s32 rpg_asset_animation_1;
extern s32 rpg_asset_animation_4;
extern s32 rpg_asset_animation_5;
#endif


#ifndef OVL_RPG_BAT_W9_FF28_H
#define OVL_RPG_BAT_W9_FF28_H
/* Dispatcher has no result read by its callers. T-9750. */
void rpg_robot_attack_1_motion(void);
#endif /* OVL_RPG_BAT_W9_FF28_H */


#ifndef W9_RPG_BAT_rpg_computer_step_13
#define W9_RPG_BAT_rpg_computer_step_13
void rpg_computer_step_13(void);
#endif

#ifndef W9_RPG_BAT_rpg_computer_step_15
#define W9_RPG_BAT_rpg_computer_step_15
void rpg_computer_step_15(void);
#endif


#ifndef W9_RPG_BAT_rpg_club_move_available
#define W9_RPG_BAT_rpg_club_move_available
extern s32 rpg_club_move_available;
#endif

#ifndef W9_RPG_BAT_rpg_asset_palette_0
#define W9_RPG_BAT_rpg_asset_palette_0
extern s32 rpg_asset_palette_0;
#endif

#ifndef W9_RPG_BAT_rpg_asset_palette_1
#define W9_RPG_BAT_rpg_asset_palette_1
extern s32 rpg_asset_palette_1;
#endif

#ifndef W9_RPG_BAT_rpg_asset_palette_2
#define W9_RPG_BAT_rpg_asset_palette_2
extern s32 rpg_asset_palette_2;
#endif

#ifndef W9_RPG_BAT_rpg_asset_palette_3
#define W9_RPG_BAT_rpg_asset_palette_3
extern s32 rpg_asset_palette_3;
#endif

#ifndef W9_RPG_BAT_rpg_asset_palette_6
#define W9_RPG_BAT_rpg_asset_palette_6
extern s32 rpg_asset_palette_6;
#endif

#ifndef W9_RPG_BAT_rpg_tennis_step_5
#define W9_RPG_BAT_rpg_tennis_step_5
void rpg_tennis_step_5(void);
#endif

#ifndef W9_RPG_TEXT_E92C
#define W9_RPG_TEXT_E92C
extern u8 rpg_text_girl_1[]; /* text destination; size unknown, rpg_dialog_yuko_listen. T-9710 */
#endif

#ifndef RPG_BAT_MOTION_T9930
#define RPG_BAT_MOTION_T9930
void rpg_science_step_23(void);
#endif

#ifndef OVL_RPG_BAT_TOGGLE_T10030
#define OVL_RPG_BAT_TOGGLE_T10030
extern s32 rpg_move_target_actor;
void rpg_apply_damage_single(void);
void rpg_art_step_8(void);
extern s32 D_8015EC68;
#endif
void rpg_mira_move(void);
void rpg_mira_target_position(s32 target_slot);

#ifndef OVL_RPG_BAT_PTRS_T10720
#define OVL_RPG_BAT_PTRS_T10720
extern s32 rpg_asset_sheet_2;
extern s32 rpg_asset_sheet_3;
extern s32 rpg_asset_frames_2;
extern s32 rpg_asset_frames_3;
extern s32 rpg_asset_animation_2;
extern s32 rpg_asset_animation_3;
#endif

#ifndef OVL_RPG_BAT_T10850_TYPES
#define OVL_RPG_BAT_T10850_TYPES
/* T-10850: initializer is called before its definition in its original object. */
s32 rpg_state_clear(void);
#endif


#ifndef OVL_RPG_BAT_T12040
#define OVL_RPG_BAT_T12040
void rpg_basketball_step_6(void);
void rpg_basketball_spark_start(s32 x, s32 y);

/* T-12040: displacement and the six rectangle words share EC50..EC6F. */
typedef struct RpgMoveMotion {
    /* 0x00 */ s32 dx;
    /* 0x04 */ s32 dy;
    /* 0x08 */ RpgMoveParams rect;
} RpgMoveMotion; /* size 0x20 */
void rpg_baseball_step_7(void);
void rpg_baseball_step_8(void);
void rpg_baseball_step_9(void);
s32 rpg_basketball_set_target_rect(void);
void rpg_basketball_step_3(void);
void rpg_basketball_step_4(void);
void rpg_basketball_step_5(void);

/* T-12040: position and displacement share the EC28..EC57 work range. */
typedef struct RpgActorMotion {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ u8 unk_08[0x20];
    /* 0x28 */ s32 dx;
    /* 0x2C */ s32 dy;
} RpgActorMotion; /* size 0x30 */

#endif

#ifndef OVL_RPG_BAT_T12300_DECLS
#define OVL_RPG_BAT_T12300_DECLS
/* T-12300: word flag defined in 8014E780.c and read by 80145CC0.c. */
extern s32 rpg_guest_value;
/* T-12300: word counters cleared by rpg_club_move_swimming (sw accesses). */
extern s32 rpg_move_value_0;
extern s32 rpg_move_value_1;
extern s32 rpg_move_x_start;
extern s32 rpg_move_x_end;
extern s32 rpg_move_extent;
extern s32 rpg_effect_phase_0;
extern s32 rpg_effect_phase_1;
extern s32 rpg_effect_phase_2;
extern s32 rpg_effect_phase_3;
/* Existing guarded definitions later in 8014A660.c. */
void rpg_swimming_splash_spawn(s32 effect_group, s32 x, s32 y);
void rpg_swimming_splash_update(void);

#ifndef OVL_RPG_BAT_T17100_TABLES
#define OVL_RPG_BAT_T17100_TABLES
/* T-17100: constant tables that unmatched functions copy whole to the stack
 * (an lw/sw run, written in C as one struct assignment, like RpgDramaLineTable). */
typedef struct RpgSparkDirections {
    /* 0x00 */ s32 offsets[16]; /* eight (x, y) unit steps at 45 degrees, 0x100 = 1.0 */
} RpgSparkDirections; /* size 0x40 */
typedef struct RpgRewardTextTable {
    /* 0x00 */ u8 *strings[4];
} RpgRewardTextTable; /* size 0x10 */
typedef struct RpgRewardSelectionFlags {
    /* 0x00 */ s32 selected[4];
} RpgRewardSelectionFlags; /* size 0x10 */
typedef struct RpgPadButtonSequence {
    /* 0x00 */ s32 buttons[10];
} RpgPadButtonSequence; /* size 0x28 */
typedef struct RpgMenuPositions {
    /* 0x00 */ s16 coordinates[12]; /* six (x, y) offsets */
} RpgMenuPositions; /* size 0x18 */
extern RpgSparkDirections rpg_spark_directions;  /* rpg_basketball_spark_burst */
extern RpgRewardTextTable rpg_loot_names_delinquents;   /* rpg_loot_text_delinquents */
extern RpgRewardSelectionFlags rpg_loot_flags_init;  /* rpg_loot_text_delinquents */
extern RpgPadButtonSequence rpg_code_sequence_a; /* rpg_code_entry_update */
extern RpgPadButtonSequence rpg_code_sequence_b; /* rpg_code_entry_update */
extern RpgMenuPositions rpg_menu_item_positions; /* rpg_command_menu_update */
extern RpgRewardTextTable rpg_loot_names_snake;   /* rpg_trip_loot_text */
extern RpgRewardTextTable rpg_loot_names_deer;   /* rpg_trip_loot_text */
extern RpgRewardTextTable rpg_loot_names_bear;   /* rpg_trip_loot_text */
extern RpgRewardSelectionFlags rpg_loot_flags_init_trip;  /* rpg_trip_loot_text */
extern RpgRewardTextTable rpg_loot_names_bancho;   /* rpg_loot_text_bancho */
extern RpgRewardTextTable rpg_titles_bancho;   /* rpg_loot_text_bancho */
extern RpgRewardSelectionFlags rpg_loot_flags_init_bancho;  /* rpg_loot_text_bancho (the first 16 bytes of a zero block) */
/* T-17100: called by rpg_club_move_no_club before their definitions. */
void rpg_no_club_step_6(void);
void rpg_no_club_step_4(void);
#endif

/* Selected positive int literals bound by exact private RPG_BAT header and global site. No metadata replay. */
#define RPG_PLAYER_ATTACK 1 /* Player action low-byte case; player_commands_and_club_moves.c:rpg_player_update cases 0x1 and assignments. */
#define RPG_PLAYER_SPECIAL 2 /* Player action low-byte case; player_commands_and_club_moves.c:rpg_player_update case 0x2 and assignments. */
#define RPG_PLAYER_GUARD 0x100 /* Guard action flag; player_commands_and_club_moves.c:rpg_player_update case and assignments. */
#define RPG_PLAYER_GUARD_RECOVER 0x104 /* Guard recovery action state; player_commands_and_club_moves.c:rpg_player_update case. */
#define RPG_PLAYER_DOWN 0x1000 /* Down action flag, set/cleared in enemy action routines; proposal associates it with player action state. */
#define RPG_PLAYER_READY 0x10000 /* Ready flag written by player_commands_and_club_moves.c:rpg_player_update after readiness delay. */
#define RPG_COMMAND_ATTACK 0x1000000 /* Top-byte menu request consumed in player_commands_and_club_moves.c:rpg_player_update. */
#define RPG_COMMAND_GUARD 0x2000000 /* Top-byte menu request consumed in player_commands_and_club_moves.c:rpg_player_update. */
#define RPG_COMMAND_SPECIAL 0x4000000 /* Top-byte menu request consumed in player_commands_and_club_moves.c:rpg_player_update. */

/* Selected positive int literals bound by exact private RPG_BAT header and global site. No metadata replay. */
enum RpgResult {
    RPG_RESULT_GOOD = 0, /* Zero selects DATE's reward branch and SHUGAKU's alternate trip reward; RPG_BAT writes zero on standing-player paths. */
    RPG_RESULT_BAD = 1 /* One selects DATE stress branch and SHUGAKU alternate result branch; set on delinquent/boss defeat and alien retreat. */
};

/* Partner HP constants at actual direct scalar sites only; not an invincibility claim. No metadata replay. */
#define RPG_GIRL_HIT_POINTS_SHIORI 256 /* Initial partner HP for GIRL_SHIORI (id 0). */
#define RPG_GIRL_HIT_POINTS_MIO 99 /* Initial partner HP for GIRL_MIO (id 1). */
#define RPG_GIRL_HIT_POINTS_YUINA 312 /* Initial partner HP for GIRL_YUINA (id 2). */
#define RPG_GIRL_HIT_POINTS_AYAKO 304 /* Initial partner HP for GIRL_AYAKO (id 3). */
#define RPG_GIRL_HIT_POINTS_SAKI 189 /* Initial partner HP for GIRL_SAKI (id 4). */
#define RPG_GIRL_HIT_POINTS_YUKARI 298 /* Initial partner HP for GIRL_YUKARI (id 5). */
#define RPG_GIRL_HIT_POINTS_NOZOMI 1878 /* Initial partner HP for GIRL_NOZOMI (id 6). */
#define RPG_GIRL_HIT_POINTS_MIRA 277 /* Initial partner HP for GIRL_MIRA (id 7). */
#define RPG_GIRL_HIT_POINTS_YUKO 365 /* Initial partner HP for GIRL_YUKO (id 8). */
#define RPG_GIRL_HIT_POINTS_MEGUMI 173 /* Initial partner HP for GIRL_MEGUMI (id 9). */
#define RPG_GIRL_HIT_POINTS_YUMI 265 /* Initial partner HP for GIRL_YUMI (id 10). */
#define RPG_GIRL_HIT_POINTS_YOSHIO 573 /* Initial partner HP for GIRL_YOSHIO (id 13). */
#define RPG_PARTNER_HIT_POINTS_SENTINEL 2147483647 /* Initial partner HP sentinel for id 14; hides the partner HP display. */

/* Enemy HP initialization values; exact selected sites recorded by preflight. No metadata replay. */
#define RPG_ENEMY_HIT_POINTS_ROBOT 5000 /* Initial robot enemy HP. */
#define RPG_ENEMY_HIT_POINTS_SENTINEL 2147483647 /* Initial alien enemy HP sentinel. */
#define RPG_ENEMY_HIT_POINTS_DELINQUENT 65536 /* Initial shared enemy HP for the delinquent encounter. */

/* Boss-event vs standard delinquent pool HP; scalar/gauge aliases retained. No metadata replay. */
#define RPG_DELINQUENT_HIT_POINTS_BOSS_EVENT 5000 /* Initial delinquent pool HP when the boss event is active. */
#define RPG_DELINQUENT_HIT_POINTS_STANDARD 9999 /* Initial delinquent pool HP otherwise. */

/* Initializer damage-scale values, applied over a 1000 denominator. No metadata replay. */
#define RPG_ENEMY_DAMAGE_SCALE_BASE 1000 /* 1000-denominator damage scale used for aliens and boss-event delinquents. */
#define RPG_ENEMY_DAMAGE_SCALE_STANDARD_DELINQUENT 1500 /* Initial enemy damage scale for the standard delinquent path (1500/1000 basis). */

/* Selected positive damage cap/critical literals; unchanged damage storage widths. No metadata replay. */
#define RPG_DAMAGE_MAXIMUM 9999 /* Maximum damage including forced critical rolls. */
#define RPG_DAMAGE_CLAMP_LIMIT 10000 /* Damage at or above this limit is clamped to the maximum. */

/* T-27075 group 6: RPG_BAT/player_attack_guard.c:21 waits 151 frames before the guard-recovery action. */
#define RPG_GUARD_RECOVERY_FRAMES 151 /* Guard recovery wait before the no-action healing branch. */

#endif
