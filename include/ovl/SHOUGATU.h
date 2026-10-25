#ifndef OVL_SHOUGATU_H
#define OVL_SHOUGATU_H

#include "common.h"
#include "main_api.h"
#include "game.h"

/* SHOUGATU data. */
extern u8 D_80146160;
extern u8 D_80146164;
void new_year_scene_audio_command_0504(void); /* T-24004: called by dispatcher before definition */
void christmas_girl_dialogue_resource_select(void);

void new_year_mira_scene_mode_step_dispatch();
void new_year_yumi_scene_mode_step_dispatch();
extern s16 D_80145F2C;
#ifndef SHOUGATU_OVERRIDE_new_year_meeting_talk_run
void new_year_meeting_talk_run(void);
#endif
extern s32 D_80143FE8;
extern s32 D_80143FEC;
extern s32 D_80143FF0;
void new_year_scene_selector_text_copy(void);
extern u8 D_80143B20;
s32 new_year_talk_run_current(void);
extern u8 D_80143B18;
extern u8 D_80145A10;
void new_year_girl_dialogue_run(void);
void new_year_scene_dialogue_command_voice_addresses_init(void);
void new_year_scene_dialogue_addresses_init(void);
void new_year_omikuji_result_text(void);
extern u8 D_80143FF4;
extern s16 D_80144E08;
extern s16 D_80144E0C;
extern s32 D_80144DFC;
extern s32 D_80144E00;
extern s32 D_80144E04;
extern s16 D_80145F30;
extern s32 D_80145F20;
extern s32 D_80145F24;
extern s32 D_80145F28;
extern s16 D_80143B00;
extern s16 D_80143B04;
extern s32 D_80143AF4;
extern s32 D_80143AF8;
extern s32 D_80143AFC;
extern s32 D_80145F34;
extern s32 D_80145F38;
extern s32 D_80145F3C;
extern s32 D_80145FD0;
extern s32 D_80145FD4;
extern s32 D_80145FD8;
extern s16 D_80145FDC;
extern s32 D_80145FE0;
extern s32 D_80146230;
extern s32 D_80146234;
extern s32 D_80146238;
extern s32 D_80146240;
extern s16 D_8014623C;
extern s32 D_80144D90;
extern s32 D_80144D94;
extern s32 D_80144D98;
extern s32 D_80144D9C;
extern s32 D_80144DA0;
extern s32 D_80144DA4;
extern s32 D_80144DA8;
extern s32 D_80144DAC;
extern s32 D_80144DB0;
extern s32 D_80144DB4;
extern s32 D_80144DB8;
extern s32 D_80144DBC;
extern s32 D_80144DC0;
extern s32 D_80144DC4;
extern s32 D_80144DC8;
extern s32 D_80144DCC;
extern s32 D_80144DD0;
extern s32 D_80144DD4;
extern s32 D_80144DD8;
extern s32 D_80144DDC;
extern s32 D_80144DE0;
extern s32 D_80144DE4;
extern s32 D_80144DE8;
extern s32 D_80144DEC;
extern s32 D_80144DF0;
extern s32 D_80144DF4;
extern s32 D_80144DF8;
void new_year_scene_girl_dialogue_tables_select();
void new_year_mira_scene_resource_addresses_init();
void christmas_party_resource_addresses_init();
void christmas_dialogue_resource_addresses_init();
void new_year_scene_particle_addresses_init();
void new_year_scene_particle_resource_addresses_init_alt();
void new_year_scene_particle_resource_addresses_init_third();
void new_year_resource_addresses_init();
void new_year_scene_asset_addresses_init();
extern u8 D_80144E14;
void new_year_yukari_scene_primary_mode_step_dispatch();
void new_year_yukari_scene_secondary_mode_step_dispatch();
void new_year_nozomi_scene_primary_mode_step_dispatch();
void new_year_nozomi_scene_secondary_mode_step_dispatch();
void new_year_girl_talk_resource_addresses_init();
void new_year_scene_callback_addresses_init();
extern s16 D_801467FC;
extern u8 D_8014653C;
void new_year_current_talk_start();
extern u8 D_80146050;
extern u8 new_year_omikuji_selector;
extern u8 D_80144F10;
extern u8 D_80143B24;
extern s32 D_80143EB4;
extern s32 D_80143EE8;
extern s32 D_80143F1C;
#ifndef SHOUGATU_OVERRIDE_new_year_current_selector_talk_start
void new_year_current_selector_talk_start();
#endif
void new_year_meeting_dialogue_addresses_init();
extern u8 D_80145FE4;
extern u8 D_80144E10;
extern u8 D_80146158;
extern u8 D_80146168[];
void new_year_current_talk_step();
extern u8 D_80143D20;
void new_year_ayako_scene_primary_mode_step_dispatch();
void new_year_ayako_scene_secondary_mode_step_dispatch();
void new_year_ayako_scene_default_mode_step_dispatch();
void new_year_saki_callback_group_0_dispatch();
void new_year_saki_callback_group_1_dispatch();
void new_year_saki_callback_group_default_dispatch();
void new_year_yuko_callback_common_dispatch();
void new_year_yuko_callback_default_dispatch();
void new_year_scene_callback_group_0_dispatch();
void new_year_scene_callback_group_1_dispatch();
void new_year_scene_callback_group_2_dispatch();
void new_year_scene_callback_group_3_dispatch();
void new_year_scene_callback_group_4_dispatch();
void new_year_girl_dialogue_setup_dispatch();
void new_year_relationship_outcome_dispatch();
extern s32 D_80145960;
extern s32 D_80145964;
extern s32 D_80145968;
extern s32 D_8014596C;
extern s32 D_80145970;
extern s32 D_80145974;
extern s32 D_80145978;
extern s32 D_8014597C;
extern s32 D_80145980;
extern s32 D_80145984;
extern s32 D_80145988;
extern s32 D_8014598C;
extern s32 D_80145990;
extern s32 D_80145994;
extern s32 D_80145998;
extern s32 D_8014599C;
extern s32 D_801459A0;
extern s32 D_801459A4;
extern s32 D_801459A8;
extern s32 D_801459AC;
extern s32 D_801459B0;
extern s32 D_801459B4;
extern s32 D_801459B8;
extern s32 D_801459BC;
extern s32 D_801459C0;
extern s32 D_801459C4;
extern s32 D_801459C8;
extern s32 D_801459CC;
extern s32 D_801459D0;
extern s32 D_801459D4;
extern s32 D_801459D8;
extern s32 D_801459DC;
extern s32 D_801459E0;
extern s32 D_801459E4;
extern s32 D_801459E8;
extern s32 D_801459EC;
extern s32 D_801459F0;
extern s32 D_801459F4;
extern s32 D_801459F8;
extern s32 D_80145AD0;
extern s32 D_80145AD4;
extern s32 D_80145AD8;
extern s32 D_80145ADC;
extern s32 D_80145AE0;
extern s32 D_80145AE4;
extern s32 D_80145AE8;
extern s32 D_80145AEC;
extern s32 D_80145AF0;
extern s32 D_80145AF4;
extern s32 D_80145AF8;
extern s32 D_80145AFC;
extern s32 D_80145B00;
extern s32 D_80145B04;
extern s32 D_80145B08;
extern s32 D_80145B0C;
extern s32 D_80145B10;
extern s32 D_80145B14;
extern s32 D_80145B18;
extern s32 D_80145B1C;
extern s32 D_80145B20;
extern s32 D_80145B24;
extern s32 D_80145B28;
extern s32 D_80145B2C;
extern s32 D_80145B30;
extern s32 D_80145B34;
extern s32 D_80145B38;
extern s32 D_80145B3C;
extern s32 D_80145B40;
extern s32 D_80145B44;
extern s32 D_80145B48;
extern s32 D_80145B4C;
extern s32 D_80145B50;
extern s32 D_80145B54;
extern s32 D_80145B58;
extern s32 D_80145B5C;
extern s32 D_80145B60;
extern s32 D_80145B64;
extern s32 D_80145B68;
extern s32 D_801464E0;
extern s32 D_80145F1C;
extern s32 D_80143A48;
extern s32 D_80143A84;
extern s32 D_80143AC0;
extern s16 D_80143BC0;
extern u8 *D_80146510;
extern u8 *D_80146514;
extern u8 *D_80146518;
extern s16 D_8014651C;
extern u8 *D_80146520;
extern u8 *D_80146524;
extern u8 *D_80146528;
extern u8 *D_8014652C;
extern u8 *D_80146530;
extern u8 *D_80146534;
extern u8 *D_80146538;
extern u8 *D_80145270;
extern u8 *D_80145274;
extern u8 *D_80145278;
extern s16 D_8014527C;
extern u8 *D_80145280;
extern u8 *D_80145284;
extern u8 *D_80145288;
extern u8 *D_8014528C;
extern u8 *D_80145290;
extern u8 *D_80145294;
extern u8 *D_80145298;
extern u8 *D_8014529C;
extern u8 *D_801452A0;
extern u8 *D_801452A4;
extern u8 *D_801452A8;
extern u8 *D_801452AC;
extern u8 *D_801452B0;
extern u8 *D_801452B4;
extern u8 *D_801453B0;
extern u8 *D_801453B4;
extern u8 *D_801453B8;
extern s16 D_801453BC;
extern u8 *D_801453C0;
extern u8 *D_801453C4;
extern u8 *D_801453C8;
extern u8 *D_801453CC;
extern u8 *D_801453D0;
extern u8 *D_801453D4;
extern u8 *D_801453D8;
extern u8 *D_801453DC;
extern u8 *D_801453E0;
extern u8 *D_801453E4;
extern u8 *D_801453E8;
extern u8 *D_801453EC;
extern u8 *D_801453F0;
extern u8 *D_801453F4;
extern s16 D_80145A08;
extern s16 D_80145A0C;
extern s32 D_801459FC;
extern s32 D_80145A00;
extern s32 D_80145A04;
extern s32 D_80146144;
extern s32 D_80146148;
extern s32 D_8014614C;
extern s32 D_80146154;
extern s32 D_80143F4C;
extern s32 D_80143F50;
extern s32 D_80143F54;
extern s32 D_80143F58;
extern s32 D_80143F5C;
extern s32 D_80143F60;
extern s32 D_80143F64;
extern s32 D_80143F68;
extern s32 D_80143F6C;
extern s32 D_80143F70;
extern s32 D_80143F74;
extern s32 D_80143F78;
extern s32 D_80143F7C;
extern s32 D_80143F80;
extern s32 D_80143F84;
extern s32 D_80143F88;
extern s32 D_80143F8C;
extern s32 D_80143F90;
extern s32 D_80143F94;
extern s32 D_80143F98;
extern s32 D_80143F9C;
extern s32 D_80143FA0;
extern s32 D_80143FA4;
extern s32 D_80143FA8;
extern s32 D_80143FAC;
extern s32 D_80143FB0;
extern s32 D_80143FB4;
extern s32 D_80143FB8;
extern s32 D_80143FBC;
extern s32 D_80143FC0;
extern s32 D_80143FC4;
extern s32 D_80143FC8;
extern s32 D_80143FCC;
extern s32 D_80143FD0;
extern s32 D_80143FD4;
extern s32 D_80143FD8;
extern s32 D_80143FDC;
extern s32 D_80143FE0;
extern s32 D_80143FE4;
extern s32 D_80143EB0;
extern s32 D_80143EB8;
extern s32 D_80143EBC;
extern s32 D_80143EC0;
extern s32 D_80143EC4;
extern s32 D_80143EC8;
extern s32 D_80143ECC;
extern s32 D_80143ED0;
extern s32 D_80143ED4;
extern s32 D_80143ED8;
extern s32 D_80143EDC;
extern s32 D_80143EE0;
extern s32 D_80143EE4;
extern s32 D_80143EEC;
extern s32 D_80143EF0;
extern s32 D_80143EF4;
extern s32 D_80143EF8;
extern s32 D_80143EFC;
extern s32 D_80143F00;
extern s32 D_80143F04;
extern s32 D_80143F08;
extern s32 D_80143F0C;
extern s32 D_80143F10;
extern s32 D_80143F14;
extern s32 D_80143F18;
extern s32 D_80143F20;
extern s32 D_80143F24;
extern s32 D_80143F28;
extern s32 D_80143F2C;
extern s32 D_80143F30;
extern s32 D_80143F34;
extern s32 D_80143F38;
extern s32 D_80143F3C;
extern s32 D_80143F40;
extern s32 D_80143F44;
extern s32 D_80143F48;
extern s32 D_80145B6C;
extern s32 D_80145B70;
extern s32 D_80145B74;
extern s32 D_80145B78;
extern s32 D_80145B7C;
extern s32 D_80145B80;
extern s32 D_80145B84;
extern s32 D_80145B88;
extern s32 D_80145B8C;
extern s32 D_80145B90;
extern s32 D_80145B94;
extern s32 D_80145B98;
extern s32 D_80145B9C;
extern s32 D_80145BA0;
extern s32 D_80145BA4;
extern s32 D_80145BA8;
extern s32 D_80145BAC;
extern s32 D_80145BB0;
extern s32 D_80145BB4;
extern s32 D_80145BB8;
extern s32 D_80145BBC;
extern s32 D_80145BC0;
extern s32 D_80145BC4;
extern s32 D_80145BC8;
extern s32 D_80145BCC;
extern s32 D_80145BD0;
extern s32 D_80145BD4;
extern s32 D_80145BD8;
extern s32 D_80145BDC;
extern s32 D_80145BE0;
extern s32 D_80145BE4;
extern s32 D_80145BE8;
extern s32 D_80145BEC;
extern s32 D_80145BF0;
extern s32 D_80145BF4;
extern s32 D_80145BF8;
extern s32 D_80145BFC;
extern s32 D_80145C00;
extern s32 D_80145C04;
extern s32 D_80145C08;
extern s32 D_80145C0C;
extern s32 D_80145C10;
extern s32 D_80145C14;
extern s32 D_80145C18;
extern s32 D_80145C1C;
extern s32 D_80145C20;
extern s32 D_80145C24;
extern s32 D_80145C28;
extern s32 D_80145C2C;
extern s32 D_80145C30;
extern s32 D_80145C34;
extern s32 D_80145C38;
extern s32 D_80145C3C;
extern s32 D_80145C40;
extern s32 D_80145C44;
extern s32 D_80145C48;
extern s32 D_80145C4C;
extern s32 D_80145C50;
extern s32 D_80145C54;
extern s32 D_80145C58;
extern s32 D_80145C5C;
extern s32 D_80145C60;
extern s32 D_80145C64;
extern s32 D_80145C68;
extern s32 D_80145C6C;
extern s32 D_80145C70;
extern s32 D_80145C74;
extern s32 D_80145C78;
extern s32 D_80145C7C;
extern s32 D_80145C80;
extern s32 D_80145C84;
extern s32 D_80145C88;
extern s32 D_80145C8C;
extern s32 D_80145C90;
extern s32 D_80145C94;
extern s32 D_80145C98;
extern s32 D_80145C9C;
extern s32 D_80145CA0;
extern s32 D_80145CA4;
extern s32 D_80145CA8;
extern s32 D_80145CAC;
extern s32 D_80145CB0;
extern s32 D_80145CB4;
extern s32 D_80145CB8;
extern s32 D_80145CBC;
extern s32 D_80145CC0;
extern s32 D_80145CC4;
extern s32 D_80145CC8;
extern s32 D_80145CCC;
extern s32 D_80145CD0;
extern s32 D_80145CD4;
extern s32 D_80145CD8;
extern s32 D_80145CDC;
extern s32 D_80145CE0;
extern s32 D_80145CE4;
extern s32 D_80145CE8;
extern s32 D_80145CEC;
extern s32 D_80145CF0;
extern s32 D_80145CF4;
extern s32 D_80145CF8;
extern s32 D_80145CFC;
extern s32 D_80145D00;
extern s32 D_80145D04;
extern s32 D_80145D08;
extern s32 D_80145D0C;
extern s32 D_80145D10;
extern s32 D_80145D14;
extern s32 D_80145D18;
extern s32 D_80145D1C;
extern s32 D_80145D20;
extern s32 D_80145D24;
extern s32 D_80145D28;
extern s32 D_80145D2C;
extern s32 D_80145D30;
extern s32 D_80145D34;
extern s32 D_80145D38;
extern s32 D_80145D3C;
extern s32 D_80145D40;
extern s32 D_80145D44;
extern s32 D_80145D48;
extern s32 D_80145D4C;
extern s32 D_80145D50;
extern s32 D_80145D54;
extern s32 D_80145D58;
extern s32 D_80145D5C;
extern s32 D_80145D60;
extern s32 D_80145D64;
extern s32 D_80145D68;
extern s32 D_80145D6C;
extern s32 D_80145D70;
extern s32 D_80145D74;
extern s32 D_80145D78;
extern s32 D_80145D7C;
extern s32 D_80145D80;
extern s32 D_80145D84;
extern s32 D_80145D88;
extern s32 D_80145D8C;
extern s32 D_80145D90;
extern s32 D_80145D94;
extern s32 D_80145D98;
extern s32 D_80145D9C;
extern s32 D_80145DA0;
extern s32 D_80145DA4;
extern s32 D_80145DA8;
extern s32 D_80145DAC;
extern s32 D_80145DB0;
extern s32 D_80145DB4;
extern s32 D_80145DB8;
extern s32 D_80145DBC;
extern s32 D_80145DC0;
extern s32 D_80145DC4;
extern s32 D_80145DC8;
extern s32 D_80145DCC;
extern s32 D_80145DD0;
extern s32 D_80145DD4;
extern s32 D_80145DD8;
extern s32 D_80145DDC;
extern s32 D_80145DE0;
extern s32 D_80145DE4;
extern s32 D_80145DE8;
extern s32 D_80145DEC;
extern s32 D_80145DF0;
extern s32 D_80145DF4;
extern s32 D_80145DF8;
extern s32 D_80145DFC;
extern s32 D_80145E00;
extern s32 D_80145E04;
extern s32 D_80145E08;
extern s32 D_80145E0C;
extern s32 D_80145E10;
extern s32 D_80145E14;
extern s32 D_80145E18;
extern s32 D_80145E1C;
extern s32 D_80145E20;
extern s32 D_80145E24;
extern s32 D_80145E28;
extern s32 D_80145E2C;
extern s32 D_80145E30;
extern s32 D_80145E34;
extern s32 D_80145E38;
extern s32 D_80145E3C;
extern s32 D_80145E40;
extern s32 D_80145E44;
extern s32 D_80145E48;
extern s32 D_80145E4C;
extern s32 D_80145E50;
extern s32 D_80145E54;
extern s32 D_80145E58;
extern s32 D_80145E5C;
extern s32 D_80145E60;
extern s32 D_80145E64;
extern s32 D_80145E68;
extern s32 D_80145E6C;
extern s32 D_80145E70;
extern s32 D_80145E74;
extern s32 D_80145E78;
extern s32 D_80145E7C;
extern s32 D_80145E80;
extern s32 D_80145E84;
extern s32 D_80145E88;
extern s32 D_80145E8C;
extern s32 D_80145E90;
extern s32 D_80145E94;
extern s32 D_80145E98;
extern s32 D_80145E9C;
extern s32 D_80145EA0;
extern s32 D_80145EA4;
extern s32 D_80145EA8;
extern s32 D_80145EAC;
extern s32 D_80145EB0;
extern s32 D_80145EB4;
extern s32 D_80145EB8;
extern s32 D_80145EBC;
extern s32 D_80145EC0;
extern s32 D_80145EC4;
extern s32 D_80145EC8;
extern s32 D_80145ECC;
extern s32 D_80145ED0;
extern s32 D_80145ED4;
extern s32 D_80145ED8;
extern s32 D_80145EDC;
extern s32 D_80145EE0;
extern s32 D_80145EE4;
extern s32 D_80145EE8;
extern s32 D_80145EEC;
extern s32 D_80145EF0;
extern s32 D_80145EF4;
extern s32 D_80145EF8;
extern s32 D_80145EFC;
extern s32 D_80145F00;
extern s32 D_80145F04;
extern s32 D_80145F08;
extern s32 D_80145F0C;
extern s32 D_80145F10;
extern s32 D_80145F14;
extern s32 D_80145F18;
extern s32 D_80143A40;
extern s32 D_80143A44;
extern s32 D_80143A4C;
extern s32 D_80143A50;
extern s32 D_80143A54;
extern s32 D_80143A58;
extern s32 D_80143A5C;
extern s32 D_80143A60;
extern s32 D_80143A64;
extern s32 D_80143A68;
extern s32 D_80143A6C;
extern s32 D_80143A70;
extern s32 D_80143A74;
extern s32 D_80143A78;
extern s32 D_80143A7C;
extern s32 D_80143A80;
extern s32 D_80143A88;
extern s32 D_80143A8C;
extern s32 D_80143A90;
extern s32 D_80143A94;
extern s32 D_80143A98;
extern s32 D_80143A9C;
extern s32 D_80143AA0;
extern s32 D_80143AA4;
extern s32 D_80143AA8;
extern s32 D_80143AAC;
extern s32 D_80143AB0;
extern s32 D_80143AB4;
extern s32 D_80143AB8;
extern s32 D_80143ABC;
extern s32 D_80143AC4;
extern s32 D_80143AC8;
extern s32 D_80143ACC;
extern s32 D_80143AD0;
extern s32 D_80143AD4;
extern s32 D_80143AD8;
extern s32 D_80143ADC;
extern s32 D_80143AE0;
extern s32 D_80143AE4;
extern s32 D_80143AE8;
extern s32 D_80143AEC;
extern s32 D_80143AF0;

