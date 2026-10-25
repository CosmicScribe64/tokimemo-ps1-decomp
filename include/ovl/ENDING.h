#ifndef OVL_ENDING_H
#define OVL_ENDING_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_talk_render /* T-18300: ending_graduation_result_dialogue_select stores the result in talk_work (main_api.h: void), as main k_disp_start does */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"

extern u8 D_8013C3E0;
s32 talk_render(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5, void *arg6, s32 arg7);
extern u32 ending_audio_scene_tick_timer; /* compares take their own constant register: unsigned (T-9020) */
extern u8 ending_audio_scene_text_timer; /* T-20930: lbu/sb only in the original (was s8) */
void ending_graduation_dialogue_resource_table_init(void);
void ending_scene_animation_asset_table_init(void);
void ending_set_default_scene_asset_table(void);
void ending_set_song_scene_assets(void);
extern u8 D_8013BFB8[];
extern u8 D_8013BFC0[];

extern s16 ending_scene_dialogue_page_index;
extern s16 ending_scene_step_index; /* read with lh/sh only (T-8080, T-9020) */
s32 ending_scene_dialogue_step_update(s32 arg0, s32 arg1, s32 arg2);
/* T-20930: (text, kind, arg2, voices, page, step, colour). No prototype: the callee reads kind, page, step and
 * colour as u8/u16 from their argument slots, and ending_scene_dialogue_step_update passes them unconverted (K&R). */
s32 ending_dialogue_line_render();
extern u8 D_8013CA38;
extern s32 **D_8013CAF0; /* T-23002: pointer to the message pointer table (ending_graduation_result_dialogue_select) */







void ending_audio_scene_state_apply(s16 arg0, u8 arg1);

void ending_set_wide_video_geometry();


void func_80139B48();

void func_80139AA0();

void ending_shift_main_scene_sprite_y(s16 arg0);
void ending_shift_audience_actor_y(s16 arg0);
extern s16 D_8013CA1C[];

extern u8 D_8013CC48[];

void ending_set_narrow_video_geometry();
s32 ending_audio_scene_update_shiori();
void ending_audio_scene_update_mio();
void ending_audio_scene_update_yuina();
void ending_audio_scene_update_ayako();
void ending_audio_scene_update_saki();
void ending_audio_scene_update_yukari();
void ending_audio_scene_update_nozomi();
void ending_audio_scene_update_mira();
void ending_audio_scene_update_yuko();
void ending_audio_scene_update_megumi();
void ending_audio_scene_update_yumi();
void ending_audio_scene_update_rei();
void ending_audio_scene_update_default();


void ending_audience_actor_blink_update(void);
s32 ending_audio_scene_flag_normalize(void);

extern s16 D_8013CBE0;
void ending_draw_song_lyric(s16 arg0);
void ending_scene_dialogue_table_bind();
void ending_scroll_dialogue_text_entries();
s32 ending_audio_level_update();
void ending_audio_scene_character_dispatch();

extern u8 ending_audio_scene_branch_state;


/* T-9580: symbols recovered from list-8 assembly. */
/* T-11190: declarations added by tools/bulk_m2c.py. */
void ending_graduation_return_to_etc_mode(void);

/* T-18300: tables that unmatched functions copy whole to the stack (lw/sw runs); the element type comes
 * from the reads of the copy (lh with a halfword index). */
typedef struct EdShorts14 {
    s16 v[14];
} EdShorts14; /* size 0x1C */
typedef struct EdShorts18 {
    s16 v[18];
} EdShorts18; /* size 0x24 */
extern EdShorts18 ending_miharu_chance_by_level; /* ending_graduation_candidate_select: chance per level 0..0x11, compared with rand() % 256 */
extern EdShorts14 D_8013CA60; /* ending_audience_scene_initialize: x positions of the 14 sprites */
extern EdShorts14 D_8013CA7C; /* ending_audience_scene_initialize: y positions of the 14 sprites */

/* T-20930: ENDING's data tables, recovered from the matched initialisers (one store per element, in
 * address order) and the readers. All are .data with the same initial values the initialisers write. */

