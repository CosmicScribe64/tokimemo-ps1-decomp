#ifndef OVL_DATE_H
#define OVL_DATE_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_portrait_layout_set /* matched with void(s32) (main_api.h: void(u8)) */
#define MAIN_API_OVERRIDE_normal_date_girl_in /* implicit int declaration: date_choice_tables_init returns its result (void in main_api.h) */

#define MAIN_API_OVERRIDE_D_800CA158 /* matched as s16 (main_api.h: u8) */
#define MAIN_API_OVERRIDE_func_8009B430 /* matched with 3 arguments: the original leaves $a3 unset (main_api.h: 4) */

#include "common.h"
#include "game.h"

void func_8009B430(s32 arg0, u8 *arg1, s32 arg2);

typedef struct {
    void (*callbacks[52])();
} DateYumiWinterSteps; /* size 0xD0 */
extern DateYumiWinterSteps date_scene_yumi_winter_steps;

typedef struct {
    void (*callbacks[42])();
} DateMiraWinterSteps; /* size 0xA8 */
extern DateMiraWinterSteps date_scene_mira_winter_steps;

typedef struct {
    void (*callbacks[48])();
} DateAyakoSummerSteps; /* size 0xC0 */
extern DateAyakoSummerSteps date_scene_ayako_summer_steps;

