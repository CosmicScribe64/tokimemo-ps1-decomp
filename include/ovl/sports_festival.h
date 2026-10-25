#ifndef OVL_SPORTS_FESTIVAL_H
#define OVL_SPORTS_FESTIVAL_H

#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "libgs.h"
#include "libgte.h"
#include "libspu.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/boot.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/date.h"
#include "main/day_event.h"
#include "main/debug.h"
#include "main/draw.h"
#include "main/event_log.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/import_views/school_events.h"
#include "main/layer.h"
#include "main/menu.h"
#include "main/message.h"
#include "main/pad.h"
#include "main/portrait.h"
#include "main/prim.h"
#include "main/scene.h"
#include "main/select_girl.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/view.h"
#include "main/vram.h"
#include "main/vsync.h"

/* Overlay-local externs (T-1300). */
void sports_runner_finish_animation_update(void);
void sports_candidate_scene_audio_request(void);
void sports_runner_actor_group_init(void);
void sports_runner_obstacle_actor_init(void);
void sports_runner_obstacle_sequences_assign(void);
void sports_runner_animation_and_rival_select(void);
void sports_runner_player_speed_input_update(void);
void sports_runner_player_position_integrate(void);
void sports_runner_player_screen_position_update(void);
void sports_runner_player_animation_update(void);
void sports_candidate_sequence_effect_update_a(void);
void sports_candidate_sequence_effect_update_b(void);
void sports_candidate_sequence_effect_update_c(void);
void sports_candidate_sequence_effect_update_d(void);
void sports_candidate_sequence_effect_update_e(void);
void sports_candidate_sequence_effect_update_f(void);
void sports_candidate_sequence_effect_phase_update(void);
void sports_candidate_sequence_effect_update_g(void);
void sports_candidate_motion_velocity_set(s32 motion_magnitude);
void sports_candidate_motion_position_approach(s32 position_step);
void sports_candidate_motion_bounds_clamp(void);
void sports_candidate_motion_input_update(void);
void sports_candidate_motion_integrate(void);
void sports_candidate_motion_project_and_bound(void);
void sports_candidate_model_rotation_update(void);
void sports_candidate_motion_speed_and_scroll_update(void);
void sports_candidate_floor_strip_draw(void);
void sports_candidate_rotated_floor_strip_draw(void);
void sports_candidate_floor_transition_draw(void);
void sports_candidate_scene_progress_update(void);
void sports_controller_c_player_velocity_input_update(void);
void sports_controller_c_player_velocity_integrate(void);
void sports_controller_c_player_position_clamp(void);
void sports_controller_c_player_sequence_select(void);
void sports_controller_c_cpu_velocity_target_update(void);
void sports_controller_c_cpu_motion_update(void);
void sports_controller_c_cpu_position_integrate(void);
void sports_controller_c_slot_animation_update(void);
void sports_runner_scene_background_tile_upload(void);
void sports_dance_background_upload(void);
void sports_controller_c_player_direction_update(void);
void sports_controller_c_motion_choices_update(void);
void sports_schoolyard_player_direction_set(void);
void sports_schoolyard_player_velocity_integrate(void);
void sports_controller_c_motion_choice_update(void);

extern s32 sports_schoolyard_animation_interval;
extern s32 sports_schoolyard_input_speed;
extern s32 D_80148F00;
extern s32 D_80148F04;
extern s32 D_80148F08;
extern s32 D_801491B0;
extern s32 D_801491B4;
extern s32 D_801491B8;
extern s32 D_8014A0A0;
extern s32 D_8014A0A4;
extern s32 D_8014A0A8;
extern s32 D_8014A0D0;
extern s32 D_8014A0D4;
extern s32 D_8014A0D8;
extern u8 sports_candidate_transition_wait_frames;
extern u8 sports_mio_scene_result;
extern u8 sports_schoolyard_transition_wait_frames;

extern u8 sports_dance_participant_girls[4];
extern s32 D_80148EF0;
extern s32 D_80148EF4;
extern s32 D_80148EF8;
extern s32 D_80148EFC;
extern s32 D_801491C0;
extern s32 D_801491C4;
extern s32 D_801491C8;
extern s32 D_801491CC;
extern s32 D_80149FA0;
extern s32 D_80149FA4;
extern s32 D_80149FA8;
extern s32 D_80149FAC;
extern s32 D_8014A0B0;
extern s32 D_8014A0B4;
extern s32 D_8014A0B8;
extern s32 D_8014A0BC;
extern s32 sports_schoolyard_motion_limit;
extern s32 sports_runner_motion_increment_limit;
extern s32 sports_controller_c_motion_limit;
extern s32 D_8014A370;
extern s32 D_8014A374;
extern s32 D_8014A378;
extern s32 D_8014A37C;
extern s32 D_8014A380;
extern s32 D_8014A384;
extern s32 D_8014A388;

extern s32 D_8014A0E0;
extern s32 D_8014A0E4;
extern s32 D_8014A0E8;
extern s16 D_8014A0EC;
extern s32 D_8014A0F0;
extern s32 D_8014A0F4;
extern s32 D_8014A0F8;
extern s32 D_8014A0FC;
extern s32 D_8014A100;
extern s32 D_8014A104;
extern s32 D_8014A108;
extern s32 D_8014A390;
extern s32 D_8014A394;
extern s32 D_8014A398;
extern s16 D_8014A39C;
extern s32 D_8014A3A0;
extern s32 D_8014A3A4;
extern s32 D_8014A3A8;
extern s32 D_8014A3AC;
extern s32 D_8014A3B0;
extern s32 D_8014A3B4;
extern s32 D_8014A3B8;