/* Message scripts: one row per message table, one entry per scene; a scene's three entries are copied
 * together into talk_text_table/164/168 (main) or into ending_scene_dialogue_tables. Entries are message block addresses
 * (D_8013CAC0[0][12] is read as a block whose first word is a message pointer table, ending_graduation_result_dialogue_select). */
extern s32 ending_dialogue_resource_tables[3][17]; /* size 0xCC */
extern s32 D_8013CAC0[3][15]; /* size 0xB4 */

/* The message tables of the running scene, copied as a whole to D_800CA13C/140/144 (ending_scene_dialogue_table_bind). */
typedef struct EdScriptSet {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} EdScriptSet; /* size 0x0C */
extern EdScriptSet ending_scene_dialogue_tables;

/* Scene image records: three TIM addresses and a halfword read from the TIM data (the order of
 * ending_audience_actor_pair_setup's and scene_anim_show's arguments: +0x04, +0x08, +0x00, +0x0C go to the display record's
 * unk_0C, unk_10, unk_34 and unk_14), then the addresses of the scene's texture pages. */
typedef struct EdScene30 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 pad_0E[2];
    /* 0x10 */ s32 unk_10[8];
} EdScene30; /* size 0x30 */
typedef struct EdScene44 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 pad_0E[2];
    /* 0x10 */ s32 unk_10[13];
} EdScene44; /* size 0x44 */
extern EdScene30 D_8013C5F0; /* ending_set_default_scene_asset_table */
extern EdScene44 ending_scene_asset_records[15]; /* ending_set_scene_assets_shiori .. ending_set_scene_assets_nozomi_cleared, one initialiser per record */
extern EdScene30 D_8013CBB0; /* ending_set_song_scene_assets */

/* The same four values per portrait as columns of 14 (ending_scene_animation_asset_table_init, ending_scene_actor_and_texture_setup case k reads entry
 * k + 1): unk_00 goes to the display record's unk_34, unk_38 to unk_0C, unk_70 to unk_10, unk_A8 to
 * unk_14. unk_A8 is never written (zero data) and is read with lh at a 4-byte stride. */
typedef struct EdHalf4 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ u8 pad_02[2];
} EdHalf4; /* size 0x04 */
typedef struct EdPortraitTbl {
    /* 0x00 */ s32 unk_00[14];
    /* 0x38 */ s32 unk_38[14];
    /* 0x70 */ s32 unk_70[14];
    /* 0xA8 */ EdHalf4 unk_A8[14];
} EdPortraitTbl; /* size 0xE0 */
extern EdPortraitTbl ending_scene_animation_asset_records;
extern s32 ending_scene_texture_page_addresses[7]; /* texture page addresses of ending_scene_actor_and_texture_setup (ending_scene_animation_asset_table_init) */

/* Callback tables (T-20930, collected from the matched files): a step function copies the table to the
 * stack whole and calls entry game_state.unk_110A (or unk_1109) with 0x80 (T-3330). */
