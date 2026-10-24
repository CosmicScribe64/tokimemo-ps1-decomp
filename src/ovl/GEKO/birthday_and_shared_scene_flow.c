/* birthday_and_shared_scene_flow.c: src/ovl/GEKO/80134D10.c:375,417,755,1347,1587; girl-birthday logic plus birthday/visit/scene script binders, shared girl-scene entry/talk/load/finish and effects. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80134D10-0x801393C0 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_portrait_layout_set /* matched with void(s32) (main_api.h: void(u8)) */
#define MAIN_API_OVERRIDE_strcpy /* matched with strcpy returning its destination: with the void prototype case 0 of the switch in encounter_girl_birthday_init takes v0 for the encounter_girl_birthday_text_section_1 value, not v1 (main_api.h: void). T-11900 */
#include "common.h"
#include "ovl/GEKO.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_10);
s32 encounter_girl_birthday_text_section_11 = 0x801B9DA0;
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_text_section_12);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_10);
s32 encounter_girl_birthday_command_section_11 = 0x801B9EA8;
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_command_section_12);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_10);
s32 encounter_girl_birthday_voice_section_11 = 0x801BA2C8;
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_voice_section_12);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_10);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_11);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_text_section_12);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_10);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_11);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_command_section_12);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_10);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_11);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_player_birthday_voice_section_12);
s32 encounter_girl_birthday_text_table = 0;
s32 encounter_girl_birthday_command_table = 0;
s32 encounter_girl_birthday_voice_table = 0;
s32 encounter_girl_birthday_saved_text_table = 0;
s32 encounter_girl_birthday_saved_command_table = 0;
s32 encounter_girl_birthday_saved_voice_table = 0;
s16 encounter_girl_birthday_group = 0;
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_line);
s16 encounter_girl_birthday_saved_group = 0;
u8 encounter_girl_birthday_school_year = 0;
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_portrait_active);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_background_variant);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_phone_phase_counter);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_scene_frame);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_phone_prelude_timer);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_phone_ring_limit);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_holiday_context);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_zero_choice);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_club_context);
/* Scene table, indexed by game_state.mode_step in encounter_girl_birthday_dispatch. Defined in C as the original did:
 * as1 treats every function named in this file's data as an address-taken label, so each
 * jump-table dispatch here gets an edge back to the function entries (T-25150). */
typedef struct GekoFnTable73 { /* 0x00 */ void (*f[73])(); } GekoFnTable73; /* size 0x124 */
GekoFnTable73 encounter_girl_birthday_steps = { {
    encounter_girl_birthday_init, encounter_girl_birthday_audio_load, encounter_girl_birthday_audio_wait, encounter_girl_birthday_scripts_load, func_8007C784,
    encounter_girl_birthday_opening_skip, mode_step_next, mode_step_next, mode_step_next, mode_step_next,
    mode_step_next, encounter_girl_birthday_context_skip, mode_step_next, encounter_girl_birthday_choice_talk, normal_date_two_select,
    encounter_girl_birthday_first_choice_apply, encounter_girl_birthday_talk, encounter_girl_birthday_reply_script_push, encounter_girl_birthday_talk, normal_date_three_select,
    encounter_girl_birthday_reply_script_pop, encounter_girl_birthday_talk, encounter_girl_birthday_appointment_check, encounter_girl_birthday_talk, encounter_girl_birthday_phone_gate_check,
    encounter_girl_birthday_phone_talk, encounter_girl_birthday_girl_state_group_offset, encounter_girl_birthday_talk_with_audio_command, encounter_girl_birthday_scene_init, encounter_girl_birthday_location_select,
    func_8007C784, dec_init, change_dec_bg, encounter_girl_birthday_portrait_read, func_8007C784,
    portrait_show_checked, func_8007C8A4, place_init, encounter_girl_birthday_holiday_girl_in, encounter_girl_birthday_talk,
    encounter_girl_birthday_reply_group_select, encounter_girl_birthday_school_girl_in, encounter_girl_birthday_school_talk, encounter_girl_birthday_talk, encounter_girl_birthday_choice_group_select,
    encounter_girl_birthday_reply_apply, encounter_girl_birthday_talk, encounter_girl_birthday_player_birthday_check, normal_date_girl_out, encounter_girl_birthday_talk,
    encounter_girl_birthday_background_out, encounter_girl_birthday_finish, encounter_girl_birthday_visit_scripts_load, func_8007C784, func_80083338,
    encounter_girl_birthday_player_birthday_script_select, encounter_girl_birthday_talk, normal_date_girl_out, encounter_girl_birthday_talk, normal_date_bg_out,
    encounter_girl_birthday_finish, encounter_girl_birthday_talk, encounter_girl_birthday_phone_gate_check, encounter_girl_birthday_phone_ring_wait, encounter_girl_birthday_phone_closing_group,
    encounter_girl_birthday_talk, encounter_girl_birthday_finish, encounter_girl_birthday_bg_request, func_8007C784, encounter_girl_birthday_bg_decode,
    encounter_girl_birthday_bg_show, encounter_girl_birthday_restart, game_mode_reset_to_title
} };
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_shiori_club_portraits);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_mio_club_portraits);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_yuina_club_portraits);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_ayako_club_portraits);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_girl_birthday_reply_names);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_10);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_11);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_12);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_13);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_14);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_15);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_16);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_17);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_18);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_19);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_20);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_21);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_22);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_23);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_24);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_25);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_26);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_27);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_28);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_29);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_30);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_31);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_32);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_33);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_34);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_35);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_36);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_37);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_38);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_39);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_40);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_41);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_42);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_43);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_44);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_45);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_46);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_47);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_48);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_49);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_50);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_51);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_52);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_53);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_54);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_55);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_56);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_57);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_58);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_59);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_60);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_61);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_62);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_63);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_64);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_65);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_66);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_67);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_68);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_69);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_70);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_71);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_72);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_73);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_74);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_75);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_76);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_77);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_text_section_78);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_10);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_11);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_12);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_13);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_14);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_15);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_16);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_17);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_18);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_19);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_20);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_21);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_22);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_23);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_24);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_25);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_26);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_27);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_28);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_29);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_30);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_31);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_32);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_33);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_34);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_35);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_36);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_37);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_38);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_39);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_40);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_41);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_42);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_43);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_44);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_45);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_46);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_47);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_48);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_49);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_50);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_51);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_52);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_53);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_54);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_55);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_56);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_57);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_58);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_59);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_60);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_61);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_62);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_63);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_64);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_65);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_66);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_67);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_68);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_69);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_70);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_71);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_72);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_73);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_74);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_75);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_76);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_77);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_command_section_78);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_0);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_1);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_2);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_3);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_4);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_5);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_6);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_7);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_8);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_9);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_10);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_11);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_12);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_13);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_14);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_15);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_16);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_17);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_18);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_19);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_20);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_21);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_22);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_23);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_24);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_25);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_26);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_27);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_28);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_29);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_30);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_31);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_32);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_33);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_34);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_35);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_36);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_37);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_38);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_39);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_40);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_41);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_42);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_43);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_44);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_45);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_46);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_47);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_48);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_49);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_50);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_51);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_52);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_53);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_54);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_55);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_56);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_57);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_58);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_59);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_60);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_61);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_62);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_63);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_64);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_65);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_66);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_67);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_68);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_69);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_70);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_71);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_72);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_73);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_74);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_75);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_76);
INCLUDE_RODATA("asm/ovl/GEKO/data/GEKO/birthday_and_shared_scene_flow.data", encounter_scene_voice_section_77);

void portrait_layout_set(s32 arg0);
void *strcpy(void *dst, void *src);

typedef struct {
    u8 s[0xCC3];
} DateTxt; /* size 0xCC3 */
extern DateTxt encounter_girl_birthday_reply_names;

