/* mira_scenes.c: src/ovl/EVENT/800FFBE0.c:4: girl 7 (Mira) scenes 1-6 and 8. Original object 0x800FFBE0-0x801011A0 (config/objects/EVENT.txt). */
#include "common.h"
#include "ovl/EVENT.h"

void spot_mira_still_pointers_1_init(void) {
    spot_mira_still_pointers_1 = 0x801CE218;
    D_80121D24 = 0x801CE21C;
    D_80121D28 = 0x801CE25C;
    D_80121D2C = *(s16 *)0x801CE27C;
    D_80121D30 = 0x801B0000;
    D_80121D34 = 0x801B2000;
    D_80121D38 = 0x801B6000;
    D_80121D3C = 0x801BA000;
    D_80121D40 = 0x801BE000;
    D_80121D44 = 0x801C2000;
    D_80121D48 = 0x801C6000;
}

void spot_mira_still_pointers_2_init(void) {
    spot_mira_still_pointers_2 = 0x801CE124;
    D_80121D50 = 0x801CE128;
    D_80121D54 = 0x801CE148;
    D_80121D58 = *(s16 *)0x801CE15C;
    D_80121D5C = 0x801B0000;
    D_80121D60 = 0x801B2000;
    D_80121D64 = 0x801B6000;
    D_80121D68 = 0x801BA000;
    D_80121D6C = 0x801BE000;
    D_80121D70 = 0x801C2000;
    D_80121D74 = 0x801C6000;
}

void spot_mira_still_pointers_3_init(void) {
    spot_mira_still_pointers_3 = 0x801CE0F4;
    D_80121D7C = 0x801CE0F8;
    D_80121D80 = 0x801CE118;
    D_80121D84 = *(s16 *)0x801CE12C;
    D_80121D88 = 0x801B0000;
    D_80121D8C = 0x801B2000;
    D_80121D90 = 0x801B6000;
    D_80121D94 = 0x801BA000;
    D_80121D98 = 0x801BE000;
    D_80121D9C = 0x801C2000;
    D_80121DA0 = 0x801C6000;
}

void spot_mira_still_pointers_4_init(void) {
    spot_mira_still_pointers_4 = 0x801CE088;
    D_80121DA8 = 0x801CE08C;
    D_80121DAC = 0x801CE0A0;
    D_80121DB0 = *(s16 *)0x801CE0B4;
    D_80121DB4 = 0x801B0000;
    D_80121DB8 = 0x801B2000;
    D_80121DBC = 0x801B6000;
    D_80121DC0 = 0x801BA000;
    D_80121DC4 = 0x801BE000;
    D_80121DC8 = 0x801C2000;
    D_80121DCC = 0x801C6000;
}

void spot_mira_still_pointers_5_init(void) {
    spot_mira_still_pointers_5 = 0x801CE0F4;
    D_80121DD4 = 0x801CE0F8;
    D_80121DD8 = 0x801CE118;
    D_80121DDC = *(s16 *)0x801CE12C;
    D_80121DE0 = 0x801B0000;
    D_80121DE4 = 0x801B2000;
    D_80121DE8 = 0x801B6000;
    D_80121DEC = 0x801BA000;
    D_80121DF0 = 0x801BE000;
    D_80121DF4 = 0x801C2000;
    D_80121DF8 = 0x801C6000;
}

void spot_mira_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_mira_scene1_run();
        break;
    case 2:
        spot_mira_scene2_run();
        break;
    case 3:
        spot_mira_scene3_run();
        break;
    case 4:
        spot_mira_scene4_run();
        break;
    case 5:
        spot_mira_scene5_run();
        break;
    case 6:
        spot_mira_scene6_run();
        break;
    case 8:
        spot_mira_scene8_run();
        break;
    default:
        func_80015FE0();
        break;
    }
}

void spot_mira_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_mira_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_mira_scene1_steps_run(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl38 tbl;

    tbl = spot_mira_scene1_steps;
    func_80078950("s%d %d\n", D_800B1AF6, D_800E9E63);
    tbl.f[D_800B1AF6](0x80);
}

void spot_mira_scene1_assets_load_13(void) {
    func_80015D28(0x3D, 0x801B0000, 0x80A0);
    spot_mira_still_pointers_1_init();
    func_80011DFC();
}

void spot_mira_scene1_label_set_27(void) {
    func_80078970(D_80094764, "男子１");
    D_800EAFB6 = 4;
    func_80011DFC();
}