extern s32 D_80149940;
typedef struct SportsPairValue {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 val;
} SportsPairValue; /* size 0x4 */
extern SportsPairValue sports_candidate_player_progress_record[];
extern volatile u16 sports_candidate_player_progress_value_view; /* sports_candidate_player_progress_record[0].val as a u16 flag; volatile is FAKE (sports_candidate_sequence_effect_update_g), T-11640 */
extern volatile u16 sports_candidate_sequence_frame_view; /* sports_candidate_sequence_motion.unk_02 as a u16 counter; volatile is FAKE (sports_candidate_sequence_effect_update_b, sports_candidate_sequence_effect_update_e), T-24124 */
extern volatile s32 sports_candidate_velocity_x_view; /* D_80149968.unk_0C as a volatile s32; volatile is FAKE (sports_candidate_sequence_effect_update_e), T-24124 */

extern s32 D_80149FB0;
extern s32 D_80149FC0;
extern s32 D_80149FD0;
extern s32 D_80149FE8;
extern s32 D_80149FF8;
extern s32 D_8014A008;
extern s16 D_8014A020;
extern s16 D_8014A030;
extern s16 D_8014A040;
void sports_runner_cpu_phase_update(void);
void sports_runner_cpu_speed_config_copy(void);
void sports_runner_cpu_motion_state_update(void);
void sports_runner_position_and_finish_update(void);
void sports_runner_actor_animation_state_update(void);
void sports_candidate_runner_display_position_update(s32);
void sports_candidate_runner_blink_animation_update(void);
void sports_controller_c_slot_animation_threshold_check(s32);
void sports_controller_c_slot_move_finish(s32);
void sports_controller_c_slot_position_step(s32);
void sports_controller_c_slot_actor_position_update(s32);
s32 sports_nozomi_courtyard_scene_step();
void sports_dance_actors_position(); /* K&R definition in 80132BD0.c (s16 first parameter) */
void sports_folk_dance_log_append(void);
void sports_folk_dance_talk_resource_addresses_init(void);
s32 sports_mio_ground_scene_step();
extern u8 sports_schoolyard_result_selector;
void sports_schoolyard_player_position_clamp(void);
void sports_schoolyard_player_sequence_update(void);
void sports_schoolyard_input_state_update(void);
void sports_schoolyard_controller_state_init(void);
void sports_schoolyard_sprite_resource_setup(void);
void sports_schoolyard_primary_actor_setup(void);
void sports_schoolyard_resource_addresses_init(void);
typedef struct TaiikuRec {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
    /* 0xC */ s32 unkC;
} TaiikuRec; /* size 0x10 */
extern s32 sports_candidate_pointer_region;

void sports_dance_actor_move();
extern s32 D_801492C4;
void sports_candidate_camera_transition_update(void);
s8 sports_girl_scene_result_candidate_select();
void sports_schoolyard_cpu_speed_limit_update(void);
void sports_schoolyard_cpu_acceleration_update(void);
void sports_schoolyard_runner_position_and_finish_order_update(void);
void sports_schoolyard_cpu_animation_state_advance(void);
void sports_schoolyard_cpu_random_animation_transition(void);
void sports_schoolyard_actor_group_init(void);
void sports_schoolyard_result_flow_update(void);
void sports_schoolyard_actor_display_init(void);
extern s32 D_8014A4F4;
extern s32 sports_controller_c_texture_page_buffer_ids[12]; /* tpage buffer ids, loaded in sports_controller_c_resource_load. T-16553 */
extern s32 sports_controller_c_texture_page_buffer_ids_table_b[9];
s32 sports_controller_c_result_actor_approach(void);
s32 sports_controller_c_result_sequence_shift(void);
void sports_controller_c_result_actor_motion_start(void);
void sports_controller_c_result_sequence_advance(void);
void sports_controller_c_slot_motion_a_start(s32 slot_index);
void sports_controller_c_sprite_resource_setup(void);
void sports_controller_c_primary_actor_init(void);
void sports_controller_c_state_init(void);
void sports_controller_c_actor_display_init(void);
void sports_controller_c_slot_motion_b_start(s32 slot_index);
extern u8 D_8014A1FC;
void sports_controller_c_player_sequence_advance(void);
void sports_scene_display_parameters_set(s16 *);
void sports_candidate_background_quad_draw(void);
extern s16 sports_runner_input_latched;
extern s32 sports_runner_input_speed;
extern u8 sports_runner_finish_order[4]; /* Zero means empty; player is 1 and CPU runners are 2 through 4. */
extern s32 sports_candidate_sequence_step;
extern s32 sports_candidate_secondary_sequence_step;
typedef struct {
    s32 sequence_count;
    u16 primary_frame_counter;
    u16 secondary_frame_counter;
} TaiikuSel; /* size 8 */
extern TaiikuSel sports_candidate_sequence_counters;
extern s32 sports_candidate_secondary_sequence_count;
void sports_candidate_sequence_state_dispatch(void);
void sports_candidate_effect_animation_step(void);
void sports_candidate_effect_motion_state_update(void);

extern u8 sports_controller_c_input_latched;
extern s32 sports_controller_c_input_speed;
extern s32 sports_controller_c_scroll_velocity;
extern s32 D_8014A3DC;
void sports_controller_c_scroll_integrate(void);
extern u8 sports_controller_c_result_status[]; /* next free slot gets i + 2 for runner i (sports_controller_c_cpu_position_integrate) */
/* Four s16 values added to the actor_pool records (sports_girl_result_actor_sequence_advance copies them to a local). */
typedef struct TkS16x4 {
    /* 0x0 */ s16 v[4];
} TkS16x4; /* size 0x8 */
extern TkS16x4 D_80149F8C;
extern TkS16x4 D_8014A360;
extern u8 D_801492AC[];
extern u8 D_8014A1D4[];
extern s32 D_8014A208[8]; /* tpage buffer ids, sports_schoolyard_scene_audio_sequence */
extern s32 D_8014A228[11]; /* tpage buffer ids, sports_schoolyard_scene_audio_sequence */
extern s16 D_8014A1E8[]; /* s16 table paired with D_8014A1D4 (sports_schoolyard_player_sequence_update) */
void sports_controller_c_mode_dispatch(void);
void sports_festival_choice_menu_init(void);
s32 sports_festival_choice_menu_update(void);
void sports_festival_first_followup_select(void);
void sports_festival_second_followup_select(void);
void sports_mio_festival_talk_resource_addresses_init(void);