extern s16 date_gift_group;
extern s16 date_gift_line;
void date_player_birthday_sections_init(void);
extern s32 D_8015CF80;
extern s32 D_8015CF84;
extern s32 D_8015CF88;
extern s32 D_8015CF8C;
extern s32 D_8015CF90;
extern s32 D_8015CF94;
extern s32 D_8015CF98;
void date_gift_sections_init(void);
extern u8 date_gift_choice;
extern s32 date_gift_text;
extern s32 date_gift_command;
extern s32 date_gift_voice;
extern s32 date_gift_saved_text;
extern s32 date_gift_saved_command;
extern s32 date_gift_saved_voice;
extern s16 date_gift_saved_group;
void date_gift_name_set(void);
extern s16 date_player_birthday_group;
extern s16 date_player_birthday_line;
extern s32 date_player_birthday_text;
extern s32 date_player_birthday_command;
extern s32 date_player_birthday_voice;
extern s32 date_scene_text;
extern s32 date_scene_command;
extern s32 date_scene_voice;
extern s16 date_park_parade_group;
extern s16 date_park_parade_line;
extern s32 D_8015BDE4;
extern s32 D_8015BE50;
extern s32 D_8015BEBC;
extern s16 date_meeting_group; /* DateMeetingGroup */
extern s16 date_meeting_line;
extern s32 D_8015BD94;
extern s32 D_8015BDA4;
extern s32 D_8015BDB4;
s32 func_8013272C(void);
s32 date_scene_choose(void);
void date_ride_dispatch(void);
/* T-24051: callbacks used by the large DATE dispatcher. */
void date_shrine_fireworks_update(void);
void date_central_park_particles_hide(void);
s32 date_central_park_particles_update(void);
s32 date_parade_sign_palette_cycle(void);
extern u8 date_girl_side;
extern s32 D_8015EE20;
extern s16 date_virtual_ship_phase;
extern s16 date_virtual_ship_subphase;
extern s16 date_virtual_ship_step;
extern s16 D_8015DE4C;
extern s32 D_8015DE28;
extern s32 D_8015DE2C;
extern s32 D_8015DE30;
extern s32 D_8015E714;
extern s32 D_8015E718;
extern s32 D_8015E71C;
extern s32 D_8015E720;
extern s32 D_8015E724;
extern s32 D_8015E728;
extern s32 D_8015E72C;
extern s32 D_8015F984;
extern s32 D_8015F988;
extern s32 D_8015F98C;
extern s32 D_8015F990;
extern s32 D_8015F994;
extern s32 D_8015F998;
extern s32 D_8015F99C;
extern s32 date_shared_section_text;
extern s32 D_8015BD98;
extern s32 D_8015BD9C;
extern s32 date_shared_section_command;
extern s32 D_8015BDA8;
extern s32 D_8015BDAC;
extern s32 date_shared_section_voice;
extern s32 D_8015BDB8;
extern s32 D_8015BDBC;
extern s32 D_8015E2FC;
extern s32 D_8015E300;
extern s32 D_8015E304;
extern s16 D_8015E308;
extern s32 D_8015E30C;
extern s32 D_8015E220;
extern s32 D_8015E224;
extern s32 D_8015E228;
extern s16 D_8015E22C;
extern s32 D_8015E230;
extern s32 D_8015E234;
extern s32 D_8015E238;
extern s32 D_8015E23C;
extern s32 D_8015E240;
extern s32 D_8015E244;
extern s32 D_8015E248;
extern s32 D_8015E24C;
extern s32 D_8015E250;
extern s32 D_8015E254;
extern s16 D_8015E258;
extern s32 D_8015E25C;
extern s32 D_8015E260;
extern s32 D_8015E264;
extern s32 D_8015E268;
extern s32 D_8015E26C;
extern s32 D_8015E270;
extern s32 D_8015E274;
extern s32 D_8015E278;
extern s32 D_8015E27C;
extern s32 D_8015E280;
extern s16 D_8015E284;
extern s32 D_8015E288;
extern s32 D_8015E28C;
extern s32 D_8015E290;
extern s32 D_8015E294;
extern s32 D_8015E298;
extern s32 D_8015E29C;
extern s32 D_8015E2A0;
extern s32 D_8015E2A4;
extern s32 D_8015E2A8;
extern s32 D_8015E2AC;
extern s16 D_8015E2B0;
extern s32 D_8015E2B4;
extern s32 D_8015E2B8;
extern s32 D_8015E2BC;
extern s32 D_8015E2C0;
extern s32 D_8015E2C4;
extern s32 D_8015E2C8;
extern s32 D_8015E2CC;
extern s32 D_8015E2D0;
extern s32 D_8015E2D4;
extern s32 D_8015E2D8;
extern s16 D_8015E2DC;
extern s32 D_8015E2E0;
extern s32 D_8015E2E4;
extern s32 D_8015E2E8;
extern s32 D_8015E2EC;
extern s32 D_8015E2F0;
extern s32 D_8015E2F4;
extern s32 D_8015E2F8;
extern s32 D_8015E690;
extern s32 D_8015E694;
extern s32 D_8015E698;
extern s16 D_8015E69C;
extern s32 D_8015E6A0;
extern s32 D_8015E6A4;
extern s32 D_8015E6A8;
extern s32 D_8015E6AC;
extern s32 D_8015E6B0;
extern s32 D_8015E6B4;
extern s32 D_8015E6B8;
extern s32 D_8015E6BC;
extern s32 D_8015E6C0;
extern s32 D_8015E6C4;
extern s16 D_8015E6C8;
extern s32 D_8015E6CC;
extern s32 D_8015E6D0;
extern s32 D_8015E6D4;
extern s32 D_8015E6D8;
extern s32 D_8015E6DC;
extern s32 D_8015E6E0;
extern s32 D_8015E6E4;
extern s32 D_8015E6E8;
extern s32 D_8015E6EC;
extern s32 D_8015E6F0;
extern s16 D_8015E6F4;
extern s32 D_8015E6F8;
extern s32 D_8015E6FC;
extern s32 D_8015E700;
extern s32 D_8015E704;
extern s32 D_8015E708;
extern s32 D_8015E70C;
extern s32 D_8015E710;
extern s32 D_8015ED70;
extern s32 D_8015ED74;
extern s32 D_8015ED78;
extern s16 D_8015ED7C;
extern s32 D_8015ED80;
extern s32 D_8015ED84;
extern s32 D_8015ED88;
extern s32 D_8015ED8C;
extern s32 D_8015ED90;
extern s32 D_8015ED94;
extern s32 D_8015ED98;
extern s32 D_8015ED9C;
extern s32 D_8015EDA0;
extern s32 D_8015EDA4;
extern s16 D_8015EDA8;
extern s32 D_8015EDAC;
extern s32 D_8015EDB0;
extern s32 D_8015EDB4;
extern s32 D_8015EDB8;
extern s32 D_8015EDBC;
extern s32 D_8015EDC0;
extern s32 D_8015EDC4;
extern s32 D_8015EDC8;
extern s32 D_8015EDCC;
extern s32 D_8015EDD0;
extern s16 D_8015EDD4;
extern s32 D_8015EDD8;
extern s32 D_8015EDDC;
extern s32 D_8015EDE0;
extern s32 D_8015EDE4;
extern s32 D_8015EDE8;
extern s32 D_8015EDEC;
extern s32 D_8015EDF0;
extern s32 D_8015F160;
extern s32 D_8015F164;
extern s32 D_8015F168;
extern s16 D_8015F16C;
extern s32 D_8015F170;
extern s32 D_8015F174;
extern s32 D_8015F178;
extern s32 D_8015F17C;
extern s32 D_8015F180;
extern s32 D_8015F184;
extern s32 D_8015F188;
extern s32 D_8015F18C;
extern s32 D_8015F190;
extern s32 D_8015F194;
extern s16 D_8015F198;
extern s32 D_8015F19C;
extern s32 D_8015F1A0;
extern s32 D_8015F1A4;
extern s32 D_8015F1A8;
extern s32 D_8015F1AC;
extern s32 D_8015F1B0;
extern s32 D_8015F1B4;
extern s32 D_8015F1E4;
extern s32 D_8015F1E8;
extern s32 D_8015F1EC;
extern s16 D_8015F1F0;
extern s32 D_8015F1F4;
extern s32 D_8015F1F8;
extern s32 D_8015F1FC;
extern s32 D_8015F200;
extern s32 D_8015F204;
extern s32 D_8015F208;
extern s32 D_8015F20C;
extern s32 D_8015F520;
extern s32 D_8015F524;
extern s32 D_8015F528;
extern s16 D_8015F52C;
extern s32 D_8015F530;
extern s32 D_8015F534;
extern s32 D_8015F538;
extern s32 D_8015F53C;
extern s32 D_8015F540;
extern s32 D_8015F544;
extern s32 D_8015F548;
extern s32 D_8015F54C;
extern s32 D_8015F550;
extern s32 D_8015F554;
extern s16 D_8015F558;
extern s32 D_8015F55C;
extern s32 D_8015F560;
extern s32 D_8015F564;
extern s32 D_8015F568;
extern s32 D_8015F56C;
extern s32 D_8015F570;
extern s32 D_8015F574;
extern s32 D_8015F578;
extern s32 D_8015F57C;
extern s32 D_8015F580;
extern s16 D_8015F584;
extern s32 D_8015F588;
extern s32 D_8015F58C;
extern s32 D_8015F590;
extern s32 D_8015F594;
extern s32 D_8015F598;
extern s32 D_8015F59C;
extern s32 D_8015F5A0;
extern s32 D_8015F5A4;
extern s32 D_8015F5A8;
extern s32 D_8015F5AC;
extern s16 D_8015F5B0;
extern s32 D_8015F5B4;
extern s32 D_8015F5B8;
extern s32 D_8015F5BC;
extern s32 D_8015F5C0;
extern s32 D_8015F5C4;
extern s32 D_8015F5C8;
extern s32 D_8015F5CC;
extern s32 D_8015F900;
extern s32 D_8015F904;
extern s32 D_8015F908;
extern s16 D_8015F90C;
extern s32 D_8015F910;
extern s32 D_8015F914;
extern s32 D_8015F918;
extern s32 D_8015F91C;
extern s32 D_8015F920;
extern s32 D_8015F924;
extern s32 D_8015F928;
extern s32 D_8015F92C;
extern s32 D_8015F930;
extern s32 D_8015F934;
extern s16 D_8015F938;
extern s32 D_8015F93C;
extern s32 D_8015F940;
extern s32 D_8015F944;
extern s32 D_8015F948;
extern s32 D_8015F94C;
extern s32 D_8015F950;
extern s32 D_8015F954;
extern s32 D_8015F958;
extern s32 D_8015F95C;
extern s32 D_8015F960;
extern s16 D_8015F964;
extern s32 D_8015F968;
extern s32 D_8015F96C;
extern s32 D_8015F970;
extern s32 D_8015F974;
extern s32 D_8015F978;
extern s32 D_8015F97C;
extern s32 D_8015F980;
extern s32 D_8015FCE0;
extern s32 D_8015FCE4;
extern s32 D_8015FCE8;
extern s16 D_8015FCEC;
extern s32 D_8015FCF0;
extern s32 D_8015FCF4;
extern s32 D_8015FCF8;
extern s32 D_8015FCFC;
extern s32 D_8015FD00;
extern s32 D_8015FD04;
extern s32 D_8015FD08;
extern s32 D_8015FD0C;
extern s32 D_8015FD10;
extern s32 D_8015FD14;
extern s16 D_8015FD18;
extern s32 D_8015FD1C;
extern s32 D_8015FD20;
extern s32 D_8015FD24;
extern s32 D_8015FD28;
extern s32 D_8015FD2C;
extern s32 D_8015FD30;
extern s32 D_8015FD34;
extern s32 D_8015FD38;
extern s32 D_8015FD3C;
extern s32 D_8015FD40;
extern s16 D_8015FD44;
extern s32 D_8015FD48;
extern s32 D_8015FD4C;
extern s32 D_8015FD50;
extern s32 D_8015FD54;
extern s32 D_8015FD58;
extern s32 D_8015FD5C;
extern s32 D_8015FD60;
extern s32 D_8015FD64;
extern s32 D_8015FD68;
extern s32 D_8015FD6C;
extern s16 D_8015FD70;
extern s32 D_8015FD74;
extern s32 D_8015FD78;
extern s32 D_8015FD7C;
extern s32 D_8015FD80;
extern s32 D_8015FD84;
extern s32 D_8015FD88;
extern s32 D_8015FD8C;
extern s32 D_801600A0;
extern s32 D_801600A4;
extern s32 D_801600A8;
extern s16 D_801600AC;
extern s32 D_801600B0;
extern s32 D_801600B4;
extern s32 D_801600B8;
extern s32 D_801600BC;
extern s32 D_801600C0;
extern s32 D_801600C4;
extern s32 D_801600C8;
extern s32 D_801600CC;
extern s32 D_801600D0;
extern s32 D_801600D4;
extern s16 D_801600D8;
extern s32 D_801600DC;
extern s32 D_801600E0;
extern s32 D_801600E4;
extern s32 D_801600E8;
extern s32 D_801600EC;
extern s32 D_801600F0;
extern s32 D_801600F4;
extern s32 D_801600F8;
extern s32 D_801600FC;
extern s32 D_80160100;
extern s16 D_80160104;
extern s32 D_80160108;
extern s32 D_8016010C;
extern s32 D_80160110;
extern s32 D_80160114;
extern s32 D_80160118;
extern s32 D_8016011C;
extern s32 D_80160120;
extern s32 D_80160124;
extern s32 D_80160128;
extern s32 D_8016012C;
extern s16 D_80160130;
extern s32 D_80160134;
extern s32 D_80160138;
extern s32 D_8016013C;
extern s32 D_80160140;
extern s32 D_80160144;
extern s32 D_80160148;
extern s32 D_8016014C;
extern s32 D_80160498;
extern s32 D_8016049C;
extern s32 D_801604A0;
extern s16 D_801604A4;
extern s32 D_801604A8;
extern s32 D_801604AC;
extern s32 D_801604B0;
extern s32 D_801604B4;
extern s32 D_801604B8;
extern s32 D_801604BC;
extern s32 D_801604C0;
extern s32 D_801604C4;
extern s32 D_801604C8;
extern s32 D_801604CC;
extern s16 D_801604D0;
extern s32 D_801604D4;
extern s32 D_801604D8;
extern s32 D_801604DC;
extern s32 D_801604E0;
extern s32 D_801604E4;
extern s32 D_801604E8;
extern s32 D_801604EC;
extern s32 D_801607E0;
extern s32 D_801607E4;
extern s32 D_801607E8;
extern s16 D_801607EC;
extern s32 D_801607F0;
extern s32 D_801607F4;
extern s32 D_801607F8;
extern s32 D_801607FC;
extern s32 D_80160800;
extern s32 D_80160804;
extern s32 D_80160808;
extern s32 D_8016080C;
extern s32 D_80160810;
extern s32 D_80160814;
extern s16 D_80160818;
extern s32 D_8016081C;
extern s32 D_80160820;
extern s32 D_80160824;
extern s32 D_80160828;
extern s32 D_8016082C;
extern s32 D_80160830;
extern s32 D_80160834;
extern s32 D_80160838;
extern s32 D_8016083C;
extern s32 D_80160840;
extern s16 D_80160844;
extern s32 D_80160848;
extern s32 D_8016084C;
extern s32 D_80160850;
extern s32 D_80160854;
extern s32 D_80160858;
extern s32 D_8016085C;
extern s32 D_80160860;
extern s32 D_80160864;
extern s32 D_80160868;
extern s32 D_8016086C;
extern s16 D_80160870;
extern s32 D_80160874;
extern s32 D_80160878;
extern s32 D_8016087C;
extern s32 D_80160880;
extern s32 D_80160884;
extern s32 D_80160888;
extern s32 D_8016088C;
void date_scene_speak(void);
void date_virtual_ship_load(void);
void date_virtual_ship_next_phase(void);
void date_central_park_particles_init(void);
void date_scene_yuko_spring_data_init_2(void);
void date_scene_yuko_spring_data_init(void);
void date_parade_resources_init(void);
void date_scene_yuko_summer_data_init(void);
void date_scene_yuko_winter_data_init(void);
void date_scene_yuko_december_data_init(void);
void date_scene_shiori_autumn_data_init(void);
void date_scene_shiori_spring_data_init(void);
void date_scene_shiori_winter_data_init(void);
void date_scene_yuina_spring_data_init(void);
void date_scene_yuina_autumn_data_init(void);
void date_scene_ayako_spring_data_init(void);
void date_scene_ayako_summer_data_init(void);
void date_scene_ayako_autumn_data_init(void);
void date_scene_ayako_winter_data_init(void);
void date_scene_ayako_spring_stripe_draw(); /* old-style definition with s16 parameters, called before it (T-17204) */
void date_scene_mira_spring_data_init(void);
void date_scene_mira_summer_data_init(void);
void date_scene_mira_autumn_data_init(void);
void date_scene_mira_winter_data_init(void);
void date_scene_mio_spring_data_init(void);
void date_scene_mio_summer_data_init(void);
void date_scene_mio_autumn_data_init(void);
void date_scene_yukari_spring_data_init(void);
void date_scene_yukari_summer_data_init(void);
void date_scene_yukari_autumn_data_init(void);
void date_scene_yukari_winter_data_init(void);
void date_scene_nozomi_spring_data_init(void);
void date_scene_nozomi_summer_data_init(void);
void date_scene_nozomi_autumn_data_init(void);
void date_scene_nozomi_winter_data_init(void);
void date_scene_megumi_spring_data_init(void);
void date_scene_megumi_summer_data_init(void);
void date_scene_megumi_autumn_data_init(void);
void date_scene_saki_spring_data_init(void);
void date_scene_saki_summer_data_init(void);
void date_scene_saki_autumn_data_init(void);
void date_scene_saki_winter_data_init(void);
void date_scene_yumi_spring_data_init(void);
void date_scene_yumi_summer_data_init(void);
void date_scene_yumi_autumn_data_init(void);
void date_scene_yumi_winter_data_init(void);
void date_shared_sections_init(void);
void date_choice_tables_init_body(void);
void date_scene_ayako_spring_data_init_2(void);
void date_fireworks_resources_init(void);
void date_scene_shiori_summer_data_init(void);
s32 portrait_layout_set(s32 arg0);
void date_player_birthday_pick_sections(void);
extern u8 date_game_center_medals_open;
extern u8 date_arrival; /* DateArrival */
extern u8 date_player_waited;
void date_art_museum_choice_face(void);
extern s8 date_ride_index; /* DateRide */
extern u8 date_movie_index;
extern u8 date_movie_group;
extern s16 date_movie_line;
extern u8 date_exhibit_artist_active;
extern u8 date_exhibit_sculpture_active;
extern u8 date_exhibit_painters_active;
extern u8 date_first_sunday;
extern s32 D_8015DEE8;
extern s32 D_8015E024;
extern s32 D_8015E160;
extern u8 date_park_new_rides;
extern u8 date_park_newest_rides;
extern u8 date_ride_chosen;
void date_load_ski_portrait(void);
void date_load_shrine_portrait(void);
void date_virtual_ship_setup(void);
s32 date_virtual_ship_run(void);
void date_virtual_ship_finish(void);
extern u8 date_groups_per_variant;
s32 date_arrival_greeting_speak();
void date_scene_sections_init(void);
extern s8 D_8015E204;
void date_choice_classes_derive(void);
void date_amusement_park_ride_setup(void);
void date_mode_stage_arrival(void);
void date_mode_stage_place(void);
void date_mode_stage_player_birthday(void);
void date_mode_stage_girl_birthday(void);
extern s32 D_8015DECC;
extern s32 D_8015E008;
extern s32 D_8015E144;
void date_gradient_quad_draw(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11);
extern u32 D_8015B648;
extern u32 D_8015B64C;
extern u32 D_8015B650;
void date_scene_yuina_summer_data_init(void);
extern s32 date_section_text;
extern s32 D_8015BDC4;
extern s32 D_8015BDC8;
extern s32 D_8015BDCC;
extern s32 D_8015BDD0;
extern s32 D_8015BDD4;
extern s32 D_8015BDD8;
extern s32 D_8015BDDC;
extern s32 D_8015BDE0;
extern s32 D_8015BDE8;
extern s32 D_8015BDEC;
extern s32 D_8015BDF0;
extern s32 D_8015BDF4;
extern s32 D_8015BDF8;
extern s32 D_8015BDFC;
extern s32 D_8015BE00;
extern s32 D_8015BE04;
extern s32 D_8015BE08;
extern s32 D_8015BE0C;
extern s32 D_8015BE10;
extern s32 D_8015BE14;
extern s32 D_8015BE18;
extern s32 D_8015BE1C;
extern s32 D_8015BE20;
extern s32 D_8015BE24;
extern s32 D_8015BE28;
extern s32 date_section_command;
extern s32 D_8015BE30;
extern s32 D_8015BE34;
extern s32 D_8015BE38;
extern s32 D_8015BE3C;
extern s32 D_8015BE40;
extern s32 D_8015BE44;
extern s32 D_8015BE48;
extern s32 D_8015BE4C;
extern s32 D_8015BE54;
extern s32 D_8015BE58;
extern s32 D_8015BE5C;
extern s32 D_8015BE60;
extern s32 D_8015BE64;
extern s32 D_8015BE68;
extern s32 D_8015BE6C;
extern s32 D_8015BE70;
extern s32 D_8015BE74;
extern s32 D_8015BE78;
extern s32 D_8015BE7C;
extern s32 D_8015BE80;
extern s32 D_8015BE84;
extern s32 D_8015BE88;
extern s32 D_8015BE8C;
extern s32 D_8015BE90;
extern s32 D_8015BE94;
extern s32 date_section_voice;
extern s32 D_8015BE9C;
extern s32 D_8015BEA0;
extern s32 D_8015BEA4;
extern s32 D_8015BEA8;
extern s32 D_8015BEAC;
extern s32 D_8015BEB0;
extern s32 D_8015BEB4;
extern s32 D_8015BEB8;
extern s32 D_8015BEC0;
extern s32 D_8015BEC4;
extern s32 D_8015BEC8;
extern s32 D_8015BECC;
extern s32 D_8015BED0;
extern s32 D_8015BED4;
extern s32 D_8015BED8;
extern s32 D_8015BEDC;
extern s32 D_8015BEE0;
extern s32 D_8015BEE4;
extern s32 D_8015BEE8;
extern s32 D_8015BEEC;
extern s32 D_8015BEF0;
extern s32 D_8015BEF4;
extern s32 D_8015BEF8;
extern s32 D_8015BEFC;
extern s32 D_8015BF00;
s32 date_scene_yuko_spring_picture_animate(void);
void date_scene_yuko_autumn_beam_lights(void);
extern s32 D_8015DE60;
extern s32 D_8015DF9C;
extern s32 D_8015E0D8;

