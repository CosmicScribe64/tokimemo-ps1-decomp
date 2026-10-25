#ifndef OVL_GEKO_H
#define OVL_GEKO_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_D_801206DA /* matched as u8 (main_api.h: s8) */
#define MAIN_API_OVERRIDE_talk_names_set /* implicit declaration, as matched; main_api.h has a prototype with narrow parameters */
#define MAIN_API_OVERRIDE_cd_last_command /* matched with s32(): the result is compared unmasked (main_api.h: u8(void)) */

#include "common.h"
#include "game.h"

/* Overlay-local externs (T-1300). */
void encounter_yoshio_scene_all_dispatch();
extern u8 encounter_girl_birthday_holiday_context;
extern u8 encounter_walk_home_high_girl_counter;
s32 encounter_walk_home_talk(void);
void encounter_player_birthday_scripts_bind(void);
void encounter_girl_birthday_scripts_bind(void);
extern s32 encounter_girl_birthday_text_table;
extern s32 encounter_girl_birthday_command_table;
extern s32 encounter_girl_birthday_voice_table;
extern s32 encounter_girl_birthday_saved_text_table;
extern s32 encounter_girl_birthday_saved_command_table;
extern s32 encounter_girl_birthday_saved_voice_table;
extern s16 encounter_girl_birthday_group;
extern s16 encounter_girl_birthday_saved_group;
void encounter_girl_birthday_reply_name_copy(void);
extern s16 encounter_player_birthday_group;
extern s16 encounter_player_birthday_line;
extern s32 encounter_player_birthday_text_table;
extern s32 encounter_player_birthday_command_table;
extern s32 encounter_player_birthday_voice_table;
extern s16 encounter_girl_birthday_line;
extern s32 encounter_scene_text_table;
extern s32 encounter_scene_command_table;
extern s32 encounter_scene_voice_table;
extern s16 encounter_invite_group;
extern s16 encounter_invite_line;
extern s32 encounter_invite_text_table;
extern s32 encounter_invite_command_table;
extern s32 encounter_invite_voice_table;
void encounter_invite_dispatch();

extern u8 D_801206DA;

extern s32 encounter_rei_pack_1_palette;
extern s32 encounter_rei_pack_1_still_image_0;
extern s32 encounter_rei_pack_1_still_image_1;
extern s32 encounter_rei_pack_1_still_image_2;
extern s32 encounter_rei_pack_1_still_image_3;
extern s32 encounter_rei_pack_1_still_image_4;
extern s32 encounter_rei_pack_1_still_image_5;

extern s32 encounter_miharu_pack_1_palette;
extern s32 encounter_miharu_pack_1_still_image_0;
extern s32 encounter_miharu_pack_1_still_image_1;
extern s32 encounter_miharu_pack_1_still_image_2;
extern s32 encounter_miharu_pack_1_still_image_3;
extern s32 encounter_miharu_pack_1_still_image_4;
extern s32 encounter_miharu_pack_1_still_image_5;

extern s32 D_80146600;
extern s32 D_80146604;
extern s32 D_80146608;
extern s16 D_8014660C;
extern s32 encounter_yuina_pack_1_palette;
extern s32 encounter_yuina_pack_1_still_image_0;
extern s32 encounter_yuina_pack_1_still_image_1;
extern s32 encounter_yuina_pack_1_still_image_2;
extern s32 encounter_yuina_pack_1_still_image_3;
extern s32 encounter_yuina_pack_1_still_image_4;
extern s32 encounter_yuina_pack_1_still_image_5;

extern s32 encounter_ayako_pack_1_animation_input_2;
extern s32 encounter_ayako_pack_1_animation_input_0;
extern s32 encounter_ayako_pack_1_animation_input_1;
extern s16 encounter_ayako_pack_1_animation_input_3;
extern s32 encounter_ayako_pack_1_palette;
extern s32 encounter_ayako_pack_1_still_image_0;
extern s32 encounter_ayako_pack_1_still_image_1;
extern s32 encounter_ayako_pack_1_still_image_2;
extern s32 encounter_ayako_pack_1_still_image_3;
extern s32 encounter_ayako_pack_1_still_image_4;
extern s32 encounter_ayako_pack_1_still_image_5;

extern s32 encounter_yukari_pack_1_animation_input_2;
extern s32 encounter_yukari_pack_1_animation_input_0;
extern s32 encounter_yukari_pack_1_animation_input_1;
extern s16 encounter_yukari_pack_1_animation_input_3;
extern s32 encounter_yukari_pack_1_palette;
extern s32 encounter_yukari_pack_1_still_image_0;
extern s32 encounter_yukari_pack_1_still_image_1;
extern s32 encounter_yukari_pack_1_still_image_2;
extern s32 encounter_yukari_pack_1_still_image_3;
extern s32 encounter_yukari_pack_1_still_image_4;
extern s32 encounter_yukari_pack_1_still_image_5;

extern s32 encounter_saki_pack_1_animation_input_2;
extern s32 encounter_saki_pack_1_animation_input_0;
extern s32 encounter_saki_pack_1_animation_input_1;
extern s16 encounter_saki_pack_1_animation_input_3;
extern s32 encounter_saki_pack_1_palette;
extern s32 encounter_saki_pack_1_still_image_0;
extern s32 encounter_saki_pack_1_still_image_1;
extern s32 encounter_saki_pack_1_still_image_2;
extern s32 encounter_saki_pack_1_still_image_3;
extern s32 encounter_saki_pack_1_still_image_4;
extern s32 encounter_saki_pack_1_still_image_5;

extern s32 encounter_yumi_pack_1_animation_input_2;
extern s32 encounter_yumi_pack_1_animation_input_0;
extern s32 encounter_yumi_pack_1_animation_input_1;
extern s16 encounter_yumi_pack_1_animation_input_3;
extern s32 encounter_yumi_pack_1_palette;
extern s32 encounter_yumi_pack_1_still_image_0;
extern s32 encounter_yumi_pack_1_still_image_1;
extern s32 encounter_yumi_pack_1_still_image_2;
extern s32 encounter_yumi_pack_1_still_image_3;
extern s32 encounter_yumi_pack_1_still_image_4;
extern s32 encounter_yumi_pack_1_still_image_5;