extern u8 D_80143B14;
void new_year_ijuin_entry_step_dispatch(void);
void new_year_yuina_scene_fourth_mode_step_dispatch(void);
void new_year_scene_step_dispatch(void);
void new_year_talk_run_current_with_flags(void);
void new_year_dialogue_fallback_table_setup(void);
void new_year_shrine_actor_mode_step_dispatch(void);
void new_year_result_step_dispatch(void);
void new_year_shrine_dialogue_mode_step_dispatch(void);
void christmas_yukari_mode_step_dispatch(void);
void new_year_yuina_scene_primary_mode_step_dispatch(void);
void new_year_yuina_scene_dialogue_audio_tick_alt(void);
void new_year_yuina_scene_fifth_mode_step_dispatch(void);
void new_year_hallway_scene_exit_branch(void);
void christmas_mio_mode_step_dispatch(void);
void christmas_result_progress_threshold(void);
void christmas_yuina_mode_step_dispatch(void);
void christmas_ayako_mode_step_dispatch(void);
void christmas_yuko_mode_step_dispatch(void);
void new_year_scene_effect_mode_step_dispatch(void);
extern s32 D_80143DB0;
extern s32 D_80143DB4;
extern s32 D_80143DB8;
extern s32 D_80143DBC;
extern s32 D_80143DC0;
extern s32 D_80143DC4;
extern s16 D_80143DC8;
extern s16 D_80143DCC;
extern s32 D_80143DD0;
extern s32 D_80143DD4;
void new_year_omikuji_resource_addresses_init(void);
extern u8 *D_80146130;
extern u8 *D_80146134;
extern u8 *D_80146138;
extern s16 D_8014613C;
extern u8 *D_80146140;
void new_year_scene_actor_resource_addresses_init(void);
void new_year_yuina_scene_secondary_mode_step_dispatch(void);
void new_year_yuina_scene_default_mode_step_dispatch(void);
void new_year_scene_choice_menu_init(void);