void encounter_girl_birthday_scripts_bind(void) {
    encounter_girl_birthday_text_section_0 = 0x801B0478;
    encounter_girl_birthday_text_section_1 = 0x801B13A4;
    encounter_girl_birthday_text_section_2 = 0x801B2998;
    encounter_girl_birthday_text_section_3 = 0x801B38C8;
    encounter_girl_birthday_text_section_4 = 0x801B4258;
    encounter_girl_birthday_text_section_5 = 0x801B4B98;
    encounter_girl_birthday_text_section_6 = 0x801B5574;
    encounter_girl_birthday_text_section_7 = 0x801B6560;
    encounter_girl_birthday_text_section_8 = 0x801B7B74;
    encounter_girl_birthday_text_section_9 = 0x801B8B18;
    encounter_girl_birthday_text_section_10 = 0x801B93D0;
    encounter_girl_birthday_text_section_11 = 0x801B9DA0;
    encounter_girl_birthday_text_section_12 = 0x801BA780;
    encounter_girl_birthday_command_section_0 = 0x801B0510;
    encounter_girl_birthday_command_section_1 = 0x801B14E0;
    encounter_girl_birthday_command_section_2 = 0x801B2AD0;
    encounter_girl_birthday_command_section_3 = 0x801B3960;
    encounter_girl_birthday_command_section_4 = 0x801B42F0;
    encounter_girl_birthday_command_section_5 = 0x801B4C38;
    encounter_girl_birthday_command_section_6 = 0x801B560C;
    encounter_girl_birthday_command_section_7 = 0x801B66A0;
    encounter_girl_birthday_command_section_8 = 0x801B7CB4;
    encounter_girl_birthday_command_section_9 = 0x801B8BB0;
    encounter_girl_birthday_command_section_10 = 0x801B9468;
    encounter_girl_birthday_command_section_11 = 0x801B9EA8;
    encounter_girl_birthday_command_section_12 = 0x801BA818;
    encounter_girl_birthday_voice_section_0 = 0x801B0888;
    encounter_girl_birthday_voice_section_1 = 0x801B1E14;
    encounter_girl_birthday_voice_section_2 = 0x801B33B0;
    encounter_girl_birthday_voice_section_3 = 0x801B3CF4;
    encounter_girl_birthday_voice_section_4 = 0x801B4684;
    encounter_girl_birthday_voice_section_5 = 0x801B4FEC;
    encounter_girl_birthday_voice_section_6 = 0x801B59F4;
    encounter_girl_birthday_voice_section_7 = 0x801B6FB8;
    encounter_girl_birthday_voice_section_8 = 0x801B85CC;
    encounter_girl_birthday_voice_section_9 = 0x801B8F28;
    encounter_girl_birthday_voice_section_10 = 0x801B97E0;
    encounter_girl_birthday_voice_section_11 = 0x801BA2C8;
    encounter_girl_birthday_voice_section_12 = 0x801BAB90;
}

void encounter_player_birthday_scripts_bind(void) {
    encounter_player_birthday_text_section_0 = 0x801B00DC;
    encounter_player_birthday_text_section_1 = 0x801B02AC;
    encounter_player_birthday_text_section_2 = 0x801B03C8;
    encounter_player_birthday_text_section_3 = 0x801B04C4;
    encounter_player_birthday_text_section_4 = 0x801B0668;
    encounter_player_birthday_text_section_5 = 0x801B0808;
    encounter_player_birthday_text_section_6 = 0x801B09DC;
    encounter_player_birthday_text_section_7 = 0x801B0BE8;
    encounter_player_birthday_text_section_8 = 0x801B0D8C;
    encounter_player_birthday_text_section_9 = 0x801B0F5C;
    encounter_player_birthday_text_section_10 = 0x801B1164;
    encounter_player_birthday_text_section_11 = 0x801B12D8;
    encounter_player_birthday_text_section_12 = 0x801B1454;
    encounter_player_birthday_command_section_0 = 0x801B00F4;
    encounter_player_birthday_command_section_1 = 0x801B02C8;
    encounter_player_birthday_command_section_2 = 0x801B03D0;
    encounter_player_birthday_command_section_3 = 0x801B04DC;
    encounter_player_birthday_command_section_4 = 0x801B0680;
    encounter_player_birthday_command_section_5 = 0x801B0820;
    encounter_player_birthday_command_section_6 = 0x801B09F8;
    encounter_player_birthday_command_section_7 = 0x801B0C00;
    encounter_player_birthday_command_section_8 = 0x801B0DA4;
    encounter_player_birthday_command_section_9 = 0x801B0F78;
    encounter_player_birthday_command_section_10 = 0x801B1180;
    encounter_player_birthday_command_section_11 = 0x801B12E0;
    encounter_player_birthday_command_section_12 = 0x801B1470;
    encounter_player_birthday_voice_section_0 = 0x801B01A8;
    encounter_player_birthday_voice_section_1 = 0x801B0398;
    encounter_player_birthday_voice_section_2 = 0x801B03F0;
    encounter_player_birthday_voice_section_3 = 0x801B0590;
    encounter_player_birthday_voice_section_4 = 0x801B0734;
    encounter_player_birthday_voice_section_5 = 0x801B08D4;
    encounter_player_birthday_voice_section_6 = 0x801B0AE4;
    encounter_player_birthday_voice_section_7 = 0x801B0CB4;
    encounter_player_birthday_voice_section_8 = 0x801B0E58;
    encounter_player_birthday_voice_section_9 = 0x801B1064;
    encounter_player_birthday_voice_section_10 = 0x801B126C;
    encounter_player_birthday_voice_section_11 = 0x801B1338;
    encounter_player_birthday_voice_section_12 = 0x801B155C;
}

void encounter_girl_birthday_dispatch(void) {

    s32 idx;
    GekoFnTable73 table;

    table = encounter_girl_birthday_steps;
    idx = game_state.mode_step;
    table.f[idx](0x80);
    if ((u8) encounter_girl_birthday_scene_frame != 0) {
        hud_screen_frame_draw(2);
        func_80083A10();
        return;
    }
    hud_screen_frame_draw(1);
    dec_bg_draw(0);
    parameter_show();
    if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((game_state.pad_held & 0xF000F000) || (D_80122D30 == 1))) {
        D_8011ECD3 |= 0x80;
    }
    if (D_80122D04 != 0) {
        D_8011ECD3 &= 0x7F;
    }
    if (D_8011ECD3 & 0x80) {
        hud_cursor_update();
    }
}


void encounter_girl_birthday_bg_request(void) {
    s32 var_v0;

    D_800B3D60 = 0;
    if ((encounter_girl_birthday_background_variant != 0) || (game_state.day_event_done_bits & 0x20)) {
        if (((u8) game_state.month >= 3U) && ((u8) game_state.month < 6U)) {
            var_v0 = dec_bg_cd_read(0x3FE9, 0);
        } else if (((u8) game_state.month >= 6U) && ((u8) game_state.month < 9U)) {
            var_v0 = dec_bg_cd_read(0x3FF2, 0);
        } else if (((u8) game_state.month >= 9U) && ((u8) game_state.month < 0xCU)) {
            var_v0 = dec_bg_cd_read(0x3FD7, 0);
        } else {
            var_v0 = dec_bg_cd_read(0x3FE0, 0);
        }
    } else if (((u8) game_state.month >= 3U) && ((u8) game_state.month < 6U)) {
        var_v0 = dec_bg_cd_read(0x3FBE, 0);
    } else if (((u8) game_state.month >= 6U) && ((u8) game_state.month < 9U)) {
        var_v0 = dec_bg_cd_read(0x3FC6, 0);
    } else if (((u8) game_state.month >= 9U) && ((u8) game_state.month < 0xCU)) {
        var_v0 = dec_bg_cd_read(0x3FAD, 0);
    } else {
        var_v0 = dec_bg_cd_read(0x3FB6, 0);
    }
    if (var_v0 == *dec_bg_show) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (var_v0 == (1 - *dec_bg_show)) {
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

s32 encounter_girl_birthday_bg_decode(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        dec_bg_reset();
        mode_step_next();
        game_state.mode_step -= 3;
        return 0;
    }
    dec_bg_decode_to_vram(0);
    mode_step_next();
}

void encounter_girl_birthday_bg_show(void) {
    dec_bg_show_switch(0);
    mode_step_next();
}

void encounter_girl_birthday_restart(void) {
    mode_step_set(1);
}

void encounter_girl_birthday_phone_closing_group(void) {
    encounter_girl_birthday_group = 0x23;
    encounter_girl_birthday_line = 0;
    mode_step_next();
}

void encounter_girl_birthday_player_birthday_script_select(void) {
    game_state.day_event_done_bits |= 0x40;
    game_state.day_event_done_bits |= 0x04;
    encounter_girl_birthday_group = 1;
    encounter_girl_birthday_line = 0;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_1;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_1;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_1;
        break;
    case GIRL_MIO:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_6;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_6;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_6;
        break;
    case GIRL_YUINA:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_3;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_3;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_3;
        break;
    case GIRL_AYAKO:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_4;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_4;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_4;
        break;
    case GIRL_SAKI:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_10;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_10;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_10;
        break;
    case GIRL_YUKARI:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_7;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_7;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_7;
        break;
    case GIRL_NOZOMI:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_8;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_8;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_8;
        break;
    case GIRL_MIRA:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_5;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_5;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_5;
        break;
    case GIRL_YUKO:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_0;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_0;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_0;
        break;
    case GIRL_MEGUMI:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_9;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_9;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_9;
        break;
    case GIRL_YUMI:
        encounter_girl_birthday_text_table = encounter_player_birthday_text_section_12;
        encounter_girl_birthday_command_table = encounter_player_birthday_command_section_12;
        encounter_girl_birthday_voice_table = encounter_player_birthday_voice_section_12;
        break;
    }
    mode_step_next();
}

