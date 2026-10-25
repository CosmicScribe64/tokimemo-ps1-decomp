#ifndef OVL_KANGEI_H
#define OVL_KANGEI_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_portrait_layout_set /* matched with void(s32) (main_api.h: void(u8)) */
#define MAIN_API_OVERRIDE_D_800B3688 /* matched as s32 (main_api.h: s32[]) */
#define MAIN_API_OVERRIDE_D_800B36C8 /* matched as s32 (main_api.h: s32[]) */
#define MAIN_API_OVERRIDE_D_800B3708 /* matched as s32 (main_api.h: s32[]) */

#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game/schedule_records.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/date.h"
#include "main/day_event.h"
#include "main/draw.h"
#include "main/event_log.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/minigame_scenes_school_events.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/message.h"
#include "main/param.h"
#include "main/portrait.h"
#include "main/prim.h"
#include "main/scene.h"
#include "main/schedule.h"
#include "main/school_day.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"

/* KANGEI overlay (load address 0x80132000): externs and types. */

extern s8 D_80139AE4;
extern s8 D_80139AC0;
extern s8 D_8013A2A8;
extern s16 D_80139ADC;
void camp_current_girl_restore(void);
/* Entries of the 80132090.c scene table D_801399B0 (defined in that file, T-25139). */
void camp_current_girl_portrait_layout(void);
void camp_scene_callback_current_girl_audio_bank_load_start(void);
void camp_scene_callback_audio_bank_poll_retry(void);
void camp_audio_transition_command(void);
void camp_dialogue_sector_load(void);
void camp_scene_ui_reset(void);
void camp_event_log_and_talk_setup(void);
void camp_club_variant_current_girl_portrait_read(void);
/* Entries of the 80134120.c scene table D_80139B30 (defined in that file, T-25150). */
void camp_audio_bank_retry(void);
void camp_audio_load_poll_retry_alt(void);
void camp_audio_bank_start(void);
void camp_dialogue_data_sector_load(void);
void camp_scene_data_sector_load(void);
void camp_girl_meeting_audio_bank_load_start(void);
void camp_girl_meeting_audio_bank_poll_retry(void);
void camp_candidate_portrait_read(void);
void camp_scene_text_index_select(void);
void camp_club_location_label_select(void);

extern s8 D_80139AC4;
extern s32 D_80139DFC;
extern s16 D_801D4094;
extern s8 D_80139DF0;
typedef struct KSub {
    /* 0x00 */ u8 unk_00[8];
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} KSub; /* size unknown, only 0x00-0x0F used */

typedef struct KObj {
    /* 0x00 */ u8 unk_00[0x34];
    /* 0x34 */ KSub *unk_34;
} KObj; /* size unknown, only 0x00-0x37 used */

extern KObj *D_80139AD0;
extern s32 D_80139AD4;
extern s32 D_80139AD8;

extern s32 D_801398B0;
extern s32 D_801398E0;
void camp_scene_callback_dialogue_pointer_table_setup(void);
void camp_current_girl_dialogue_pointer_bind(void);
void camp_day_phase_dialogue_pointer_table_setup(void);
void camp_scene_animation_picture_pointer_bind(void);
void camp_script_pointer_table_bind(void);
void camp_script_pointer_table_bind_alt(void);

extern s16 D_80139AE0;
extern u8 camp_saved_girl;
extern s32 D_80139DB0;
extern s32 D_80139DB4;
extern s32 D_80139DB8;
extern s32 D_80139DBC;
extern s32 D_80139DC0;
extern s32 D_80139DC4;
extern s32 D_80139DC8;
extern s8 D_80139AC8;
s32 camp_day_event_parameter_update(void);
void camp_opening_talk_run(void);
void camp_current_girl_scene_pointer_bind(void);
void camp_current_girl_script_pointer_bind(void);

extern s32 D_8013980C;
void camp_girl_met_flag_set(void);