/* Bit 2 of the first byte of a GirlRecord flag word, set as a bit-field store: `|= 4` gives one temporary too many (T-6010, T-9000) */
typedef struct {
    u8 pad:2;
    u8 f:1;
    u8 rest:5;
} Bits64B8;
void christmas_party_candidate_select(void);

void new_year_scene_particle_actor_resource_addresses_init(void);
/* Three Shift-JIS strings of 9 bytes (27 bytes, so the struct copy ends with lwl/swl). */
typedef struct SjisStr3 {
    u8 s[3][9];
} SjisStr3;
extern SjisStr3 D_80143D88;
void new_year_player_birthday_status_gate(void);

extern u8 D_80143DDC;
extern s32 D_801464B0;
extern s32 D_801464B4;
extern s32 D_801464B8;
extern s16 D_801464BC;
extern s32 D_801464C0;
extern s32 D_801464C4;
extern s32 D_801464C8;
extern s32 D_801464CC;
extern s32 D_801464D0;
extern s32 D_801464D4;
extern s32 D_801464D8;
extern s32 D_801464DC;
extern s32 D_801464E4;
extern s32 D_801464E8;
extern s32 D_801464EC;
extern s16 D_801464F0;
extern s32 D_801464F4;
extern s32 D_801464F8;
extern s32 D_801464FC;
extern s32 D_80146500;
extern s32 D_80146504;
extern s32 D_80146508;
extern s32 D_8014650C;