extern s32 encounter_yumi_pack_2_animation_input_2;
extern s32 encounter_yumi_pack_2_animation_input_0;
extern s32 encounter_yumi_pack_2_animation_input_1;
extern s16 encounter_yumi_pack_2_animation_input_3;
extern s32 encounter_yumi_pack_2_palette;
extern s32 encounter_yumi_pack_2_still_image_0;
extern s32 encounter_yumi_pack_2_still_image_1;
extern s32 encounter_yumi_pack_2_still_image_2;
extern s32 encounter_yumi_pack_2_still_image_3;
extern s32 encounter_yumi_pack_2_still_image_4;
extern s32 encounter_yumi_pack_2_still_image_5;

extern s32 encounter_miharu_pack_2_animation_input_2;
extern s32 encounter_miharu_pack_2_animation_input_0;
extern s32 encounter_miharu_pack_2_animation_input_1;
extern s16 encounter_miharu_pack_2_animation_input_3;
extern s32 encounter_miharu_pack_2_palette;
extern s32 encounter_miharu_pack_2_still_image_0;
extern s32 encounter_miharu_pack_2_still_image_1;
extern s32 encounter_miharu_pack_2_still_image_2;
extern s32 encounter_miharu_pack_2_still_image_3;
extern s32 encounter_miharu_pack_2_still_image_4;
extern s32 encounter_miharu_pack_2_still_image_5;

extern s32 encounter_yoshio_pack_1_animation_input_2;
extern s32 encounter_yoshio_pack_1_animation_input_0;
extern s32 encounter_yoshio_pack_1_animation_input_1;
extern s16 encounter_yoshio_pack_1_animation_input_3;
extern s32 encounter_yoshio_pack_1_palette;
extern s32 encounter_yoshio_pack_1_still_image_0;
extern s32 encounter_yoshio_pack_1_still_image_1;
extern s32 encounter_yoshio_pack_1_still_image_2;
extern s32 encounter_yoshio_pack_1_still_image_3;
extern s32 encounter_yoshio_pack_1_still_image_4;
extern s32 encounter_yoshio_pack_1_still_image_5;

extern s32 encounter_shiori_pack_1_animation_input_2;
extern s32 encounter_shiori_pack_1_animation_input_0;
extern s32 encounter_shiori_pack_1_animation_input_1;
extern s16 encounter_shiori_pack_1_animation_input_3;
extern s32 encounter_shiori_pack_1_palette;
extern s32 encounter_falling_quad_palette;
extern s32 encounter_shiori_pack_1_still_image_0;
extern s32 encounter_shiori_pack_1_still_image_1;
extern s32 encounter_shiori_pack_1_still_image_2;
extern s32 encounter_shiori_pack_1_still_image_3;
extern s32 encounter_shiori_pack_1_still_image_4;
extern s32 encounter_shiori_pack_1_still_image_5;
extern s32 D_801463B0;

extern s32 encounter_invite_text_section_0;
extern s32 encounter_invite_text_section_1;
extern s32 encounter_invite_text_section_2;
extern s32 encounter_invite_text_section_3;
extern s32 encounter_invite_text_section_4;
extern s32 encounter_invite_text_section_5;
extern s32 encounter_invite_text_section_6;
extern s32 encounter_invite_text_section_7;
extern s32 encounter_invite_text_section_8;
extern s32 encounter_invite_text_section_9;
extern s32 encounter_invite_text_section_10;
extern s32 encounter_invite_text_section_11;
extern s32 encounter_invite_text_section_12;
extern s32 encounter_invite_command_section_0;
extern s32 encounter_invite_command_section_1;
extern s32 encounter_invite_command_section_2;
extern s32 encounter_invite_command_section_3;
extern s32 encounter_invite_command_section_4;
extern s32 encounter_invite_command_section_5;
extern s32 encounter_invite_command_section_6;
extern s32 encounter_invite_command_section_7;
extern s32 encounter_invite_command_section_8;
extern s32 encounter_invite_command_section_9;
extern s32 encounter_invite_command_section_10;
extern s32 encounter_invite_command_section_11;
extern s32 encounter_invite_command_section_12;
extern s32 encounter_invite_voice_section_0;
extern s32 encounter_invite_voice_section_1;
extern s32 encounter_invite_voice_section_2;
extern s32 encounter_invite_voice_section_3;
extern s32 encounter_invite_voice_section_4;
extern s32 encounter_invite_voice_section_5;
extern s32 encounter_invite_voice_section_6;
extern s32 encounter_invite_voice_section_7;
extern s32 encounter_invite_voice_section_8;
extern s32 encounter_invite_voice_section_9;
extern s32 encounter_invite_voice_section_10;
extern s32 encounter_invite_voice_section_11;
extern s32 encounter_invite_voice_section_12;

extern s16 encounter_walk_home_group;

extern u8 encounter_walk_home_topic; /* EncounterWalkHomeTopic */

extern s32 encounter_walk_home_text_table;
extern s32 encounter_walk_home_command_table;
extern s32 encounter_walk_home_voice_table;
extern u8 encounter_walk_home_line;

extern u8 encounter_girl_birthday_zero_choice;
extern s8 encounter_girl_birthday_portrait_active;
extern s32 encounter_player_birthday_text_section_11;
extern s32 encounter_player_birthday_command_section_11;
extern s32 encounter_player_birthday_voice_section_11;
extern s8 encounter_walk_home_followup_allowed;
extern s8 encounter_girl_birthday_scene_frame;
extern u8 encounter_girl_birthday_background_variant;
extern s32 encounter_girl_birthday_phone_phase_counter;
extern s16 encounter_girl_birthday_phone_prelude_timer;
extern s16 encounter_girl_birthday_phone_ring_limit;
s32 encounter_girl_birthday_phone_ring_wait(void);
s32 encounter_girl_birthday_phone_talk(void);
s32 encounter_walk_home_followup_talk();

void encounter_yuko_scene_one_dispatch(void);
void encounter_ayako_scene_one_dispatch(void);
void encounter_mio_scene_one_dispatch(void);
void encounter_nozomi_scene_one_dispatch(void);
void encounter_saki_scene_one_dispatch(void);
void encounter_player_birthday_dispatch(void);

extern u8 encounter_invite_topic;

void encounter_girl_birthday_closing_jump(void);

/* defined in C in one object, called from another (T-0500) */
void encounter_scene_talk(void);
void encounter_ayako_scene_one_step_dispatch(void);

void encounter_miharu_scene_one_step_dispatch(void);
void encounter_miharu_scene_two_step_dispatch(void);

void encounter_yumi_scene_one_step_dispatch();