typedef struct {
    s32 v[11];
} TaiikuTbl11; /* size 0x2C */
extern u8 sports_runner_selected_girl;
extern TaiikuTbl11 D_801498E8;
extern TaiikuTbl11 D_80149914;
void sports_controller_c_player_frame_update(void);
void sports_controller_c_cpu_frame_update(void);
void sports_controller_c_frame_update(void);
void sports_controller_c_slot_motion_update(void);
extern s16 D_8014A3DE;
extern u8 sports_schoolyard_result_status[]; /* next free slot gets i + 2 for runner i (sports_schoolyard_runner_position_and_finish_order_update) */
extern s32 D_8014A130[];
extern s16 D_8014A1CE;

#endif
extern s32 D_80149954;
extern s16 D_80149958;
extern s16 D_8014995A;
extern u8 sports_candidate_result_order[];
void sports_candidate_sequence_audio_counter_update(void);
extern u8 sports_schoolyard_speed_update_frames;

typedef struct TkCnt {
    /* 0x0 */ u8 unk_00;
    /* 0x4 */ u32 unk_04;
    /* 0x8 */ u32 unk_08;
} TkCnt; /* size 0xC */

typedef union {
    s32 word;
    struct {
        s16 low;
        s16 high; /* D_8014929A */
    } halves;
} TaiHalves; /* size 0x4 */
extern TaiHalves D_80149298; /* defined in 80133C80.c */
void sports_runner_scene_scroll_update(void);
void sports_runner_race_phase_one(void);
void sports_runner_obstacle_scroll_update(void);
s32 sports_runner_actor_timer_increment();
void sports_runner_player_control_update(void);
s32 sports_runner_scroll_update();
void sports_runner_cpu_update(void);
void sports_runner_rival_choice_input_process(void);
void sports_runner_race_state_init(void);
s32 sports_runner_finish_audio_and_lane_update();
s32 sports_runner_finish_actor_display_update();
s32 sports_runner_finish_line_update();
void sports_runner_course_end_transition(void);
void sports_runner_finish_actor_group_init();
s32 sports_runner_finish_display_selector();
void sports_runner_sprite_memory_init(void);
void sports_runner_display_context_init(void);
s16 sports_runner_finish_marker_x_select();

#ifndef OVL_TAIIKU_W8_3_TYPES_H
#define OVL_TAIIKU_W8_3_TYPES_H
/* Local random-selection table: eleven rows of ten signed halfwords.
 * Both the block copy and row stride are visible in sports_runner_obstacle_sequence_select. T-9530. */
typedef struct SportsRandomTable {
    /* 0x00 */ s16 value[11][10];
} SportsRandomTable; /* size 0xDC */
extern SportsRandomTable D_80149570;
extern s16 sports_runner_character_index;

typedef struct TaiWords4 {
    /* 0x00 */ s32 value[4];
} TaiWords4; /* size 0x10 */
typedef struct TaiWords11 {
    /* 0x00 */ s32 value[11];
} TaiWords11; /* size 0x2C */
extern TaiWords4 D_801498AC;
extern TaiWords11 D_801498BC;
extern s16 sports_runner_girl_animation_groups[];
extern s32 sports_runner_scroll_velocity;
extern s16 D_801492A0; /* sports_runner_race_state_init */
extern u8 D_801492A4[2]; /* sports_runner_race_state_init */
extern u8 D_801492A8[4]; /* sports_runner_race_state_init */
#endif /* OVL_TAIIKU_W8_3_TYPES_H */
#ifndef TAIIKU_W8_CONTROLLER_TYPES_H
#define TAIIKU_W8_CONTROLLER_TYPES_H
/* Game C runner records (801452D0.c): sports_controller_c_player_controller is the player, sports_controller_c_cpu_controllers[3] the three other
 * runners, two objects: sports_controller_c_slot_move_finish indexes sports_controller_c_cpu_controllers by runner - 1 and the loops of
 * sports_controller_c_cpu_velocity_target_update/sports_controller_c_cpu_motion_update by i; written as one 4-element array they compile differently.
 * T-9570, T-16550. */
typedef struct SportsController {
    /* 0x00 */ u8 animation_state;
    /* 0x01 */ u8 finish_recorded;
    /* 0x02 */ u16 direction_sound_frames;
    /* 0x04 */ u32 animation_frames;
    /* 0x08 */ u32 input_sample_frames;
    /* 0x0C */ u32 motion_update_frames;
    /* 0x10 */ s32 motion_delta;
    /* 0x14 */ s32 motion_increment;
    /* 0x18 */ Fix32 position;
} SportsController; /* size 0x1C */
extern SportsController sports_controller_c_player_controller;
extern SportsController sports_controller_c_cpu_controllers[3];

#endif

/* T-9510: coordinate bounds loaded with LH in sports_candidate_motion_position_approach. */