extern s32 D_8015DEBC;
extern s32 D_8015DFF8;
extern s32 D_8015E134;

extern s32 D_8015DF1C;
extern s32 D_8015E058;
extern s32 D_8015E194;

extern s32 D_8015DEC0;
extern s32 D_8015DFFC;
extern s32 D_8015E138;
extern s32 D_8015EDF8;
extern s32 D_8015EDFC;
extern s32 D_8015EE04;
extern s32 D_8015EE08;
extern s32 D_8015EE0C;
extern s32 D_8015EE14;
extern s32 D_8015EE18;

extern s32 D_8015EE10;
extern s32 D_8015EE1C;
extern s32 D_8015EDF4;
extern s16 D_8015EE00;
extern s32 D_8015B5C0;
extern s32 D_8015B5C4;
extern s32 D_8015B5C8;
extern s16 D_8015B5CC;
extern s32 D_8015B5D0;
extern s32 D_8015B5D4;
extern s32 D_8015B5D8;
extern s32 D_8015B5DC;
extern s32 D_8015B5E0;
extern s32 D_8015B5E4;
extern s32 D_8015B5E8;
extern s32 D_8015B5EC;
extern s32 D_8015B5F0;
extern s32 D_8015B5F4;
extern s32 D_8015B5F8;
extern s32 D_8015B5FC;
extern s32 D_8015B600;
extern s32 D_8015B604;
extern s32 D_8015B608;
extern s32 D_8015B60C;
extern s32 D_8015B610;
extern s32 D_8015B614;
extern s32 D_8015B618;
extern s32 D_8015B61C;
extern s32 D_8015B620;
extern s16 D_8015B624;
extern s16 D_8015B628;
extern s32 D_8015B62C;
extern s32 D_8015B630;
extern s16 date_opening_group; /* halfword stores in date_arrival_init (T-11580) */
extern s16 date_opening_line;
extern s16 date_arrival_group;
extern u8 *date_class_table_shrine;
extern u8 *date_face_table_shrine;
extern s16 date_arrival_line;
extern s16 date_closing_group;
extern s16 date_closing_line;
void date_choice_score(void);