extern u8 encounter_yumi_white_flash_enabled;
extern u32 encounter_yumi_closing_actor_timer;
void encounter_yumi_white_flash_draw();
void encounter_yumi_closing_actors_update();

void encounter_yumi_scene_seven_step_dispatch();
void encounter_yukari_scene_two_step_dispatch(void);
extern s32 encounter_walk_home_text_section_0;
extern s32 encounter_walk_home_text_section_1;
extern s32 encounter_walk_home_text_section_2;
extern s32 encounter_walk_home_text_section_3;
extern s32 encounter_walk_home_text_section_4;
extern s32 encounter_walk_home_text_section_5;
extern s32 encounter_walk_home_text_section_6;
extern s32 encounter_walk_home_text_section_7;
extern s32 encounter_walk_home_text_section_8;
extern s32 encounter_walk_home_text_section_9;
extern s32 encounter_walk_home_text_section_10;
extern s32 encounter_walk_home_text_section_11;
extern s32 encounter_walk_home_command_section_0;
extern s32 encounter_walk_home_command_section_1;
extern s32 encounter_walk_home_command_section_2;
extern s32 encounter_walk_home_command_section_3;
extern s32 encounter_walk_home_command_section_4;
extern s32 encounter_walk_home_command_section_5;
extern s32 encounter_walk_home_command_section_6;
extern s32 encounter_walk_home_command_section_7;
extern s32 encounter_walk_home_command_section_8;
extern s32 encounter_walk_home_command_section_9;
extern s32 encounter_walk_home_command_section_10;
extern s32 encounter_walk_home_command_section_11;
extern s32 encounter_walk_home_voice_section_0;
extern s32 encounter_walk_home_voice_section_1;
extern s32 encounter_walk_home_voice_section_2;
extern s32 encounter_walk_home_voice_section_3;
extern s32 encounter_walk_home_voice_section_4;
extern s32 encounter_walk_home_voice_section_5;
extern s32 encounter_walk_home_voice_section_6;
extern s32 encounter_walk_home_voice_section_7;
extern s32 encounter_walk_home_voice_section_8;
extern s32 encounter_walk_home_voice_section_9;
extern s32 encounter_walk_home_voice_section_10;
extern s32 encounter_walk_home_voice_section_11;
extern s32 encounter_scene_text_section_23;
extern s32 encounter_scene_command_section_23;
extern s32 encounter_scene_voice_section_23;
void encounter_rei_scene_one_step_dispatch(void);
s32 cd_last_command();
void encounter_yuko_scene_one_step_dispatch(void);

void encounter_nozomi_scene_one_step_dispatch(void);

extern u8 D_8014755C;

void encounter_yoshio_scene_all_step_dispatch(void);