#ifndef OVL_TAIIKU_T9510_COORD_TYPES
#define OVL_TAIIKU_T9510_COORD_TYPES
/* T-9510: the two coordinate words are read both whole and by high half. */
typedef union TkCoord32 {
    /* 0x00 */ s32 word;
    struct {
        /* 0x00 */ s16 lo;
        /* 0x02 */ s16 hi;
    } halves;
} TkCoord32; /* size 0x04 */

typedef struct TkCoordsView {
    /* 0x0000 */ u8 pad0[0x19E8];
    /* 0x19E8 */ TkCoord32 pos[2];
} TkCoordsView; /* accessed prefix size 0x19F0 */
#endif

typedef struct TaiikuWords48 {
    /* 0x00 */ s32 unk_00[12][4];
} TaiikuWords48; /* size 0xC0 */
extern TaiikuWords48 D_80149030;
extern TaiikuWords48 D_801490F0;

extern s32 sports_schoolyard_scroll_velocity;

#ifndef OVL_TAIIKU_RANDOM_T9610
#define OVL_TAIIKU_RANDOM_T9610
extern SportsRandomTable D_8014964C;
#endif

#ifndef OVL_TAIIKU_W9_2_INIT_FIELDS_H
#define OVL_TAIIKU_W9_2_INIT_FIELDS_H
/* Workspace address words and asset halfwords used by sports_scene_actor_resource_addresses_init. T-9720. */
extern s32 D_80149FB4;
extern s32 D_80149FB8;
extern s32 D_80149FBC;
extern s32 D_80149FC4;
extern s32 D_80149FC8;
extern s32 D_80149FCC;
extern s32 D_80149FD4;
extern s32 D_80149FD8;
extern s32 D_80149FDC;
extern s32 D_80149FE0;
extern s32 D_80149FE4;
extern s32 D_80149FEC;
extern s32 D_80149FF0;
extern s32 D_80149FF4;
extern s32 D_80149FFC;
extern s32 D_8014A000;
extern s32 D_8014A004;
extern s32 D_8014A00C;
extern s32 D_8014A010;
extern s32 D_8014A014;
extern s32 D_8014A018;
extern s32 D_8014A01C;
extern s16 D_8014A024;
extern s16 D_8014A028;
extern s16 D_8014A02C;
extern s16 D_8014A034;
extern s16 D_8014A038;
extern s16 D_8014A03C;
extern s16 D_8014A044;
extern s16 D_8014A048;
extern s16 D_8014A04C;
extern s16 D_8014A050;
extern s16 D_8014A054;
extern s16 D_8018EC40;
extern s16 D_8018EC44;
extern s16 D_8018EC48;
extern s16 D_8018EC4C;
extern s16 D_8018EC50;
extern s16 D_8018EC54;
extern s16 D_8018EC58;
extern s16 D_8018EC5C;
extern s16 D_8018EC60;
extern s16 D_8018EC64;
extern s16 D_8018EC68;
extern s16 D_8018EC6C;
extern s16 D_8018EC70;
extern s16 D_8018EC74;
/* T-9930: calls from the race-state dispatcher; matched definitions below it. */
void sports_schoolyard_frame_counters_advance(void);
void sports_schoolyard_player_frame_update(void);
void sports_schoolyard_cpu_animation_and_motion_update(void);
#endif

#ifndef OVL_TAIIKU_SLOT_T10020
#define OVL_TAIIKU_SLOT_T10020
/* Game C runner sprite records sports_runner_display_slots[4], one per runner of sports_controller_c_player_controller (T-10020, T-10850, T-16550). */
typedef struct SportsRunnerDisplaySlot {
    /* 0x00 */ s16 x_step_fraction;
    /* 0x02 */ s16 x_step_integer;
    /* 0x04 */ s16 y_step_fraction;
    /* 0x06 */ s16 y_step_integer;
    /* 0x08 */ s32 horizontal_direction_bias;
    /* 0x0C */ u8 motion_state;
    /* 0x0D */ u8 motion_timer;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ u8 pad_10[4];
    /* 0x14 */ u16 direction_bias_frames;
    /* 0x16 */ u16 cooldown;
    /* 0x18 */ u16 direction_adjustment_frames;
    /* 0x1A */ u8 pad_1A[2];
} SportsRunnerDisplaySlot; /* size 0x1C */
extern SportsRunnerDisplaySlot sports_runner_display_slots[4];
#endif

#ifndef OVL_TAIIKU_T10010_SCROLL_POSITION
#define OVL_TAIIKU_T10010_SCROLL_POSITION
void sports_schoolyard_scroll_update(void);
extern Fix32 D_8014A1CC; /* Scroll position; signed high half at +2. T-10010. */
void sports_candidate_motion_reset(void);
extern s32 sports_candidate_input_latched;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} Taiiku2x16; /* size 4 */
typedef struct {
    u16 action_frames;
    u16 sequence_frames;
    s16 target_x;
    s16 target_y;
    s32 acceleration_x;
    s32 acceleration_y;
} TaiikuA0; /* size 0x10 */
extern TaiikuA0 sports_candidate_sequence_motion;
extern u8 D_801499DC;
extern s16 sports_candidate_speed_update_frames;

typedef struct {
    s32 unk_00;
    s32 unk_04;
} Taiiku2s32; /* size 8 */
typedef struct {
    s16 unk_00;
    s16 unk_02;
} Taiiku2s16; /* size 4 */
typedef struct {
    s32 boundary_side;
    s32 input_acceleration_x;
    s32 input_acceleration_y;
} Taiiku3s32; /* size 0xC */
/* 0x20-byte block at D_80149968 (T-11640): sports_candidate_motion_project_and_bound reads the high halves of the 16.16 words at
 * 0x14 and 0x18 through the block's base address, so the words are fields of one object. */
