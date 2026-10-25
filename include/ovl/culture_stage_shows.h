#ifndef OVL_CULTURE_STAGE_SHOWS_H
#define OVL_CULTURE_STAGE_SHOWS_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_set_kanji_string /* matched with s32() (main_api.h: void(s32,s32,s32,void*,s32)) */

#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/debug.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/import_views/school_events.h"
#include "main/message.h"
#include "main/prim.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/debug.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/import_views/school_events.h"
#include "main/message.h"
#include "main/prim.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"

s32 set_kanji_string();

/* T-12090: dispatcher and its later definitions in 80132000.c. */
void stage_show_mode_step_dispatch(void);
void stage_show_actor_and_event_state_init(void);
void stage_show_frame_wait(void);
void stage_show_frame_state_wait(void);
void stage_show_timed_step_wait(u32 wait_frames);
void stage_show_selector_prompt_wait(void);

/* BUNKA_SD functions called across the overlay. */
void stage_show_scene_a_dispatch(void);
void stage_show_scene_b_dispatch(void);
void stage_show_scene_c_dispatch(void);
void stage_show_input_update(void);
void stage_show_scene_a_input_update(void);
void stage_show_drama_first_show_event_list_init(void);
void stage_show_drama_first_show_actor_slot_clear(s32 text);
void stage_show_computer_show_input_update(void);
void stage_show_science_first_show_input_update(void);
void stage_show_scene_b_input_update(void);
void stage_show_scene_c_input_update(void);
void stage_show_drama_second_event_text_and_audio_display(void);
void stage_show_drama_second_kanji_line_start(s32 text);
void stage_show_brass_band_input_update(void);
void stage_show_actor_scene_init(void);
void stage_show_computer_show_actor_texture_setup(void);
void stage_show_science_second_show_actor_texture_setup(void);
void stage_show_science_final_show_actor_texture_setup(void);
void stage_show_science_first_show_actor_table_initialize(void);
void stage_show_drama_second_actor_slot_state_set(s32 actor_index, s32 animation_frame, s32 set_draw_flag);
s32 stage_show_picture_index_set(void);

void stage_show_computer_show_timed_audio_commands(void);
s32 stage_show_computer_show_frame_and_fade_update(void);
void stage_show_scene_a_actor_init(void);
void stage_show_drama_final_show_actor_table_initialize(void);
void stage_show_drama_second_show_actor_table_initialize(void);
void stage_show_drama_final_actor_slot_state_set(s32 actor_index, s32 animation_frame, s32 set_draw_flag);

void stage_show_science_final_show_timed_audio_commands(void);
void stage_show_science_final_show_frame_state_update(void);
void stage_show_brass_band_show_actor_picture_setup(void);
void stage_show_brass_band_mode_step_dispatch(void);
void stage_show_drama_first_show_actor_slot_setup(s32 actor_index, s32 animation_frame, s32 set_draw_flag);
void stage_show_science_first_show_timed_audio_commands(void);
void stage_show_science_first_show_initial_frame_state(void);
void stage_show_drama_final_event_text_and_audio_display(void);
void stage_show_drama_final_kanji_line_start(s32 text);
extern s32 stage_show_saved_text_speed;
extern s32 stage_show_scene_b_event_cursor, stage_show_scene_b_script_index, stage_show_scene_b_frame, stage_show_scene_b_interpreter_state;

extern s32 stage_show_text_line_index;
extern s32 stage_show_scene_a_text_cursor, stage_show_scene_a_event_cursor, stage_show_scene_a_script_index, stage_show_scene_a_frame, stage_show_scene_a_interpreter_state, stage_show_scene_a_text_active;
extern s32 stage_show_final_text_line_index, stage_show_scene_b_text_active;
extern s32 stage_show_scene_c_text_cursor, stage_show_scene_c_event_cursor, stage_show_scene_c_script_index, stage_show_scene_c_frame, stage_show_scene_c_interpreter_state, stage_show_scene_c_text_active;

void stage_show_scene_state_reset();
void stage_show_actor_brightness_ramp_up();
s32 stage_show_brass_band_sector_and_track_setup();
void stage_show_brass_band_show_playback_tick();
void stage_show_brass_band_actor_brightness_fade();
s32 stage_show_brass_band_audio_transition(void);
void stage_show_brass_band_cd_audio_end_wait(void);
s32 stage_show_science_second_show_sector_load(void);
s32 stage_show_science_final_show_sector_load(void);
s32 stage_show_science_first_show_sector_load(void);

extern s32 stage_show_first_sound_delay_frames;

extern s32 stage_show_second_sound_delay_frames;

extern u8 D_8013B810[];
extern u8 D_8013B938[];
extern u8 *stage_show_scene_a_text_data;
extern u8 *stage_show_scene_a_voice_data;
extern u8 D_8013C750[];
extern u8 D_8013C7B4[];
extern u8 *stage_show_final_text_table;
extern u8 *stage_show_final_voice_ranges;
s32 stage_show_scene_b_data_load(void);
extern u8 D_8013CEF0[];
extern u8 D_8013CFC8[];
extern u8 *stage_show_scene_c_text_data;
extern u8 *stage_show_scene_c_voice_data;
s32 stage_show_scene_c_data_load(void);
void stage_show_computer_show_mode_step_dispatch(void);
void stage_show_science_final_show_mode_step_dispatch(void);
void stage_show_science_final_show_input_update(void);
void stage_show_science_second_show_mode_step_dispatch(void);
void stage_show_science_second_show_input_update(void);