void encounter_mira_scene_eight_step_dispatch(void);
void encounter_megumi_falling_quads_update(void);
void encounter_shiori_scene_two_step_dispatch(void);
void encounter_megumi_scene_one_step_dispatch(void);
void encounter_shiori_scene_one_step_dispatch(void);
void encounter_megumi_scene_two_step_dispatch(void);
void encounter_yuina_scene_seven_step_dispatch(void);
void encounter_mira_scene_one_step_dispatch(void);
s32 encounter_shiori_falling_quads_update(void);
extern s32 encounter_shiori_pack_2_animation_input_2, encounter_shiori_pack_2_animation_input_0, encounter_shiori_pack_2_animation_input_1, encounter_shiori_pack_2_palette, encounter_shiori_pack_2_still_image_0, encounter_shiori_pack_2_still_image_1, encounter_shiori_pack_2_still_image_2, encounter_shiori_pack_2_still_image_3, encounter_shiori_pack_2_still_image_4, encounter_shiori_pack_2_still_image_5;
extern s16 encounter_shiori_pack_2_animation_input_3;
void encounter_shiori_assets_bind_2(void);
extern s32 encounter_mira_pack_1_animation_input_2, encounter_mira_pack_1_animation_input_0, encounter_mira_pack_1_animation_input_1, encounter_mira_pack_1_palette, encounter_mira_pack_1_still_image_0, encounter_mira_pack_1_still_image_1, encounter_mira_pack_1_still_image_2, encounter_mira_pack_1_still_image_3, encounter_mira_pack_1_still_image_4, encounter_mira_pack_1_still_image_5;
extern s16 encounter_mira_pack_1_animation_input_3;
extern s32 encounter_megumi_pack_1_animation_input_2, encounter_megumi_pack_1_animation_input_0, encounter_megumi_pack_1_animation_input_1, encounter_megumi_pack_1_palette, encounter_megumi_pack_1_still_image_0, encounter_megumi_pack_1_still_image_1, encounter_megumi_pack_1_still_image_2, encounter_megumi_pack_1_still_image_3, encounter_megumi_pack_1_still_image_4, encounter_megumi_pack_1_still_image_5;
extern s16 encounter_megumi_pack_1_animation_input_3;
void encounter_mira_still_show_1(void);
void encounter_megumi_still_show_1(void);
extern u8 *encounter_yuko_pack_1_animation_input_2;
extern u8 *encounter_yuko_pack_1_animation_input_0;
extern u8 *encounter_yuko_pack_1_animation_input_1;
extern s16 encounter_yuko_pack_1_animation_input_3;
extern u8 *encounter_yuko_pack_1_palette;
extern u8 *encounter_yuko_pack_1_still_image_0;
extern u8 *encounter_yuko_pack_1_still_image_1;
extern u8 *encounter_yuko_pack_1_still_image_2;
extern u8 *encounter_yuko_pack_1_still_image_3;
extern u8 *encounter_yuko_pack_1_still_image_4;
extern u8 *encounter_yuko_pack_1_still_image_5;
extern s32 encounter_player_birthday_text_section_0;
extern s32 encounter_player_birthday_text_section_1;
extern s32 encounter_player_birthday_text_section_2;
extern s32 encounter_player_birthday_text_section_3;
extern s32 encounter_player_birthday_text_section_4;
extern s32 encounter_player_birthday_text_section_5;
extern s32 encounter_player_birthday_text_section_6;
extern s32 encounter_player_birthday_text_section_7;
extern s32 encounter_player_birthday_text_section_8;
extern s32 encounter_player_birthday_text_section_9;
extern s32 encounter_player_birthday_text_section_10;
extern s32 encounter_player_birthday_text_section_12;
extern s32 encounter_player_birthday_command_section_0;
extern s32 encounter_player_birthday_command_section_1;
extern s32 encounter_player_birthday_command_section_2;
extern s32 encounter_player_birthday_command_section_3;
extern s32 encounter_player_birthday_command_section_4;
extern s32 encounter_player_birthday_command_section_5;
extern s32 encounter_player_birthday_command_section_6;
extern s32 encounter_player_birthday_command_section_7;
extern s32 encounter_player_birthday_command_section_8;
extern s32 encounter_player_birthday_command_section_9;
extern s32 encounter_player_birthday_command_section_10;
extern s32 encounter_player_birthday_command_section_12;
extern s32 encounter_player_birthday_voice_section_0;
extern s32 encounter_player_birthday_voice_section_1;
extern s32 encounter_player_birthday_voice_section_2;
extern s32 encounter_player_birthday_voice_section_3;
extern s32 encounter_player_birthday_voice_section_4;
extern s32 encounter_player_birthday_voice_section_5;
extern s32 encounter_player_birthday_voice_section_6;
extern s32 encounter_player_birthday_voice_section_7;
extern s32 encounter_player_birthday_voice_section_8;
extern s32 encounter_player_birthday_voice_section_9;
extern s32 encounter_player_birthday_voice_section_10;
extern s32 encounter_player_birthday_voice_section_12;
extern s32 encounter_girl_birthday_text_section_0;
extern s32 encounter_girl_birthday_text_section_1;
extern s32 encounter_girl_birthday_text_section_2;
extern s32 encounter_girl_birthday_text_section_3;
extern s32 encounter_girl_birthday_text_section_4;
extern s32 encounter_girl_birthday_text_section_5;
extern s32 encounter_girl_birthday_text_section_6;
extern s32 encounter_girl_birthday_text_section_7;
extern s32 encounter_girl_birthday_text_section_8;
extern s32 encounter_girl_birthday_text_section_9;
extern s32 encounter_girl_birthday_text_section_10;
extern s32 encounter_girl_birthday_text_section_11;
extern s32 encounter_girl_birthday_text_section_12;
extern s32 encounter_girl_birthday_command_section_0;
extern s32 encounter_girl_birthday_command_section_1;
extern s32 encounter_girl_birthday_command_section_2;
extern s32 encounter_girl_birthday_command_section_3;
extern s32 encounter_girl_birthday_command_section_4;
extern s32 encounter_girl_birthday_command_section_5;
extern s32 encounter_girl_birthday_command_section_6;
extern s32 encounter_girl_birthday_command_section_7;
extern s32 encounter_girl_birthday_command_section_8;
extern s32 encounter_girl_birthday_command_section_9;
extern s32 encounter_girl_birthday_command_section_10;
extern s32 encounter_girl_birthday_command_section_11;
extern s32 encounter_girl_birthday_command_section_12;
extern s32 encounter_girl_birthday_voice_section_0;
extern s32 encounter_girl_birthday_voice_section_1;
extern s32 encounter_girl_birthday_voice_section_2;
extern s32 encounter_girl_birthday_voice_section_3;
extern s32 encounter_girl_birthday_voice_section_4;
extern s32 encounter_girl_birthday_voice_section_5;
extern s32 encounter_girl_birthday_voice_section_6;
extern s32 encounter_girl_birthday_voice_section_7;
extern s32 encounter_girl_birthday_voice_section_8;
extern s32 encounter_girl_birthday_voice_section_9;
extern s32 encounter_girl_birthday_voice_section_10;
extern s32 encounter_girl_birthday_voice_section_11;
extern s32 encounter_girl_birthday_voice_section_12;
extern s32 encounter_scene_text_section_0;
extern s32 encounter_scene_text_section_1;
extern s32 encounter_scene_text_section_2;
extern s32 encounter_scene_text_section_3;
extern s32 encounter_scene_text_section_4;
extern s32 encounter_scene_text_section_5;
extern s32 encounter_scene_text_section_6;
extern s32 encounter_scene_text_section_7;
extern s32 encounter_scene_text_section_8;
extern s32 encounter_scene_text_section_9;
extern s32 encounter_scene_text_section_10;
extern s32 encounter_scene_text_section_11;
extern s32 encounter_scene_text_section_12;
extern s32 encounter_scene_text_section_13;
extern s32 encounter_scene_text_section_14;
extern s32 encounter_scene_text_section_15;
extern s32 encounter_scene_text_section_16;
extern s32 encounter_scene_text_section_17;
extern s32 encounter_scene_text_section_18;
extern s32 encounter_scene_text_section_19;
extern s32 encounter_scene_text_section_20;
extern s32 encounter_scene_text_section_21;
extern s32 encounter_scene_text_section_22;
extern s32 encounter_scene_text_section_24;
extern s32 encounter_scene_text_section_25;
extern s32 encounter_scene_text_section_26;
extern s32 encounter_scene_text_section_27;
extern s32 encounter_scene_text_section_28;
extern s32 encounter_scene_text_section_29;
extern s32 encounter_scene_text_section_30;
extern s32 encounter_scene_text_section_31;
extern s32 encounter_scene_text_section_32;
extern s32 encounter_scene_text_section_33;
extern s32 encounter_scene_text_section_34;
extern s32 encounter_scene_text_section_35;
extern s32 encounter_scene_text_section_36;
extern s32 encounter_scene_text_section_37;
extern s32 encounter_scene_text_section_38;
extern s32 encounter_scene_text_section_39;
extern s32 encounter_scene_text_section_40;
extern s32 encounter_scene_text_section_41;
extern s32 encounter_scene_text_section_42;
extern s32 encounter_scene_text_section_43;
extern s32 encounter_scene_text_section_44;
extern s32 encounter_scene_text_section_45;
extern s32 encounter_scene_text_section_46;
extern s32 encounter_scene_text_section_47;
extern s32 encounter_scene_text_section_48;
extern s32 encounter_scene_text_section_49;
extern s32 encounter_scene_text_section_50;
extern s32 encounter_scene_text_section_51;
extern s32 encounter_scene_text_section_52;
extern s32 encounter_scene_text_section_53;
extern s32 encounter_scene_text_section_54;
extern s32 encounter_scene_text_section_55;
extern s32 encounter_scene_text_section_56;
extern s32 encounter_scene_text_section_57;
extern s32 encounter_scene_text_section_58;
extern s32 encounter_scene_text_section_59;
extern s32 encounter_scene_text_section_60;
extern s32 encounter_scene_text_section_61;
extern s32 encounter_scene_text_section_62;
extern s32 encounter_scene_text_section_63;
extern s32 encounter_scene_text_section_64;
extern s32 encounter_scene_text_section_65;
extern s32 encounter_scene_text_section_66;
extern s32 encounter_scene_text_section_67;
extern s32 encounter_scene_text_section_68;
extern s32 encounter_scene_text_section_69;
extern s32 encounter_scene_text_section_70;
extern s32 encounter_scene_text_section_71;
extern s32 encounter_scene_text_section_72;
extern s32 encounter_scene_text_section_73;
extern s32 encounter_scene_text_section_74;
extern s32 encounter_scene_text_section_75;
extern s32 encounter_scene_text_section_76;
extern s32 encounter_scene_text_section_77;
extern s32 encounter_scene_text_section_78;
extern s32 encounter_scene_command_section_0;
extern s32 encounter_scene_command_section_1;
extern s32 encounter_scene_command_section_2;
extern s32 encounter_scene_command_section_3;
extern s32 encounter_scene_command_section_4;
extern s32 encounter_scene_command_section_5;
extern s32 encounter_scene_command_section_6;
extern s32 encounter_scene_command_section_7;
extern s32 encounter_scene_command_section_8;
extern s32 encounter_scene_command_section_9;
extern s32 encounter_scene_command_section_10;
extern s32 encounter_scene_command_section_11;
extern s32 encounter_scene_command_section_12;
extern s32 encounter_scene_command_section_13;
extern s32 encounter_scene_command_section_14;
extern s32 encounter_scene_command_section_15;
extern s32 encounter_scene_command_section_16;
extern s32 encounter_scene_command_section_17;
extern s32 encounter_scene_command_section_18;
extern s32 encounter_scene_command_section_19;
extern s32 encounter_scene_command_section_20;
extern s32 encounter_scene_command_section_21;
extern s32 encounter_scene_command_section_22;
extern s32 encounter_scene_command_section_24;
extern s32 encounter_scene_command_section_25;
extern s32 encounter_scene_command_section_26;
extern s32 encounter_scene_command_section_27;
extern s32 encounter_scene_command_section_28;
extern s32 encounter_scene_command_section_29;
extern s32 encounter_scene_command_section_30;
extern s32 encounter_scene_command_section_31;
extern s32 encounter_scene_command_section_32;
extern s32 encounter_scene_command_section_33;
extern s32 encounter_scene_command_section_34;
extern s32 encounter_scene_command_section_35;
extern s32 encounter_scene_command_section_36;
extern s32 encounter_scene_command_section_37;
extern s32 encounter_scene_command_section_38;
extern s32 encounter_scene_command_section_39;
extern s32 encounter_scene_command_section_40;
extern s32 encounter_scene_command_section_41;
extern s32 encounter_scene_command_section_42;
extern s32 encounter_scene_command_section_43;
extern s32 encounter_scene_command_section_44;
extern s32 encounter_scene_command_section_45;
extern s32 encounter_scene_command_section_46;
extern s32 encounter_scene_command_section_47;
extern s32 encounter_scene_command_section_48;
extern s32 encounter_scene_command_section_49;
extern s32 encounter_scene_command_section_50;
extern s32 encounter_scene_command_section_51;
extern s32 encounter_scene_command_section_52;
extern s32 encounter_scene_command_section_53;
extern s32 encounter_scene_command_section_54;
extern s32 encounter_scene_command_section_55;
extern s32 encounter_scene_command_section_56;
extern s32 encounter_scene_command_section_57;
extern s32 encounter_scene_command_section_58;
extern s32 encounter_scene_command_section_59;
extern s32 encounter_scene_command_section_60;
extern s32 encounter_scene_command_section_61;
extern s32 encounter_scene_command_section_62;
extern s32 encounter_scene_command_section_63;
extern s32 encounter_scene_command_section_64;
extern s32 encounter_scene_command_section_65;
extern s32 encounter_scene_command_section_66;
extern s32 encounter_scene_command_section_67;
extern s32 encounter_scene_command_section_68;
extern s32 encounter_scene_command_section_69;
extern s32 encounter_scene_command_section_70;
extern s32 encounter_scene_command_section_71;
extern s32 encounter_scene_command_section_72;
extern s32 encounter_scene_command_section_73;
extern s32 encounter_scene_command_section_74;
extern s32 encounter_scene_command_section_75;
extern s32 encounter_scene_command_section_76;
extern s32 encounter_scene_command_section_77;
extern s32 encounter_scene_command_section_78;
extern s32 encounter_scene_voice_section_0;
extern s32 encounter_scene_voice_section_1;
extern s32 encounter_scene_voice_section_2;
extern s32 encounter_scene_voice_section_3;
extern s32 encounter_scene_voice_section_4;
extern s32 encounter_scene_voice_section_5;
extern s32 encounter_scene_voice_section_6;
extern s32 encounter_scene_voice_section_7;
extern s32 encounter_scene_voice_section_8;
extern s32 encounter_scene_voice_section_9;
extern s32 encounter_scene_voice_section_10;
extern s32 encounter_scene_voice_section_11;
extern s32 encounter_scene_voice_section_12;
extern s32 encounter_scene_voice_section_13;
extern s32 encounter_scene_voice_section_14;
extern s32 encounter_scene_voice_section_15;
extern s32 encounter_scene_voice_section_16;
extern s32 encounter_scene_voice_section_17;
extern s32 encounter_scene_voice_section_18;
extern s32 encounter_scene_voice_section_19;
extern s32 encounter_scene_voice_section_20;
extern s32 encounter_scene_voice_section_21;
extern s32 encounter_scene_voice_section_22;
extern s32 encounter_scene_voice_section_24;
extern s32 encounter_scene_voice_section_25;
extern s32 encounter_scene_voice_section_26;
extern s32 encounter_scene_voice_section_27;
extern s32 encounter_scene_voice_section_28;
extern s32 encounter_scene_voice_section_29;
extern s32 encounter_scene_voice_section_30;
extern s32 encounter_scene_voice_section_31;
extern s32 encounter_scene_voice_section_32;
extern s32 encounter_scene_voice_section_33;
extern s32 encounter_scene_voice_section_34;
extern s32 encounter_scene_voice_section_35;
extern s32 encounter_scene_voice_section_36;
extern s32 encounter_scene_voice_section_37;
extern s32 encounter_scene_voice_section_38;
extern s32 encounter_scene_voice_section_39;
extern s32 encounter_scene_voice_section_40;
extern s32 encounter_scene_voice_section_41;
extern s32 encounter_scene_voice_section_42;
extern s32 encounter_scene_voice_section_43;
extern s32 encounter_scene_voice_section_44;
extern s32 encounter_scene_voice_section_45;
extern s32 encounter_scene_voice_section_46;
extern s32 encounter_scene_voice_section_47;
extern s32 encounter_scene_voice_section_48;
extern s32 encounter_scene_voice_section_49;
extern s32 encounter_scene_voice_section_50;
extern s32 encounter_scene_voice_section_51;
extern s32 encounter_scene_voice_section_52;
extern s32 encounter_scene_voice_section_53;
extern s32 encounter_scene_voice_section_54;
extern s32 encounter_scene_voice_section_55;
extern s32 encounter_scene_voice_section_56;
extern s32 encounter_scene_voice_section_57;
extern s32 encounter_scene_voice_section_58;
extern s32 encounter_scene_voice_section_59;
extern s32 encounter_scene_voice_section_60;
extern s32 encounter_scene_voice_section_61;
extern s32 encounter_scene_voice_section_62;
extern s32 encounter_scene_voice_section_63;
extern s32 encounter_scene_voice_section_64;
extern s32 encounter_scene_voice_section_65;
extern s32 encounter_scene_voice_section_66;
extern s32 encounter_scene_voice_section_67;
extern s32 encounter_scene_voice_section_68;
extern s32 encounter_scene_voice_section_69;
extern s32 encounter_scene_voice_section_70;
extern s32 encounter_scene_voice_section_71;
extern s32 encounter_scene_voice_section_72;
extern s32 encounter_scene_voice_section_73;
extern s32 encounter_scene_voice_section_74;
extern s32 encounter_scene_voice_section_75;
extern s32 encounter_scene_voice_section_76;
extern s32 encounter_scene_voice_section_77;
extern s32 encounter_scene_voice_section_78;
extern s32 encounter_mio_pack_1_animation_input_2;
extern s32 encounter_mio_pack_1_animation_input_0;
extern s32 encounter_mio_pack_1_animation_input_1;
extern s16 encounter_mio_pack_1_animation_input_3;
extern s32 encounter_mio_pack_1_palette;
extern s32 encounter_mio_pack_1_still_image_0;
extern s32 encounter_mio_pack_1_still_image_1;
extern s32 encounter_mio_pack_1_still_image_2;
extern s32 encounter_mio_pack_1_still_image_3;
extern s32 encounter_mio_pack_1_still_image_4;
extern s32 encounter_mio_pack_1_still_image_5;
extern s32 encounter_nozomi_pack_1_animation_input_2;
extern s32 encounter_nozomi_pack_1_animation_input_0;
extern s32 encounter_nozomi_pack_1_animation_input_1;
extern s16 encounter_nozomi_pack_1_animation_input_3;
extern s32 encounter_nozomi_pack_1_palette;
extern s32 encounter_nozomi_pack_1_still_image_0;
extern s32 encounter_nozomi_pack_1_still_image_1;
extern s32 encounter_nozomi_pack_1_still_image_2;
extern s32 encounter_nozomi_pack_1_still_image_3;
extern s32 encounter_nozomi_pack_1_still_image_4;
extern s32 encounter_nozomi_pack_1_still_image_5;