#ifndef OVL_SHOUGATU_RECORDS_T9740
#define OVL_SHOUGATU_RECORDS_T9740
/* T-9740: the 28 callbacks copied by christmas_girl_dialogue_step_dispatch. */
typedef struct ShoCalendarFn28 {
    /* 0x00 */ void (*f[28])();
} ShoCalendarFn28; /* size 0x70 */
extern ShoCalendarFn28 D_8014605C;

s32 christmas_girl_dialogue_step_dispatch(void);
#endif

/* Scene initializer; original byte stores at D_80143B1C (T-9740). */
extern u8 D_80143B1C;
void new_year_scene_init(void);
void new_year_girl_dialogue_tables_select(void);

#ifndef OVL_SHOUGATU_BYTE_FLAGS_T10010
#define OVL_SHOUGATU_BYTE_FLAGS_T10010
/* Byte flag view for GirlRecord +0x0E, including bit 3. T-10010. */
typedef struct ShouByteFlags {
    /* 0x00, bit 0 */ u8 b0 : 1;
    /* 0x00, bit 1 */ u8 b1 : 1;
    /* 0x00, bit 2 */ u8 b2 : 1;
    /* 0x00, bit 3 */ u8 b3 : 1;
    /* 0x00, bits 4-7 */ u8 rest : 4;
} ShouByteFlags; /* size 0x01 */
void new_year_scene_status_talk_group_select(void);
void new_year_scene_text_display_tick(void);

