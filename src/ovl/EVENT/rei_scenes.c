/* rei_scenes.c: src/ovl/EVENT/800FD2F0.c:7: girl 12 (Rei) scenes 1-3. Original object 0x800FD2F0-0x800FDC20 (config/objects/EVENT.txt). */
#define MAIN_API_OVERRIDE_D_800EECB0 /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/EVENT.h"

extern u32 D_800EECB0;

void spot_rei_tile_pointers_1_init(void) {
    spot_rei_tile_pointers_1 = 0x801B0000;
    D_801215E4 = 0x801B2000;
    D_801215E8 = 0x801B6000;
    D_801215EC = 0x801BA000;
    D_801215F0 = 0x801BE000;
    D_801215F4 = 0x801C2000;
    D_801215F8 = 0x801C6000;
}

void spot_rei_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_rei_scene1_run();
        return;
    case 2:
        spot_rei_scene2_run();
        return;
    case 3:
        spot_rei_scene3_run();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void spot_rei_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_rei_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_rei_scene2_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_rei_scene2_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_rei_scene3_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_rei_scene3_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_rei_scene1_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl51 tbl;

    tbl = spot_rei_scene1_steps;
    func_80078950("s%d %d\n", D_800B1AF6, D_800E9E63);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_rei_scene1_step_32(void) {
    u32 r;

    spot_talk_step();
    r = D_800B1AE4 % 60;
    if (r == (D_800B1AE4 * 0)) { /* FAKE: the no-op multiply moves the load into $v1, as in spot_yuina_scene3_run (T-9350) */
        func_800433D0(0x504);
        r = D_800B1AE4 % 60;
    }
    if (r == 0x1E) {
        func_800433D0(0x503);
    }
    func_80011DFC();
}

void spot_rei_scene2_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl37 tbl;

    tbl = spot_rei_scene2_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_rei_scene2_se_play_18(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void spot_rei_scene2_assets_load_07(void) {
    func_80015D28(0x35, 0x801B0000, 0x7EF5);
    spot_rei_tile_pointers_1_init();
    func_80011DFC();
}

void spot_rei_scene3_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = spot_rei_scene3_steps;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_rei_scene1_branch_17(void) {
    if (D_800EECBC != 0) {
        D_80094714 = (u16) D_80094714 + 5;
    }
    func_80011DFC();
}

void spot_rei_scene1_branch_20(void) {
    func_80011DFC();
    if (D_800EECBC != 0) {
        D_800B1AF6 += 0x13;
    }
}

void spot_rei_unused_set_state_1(void) {
    func_80011DFC();
}

void spot_rei_shared_init_1(void) {
    /* Chain over two bytes written as element stores: one li v0,12 for both (T-9210). */
    (&D_800B1746)[0] = (&D_800B0F30)[0] = 0xC;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884("");
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    spot_script_sections_init();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    spot_script_sections_init();
    spot_talk_text_table = D_80120618;
    spot_talk_command_table = D_80120638;
    spot_talk_voice_table = D_80120658;
    func_80078970(D_80094784, "廊下");
    if (D_800EECB0 == 1) {
        D_800B096E += 5;
    }
    func_8004C250();
    func_80011DFC();
}

void spot_rei_scene2_init_00(void) {
    /* Chain over two bytes written as element stores: one li v0,12 for both (T-9210). */
    (&D_800B1746)[0] = (&D_800B0F30)[0] = 0xC;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884("");
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    spot_script_sections_init();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    spot_script_sections_init();
    spot_talk_text_table = D_8012061C;
    spot_talk_command_table = D_8012063C;
    spot_talk_voice_table = (s32)D_8012065C;
    func_80078970(D_80094784, "廊下");
    func_80011DFC();
}

void spot_rei_scene2_still_show_09(void) {
    spot_rei_tile_pointers_1_init();
    func_80012D64(spot_rei_tile_pointers_1, 0x11, 1, 2, 0);
    func_8004C46C(D_801215E4, D_801215E8, D_801215EC, D_801215F0, D_801215F4, D_801215F8);
    func_8004C6B0(0, 0, 0, 0);
    func_80011DFC();
}

void spot_rei_scene3_set_state_01(void) {
    spot_talk_text_table = D_80120620;
    spot_talk_command_table = D_80120640;
    spot_talk_voice_table = (s32) D_80120660;
    func_80011DFC();
}

void spot_rei_scene1_bg_load_01(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void spot_rei_scene1_bg_load_27(void) {
    func_800469F4(0x488A);
    func_80011DFC();
}

void spot_rei_scene2_bg_load_03(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void spot_rei_scene3_bg_load_02(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}