typedef struct EdFnTbl6 { void (*f[6])(); } EdFnTbl6; /* size 0x18 */
typedef struct EdFnTbl9 { void (*f[9])(); } EdFnTbl9; /* size 0x24 */
typedef struct EdFnTbl13 { void (*f[13])(); } EdFnTbl13; /* size 0x34 */
typedef struct EdFnTbl14 { void (*f[14])(); } EdFnTbl14; /* size 0x38 */
typedef struct EdFnTbl16 { void (*f[16])(); } EdFnTbl16; /* size 0x40 */
typedef struct EdFnTbl25 { void (*f[25])(); } EdFnTbl25; /* size 0x64 */
typedef struct EdFnTbl43 { void (*f[43])(); } EdFnTbl43; /* size 0xAC */
extern EdFnTbl16 D_8013C370; /* ending_candidate_selection_step_dispatch */
extern EdFnTbl43 ending_companion_scene_step_callbacks; /* ending_companion_scene_step_dispatch */
extern EdFnTbl9 ending_secondary_scene_step_callbacks; /* ending_secondary_scene_step_dispatch */
extern EdFnTbl13 ending_scene_effect_step_callbacks; /* ending_scene_effect_step_dispatch */
extern EdFnTbl9 ending_audience_scene_step_callbacks; /* ending_audience_scene_step_dispatch */
void ending_audience_scene_state_init(void);
void ending_audience_scene_initialize(void);
void ending_audience_scene_animation_update(void);
void ending_girl_scene_data_load_dispatch(void);
void ending_audience_scene_data_load(void);
extern EdFnTbl6 ending_audio_scene_step_callbacks; /* ending_audio_scene_step_dispatch */
/* Entries of the scene table ending_graduation_scene_step_callbacks (801396E0.c). */
void ending_yoshio_mode_stage_select(void);
void ending_graduation_scene_data_load(void);
void ending_graduation_scene_screen_init(void);
void ending_candidate_scene_data_load(void);
s32 ending_validate_background_stream_header(void);
void ending_scene_fade_in_update(void);
void ending_scene_fade_out_update(void);
void ending_graduation_sprite_layout_init(void);
void ending_graduation_result_dialogue_select(void);
s32 ending_result_text_render_wait(void);
s32 ending_result_text_entries_clear(void); /* FAKE (T-23002; prototype moved here from the guard by T-25127): s32, see its definition; ending_graduation_result_dialogue_select reads its $v0 */
extern EdFnTbl14 ending_graduation_scene_step_callbacks; /* ending_career_summary_step_dispatch */
extern EdFnTbl25 D_8013CBE4; /* ending_song_scene_step_dispatch */

/* Small tables copied to the stack whole (8013B6A0.c). */
typedef struct EdWords4 {
    s32 w[4];
} EdWords4; /* size 0x10 */
typedef struct EdShorts13 {
    s16 h[13];
} EdShorts13; /* size 0x1A */
extern EdWords4 D_8013CD90; /* ending_song_title_audio_start: two pairs of cd_audio_request_set arguments */
extern EdShorts13 D_8013CDA0; /* ending_advance_song_lyrics: thresholds compared with D_8013CBE0 */
#endif /* OVL_ENDING_H */

#ifndef OVL_ENDING_TIMER_T9780
#define OVL_ENDING_TIMER_T9780
s32 ending_scroll_result_text_entries(void);
/* T-27066: ending_graduation_result_dialogue_select initializes the best result to 0 and uses 0x78 to separate high from lower parameter-result dialogue; career_and_epilogue_summary.c:314,408. */
#define ENDING_PARAM_RESULT_INITIAL_BEST 0
#define ENDING_PARAM_RESULT_HIGH_MIN 0x78

/* T-27075 group 1: ENDING/career_and_epilogue_summary.c:326-442; bits 0-5 select ordered university/company result labels. Bits 6/7 remain indexed because broader meaning is unproven. */
#define ENDING_CAREER_RESULT_UNIVERSITY_TIER_1_MASK 0x01 /* b[1] bit 0 selects the first university-result label in ordered source branches. */
#define ENDING_CAREER_RESULT_UNIVERSITY_TIER_2_MASK 0x02 /* b[1] bit 1 selects the second university-result label in ordered source branches. */
#define ENDING_CAREER_RESULT_UNIVERSITY_TIER_3_MASK 0x04 /* b[1] bit 2 selects the third university-result label in ordered source branches. */
#define ENDING_CAREER_RESULT_COMPANY_TIER_1_MASK 0x08 /* b[1] bit 3 selects the first company-result label in ordered source branches. */
#define ENDING_CAREER_RESULT_COMPANY_TIER_2_MASK 0x10 /* b[1] bit 4 selects the second company-result label in ordered source branches. */
#define ENDING_CAREER_RESULT_COMPANY_TIER_3_MASK 0x20 /* b[1] bit 5 selects the third company-result label in ordered source branches. */
#define ENDING_CAREER_RESULT_FLAG_BIT_6_MASK 0x40 /* b[1] bit 6 is tested in later career-result branches; broader meaning not inferred. */
#define ENDING_CAREER_RESULT_FLAG_BIT_7_MASK 0x80 /* b[1] bit 7 is tested in later career-result branches; broader meaning not inferred. */

#endif