#endif

#ifndef OVL_SHOUGATU_T10850_TYPES
#define OVL_SHOUGATU_T10850_TYPES
/* T-10850: 23 callback pointers copied by christmas_party_actor_update. */
typedef struct ShoCallbacks23 {
    /* 0x00 */ void (*f[23])();
} ShoCallbacks23; /* size 0x5C */
extern ShoCallbacks23 D_80145FE8;

void new_year_yuina_scene_third_mode_step_dispatch(void);
void new_year_yuina_scene_group_dispatch(void);
void new_year_yuina_scene_state_init(void);
void new_year_girl_portrait_load(void);
void new_year_christmas_portrait_resource_select(void);
void new_year_girl_scene_resource_load(void);

#endif

#ifndef OVL_SHOUGATU_T13202_INITIALIZER
#define OVL_SHOUGATU_T13202_INITIALIZER
/* T-13202: widths read/written by the sprite initializer. */
extern s16 D_80146150;
extern s8 D_8014615C;
#endif

#ifndef OVL_SHOUGATU_T15600_DISPATCH
#define OVL_SHOUGATU_T15600_DISPATCH
/* T-15600: definitions in 80136F90.c / 8013F190.c, called by the earlier dispatcher. */
s32 new_year_meeting_completion_state_apply(void);
void new_year_meeting_scene_init(void); /* T-17500 */
void new_year_meeting_audio_bank_load_start(void);
void new_year_meeting_audio_bank_load_retry(void);
void new_year_meeting_resources_load(void);
void christmas_saki_mode_step_dispatch(void); /* T-24109 */
void christmas_nozomi_mode_step_dispatch(void);
void christmas_mira_mode_step_dispatch(void);
void christmas_megumi_mode_step_dispatch(void);
#endif