void spot_mira_scene1_label_set_29(void) {
    func_80078970(D_80094764, "男子２");
    D_800EAFB6 = 5;
    func_80011DFC();
}

void spot_mira_scene1_names_set_31(void) {
    func_8004BC20((s32) D_800B1746);
    D_800EAFB6 = 2;
    func_80011DFC();
}

void spot_mira_scene8_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_mira_scene8_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_mira_scene8_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl21 tbl;

    tbl = spot_mira_scene8_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_mira_scene1_init_00(void) {
    s16 i;

    D_800B1746 = 7;
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
    for (i = 0; i < 5; i++) {
        ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i + 11] = 1;
        ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i + 43] = 1;
        ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i + 21] = 1;
        ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i + 53] = 1;
    }
    func_80045B14();
    spot_script_sections_init();
    func_8003535C();
    func_8003580C();
    func_80037A90();
    func_8004C360();
    D_800F19AB = 0;
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    spot_script_sections_init();
    spot_talk_text_table = D_801204D4;
    spot_talk_command_table = D_801204F0;
    spot_talk_voice_table = D_8012050C;
    func_80078970(D_80094784, "廊下");
    D_800B0B76++;
    func_8004C250();
    func_80011DFC();
}

void spot_mira_scene1_still_show_15(void) {
    spot_mira_still_pointers_1_init();
    func_80012D64(D_80121D30, 0x11, 1, 2, 0);
    func_8004C46C(D_80121D34, D_80121D38, D_80121D3C, D_80121D40, D_80121D44, D_80121D48);
    func_8004C6B0(D_80121D24, D_80121D28, spot_mira_still_pointers_1, (s32) D_80121D2C);
    func_80011DFC();
}

void spot_mira_scene8_init_00(void) {
    D_800B1746 = 7;
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
    D_80094714 = 8;
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
    spot_talk_text_table = D_801204EC;
    spot_talk_command_table = D_80120508;
    spot_talk_voice_table = D_80120524;
    func_80078970(D_80094784, "廊下");
    func_80011DFC();
}

void spot_mira_scene2_still_show_03(void) {
    spot_talk_text_table = D_801204D8;
    spot_talk_command_table = D_801204F4;
    spot_talk_voice_table = D_80120510;
    spot_face_cells = 1;
    D_8012438E = 2;
    D_80124390 = 4;
    spot_reaction_tier_cells = 1;
    D_80124396 = 2;
    D_80124398 = 3;
    spot_reaction_cells = 0;
    D_8012439E = 2;
    D_801243A0 = 4;
    spot_reaction_low_cells = 0;
    D_801243A6 = 2;
    D_801243A8 = 4;
    spot_mira_still_pointers_2_init();
    func_80012D64(D_80121D5C, 0x11, 1, 2, 0);
    func_8004C46C(D_80121D60, D_80121D64, D_80121D68, D_80121D6C, D_80121D70, D_80121D74);
    func_8004C6B0(D_80121D50, D_80121D54, spot_mira_still_pointers_2, (s32) D_80121D58);
    func_80011DFC();
}

void spot_mira_scene3_still_show_stats_03(void) {
    spot_talk_text_table = D_801204DC;
    spot_talk_command_table = D_801204F8;
    spot_talk_voice_table = D_80120514;
    D_800B0A04[7].unk_02 += 1;
    D_800B0A04[7].unk_06 += 1;
    D_800B0A04[7].unk_0A -= 0x14;
    func_8004C250();
    spot_mira_still_pointers_3_init();
    func_80012D64(D_80121D88, 0x11, 1, 2, 0);
    func_8004C46C(D_80121D8C, D_80121D90, D_80121D94, D_80121D98, D_80121D9C, D_80121DA0);
    func_8004C6B0(D_80121D7C, D_80121D80, spot_mira_still_pointers_3, D_80121D84);
    func_80011DFC();
}

void spot_mira_scene4_still_show_stats_03(void) {
    spot_talk_text_table = D_801204E0;
    spot_talk_command_table = D_801204FC;
    spot_talk_voice_table = D_80120518;
    func_80078970(D_800947C4, "遊園地");
    D_800B0A04[7].unk_06 -= 1;
    D_800B0A04[7].unk_0A += 0xA;
    func_8004C250();
    spot_mira_still_pointers_4_init();
    func_80012D64(D_80121DB4, 0x11, 1, 2, 0);
    func_8004C46C(D_80121DB8, D_80121DBC, D_80121DC0, D_80121DC4, D_80121DC8, D_80121DCC);
    func_8004C6B0(D_80121DA8, D_80121DAC, spot_mira_still_pointers_4, D_80121DB0);
    func_80011DFC();
}