/* T-9730: address-only script data selected by the background dispatcher. */
extern u8 D_8013B670[];
extern u8 D_8013B6B4[];
extern u8 D_8013B6E4[];
extern u8 D_8013B71C[];
extern u8 D_8013B74C[];
extern u8 D_8013B794[];
extern u8 D_8013B7B0[];
extern u8 D_8013B7C8[];
extern u8 D_8013B7DC[];
extern u8 *stage_show_selected_text_table;
extern u8 *stage_show_selected_voice_ranges;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void stage_show_computer_show_audio_start(void);
void stage_show_computer_show_animation_tick(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void stage_show_science_final_show_audio_start(void);
void stage_show_science_final_show_animation_tick(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void stage_show_science_second_show_audio_start(void);
void stage_show_science_second_show_animation_tick(void);
s32 stage_show_science_second_show_display_state_update(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void stage_show_scene_a_state_init(void);
void stage_show_scene_a_actor_set(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void stage_show_drama_second_show_state_initialize(void);
void stage_show_drama_second_timer_gate(u32 wait_frames);
void stage_show_drama_second_event_actor_set_a(void);
void stage_show_drama_second_event_actor_set_b(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void stage_show_drama_final_show_state_initialize(void);
void stage_show_drama_final_event_actor_set_a(void);
void stage_show_drama_final_event_list_step(void);
void stage_show_drama_final_timer_gate(u32 wait_frames);
void stage_show_drama_final_event_actor_clear_and_set(void);
void stage_show_drama_final_event_actor_set_b(void);
void stage_show_science_first_show_audio_start(void);
void stage_show_science_first_show_animation_tick(void);
void stage_show_science_first_show_mode_step_dispatch(void);

/* T-12090: typed display setup data and pending dispatcher callees. */
void stage_show_brass_band_animation_frame_select(void);
void stage_show_speaker_label_wait(void);
void stage_show_script_line_render(void);
extern u8 *D_8013B7F8;
extern u8 *D_8013B7FC;
extern u8 *D_8013B800;
extern u8 *D_8013B804;
extern u8 *D_8013D9C0;
extern u8 *D_8013D9C4;
extern u8 *D_8013D9C8;
extern u8 *D_8013D9CC;

/* T-18100: scene scripts. The three scenes (A: 80133540.c, B: 801369D0.c, C: 80137B40.c) each
 * keep a table of event lists, one list per script, and a table of the text each script opens with
 * (passed to stage_show_drama_first_show_actor_slot_clear / stage_show_drama_final_kanji_line_start / stage_show_drama_second_kanji_line_start as a word). An event fires when its
 * unk_00 equals the scene's frame counter (stage_show_scene_a_frame, stage_show_scene_b_frame, stage_show_scene_c_frame). */
typedef struct BsdEvent {
    /* 0x00 */ s32 frame; /* frame */
    /* 0x04 */ s32 command_word; /* 0x0FFFFFFF ends the list; bits 16..23 display slot, bits 0..7 image
                            * (0xFE hides the slot), bits 28..31 start the script's text */
} BsdEvent; /* size 0x08 */

extern s32 stage_show_scene_a_text_script_addresses[29];       /* scene A text */
extern BsdEvent *stage_show_scene_a_event_lists[29]; /* scene A event lists */
extern s32 stage_show_scene_b_text_script_addresses[12];       /* scene B text */
extern BsdEvent *stage_show_scene_b_event_lists[12]; /* scene B event lists */
extern s32 stage_show_scene_c_text_script_addresses[31];       /* scene C text */
extern BsdEvent *stage_show_scene_c_event_lists[31]; /* scene C event lists */

/* T-18100: loaded picture sets, four words each, set in .data to fixed addresses of the loaded
 * scene file: the texture-page table handed to load_tpage_buf_lock, two image pointers stored in
 * display records +0x0C/+0x10, and the palette for load_palette. Separate objects: matched code
 * reaches each word through its own symbol. */
extern s32 *D_8013C6E8; /* scene A */
extern u8 *D_8013C6EC;
extern u8 *D_8013C6F0;
extern s32 D_8013C6F4;
extern s32 *D_8013C710;
extern u8 *D_8013C714;
extern u8 *D_8013C718;
extern s32 D_8013C71C;
extern s32 *D_8013C720;
extern u8 *D_8013C724;
extern u8 *D_8013C728;
extern s32 D_8013C72C;
extern s32 *D_8013C730;
extern u8 *D_8013C734;
extern u8 *D_8013C738;
extern s32 D_8013C73C;
extern s32 *D_8013C740;
extern u8 *D_8013C744;
extern u8 *D_8013C748;
extern s32 D_8013C74C;
extern s32 *D_8013CEDC; /* scene B */
extern u8 *D_8013CEE0;
extern u8 *D_8013CEE4;
extern s32 D_8013CEE8;
extern s32 *D_8013D9A8; /* scene C */
extern u8 *D_8013D9AC;
extern u8 *D_8013D9B0;
extern s32 D_8013D9B4;

/* T-27066: stage_show_computer_show_animation_tick increments local t and advances when it reaches 0x818; computer_show.c:90-100. */
#define STAGE_SHOW_COMPUTER_ANIMATION_END_TICK 0x818

#endif /* OVL_CULTURE_STAGE_SHOWS_H */