#ifndef OVL_SHOUGATU_STEP_TABLES_T17500
#define OVL_SHOUGATU_STEP_TABLES_T17500
/* Step tables (T-3330, T-17500): one handler per scene step copies its table to the stack whole and
 * calls tbl.f[game_state.unk_110A]. The typedefs and externs were local to each .c file before. */
typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
typedef struct {
    void (*f[11])();
} FnTbl11; /* size 0x2C */
typedef struct {
    void (*f[12])();
} FnTbl12; /* size 0x30 */
typedef struct {
    void (*f[13])();
} FnTbl13; /* size 0x34 */
typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
typedef struct {
    void (*f[17])();
} FnTbl17; /* size 0x44 */
typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
typedef struct {
    void (*f[24])();
} FnTbl24; /* size 0x60 */
typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
typedef struct {
    void (*f[26])();
} FnTbl26; /* size 0x68 */
typedef struct {
    void (*f[27])();
} FnTbl27; /* size 0x6C */
typedef struct {
    void (*f[30])();
} FnTbl30; /* size 0x78 */
typedef struct {
    void (*f[31])();
} FnTbl31; /* size 0x7C */
typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
typedef struct {
    void (*f[34])();
} FnTbl34; /* size 0x88 */
typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
typedef struct {
    void (*f[48])();
} FnTbl48; /* size 0xC0 */
typedef struct {
    void (*f[50])();
} FnTbl50; /* size 0xC8 */
typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
typedef struct {
    void (*f[60])();
} FnTbl60; /* size 0xF0 */
typedef struct {
    void (*f[62])();
} FnTbl62; /* size 0xF8 */
extern FnTbl13 D_80143B28;
extern FnTbl23 D_80143B60;
extern FnTbl60 D_80143BC4;
extern FnTbl23 D_80143CC0;
extern FnTbl25 D_80143D24;
extern FnTbl31 D_80143DE0;
extern FnTbl50 D_80143FF8;
extern FnTbl24 D_80144E20;
extern FnTbl34 D_80144E80;
extern FnTbl20 D_80144F14;
extern FnTbl20 D_80144F64;
extern FnTbl37 D_80144FB4;
extern FnTbl30 D_80145048;
extern FnTbl24 D_801450C0;
extern FnTbl20 D_80145120;
extern FnTbl22 D_80145170;
extern FnTbl20 D_801451C8;
extern FnTbl20 D_80145218;
extern FnTbl62 D_801452B8;
extern FnTbl20 D_801453F8;
extern FnTbl20 D_80145448;
extern FnTbl25 D_80145498;
extern FnTbl20 D_801454FC;
extern FnTbl37 D_8014554C;
extern FnTbl26 D_801455E0;
extern FnTbl19 D_80145648;
extern FnTbl27 D_801456A0;
extern FnTbl20 D_8014570C;
extern FnTbl42 D_80145760;
extern FnTbl20 D_80145808;
extern FnTbl20 D_80145858;
extern FnTbl42 D_801458B0;
extern FnTbl25 D_80145A14;
extern FnTbl19 D_80145A80;
extern FnTbl33 D_80145F40;
extern FnTbl27 D_8014616C;
extern FnTbl31 D_80146244;
extern FnTbl17 D_801462C0;
extern FnTbl14 D_80146304;
extern FnTbl10 D_8014633C;
extern FnTbl14 D_80146364;
extern FnTbl12 D_8014639C;
extern FnTbl17 D_801463CC;
extern FnTbl17 D_80146410;
extern FnTbl11 D_80146454;
extern FnTbl12 D_80146480;
extern FnTbl40 D_80146804;
extern FnTbl51 D_801468A4;
extern FnTbl48 D_80146970;
#endif

