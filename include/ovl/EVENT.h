#ifndef OVL_EVENT_H
#define OVL_EVENT_H

/* EVENT Nozomi scene-step dispatcher; overlay code overlaps main BSS. T-27064. */
void spot_nozomi_scene1_steps_run(void);

#define MAIN_API_OVERRIDE_D_8012274C /* EVENT load-time array view, main context is s32 (T-24037). */
#define MAIN_API_OVERRIDE_D_80122754 /* EVENT load-time array view, main context is s32 (T-24037). */
#define MAIN_API_OVERRIDE_D_80122764 /* EVENT load-time array view, main context is s32 (T-24037). */
#define MAIN_API_OVERRIDE_D_80122774 /* EVENT load-time array view, main context is s32 (T-24037). */
/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_D_80120674 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206B8 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120658 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_8012066C /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120698 /* EVENT overlay data at this address, s32 (main_api.h: u8) */
#define MAIN_API_OVERRIDE_D_80120694 /* EVENT overlay data at this address, s32 (main_api.h: u8[]) */
#define MAIN_API_OVERRIDE_D_801206A8 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206AC /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206C8 /* EVENT overlay data at this address, s16 (main_api.h: s32) */
#define MAIN_API_OVERRIDE_D_801206E0 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206F0 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120720 /* EVENT overlay data at this address, s16 (main_api.h: s8) */
#define MAIN_API_OVERRIDE_D_801206DC /* EVENT overlay data at this address, s32 (main_api.h: s8) */
#define MAIN_API_OVERRIDE_D_801206D8 /* EVENT overlay data at this address, s32 (main_api.h: u8[]) */
#define MAIN_API_OVERRIDE_D_8012121C /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801207B0 /* EVENT overlay data at this address, s16 (main_api.h: s32) */
#define MAIN_API_OVERRIDE_D_801207B4 /* EVENT overlay data at this address, s16 (main_api.h: s32) */
#define MAIN_API_OVERRIDE_D_801206EC /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120724 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120730 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120734 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80121874 /* EVENT overlay data at this address, s32 (main_api.h: u8) */
#define MAIN_API_OVERRIDE_D_801206B0 /* EVENT overlay data at this address, s32 (main_api.h: volatile s16) */

#include "common.h"
#include "game.h"