extern s32 D_801398E8;
extern s32 D_801398EC;
extern s32 D_801398F0;
extern s32 D_801398F4;
void camp_script_text_copy_from_selector(void);
void portrait_layout_set(s32 arg0); /* overlay view; main_only.h has the u8 view */
extern s32 D_80139DCC;
extern s32 D_80139DD0;
extern s32 D_80139DD4;
extern s32 D_80139DD8;
extern s32 D_80139DDC;
extern s32 D_80139DE0;
extern s16 D_80139DE4;
extern s16 D_80139DE8;
extern s32 D_80139DEC;
void camp_mode_step_callback_dispatch();

void camp_ayako_actor_resource_record_bind();

void camp_yumi_actor_resource_record_bind();

extern s32 D_80139E10;

extern s32 D_800B36C8;
extern s32 D_800B3708;
extern s32 D_800B3688;

void camp_date_girl_scene_pointer_setup();

/* defined in C in one object, called from another (T-0500) */
void camp_background_pointer_setup(void);

void camp_candidate_scene_initialize(void);

void camp_scene_actor_and_particle_update(void);
extern u8 D_80139DF4[];
extern u8 D_80139E0C;
void camp_scene_talk_and_audio_tick(void);
void camp_scene_picture_transition_animate();
void camp_scene_background_fade_out(void);
void camp_scene_audio_bank_poll_retry(void);
void camp_scene_background_fade_in(void);
void camp_player_birthday_callback_dispatch(void);
s32 camp_final_audio_load_complete(void);
extern u8 D_8013A2AC;
void camp_scene_callback_dispatch(void);

void camp_script_row_one_pointer_bind(void);
void camp_unmet_girl_callback_dispatch(void);

typedef struct KangeiWords11 {
    /* 0x00 */ s32 unk_00[11];
} KangeiWords11; /* size 0x2C */
typedef struct KangeiWords2 {
    /* 0x00 */ s32 unk_00[2];
} KangeiWords2; /* size 0x08 */
/* T-18400: picture sets. Each set is three pointers into loaded data (D_80139AD0, D_80139AD4,
 * D_80139AD8 get the three components of the current one); the tables hold one row per component. */
extern s32 D_80139950[3][8]; /* size 0x60, filled by camp_scene_callback_dialogue_pointer_table_setup */
extern s32 D_80139A50[3]; /* size 0x0C, filled by camp_background_pointer_setup */
extern s32 D_80139A5C[3][8]; /* size 0x60, filled by camp_background_pointer_setup */
extern s32 D_8013A170[2][3][13]; /* size 0x138, filled by camp_script_pointer_table_bind */
extern KangeiWords11 D_80139BA8;
extern KangeiWords2 D_80139BD4;
extern KangeiWords2 D_80139BDC;
extern KangeiWords2 D_80139BE4;
/* T-18400: the same four tables for camp_club_variant_current_girl_portrait_read (summer variant of camp_candidate_portrait_read). */
extern KangeiWords11 D_80139A08;
extern KangeiWords2 D_80139A34;
extern KangeiWords2 D_80139A3C;
extern KangeiWords2 D_80139A44;
extern s8 D_80139ABC;

/* T-9580: bits 25..26 of the packed +0x66C record word. */
typedef struct KangeiModeFlags {
    u32 low : 25;
    u32 mode : 2;
    u32 high : 5;
} KangeiModeFlags; /* size 0x04 */

/* T-9580: overlay view keeps the packed records at their GameState offset;
 * T-24122 adds the 4-bit club nibble at +0x0F4 (bits 12..15) for camp_scene_text_index_select
 * and 32-bit fixed-point stat words at +0x100..+0x118 for camp_day_event_parameter_update. */