#ifndef OVL_SHOUGATU_STACK_TABLES_T17500
#define OVL_SHOUGATU_STACK_TABLES_T17500
/* Constant tables that one function copies to the stack whole (T-17500). The copy is lw/sw pairs
 * plus an lh/sh or lwl/swl tail, so the element type sets the tail. */
typedef struct ShoShorts7 {
    /* 0x00 */ s16 v[7];
} ShoShorts7; /* size 0xE */
/* {0x46, 0x80, 0xA0, 0xC0, 0xE0, 0x100, 0x3E8} and {0, 3, 5, 7, 9, 0xB, 0x10}: two per function */
extern ShoShorts7 D_801460CC; /* christmas_shiori_stat_reward_apply */
extern ShoShorts7 D_801460DC;
extern ShoShorts7 D_801460EC; /* christmas_girl_select_and_reward */
extern ShoShorts7 D_801460FC;
extern ShoShorts7 D_8014610C; /* christmas_alternate_girl_select_and_reward */
extern ShoShorts7 D_8014611C;

/* Seven 5-byte Shift-JIS strings (35 bytes: the copy ends with lwl/swl); read as s[i]. */
typedef struct ShoStr7x5 {
    /* 0x00 */ u8 s[7][5];
} ShoStr7x5; /* size 0x23 */
extern ShoStr7x5 D_80143E5C; /* new_year_omikuji_scene */
extern ShoStr7x5 D_80143E80; /* new_year_omikuji_result_text */