void encounter_girl_birthday_visit_scripts_load(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    encounter_player_birthday_scripts_bind();
    mode_step_next();
}

s32 encounter_girl_birthday_player_birthday_check(void) {
    s16 i;
    s16 best;
    u32 cur;
    u32 bestState;
    u32 state;
    u32 newState;
    u8 yuukou;
    u8 tokimeki;
    u8 bestTokimeki;

    best = 0xFF;
    bestState = 4;
    for (i = 0; i < 11; i++) {
        /* FAKE: T-24302, empty test; gives bestState and newState the original s-registers. */
        if (i) {}
        if (game_state.girls[i].flags.f.met) {
            state = get_g_zyotai_s(i) & 0x7F;
            newState = state & 0xFF;
            if ((u8) state < bestState ||
                /* FAKE: T-24302, "* 1" keeps the original bne operand order. */
                ((cur = best, bestState * 1 == (u8) state) &&
                 (yuukou = get_h_yuukou(i), tokimeki = get_h_tokimeki(i),
                  bestTokimeki = get_h_tokimeki(cur),
                  get_h_yuukou(cur) + bestTokimeki < tokimeki + yuukou))) {
                best = (u8) i;
                bestState = (u8) newState;
            }
        }
    }
    state = get_g_zyotai_s(game_state.current_girl);
    if ((game_state.player_birthday & 0xF) == game_state.month &&
        ((u32) (game_state.player_birthday << 23) >> 27) == game_state.day &&
        !(game_state.day_event_done_bits & 4) && (u8) (state & 0x7F) < 2 && game_state.current_girl == best) {
        /* FAKE: T-24302, two byte-view ORs spend the ugen temporaries the original uses (ori t8). */
        ((u8 *)&game_state)[0x71E] |= 0x44;
        ((u8 *)&game_state)[0x71E] |= 0x44;
        mode_step_set(0x34);
        return 0;
    }
    return mode_step_next();
}

s32 encounter_girl_birthday_talk_with_audio_command(void) {
    encounter_girl_birthday_talk();
    if (game_state.mode_step_timer.w++ == 1) {
        if (encounter_girl_birthday_line == 4) {
            audio_command(0x60C);
        }
    }
}