extern u8 encounter_yuina_voice_trigger_latched;
void encounter_scene_phone_talk(void);
s32 encounter_shiori_birthday_phone_talk(void);
void encounter_shiori_birthday_init(void);
s32 encounter_yuina_scene_one_step_dispatch(void);
void encounter_yumi_scene_init_3(void);
void encounter_shiori_still_show_2(void);
void encounter_rei_scene_one_dispatch(void);
void encounter_rei_scene_two_dispatch(void);
void encounter_rei_scene_three_dispatch(void);
void encounter_rei_scene_select_1(void);
void encounter_mio_assets_bind_1(void);
void encounter_mio_still_show_1(void);
void encounter_rei_scene_two_step_dispatch(void);
void encounter_mio_scene_one_step_dispatch(void);
void encounter_saki_scene_one_step_dispatch(void);
s32 encounter_girl_birthday_bg_decode(void);
s32 encounter_yumi_closing_bg_decode(void);
void encounter_yumi_scene_two_step_dispatch(void);
void encounter_yukari_scene_one_step_dispatch(void);
extern u8 encounter_girl_birthday_school_year;
void encounter_nozomi_assets_bind_1(void);
void encounter_nozomi_still_show_1(void);
void encounter_rei_scene_three_step_dispatch(void);
void encounter_shiori_scene_one_dispatch(void);
void encounter_shiori_scene_two_dispatch(void);
void encounter_shiori_scene_eight_dispatch(void);
void encounter_shiori_scene_select_1(void);
void encounter_yumi_scene_one_dispatch(void);
void encounter_yumi_scene_two_dispatch(void);
void encounter_yumi_scene_seven_dispatch(void);
void encounter_yumi_scene_select_1(void);
void encounter_miharu_scene_one_dispatch(void);
void encounter_miharu_scene_two_dispatch(void);
void encounter_miharu_scene_four_dispatch(void);
void encounter_miharu_scene_select_1(void);
void encounter_yuina_scene_one_dispatch(void);
void encounter_yuina_scene_seven_dispatch(void);
void encounter_yuina_scene_select_1(void);
void encounter_mira_scene_one_dispatch(void);
void encounter_mira_scene_eight_dispatch(void);
void encounter_mira_scene_select_1(void);
void encounter_yukari_scene_one_dispatch(void);
void encounter_yukari_scene_two_dispatch(void);
void encounter_yukari_scene_select_1(void);
void encounter_megumi_scene_one_dispatch(void);
void encounter_megumi_scene_two_dispatch(void);
void encounter_megumi_scene_select_1(void);
void encounter_miharu_assets_bind_2();