/* Six pairs of colour words passed by value to christmas_result_gouraud_triangle_submit (`*8` index), and a 3 x 12 byte
 * selector table indexed [D_800E62BE][...]; both copied by christmas_result_choice_triangles_draw. */
typedef struct ShoColors6x2 {
    /* 0x00 */ u32 v[6][2];
} ShoColors6x2; /* size 0x30 */
typedef struct ShoBytes3x12 {
    /* 0x00 */ u8 v[36];
} ShoBytes3x12; /* size 0x24 */
extern ShoColors6x2 D_801461D8;
extern ShoBytes3x12 D_80146208;
extern u8 D_80146054;
extern u8 D_80146058;

/* Work arrays of the new_year_scene_particle_effect_update family, 50 entries each (counter D_801467FC < 0x32). The
 * original writes D_80146540[i - 1] as %lo(D_8014653C)+i*4; D_8014653C itself is a separate u8. */
extern s32 D_80146540[50];
extern s32 D_80146608[50];
extern s32 D_801466D0[50];
extern s16 D_80146798[50];
extern u32 D_80146800;
s32 new_year_scene_particle_effect_update(void);
s32 new_year_scene_quad_effect_update(void);
#endif

/* T-27066: new_year_mode_handler dispatches mode_stage cases 0-6 to distinct callbacks and resets title on other values; seasonal_mode_stage_dispatch.c:53-80. */
enum NewYearPrimaryModeStage {
    NEW_YEAR_PRIMARY_MODE_STAGE_0 = 0,
    NEW_YEAR_PRIMARY_MODE_STAGE_1 = 1,
    NEW_YEAR_PRIMARY_MODE_STAGE_2 = 2,
    NEW_YEAR_PRIMARY_MODE_STAGE_3 = 3,
    NEW_YEAR_PRIMARY_MODE_STAGE_4 = 4,
    NEW_YEAR_PRIMARY_MODE_STAGE_5 = 5,
    NEW_YEAR_PRIMARY_MODE_STAGE_6 = 6
};

/* T-27066: the 0x8013D180 dispatcher routes mode_stage cases 0-8 to distinct handlers and resets title on other values; seasonal_scene_dispatch.c:289-317. */
enum NewYearSecondaryModeStage {
    NEW_YEAR_SECONDARY_MODE_STAGE_0 = 0,
    NEW_YEAR_SECONDARY_MODE_STAGE_1 = 1,
    NEW_YEAR_SECONDARY_MODE_STAGE_2 = 2,
    NEW_YEAR_SECONDARY_MODE_STAGE_3 = 3,
    NEW_YEAR_SECONDARY_MODE_STAGE_4 = 4,
    NEW_YEAR_SECONDARY_MODE_STAGE_5 = 5,
    NEW_YEAR_SECONDARY_MODE_STAGE_6 = 6,
    NEW_YEAR_SECONDARY_MODE_STAGE_7 = 7,
    NEW_YEAR_SECONDARY_MODE_STAGE_8 = 8
};

/* T-27066: new_year_birthday_gate writes done bits 1 for the Shiori birthday branch, 4 for matching player birthday with low status, and 8 for the other status branch; scene_resource_tables.c:228,235,238. */
#define NEW_YEAR_GIRL_BIRTHDAY_DONE_BIT 1
#define NEW_YEAR_PLAYER_BIRTHDAY_LOW_STATUS_BIT 4
#define NEW_YEAR_PLAYER_BIRTHDAY_OTHER_STATUS_BIT 8

#endif /* OVL_SHOUGATU_H */