void func_80011DFC();
void spot_yoshio_scene1_run();
s32 func_8001496C();
extern s32 spot_miharu_tile_pointers_1;
extern s32 D_80123E94;
extern s32 D_80123E98;
extern s32 D_80123E9C;
extern s32 D_80123EA0;
extern s32 D_80123EA4;
extern s32 D_80123EA8;
void func_80012D64(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 D_801258C0;
extern s32 D_801258C4;
extern s32 D_801258C8;
extern s32 D_801258CC;
extern s32 D_801258D0;
extern s32 D_801258D4;
extern s32 D_801258D8;
extern s32 spot_talk_text_table;
extern s32 spot_talk_command_table;
extern s32 spot_talk_voice_table;
extern s16 D_8012437C;
extern s16 D_80124380;
extern s32 D_80124224;
extern s32 D_80124290;
extern s32 D_801242FC;
extern s16 D_80124384;
extern s16 D_80124388;
extern s32 D_801241D4;
extern s32 D_801241E4;
extern s32 D_801241F4;
extern u8 spot_place_variant;
void spot_ride_movie_run(void);
extern s32 D_8012079C;
extern s32 D_801207A0;
extern s16 D_801207A4;
extern s32 D_801207A8;
extern s32 spot_shiori_tile_pointers_1;
extern s32 D_80120CAC;
extern s32 D_80120CB0;
extern s32 D_80120CB4;
extern s32 D_80120CB8;
extern s32 D_80120CBC;
extern s32 D_80120CC0;
extern s32 spot_yuina_tile_pointers_1;
extern s32 D_80121210;
extern s32 D_80121214;
extern s32 D_80121218;
extern s32 D_8012121C;
extern s32 D_80121220;
extern s32 D_80121224;
extern s32 spot_rei_tile_pointers_1;
extern s32 D_801215E4;
extern s32 D_801215E8;
extern s32 D_801215EC;
extern s32 D_801215F0;
extern s32 D_801215F4;
extern s32 D_801215F8;
extern s32 spot_yukari_tile_pointers_1;
extern s32 D_80122734;
extern s32 D_80122738;
extern s32 D_8012273C;
extern s32 spot_place_frame_pointers;
extern s32 D_801252C0;
extern s32 D_801252C4;
extern s32 D_801252C8;
extern s32 D_801252CC;
extern s32 D_801252D0;
extern s32 D_801252DC;
extern s32 D_801252E0;
extern s32 spot_yuko_still_pointers_1;
extern s32 D_80120694;
extern s32 D_80120698;
extern s16 D_8012069C;
extern s32 D_801206A8;
extern s32 D_801206AC;
extern s32 D_801206B0;
extern s32 D_801206B4;
extern s32 D_801206B8;
extern s32 spot_yuko_still_pointers_2;
extern s32 D_801206C0;
extern s32 D_801206C4;
extern s16 D_801206C8;
extern s32 D_801206D0;
extern s32 D_801206D4;
extern s32 D_801206D8;
extern s32 D_801206DC;
extern s32 D_801206E0;
extern s32 D_801206EC;
extern s32 D_801206F0;
extern s32 D_801206F8;
extern s32 D_801206FC;
extern s32 D_80120700;
extern s32 D_80120704;
extern s32 D_80120708;
extern s32 spot_yuko_still_pointers_4;
extern s32 D_80120718;
extern s32 D_8012071C;
extern s16 D_80120720;
extern s32 D_80120724;
extern s32 D_80120730;
extern s32 D_80120734;
extern s32 D_80120738;
extern s32 D_8012073C;
extern s32 spot_yuko_still_pointers_5;
extern s32 D_80120744;
extern s32 D_80120748;
extern s16 D_8012074C;
extern s32 D_80120758;
extern s32 D_8012075C;
extern s32 D_80120764;
extern s32 D_80120768;
extern s32 D_80120774;
extern s32 D_80120780;
extern s32 D_80120784;
extern s32 D_80120788;
extern s32 D_8012078C;
extern s32 D_80120790;
extern s32 spot_shiori_still_pointers_1;
extern s32 D_80120C28;
extern s32 D_80120C2C;
extern s16 D_80120C30;
extern s32 D_80120C34;
extern s32 D_80120C38;
extern s32 D_80120C3C;
extern s32 D_80120C40;
extern s32 D_80120C44;
extern s32 D_80120C48;
extern s32 D_80120C4C;
extern s32 spot_shiori_still_pointers_2;
extern s32 D_80120C54;
extern s32 D_80120C58;
extern s16 D_80120C5C;
extern s32 D_80120C60;
extern s32 D_80120C64;
extern s32 D_80120C68;
extern s32 D_80120C6C;
extern s32 D_80120C70;
extern s32 D_80120C74;
extern s32 D_80120C78;
extern s32 spot_shiori_still_pointers_3;
extern s32 D_80120C80;
extern s32 D_80120C84;
extern s16 D_80120C88;
extern s32 D_80120C8C;
extern s32 D_80120C90;
extern s32 D_80120C94;
extern s32 D_80120C98;
extern s32 D_80120C9C;
extern s32 D_80120CA0;
extern s32 D_80120CA4;
extern s32 spot_shiori_still_pointers_4;
extern s32 D_80120CC8;
extern s32 D_80120CCC;
extern s16 D_80120CD0;
extern s32 D_80120CD4;
extern s32 D_80120CD8;
extern s32 D_80120CDC;
extern s32 D_80120CE0;
extern s32 D_80120CE4;
extern s32 D_80120CE8;
extern s32 D_80120CEC;
extern s32 spot_yuina_still_pointers_1;
extern s32 D_801211E4;
extern s32 D_801211E8;
extern s16 D_801211EC;
extern s32 D_801211F0;
extern s32 D_801211F4;
extern s32 D_801211F8;
extern s32 D_801211FC;
extern s32 D_80121200;
extern s32 D_80121204;
extern s32 D_80121208;
extern s32 spot_yuina_still_pointers_2;
extern s32 D_8012122C;
extern s32 D_80121230;
extern s16 D_80121234;
extern s32 D_8012123C;
extern s32 D_80121240;
extern s32 D_80121244;
extern s32 D_80121248;
extern s32 D_8012124C;
extern s32 D_80121250;
extern s32 spot_yuina_still_pointers_3;
extern s32 D_8012128C;
extern s32 D_80121290;
extern s16 D_80121294;
extern s32 D_80121298;
extern s32 D_8012129C;
extern s32 D_801212A0;
extern s32 D_801212A4;
extern s32 D_801212A8;
extern s32 D_801212AC;
extern s32 D_801212B0;
extern s32 spot_ayako_still_pointers_1;
extern s32 D_80121804;
extern s16 D_8012180C;
extern s32 D_80121810;
extern s32 D_80121814;
extern s32 D_8012181C;
extern s32 D_80121820;
extern s32 D_80121824;
extern s32 D_80121828;
extern s32 spot_ayako_still_pointers_2;
extern s32 D_80121830;
extern s32 D_80121834;
extern s16 D_80121838;
extern s32 D_80121840;
extern s32 D_80121844;
extern s32 D_80121848;
extern s32 D_8012184C;
extern s32 D_80121850;
extern s32 D_80121854;
extern s32 spot_ayako_still_pointers_3;
extern s32 D_8012185C;
extern s32 D_80121868;
extern s32 D_8012186C;
extern s32 D_80121870;
extern s32 D_80121874;
extern s32 D_80121878;
extern s32 D_8012187C;
extern s32 D_80121880;
extern s32 D_80121888;
extern s32 D_8012188C;
extern s16 D_80121890;
extern s32 D_80121894;
extern s32 D_80121898;
extern s32 D_8012189C;
extern s32 D_801218A0;
extern s32 D_801218A4;
extern s32 D_801218A8;
extern s32 D_801218AC;
extern s32 spot_ayako_still_pointers_5;
extern s32 D_801218B4;
extern s32 D_801218B8;
extern s16 D_801218BC;
extern s32 D_801218C0;
extern s32 D_801218C4;
extern s32 D_801218C8;
extern s32 D_801218CC;
extern s32 D_801218D0;
extern s32 D_801218D4;
extern s32 D_801218D8;
extern s32 spot_mira_still_pointers_1;
extern s32 D_80121D24;
extern s32 D_80121D28;
extern s16 D_80121D2C;
extern s32 D_80121D30;
extern s32 D_80121D34;
extern s32 D_80121D38;
extern s32 D_80121D3C;
extern s32 D_80121D40;
extern s32 D_80121D44;
extern s32 D_80121D48;
extern s32 spot_mira_still_pointers_2;
extern s32 D_80121D50;
extern s32 D_80121D54;
extern s16 D_80121D58;
extern s32 D_80121D5C;
extern s32 D_80121D60;
extern s32 D_80121D64;
extern s32 D_80121D68;
extern s32 D_80121D6C;
extern s32 D_80121D70;
extern s32 D_80121D74;
extern s32 spot_mira_still_pointers_3;
extern s32 D_80121D7C;
extern s32 D_80121D80;
extern s16 D_80121D84;
extern s32 D_80121D88;
extern s32 D_80121D8C;
extern s32 D_80121D90;
extern s32 D_80121D94;
extern s32 D_80121D98;
extern s32 D_80121D9C;
extern s32 D_80121DA0;
extern s32 spot_mira_still_pointers_4;
extern s32 D_80121DA8;
extern s32 D_80121DAC;
extern s16 D_80121DB0;
extern s32 D_80121DB4;
extern s32 D_80121DB8;
extern s32 D_80121DBC;
extern s32 D_80121DC0;
extern s32 D_80121DC4;
extern s32 D_80121DC8;
extern s32 D_80121DCC;
extern s32 spot_mira_still_pointers_5;
extern s32 D_80121DD4;
extern s32 D_80121DD8;
extern s16 D_80121DDC;
extern s32 D_80121DE0;
extern s32 D_80121DE4;
extern s32 D_80121DE8;
extern s32 D_80121DEC;
extern s32 D_80121DF0;
extern s32 D_80121DF4;
extern s32 D_80121DF8;
extern s32 spot_mio_still_pointers_1;
extern s32 D_801221D4;
extern s32 D_801221D8;
extern s16 D_801221DC;
extern s32 D_801221E0;
extern s32 D_801221E4;
extern s32 D_801221E8;
extern s32 D_801221EC;
extern s32 D_801221F0;
extern s32 D_801221F4;
extern s32 D_801221F8;
extern s32 spot_mio_still_pointers_2;
extern s32 D_80122200;
extern s32 D_80122204;
extern s16 D_80122208;
extern s32 D_8012220C;
extern s32 D_80122210;
extern s32 D_80122214;
extern s32 D_80122218;
extern s32 D_8012221C;
extern s32 D_80122220;
extern s32 D_80122224;
extern s32 spot_mio_still_pointers_3;
extern s32 D_8012222C;
extern s32 D_80122230;
extern s16 D_80122234;
extern s32 D_80122238;
extern s32 D_8012223C;
extern s32 D_80122240;
extern s32 D_80122244;
extern s32 D_80122248;
extern s32 D_8012224C;
extern s32 D_80122250;
extern s32 spot_mio_still_pointers_4;
extern s32 D_80122258;
extern s32 D_8012225C;
extern s16 D_80122260;
extern s32 D_80122264;
extern s32 D_80122268;
extern s32 D_8012226C;
extern s32 D_80122270;
extern s32 D_80122274;
extern s32 D_80122278;
extern s32 D_8012227C;
extern s32 spot_mio_still_pointers_5;
extern s32 D_80122284;
extern s32 D_80122288;
extern s16 D_8012228C;
extern s32 D_80122290;
extern s32 D_80122294;
extern s32 D_80122298;
extern s32 D_8012229C;
extern s32 D_801222A0;
extern s32 D_801222A4;
extern s32 D_801222A8;
extern s32 spot_yukari_still_pointers_1;
extern s32 D_80122684;
extern s32 D_80122688;
extern s16 D_8012268C;
extern s32 D_80122690;
extern s32 D_80122694;
extern s32 D_80122698;
extern s32 D_8012269C;
extern s32 D_801226A0;
extern s32 D_801226A4;
extern s32 D_801226A8;
extern s32 spot_yukari_still_pointers_2;
extern s32 D_801226B0;
extern s32 D_801226B4;
extern s16 D_801226B8;
extern s32 D_801226BC;
extern s32 D_801226C0;
extern s32 D_801226C4;
extern s32 D_801226C8;
extern s32 D_801226CC;
extern s32 D_801226D0;
extern s32 D_801226D4;
extern s32 spot_yukari_still_pointers_3;
extern s32 D_801226DC;
extern s32 D_801226E0;
extern s16 D_801226E4;
extern s32 D_801226E8;
extern s32 D_801226EC;
extern s32 D_801226F0;
extern s32 D_801226F4;
extern s32 D_801226F8;
extern s32 D_801226FC;
extern s32 D_80122700;
extern s32 spot_yukari_still_pointers_4;
extern s32 D_80122708;
extern s32 D_8012270C;
extern s16 D_80122710;
extern s32 D_80122714;
extern s32 D_80122718;
extern s32 D_8012271C;
extern s32 D_80122720;
extern s32 D_80122724;
extern s32 D_80122728;
extern s32 D_8012272C;
extern s32 spot_nozomi_still_pointers_1;
extern s32 D_80122B34;
extern s32 D_80122B38;
extern s16 D_80122B3C;
extern s32 D_80122B40;
extern s32 D_80122B44;
extern s32 D_80122B48;
extern s32 D_80122B4C;
extern s32 D_80122B50;
extern s32 D_80122B54;
extern s32 D_80122B58;
extern s32 spot_nozomi_still_pointers_2;
extern s32 D_80122B60;
extern s32 D_80122B64;
extern s16 D_80122B68;
extern s32 D_80122B6C;
extern s32 D_80122B70;
extern s32 D_80122B74;
extern s32 D_80122B78;
extern s32 D_80122B7C;
extern s32 D_80122B80;
extern s32 D_80122B84;
extern s32 spot_nozomi_still_pointers_3;
extern s32 D_80122B8C;
extern s32 D_80122B90;
extern s16 D_80122B94;
extern s32 D_80122B98;
extern s32 D_80122B9C;
extern s32 D_80122BA0;
extern s32 D_80122BA4;
extern s32 D_80122BA8;
extern s32 D_80122BAC;
extern s32 D_80122BB0;
extern s32 spot_nozomi_still_pointers_4;
extern s32 D_80122BB8;
extern s32 D_80122BBC;
extern s16 D_80122BC0;
extern s32 D_80122BC4;
extern s32 D_80122BC8;
extern s32 D_80122BCC;
extern s32 D_80122BD0;
extern s32 D_80122BD4;
extern s32 D_80122BD8;
extern s32 D_80122BDC;
extern s32 spot_nozomi_still_pointers_5;
extern s32 D_80122BE4;
extern s32 D_80122BE8;
extern s16 D_80122BEC;
extern s32 D_80122BF0;
extern s32 D_80122BF4;
extern s32 D_80122BF8;
extern s32 D_80122BFC;
extern s32 D_80122C00;
extern s32 D_80122C04;
extern s32 D_80122C08;
extern s32 spot_megumi_still_pointers_1;
extern s32 D_80122FA0;
extern s32 D_80122FA4;
extern s16 D_80122FA8;
extern s32 D_80122FAC;
extern s32 D_80122FB0;
extern s32 D_80122FB4;
extern s32 D_80122FB8;
extern s32 D_80122FBC;
extern s32 D_80122FC0;
extern s32 D_80122FC4;
extern s32 spot_megumi_still_pointers_2;
extern s32 D_80122FCC;
extern s32 D_80122FD0;
extern s16 D_80122FD4;
extern s32 D_80122FD8;
extern s32 D_80122FDC;
extern s32 D_80122FE0;
extern s32 D_80122FE4;
extern s32 D_80122FE8;
extern s32 D_80122FEC;
extern s32 D_80122FF0;
extern s32 spot_megumi_still_pointers_3;
extern s32 D_80122FF8;
extern s32 D_80122FFC;
extern s16 D_80123000;
extern s32 D_80123004;
extern s32 D_80123008;
extern s32 D_8012300C;
extern s32 D_80123010;
extern s32 D_80123014;
extern s32 D_80123018;
extern s32 D_8012301C;
extern s32 spot_megumi_still_pointers_4;
extern s32 D_80123024;
extern s32 D_80123028;
extern s16 D_8012302C;
extern s32 D_80123030;
extern s32 D_80123034;
extern s32 D_80123038;
extern s32 D_8012303C;
extern s32 D_80123040;
extern s32 D_80123044;
extern s32 D_80123048;
extern s32 spot_saki_still_pointers_1;
extern s32 D_80123434;
extern s32 D_80123438;
extern s16 D_8012343C;
extern s32 D_80123440;
extern s32 D_80123444;
extern s32 D_80123448;
extern s32 D_8012344C;
extern s32 D_80123450;
extern s32 D_80123454;
extern s32 D_80123458;
extern s32 spot_saki_still_pointers_2;
extern s32 D_80123460;
extern s32 D_80123464;
extern s16 D_80123468;
extern s32 D_8012346C;
extern s32 D_80123470;
extern s32 D_80123474;
extern s32 D_80123478;
extern s32 D_8012347C;
extern s32 D_80123480;
extern s32 D_80123484;
extern s32 spot_saki_still_pointers_3;
extern s32 D_8012348C;
extern s32 D_80123490;
extern s16 D_80123494;
extern s32 D_80123498;
extern s32 D_8012349C;
extern s32 D_801234A0;
extern s32 D_801234A4;
extern s32 D_801234A8;
extern s32 D_801234AC;
extern s32 D_801234B0;
extern s32 spot_saki_still_pointers_4;
extern s32 D_801234B8;
extern s32 D_801234BC;
extern s16 D_801234C0;
extern s32 D_801234C4;
extern s32 D_801234C8;
extern s32 D_801234CC;
extern s32 D_801234D0;
extern s32 D_801234D4;
extern s32 D_801234D8;
extern s32 D_801234DC;
extern s32 spot_saki_still_pointers_5;
extern s32 D_801234E4;
extern s32 D_801234E8;
extern s16 D_801234EC;
extern s32 D_801234F0;
extern s32 D_801234F4;
extern s32 D_801234F8;
extern s32 D_801234FC;
extern s32 D_80123500;
extern s32 D_80123504;
extern s32 D_80123508;
extern s32 spot_yumi_still_pointers_1;
extern s32 D_80123884;
extern s32 D_80123888;
extern s16 D_8012388C;
extern s32 D_80123890;
extern s32 D_80123894;
extern s32 D_80123898;
extern s32 D_8012389C;
extern s32 D_801238A0;
extern s32 D_801238A4;
extern s32 D_801238A8;
extern s32 spot_yumi_still_pointers_2;
extern s32 D_801238B0;
extern s32 D_801238B4;
extern s16 D_801238B8;
extern s32 D_801238BC;
extern s32 D_801238C0;
extern s32 D_801238C4;
extern s32 D_801238C8;
extern s32 D_801238CC;
extern s32 D_801238D0;
extern s32 D_801238D4;
extern s32 spot_yumi_still_pointers_3;
extern s32 D_801238DC;
extern s32 D_801238E0;
extern s16 D_801238E4;
extern s32 D_801238E8;
extern s32 D_801238EC;
extern s32 D_801238F0;
extern s32 D_801238F4;
extern s32 D_801238F8;
extern s32 D_801238FC;
extern s32 D_80123900;
extern s32 spot_yumi_still_pointers_4;
extern s32 D_80123908;
extern s32 D_8012390C;
extern s16 D_80123910;
extern s32 D_80123914;
extern s32 D_80123918;
extern s32 D_8012391C;
extern s32 D_80123920;
extern s32 D_80123924;
extern s32 D_80123928;
extern s32 D_8012392C;
extern s32 spot_yumi_still_pointers_5;
extern s32 D_80123934;
extern s32 D_80123938;
extern s16 D_8012393C;
extern s32 D_80123940;
extern s32 D_80123944;
extern s32 D_80123948;
extern s32 D_8012394C;
extern s32 D_80123950;
extern s32 D_80123954;
extern s32 D_80123958;
extern s32 spot_yumi_still_pointers_6;
extern s32 D_80123960;
extern s32 D_80123964;
extern s16 D_80123968;
extern s32 D_8012396C;
extern s32 D_80123970;
extern s32 D_80123974;
extern s32 D_80123978;
extern s32 D_8012397C;
extern s32 D_80123980;
extern s32 D_80123984;
extern s32 spot_miharu_still_pointers_1;
extern s32 D_80123EB0;
extern s32 D_80123EB4;
extern s16 D_80123EB8;
extern s32 D_80123EBC;
extern s32 D_80123EC0;
extern s32 D_80123EC4;
extern s32 D_80123EC8;
extern s32 D_80123ECC;
extern s32 D_80123ED0;
extern s32 D_80123ED4;
extern s32 spot_yoshio_still_pointers_1;
extern s32 D_801240F4;
extern s32 D_801240F8;
extern s16 D_801240FC;
extern s32 D_80124100;
extern s32 D_80124104;
extern s32 D_80124108;
extern s32 D_8012410C;
extern s32 D_80124110;
extern s32 D_80124114;
extern s32 D_80124118;
extern s32 spot_date_common_pointers;
extern s32 D_801241D8;
extern s32 D_801241DC;
extern s32 D_801241E0;
extern s32 D_801241E8;
extern s32 D_801241EC;
extern s32 D_801241F0;
extern s32 D_801241F8;
extern s32 D_801241FC;
extern s32 spot_shiori_overlay_pointers_1;
extern s32 D_80120BF4;
extern s32 D_80120BF8;
extern s16 D_80120BFC;
extern s32 D_80120C00;
extern s32 D_80120C04;
extern s32 D_80120C08;
extern s32 D_80120C0C;
extern s32 D_80120C10;
extern s32 D_80120C14;
extern s32 D_80120C18;
extern s32 D_80120C1C;
extern s32 D_80120C20;
extern s32 spot_yuina_overlay_pointers_1;
extern s32 D_80121258;
extern s32 D_8012125C;
extern s16 D_80121260;
extern s32 D_80121264;
extern s32 D_80121268;
extern s32 D_8012126C;
extern s32 D_80121270;
extern s32 D_80121274;
extern s32 D_80121278;
extern s32 D_80121280;
extern s32 D_80121284;
extern s32 spot_megumi_overlay_pointers_1;
extern s32 D_80122F74;
extern s32 D_80122F78;
extern s16 D_80122F7C;
extern s32 D_80122F80;
extern s32 D_80122F84;
extern s32 D_80122F88;
extern s32 D_80122F8C;
extern s32 D_80122F90;
extern s32 D_80122F94;
extern s32 D_80122F98;
extern s32 spot_place_asset_pointers;
extern s32 D_80125274;
extern s32 D_80125278;
extern s32 D_80125280;
extern s32 D_80125284;
extern s32 D_80125288;
extern s32 D_8012528C;
extern s32 D_80125290;
extern s32 D_80125294;
extern s32 D_80125298;
extern s32 D_8012529C;
extern s32 D_801252A0;
extern s32 D_801252A4;
extern s32 D_801252A8;
extern s32 D_801252AC;
extern s32 D_801252B0;
extern s32 D_801252B4;
extern s32 D_801252B8;
extern s32 spot_place_result_tables;
extern s32 D_801251D4;
extern s32 D_801251D8;
extern s32 D_801251DC;
extern s32 D_801251E0;
extern s32 D_801251E4;
extern s32 D_801251E8;
extern s32 D_801251EC;
extern s32 D_801251F0;
extern s32 D_801251F4;
extern s32 D_801251F8;
extern s32 D_801251FC;
extern s32 D_80125200;
extern s32 D_80125204;
extern s32 D_80125208;
extern s32 D_8012520C;
extern s32 D_80125210;
extern s32 D_80125214;
extern s32 D_80125218;
extern s32 D_8012521C;
extern s32 D_80125220;
extern s32 D_80125224;
extern s32 D_80125228;
extern s32 D_8012522C;
extern s32 D_80125230;
extern s32 D_80125234;
extern s32 D_80125238;
extern s32 D_8012523C;
extern s32 D_80125240;
extern s32 D_80125244;
extern s32 D_80125248;
extern s32 D_8012524C;
extern s32 D_80125250;
extern s32 D_80125254;
extern s32 D_80125258;
extern s32 D_8012525C;
extern s32 D_80125260;
extern s32 D_80125264;
extern s32 D_80125268;
extern s32 D_8012526C;
extern s32 spot_date_sections;
extern s32 D_80124204;
extern s32 D_80124208;
extern s32 D_8012420C;
extern s32 D_80124210;
extern s32 D_80124214;
extern s32 D_80124218;
extern s32 D_8012421C;
extern s32 D_80124220;
extern s32 D_80124228;
extern s32 D_8012422C;
extern s32 D_80124230;
extern s32 D_80124234;
extern s32 D_80124238;
extern s32 D_8012423C;
extern s32 D_80124244;
extern s32 D_80124248;
extern s32 D_8012424C;
extern s32 D_80124250;
extern s32 D_80124254;
extern s32 D_80124258;
extern s32 D_8012425C;
extern s32 D_80124260;
extern s32 D_80124264;
extern s32 D_80124268;
extern s32 D_8012426C;
extern s32 D_80124270;
extern s32 D_80124274;
extern s32 D_80124278;
extern s32 D_8012427C;
extern s32 D_80124280;
extern s32 D_80124284;
extern s32 D_80124288;
extern s32 D_8012428C;
extern s32 D_80124294;
extern s32 D_80124298;
extern s32 D_8012429C;
extern s32 D_801242A0;
extern s32 D_801242A4;
extern s32 D_801242A8;
extern s32 D_801242B0;
extern s32 D_801242B4;
extern s32 D_801242B8;
extern s32 D_801242BC;
extern s32 D_801242C0;
extern s32 D_801242C4;
extern s32 D_801242C8;
extern s32 D_801242CC;
extern s32 D_801242D0;
extern s32 D_801242D4;
extern s32 D_801242D8;
extern s32 D_801242DC;
extern s32 D_801242E0;
extern s32 D_801242E4;
extern s32 D_801242E8;
extern s32 D_801242EC;
extern s32 D_801242F0;
extern s32 D_801242F4;
extern s32 D_801242F8;
extern s32 D_80124300;
extern s32 D_80124304;
extern s32 D_80124308;
extern s32 D_8012430C;
extern s32 D_80124310;
extern s32 D_80124314;
extern s32 D_8012431C;
extern s32 D_80124320;
extern s32 D_80124324;
extern s32 D_80124328;
extern s32 D_8012432C;
extern s32 D_80124330;
extern s32 D_80124334;
extern s32 D_80124338;
extern s32 D_8012433C;
extern s32 D_80124340;
extern s32 D_80124240;
extern s32 D_801242AC;
extern s32 D_80124318;
extern s32 spot_script_sections;
extern s32 D_801202D4;
extern s32 D_801202D8;
extern s32 D_801202DC;
extern s32 D_801202E0;
extern s32 D_801202E4;
extern s32 D_801202E8;
extern s32 D_801202EC;
extern s32 D_801202F0;
extern s32 D_801202F4;
extern s32 D_801202F8;
extern s32 D_801202FC;
extern s32 D_80120300;
extern s32 D_80120304;
extern s32 D_80120308;
extern s32 D_8012030C;
extern s32 D_80120310;
extern s32 D_80120314;
extern s32 D_80120318;
extern s32 D_8012031C;
extern s32 D_80120320;
extern s32 D_80120324;
extern s32 D_80120328;
extern s32 D_8012032C;
extern s32 D_80120330;
extern s32 D_80120334;
extern s32 D_80120338;
extern s32 D_8012033C;
extern s32 D_80120340;
extern s32 D_80120344;
extern s32 D_80120348;
extern s32 D_8012034C;
extern s32 D_80120350;
extern s32 D_80120354;
extern s32 D_80120358;
extern s32 D_8012035C;
extern s32 D_80120360;
extern s32 D_80120364;
extern s32 D_80120368;
extern s32 D_8012036C;
extern s32 D_80120370;
extern s32 D_80120374;
extern s32 D_80120378;
extern s32 D_8012037C;
extern s32 D_80120380;
extern s32 D_80120384;
extern s32 D_80120388;
extern s32 D_8012038C;
extern s32 D_80120390;
extern s32 D_80120394;
extern s32 D_80120398;
extern s32 D_8012039C;
extern s32 D_801203A0;
extern s32 D_801203A4;
extern s32 D_801203A8;
extern s32 D_801203AC;
extern s32 D_801203B0;
extern s32 D_801203B4;
extern s32 D_801203B8;
extern s32 D_801203BC;
extern s32 D_801203C0;
extern s32 D_801203C4;
extern s32 D_801203C8;
extern s32 D_801203CC;
extern s32 D_801203D0;
extern s32 D_801203D4;
extern s32 D_801203D8;
extern s32 D_801203DC;
extern s32 D_801203E0;
extern s32 D_801203E4;
extern s32 D_801203E8;
extern s32 D_801203EC;
extern s32 D_801203F0;
extern s32 D_801203F4;
extern s32 D_801203F8;
extern s32 D_801203FC;
extern s32 D_80120400;
extern s32 D_80120404;
extern s32 D_80120408;
extern s32 D_8012040C;
extern s32 D_80120410;
extern s32 D_80120414;
extern s32 D_80120418;
extern s32 D_8012041C;
extern s32 D_80120420;
extern s32 D_80120424;
extern s32 D_80120428;
extern s32 D_8012042C;
extern s32 D_80120430;
extern s32 D_80120434;
extern s32 D_80120438;
extern s32 D_8012043C;
extern s32 D_80120440;
extern s32 D_80120444;
extern s32 D_80120448;
extern s32 D_8012044C;
extern s32 D_80120450;
extern s32 D_80120454;
extern s32 D_80120458;
extern s32 D_8012045C;
extern s32 D_80120460;
extern s32 D_80120464;
extern s32 D_80120468;
extern s32 D_8012046C;
extern s32 D_80120470;
extern s32 D_80120474;
extern s32 D_80120478;
extern s32 D_8012047C;
extern s32 D_80120480;
extern s32 D_80120484;
extern s32 D_80120488;
extern s32 D_8012048C;
extern s32 D_80120490;
extern s32 D_80120494;
extern s32 D_80120498;
extern s32 D_8012049C;
extern s32 D_801204A0;
extern s32 D_801204A4;
extern s32 D_801204A8;
extern s32 D_801204AC;
extern s32 D_801204B0;
extern s32 D_801204B4;
extern s32 D_801204B8;
extern s32 D_801204BC;
extern s32 D_801204C0;
extern s32 D_801204C4;
extern s32 D_801204C8;
extern s32 D_801204CC;
extern s32 D_801204D0;
extern s32 D_801204D4;
extern s32 D_801204D8;
extern s32 D_801204DC;
extern s32 D_801204E0;
extern s32 D_801204E4;
extern s32 D_801204E8;
extern s32 D_801204EC;
extern s32 D_801204F0;
extern s32 D_801204F4;
extern s32 D_801204F8;
extern s32 D_801204FC;
extern s32 D_80120500;
extern s32 D_80120504;
extern s32 D_80120508;
extern s32 D_8012050C;
extern s32 D_80120510;
extern s32 D_80120514;
extern s32 D_80120518;
extern s32 D_8012051C;
extern s32 D_80120520;
extern s32 D_80120524;
extern s32 D_80120528;
extern s32 D_8012052C;
extern s32 D_80120530;
extern s32 D_80120534;
extern s32 D_80120538;
extern s32 D_8012053C;
extern s32 D_80120540;
extern s32 D_80120544;
extern s32 D_80120548;
extern s32 D_8012054C;
extern s32 D_80120550;
extern s32 D_80120554;
extern s32 D_80120558;
extern s32 D_8012055C;
extern s32 D_80120560;
extern s32 D_80120564;
extern s32 D_80120568;
extern s32 D_8012056C;
extern s32 D_80120570;
extern s32 D_80120574;
extern s32 D_80120578;
extern s32 D_8012057C;
extern s32 D_80120580;
extern s32 D_80120584;
extern s32 D_80120588;
extern s32 D_8012058C;
extern s32 D_80120590;
extern s32 D_80120594;
extern s32 D_80120598;
extern s32 D_8012059C;
extern s32 D_801205A0;
extern s32 D_801205A4;
extern s32 D_801205A8;
extern s32 D_801205AC;
extern s32 D_801205B0;
extern s32 D_801205B4;
extern s32 D_801205B8;
extern s32 D_801205BC;
extern s32 D_801205C0;
extern s32 D_801205C4;
extern s32 D_801205C8;
extern s32 D_801205CC;
extern s32 D_801205D0;
extern s32 D_801205D4;
extern s32 D_801205D8;
extern s32 D_801205DC;
extern s32 D_801205E0;
extern s32 D_801205E4;
extern s32 D_801205E8;
extern s32 D_801205EC;
extern s32 D_801205F0;
extern s32 D_801205F4;
extern s32 D_801205F8;
extern s32 D_801205FC;
extern s32 D_80120600;
extern s32 D_80120604;
extern s32 D_80120608;
extern s32 D_8012060C;
extern s32 D_80120610;
extern s32 D_80120614;
extern s32 D_80120618;
extern s32 D_8012061C;
extern s32 D_80120620;
extern s32 D_80120624;
extern s32 D_80120628;
extern s32 D_8012062C;
extern s32 D_80120630;
extern s32 D_80120634;
extern s32 D_80120638;
extern s32 D_8012063C;
extern s32 D_80120640;
extern s32 D_80120644;
extern s32 D_80120648;
extern s32 D_8012064C;
extern s32 D_80120650;
extern s32 D_80120658;
extern s32 D_8012066C;
extern s32 D_80120670;
extern s32 D_80120674;
void func_80011E28();
void func_80011EC4();
void func_800335E0();
void func_800201FC();
extern u8 D_801243B4;
extern u8 spot_arrival_mode;
extern u8 spot_ride;
extern u8 D_80125324;
extern u8 D_801252F4;
extern u8 D_801252F8;
extern u8 D_801252FC;
extern s16 spot_reaction_tier_cells;
extern s16 D_80124396;
extern s16 D_80124398;
extern s16 spot_reaction_cells;
extern s16 D_8012439E;
extern s16 D_801243A0;
extern s16 spot_reaction_low_cells;
extern s16 D_801243A6;
extern s16 D_801243A8;
void func_80015D28();
void spot_yuko_still_pointers_1_init();
void spot_yuko_still_pointers_2_init();
void spot_yuko_still_pointers_3_init();
void spot_yuko_still_pointers_4_init();
void spot_yuko_still_pointers_5_init();
void spot_yuko_still_pointers_6_init();
void spot_shiori_overlay_pointers_1_init();
void spot_shiori_still_pointers_4_init();
void spot_shiori_still_pointers_1_init();
void spot_shiori_still_pointers_2_init();
void spot_shiori_still_pointers_3_init();
void spot_shiori_tile_pointers_1_init();
void spot_yuina_still_pointers_1_init();
void spot_yuina_tile_pointers_1_init();
void spot_yuina_still_pointers_2_init();
void spot_yuina_overlay_pointers_1_init();
void spot_rei_tile_pointers_1_init();
void spot_ayako_still_pointers_1_init();
void spot_ayako_still_pointers_2_init();
void spot_ayako_still_pointers_3_init();
void spot_ayako_still_pointers_4_init();
void spot_ayako_still_pointers_5_init();
void spot_mira_still_pointers_1_init();
void spot_mira_still_pointers_2_init();
void spot_mira_still_pointers_3_init();
void spot_mira_still_pointers_4_init();
void spot_mira_still_pointers_5_init();
void spot_mio_still_pointers_1_init();
void spot_mio_still_pointers_2_init();
void spot_mio_still_pointers_3_init();
void spot_mio_still_pointers_4_init();
void spot_mio_still_pointers_5_init();
void spot_yukari_still_pointers_1_init();
void spot_yukari_still_pointers_2_init();
void spot_yukari_still_pointers_3_init();
void spot_yukari_still_pointers_4_init();
void spot_yukari_tile_pointers_1_init();
void spot_nozomi_still_pointers_1_init();
void spot_nozomi_still_pointers_2_init();
void spot_nozomi_still_pointers_3_init();
void spot_nozomi_still_pointers_4_init();
void spot_nozomi_still_pointers_5_init();
void spot_megumi_overlay_pointers_1_init();
void spot_megumi_still_pointers_1_init();
void spot_megumi_still_pointers_2_init();
void spot_megumi_still_pointers_3_init();
void spot_saki_still_pointers_1_init();
void spot_saki_still_pointers_2_init();
void spot_saki_still_pointers_3_init();
void spot_saki_still_pointers_4_init();
void spot_saki_still_pointers_5_init();
void spot_yumi_still_pointers_1_init();
void spot_yumi_still_pointers_6_init();
void spot_yumi_still_pointers_2_init();
void spot_yumi_still_pointers_3_init();
void spot_yumi_still_pointers_4_init();
void spot_miharu_still_pointers_1_init();
void spot_yoshio_still_pointers_1_init();
void spot_place_frame_pointers_init();
void func_80015FE0();
void spot_shiori_scene1_steps_run();
void spot_shiori_scene2_steps_run();
void spot_shiori_scene8_steps_run();
void spot_outcome_apply_step();
void spot_fill_sections_common();
void spot_place_result_pointers_init();
void spot_yumi_still_pointers_5_init();
void spot_place_asset_pointers_init();
void func_800143DC();
void spot_megumi_still_pointers_4_init();
void func_800137AC();
void func_8001A16C();
void spot_yumi_scene7_gradient_quad_draw();
void spot_talk_step();
void spot_portrait_read_a();
void spot_portrait_read_b();
extern u8 spot_place;
void spot_place_empty_step();
void spot_script_sections_init();
extern s16 spot_face_cells;
extern s16 D_8012438E;
extern s16 D_80124390;
void spot_reaction_rows_build_step();
void func_800189C4();
void func_8001A0CC();
void spot_gradient_quad_draw();
void func_8001A1EC();
s32 func_8002328C();
extern u8 D_80125300;
s32 func_8002336C();
extern u8 D_80125304;
extern u8 D_80125308;
void func_80033E88();
extern s8 D_80125310;
s32 spot_date_event_choose(void);
void spot_yuina_still_pointers_3_init();
void spot_yuko_animation_pointers_1_init();
void spot_yuko_scene1_steps_run();
void spot_yuko_scene4_gradient_sprites_draw();
void spot_megumi_scene1_steps_run();
void spot_megumi_scene2_steps_run();
void spot_megumi_scene1_sprites_update();

void spot_saki_scene1_steps_run(void);

void spot_mira_scene1_steps_run();

void spot_mira_scene8_steps_run();
void spot_mio_scene1_steps_run();
void spot_miharu_scene1_steps_run();
void spot_miharu_scene2_steps_run();
void spot_miharu_scene4_steps_run();
void spot_yoshio_scene1_steps_run();

extern u8 D_8012436C;
extern s16 D_80124370;
extern u8 D_80125314;
s32 spot_shiori_scene1_sprites_update(void);
extern s16 D_80120CF0[16];
extern s16 D_80120D10[16];

/* T-24081: signed-field view of SpriteQuad at +0x0A/+0x0E so constant 8 ($s3) and 0x18 ($s5) are shared with s16 unk_08
 * without taking the address of a cast u8[] element (which folds into base + offset instead of ixa). */
typedef struct SpotSpriteQuad {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s8 unk_0E;
    /* 0x0F */ u8 pad_0F[0x15];
} SpotSpriteQuad; /* size 0x24 */
void spot_megumi_scene1_sprites_update(void);

extern u8 D_80125328;
void spot_rei_scene1_steps_run();
void spot_rei_scene2_steps_run();
void spot_rei_scene3_steps_run();
extern s8 D_801252E8;
extern s16 spot_dialogue_group_offset;
extern u8 D_80124374;
extern s16 D_80124378;
extern u8 D_80125318;
s32 spot_yuina_scene1_steps_run(void);
void spot_yukari_scene1_steps_run();
void spot_yukari_scene2_steps_run();
void spot_yumi_scene1_steps_run();
void spot_yumi_scene2_steps_run();
void spot_yumi_scene7_steps_run();
extern s8 D_80123988;
extern s32 D_8012398C;
void spot_yumi_scene7_step_37(void);
void spot_yumi_scene2_branch_on_mood_14(void);
void spot_yuina_scene3_set_state_17(void);
void spot_yuina_scene2_still_show_stats_03(void);
void spot_yukari_scene6_still_show_stats_03(void);
void spot_yumi_scene5_still_show_stats_03(void);
void spot_yuina_scene5_still_show_stats_03(void);
void spot_yuina_scene5_run(void);
void spot_yuina_scene1_run();
void spot_yuina_scene2_run();
void spot_yuina_scene3_run();
void spot_yuina_scene4_run();
void spot_yukari_scene1_run();
void spot_yukari_scene2_run();
void spot_yukari_scene3_run();
s32 spot_yukari_scene4_run(void);
void spot_yukari_scene4_sprites_update(void);
void spot_yukari_scene5_run();
void spot_yukari_scene6_run();
void spot_yumi_scene1_run();
void spot_yumi_scene2_run();
void spot_yumi_scene3_run();
void spot_yumi_scene4_run();
void spot_yumi_scene5_run();
void spot_yumi_scene6_run();
void spot_yumi_scene7_run();
extern s16 spot_mood_tier;
extern u8 D_80124360;
void spot_rei_scene1_run();
void spot_rei_scene2_run();
void spot_rei_scene3_run();

extern s16 D_801207AC;
extern s16 D_801207B0;
extern s16 D_801207B4;
void spot_mio_scene1_run(void);
void spot_mio_scene2_run(void);
void spot_mio_scene3_run(void);
void spot_mio_scene4_run(void);
void spot_mio_scene5_run(void);
void func_80036A8C();
void func_800372E0(s32 a);
void func_80035774(void);
void func_800358D0(void);
void spot_place_scene_frame(void);
s32 spot_place_shopping_street_branch_27(void);
void spot_place_shopping_street_branch_on_mood_16(void);
void spot_place_shopping_street_branch_on_mood_18(void);
void spot_place_pool_face_step_24(void);
void spot_place_sea_face_step_35(void);
s32 spot_arrival_talk_step();
s32 spot_arrival_talk_gate_step(void);
void spot_talk_until_done_step(void);
extern u8 D_80124364;
extern u8 D_80124368;
void spot_place_neighborhood_park_talk_variant_06(void);
void spot_rei_scene3_bg_load_02(void);
void spot_yoshio_scene1_bg_load_18(void);

void spot_yuina_scene3_se_a(void);
void spot_yuina_scene3_se_b(void);

void spot_saki_scene1_run(void);

extern u8 D_8012411C;
void spot_yumi_scene7_flash_draw(void);
void spot_yumi_scene7_sprites_update(void);
void spot_yumi_scene4_se_play(void);
void spot_yuko_scene3_gradient_sprites_draw(void);

void spot_miharu_scene1_run(void);
void spot_miharu_scene2_run(void);
void spot_miharu_scene4_run(void);
void spot_miharu_scene3_run(void);
void spot_miharu_scene5_run(void);
void spot_shiori_scene1_run(void);
void spot_shiori_scene2_run(void);
void spot_shiori_scene3_run(void);
void spot_shiori_scene4_run(void);
void spot_shiori_scene5_run(void);
void spot_shiori_scene6_run(void);
void spot_shiori_scene8_run(void);
void spot_saki_scene4_run(void);
void spot_saki_scene5_run(void);
void spot_saki_scene2_run(void);
void spot_saki_scene3_run(void);
extern s8 D_801212B4;
void spot_nozomi_scene1_run(void);
void spot_nozomi_scene2_run(void);
void spot_nozomi_scene3_run(void);
void spot_nozomi_scene4_run(void);
void spot_nozomi_scene5_run(void);
void spot_megumi_scene1_run(void);
void spot_megumi_scene2_run(void);
void spot_megumi_scene3_run(void);
void spot_megumi_scene4_run(void);
void spot_megumi_scene5_run(void);
void spot_megumi_scene6_run(void);
void spot_mira_scene1_run(void);
void spot_mira_scene8_run(void);
void spot_mira_scene2_run(void);
void spot_mira_scene3_run(void);
void spot_mira_scene4_run(void);
void spot_mira_scene5_run(void);
void spot_mira_scene6_run(void);
void spot_mira_unused_branch_on_mood_2(void);
void spot_nozomi_scene4_branch_on_mood_07(void);

#endif /* OVL_EVENT_H */
extern s16 D_80124348;
void spot_ayako_scene1_run(void);
void spot_ayako_scene2_run(void);
void spot_ayako_scene3_run();
void spot_ayako_scene4_run();
void spot_ayako_scene5_run();
void spot_yuko_scene1_run(void);
void spot_yuko_scene2_run(void);
void spot_yuko_scene3_run(void);
void spot_yuko_scene4_run(void);
void spot_yuko_scene5_run(void);
void spot_yuko_scene6_run(void);
extern s8 D_801218DC[];

#ifndef OVL_EVENT_W8_3_TYPES_H
#define OVL_EVENT_W8_3_TYPES_H
typedef struct EventCallbackTable47 {
    /* 0x00 */ void (*callback[47])(s32);
} EventCallbackTable47; /* size 0xBC */
extern EventCallbackTable47 spot_shiori_scene8_steps;
void spot_shiori_scene8_face_update(void);
void spot_megumi_scene1_still_fade_out_37(void);
void spot_megumi_scene1_still_fade_in_35(void);

#endif /* OVL_EVENT_W8_3_TYPES_H */

#ifndef OVL_EVENT_RECORDS_T9740
#define OVL_EVENT_RECORDS_T9740
/* D_800E9620 records 30/31 are read as Actor (game.h, T-9740, T-20900).
 * D_800EC190 uses the shared SpriteQuad game-record layout. */
s32 spot_yoshio_scene1_step_37(void);
#endif

#ifndef EVENT_TABLE_E7_T9940
#define EVENT_TABLE_E7_T9940
typedef struct SpotPlaceAffinityTable {
    /* 0x00 */ u8 unk_00[11][21]; /* HYPOTHESIS: type used for 11 heroines x 21 places of preference values; body indexes it as table[girl][place] */
} SpotPlaceAffinityTable; /* size 0xE7; inline copy includes a three-byte tail */
extern SpotPlaceAffinityTable spot_place_affinity;
#endif


#ifndef OVL_EVENT_T10040
#define OVL_EVENT_T10040
/* Forward declarations for the original object, T-10040. */
void spot_ride_movie_pointers_init(void);
u16 spot_ride_scream_virtual_movie(void);
s32 spot_ride_ferris_wheel_movie(void);
void spot_ride_coaster_movie(void);
void spot_ride_ghost_house_movie(void);

/* T-10040: dialogue data and object-local forward declarations. */
s32 spot_ride_movie_shift_sprites(s16 arg0);

/* Stage/fade view at D_800B1AF0; store/load ordering in 8011EDFC. */
typedef struct SpotDevelopmentStepViewShort {
    /* 0x00 */ s32 step_timer;
    /* 0x04 */ u8 pad04[2];
    /* 0x06 */ u8 step;
    /* 0x07 */ u8 pad07;
} SpotDevelopmentStepViewShort; /* size 0x08; T-10040 */
extern s16 D_801258E4[3];
extern s16 D_801258EC[3];
extern s16 D_801258F4[3];
extern s16 D_801258FC[3];
extern u16 D_801258DC;
typedef struct EvFnTbl53 { void (*f[53])(); } EvFnTbl53;
extern EvFnTbl53 spot_mio_scene5_steps;
typedef struct EvFnTbl44 { void (*f[44])(); } EvFnTbl44;
extern EvFnTbl44 spot_yukari_scene4_steps;
void spot_place_shrine_set_state_38(void);

#endif

#ifndef OVL_EVENT_T10630_H
#define OVL_EVENT_T10630_H
/* Movie control bytes at D_800F1990, cleared by spot_ride_coaster_movie. */
typedef struct SpotRideMovieWork {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ u8 pad08[2];
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 pad0C[12];
    /* 0x18 */ s8 unk_18;
    /* 0x19 */ s8 unk_19;
    /* 0x1A */ s8 unk_1A;
    /* 0x1B */ u8 pad1B;
} SpotRideMovieWork; /* size 0x1C, minimum observed extent */
s32 func_80015A44();
void func_8002469C();
void func_80027438();
void func_8002746C();
void func_8002755C();
void func_800352B4();
#endif

/* T-10630: signed UV step and RNG seed helper in the EVENT object. */
extern s16 D_801258E0;
void spot_bios_srand(u32 seed);

#ifndef EVENT_SELECTION_TYPES_T10730
#define EVENT_SELECTION_TYPES_T10730
/* T-10730: unsigned triples selected by girl, option, then its two-state flag. */
typedef struct SpotResultTriple {
    /* 0x00 */ u8 value[3];
} SpotResultTriple; /* size 0x03 */

typedef struct SpotShopChoiceTable {
    /* 0x00 */ SpotResultTriple choice[3][2];
} SpotShopChoiceTable; /* size 0x12 */

typedef struct SpotRideChoiceTable {
    /* 0x00 */ SpotResultTriple choice[6][2];
} SpotRideChoiceTable; /* size 0x24 */

#endif

#ifndef OVL_EVENT_MOTION_T10730
#define OVL_EVENT_MOTION_T10730
typedef struct SpotMuseumChoiceTable {
    /* 0x00 */ SpotResultTriple choice[5];
} SpotMuseumChoiceTable; /* size 0x0F */
#endif

#ifndef OVL_EVENT_STATE_T10730
#define OVL_EVENT_STATE_T10730
/* T-10730: legacy EVENT state, offsets relative to D_800B0860. */
typedef struct SpotDevelopmentState {
    /* T-24017: signed halfwords indexed by the word at D_800E9600. */
    /* 0x000 */ u8 pad_000[0x14];
    /* 0x014 */ s16 screen_center_x[2];
    /* 0x018 */ s16 screen_center_y[2];
    /* 0x01C */ u8 pad_01C[0x19];
    /* 0x035 */ u8 year;
    /* 0x036 */ u8 month;
    /* 0x037 */ u8 day;
    /* 0x038 */ u8 pad_038[0xAC];
    /* 0x0E4 */ GirlFlags player_flags; /* HYPOTHESIS: dev counterpart of core's player_flags (shipped +0xF4; dev +0xE4 by the +0x10 shift of the parameter block); the special event chooser tests bit 1 of it for the Yuko winter event where the DATE twin tests ScheduleFlags bit 17 of player_flags in the same condition (dev bit numbering assumed) */
    /* 0x0E8 */ u8 pad_0E8[0x16];
    /* 0x0FE */ s16 athletics;
    /* 0x100 */ u8 pad_100[0xA4];
    /* 0x1A4 */ LegacyGirlRecord girls[12];
    /* 0x414 */ u8 pad_414[0x1B6];
    /* 0x5CA */ u8 scene_flags[0x4B];
    /* 0x615 */ u8 pad_615[0xB3];
    /* 0x6C8 */ s8 saved_game_mode;
    /* 0x6C9 */ s8 saved_mode_stage;
    /* 0x6CA */ s8 saved_mode_step;
    /* T-15200: selector-indexed bytes at D_800B0F35 and flag D_800B0F45.
     * Twelve slots follow the existing LegacyGirlRecord table extent; semantics unknown. */
    /* 0x6CB */ u8 pad_6CB[0xA];
    /* 0x6D5 */ u8 girl_last_date_spot[12];
    /* 0x6E1 */ u8 pad_6E1[4];
    /* 0x6E5 */ u8 appointment_flag;
    /* 0x6E6 */ u8 pad_6E6[0x800];
    /* 0xEE6 */ u8 current_girl;
    /* 0xEE7 */ u8 pad_EE7[0x25];
    /* 0xF0C */ s32 pad_pressed;
    /* 0xF10 */ u8 pad_F10[0x127];
    /* 0x1037 */ u8 unk_1037;
    /* 0x1038 */ u8 pad_1038[0x25D];
    /* 0x1295 */ u8 stage;
    /* 0x1296 */ u8 step;
    /* 0x1297 */ u8 pad_1297;
} SpotDevelopmentState; /* minimum size 0x1298 */

/* Unsigned counter view at D_800B1AF0 (spot_ayako_scene2_step_23, matching DATE game_state.unk_1104.u). */
typedef struct SpotFrameCounter {
    /* 0x00 */ u32 u;
} SpotFrameCounter; /* size 0x04 */

/* Bit 1 of LegacyGirlRecord.unk_0C is tested with a word shift/sign test (T-14300). */
typedef struct SpotLegacyGirlFlagBits {
    /* 0x00 */ u32 lo : 1;
    /* 0x00 */ u32 bit1 : 1;
    /* 0x00 */ u32 hi : 30;
} SpotLegacyGirlFlagBits; /* size 0x04 */

/* T-24103: LegacyGirlRecord view with GirlFlags at +0x0C, WordView at +0x10 and the 0x20-byte
 * place-visit counter array at +0x14 (spot_place_neighborhood_park_special_case_04, spot_scene_close_step). */
typedef struct SpotLegacyGirlRecord {
    /* 0x00 */ s16 tokimeki_fraction;
    /* 0x02 */ s16 tokimeki;
    /* 0x04 */ s16 affection_fraction;
    /* 0x06 */ s16 affection;
    /* 0x08 */ s16 syoushin_fraction;
    /* 0x0A */ s16 syoushin; /* HYPOTHESIS: game interpretation unknown; retain core's developer term for the halfword at +0xA; in this build the scene steps subtract 10 to 28 after good outcomes and add 10 to 20 after bad ones, and spot_scene_close_step subtracts 0x28 */
    /* 0x0C */ GirlFlags flags;
    /* 0x10 */ WordView birthday;
    /* 0x14 */ u8 scene_counters[0x20];
} SpotLegacyGirlRecord; /* size 0x34 */

typedef struct SpotDevelopmentStateGirls {
    /* 0x000 */ u8 pad_000[0x35];
    /* 0x035 */ u8 year;
    /* 0x036 */ u8 month;
    /* 0x037 */ u8 day;
    /* 0x038 */ u8 pad_038[0xCE];
    /* 0x106 */ s16 looks;
    /* 0x108 */ u8 pad_108[0x9C];
    /* 0x1A4 */ SpotLegacyGirlRecord girls[12];
    /* 0x414 */ u8 pad_414[0x2B8];
    /* 0x6CC */ u8 appointment_place_0; /* HYPOTHESIS: name follows core's appointment_place_0 (shipped unk_724, a core HYPOTHESIS: recent appointment places); dev +0x6CC is D_800B0F2C, which spot_place_init_step copies to spot_place as DATE func_80144720 copies unk_724 to D_800CA2F8, and spot_scene_close_step writes the date place back here as DATE func_8013CDF8 writes unk_724 */
    /* 0x6CD */ u8 pad_6CD[8];
    /* 0x6D5 */ u8 girl_last_date_spot[12];
    /* 0x6E1 */ u8 pad_6E1[0x805];
    /* 0xEE6 */ u8 current_girl;
    /* 0xEE7 */ u8 pad_EE7[0x39D];
    /* 0x1284 */ s32 frame_count;
    /* 0x1288 */ u8 pad_1288[8];
    /* 0x1290 */ s32 step_timer;
    /* 0x1294 */ u8 mode;
} SpotDevelopmentStateGirls; /* minimum size 0x1298 */

/* T-24103: 16.16 fixed-point word view of LegacyGirlRecord +0x00/+0x04/+0x08 (spot_outcome_apply_step). */
typedef struct SpotLegacyGirlRecordFixed {
    /* 0x00 */ s32 tokimeki_fixed;
    /* 0x04 */ s32 affection_fixed;
    /* 0x08 */ s32 syoushin_fixed;
    /* 0x0C */ u8 pad_0C[0x28];
} SpotLegacyGirlRecordFixed; /* size 0x34 */

typedef struct SpotDevelopmentStateFixed {
    /* 0x000 */ u8 pad_000[0x1A4];
    /* 0x1A4 */ SpotLegacyGirlRecordFixed girls[12];
} SpotDevelopmentStateFixed; /* size 0x414 */

extern u8 D_801252EC;
extern u8 D_801252F0;
#endif

#ifndef OVL_EVENT_MOVIE_STATE_T10830_H
#define OVL_EVENT_MOVIE_STATE_T10830_H
/* Scene-control view at D_800B1AF0. Extends SpotDevelopmentStepViewShort's
 * observed prefix with the page selector used by spot_ride_scream_virtual_movie. */
typedef struct SpotDevelopmentStepView {
    /* 0x00 */ s32 step_timer;
    /* 0x04 */ u8 mode;
    /* 0x05 */ u8 stage;
    /* 0x06 */ u8 step;
    /* 0x07 */ u8 pad07[2];
    /* 0x09 */ u8 substep;
    /* 0x0A */ u8 pad0A[2];
} SpotDevelopmentStepView; /* size 0x0C; observed fields through byte 0x09 */
#endif

#ifndef OVL_EVENT_PARTICLE_T10870_H
#define OVL_EVENT_PARTICLE_T10870_H
/* Partial state view at D_800B0860. The word table's observed accesses
 * extend through element 57; its full allocation size is not yet known. */
typedef struct SpotDevelopmentEffectView {
    /* 0x0000 */ u8 pad0000[0x1031];
    /* 0x1031 */ u8 unk_1031[6];
    /* 0x1037 */ u8 pad1037[0x371];
    /* 0x13A8 */ s32 unk_13A8[58];
} SpotDevelopmentEffectView; /* size 0x1490, minimum observed extent */

extern s32 D_801257DC[10];
extern s16 D_8012527C;
extern s32 D_8012585C;
extern s16 D_80125860;
extern u8 D_80125864;
/* spot_place_kirameki_central_park_step_10 preserves twenty horizontal coordinate words. */
extern WordView D_80125868[20];
#endif

#ifndef OVL_EVENT_RESULT_ROW_T10870_H
#define OVL_EVENT_RESULT_ROW_T10870_H
/* Three signed results at spot_face_cells and spot_reaction_cells. The rows are
 * separate allocations/views, not adjacent elements of one array. */
typedef struct SpotResultCells {
    /* 0x00 */ s16 value[3];
} SpotResultCells; /* size 0x06 */
#endif
typedef struct SpotBirthdayBits {
    /* 0x00 */ u32 month : 4;
    /* 0x00 */ u32 day : 5;
    /* 0x00 */ u32 rest : 23;
} SpotBirthdayBits; /* size 0x04 */
extern s8 D_8012532C;
extern s16 D_80124354;
extern u8 D_80124358;
extern u8 D_801243AC;
extern s16 D_801243A2;
extern s8 D_8012530C;
void func_80011E8C();

typedef struct SpotPlaceNameTable {
    /* 0x00 */ u8 unk_00[22][33];
} SpotPlaceNameTable; /* size 0x2D6 */
extern SpotPlaceNameTable spot_place_names;
extern s16 D_8012434C;
extern s16 D_80124350;
void spot_ride_setup(void);

#ifndef OVL_EVENT_STEP_TABLES_T17400
#define OVL_EVENT_STEP_TABLES_T17400
/* Step tables (T-3330, T-17400): one handler per scene step; each handler copies its table to the stack
 * whole and calls tbl.f[D_800B1AF6]. NN is the run of code pointers in the data. The types and externs were
 * local to each .c file before; the tables of the unmatched handlers are declared here too. D_801220EC (FnTbl42)
 * and D_8012277C (FnTbl32) stay main_api.h scalars read through a cast: main_api.h has no override guard for them. */
typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
typedef struct {
    void (*f[15])();
} FnTbl15; /* size 0x3C */
typedef struct {
    void (*f[18])();
} FnTbl18; /* size 0x48 */
typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
typedef struct {
    void (*f[21])();
} FnTbl21; /* size 0x54 */
typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
typedef struct {
    void (*f[29])();
} FnTbl29; /* size 0x74 */
typedef struct {
    void (*f[31])();
} FnTbl31; /* size 0x7C */
typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
typedef struct {
    void (*f[34])();
} FnTbl34; /* size 0x88 */
typedef struct {
    void (*f[35])();
} FnTbl35; /* size 0x8C */
typedef struct {
    void (*f[36])();
} FnTbl36; /* size 0x90 */
typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */
typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
typedef struct {
    void (*f[45])();
} FnTbl45; /* size 0xB4 */
typedef struct {
    void (*f[46])();
} FnTbl46; /* size 0xB8 */
typedef struct {
    void (*f[47])();
} FnTbl47; /* size 0xBC */
typedef struct {
    void (*f[48])();
} FnTbl48; /* size 0xC0 */
typedef struct {
    void (*f[49])();
} FnTbl49; /* size 0xC4 */
typedef struct {
    void (*f[50])();
} FnTbl50; /* size 0xC8 */
typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
typedef struct {
    void (*f[52])();
} FnTbl52; /* size 0xD0 */
typedef struct {
    void (*f[55])();
} FnTbl55; /* size 0xDC */
typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
typedef struct {
    void (*f[58])();
} FnTbl58; /* size 0xE8 */
typedef struct {
    void (*f[61])();
} FnTbl61; /* size 0xF4 */
typedef struct {
    void (*f[63])();
} FnTbl63; /* size 0xFC */
typedef struct {
    void (*f[67])();
} FnTbl67; /* size 0x10C */
typedef struct {
    void (*f[103])();
} FnTbl103; /* size 0x19C */
extern FnTbl52 spot_yuko_scene1_steps;
extern FnTbl41 spot_yuko_scene2_steps;
extern FnTbl35 spot_yuko_scene3_steps;
extern FnTbl47 spot_yuko_scene4_steps;
extern FnTbl42 spot_yuko_scene5_steps;
extern FnTbl42 spot_yuko_scene6_steps;
extern FnTbl39 spot_shiori_scene1_steps;
extern FnTbl22 spot_shiori_scene2_steps;
extern FnTbl45 spot_shiori_scene3_steps;
extern FnTbl44 spot_shiori_scene4_steps;
extern FnTbl58 spot_shiori_scene5_steps;
extern FnTbl45 spot_shiori_scene6_steps;
extern FnTbl42 spot_yuina_scene1_steps; /* unmatched user */
extern FnTbl41 spot_yuina_scene2_steps;
extern FnTbl39 spot_yuina_scene3_steps;
extern FnTbl39 spot_yuina_scene4_steps; /* unmatched user */
extern FnTbl38 spot_yuina_scene5_steps;
extern FnTbl51 spot_rei_scene1_steps;
extern FnTbl37 spot_rei_scene2_steps;
extern FnTbl41 spot_rei_scene3_steps;
extern FnTbl32 spot_ayako_scene1_steps; /* unmatched user */
extern FnTbl49 spot_ayako_scene2_steps;
extern FnTbl48 spot_ayako_scene3_steps; /* unmatched user */
extern FnTbl42 spot_ayako_scene4_steps;
extern FnTbl45 spot_ayako_scene5_steps;
extern FnTbl38 spot_mira_scene1_steps;
extern FnTbl21 spot_mira_scene8_steps;
extern FnTbl40 spot_mira_scene2_steps;
extern FnTbl38 spot_mira_scene3_steps;
extern FnTbl51 spot_mira_scene4_steps;
extern FnTbl15 spot_mira_scene6_steps;
extern FnTbl39 spot_mio_scene1_steps;
extern FnTbl67 spot_mio_scene2_steps;
extern FnTbl39 spot_mio_scene3_steps;
extern FnTbl46 spot_mio_scene4_steps;
extern FnTbl19 spot_yukari_scene2_steps;
extern FnTbl38 spot_yukari_scene3_steps;
extern FnTbl40 spot_yukari_scene5_steps;
extern FnTbl56 spot_yukari_scene6_steps;
extern FnTbl29 spot_nozomi_scene1_steps;
extern FnTbl50 spot_nozomi_scene2_steps;
extern FnTbl55 spot_nozomi_scene3_steps;
extern FnTbl41 spot_nozomi_scene4_steps;
extern FnTbl40 spot_nozomi_scene5_steps;
extern FnTbl45 spot_megumi_scene1_steps;
extern FnTbl20 spot_megumi_scene2_steps;
extern FnTbl47 spot_megumi_scene3_steps;
extern FnTbl63 spot_megumi_scene4_steps;
extern FnTbl36 spot_megumi_scene5_steps;
extern FnTbl36 spot_megumi_scene6_steps;
extern FnTbl35 spot_saki_scene1_steps;
extern FnTbl37 spot_saki_scene2_steps;
extern FnTbl55 spot_saki_scene3_steps;
extern FnTbl40 spot_saki_scene4_steps;
extern FnTbl51 spot_saki_scene5_steps;
extern FnTbl47 spot_yumi_scene1_steps;
extern FnTbl21 spot_yumi_scene2_steps;
extern FnTbl55 spot_yumi_scene7_steps;
extern FnTbl41 spot_yumi_scene3_steps;
extern FnTbl44 spot_yumi_scene4_steps;
extern FnTbl44 spot_yumi_scene5_steps;
extern FnTbl52 spot_yumi_scene6_steps;
extern FnTbl22 spot_miharu_scene1_steps;
extern FnTbl25 spot_miharu_scene2_steps;
extern FnTbl14 spot_miharu_scene3_steps;
extern FnTbl56 spot_miharu_scene4_steps; /* unmatched user */
extern FnTbl14 spot_miharu_scene5_steps;
extern FnTbl42 spot_yoshio_scene1_steps;
extern FnTbl61 spot_place_arrival_steps; /* unmatched user */
extern FnTbl18 spot_place_neighborhood_park_steps; /* unmatched user */
extern FnTbl35 spot_place_kirameki_central_park_steps; /* unmatched user */
extern FnTbl48 spot_place_shopping_street_steps; /* unmatched user */
extern FnTbl32 spot_place_aquarium_steps; /* unmatched user */
extern FnTbl35 spot_place_zoo_steps; /* unmatched user */
extern FnTbl34 spot_place_botanical_garden_steps; /* unmatched user */
extern FnTbl36 spot_place_planetarium_steps; /* unmatched user */
extern FnTbl37 spot_place_art_museum_steps; /* unmatched user */
extern FnTbl36 spot_place_library_steps; /* unmatched user */
extern FnTbl39 spot_place_game_center_steps; /* unmatched user */
extern FnTbl32 spot_place_bowling_alley_steps; /* unmatched user */
extern FnTbl32 spot_place_karaoke_steps; /* unmatched user */
extern FnTbl103 spot_place_amusement_park_steps; /* unmatched user */
extern FnTbl29 spot_place_stadium_steps; /* unmatched user */
extern FnTbl45 spot_place_movie_theater_steps; /* unmatched user */
extern FnTbl41 spot_place_concert_hall_steps; /* unmatched user */
extern FnTbl37 spot_place_pool_steps; /* unmatched user */
extern FnTbl47 spot_place_sea_steps; /* unmatched user */
extern FnTbl61 spot_place_shrine_steps; /* unmatched user */
extern FnTbl22 spot_place_skating_rink_steps; /* unmatched user */
extern FnTbl31 spot_place_ski_resort_steps; /* unmatched user */
#endif

#ifndef OVL_EVENT_RECORD_COPIES_T17400
#define OVL_EVENT_RECORD_COPIES_T17400
/* T-17400: shared views that were local to the .c files. */
typedef struct {
    s32 a;
    s32 b;
} SpotLinePair; /* size 0x08 */
typedef struct {
    SpotLinePair e[4];
} EvTbl; /* size 0x20: four (a, b) pairs, copied to the stack whole */
extern SpotLinePair D_80121A2C;
extern EvTbl D_80120A74;
extern EvTbl D_80121AD8;
#endif

#ifndef OVL_EVENT_STACK_TABLES_T17400
#define OVL_EVENT_STACK_TABLES_T17400
/* T-17400: constant tables that a handler copies to the stack whole (lw/sw runs plus an lh/sh tail) and
 * then reads by index. Element widths from the reads of the copy (lh for the halfword tables). */
typedef struct {
    u16 v[21];
} SpotPlaceHalfwords; /* size 0x2A */
extern SpotPlaceHalfwords spot_place_bank_argument_f; /* spot_place_bank_load_step */
extern SpotPlaceHalfwords spot_place_bank_argument_d; /* spot_place_bank_load_step */
extern SpotPlaceHalfwords spot_place_bank_argument_e; /* spot_place_bank_load_step */
extern SpotPlaceHalfwords spot_place_bgm_ids; /* spot_place_bgm_start_step */
typedef struct {
    s16 v[8][2];
} EvPairs8; /* size 0x20: spot_yukari_scene4_sprites_update reads v[i][1] (lh 0x6A + 4 * i) */
extern EvPairs8 D_80122990;
typedef struct {
    s16 v[41][2];
} EvShorts82; /* size 0xA4: 41 (start, end) frame pairs in spot_ayako_scene2_step_23 */
extern EvShorts82 D_80121A34; /* spot_ayako_scene2_step_23 */
typedef struct {
    s16 v[10];
} EvShorts10; /* size 0x14 */
extern EvShorts10 D_80123B7C; /* spot_yumi_scene7_still_show_07, with the next two */
extern EvShorts10 D_80123B90;
extern EvShorts10 D_80123BA4;
typedef struct {
    s16 v[11];
} EvShorts11; /* size 0x16 */
extern EvShorts11 D_80125330; /* spot_place_neighborhood_park_special_case_04 */
extern EvShorts11 D_80125348;
typedef struct {
    s16 v[5];
} EvShorts5; /* size 0x0A */
extern EvShorts5 spot_outcome_terms; /* spot_outcome_apply_step */
#endif

#ifndef OVL_EVENT_PROTOS_T17400
#define OVL_EVENT_PROTOS_T17400
void spot_bars_overlay_draw(void); /* T-17400: defined in 8010A210.c after its first caller */
s32 spot_ayako_scene2_step_23(void);
s32 spot_ayako_scene2_pick_maximum_input(void);
s32 spot_place_arrival_run(void);
s32 spot_place_scene_run(void);
void spot_place_neighborhood_park_arrival(void);
void spot_place_kirameki_central_park_arrival(void);
void spot_place_aquarium_arrival(void);
void spot_place_zoo_arrival(void);
void spot_place_botanical_garden_arrival(void);
void spot_place_planetarium_arrival(void);
void spot_place_art_museum_arrival(void);
void spot_place_library_arrival(void);
void spot_place_game_center_arrival(void);
void spot_place_bowling_alley_arrival(void);
void spot_place_karaoke_arrival(void);
void spot_place_amusement_park_arrival(void);
void spot_place_shopping_street_arrival(void);
void spot_place_stadium_arrival(void);
void spot_place_movie_theater_arrival(void);
void spot_place_concert_hall_arrival(void);
void spot_place_pool_arrival(void);
void spot_place_sea_arrival(void);
void spot_place_shrine_arrival(void);
void spot_place_skating_rink_arrival(void);
void spot_place_ski_resort_arrival(void);
void spot_reaction_rows_build_step(void);
void spot_particles_update(void);
void spot_place_falling_sprites_hide(void);
s32 spot_place_falling_sprites_update(void);
#endif

#ifndef OVL_EVENT_LEGACY_RENDER_T24017
#define OVL_EVENT_LEGACY_RENDER_T24017
/* Legacy address is outside the current MAIN executable range. The returned
 * word is consumed as a packet pointer by spot_yumi_scene7_gradient_quad_draw. T-24017. */
void *func_80019D00();
#endif

#ifndef OVL_EVENT_SCENE_ARRAYS_T24037
#define OVL_EVENT_SCENE_ARRAYS_T24037
/* T-24037: EVENT load-time aliases; loops bound each array to eight entries. */
extern u8 D_8012274C[8]; /* flags, lbu/sb stride 1 */
extern s16 D_80122754[8]; /* x coordinates, lh stride 2 */
extern s16 D_80122764[8]; /* y coordinates, lh stride 2 */
extern u8 D_80122774[8]; /* counters, lbu/sb stride 1 */
typedef s16 EvMovieShort;
extern EvMovieShort D_800F19D0[4]; /* spot_ayako_scene2_pick_maximum_input */

#endif