typedef struct {
    /* 0x00 */ s32 acceleration_x;
    /* 0x04 */ s32 acceleration_y;
    /* 0x08 */ u16 input_decay_frames;
    /* 0x0A */ u16 velocity_update_frames;
    /* 0x0C */ s32 velocity_x;
    /* 0x10 */ s32 velocity_y;
    /* 0x14 */ s32 position_x;
    /* 0x18 */ s32 position_y;
    /* 0x1C */ s16 screen_coordinates[2]; /* [0] x, [1] y */
} SportsFixedPointBlock; /* size 0x20 */
extern SportsFixedPointBlock D_80149968;
extern Taiiku3s32 sports_candidate_input_and_boundary_state;





/* Game A runner record (80133C80.c), 0x24 bytes. The 16.16 word at 0x20 is the track position,
 * read by its signed high half. Game C's SportsController has the same roles at other offsets
 * (speed 0x10, timers, position). T-16550. */
typedef struct SportsRunnerA {
    /* 0x00 */ u8 result_actor_slide_complete;
    /* 0x01 */ u8 result_actor_slide_pending;
    /* 0x02 */ u8 display_actor_index; /* index of the runner's display record (actor_pool record 46 + n) */
    /* 0x03 */ u8 animation_frame; /* animation frame */
    /* 0x04 */ u8 animation_cycle_index;
    /* 0x05 */ u8 start_sequence_state; /* state of the start sequence */
    /* 0x06 */ u8 finish_recorded; /* Set after inserting or forcing this runner in the finish order. */
    /* 0x07 */ u8 choice_input_frames;
    /* 0x08 */ u16 slide_wait_limit;
    /* 0x0A */ u16 slide_wait_frames;
    /* 0x0C */ u16 animation_tick_frames;
    /* 0x0E */ u16 animation_cycle_frames;
    /* 0x10 */ s32 speed; /* speed */
    /* 0x14 */ u32 speed_update_frames;
    /* 0x18 */ s32 motion_increment; /* Accumulated speed added to the 16.16 track position. */
    /* 0x1C */ u32 motion_update_frames; /* Elapsed frames since the last motion-increment update. */
    /* 0x20 */ TaiHalves track_position; /* 16.16 position */
} SportsRunnerA; /* size 0x24 */
/* Game B runner record (80142240.c), 0x24 bytes: a second layout for the same roles as
 * SportsRunnerA (speed 0x08, accumulated speed 0x0C, timers 0x10/0x18/0x1A, 16.16 position 0x14). T-16550. */
typedef struct SportsRunnerB {
    /* 0x00 */ u8 animation_frame; /* animation frame */
    /* 0x01 */ u8 finish_recorded;
    /* 0x02 */ u8 animation_transition_pending;
    /* 0x03 */ u8 pad_03;
    /* 0x04 */ s32 animation_frames;
    /* 0x08 */ s32 speed; /* speed */
    /* 0x0C */ s32 motion_increment;
    /* 0x10 */ u32 motion_update_frames;
    /* 0x14 */ TaiHalves track_position; /* 16.16 position */
    /* 0x18 */ u16 animation_start_frames;
    /* 0x1A */ u16 action_recovery_frames;
    /* 0x1C */ s32 action_ready;
    /* 0x20 */ s32 unk_20;
} SportsRunnerB; /* size 0x24 */
/* sports_controller_c_runners[4]: [0] the player, [1..3] the other runners; every access goes through
 * %lo(sports_controller_c_runners). The u8 view it had made IDO keep the array base in a register for the
 * constant-index clamps of sports_schoolyard_player_position_clamp, which a struct member does not; a 4-element array does. T-16550. */
extern SportsRunnerB sports_controller_c_runners[4];

/* sports_runner_a_runners..D_80149297: the player and three other runners, one object. Every access in the
 * original goes through %lo(sports_runner_a_runners); the matched loops index the other runners by i from 0
 * (cpu[i]), and sports_runner_finish_display_selector indexes all four by runner number. T-16550. */
typedef struct SportsRunnersA {
    /* 0x00 */ SportsRunnerA player;
    /* 0x24 */ SportsRunnerA cpu[3];
} SportsRunnersA; /* size 0x90 */
extern SportsRunnersA sports_runner_a_runners;

#endif


#ifndef OVL_TAIIKU_LIMITS_T10720
#define OVL_TAIIKU_LIMITS_T10720
/* Limit tables copied to the stack by sports_runner_cpu_motion_state_update. */
typedef struct {
    /* 0x00 */ s32 v[3];
} TaiikuI3; /* size 0xC */
typedef struct {
    /* 0x00 */ s16 v[3];
} TaiikuH3; /* size 6 */
extern TaiikuI3 sports_runner_cpu_motion_limits;
extern TaiikuH3 sports_runner_cpu_pause_positions;
#endif

#ifndef OVL_TAIIKU_U16TBL_T10720
#define OVL_TAIIKU_U16TBL_T10720
typedef struct {
    u16 v[11];
} TaiikuU11; /* size 0x16 */
typedef struct {
    s16 v[11];
} TaiikuS11; /* size 0x16 */
extern TaiikuS11 D_80149EE8;
extern s32 D_80149DA4[];
extern TaiikuU11 sports_candidate_sequence_frame_limits;
extern TaiikuS11 D_80149F68;
extern s32 sports_candidate_motion_speed;
#endif

#ifndef OVL_TAIIKU_T10850_TYPES
#define OVL_TAIIKU_T10850_TYPES