void spot_mira_scene5_still_show_stats_05(void) {
    spot_talk_text_table = D_801204E4;
    spot_talk_command_table = D_80120500;
    spot_talk_voice_table = D_8012051C;
    D_800B0A04[7].unk_02 += 3;
    D_800B0A04[7].unk_06 += 2;
    D_800B0A04[7].unk_0A -= 0x14;
    func_8004C250();
    spot_mira_still_pointers_5_init();
    func_80012D64(D_80121DE0, 0x11, 1, 2, 0);
    func_8004C46C(D_80121DE4, D_80121DE8, D_80121DEC, D_80121DF0, D_80121DF4, D_80121DF8);
    func_8004C6B0(D_80121DD4, D_80121DD8, spot_mira_still_pointers_5, D_80121DDC);
    func_80011DFC();
}

void spot_mira_scene6_stat_update_01(void) {
    spot_talk_text_table = D_801204E8;
    spot_talk_command_table = D_80120504;
    spot_talk_voice_table = D_80120520;
    D_800B0A04[7].unk_02 += 3;
    D_800B0A04[7].unk_06 += 2;
    func_8004C250();
    func_80011DFC();
}

void spot_mira_scene1_bg_load_03(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void spot_mira_scene1_bg_load_21(void) {
    func_800469F4(0x4667);
    func_80011DFC();
}

void spot_mira_scene2_bg_load_10(void) {
    func_800469F4(0x47B5);
    func_80011DFC();
}

void spot_mira_scene2_bg_load_21(void) {
    func_800469F4(0x4499);
    func_80011DFC();
}

void spot_mira_scene3_bg_load_10(void) {
    func_800469F4(0x481D);
    func_80011DFC();
}

void spot_mira_scene3_bg_load_18(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void spot_mira_scene4_bg_load_17(void) {
    func_800469F4(0x4770);
    func_80011DFC();
}

void spot_mira_scene4_bg_load_32(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void spot_mira_scene5_bg_load_09(void) {
    func_800469F4(0x47D8);
    func_80011DFC();
}

void spot_mira_scene5_bg_load_25(void) {
    func_800469F4(0x44D1);
    func_80011DFC();
}

void spot_mira_scene8_bg_load_03(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void spot_mira_scene2_run(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl40 tbl;

    tbl = spot_mira_scene2_steps;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void spot_mira_scene2_se_play_16(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void spot_mira_scene2_assets_load_01(void) {
    func_80015D28(0x3D, 0x801B0000, 0x80DD);
    spot_mira_still_pointers_2_init();
    func_80011DFC();
}

void spot_mira_scene2_palette_load_15(void) {
    u16 t;

    /* FAKE: one-line do-while around both statements fixes the as1 load order (lhu before lw); T-8040 */
    do { t = D_80094714; D_80094714 = t + D_800EECBC; } while (0);
    func_80012D64(D_80121D5C, 0x11, 1, 2, D_800EECBC);
    func_80011DFC();
}

void spot_mira_scene3_run(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl38 tbl;

    tbl = spot_mira_scene3_steps;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    tbl.f[D_800B1AF6](0x80);
}

void spot_mira_scene3_assets_load_01(void) {
    func_80015D28(0x3D, 0x801B0000, 0x811A);
    spot_mira_still_pointers_3_init();
    func_80011DFC();
}

void spot_mira_scene4_run(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl51 tbl;

    tbl = spot_mira_scene4_steps;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void spot_mira_scene4_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8157);
    spot_mira_still_pointers_4_init();
    func_80011DFC();
}

void spot_mira_scene5_run(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl42 tbl;

    tbl = *(FnTbl42 *)&D_801220EC;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    tbl.f[D_800B1AF6](0x80);
}

void spot_mira_scene5_se_play_16(void) {
    func_800433D0(0x502);
    func_80011DFC();
}

void spot_mira_unused_assets_load_1(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8194);
    spot_mira_still_pointers_5_init();
    func_80011DFC();
}

void spot_mira_unused_branch_on_mood_2(void) {
    /* the u8 cast reproduces the conversion of the callee's u8 result (andi) that shifts the temporary registers */
    if (((u8)func_8002328C(D_800B1746) & 0x7F) >= 2U) {
        D_80094714 += 3;
    }
    func_80011DFC();
}

void spot_mira_scene6_run(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl15 tbl;

    tbl = spot_mira_scene6_steps;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    tbl.f[D_800B1AF6](0x80);
}