void date_scene_yuina_spring_radial_lines();

extern u8 date_school_year;

extern s32 D_8015DE50;
extern s32 D_8015DE54;
extern s32 D_8015DE58;
extern s32 D_8015DE5C;
extern s32 D_8015DE64;
extern s32 D_8015DE68;
extern s32 D_8015DE6C;
extern s32 D_8015DE70;
extern s32 D_8015DE74;
extern s32 D_8015DE78;
extern s32 D_8015DE7C;
extern s32 D_8015DE80;
extern s32 D_8015DE84;
extern s32 D_8015DE88;
extern s32 D_8015DE8C;
extern s32 D_8015DE90;
extern s32 D_8015DE94;
extern s32 D_8015DE98;
extern s32 D_8015DE9C;
extern s32 D_8015DEA0;
extern s32 D_8015DEA4;
extern s32 D_8015DEA8;
extern s32 D_8015DEAC;
extern s32 D_8015DEB0;
extern s32 D_8015DEB4;
extern s32 D_8015DEB8;
extern s32 D_8015DEC4;
extern s32 D_8015DEC8;
extern s32 D_8015DED0;
extern s32 D_8015DED4;
extern s32 D_8015DED8;
extern s32 D_8015DEDC;
extern s32 D_8015DEE0;
extern s32 D_8015DEE4;
extern s32 D_8015DEEC;
extern s32 D_8015DEF0;
extern s32 D_8015DEF4;
extern s32 D_8015DEF8;
extern s32 D_8015DEFC;
extern s32 D_8015DF00;
extern s32 D_8015DF04;
extern s32 D_8015DF08;
extern s32 D_8015DF0C;
extern s32 D_8015DF10;
extern s32 D_8015DF14;
extern s32 D_8015DF18;
extern s32 D_8015DF20;
extern s32 D_8015DF24;
extern s32 D_8015DF28;
extern s32 D_8015DF2C;
extern s32 D_8015DF30;
extern s32 D_8015DF34;
extern s32 D_8015DF38;
extern s32 D_8015DF3C;
extern s32 D_8015DF40;
extern s32 D_8015DF44;
extern s32 D_8015DF48;
extern s32 D_8015DF4C;
extern s32 D_8015DF50;
extern s32 D_8015DF54;
extern s32 D_8015DF58;
extern s32 D_8015DF5C;
extern s32 D_8015DF60;
extern s32 D_8015DF64;
extern s32 D_8015DF68;
extern s32 D_8015DF6C;
extern s32 D_8015DF70;
extern s32 D_8015DF74;
extern s32 D_8015DF78;
extern s32 D_8015DF7C;
extern s32 D_8015DF80;
extern s32 D_8015DF84;
extern s32 D_8015DF88;
extern s32 D_8015DF8C;
extern s32 D_8015DF90;
extern s32 D_8015DF94;
extern s32 D_8015DF98;
extern s32 D_8015DFA0;
extern s32 D_8015DFA4;
extern s32 D_8015DFA8;
extern s32 D_8015DFAC;
extern s32 D_8015DFB0;
extern s32 D_8015DFB4;
extern s32 D_8015DFB8;
extern s32 D_8015DFBC;
extern s32 D_8015DFC0;
extern s32 D_8015DFC4;
extern s32 D_8015DFC8;
extern s32 D_8015DFCC;
extern s32 D_8015DFD0;
extern s32 D_8015DFD4;
extern s32 D_8015DFD8;
extern s32 D_8015DFDC;
extern s32 D_8015DFE0;
extern s32 D_8015DFE4;
extern s32 D_8015DFE8;
extern s32 D_8015DFEC;
extern s32 D_8015DFF0;
extern s32 D_8015DFF4;
extern s32 D_8015E000;
extern s32 D_8015E004;
extern s32 D_8015E00C;
extern s32 D_8015E010;
extern s32 D_8015E014;
extern s32 D_8015E018;
extern s32 D_8015E01C;
extern s32 D_8015E020;
extern s32 D_8015E028;
extern s32 D_8015E02C;
extern s32 D_8015E030;
extern s32 D_8015E034;
extern s32 D_8015E038;
extern s32 D_8015E03C;
extern s32 D_8015E040;
extern s32 D_8015E044;
extern s32 D_8015E048;
extern s32 D_8015E04C;
extern s32 D_8015E050;
extern s32 D_8015E054;
extern s32 D_8015E05C;
extern s32 D_8015E060;
extern s32 D_8015E064;
extern s32 D_8015E068;
extern s32 D_8015E06C;
extern s32 D_8015E070;
extern s32 D_8015E074;
extern s32 D_8015E078;
extern s32 D_8015E07C;
extern s32 D_8015E080;
extern s32 D_8015E084;
extern s32 D_8015E088;
extern s32 D_8015E08C;
extern s32 D_8015E090;
extern s32 D_8015E094;
extern s32 D_8015E098;
extern s32 D_8015E09C;
extern s32 D_8015E0A0;
extern s32 D_8015E0A4;
extern s32 D_8015E0A8;
extern s32 D_8015E0AC;
extern s32 D_8015E0B0;
extern s32 D_8015E0B4;
extern s32 D_8015E0B8;
extern s32 D_8015E0BC;
extern s32 D_8015E0C0;
extern s32 D_8015E0C4;
extern s32 D_8015E0C8;
extern s32 D_8015E0CC;
extern s32 D_8015E0D0;
extern s32 D_8015E0D4;
extern s32 D_8015E0DC;
extern s32 D_8015E0E0;
extern s32 D_8015E0E4;
extern s32 D_8015E0E8;
extern s32 D_8015E0EC;
extern s32 D_8015E0F0;
extern s32 D_8015E0F4;
extern s32 D_8015E0F8;
extern s32 D_8015E0FC;
extern s32 D_8015E100;
extern s32 D_8015E104;
extern s32 D_8015E108;
extern s32 D_8015E10C;
extern s32 D_8015E110;
extern s32 D_8015E114;
extern s32 D_8015E118;
extern s32 D_8015E11C;
extern s32 D_8015E120;
extern s32 D_8015E124;
extern s32 D_8015E128;
extern s32 D_8015E12C;
extern s32 D_8015E130;
extern s32 D_8015E13C;
extern s32 D_8015E140;
extern s32 D_8015E148;
extern s32 D_8015E14C;
extern s32 D_8015E150;
extern s32 D_8015E154;
extern s32 D_8015E158;
extern s32 D_8015E15C;
extern s32 D_8015E164;
extern s32 D_8015E168;
extern s32 D_8015E16C;
extern s32 D_8015E170;
extern s32 D_8015E174;
extern s32 D_8015E178;
extern s32 D_8015E17C;
extern s32 D_8015E180;
extern s32 D_8015E184;
extern s32 D_8015E188;
extern s32 D_8015E18C;
extern s32 D_8015E190;
extern s32 D_8015E198;
extern s32 D_8015E19C;
extern s32 D_8015E1A0;
extern s32 D_8015E1A4;
extern s32 D_8015E1A8;
extern s32 D_8015E1AC;
extern s32 D_8015E1B0;
extern s32 D_8015E1B4;
extern s32 D_8015E1B8;
extern s32 D_8015E1BC;
extern s32 D_8015E1C0;
extern s32 D_8015E1C4;
extern s32 D_8015E1C8;
extern s32 D_8015E1CC;
extern s32 D_8015E1D0;
extern s32 D_8015E1D4;
extern s32 D_8015E1D8;
extern s32 D_8015E1DC;
extern s32 D_8015E1E0;
extern s32 D_8015E1E4;
extern s32 D_8015E1E8;
extern s32 D_8015E1EC;
extern s32 D_8015E1F0;
extern s32 D_8015E1F4;
extern s32 D_8015E1F8;
extern s32 D_8015E1FC;
extern s32 D_8015E200;
extern s32 D_8015CE80;
extern s32 D_8015CE84;
extern s32 D_8015CE88;
extern s32 D_8015CE8C;
extern s32 D_8015CE90;
extern s32 D_8015CE94;
extern s32 D_8015CE98;
extern s32 D_8015CE9C;
extern s32 D_8015CEA0;
extern s32 D_8015CEA4;
extern s32 D_8015CEA8;
extern s32 D_8015CEAC;
extern s32 D_8015CEB0;
extern s32 D_8015CEB4;
extern s32 D_8015CEB8;
extern s32 D_8015CEBC;
extern s32 D_8015CEC0;
extern s32 D_8015CEC4;
extern s32 D_8015CEC8;
extern s32 D_8015CECC;
extern s32 D_8015CED0;
extern s32 D_8015CED4;
extern s32 D_8015CED8;
extern s32 D_8015CEDC;
extern s32 D_8015CEE0;
extern s32 D_8015CEE4;
extern s32 D_8015CEE8;
extern s32 D_8015CEEC;
extern s32 D_8015CEF0;
extern s32 D_8015CEF4;
extern s32 D_8015CEF8;
extern s32 D_8015CEFC;
extern s32 D_8015CF00;
extern s32 D_8015CF04;
extern s32 D_8015CF08;
extern s32 D_8015CF0C;
extern s32 D_8015CF10;
extern s32 D_8015CF14;
extern s32 D_8015CF18;
extern s32 D_8015CFD0;
extern s32 D_8015CFD4;
extern s32 D_8015CFD8;
extern s32 D_8015CFDC;
extern s32 D_8015CFE0;
extern s32 D_8015CFE4;
extern s32 D_8015CFE8;
extern s32 D_8015CFEC;
extern s32 D_8015CFF0;
extern s32 D_8015CFF4;
extern s32 D_8015CFF8;
extern s32 D_8015CFFC;
extern s32 D_8015D000;
extern s32 D_8015D004;
extern s32 D_8015D008;
extern s32 D_8015D00C;
extern s32 D_8015D010;
extern s32 D_8015D014;
extern s32 D_8015D018;
extern s32 D_8015D01C;
extern s32 D_8015D020;
extern s32 D_8015D024;
extern s32 D_8015D028;
extern s32 D_8015D02C;
extern s32 D_8015D030;
extern s32 D_8015D034;
extern s32 D_8015D038;
extern s32 D_8015D03C;
extern s32 D_8015D040;
extern s32 D_8015D044;
extern s32 D_8015D048;
extern s32 D_8015D04C;
extern s32 D_8015D050;
extern s32 D_8015D054;
extern s32 D_8015D058;
extern s32 D_8015D05C;
extern s32 D_8015D060;
extern s32 D_8015D064;
extern s32 D_8015D068;
extern s32 D_8015EA40;
extern s32 D_8015EA44;
extern s32 D_8015EA48;
extern s32 D_8015EA4C;
extern s32 D_8015EA50;
extern s32 D_8015EA54;
extern s32 D_8015EA58;
extern u8 *D_8015EA5C;
extern u8 *D_8015EA60;
extern u8 *D_8015EA64;
extern s16 D_8015EA68;
extern u8 *D_8015EA6C;
extern u8 *D_8015EA70;
extern u8 *D_8015EA74;
extern u8 *D_8015EA78;
extern u8 *D_8015EA7C;
extern u8 *D_8015EA80;
extern u8 *D_8015EA84;
extern u8 *D_8015EA88;
extern u8 *D_8015EA8C;
extern u8 *D_8015EA90;
extern s16 D_8015EA94;
extern u8 *D_8015EA98;
extern u8 *D_8015EA9C;
extern u8 *D_8015EAA0;
extern u8 *D_8015EAA4;
extern u8 *D_8015EAA8;
extern u8 *D_8015EAAC;
extern u8 *D_8015EAB0;
extern u8 *D_8015EAB4;
extern u8 *D_8015EAB8;
extern u8 *D_8015EABC;
extern u8 *D_8015EAC0;
extern u8 *D_8015EAC4;
extern s16 D_8015EAC8;
extern u8 *D_8015EACC;
extern u8 *D_8015EAD0;
extern u8 *D_8015EAD4;
extern u8 *D_8015EAD8;
extern u8 *D_8015EADC;
extern u8 *D_8015EAE0;
extern u8 *D_8015EAE4;
extern s32 D_80160440;
extern s32 D_80160444;
extern s32 D_80160448;
extern s16 D_8016044C;
extern s32 D_80160450;
extern s32 D_80160454;
extern s32 D_80160458;
extern s32 D_8016045C;
extern s32 D_80160460;
extern s32 D_80160464;
extern s32 D_80160468;
extern u8 *D_8015F1B8;
extern u8 *D_8015F1BC;
extern u8 *D_8015F1C0;
extern s16 D_8015F1C4;
extern u8 *D_8015F1C8;
extern u8 *D_8015F1CC;
extern u8 *D_8015F1D0;
extern u8 *D_8015F1D4;
extern u8 *D_8015F1D8;
extern u8 *D_8015F1DC;
extern u8 *D_8015F1E0;
extern s32 D_8016046C;
extern s32 D_80160470;
extern s32 D_80160474;
extern s16 D_80160478;
extern s32 D_8016047C;
extern s32 D_80160480;
extern s32 D_80160484;
extern s32 D_80160488;
extern s32 D_8016048C;
extern s32 D_80160490;
extern s32 D_80160494;
extern s16 D_8015E310;
extern s16 D_8015E314;
extern s8 D_8015F5D0;
extern u8 date_show_taken;
extern s8 date_parade_offered;
void date_ambush_sections_init();