void encounter_girl_birthday_talk(void);

void encounter_scene_scripts_bind(void);
#endif

#ifndef OVL_GEKO_CALLBACK_T9610
#define OVL_GEKO_CALLBACK_T9610
/* Complete local-copy dispatch table in encounter_miharu_scene_four_step_dispatch. */
typedef struct EncounterMiharuClosingSteps {
    /* 0x00 */ void (*callback[64])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:731; 64 callbacks copied in Miharu scene4 dispatcher. Fixed-width declarations, offsets and all override views retained. */
} EncounterMiharuClosingSteps; /* size 0x100 */
extern EncounterMiharuClosingSteps encounter_miharu_scene_four_steps;
void encounter_miharu_scene_four_step_dispatch(void);
#endif

#ifndef OVL_GEKO_THRESHOLDS_T9780
#define OVL_GEKO_THRESHOLDS_T9780
/* Threshold table copied by encounter_walk_home_topic_offer. */
typedef struct EncounterGirlThresholds {
    /* 0x00 */ s16 by_girl[11]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:741; 11 signed per-girl thresholds; topic-offer and birthday checks. Fixed-width declarations, offsets and all override views retained. */
} EncounterGirlThresholds; /* size 0x16 */
extern EncounterGirlThresholds encounter_walk_home_topic_replace_thresholds;
s32 encounter_walk_home_topic_offer(s16 topic);
void encounter_megumi_still_fade_out(void);
void encounter_megumi_still_fade_in(void);