s32 encounter_girl_birthday_finish(void) {
    u8 state;

    if (game_state.day_event_done_bits & 0x20) {
        audio_command(0xCF);
        game_mode_set(GAME_MODE_NEW_YEAR_A);
        return 0;
    }
    /* FAKE: read through a u8 view so the base and the result share $v0 (T-22010). */
    state = *(u8 *)&game_state.substep;
    if (state == 0) {
        hud_day_bgm_load();
        state = game_state.substep + 1;
    }
    game_state.substep = state;
    if (state == 1) {
        if ((u32)game_state.mode_step_timer.w++ >= 0x400) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            /* FAKE: the dead test gives the byte a second use, so uopt allocates $v0 for the load (T-22010). */
            if ((s32)game_state.substep) {}
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    D_80122D4C = 0;
    return day_event_next(1);
}

void encounter_girl_birthday_audio_load(void) {
    D_800B3D60 = 0;
    if (game_state.day_events[0].type == DAY_EVENT_EXAM) {
        audio_bank_start(0, 0x200);
    }
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void encounter_girl_birthday_audio_wait(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void encounter_girl_birthday_scripts_load(void) {
    cd_read_sectors(0x16, 0x801B0000, 0xAF0D);
    encounter_girl_birthday_scripts_bind();
    mode_step_next();
}

void encounter_girl_birthday_scene_init(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    k_reset(1);
    talk_clear();
    text_marker_hide();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    encounter_girl_birthday_scene_frame = 1;
    mode_step_next();
}

s32 encounter_girl_birthday_init(void) {
    s32 pad1; /* FAKE: T-11900 frame slot: keeps i at sp+0x62 */
    s16 i;
    s32 pad[8]; /* FAKE: T-11900 frame slots: keep zyotai at sp+0x3F and rect at sp+0x34 */
    u8 zyotai;
    RECT rect;
    GameState *gs = &game_state;

    gs->current_girl = gs->day_events[gs->day_event_index].girl;
    if ((gs->current_girl == GIRL_SHIORI && (gs->day_event_done_bits & 1)) || (gs->current_girl != GIRL_SHIORI && (gs->day_event_done_bits & 2))) {
        encounter_girl_birthday_finish();
        return 0;
    }
    if (!gs->girls[gs->current_girl].flags.f.b6 && gs->current_girl != GIRL_SHIORI) {
        encounter_girl_birthday_finish();
        return 0;
    }
    encounter_girl_birthday_background_variant = (gs->day_event_done_bits & 0x10) > 0;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    hud_icon_bar_init();
    parameter_show_init();
    parameter_show_init();
    for (i = 0; i < 15; i++) {
        icon_can_use_set(i, 0);
    }
    hud_icon_bar_update(1);
    text_system_reset();
    encounter_girl_birthday_scene_frame = 0;
    encounter_girl_birthday_club_context = 0;
    encounter_girl_birthday_holiday_context = 1;
    /* The day tables at +0x44/+0x64 are indexed by day - 1 from +0x45/+0x65 (hence `- 1 + 1`).
     * FAKE: T-11900 the call reads the date through game_state, not gs: through gs uopt keeps
     * unk_03E/unk_03F in the argument registers for the whole function. */
    if (gs->month != 12 || gs->day != 24) {
        if (gs->weekday != WEEKDAY_SUNDAY && gs->month_calendar[0].marker[gs->day - 1 + 1] != 0xA && gs->month_calendar[0].marker[gs->day - 1 + 1] != 0x10 && gs->month_calendar[0].day_kind[gs->day - 1 + 1] != 1 && gs->month_calendar[0].day_kind[gs->day - 1 + 1] != 2 && gs->month_calendar[0].day_kind[gs->day - 1 + 1] != 3 && encounter_girl_birthday_background_variant == 0) {
            encounter_girl_birthday_holiday_context = 0;
        } else if ((gs->last_command.b[0] >> 4) == 4 && (gs->player_flags.h >> 12) == (gs->girls[gs->current_girl].flags.b[2] >> 4) && (hud_appointment_find_today(game_state.year, game_state.month, game_state.day) <= 0 || gs->current_girl != ((u32)(gs->unk_54C[0].w << 3) >> 28))) {
            encounter_girl_birthday_holiday_context = 0;
            encounter_girl_birthday_club_context = 1;
        }
    }
    encounter_girl_birthday_zero_choice = 0;
    hizuke_init();
    message_window_init();
    tpage_buf_clear();
    strcpy(D_800CA19C, "廊下");
    strcpy(D_800CA1DC, "近所の公園");
    k_reset(1);
    talk_clear();
    text_marker_hide();
    D_80122D2C = 0;
    gs->tile_layer_enabled = 0;
    gs->quads_enabled = 1;
    if (gs->current_girl == GIRL_SHIORI) {
        gs->day_event_done_bits |= 1;
    } else {
        gs->day_event_done_bits |= 2;
    }
    gs->screen_brightness = 0x80;
    dec_bg_brightness = 0x80;
    D_800B5940 = 0;
    addr_init_bustup();
    encounter_girl_birthday_phone_prelude_timer = 0;
    encounter_girl_birthday_phone_ring_limit = gs->frame_count % 3 + 2;
    zyotai = get_g_zyotai_s(gs->current_girl) & 0x7F;
    encounter_girl_birthday_group = 0;
    encounter_girl_birthday_line = 0;
    talk_group = (zyotai >= 2) + (zyotai >= 3) + (zyotai >= 4) + 30;
    talk_line = 0;
    talk_names_set(gs->current_girl);
    encounter_girl_birthday_scripts_bind();
    switch (gs->current_girl) {
    case GIRL_SHIORI:
        strcpy(D_800CA1DC, "自宅前");
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_1;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_1;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_1;
        talk_text_table = encounter_girl_birthday_text_section_1;
        talk_command_table = encounter_girl_birthday_command_section_1;
        talk_voice_table = encounter_girl_birthday_voice_section_1;
        break;
    case GIRL_MIO:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_5;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_5;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_5;
        break;
    case GIRL_YUINA:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_2;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_2;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_2;
        talk_text_table = encounter_girl_birthday_text_section_2;
        talk_command_table = encounter_girl_birthday_command_section_2;
        talk_voice_table = encounter_girl_birthday_voice_section_2;
        break;
    case GIRL_AYAKO:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_3;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_3;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_3;
        break;
    case GIRL_SAKI:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_10;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_10;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_10;
        break;
    case GIRL_YUKARI:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_6;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_6;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_6;
        break;
    case GIRL_NOZOMI:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_8;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_8;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_8;
        talk_text_table = encounter_girl_birthday_text_section_8;
        talk_command_table = encounter_girl_birthday_command_section_8;
        talk_voice_table = encounter_girl_birthday_voice_section_8;
        if (zyotai < 2) {
            encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_7;
            encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_7;
            encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_7;
            talk_text_table = encounter_girl_birthday_text_section_7;
            talk_command_table = encounter_girl_birthday_command_section_7;
            talk_voice_table = encounter_girl_birthday_voice_section_7;
        }
        break;
    case GIRL_MIRA:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_4;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_4;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_4;
        break;
    case GIRL_YUKO:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_0;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_0;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_0;
        break;
    case GIRL_MEGUMI:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_9;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_9;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_9;
        break;
    case GIRL_YUMI:
        encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_12;
        encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_12;
        encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_12;
        break;
    }
    if (gs->month >= 4) {
        encounter_girl_birthday_school_year = gs->year - 0x5F;
    } else {
        encounter_girl_birthday_school_year = gs->year - 0x60;
    }
    encounter_girl_birthday_portrait_active = 0;
    encounter_girl_birthday_phone_phase_counter = 0;
    if ((gs->year == 0x5F && gs->month == 6 && gs->day == 3) || (gs->year == 0x60 && gs->month == 6 && gs->day == 1) || (gs->year == 0x61 && gs->month == 6 && gs->day == 7) || (gs->year == 0x5F && gs->month == 10 && gs->day == 14) || (gs->year == 0x60 && gs->month == 10 && gs->day == 12) || (gs->year == 0x61 && gs->month == 10 && gs->day == 11)) {
        rect.x = 0x140;
        rect.y = 0x1A0;
        rect.w = 0x140;
        rect.h = 0x58;
        func_8009C7F8(&rect, 8, 0, 0);
        rect.x = 0x2C0;
        rect.y = 0xA0;
        func_8009C7F8(&rect, 8, 0, 0);
    }
    mode_step_next();
    if (encounter_girl_birthday_background_variant != 0 || (gs->day_event_done_bits & 0x20)) {
        mode_step_set(0x43);
    }
    if (encounter_girl_birthday_holiday_context != 0) {
        mode_step_set(0x43);
    }
}

void encounter_girl_birthday_opening_skip(void) {
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void encounter_girl_birthday_context_skip(void) {
    if (encounter_girl_birthday_holiday_context == 1) {
        mode_step_next();
    }
    mode_step_next();
}

s32 encounter_girl_birthday_appointment_check(void) {
    if (encounter_girl_birthday_holiday_context == 0) {
        game_state.mode_step += 5;
    } else {
        encounter_girl_birthday_group = 0x13;
        if ((hud_appointment_find_today(game_state.year, game_state.month, game_state.day) > 0) && (game_state.current_girl == ((u32)(game_state.unk_54C[0].w << 3) >> 0x1C)) && (D_80122D4C == 0)) {
            mode_step_set(0x3D);
            return 0;
        }
    }
    mode_step_next();
}

void encounter_girl_birthday_corridor_bg_load(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void encounter_girl_birthday_location_select(void) {
    u32 club;

    audio_bank_start(1, 0x200);
    if (encounter_girl_birthday_holiday_context == 0) {
        if (encounter_girl_birthday_club_context == 0) {
            strcpy(D_800CA19C, "廊下");
            bg_read_sub2(0x4045);
            event_log_add(0xA, 0x25);
        } else {
            club = game_state.player_flags.h >> 12;
            switch (club) {
            case 0:
                strcpy(D_800CA19C, "図書室");
                bg_read_sub2(0x40BC);
                break;
            case 1:
                strcpy(D_800CA19C, "演劇部");
                bg_read_sub2(0x40B2);
                break;
            case 2:
                strcpy(D_800CA19C, "実験室");
                bg_read_sub2(0x40C6);
                break;
            case 3:
                strcpy(D_800CA19C, "電脳部室");
                bg_read_sub2(0x40D0);
                break;
            case 4:
                strcpy(D_800CA19C, "美術室");
                bg_read_sub2(0x40D9);
                break;
            case 5:
                strcpy(D_800CA19C, "音楽室");
                bg_read_sub2(0x40E2);
                break;
            case 6:
                strcpy(D_800CA19C, "グランド");
                bg_read_sub2(0x4296);
                break;
            case 7:
                strcpy(D_800CA19C, "グランド");
                bg_read_sub2(0x4296);
                break;
            case 8:
                strcpy(D_800CA19C, "グランド");
                bg_read_sub2(0x4296);
                break;
            case 9:
                strcpy(D_800CA19C, "プール");
                bg_read_sub2(0x40A0);
                break;
            default:
                strcpy(D_800CA19C, "体育館");
                bg_read_sub2(0x40A9);
                break;
            }
            event_log_add(10, (game_state.player_flags.h >> 12) + 0x33);
        }
    } else if ((encounter_girl_birthday_background_variant != 0) || (game_state.day_event_done_bits & 0x20)) {
        if (game_state.current_girl == GIRL_SHIORI) {
            strcpy(D_800CA19C, "自宅前");
            bg_read_sub2(0x4156);
            event_log_add(0xA, 0x28);
        } else {
            strcpy(D_800CA19C, "近所の公園");
            bg_read_sub2(0x42E2);
            event_log_add(0xA, 0x27);
        }
    } else if (game_state.current_girl == GIRL_SHIORI) {
        strcpy(D_800CA19C, "自宅前");
        bg_read_sub2(0x414C);
        event_log_add(0xA, 0x2F);
    } else {
        strcpy(D_800CA19C, "近所の公園");
        bg_read_sub2(0x42D8);
        event_log_add(0xA, 0x26);
    }
    mode_step_next();
}

void encounter_girl_birthday_talk(void) {
    talk_tables = &encounter_girl_birthday_group;
    D_800CA138 = &encounter_girl_birthday_line;
    D_800CA13C = encounter_girl_birthday_text_table;
    D_800CA140 = encounter_girl_birthday_command_table;
    D_800CA144 = encounter_girl_birthday_voice_table;
    talk_run(D_80122CDC, 1, 0);
}

void encounter_girl_birthday_choice_talk(void) {
    talk_tables = &encounter_girl_birthday_group;
    D_800CA138 = &encounter_girl_birthday_line;
    D_800CA13C = encounter_girl_birthday_text_table;
    D_800CA140 = encounter_girl_birthday_command_table;
    D_800CA144 = encounter_girl_birthday_voice_table;
    talk_run(0xFF, 1, 0);
}

/* The bnez slot of the encounter_girl_birthday_holiday_context test takes the selector lui only because encounter_girl_birthday_steps (above) names this
 * function: the switch dispatch then has an edge back to the entry, which makes the switch head light
 * enough for as1 to scan it (T-11900; T-25150 rule, history T-24203, T-24900). */
void encounter_girl_birthday_portrait_read(void) {
    u8 girl;
    GekoWords11 all;
    GekoWords2 first;
    GekoWords2 second;
    GekoWords2 third;

    all = encounter_girl_birthday_shiori_club_portraits;
    first = encounter_girl_birthday_mio_club_portraits;
    second = encounter_girl_birthday_yuina_club_portraits;
    third = encounter_girl_birthday_ayako_club_portraits;
    if (game_state.month < 6 || game_state.month >= 10) {
        all.v[0] = 0x544A;
        all.v[3] = 0x544A;
        all.v[5] = 0x544A;
        first.v[0] = 0x5D41;
        second.v[1] = 0x522E;
        third.v[1] = 0x58AF;
    }
    if (encounter_girl_birthday_holiday_context == 0) {
        girl = game_state.current_girl;
        if (encounter_girl_birthday_club_context == 0) {
            read_bustup_uniform();
            return;
        }
        switch (girl) {
        case 0:
            portrait_read_start(all.v[game_state.player_flags.h >> 12]);
            break;
        case 1:
            portrait_read_start(first.v[game_state.player_flags.h >> 12]);
            break;
        case 2:
            portrait_read_start(second.v[(game_state.player_flags.h >> 12) - 2]);
            break;
        case 3:
            portrait_read_start(third.v[(game_state.player_flags.h >> 12) - 4]);
            break;
        case 4:
            portrait_read_start(0x66EC);
            break;
        case 5:
            portrait_read_start(0x5FE4);
            break;
        case 6:
            portrait_read_start(0x625A);
            break;
        default:
            portrait_read_start(0x6962);
            break;
        }
        mode_step_next();
        return;
    }
    read_bustup();
}

void encounter_girl_birthday_phone_gate_check(void) {
    if (game_state.girls[game_state.current_girl].flags.f.b3) {
        encounter_girl_birthday_group += 1;
        mode_step_next();
    } else {
        encounter_girl_birthday_closing_jump();
    }
}

void encounter_girl_birthday_school_talk(void) {
    if (encounter_girl_birthday_holiday_context == 0) {
        encounter_girl_birthday_talk();
        return;
    }
    mode_step_next();
}

void encounter_girl_birthday_girl_state_group_offset(void) {
    s32 v;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    v = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (v == 2) {
        encounter_girl_birthday_group += 2;
    } else if (v == 3) {
        encounter_girl_birthday_group += 4;
    } else if (v == 4) {
        encounter_girl_birthday_group += 6;
    }
    mode_step_next();
}

void encounter_girl_birthday_school_girl_in(void) {
    if (encounter_girl_birthday_holiday_context == 1) {
        mode_step_next();
        return;
    }
    normal_date_girl_in();
}

void encounter_girl_birthday_holiday_girl_in(void) {
    if (encounter_girl_birthday_holiday_context == 1) {
        normal_date_girl_in();
        return;
    }
    encounter_girl_birthday_group = 4;
    mode_step_next();
}

void encounter_girl_birthday_reply_group_select(void) {
    s32 v;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    v = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (encounter_girl_birthday_holiday_context == 1) {
        encounter_girl_birthday_group = 6;
    }
    if (v == 2) {
        encounter_girl_birthday_group += 2;
    } else if (v == 3) {
        encounter_girl_birthday_group += 4;
    } else if (v == 4) {
        encounter_girl_birthday_group += 6;
    }
    mode_step_next();
}

void encounter_girl_birthday_choice_group_select(void) {
    encounter_girl_birthday_group = (D_80122CDC * 2) + 0xD;
    mode_step_next();
}

/* K&R int return without final return statement: early returns load 0 into $v0, tail falls through. T-24090 */
s32 encounter_girl_birthday_phone_ring_wait(void) {
    if (encounter_girl_birthday_phone_phase_counter == 0) {
        if (encounter_girl_birthday_phone_prelude_timer < 0x5A) {
            encounter_girl_birthday_phone_prelude_timer += 1;
            if (encounter_girl_birthday_phone_prelude_timer == 1) {
                audio_command(0x60A);
            }
            if (encounter_girl_birthday_phone_prelude_timer == 0x1E) {
                audio_command(0x604);
            }
            return 0;
        }
        if (!(game_state.mode_step_timer.u & 7) && game_state.mode_step_timer.u < 0x40U) {
            audio_command((((girl_phone_digits[game_state.current_girl][game_state.mode_step_timer.u >> 3] + 0x505) & 0xFF) | 0x600) & 0xFFFF);
        }
        game_state.mode_step_timer.u += 1;
        if (!(game_state.mode_step_timer.u & 0x3F) && game_state.mode_step_timer.u < 0x140U) {
            k_reset(1);
            set_kanji_string(-0x80, 0x30, 0, "プルルルルル", 0);
            audio_command(0x608);
        }
        if ((game_state.pad_pressed & 0x400040) && game_state.mode_step_timer.u >= 0x40U) {
            audio_command(0x1508);
        } else if (game_state.mode_step_timer.u < 0x140U) {
            return 0;
        }
        k_reset(1);
    }
    encounter_girl_birthday_phone_phase_counter += 1;
    mode_step_next();
}

/* K&R int return without final return statement: early returns load 0 into $v0, tail falls through. T-24090 */
s32 encounter_girl_birthday_phone_talk(void) {
    if (encounter_girl_birthday_phone_phase_counter == 0) {
        if (encounter_girl_birthday_phone_prelude_timer < 0x5A) {
            encounter_girl_birthday_phone_prelude_timer += 1;
            if (encounter_girl_birthday_phone_prelude_timer == 1) {
                audio_command(0x60A);
            }
            if (encounter_girl_birthday_phone_prelude_timer == 0x1E) {
                audio_command(0x604);
            }
            return 0;
        }
        if (!(game_state.mode_step_timer.u & 7) && game_state.mode_step_timer.u < 0x40U) {
            audio_command((((girl_phone_digits[game_state.current_girl][game_state.mode_step_timer.u >> 3] + 0x505) & 0xFF) | 0x600) & 0xFFFF);
        }
        game_state.mode_step_timer.u += 1;
        if (!(game_state.mode_step_timer.u & 0x3F) && game_state.mode_step_timer.u < (u32)(encounter_girl_birthday_phone_ring_limit << 6)) {
            k_reset(1);
            set_kanji_string(-0x80, 0x30, 0, "プルルルルル", 0);
            audio_command(0x608);
        }
        if ((game_state.pad_pressed & 0x400040) && game_state.mode_step_timer.u >= 0x40U) {
            audio_command(0x1508);
        } else if (game_state.mode_step_timer.u < (u32)(encounter_girl_birthday_phone_ring_limit << 6)) {
            return 0;
        }
        k_reset(1);
        audio_command(0x60B);
    }
    encounter_girl_birthday_phone_phase_counter += 1;
    normal_date_speak();
}

void encounter_girl_birthday_first_choice_apply(void) {
    if (D_80122CDC != 0) {
        game_state.girls[game_state.current_girl].affection -= 1;
        game_state.girls[game_state.current_girl].syoushin += 5;
        check_para_limit();
        encounter_girl_birthday_zero_choice = 0;
        encounter_girl_birthday_closing_jump();
        return;
    }
    encounter_girl_birthday_zero_choice = 1;
    encounter_girl_birthday_group += 1;
    mode_step_next();
}

void encounter_girl_birthday_closing_jump(void) {
    mode_step_set(0x31);
}

void encounter_girl_birthday_reply_script_push(void) {
    encounter_girl_birthday_saved_group = encounter_girl_birthday_group;
    encounter_girl_birthday_saved_text_table = encounter_girl_birthday_text_table;
    encounter_girl_birthday_saved_command_table = encounter_girl_birthday_command_table;
    encounter_girl_birthday_saved_voice_table = encounter_girl_birthday_voice_table;
    encounter_girl_birthday_group = game_state.current_girl * 3;
    encounter_girl_birthday_group += encounter_girl_birthday_school_year;
    encounter_girl_birthday_text_table = encounter_girl_birthday_text_section_11;
    encounter_girl_birthday_command_table = encounter_girl_birthday_command_section_11;
    encounter_girl_birthday_voice_table = encounter_girl_birthday_voice_section_11;
    mode_step_next();
}

void encounter_girl_birthday_reply_script_pop(void) {
    encounter_girl_birthday_group = encounter_girl_birthday_saved_group;
    encounter_girl_birthday_text_table = encounter_girl_birthday_saved_text_table;
    encounter_girl_birthday_command_table = encounter_girl_birthday_saved_command_table;
    encounter_girl_birthday_voice_table = encounter_girl_birthday_saved_voice_table;
    encounter_girl_birthday_reply_name_copy();
    mode_step_next();
}

void encounter_girl_birthday_reply_apply(void) {
    portrait_layout_set(D_80122CDC + 1);
    if (D_80122CDC == 0) {
        game_state.girls[game_state.current_girl].tokimeki += 3;
        game_state.girls[game_state.current_girl].affection += 2;
        game_state.girls[game_state.current_girl].syoushin -= 0xF;
    } else if (D_80122CDC == 1) {
        game_state.girls[game_state.current_girl].tokimeki += 2;
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.girls[game_state.current_girl].syoushin -= 0xA;
    } else {
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.girls[game_state.current_girl].syoushin += 0xA;
    }
    check_para_limit();
    mode_step_next();
}

void encounter_girl_birthday_holiday_portrait_show(void) {
    if (encounter_girl_birthday_holiday_context == 1) {
        encounter_girl_birthday_portrait_active = 1;
        func_8007C8A4();
        return;
    }
    mode_step_next();
    mode_step_next();
}

void encounter_girl_birthday_reply_name_copy(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    DateTxt tbl;

    tbl = encounter_girl_birthday_reply_names;
    strcpy(D_800CA25C, &tbl.s[game_state.current_girl * 0x129 + encounter_girl_birthday_school_year * 0x63 + D_80122CDC * 0x21]);
}

void encounter_girl_birthday_background_out(void) {
    if (encounter_girl_birthday_zero_choice == 0) {
        mode_step_next();
        return;
    }
    normal_date_bg_out();
}

void encounter_scene_scripts_bind(void) {
    encounter_scene_text_section_0 = 0x80198304;
    encounter_scene_command_section_0 = 0x80198340;
    encounter_scene_text_section_1 = 0x801987BC;
    encounter_scene_command_section_1 = 0x801987E4;
    encounter_scene_text_section_2 = 0x80198B9C;
    encounter_scene_command_section_2 = 0x80198BBC;
    encounter_scene_text_section_3 = 0x80198E94;
    encounter_scene_command_section_3 = 0x80198EB8;
    encounter_scene_text_section_4 = 0x801991B8;
    encounter_scene_command_section_4 = 0x801991E0;
    encounter_scene_text_section_5 = 0x80199608;
    encounter_scene_command_section_5 = 0x80199640;
    encounter_scene_text_section_6 = 0x80198390;
    encounter_scene_command_section_6 = 0x801983C0;
    encounter_scene_text_section_7 = 0x801986F4;
    encounter_scene_command_section_7 = 0x8019870C;
    encounter_scene_text_section_8 = 0x80198A78;
    encounter_scene_command_section_8 = 0x80198AB4;
    encounter_scene_text_section_9 = 0x80198FFC;
    encounter_scene_command_section_9 = 0x80199028;
    encounter_scene_text_section_10 = 0x801994DC;
    encounter_scene_command_section_10 = 0x80199510;
    encounter_scene_text_section_11 = 0x801998FC;
    encounter_scene_command_section_11 = 0x80199920;
    encounter_scene_text_section_12 = 0x80199D78;
    encounter_scene_command_section_12 = 0x80199DB8;
    encounter_scene_text_section_13 = 0x8019A584;
    encounter_scene_command_section_13 = 0x8019A5D8;
    encounter_scene_text_section_14 = 0x801982E8;
    encounter_scene_command_section_14 = 0x80198314;
    encounter_scene_text_section_15 = 0x8019869C;
    encounter_scene_command_section_15 = 0x801986B8;
    encounter_scene_text_section_16 = 0x801989DC;
    encounter_scene_command_section_16 = 0x801989FC;
    encounter_scene_text_section_17 = 0x80198DD0;
    encounter_scene_command_section_17 = 0x80198DF0;
    encounter_scene_text_section_18 = 0x8019915C;
    encounter_scene_command_section_18 = 0x80199180;
    encounter_scene_text_section_19 = 0x80199604;
    encounter_scene_command_section_19 = 0x80199644;
    encounter_scene_text_section_20 = 0x80199B18;
    encounter_scene_command_section_20 = 0x80199B38;
    encounter_scene_text_section_21 = 0x8019894C;
    encounter_scene_command_section_21 = 0x801989C4;
    encounter_scene_text_section_22 = 0x80199068;
    encounter_scene_command_section_22 = 0x80199098;
    encounter_scene_text_section_23 = 0x801994E4;
    encounter_scene_command_section_23 = 0x80199524;
    encounter_scene_text_section_24 = 0x801983D0;
    encounter_scene_command_section_24 = 0x801983FC;
    encounter_scene_text_section_25 = 0x8019889C;
    encounter_scene_command_section_25 = 0x801988D4;
    encounter_scene_text_section_26 = 0x80198D54;
    encounter_scene_command_section_26 = 0x80198D88;
    encounter_scene_text_section_27 = 0x80199154;
    encounter_scene_command_section_27 = 0x80199178;
    encounter_scene_text_section_28 = 0x801996D4;
    encounter_scene_command_section_28 = 0x80199714;
    encounter_scene_text_section_29 = 0x80199D8C;
    encounter_scene_command_section_29 = 0x80199DF0;
    encounter_scene_text_section_30 = 0x8019829C;
    encounter_scene_command_section_30 = 0x801982D0;
    encounter_scene_text_section_31 = 0x801986D0;
    encounter_scene_command_section_31 = 0x80198704;
    encounter_scene_text_section_32 = 0x80198A44;
    encounter_scene_command_section_32 = 0x80198A6C;
    encounter_scene_text_section_33 = 0x80198FF4;
    encounter_scene_command_section_33 = 0x80199040;
    encounter_scene_text_section_34 = 0x801995F8;
    encounter_scene_command_section_34 = 0x8019961C;
    encounter_scene_text_section_35 = 0x80199958;
    encounter_scene_command_section_35 = 0x8019997C;
    encounter_scene_text_section_36 = 0x80199D60;
    encounter_scene_command_section_36 = 0x80199DB4;
    encounter_scene_text_section_37 = 0x80198308;
    encounter_scene_command_section_37 = 0x80198330;
    encounter_scene_text_section_38 = 0x801987A8;
    encounter_scene_command_section_38 = 0x801987E4;
    encounter_scene_text_section_39 = 0x80198C40;
    encounter_scene_command_section_39 = 0x80198C60;
    encounter_scene_text_section_40 = 0x80198FA0;
    encounter_scene_command_section_40 = 0x80198FBC;
    encounter_scene_text_section_41 = 0x80199424;
    encounter_scene_command_section_41 = 0x80199464;
    encounter_scene_text_section_42 = 0x801983B8;
    encounter_scene_command_section_42 = 0x801983E0;
    encounter_scene_text_section_43 = 0x8019881C;
    encounter_scene_command_section_43 = 0x8019883C;
    encounter_scene_text_section_44 = 0x80198C9C;
    encounter_scene_command_section_44 = 0x80198CBC;
    encounter_scene_text_section_45 = 0x8019920C;
    encounter_scene_command_section_45 = 0x80199240;
    encounter_scene_text_section_46 = 0x801997C0;
    encounter_scene_command_section_46 = 0x801997E0;
    encounter_scene_text_section_47 = 0x80199DC0;
    encounter_scene_command_section_47 = 0x80199E1C;
    encounter_scene_text_section_48 = 0x8019A30C;
    encounter_scene_command_section_48 = 0x8019A344;
    encounter_scene_text_section_49 = 0x80198374;
    encounter_scene_command_section_49 = 0x801983A0;
    encounter_scene_text_section_50 = 0x80198C3C;
    encounter_scene_command_section_50 = 0x80198CAC;
    encounter_scene_text_section_51 = 0x80199520;
    encounter_scene_command_section_51 = 0x8019955C;
    encounter_scene_text_section_52 = 0x80199CA4;
    encounter_scene_command_section_52 = 0x80199CF0;
    encounter_scene_text_section_53 = 0x8019A4D8;
    encounter_scene_command_section_53 = 0x8019A518;
    encounter_scene_text_section_54 = 0x80198418;
    encounter_scene_command_section_54 = 0x8019845C;
    encounter_scene_text_section_55 = 0x801989A4;
    encounter_scene_command_section_55 = 0x801989BC;
    encounter_scene_text_section_56 = 0x80198DBC;
    encounter_scene_command_section_56 = 0x80198DFC;
    encounter_scene_text_section_57 = 0x80199540;
    encounter_scene_command_section_57 = 0x801995CC;
    encounter_scene_text_section_58 = 0x80199BD8;
    encounter_scene_command_section_58 = 0x80199BF8;
    encounter_scene_text_section_59 = 0x80199F04;
    encounter_scene_command_section_59 = 0x80199F2C;
    encounter_scene_text_section_60 = 0x8019A37C;
    encounter_scene_command_section_60 = 0x8019A3B8;
    encounter_scene_text_section_61 = 0x801987E8;
    encounter_scene_command_section_61 = 0x80198850;
    encounter_scene_text_section_62 = 0x8019932C;
    encounter_scene_command_section_62 = 0x80199360;
    encounter_scene_text_section_63 = 0x80199A80;
    encounter_scene_command_section_63 = 0x80199ACC;
    encounter_scene_text_section_64 = 0x80199F68;
    encounter_scene_command_section_64 = 0x80199F90;
    encounter_scene_text_section_65 = 0x8019A474;
    encounter_scene_command_section_65 = 0x8019A4B8;
    encounter_scene_text_section_66 = 0x8019ABDC;
    encounter_scene_command_section_66 = 0x8019AC40;
    encounter_scene_text_section_67 = 0x80198784;
    encounter_scene_command_section_67 = 0x801987F8;
    encounter_scene_text_section_68 = 0x80198FB4;
    encounter_scene_command_section_68 = 0x80198FE0;
    encounter_scene_text_section_69 = 0x80199418;
    encounter_scene_command_section_69 = 0x8019943C;
    encounter_scene_text_section_70 = 0x801998C8;
    encounter_scene_command_section_70 = 0x801998F4;
    encounter_scene_text_section_71 = 0x80199EF4;
    encounter_scene_command_section_71 = 0x80199F24;
    encounter_scene_text_section_72 = 0x8019A58C;
    encounter_scene_command_section_72 = 0x8019A5C8;
    encounter_scene_text_section_73 = 0x8019AB0C;
    encounter_scene_command_section_73 = 0x8019AB4C;
    encounter_scene_text_section_74 = 0x80199B14;
    encounter_scene_command_section_74 = 0x80199B6C;
    encounter_scene_text_section_75 = 0x80199F30;
    encounter_scene_command_section_75 = 0x80199F50;
    encounter_scene_text_section_76 = 0x8019A0E0;
    encounter_scene_command_section_76 = 0x8019A0F8;
    encounter_scene_text_section_77 = 0x8019A628;
    encounter_scene_command_section_77 = 0x8019A690;
    encounter_scene_text_section_78 = 0x8019AFF8;
    encounter_scene_command_section_78 = 0x8019B06C;
    encounter_scene_voice_section_0 = 0x801985A4;
    encounter_scene_voice_section_1 = 0x8019897C;
    encounter_scene_voice_section_2 = 0x80198D34;
    encounter_scene_voice_section_3 = 0x80198FFC;
    encounter_scene_voice_section_4 = 0x80199340;
    encounter_scene_voice_section_5 = 0x80199850;
    encounter_scene_voice_section_6 = 0x8019861C;
    encounter_scene_voice_section_7 = 0x801987A4;
    encounter_scene_voice_section_8 = 0x80198CE0;
    encounter_scene_voice_section_9 = 0x80199230;
    encounter_scene_voice_section_10 = 0x801996CC;
    encounter_scene_voice_section_11 = 0x80199AEC;
    encounter_scene_voice_section_12 = 0x8019A020;
    encounter_scene_voice_section_13 = 0x8019AA8C;
    encounter_scene_voice_section_14 = 0x801984E8;
    encounter_scene_voice_section_15 = 0x801987DC;
    encounter_scene_voice_section_16 = 0x80198BAC;
    encounter_scene_voice_section_17 = 0x80198F4C;
    encounter_scene_voice_section_18 = 0x801992C4;
    encounter_scene_voice_section_19 = 0x80199890;
    encounter_scene_voice_section_20 = 0x80199CCC;
    encounter_scene_voice_section_21 = 0x80198F14;
    encounter_scene_voice_section_22 = 0x80199190;
    encounter_scene_voice_section_23 = 0x801997F8;
    encounter_scene_voice_section_24 = 0x8019863C;
    encounter_scene_voice_section_25 = 0x80198A5C;
    encounter_scene_voice_section_26 = 0x80198F78;
    encounter_scene_voice_section_27 = 0x801992BC;
    encounter_scene_voice_section_28 = 0x80199A20;
    encounter_scene_voice_section_29 = 0x8019A050;
    encounter_scene_voice_section_30 = 0x80198470;
    encounter_scene_voice_section_31 = 0x801988BC;
    encounter_scene_voice_section_32 = 0x80198B98;
    encounter_scene_voice_section_33 = 0x80199388;
    encounter_scene_voice_section_34 = 0x80199804;
    encounter_scene_voice_section_35 = 0x80199A88;
    encounter_scene_voice_section_36 = 0x8019A044;
    encounter_scene_voice_section_37 = 0x8019851C;
    encounter_scene_voice_section_38 = 0x801989A4;
    encounter_scene_voice_section_39 = 0x80198DBC;
    encounter_scene_voice_section_40 = 0x80199134;
    encounter_scene_voice_section_41 = 0x80199678;
    encounter_scene_voice_section_42 = 0x801985E4;
    encounter_scene_voice_section_43 = 0x80198998;
    encounter_scene_voice_section_44 = 0x80198E50;
    encounter_scene_voice_section_45 = 0x80199484;
    encounter_scene_voice_section_46 = 0x801999C8;
    encounter_scene_voice_section_47 = 0x8019A0AC;
    encounter_scene_voice_section_48 = 0x8019A500;
    encounter_scene_voice_section_49 = 0x801985A8;
    encounter_scene_voice_section_50 = 0x801991DC;
    encounter_scene_voice_section_51 = 0x801997C0;
    encounter_scene_voice_section_52 = 0x8019A0C0;
    encounter_scene_voice_section_53 = 0x8019A840;
    encounter_scene_voice_section_54 = 0x80198784;
    encounter_scene_voice_section_55 = 0x80198B14;
    encounter_scene_voice_section_56 = 0x8019902C;
    encounter_scene_voice_section_57 = 0x80199A18;
    encounter_scene_voice_section_58 = 0x80199D38;
    encounter_scene_voice_section_59 = 0x8019A0E0;
    encounter_scene_voice_section_60 = 0x8019A5CC;
    encounter_scene_voice_section_61 = 0x80198E04;
    encounter_scene_voice_section_62 = 0x80199664;
    encounter_scene_voice_section_63 = 0x80199D90;
    encounter_scene_voice_section_64 = 0x8019A10C;
    encounter_scene_voice_section_65 = 0x8019A73C;
    encounter_scene_voice_section_66 = 0x8019AFE8;
    encounter_scene_voice_section_67 = 0x80198CF4;
    encounter_scene_voice_section_68 = 0x801991E8;
    encounter_scene_voice_section_69 = 0x801995D0;
    encounter_scene_voice_section_70 = 0x80199B34;
    encounter_scene_voice_section_71 = 0x8019A180;
    encounter_scene_voice_section_72 = 0x8019A848;
    encounter_scene_voice_section_73 = 0x8019ADD0;
    encounter_scene_voice_section_74 = 0x80199DE4;
    encounter_scene_voice_section_75 = 0x8019A05C;
    encounter_scene_voice_section_76 = 0x8019A174;
    encounter_scene_voice_section_77 = 0x8019AB34;
    encounter_scene_voice_section_78 = 0x8019B494;
}

void encounter_scene_mode_handler(void) {

    portrait_wink_update();
    switch (D_80122CD4) {
    case 0:
        encounter_shiori_scene_select_1();
        break;
    case 1:
        encounter_mio_scene_select_1();
        break;
    case 2:
        encounter_yuina_scene_select_1();
        break;
    case 3:
        encounter_ayako_scene_select_1();
        break;
    case 4:
        encounter_saki_scene_select_1();
        break;
    case 5:
        encounter_yukari_scene_select_1();
        break;
    case 6:
        encounter_nozomi_scene_select_1();
        break;
    case 7:
        encounter_mira_scene_select_1();
        break;
    case 8:
        encounter_yuko_scene_select_1();
        break;
    case 9:
        encounter_megumi_scene_select_1();
        break;
    case 10:
        encounter_yumi_scene_select_1();
        break;
    case 11:
        encounter_miharu_scene_select_1();
        break;
    case 12:
        encounter_rei_scene_select_1();
        break;
    case 13:
        encounter_yoshio_entry();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    if (D_80122D38 == 0) {
        hud_screen_frame_draw(2);
    }
    message_window_show();
    message_window_title_update();
    hizuke_show();
    if (D_80122D38 == 0) {
        func_80083A10();
    }
    draw_shake_update();
    if (D_80122D38 != 0) {
        hud_screen_frame_draw(1);
        dec_bg_draw(0);
        parameter_show();
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((game_state.pad_held & 0xF000F000) || (D_80122D30 == 1))) {
            D_8011ECD3 |= 0x80;
        }
        if (D_80122D04 != 0) {
            D_8011ECD3 &= 0x7F;
        }
        if (D_8011ECD3 & 0x80) {
            hud_cursor_update();
        }
    }
}


void encounter_scene_choice_talk(void) {
    talk_tables = &D_800CA150;
    D_800CA138 = &D_800CA154;
    D_800CA13C = encounter_scene_text_table;
    D_800CA140 = encounter_scene_command_table;
    D_800CA144 = encounter_scene_voice_table;
    talk_run(0xFF, 1, 0);
}

void encounter_scene_talk(void) {
    talk_tables = &D_800CA150;
    D_800CA138 = &D_800CA154;
    D_800CA13C = encounter_scene_text_table;
    D_800CA140 = encounter_scene_command_table;
    D_800CA144 = encounter_scene_voice_table;
    talk_run(D_80122CDC, 1, 0);
}

void encounter_scene_phone_talk(void) {
    talk_tables = &D_800CA150;
    D_800CA138 = &D_800CA154;
    D_800CA13C = encounter_scene_text_table;
    D_800CA140 = encounter_scene_command_table;
    D_800CA144 = encounter_scene_voice_table;
    talk_run(D_80122CDC, 1, 2);
}

void encounter_scene_audio_load(void) {
    if (D_80122CD4 == 8 || (D_80122CD4 == 0xB && talk_work == 2)) {
        audio_bank_load_start(1, 0xC063, 0xC045, D_800B3688[D_80122CD4], D_800B36C8[D_80122CD4], D_800B3708[D_80122CD4]);
    } else {
        audio_bank_load_start(1, 0xC043, 0xC01D, D_800B3688[D_80122CD4], D_800B36C8[D_80122CD4], D_800B3708[D_80122CD4]);
    }
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void encounter_scene_audio_wait_start(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void encounter_scene_audio_wait(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void encounter_scene_audio_start(void) {
    audio_bank_start(1, 0x200);
    mode_step_next();
}

s32 encounter_scene_finish(void) {
    u8 state;

    /* FAKE: read through a u8 view so the base and the result share $v0 (T-22010, T-24115). */
    state = *(u8 *)&game_state.substep;
    if (state == 0) {
        audio_command(0xB2);
        hud_day_bgm_load();
        state = game_state.substep + 1;
    }
    game_state.substep = state;
    if (state == 1) {
        if ((u32)game_state.mode_step_timer.w++ >= 0x400U) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() != 1) {
            return 0;
        }
    }
    audio_command(0x22);
    audio_command(0x21);
    audio_command(0x2F);
    /* FAKE: dead (s32) read hoists lui $a1, %hi(D_800CA368) into the load delay of D_80122CD4 (T-24115). */
    if ((s32)D_800CA368) {}
    game_state.current_girl = D_80122CD4;
    if (D_800CA368 != 0xFF) {
        event_log_add(2, D_800CA368);
        if (D_800CA368 == 0x14) {
            if (game_state.event_log[game_state.event_log_count - 1].scene == 2 &&
                game_state.event_log[game_state.event_log_count - 1].flag == D_800CA368) {
                game_state.event_log[game_state.event_log_count - 1].rank = 4;
            } else {
                game_state.event_log[game_state.event_log_count].rank = 4;
            }
        }
    }
    D_80122D44 = 0;
    return day_event_next(1);
}

void encounter_scene_scripts_load(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        cd_read_sectors(6U, 0x80198000, 0xAE25);
        break;
    case GIRL_MIO:
        cd_read_sectors(3U, 0x80198000, 0xAE39);
        break;
    case GIRL_YUINA:
        cd_read_sectors(4U, 0x80198000, 0xAE2B);
        break;
    case GIRL_AYAKO:
        cd_read_sectors(5U, 0x80198000, 0xAE2F);
        break;
    case GIRL_SAKI:
        cd_read_sectors(7U, 0x80198000, 0xAE4C);
        break;
    case GIRL_YUKARI:
        cd_read_sectors(5U, 0x80198000, 0xAE3C);
        break;
    case GIRL_NOZOMI:
        cd_read_sectors(6U, 0x80198000, 0xAE41);
        break;
    case GIRL_MIRA:
        cd_read_sectors(5U, 0x80198000, 0xAE34);
        break;
    case GIRL_YUKO:
        cd_read_sectors(4U, 0x80198000, 0xAE21);
        break;
    case GIRL_MEGUMI:
        cd_read_sectors(5U, 0x80198000, 0xAE47);
        break;
    case GIRL_YUMI:
        cd_read_sectors(6U, 0x80198000, 0xAE5A);
        break;
    case GIRL_MIHARU:
    case GIRL_REI:
    case GIRL_YOSHIO:
        cd_read_sectors(7U, 0x80198000, 0xAE53);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    encounter_scene_scripts_bind();
    mode_step_next();
}

void encounter_scene_light_effect_init(void) {
    _sprite_set_light_effect1(0x120, -0xF8, 0xA0, 0x3C, -0xA0, 0x3C, 9, 0x808080, 0);
    _sprite_set_light_effect1(0x120, -0xF8, 0x40, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0);
    _sprite_set_light_effect1(0x120, -0xF8, 0x10, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x40);
    _sprite_set_light_effect1(0x120, -0xF8, -0x1C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x63);
    _sprite_set_light_effect1(0x120, -0xF8, -0x50, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x24);
    _sprite_set_light_effect1(0x120, -0xF8, -0x8C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x73);
    _sprite_set_light_effect1(0x120, -0xF8, -0xF0, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x11);
    _sprite_set_light_effect1(0x120, -0xF8, -0x140, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x50);
    _sprite_set_light_effect1(0x120, -0xF8, -0x30, 0x3C, 0, 0xB4, 9, 0x808080, 0);
    dtd_on_tpage(0, 0, 9, 1, 0);
    _sprite_set_light_effect1(-0x140, -0x118, -0x142, -0x141, -0xA2, -0x79, 9, 0x808080, 0);
    dtd_on_tpage(0, 0, 9, 1, 0);
}