void date_scene_shiori_autumn(void);
void date_scene_shiori_spring(void);
void date_scene_shiori_winter(void);
void date_scene_shiori_summer(void);
void date_scene_miharu_spring(void);
void date_scene_miharu_summer(void);

extern s8 D_8015EE24[];
void date_scene_mio_winter_narration_speak(void);
s32 date_art_museum_mira_face(void);
extern u8 date_shop_index; /* DateShop */
void date_shopping_street_bgm(void);
void date_scene_yuko_winter(void);
void date_scene_nozomi_winter(void);
void date_scene_saki_autumn(void);
void date_scene_yukari_spring(void);
s32 date_scene_yukari_summer(void);
void date_scene_yukari_summer_actors_update(void);
void date_scene_yukari_autumn(void);
void date_scene_yukari_winter(void);
void date_scene_yukari_dispatch(void);
void date_scene_megumi_spring(void);
void date_scene_megumi_summer(void);
void date_scene_megumi_autumn(void);
void date_scene_megumi_winter(void);
void date_scene_megumi_dispatch(void);
void date_scene_yumi_spring(void);
void date_scene_yumi_summer(void);
void date_scene_yumi_autumn(void);
void date_scene_yumi_winter(void);
void date_scene_yumi_dispatch(void);
void date_scene_mira_spring(void);
void date_scene_mio_autumn(void);
void date_scene_nozomi_autumn(void);
void date_scene_ayako_autumn(void);
extern u8 *D_8015B634;
extern u8 *D_8015B638;
extern u8 *D_8015B63C;
extern s16 D_8015B640;
extern u8 *D_8015B644;
void date_scene_yuina_spring(void);
void date_scene_yuina_summer(void);
void date_scene_yuina_summer_audio_command_1(void);
void date_scene_yuina_summer_audio_command_2(void);
void date_scene_yuina_autumn(void);
void date_scene_yuina_winter(void);
void date_scene_yuina_dispatch(void);
void date_scene_ayako_spring(void);
void date_scene_ayako_summer(void);
void date_scene_ayako_winter(void);
void date_scene_ayako_dispatch(void);
void date_scene_mio_spring(void);
void date_scene_mio_summer(void);
void date_scene_mio_winter(void);
void date_scene_mio_dispatch(void);
void date_scene_nozomi_spring(void);
void date_scene_nozomi_summer(void);
void date_scene_nozomi_dispatch(void);
void date_scene_saki_spring(void);
void date_scene_saki_summer(void);
void date_scene_saki_winter(void);
void date_scene_saki_dispatch(void);
extern s8 date_sign_palette_frame;
void date_scene_yuko_spring(void);
void date_scene_yuko_summer(void);
void date_scene_yuko_autumn(void);
void date_scene_yuko_december(void);

void date_scene_mira_summer(void);
void date_scene_mira_autumn(void);
void date_scene_mira_winter(void);
void date_scene_mira_special(void);
extern u8 date_concert_index;
extern u8 date_concert_group;
extern s16 date_concert_line;
s32 date_concert_hall_program_talk(void);

typedef struct DatePlaceAudioCommands {
    /* 0x00 */ u16 by_place[21];
} DatePlaceAudioCommands; /* size 0x2A */
extern DatePlaceAudioCommands date_place_sequence_command;

extern s16 D_8015DDD0[];
extern s32 D_8015DE34[3];
extern s32 D_8015DE38;
extern s32 D_8015DE3C;
void date_virtual_ship_draw_object();
void date_virtual_ship_next_subphase(void);
extern s16 D_8015E318;
s32 date_scene_yuko_summer_sign_animate(void);

extern s8 date_girl_side_next;

#endif /* OVL_DATE_H */

#ifndef OVL_DATE_CALLBACK_T9610
#define OVL_DATE_CALLBACK_T9610
/* Complete local-copy dispatch table in date_scene_mira_summer. */
typedef struct DateMiraSummerSteps {
    /* 0x00 */ void (*callback[39])();
} DateMiraSummerSteps; /* size 0x9C */
extern DateMiraSummerSteps date_scene_mira_summer_steps;
#endif