void encounter_shiori_scene_eight_step_dispatch(void);
void encounter_shiori_birthday_portrait_update(void);
void encounter_player_birthday_girl_script_select(void);
s32 encounter_invite_sunday_component(s32 arg0);
void encounter_invite_progress_place_override(void);

#endif

#ifndef OVL_GEKO_TYPES_T17700
#define OVL_GEKO_TYPES_T17700
/* T-17700 types-first pass. The tables below are copied whole to a stack local by an unmatched function
 * (lw/sw runs or a copy loop in the asm); the copy is one struct assignment. Element types come from the data
 * and the reads of the copy; the size comment is the size the copy moves. */

/* Days per month (31, 28, 31, 30, ...) or a per-month value; the February entry is patched for leap years. */
typedef struct EncounterMonthDays {
    /* 0x00 */ s16 days_in_month[12]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:764; 12 calendar month lengths; leap-day patch and rollover arithmetic. Fixed-width declarations, offsets and all override views retained. */
} EncounterMonthDays; /* size 0x18 */
extern EncounterGirlThresholds encounter_walk_home_birthday_thresholds; /* encounter_walk_home_birthday_topic_check; the same values as encounter_walk_home_topic_replace_thresholds */
extern EncounterMonthDays encounter_walk_home_birthday_month_days;  /* encounter_walk_home_birthday_topic_check */
extern EncounterMonthDays encounter_walk_home_scan_month_days;  /* encounter_walk_home_topic_select */
extern EncounterMonthDays encounter_invite_month_days;  /* encounter_invite_place_select_init */
extern EncounterMonthDays encounter_invite_sunday_month_days;  /* encounter_invite_sunday_component */

/* Twelve SJIS strings of 20 bytes, picked by D_800E6374 >> 12 (encounter_walk_home_followup_init). */
typedef struct EncounterClubNames {
    /* 0x00 */ u8 by_club[12][20]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:774; 12 club-name labels indexed by player club. Fixed-width declarations, offsets and all override views retained. */
} EncounterClubNames; /* size 0xF0 */
extern EncounterClubNames encounter_club_names;

/* Picture ids passed to portrait_read_start (encounter_girl_birthday_portrait_read). */
typedef struct GekoWords11 {
    /* 0x00 */ s32 v[11];
} GekoWords11; /* size 0x2C */
typedef struct GekoWords2 {
    /* 0x00 */ s32 v[2];
} GekoWords2; /* size 0x8 */
extern GekoWords11 encounter_girl_birthday_shiori_club_portraits;
extern GekoWords2 encounter_girl_birthday_mio_club_portraits;
extern GekoWords2 encounter_girl_birthday_yuina_club_portraits;
extern GekoWords2 encounter_girl_birthday_ayako_club_portraits;

/* Step table of encounter_yuina_scene_one_step_dispatch, indexed by D_800E738A. */
typedef struct EncounterYuinaSceneOneSteps {
    /* 0x00 */ void (*callbacks[42])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:792; 42 callbacks indexed by mode_step for Yuina talk_work1. Fixed-width declarations, offsets and all override views retained. */
} EncounterYuinaSceneOneSteps; /* size 0xA8 */
extern EncounterYuinaSceneOneSteps encounter_yuina_scene_one_steps;

/* Three coordinate rows of encounter_yumi_closing_still_show. */
typedef struct GekoShorts10 {
    /* 0x00 */ s16 v[10];
} GekoShorts10; /* size 0x14 */
extern GekoShorts10 encounter_yumi_closing_actor_initial_values;
extern GekoShorts10 encounter_yumi_closing_actor_x;
extern GekoShorts10 encounter_yumi_closing_actor_y;

/* encounter_yoshio_ranked_girl_init: six halfwords of -1 and six of 1, 1, 1, 0, 0, 0 (the second label is 0x18 bytes, only 0xC
 * are copied); the draft reads both copies as s16. */
typedef struct EncounterGirlRankingValues {
    /* 0x00 */ s16 entries[6]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:807; Six signed entries shared by ranking-score and girl-id initial arrays. Fixed-width declarations, offsets and all override views retained. */
} EncounterGirlRankingValues; /* size 0xC */
extern EncounterGirlRankingValues encounter_yoshio_rank_scores_init;
extern EncounterGirlRankingValues encounter_yoshio_rank_girls_init;

/* Byte and text tables of encounter_invite_place_select_init, encounter_invite_appointment_store and encounter_invite_progress_place_override. */
typedef struct EncounterGirlInviteWeights {
    /* 0x00 */ u8 weights[209]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:814; 11 girls x19 invitation-topic byte weights. Fixed-width declarations, offsets and all override views retained. */
} EncounterGirlInviteWeights; /* size 0xD1 */
typedef struct EncounterGirlSeasonAvailability {
    /* 0x00 */ u8 available[132]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:817; 11 girls x12 season availability entries for movie/concert. Fixed-width declarations, offsets and all override views retained. */
} EncounterGirlSeasonAvailability; /* size 0x84 */
typedef struct EncounterInviteTopicNames {
    /* 0x00 */ u8 by_topic[19][0x21]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:820; 19 labels of33 bytes, no labels copied into proposal. Fixed-width declarations, offsets and all override views retained. */
} EncounterInviteTopicNames; /* size 0x273 */
typedef struct EncounterPreviousSpotExclusion {
    /* 0x00 */ u8 excluded_place[37]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:823; 37-byte lookup from prior spot id to excluded place. Fixed-width declarations, offsets and all override views retained. */
} EncounterPreviousSpotExclusion; /* size 0x25 */
typedef struct EncounterInvitationByteMap {
    /* 0x00 */ u8 values[21]; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:826; Shared invitation byte-map shape used for place minimum years/months, place-to-topic and topic-to-place mappings; no uniform key/value meaning is asserted. Fixed-width declarations, offsets and all override views retained. */
} EncounterInvitationByteMap; /* size 0x15 */
extern EncounterGirlInviteWeights encounter_invite_girl_topic_weights;
extern EncounterGirlSeasonAvailability encounter_invite_movie_theater_availability;
extern EncounterGirlSeasonAvailability encounter_invite_concert_hall_availability;
extern EncounterInvitationByteMap encounter_invite_place_minimum_year;
extern EncounterInvitationByteMap encounter_invite_place_minimum_month;
extern EncounterInviteTopicNames encounter_invite_topic_names;
extern EncounterPreviousSpotExclusion encounter_invite_previous_place_exclusion;
extern EncounterInvitationByteMap encounter_invite_place_to_topic;
extern EncounterInvitationByteMap encounter_invite_topic_to_place;
extern EncounterInvitationByteMap encounter_invite_progress_place_to_topic;

