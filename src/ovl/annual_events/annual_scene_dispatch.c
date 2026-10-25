/* annual_scene_dispatch.c: ; src/ovl/GYOZI/80138510.c:46-71 dispatches five scene-stage handlers. Original object 0x80138510-0x801388E0 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

void annual_scene_resource_addresses_init(void) {
    D_801470F0 = 0x80197210;
    D_801470F4 = 0x80197504;
    D_801470F8 = 0x801978B8;
    D_801470FC = 0x80197D44;
    D_80147100 = 0x801982E0;
    D_80147104 = 0x801987D0;
    D_80147108 = 0x80198B1C;
    D_8014710C = 0x80198E50;
    D_80147110 = 0x801991E4;
    D_80147114 = 0x80199504;
    D_80147118 = 0x80199B14;
    D_8014711C = 0x8019A0E4;
    D_80147120 = 0x8019A380;
    D_80147124 = 0x80197234;
    D_80147128 = 0x8019752C;
    D_8014712C = 0x801978EC;
    D_80147130 = 0x80197D68;
    D_80147134 = 0x80198340;
    D_80147138 = 0x801987EC;
    D_8014713C = 0x80198B40;
    D_80147140 = 0x80198E6C;
    D_80147144 = 0x80199204;
    D_80147148 = 0x80199520;
    D_8014714C = 0x80199B88;
    D_80147150 = 0x8019A100;
    D_80147154 = 0x8019A39C;
    D_80147158 = 0x80197400;
    D_8014715C = 0x801975CC;
    D_80147160 = 0x80197B4C;
    D_80147164 = 0x80197EE0;
    D_80147168 = 0x8019865C;
    D_8014716C = 0x80198910;
    D_80147170 = 0x80198CB8;
    D_80147174 = 0x80198FC8;
    D_80147178 = 0x80199328;
    D_8014717C = 0x80199628;
    D_80147180 = 0x80199ECC;
    D_80147184 = 0x8019A224;
    D_80147188 = 0x8019A4A4;
}

void annual_scene_group_mode_update(void) {
    func_8008E6AC();
    switch (D_800F6479) {
    case 0:
        annual_scene_setup_mode_step_dispatch();
        break;
    case 1:
        annual_candidate_scene_mode_step_dispatch();
        break;
    case 2:
        annual_result_mode_step_dispatch();
        break;
    case 3:
        annual_result_followup_mode_step_dispatch();
        break;
    default:
        talk_voice_play();
        break;
    }
    func_8008A148();
    func_8008F54C();
    func_80076450(2);
    func_80074A14();
    func_80075BFC();
    func_800748B8();
    func_8008E8B4();
}

void annual_scene_group_load_setup(void) {
    D_800D9234 = (u8 *)&annual_scene_group_talk_group;
    D_800D9238 = (u8 *)&annual_scene_group_talk_line;
    D_800D923C = annual_scene_group_talk_text_table;
    D_800D9240 = annual_scene_group_talk_command_table;
    D_800D9244 = annual_scene_group_talk_voice_table;
    func_8008D610(D_8012E66C, 1, 0);
}