/* T-10850: stack copies in sports_controller_c_slot_actor_position_update, two 0x14 and two 0x10 byte tables. */
typedef struct TaiikuHalf10 {
    /* 0x00 */ s16 h[10];
} TaiikuHalf10; /* size 0x14 */
typedef struct TaiikuHalf8 {
    /* 0x00 */ s16 h[8];
} TaiikuHalf8; /* size 0x10 */
extern TaiikuHalf10 D_8014A620;
extern TaiikuHalf10 D_8014A634;
extern TaiikuHalf8 D_8014A648;
extern TaiikuHalf8 D_8014A658;
extern u16 D_8014A4C4[8];
extern u16 D_8014A4D8[6];
extern u16 D_8014A4E8[6];
extern TaiikuHalf8 D_8014A668; /* copied to a stack table in sports_controller_c_slot_animation_threshold_check. T-16553 */
extern u16 D_8014A4D4[8]; /* with D_8014A4C4 and D_8014A4E4: sprite frame offsets by animation step. T-16553 */
extern u16 D_8014A4E4[8];
#endif

#ifndef OVL_TAIIKU_CAMERA_T10890
#define OVL_TAIIKU_CAMERA_T10890
/* T-10890: three words at overlay_scratch used by the rotated track grid. */
typedef struct TaiikuCamera0C {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 angle;
} TaiikuCamera0C; /* size 0x0C */
/* T-25102: the 0x100-byte block at overlay_scratch (main bzeroes it) as TAIIKU uses it: the camera and, at 0xF4,
 * the step of the track-grid intro that other functions still read as D_80122EC4. sports_candidate_camera_transition_update matches
 * only with the step and the camera reached through one symbol: as1 keeps the order of accesses to one
 * symbol (the step load stays above the camera stores), and reorders accesses to two. */
typedef struct TaiikuWork {
    /* 0x00 */ TaiikuCamera0C camera;
    /* 0x0C */ u8 unk_0C[0xE8];
    /* 0xF4 */ s32 step;
} TaiikuWork; /* size 0xF8 */
#endif

#ifndef OVL_TAIIKU_RESULT_TABLES_T10890
#define OVL_TAIIKU_RESULT_TABLES_T10890
/* Eleven parallel per-character result entries, copied by sports_runner_race_phase_two. */
typedef struct TaiText11 {
    /* 0x00 */ u8 *value[11];
} TaiText11; /* size 0x2C */
extern TaiikuS11 D_801493FC;
extern TaiText11 D_80149414;
extern TaiText11 D_80149440;
extern TaiWords11 D_8014946C;
extern TaiWords11 D_80149498;
extern u8 sports_runner_audio_request_sent;
s32 sports_runner_finish_actor_slide_update(s32 runner_index);
#endif

#ifndef OVL_TAIIKU_SELECTION_T10890
#define OVL_TAIIKU_SELECTION_T10890
/* Copied as 0x58 bytes, indexed by eleven rows of eight actor flags. T-10890. */
typedef struct TaiFlags58 {
    /* 0x00 */ u8 value[11][8];
} TaiFlags58; /* size 0x58 */
extern TaiWords11 D_80149728;
extern TaiikuS11 D_80149754;
extern TaiikuS11 D_8014976C;
extern TaiFlags58 sports_runner_girl_motion_flags;
extern u8 D_801491D8;
#endif

#ifndef OVL_TAIIKU_MOTION_CALLS_T10890
#define OVL_TAIIKU_MOTION_CALLS_T10890
/* Definitions in 80132BD0.c and 80138950.c. T-10890. */
void sports_dance_actor_move_clamp(s32 actor_index);
void sports_candidate_motion_state_init(void);
void sports_candidate_animation_selector_update(void);
void sports_candidate_pointer_position_clamp(void);
s32 sports_candidate_pointer_region_select(void);
#endif

#ifndef OVL_TAIIKU_ACTOR_MOTION_T10890
#define OVL_TAIIKU_ACTOR_MOTION_T10890
/* One 16.16 position word; motion reads its signed high half. T-10890. */
extern TaiHalves sports_candidate_player_position;
/* Indexed at arg0=1..3; the leading byte is the preceding data padding. */
extern u8 D_801499CB[4];
s32 sports_candidate_runner_state_select(s32 runner_index);
s16 sports_candidate_runner_phase_select(s32 runner_index);
#endif

 
#ifndef OVL_TAIIKU_T10890_PAGE_TABLES
#define OVL_TAIIKU_T10890_PAGE_TABLES
/* T-10890: nine signed halfword choices; copied as 16 bytes plus 2 bytes. */
typedef struct TaiShorts12 {
    /* 0x00 */ s16 value[9];
} TaiShorts12; /* size 0x12; data symbols are aligned to four bytes */
extern TaiShorts12 D_801493C0;
extern TaiShorts12 D_801493D4;
extern TaiShorts12 D_801493E8;
/* Texture-page handles walked in sports_runner_race_scene_setup. */
extern s32 D_801492C8[5];
extern s32 D_801492DC[6];
extern s32 D_801492F4;
extern s32 D_801492F8[10];
extern s32 D_8014A070[6];
void sports_candidate_scene_layer_resource_addresses_init(void);
void sports_runner_scene_graphics_init(void);
#endif

/* T-11080: six-entry s16 table in the data of 80133C80.c (read by sports_runner_finish_actor_display_update). */
extern s16 D_801492B8[6];

#ifndef OVL_TAIIKU_WAIT3_T12020
#define OVL_TAIIKU_WAIT3_T12020
typedef struct TaiWait3 {
    /* 0x00 */ u16 value[3];
} TaiWait3; /* size 0x06 */
extern TaiWait3 sports_runner_cpu_slide_waits;

#endif

/* Eleven character IDs used by sports_runner_obstacle_actor_init. T-12020. */
extern TaiikuS11 sports_runner_character_codes;

/* Eleven animation limits copied by sports_runner_player_animation_update. T-12020. */
extern TaiikuU11 sports_runner_athletics_motion_limits;