/* Four-byte calendar entry passed by value to hud_appointment_add by encounter_invite_appointment_store (the layout of TelDateEntry in TEL.h,
 * T-10630). */
typedef union EncounterAppointmentEntry {
    /* 0x00 */ WordView word;
    /* 0x00 */ struct {
        u32 place : 5;  /* bits 0..4 */
        u32 year : 7;   /* bits 5..11 */
        u32 month : 4;  /* bits 12..15 */
        u32 day : 5;    /* bits 16..20 */
        u32 unk_21 : 3;
        u32 unk_24 : 1;
        u32 girl : 4;   /* bits 25..28 */
        u32 active : 1; /* bit 29 */
        u32 unk_30 : 2;
    } bits; /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; include/ovl/GEKO.h:842; Packed appointment word assembled by invitation store; bit layout matches shared appointment record, pending type merge after byte proof. Fixed-width declarations, offsets and all override views retained. */
} EncounterAppointmentEntry; /* size 0x04 */

/* Per-record halfwords of the SpriteQuad records 4..13 (sprite_quads); the code indexes them with i - 4, so the
 * relocations name encounter_shiori_pack_2_still_image_4 and D_801463F8 (encounter_megumi_falling_quads_init, encounter_shiori_falling_quads_update, encounter_megumi_falling_quads_update). */
extern s16 encounter_falling_quad_phases[12];
extern s16 encounter_falling_quad_delays[16];

extern u8 encounter_girl_birthday_club_context; /* encounter_girl_birthday_portrait_read: zero picks read_bustup_uniform (read_bustup_uniform) */

/* 80134D10.c: functions named by the scene table encounter_girl_birthday_steps (defined in C, T-11900). */
s32 encounter_girl_birthday_init(void);
void encounter_girl_birthday_audio_load(void);
void encounter_girl_birthday_audio_wait(void);
void encounter_girl_birthday_scripts_load(void);
void encounter_girl_birthday_opening_skip(void);
void encounter_girl_birthday_context_skip(void);
void encounter_girl_birthday_choice_talk(void);
void encounter_girl_birthday_first_choice_apply(void);
void encounter_girl_birthday_reply_script_push(void);
void encounter_girl_birthday_reply_script_pop(void);
s32 encounter_girl_birthday_appointment_check(void);
void encounter_girl_birthday_phone_gate_check(void);
void encounter_girl_birthday_girl_state_group_offset(void);
s32 encounter_girl_birthday_talk_with_audio_command(void);
void encounter_girl_birthday_scene_init(void);
void encounter_girl_birthday_location_select(void);
void encounter_girl_birthday_portrait_read(void);
void encounter_girl_birthday_holiday_girl_in(void);
void encounter_girl_birthday_reply_group_select(void);
void encounter_girl_birthday_school_girl_in(void);
void encounter_girl_birthday_school_talk(void);
void encounter_girl_birthday_choice_group_select(void);
void encounter_girl_birthday_reply_apply(void);
s32 encounter_girl_birthday_player_birthday_check(void);
void encounter_girl_birthday_background_out(void);
s32 encounter_girl_birthday_finish(void);
void encounter_girl_birthday_visit_scripts_load(void);
void encounter_girl_birthday_player_birthday_script_select(void);
void encounter_girl_birthday_phone_closing_group(void);
void encounter_girl_birthday_bg_request(void);
void encounter_girl_birthday_bg_show(void);
void encounter_girl_birthday_restart(void);
/* HYPOTHESIS: GEKO topic grouping vocabulary remains unconfirmed; technical code-domain values and source sites are preserved. */
enum EncounterWalkHomeTopic {
    ENCOUNTER_TOPIC_NONE = 0,
    ENCOUNTER_TOPIC_EXAM_MARKER = 1,
    ENCOUNTER_TOPIC_CULTURE_FESTIVAL_MARKER = 2,
    ENCOUNTER_TOPIC_CLUB_VARIANT_THREE = 3,
    ENCOUNTER_TOPIC_CLUB_VARIANT_FOUR = 4,
    ENCOUNTER_TOPIC_CLUB_VARIANT_FIVE = 5,
    ENCOUNTER_TOPIC_SPORTS_FESTIVAL_MARKER = 6,
    ENCOUNTER_TOPIC_SCHOOL_TRIP_LOW_STATE_OR_YUMI = 7,
    ENCOUNTER_TOPIC_SCHOOL_TRIP_OTHER = 8,
    ENCOUNTER_TOPIC_NEW_YEAR_PREP_MARKER = 9,
    ENCOUNTER_TOPIC_BIRTHDAY = 10,
    ENCOUNTER_TOPIC_NOTICE_B = 11,
    ENCOUNTER_TOPIC_NOTICE_D = 12,
    ENCOUNTER_TOPIC_NOTICE_F = 13,
    ENCOUNTER_TOPIC_NOTICE_A_1996 = 14,
    ENCOUNTER_TOPIC_NOTICE_A_1997 = 15,
    ENCOUNTER_TOPIC_PRIORITY_MARKER = 16,
    ENCOUNTER_TOPIC_MIRA_RANDOM = 100
};

/* HYPOTHESIS: GEKO Sunday component grouping vocabulary remains unconfirmed; technical code-domain values and source sites are preserved. */
enum EncounterSundayComponent {
    ENCOUNTER_SUNDAY_YEAR = 0,
    ENCOUNTER_SUNDAY_MONTH = 1,
    ENCOUNTER_SUNDAY_DAY = 2
};

#endif