#ifndef OVL_DATE_COUNTER_T9780
#define OVL_DATE_COUNTER_T9780
void date_amusement_park_parade_prepare(void);
s32 date_ferris_wheel_still_copy(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
#endif

#ifndef OVL_DATE_TABLES_T10020
#define OVL_DATE_TABLES_T10020
typedef struct DateProgramAudioRequest {
    /* 0x00 */ s32 packed_clip_start;
    /* 0x04 */ s32 packed_clip_end;
} DateProgramAudioRequest; /* size 0x08; paired arguments, T-10020. */
typedef struct DateFilmAudioRequests {
    /* 0x00 */ DateProgramAudioRequest rows[11];
} DateFilmAudioRequests; /* size 0x58 */
typedef struct DateConcertAudioRequests {
    /* 0x00 */ DateProgramAudioRequest rows[12];
} DateConcertAudioRequests; /* size 0x60 */
extern DateFilmAudioRequests date_movie_audio_pairs;
extern DateConcertAudioRequests date_concert_audio_pairs;
#endif

#ifndef OVL_DATE_DISPATCH_T10020
#define OVL_DATE_DISPATCH_T10020
void date_virtual_ship_phase_load(void);
void date_virtual_ship_phase_run(void);
void date_virtual_ship_read_portrait(void);
void date_virtual_ship_wait_read(void);
#endif


#ifndef OVL_DATE_T10040
#define OVL_DATE_T10040
/* Forward declarations for the original object, T-10040. */
void date_ferris_wheel_resources_init(void);
u16 date_ride_scream_machine(void);
s32 date_ride_ferris_wheel(void);
s32 date_ride_roller_coaster(void);
s32 date_ride_quad_shift(s16 a0);
s32 date_ride_ghost_house(void);
void date_ride_background_skip(s32 arg0);

extern s16 D_8015CFA4[3];
extern s16 D_8015CFAC[3];
extern s16 D_8015CFB4[3];
extern s16 D_8015CFBC[3];
extern u16 date_ride_frame;
extern s16 D_8015CFC4;
void date_scene_yuko_summer_light_beams(void);
#endif

/* T-10630: signed UV oscillation step used by date_ride_scream_machine. */
extern s16 D_8015CFA0;

#ifndef DATE_SELECTION_TYPES_T10730
#define DATE_SELECTION_TYPES_T10730
/* T-10730: unsigned triples selected by girl, option, then its two-state flag. */
typedef struct DateChoiceTriple {
    /* 0x00 */ u8 value[3];
} DateChoiceTriple; /* size 0x03 */

typedef struct DateShopChoiceRow {
    /* 0x00 */ DateChoiceTriple choice[3][2];
} DateShopChoiceRow; /* size 0x12 */

typedef struct DateRideChoiceRow {
    /* 0x00 */ DateChoiceTriple choice[6][2];
} DateRideChoiceRow; /* size 0x24 */

#endif

#ifndef OVL_DATE_MOTION_T10730
#define OVL_DATE_MOTION_T10730
typedef struct DateMuseumChoiceRow {
    /* 0x00 */ DateChoiceTriple choice[5];
} DateMuseumChoiceRow; /* size 0x0F */
#endif

#ifndef OVL_DATE_FLAG_T10730
#define OVL_DATE_FLAG_T10730
extern u8 date_stadium_game_day;
#endif

#ifndef OVL_DATE_COUNTS_T10730
#define OVL_DATE_COUNTS_T10730
/* Two signed-halfword local copies in date_neighborhood_park_pick_greeting, from date_opening_count_a/B774. */
typedef struct DateGreetingCounts {
    /* 0x00 */ s16 value[11];
} DateGreetingCounts; /* size 0x16 */
#endif

#ifndef DATE_SELECTION_VIEW_T10730
#define DATE_SELECTION_VIEW_T10730
/* T-10730: byte selector slots at D_800CA2F8 and D_800CA2FC. This view
 * records only the observed range, not the complete allocation size. */
typedef struct DateSelectionView {
    /* 0x00 */ u8 place;
    /* 0x01 */ u8 pad01[3];
    /* 0x04 */ u8 option;
} DateSelectionView; /* minimum size 0x05 */
#endif

#ifndef OVL_DATE_MOVIE_T10830_H
#define OVL_DATE_MOVIE_T10830_H
/* Overlay access view at bg_lba_cache. date_ride_roller_coaster resets the halfword
 * fields and bytes 0x18..0x1A. Main 80057390 owns the word-array view
 * and confirms the later fields. EVENT shares the first 0x1C-byte layout. */
typedef struct DateMovieWork {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ u8 pad08[2];
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 pad0C[12];
    /* 0x18 */ u8 unk_18;
    /* 0x19 */ u8 unk_19;
    /* 0x1A */ u8 unk_1A;
    /* 0x1B */ u8 unk_1B;
    /* 0x1C */ u8 unk_1C;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
} DateMovieWork; /* size 0x2C, minimum observed extent */

/* First two words of the bitstream reached through bg_still_bitstream.
 * date_ride_roller_coaster validates word 0 with 0xFFFF0000 and word 1 with
 * 0xFF010000; later bytes have not been recovered here. */
typedef struct DateMovieHeader8 {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 unk_04;
} DateMovieHeader8; /* size 0x08, minimum observed extent */
#endif

#ifndef OVL_DATE_PARTICLE_T10870_H
#define OVL_DATE_PARTICLE_T10870_H
/* Fixed-point horizontal positions, with signed high-halfword updates. */
extern WordView date_particle_targets[20];
/* date_shrine_fireworks_setup initializes ten falling-particle velocities. */
extern WordView date_firework_targets[10];
extern s32 D_8015BD34;
extern s16 D_8015BD38;
extern u8 D_8015BD3C;
#endif

#ifndef OVL_DATE_RIDE_TABLES_T10870_H
#define OVL_DATE_RIDE_TABLES_T10870_H
/* date_amusement_park_ride_setup: girl rows contain six rides, two choices per ride,
 * and three byte-valued results per choice (36 bytes per girl). */
extern DateRideChoiceRow *date_class_table_amusement_park;
extern DateRideChoiceRow *date_face_table_amusement_park;
#endif

#ifndef OVL_DATE_MUSEUM_TABLES_T10870_H
#define OVL_DATE_MUSEUM_TABLES_T10870_H
/* date_place_setup_art_museum reads three result bytes from each 15-byte girl row. */
extern DateMuseumChoiceRow *date_class_table_art_museum;
extern DateMuseumChoiceRow *date_face_table_art_museum;
#endif

#ifndef OVL_DATE_EVENT_CHOICE_T10870_H
#define OVL_DATE_EVENT_CHOICE_T10870_H
/* date_neighborhood_park_pick_greeting copies eleven signed counts from each table. */
extern DateGreetingCounts date_opening_count_a;
extern DateGreetingCounts date_opening_count_b;
extern u8 date_birthday_near;
extern u8 date_hint_roll;
/* Halfword access view at date_opening_group. The legacy declarations describe
 * individual low bytes; this handler reads/stores signed halfwords. */
typedef struct DateEventChoice6 {
    /* 0x00 */ s16 kind;
    /* 0x02 */ u8 pad02[2];
    /* 0x04 */ s16 index;
} DateEventChoice6; /* size 0x06, minimum observed extent */
#endif

#ifndef OVL_DATE_PARK_TABLES_T10900_H
#define OVL_DATE_PARK_TABLES_T10900_H
/* date_place_setup_central_park: flat byte tables, nine bytes per girl (three choices for
 * each of three park variants). Keeping the flat pointer view preserves
 * the original reloads between writes to the two result rows. */
extern u8 *date_class_table_central_park;
extern u8 *date_face_table_central_park;
/* date_shopping_street_enter_shop: eighteen bytes per girl: three shops, two flag variants,
 * three result bytes. A multidimensional view lets IDO elide target loads. */
extern u8 *date_class_table_shopping_street;
extern u8 *date_face_table_shopping_street;
#endif

#ifndef OVL_DATE_CONCERT_TABLES_T10900_H
#define OVL_DATE_CONCERT_TABLES_T10900_H
/* date_place_setup_concert_hall: thirty-six bytes per girl, twelve chronological seasonal
 * variants with three result bytes each. Use flat indexing to retain the
 * target's reloads while populating D_800CA22C and D_800CA21C. */
extern u8 *date_class_table_concert_hall;
extern u8 *date_face_table_concert_hall;
#endif

#ifndef OVL_DATE_CINEMA_TABLES_T10900_H
#define OVL_DATE_CINEMA_TABLES_T10900_H
/* date_place_setup_movie_theater: thirty-three bytes per girl, eleven seasonal variants
 * with three result bytes each. The first year starts with summer. */
extern u8 *date_class_table_movie_theater;
extern u8 *date_face_table_movie_theater;
#endif

#ifndef OVL_DATE_POOL_TABLES_T11580_H
#define OVL_DATE_POOL_TABLES_T11580_H
/* date_place_setup_pool (pool): six bytes per girl, two variants with three result bytes
 * each, read through flat byte pointers like the other DATE tables. */
extern u8 *date_class_table_pool;
extern u8 *date_face_table_pool;
/* date_place_setup_skating_rink: the skating rink tables, same layout. */
extern u8 *date_class_table_skating_rink;
extern u8 *date_face_table_skating_rink;
/* The other place handlers read their two result rows the same way:
 * date_class_table_bowling_alley (bowling), date_class_table_karaoke (karaoke), date_class_table_sea (beach),
 * date_class_table_botanical_garden (botanical garden), date_class_table_ski_resort (ski slope), six bytes per
 * girl; date_class_table_game_center (game centre, used twice), twelve bytes per girl. */
extern u8 *date_class_table_bowling_alley;
extern u8 *date_face_table_bowling_alley;
extern u8 *date_class_table_game_center;
extern u8 *date_face_table_game_center;
extern u8 *date_class_table_karaoke;
extern u8 *date_face_table_karaoke;
extern u8 *date_class_table_sea;
extern u8 *date_face_table_sea;
extern u8 *date_class_table_botanical_garden;
extern u8 *date_face_table_botanical_garden;
extern u8 *date_class_table_ski_resort;
extern u8 *date_face_table_ski_resort;
#endif

#ifndef DATE_MOVIE_SCENE_T10910A
#define DATE_MOVIE_SCENE_T10910A
/* T-10910A: data range 8015CF80..8015CFD0 belongs to 801461B0.
 * The seven buffer addresses are initialized by date_ferris_wheel_resources_init. Three-element
 * signed coordinate/velocity rows have eight-byte storage strides, not six.
 * This view preserves the existing scalar declarations used by matched code. */
typedef struct DateMovieScene50 {
    /* 0x00 */ u32 buffer_addr[7];
    /* 0x1C */ u16 timer;
    /* 0x1E */ u16 unk_1E;
    /* 0x20 */ s16 step;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ s16 x[3];
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ s16 y[3];
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ s16 dx[3];
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ s16 dy[3];
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ s16 sound_timer;
    /* 0x46 */ u8 pad46[10];
} DateMovieScene50; /* size 0x50 */
#endif
extern s16 date_special_day_group; /* DateSpecialDay */
extern s16 date_special_day_line;
#ifndef OVL_DATE_T11020_H
#define OVL_DATE_T11020_H
/* T-11020: 20-byte table copied to the stack (date_choice_score). */
typedef struct DateGirlCounterWeights {
    /* 0x00 */ s32 by_response_class[5];
} DateGirlCounterWeights; /* size 0x14 */
extern DateGirlCounterWeights date_syoushin_weight;
void date_score_multipliers(s16 score_delta, s32 *trivia_multiplier_out, s32 *place_multiplier_out);
#endif
#ifndef OVL_DATE_T11020_TABLES_H
#define OVL_DATE_T11020_TABLES_H
extern u8 *date_class_table_aquarium;
extern u8 *date_face_table_aquarium;
extern u8 *date_class_table_library;
extern u8 *date_face_table_library;
extern u8 *date_class_table_zoo;
extern u8 *date_face_table_zoo;
extern u8 *date_class_table_stadium;
extern u8 *date_face_table_stadium;
#endif
extern u8 *date_class_table_planetarium;
extern u8 *date_face_table_planetarium;
extern s16 D_800CA158;
#ifndef OVL_DATE_T11030_H
#define OVL_DATE_T11030_H
void date_movie_theater_load_poster(void);
void date_concert_hall_load_program_background(void);

#endif
#ifndef OVL_DATE_T11140_H
#define OVL_DATE_T11140_H
void date_load_girl_script(void);
void date_load_shared_script(void);
void date_load_choice_tables(void);
void date_scene_load_script(void);

/* T-14501: section fillers called before their definitions. */
void date_fill_sections_shiori(void);
void date_fill_sections_mio(void);
void date_fill_sections_yuina(void);

#endif

/* T-12100: calls recovered in DATE startup state dispatch. */
void date_virtual_ship_vector_helper(void);
void date_virtual_ship_matrix_helper(void);

#ifndef OVL_DATE_STEP_TABLES_T17200
#define OVL_DATE_STEP_TABLES_T17200
/* Step tables (T-3330, T-17200): one handler per scene step; each handler copies its table to the
 * stack whole and calls tbl.f[game_state.unk_110A]. The types were local to each .c file before. */
typedef struct {
    void (*callbacks[15])();
} FnTbl15; /* size 0x3C */
typedef struct {
    void (*callbacks[18])();
} DateNeighborhoodParkSteps; /* size 0x48 */
typedef struct {
    void (*callbacks[20])();
} DateMiharuSceneSteps; /* size 0x50 */
typedef struct {
    void (*callbacks[28])();
} DateGirlBirthdaySteps; /* size 0x70 */
typedef struct {
    void (*callbacks[29])();
} DateStadiumSteps; /* size 0x74 */
typedef struct {
    void (*callbacks[31])();
} DateWinterSportSteps; /* size 0x7C */
typedef struct {
    void (*callbacks[32])();
} FnTbl32; /* size 0x80 */
typedef struct {
    void (*callbacks[34])();
} FnTbl34; /* size 0x88 */
typedef struct {
    void (*callbacks[35])();
} FnTbl35; /* size 0x8C */
typedef struct {
    void (*callbacks[36])();
} FnTbl36; /* size 0x90 */
typedef struct {
    void (*callbacks[37])();
} DateArtMuseumSteps; /* size 0x94 */
typedef struct {
    void (*callbacks[38])();
} DateYukariAndSakiSpringSteps; /* size 0x98 */
typedef struct {
    void (*callbacks[39])();
} FnTbl39; /* size 0x9C */
typedef struct {
    void (*callbacks[40])();
} FnTbl40; /* size 0xA0 */
typedef struct {
    void (*callbacks[41])();
} FnTbl41; /* size 0xA4 */
typedef struct {
    void (*callbacks[42])();
} FnTbl42; /* size 0xA8 */
typedef struct {
    void (*callbacks[43])();
} FnTbl43; /* size 0xAC */
typedef struct {
    void (*callbacks[44])();
} FnTbl44; /* size 0xB0 */
typedef struct {
    void (*callbacks[45])();
} FnTbl45; /* size 0xB4 */
typedef struct {
    void (*callbacks[46])();
} DateYumiSummerSteps; /* size 0xB8 */
typedef struct {
    void (*callbacks[47])();
} FnTbl47; /* size 0xBC */
typedef struct {
    void (*callbacks[48])();
} DateShoppingStreetSteps; /* size 0xC0 */
typedef struct {
    void (*callbacks[50])();
} FnTbl50; /* size 0xC8 */
typedef struct {
    void (*callbacks[51])();
} DateSakiWinterSteps; /* size 0xCC */
typedef struct {
    void (*callbacks[53])();
} DateMioWinterSteps; /* size 0xD4 */
typedef struct {
    void (*callbacks[55])();
} DateAyakoSpringSteps; /* size 0xDC */
typedef struct {
    void (*callbacks[56])();
} FnTbl56; /* size 0xE0 */
typedef struct {
    void (*callbacks[57])();
} DateMiraAutumnSteps; /* size 0xE4 */
typedef struct {
    void (*callbacks[61])();
} DateNozomiSummerSteps; /* size 0xF4 */
typedef struct {
    void (*callbacks[63])();
} DateMegumiSummerSteps; /* size 0xFC */
typedef struct {
    void (*callbacks[65])();
} DateShioriWinterSteps; /* size 0x104 */
typedef struct {
    void (*callbacks[67])();
} DateMioSpringSteps; /* size 0x10C */
typedef struct {
    void (*callbacks[74])();
} DateShrineSteps; /* size 0x128 */
typedef struct {
    void (*callbacks[76])();
} FnTbl76; /* size 0x130 */
typedef struct {
    void (*callbacks[104])();
} DateAmusementParkSteps; /* size 0x1A0 */
extern FnTbl76 date_mode_stage_arrival_steps;
extern DateNeighborhoodParkSteps date_neighborhood_park_steps;
extern FnTbl35 date_central_park_steps;
extern DateShoppingStreetSteps date_shopping_street_steps;
extern FnTbl32 date_aquarium_steps;
extern FnTbl35 date_zoo_steps;
extern FnTbl34 date_botanical_garden_steps;
extern FnTbl36 date_planetarium_steps;
extern DateArtMuseumSteps date_art_museum_steps;
extern FnTbl50 date_library_steps;
extern FnTbl39 date_game_center_steps;
extern FnTbl32 date_bowling_alley_steps;
extern FnTbl34 date_karaoke_steps;
extern DateAmusementParkSteps date_amusement_park_steps;
extern DateStadiumSteps date_stadium_steps;
extern FnTbl45 date_movie_theater_steps;
extern FnTbl41 date_concert_hall_steps;
extern FnTbl35 date_pool_steps;
extern FnTbl76 date_sea_steps;
extern DateShrineSteps date_shrine_steps;
extern DateWinterSportSteps date_skating_rink_steps;
extern DateWinterSportSteps date_ski_resort_steps;
extern FnTbl15 date_player_birthday_steps;
extern DateGirlBirthdaySteps date_girl_birthday_steps;
extern FnTbl42 date_scene_yuko_spring_steps;
extern FnTbl39 date_scene_yuko_summer_steps;
extern FnTbl44 date_scene_yuko_autumn_steps;
extern FnTbl43 date_scene_yuko_winter_steps;
extern FnTbl44 date_scene_yuko_december_steps;
extern FnTbl41 date_scene_shiori_autumn_steps;
extern FnTbl43 date_scene_shiori_spring_steps;
extern DateShioriWinterSteps date_scene_shiori_winter_steps;
extern FnTbl45 date_scene_shiori_summer_steps;
extern FnTbl40 date_scene_yuina_spring_steps;
extern FnTbl39 date_scene_yuina_summer_steps;
extern FnTbl40 date_scene_yuina_autumn_steps;
extern FnTbl40 date_scene_yuina_winter_steps;
extern DateAyakoSpringSteps date_scene_ayako_spring_steps;
extern FnTbl42 date_scene_ayako_autumn_steps;
extern FnTbl47 date_scene_ayako_winter_steps;
extern FnTbl41 date_scene_mira_spring_steps;
extern DateMiraAutumnSteps date_scene_mira_autumn_steps;
extern FnTbl15 date_scene_mira_special_steps;
extern DateMioSpringSteps date_scene_mio_spring_steps;
extern FnTbl42 date_scene_mio_summer_steps;
extern FnTbl41 date_scene_mio_autumn_steps;
extern DateMioWinterSteps date_scene_mio_winter_steps;
extern DateYukariAndSakiSpringSteps date_scene_yukari_spring_steps;
extern FnTbl45 date_scene_yukari_summer_steps;
extern FnTbl40 date_scene_yukari_autumn_steps;
extern FnTbl56 date_scene_yukari_winter_steps;
extern FnTbl50 date_scene_nozomi_spring_steps;
extern DateNozomiSummerSteps date_scene_nozomi_summer_steps;
extern FnTbl41 date_scene_nozomi_autumn_steps;
extern FnTbl43 date_scene_nozomi_winter_steps;
extern FnTbl47 date_scene_megumi_spring_steps;
extern DateMegumiSummerSteps date_scene_megumi_summer_steps;
extern FnTbl42 date_scene_megumi_autumn_steps;
extern FnTbl36 date_scene_megumi_winter_steps;
extern DateYukariAndSakiSpringSteps date_scene_saki_spring_steps;
extern FnTbl56 date_scene_saki_summer_steps;
extern FnTbl43 date_scene_saki_autumn_steps;
extern DateSakiWinterSteps date_scene_saki_winter_steps;
extern FnTbl44 date_scene_yumi_spring_steps;
extern DateYumiSummerSteps date_scene_yumi_summer_steps;
extern FnTbl44 date_scene_yumi_autumn_steps;
extern DateMiharuSceneSteps date_scene_miharu_spring_steps;
extern DateMiharuSceneSteps date_scene_miharu_summer_steps;
#endif

#ifndef OVL_DATE_STACK_TABLES_T17200
#define OVL_DATE_STACK_TABLES_T17200
/* Constant tables that one handler copies to the stack whole (T-17200); element types from the loads. */
typedef struct DatePlaceAffinityTable {
    /* 0x00 */ u8 by_girl_and_place[11][21];
} DatePlaceAffinityTable; /* size 0xE7; date_score_multipliers reads [game_state.unk_F5F][D_800CA2F8] */
extern DatePlaceAffinityTable date_place_affinity;

typedef struct DatePlaceAudioValues {
    /* 0x00 */ s32 by_place[21];
} DatePlaceAudioValues; /* size 0x54; date_place_bgm_load_start reads [D_800CA2F8] */
extern DatePlaceAudioValues date_place_bank_third_argument;
extern DatePlaceAudioValues date_place_bank_first_data_argument;
extern DatePlaceAudioValues date_place_bank_second_data_argument;
extern DatePlaceAudioCommands date_place_load_command; /* date_place_bgm_load_start reads [D_800CA2F8] with lhu */

typedef struct DateSceneAudioRequest {
    /* 0x00 */ s32 packed_clip_bounds[2];
} DateSceneAudioRequest; /* size 0x08; date_scene_ayako_spring_clip_play copies it to the stack */
extern DateSceneAudioRequest D_8015EF0C;

typedef struct DateShorts8 {
    /* 0x00 */ s16 v[8];
} DateShorts8; /* size 0x10; screen x or y lists of date_scene_yukari_summer_setup and date_scene_yukari_summer_actors_update */
extern DateShorts8 D_8015F9B0;
extern DateShorts8 D_8015F9C0;
extern DateShorts8 D_8015FB1C;
extern DateShorts8 D_8015FB2C;

typedef struct DatePairs8 {
    /* 0x00 */ s16 v[8][2];
} DatePairs8; /* size 0x20; date_scene_yukari_summer_actors_update reads v[i][1] */
extern DatePairs8 D_8015FB3C;

/* date_scene_yukari_summer_actors_update and date_scene_yukari_summer_setup: eight moving objects (display records 98..105). D_8015F9A0 holds their
 * state (0 idle, 1 armed, 2 moving), D_8015F9A8 the idle counters (T-17205). */
extern u8 D_8015F9A0[8];
extern u8 D_8015F9A8[8];

typedef struct DatePlaceNameTable {
    /* 0x00 */ u8 names[22][33];
} DatePlaceNameTable; /* size 0x2D6; date_meeting_place_check copies date_place_names to the stack */
extern DatePlaceNameTable date_place_names;

#endif

#ifndef OVL_DATE_PROTOS_T17200
#define OVL_DATE_PROTOS_T17200
void date_place_setup_neighborhood_park(void);
void date_place_setup_central_park(void);
void date_place_setup_aquarium(void);
void date_place_setup_zoo(void);
void date_place_setup_botanical_garden(void);
void date_place_setup_planetarium(void);
void date_place_setup_art_museum(void);
void date_place_setup_library(void);
void date_place_setup_game_center(void);
void date_place_setup_bowling_alley(void);
void date_place_setup_karaoke(void);
void date_place_setup_amusement_park(void);
void date_place_setup_shopping_street(void);
void date_place_setup_stadium(void);
void date_place_setup_movie_theater(void);
void date_place_setup_concert_hall(void);
void date_place_setup_pool(void);
void date_place_setup_sea(void);
void date_place_setup_shrine(void);
void date_place_setup_skating_rink(void);
void date_place_setup_ski_resort(void);
void date_scene_yumi_summer_audio_command_1(void);
/* HYPOTHESIS: DATE arrival states; source-backed literal sites. */
enum DateArrival {
    DATE_ARRIVAL_WAIT_GREETING = 0,
    DATE_ARRIVAL_DIRECT_GREETING = 1,
    DATE_ARRIVAL_CORRECTED_PLACE = 2
};

/* T-27051 DATE ride-choice values. */
enum DateRide {
    DATE_RIDE_ROLLER_COASTER = 0,
    DATE_RIDE_GHOST_HOUSE = 1,
    DATE_RIDE_FERRIS_WHEEL = 2,
    DATE_RIDE_SCREAM_MACHINE = 3,
    DATE_RIDE_VIRTUAL_SHIP = 4
};

/* T-27051 DATE shop-choice values. */
enum DateShop {
    DATE_SHOP_BOUTIQUE = 0,
    DATE_SHOP_FANCY_SHOP = 1,
    DATE_SHOP_JUNK_SHOP = 2
};

/* T-27051 DATE meeting-group values. */
enum DateMeetingGroup {
    DATE_MEETING_ASK_TWO = 0,
    DATE_MEETING_ASK_THREE = 3,
    DATE_MEETING_NOT_HERE = 7,
    DATE_MEETING_REFUSES = 8
};

/* T-27051 DATE special-day values. */
enum DateSpecialDay {
    DATE_SPECIAL_DAY_MARCH = 0,
    DATE_SPECIAL_DAY_JULY_SUNDAY = 1,
    DATE_SPECIAL_DAY_SEPTEMBER_ODD = 2,
    DATE_SPECIAL_DAY_SEPTEMBER_EVEN = 3,
    DATE_SPECIAL_DAY_DECEMBER_23 = 4
};

#endif