#ifndef OVL_TAIIKU_T13600_DECLS
#define OVL_TAIIKU_T13600_DECLS
/* T-13600: scene dispatcher and helpers defined later in 80138950.c. */
void sports_candidate_scene_frame_update(void);
void sports_candidate_scene_frame_counters_advance(void);
void sports_candidate_scene_actor_state_update(void);
void sports_candidate_motion_update(void);
void sports_candidate_sequence_effect_dispatch(void);
void sports_candidate_rival_display_update(void);
#endif

#ifndef OVL_TAIIKU_RUNNERS_T11640
#define OVL_TAIIKU_RUNNERS_T11640
/* T-11640: TAIIKU's view of the actor_pool display records (Actor layout). The bytes at
 * 0x01 and 0x06 are signed here: sports_girl_scene_result_actor_setup stores them with constants that the original keeps
 * apart from the unsigned switch constants of its later loops, which a u8 field merges. T-20900: with the
 * shared u8 record sports_girl_scene_choose, sports_girl_scene_result_update and sports_girl_scene_result_actor_setup do not match, so this view stays; its other
 * fields are the shared layout. */
typedef struct TaiikuActor44 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ s8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 *unk_0C;
    /* 0x10 */ u8 *unk_10;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ s16 unk_28;
    /* 0x2A */ s16 unk_2A;
    /* 0x2C */ s16 unk_2C;
    /* 0x2E */ s16 unk_2E;
    /* 0x30 */ s16 unk_30;
    /* 0x32 */ s16 unk_32;
    /* 0x34 */ u8 *unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 unk_41;
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
} TaiikuActor44; /* size 0x44 */
/* T-11640: per-runner tables of sports_candidate_scene_progress_update (runners 1..3; sports_candidate_cpu_progress_records holds their 16.16 positions). */
extern TaiHalves sports_candidate_cpu_progress_records[]; /* 16.16 positions; sports_candidate_player_progress_record[1..3] seen from sports_candidate_runner_state_select */
extern TaiikuI3 sports_candidate_progress_deltas; /* speed added to each position, copied to the stack */
extern u16 sports_candidate_activity_frames[];
extern u8 sports_candidate_activity_flags[];
extern s32 D_80149DF4[];
extern s32 D_80149DAC[];
/* Tables sports_scene_asset_load_setup copies to the stack and then into D_80149DA4/D_80149DAC/D_80149DF4. */
typedef struct {
    /* 0x00 */ s32 v[2];
} TaiikuW2; /* size 8 */
typedef struct {
    /* 0x00 */ s32 v[18];
} TaiikuW18; /* size 0x48 */
typedef struct {
    /* 0x00 */ s32 v[5];
} TaiikuW5; /* size 0x14 */
extern TaiikuW2 D_80149E2C;
extern TaiikuW18 D_80149E34;
extern TaiikuW5 D_80149E7C;
extern s32 D_80149964;
extern s16 D_80149966;
/* GTE layouts used by sports_candidate_model_rotation_update and sports_candidate_model_transform_update (moved from 80138950.c, T-11640). */
typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} TaiikuSVec; /* SVECTOR layout */



/* Track grid tables filled by sports_candidate_scene_state_init (80 entries each). */
extern s16 D_801499E4[];
extern s16 D_80149A84[];
extern s16 D_80149B24[];
extern s16 D_80149BC4[];
extern s16 D_80149C64[];
extern s16 D_80149D04[];
void sports_candidate_scene_state_init(void);
void sports_candidate_scene_render_resource_setup(void);
void sports_candidate_actor_display_init(void);
void sports_candidate_model_transform_update(void);
void sports_girl_scene_result_actor_setup(void);
void sports_girl_result_actor_motion_update(void);
void sports_girl_result_actor_sequence_advance(void);
void sports_girl_result_animation_toggle(void);
void sports_schoolyard_runner_animation_step(void);
void sports_schoolyard_runner_animation_table_step(void);
void sports_schoolyard_actor_visibility_toggle(void);
void sports_scene_audio_sequence_request(); /* K&R definition in 80132BD0.c (u8 second parameter) */
void sports_girl_scene_flagged_actors_scroll(void);
void sports_girl_scene_actors_init(void);
void sports_candidate_model_lighting_init(void);
void sports_candidate_scene_lighting_init(void);
void sports_candidate_model_slots_init(void);
void sports_candidate_model_draw_packets_init(); /* K&R definition */
void sports_candidate_model_draw_packet_bind(s32 arg0, s32 arg1);
void sports_candidate_sequence_direction_audio_update(void);
void sports_candidate_actor_proximity_motion_update(void);
/* Tables sports_girl_scene_choose copies to the stack and indexes by D_80149940. */
extern TaiWords11 D_80149E90;
extern TaiWords11 D_80149EBC;
typedef struct {
    /* 0x00 */ s16 v[16];
} TaiikuH16; /* size 0x20 */
typedef struct {
    /* 0x00 */ u8 v[16];
} TaiikuB16; /* size 0x10 */
/* Scene tables of sports_girl_scene_actors_init, copied to the stack: x, y and frame of records 96..111. */
extern TaiikuH16 D_80149F00;
extern TaiikuH16 D_80149F20;
extern TaiikuB16 D_80149F40;
#endif

#ifndef OVL_TAIIKU_STACK_TABLES_T16550
#define OVL_TAIIKU_STACK_TABLES_T16550
/* Constant tables that unmatched functions copy to the stack as one struct (lw/sw runs in the
 * asm, plus lh/sh for a halfword tail). Element type from the drafts' use of the stack copy,
 * length from the copy. T-16550. */