typedef struct KangeiStateView {
    /* 0x000 */ u8 pad_000[0x0F4];
    /* 0x0F4 */ u16 club_low : 12;
    /* 0x0F4 */ u16 club : 4;
    /* 0x0F6 */ u8 pad_0F6[0x00A];
    /* 0x100 */ s32 param_liberal_arts_word; /* 32-bit fixed-point view of GameState.param_liberal_arts. */
    /* 0x104 */ s32 param_science_word; /* 32-bit fixed-point view of GameState.param_science. */
    /* 0x108 */ s32 param_art_word; /* 32-bit fixed-point view of GameState.param_art. */
    /* 0x10C */ s32 param_athletics_word; /* 32-bit fixed-point view of GameState.param_athletics. */
    /* 0x110 */ s32 unk_110;
    /* 0x114 */ s32 unk_114;
    /* 0x118 */ s32 param_guts_word; /* 32-bit fixed-point view of GameState.param_guts. */
    /* 0x11C */ u8 pad_11C[0x550];
    /* 0x66C */ KangeiModeFlags modes[12];
} KangeiStateView; /* size 0x69C */

/* T-18400: overlay callees of the unmatched functions, called before their definitions. */
void camp_calendar_day_advance(void);
void camp_entry_state_initialize(void);
void camp_audio_load_poll_retry(void);
s32 camp_day_event_complete(void);
void camp_mode_step_timer_update(void);
void camp_mode_step_scene_phase_callback_dispatch(void);
void camp_current_girl_scene_gate(void);
s32 camp_paired_actor_step_dispatch(void);
void camp_scene_picture_callback_dispatch(void);
void camp_scene_result_callback_dispatch(void);
void camp_birthday_scene_gate(void);
void camp_transition_quad_primitive_build(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8);
/* T-18400: club name tables that camp_club_scene_actor_setup copies to the stack whole and passes
 * name[game_state.unk_0F4.h >> 12] to strcpy (as EVENT's EventTableE7, T-9940). */
typedef struct KangeiNames20 {
    /* 0x00 */ u8 name[11][20];
} KangeiNames20; /* size 0xDC */
typedef struct KangeiNames21 {
    /* 0x00 */ u8 name[11][21];
} KangeiNames21; /* size 0xE7; the inline copy ends with a three-byte tail */
extern KangeiNames20 D_80139FAC;
extern KangeiNames21 D_8013A088;
/* T-18400: two image records copied into display record 96 by camp_special_actor_scene_setup
 * (unk_00 to +0x34, unk_04 to +0x0C, unk_08 to +0x10, unk_0C to +0x14); filled by camp_yumi_actor_resource_record_bind. */
typedef struct KangeiImage10 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 pad_0E[2];
} KangeiImage10; /* size 0x10 */
extern KangeiImage10 D_80139D10[2];
/* T-18400: fade colour of camp_scene_picture_transition_animate (byte loads and stores, one word apart). */
extern u8 D_80139E00;
extern u8 D_80139E04;
extern u8 D_80139E08;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void camp_birthday_callback_dispatch(void);

#endif /* OVL_KANGEI_H */
extern u8 D_80139DF8;
void camp_shiori_scene_entry_setup(void);

#ifndef OVL_KANGEI_SPRITE_T9740
#define OVL_KANGEI_SPRITE_T9740


/* T-18400: the ten falling streaks of camp_scene_actor_and_particle_update (MASTER's D_8013C58C/D_8013C5B4). */
typedef struct KangeiPoint2 {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} KangeiPoint2; /* size 0x04 */
extern KangeiPoint2 D_80139F10[10];
extern s16 D_80139F38[10];
void camp_scene_actor_effect_fade_in(void);
#endif

/* Integer definition retains the original fade-out result register (T-9740). */
s32 camp_scene_actor_effect_fade_out(void);

#ifndef OVL_KANGEI_DISPATCH_T10030
#define OVL_KANGEI_DISPATCH_T10030
typedef struct KangeiDispatchTable80 {
    /* 0x00 */ void (*functions[32])(s32);
} KangeiDispatchTable80; /* size 0x80; T-10030 */
extern KangeiDispatchTable80 D_80139D30;
void camp_current_girl_fixed_portrait_read(void);

#endif