typedef struct TaiBytes10 {
    /* 0x00 */ u8 v[10];
} TaiBytes10; /* size 0xA */
typedef struct TaiBytes23 {
    /* 0x00 */ u8 v[23];
} TaiBytes23; /* size 0x17 */
typedef struct TaiShorts12x {
    /* 0x00 */ s16 v[12];
} TaiShorts12x; /* size 0x18 */
typedef struct TaiShorts20 {
    /* 0x00 */ s16 v[20];
} TaiShorts20; /* size 0x28 */
typedef struct TaiShorts23 {
    /* 0x00 */ s16 v[23];
} TaiShorts23; /* size 0x2E */
typedef struct TaiWords6 {
    /* 0x00 */ s32 v[6];
} TaiWords6; /* size 0x18 */
typedef struct TaiRec16 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} TaiRec16; /* size 0x10 */
typedef struct TaiWords7 {
    /* 0x00 */ s32 v[7];
} TaiWords7; /* size 0x1C */
typedef struct TaiWords8 {
    /* 0x00 */ s32 v[8];
} TaiWords8; /* size 0x20 */
typedef struct TaiWords10 {
    /* 0x00 */ s32 v[10];
} TaiWords10; /* size 0x28 */
typedef struct TaiWords9 {
    /* 0x00 */ s32 v[9];
} TaiWords9; /* size 0x24 */
typedef struct TaiWords12 {
    /* 0x00 */ s32 v[12];
} TaiWords12; /* size 0x30 */
extern s32 D_8014A56C; /* sports_controller_c_resource_setup */
extern TaiWords12 D_8014A570; /* sports_controller_c_resource_setup */
extern TaiWords9 D_8014A5A0; /* sports_controller_c_resource_setup */
extern TaiWords11 D_80148EC4; /* sports_folk_dance_entry */
extern TaiBytes23 D_80148F10; /* sports_dance_actors_init */
extern TaiShorts23 D_80148FB8; /* sports_dance_actors_position */
extern TaiShorts23 D_80148F28; /* sports_dance_actors_init */
extern TaiShorts23 D_80148F58; /* sports_dance_actors_init */
extern TaiShorts23 D_80148F88; /* sports_dance_actors_init */
extern TaiBytes10 D_80148FE8; /* sports_runner_scene_actor_init */
extern TaiikuHalf10 D_80148FF4; /* sports_runner_scene_actor_init */
extern TaiikuHalf10 D_80149008; /* sports_runner_scene_actor_init */
extern TaiikuHalf10 D_8014901C; /* sports_runner_scene_actor_init */
extern s32 D_80149364; /* sports_runner_race_resource_setup */
extern TaiikuW5 D_80149368; /* sports_runner_race_resource_setup */
extern TaiWords7 D_8014937C; /* sports_runner_race_resource_setup */
extern TaiWords10 D_80149398; /* sports_runner_race_resource_setup */
extern TaiWords11 D_801494C4; /* 0x2C-byte table copied by sports_runner_race_result_apply. T-24521. */
extern TaiikuS11 sports_runner_girl_animation_group_table; /* sports_runner_race_state_init */
extern TaiShorts20 D_80149508; /* sports_runner_actor_group_init */
extern TaiShorts20 D_80149530; /* sports_runner_actor_group_init */
/* Four 16-byte runner-slot objects of sports_runner_finish_actor_slide_update (player, cpu 1..3). T-25106. */
extern s32 sports_runner_slide_frames[4];
extern s32 D_8014982C[4];
extern s32 D_8014983C[4];
extern Fix32 D_8014984C[4];
extern TkS16x4 D_8014985C; /* sports_runner_finish_line_update */
extern TaiBytes10 D_80149864; /* sports_runner_finish_actor_group_init */
extern TaiikuHalf10 D_80149870; /* sports_runner_finish_actor_group_init */
extern TaiikuHalf10 D_80149884; /* sports_runner_finish_actor_group_init */
extern TaiikuHalf10 D_80149898; /* sports_runner_finish_actor_group_init */
extern TaiWords6 D_8014A088; /* initialiser of the auto array tbl in sports_festival_entry_and_scene_select (its .data, after D_8014A070), end bound in sports_controller_c_resource_load */
extern TaiikuHalf10 D_8014A27C; /* sports_schoolyard_scene_resource_setup */
extern TaiikuHalf10 D_8014A290; /* sports_schoolyard_scene_resource_setup */
extern TaiWords8 D_8014A2A4; /* sports_schoolyard_scene_resource_setup */
extern TaiWords11 D_8014A2C4; /* sports_schoolyard_scene_resource_setup */
extern TaiShorts12 D_8014A2F0; /* sports_schoolyard_actor_display_init */
extern TaiShorts12 D_8014A304; /* sports_schoolyard_actor_display_init */
extern TkS16x4 D_8014A318; /* sports_schoolyard_finish_order_update */
extern TaiShorts12x D_8014A5C4; /* sports_controller_c_actor_display_init */
extern TaiShorts12x D_8014A5DC; /* sports_controller_c_actor_display_init */
extern TkS16x4 D_8014A5F4; /* sports_controller_c_finish_order_update */
/* T-27066: folk-dance participant syoushin at or above 0x4C takes the high-rank branch at both observed checks in folk_dance_scene.c:223,284. */
#define SPORTS_FOLK_DANCE_HIGH_SYOUSHIN_RANK_LIMIT 0x4C

/* T-27056 R2/R4 runner motion update interval. */
enum SportsRunnerMotionTiming {
    SPORTS_RUNNER_MOTION_UPDATE_FRAMES = 11 /* Elapsed update gate, then timer resets. */
};

#endif

/* Result of sports_nozomi_courtyard_scene_step: set to 1 by its last state. T-16550. */
extern s32 sports_nozomi_scene_result;
