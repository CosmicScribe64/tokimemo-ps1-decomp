/* nozomi_scenes.c: src/ovl/EVENT/80103B60.c:4: girl 6 (Nozomi) scenes 1-5; also the sprite group shift helper. Original object 0x80103B60-0x801050D0 (config/objects/EVENT.txt). */
#include "common.h"
#include "ovl/EVENT.h"

void spot_sprite_group_shift(s16 dx, s16 dy) {
    s16 i;

    for (i = 0; i < 6; i++) {
        *(s16 *)(D_800EC190 + i * 0x24 + 0x94) += dx;
        *(s16 *)(D_800EC190 + i * 0x24 + 0x96) += dy;
    }
}

void spot_yukari_scene6_assets_load_00(void) {
    func_80015D28(0x35, 0x801B0000, 0x852D);
    spot_yukari_tile_pointers_1_init();
    func_80011DFC();
}

void spot_yukari_scene6_branch_09(void) {
    if (D_800B0A04[5].unk_0C.b14) {
        D_80094714 += 1;
    }
    func_80011DFC();
}

void spot_yukari_scene6_branch_11(void) {
    if (!D_800B0A04[5].unk_0C.b14) {
        D_80094714 += 1;
    }
    func_80011DFC();
}

void spot_nozomi_still_pointers_1_init(void) {
    spot_nozomi_still_pointers_1 = 0x801CE124;
    D_80122B34 = 0x801CE128;
    D_80122B38 = 0x801CE148;
    D_80122B3C = *(s16 *)0x801CE15C;
    D_80122B40 = 0x801B0000;
    D_80122B44 = 0x801B2000;
    D_80122B48 = 0x801B6000;
    D_80122B4C = 0x801BA000;
    D_80122B50 = 0x801BE000;
    D_80122B54 = 0x801C2000;
    D_80122B58 = 0x801C6000;
}

void spot_nozomi_still_pointers_2_init(void) {
    spot_nozomi_still_pointers_2 = 0x801CE0DC;
    D_80122B60 = 0x801CE0E0;
    D_80122B64 = 0x801CE0F8;
    D_80122B68 = *(s16 *)0x801CE10C;
    D_80122B6C = 0x801B0000;
    D_80122B70 = 0x801B2000;
    D_80122B74 = 0x801B6000;
    D_80122B78 = 0x801BA000;
    D_80122B7C = 0x801BE000;
    D_80122B80 = 0x801C2000;
    D_80122B84 = 0x801C6000;
}

void spot_nozomi_still_pointers_3_init(void) {
    spot_nozomi_still_pointers_3 = 0x801D20F8;
    D_80122B8C = 0x801D2100;
    D_80122B90 = 0x801D2120;
    D_80122B94 = *(s16 *)0x801D213C;
    D_80122B98 = 0x801B0000;
    D_80122B9C = 0x801B2000;
    D_80122BA0 = 0x801B6000;
    D_80122BA4 = 0x801BA000;
    D_80122BA8 = 0x801BE000;
    D_80122BAC = 0x801C2000;
    D_80122BB0 = 0x801C6000;
}

void spot_nozomi_still_pointers_4_init(void) {
    spot_nozomi_still_pointers_4 = 0x801D2240;
    D_80122BB8 = 0x801D2248;
    D_80122BBC = 0x801D2288;
    D_80122BC0 = *(s16 *)0x801D22A0;
    D_80122BC4 = 0x801B0000;
    D_80122BC8 = 0x801B2000;
    D_80122BCC = 0x801B6000;
    D_80122BD0 = 0x801BA000;
    D_80122BD4 = 0x801BE000;
    D_80122BD8 = 0x801C2000;
    D_80122BDC = 0x801C6000;
}

void spot_nozomi_still_pointers_5_init(void) {
    spot_nozomi_still_pointers_5 = 0x801CE080;
    D_80122BE4 = 0x801CE084;
    D_80122BE8 = 0x801CE094;
    D_80122BEC = *(s16 *)0x801CE0A8;
    D_80122BF0 = 0x801B0000;
    D_80122BF4 = 0x801B2000;
    D_80122BF8 = 0x801B6000;
    D_80122BFC = 0x801BA000;
    D_80122C00 = 0x801BE000;
    D_80122C04 = 0x801C2000;
    D_80122C08 = 0x801C6000;
}

void spot_nozomi_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_nozomi_scene1_run();
        return;
    case 2:
        spot_nozomi_scene2_run();
        return;
    case 3:
        spot_nozomi_scene3_run();
        return;
    case 4:
        spot_nozomi_scene4_run();
        return;
    case 5:
        spot_nozomi_scene5_run();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void spot_nozomi_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_nozomi_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_nozomi_scene1_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotNozomiAndStadiumStepTable tbl;

    tbl = spot_nozomi_scene1_steps;
    idx = D_800B1AF6;
    func_80078950("s%d %d\n", idx, D_800E9E63);
    tbl.f[D_800B1AF6]();
}

void spot_nozomi_scene1_assets_load_07(void) {
    func_80015D28(0x3D, 0x801B0000, 0x85A7);
    spot_nozomi_still_pointers_1_init();
    func_80011DFC();
}

void spot_nozomi_scene1_init_00(void) {
    D_800B1746 = 6;
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
    spot_talk_text_table = D_80120498;
    spot_talk_command_table = D_801204AC;
    spot_talk_voice_table = D_801204C0;
    func_80078970(D_80094784, "廊下");
    D_800B0A04[6].unk_02 += 1;
    D_800B0A04[6].unk_06 += 2;
    func_8004C250();
    func_80011DFC();
}

void spot_nozomi_scene1_still_show_09(void) {
    spot_nozomi_still_pointers_1_init();
    func_80012D64(D_80122B40, 0x11, 1, 2, 0);
    func_8004C46C(D_80122B44, D_80122B48, D_80122B4C, D_80122B50, D_80122B54, D_80122B58);
    func_8004C6B0(D_80122B34, D_80122B38, spot_nozomi_still_pointers_1, (s32) D_80122B3C);
    func_80011DFC();
}

void spot_nozomi_scene2_still_show_stats_01(void) {
    spot_talk_text_table = D_8012049C;
    spot_talk_command_table = D_801204B0;
    spot_talk_voice_table = D_801204C4;
    spot_nozomi_still_pointers_2_init();
    func_80012D64(D_80122B6C, 0x11, 1, 2, 0);
    func_8004C46C(D_80122B70, D_80122B74, D_80122B78, D_80122B7C, D_80122B80, D_80122B84);
    func_8004C6B0(D_80122B60, D_80122B64, spot_nozomi_still_pointers_2, D_80122B68);
    D_800B0A04[6].unk_02 += 1;
    D_800B0A04[6].unk_06 += 1;
    D_800B0A04[6].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_nozomi_scene3_still_show_stats_03(void) {
    spot_talk_text_table = D_801204A0;
    spot_talk_command_table = D_801204B4;
    spot_talk_voice_table = D_801204C8;
    spot_nozomi_still_pointers_3_init();
    func_80012D64(D_80122B98, 0x11, 1, 2, 0);
    func_8004C46C(D_80122B9C, D_80122BA0, D_80122BA4, D_80122BA8, D_80122BAC, D_80122BB0);
    func_8004C6B0(D_80122B8C, D_80122B90, spot_nozomi_still_pointers_3, D_80122B94);
    D_800B0A04[6].unk_02 += 3;
    D_800B0A04[6].unk_06 += 2;
    D_800B0A04[6].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_nozomi_scene4_still_show_stats_03(void) {
    spot_talk_text_table = D_801204A4;
    spot_talk_command_table = D_801204B8;
    spot_talk_voice_table = D_801204CC;
    spot_nozomi_still_pointers_4_init();
    func_80012D64(D_80122BC4, 0x11, 1, 2, 0);
    func_8004C46C(D_80122BC8, D_80122BCC, D_80122BD0, D_80122BD4, D_80122BD8, D_80122BDC);
    func_8004C6B0(D_80122BB8, D_80122BBC, spot_nozomi_still_pointers_4, D_80122BC0);
    D_800B0A04[6].unk_02 += 2;
    D_800B0A04[6].unk_06 += 2;
    D_800B0A04[6].unk_0A -= 0xA;
    func_8004C250();
    func_80011DFC();
}

void spot_nozomi_scene5_still_show_stats_03(void) {
    spot_talk_text_table = D_801204A8;
    spot_talk_command_table = D_801204BC;
    spot_talk_voice_table = D_801204D0;
    if (((u8)func_8002328C(6) & 0x7F) < 2U) {
        D_800B0A04[6].unk_02 += 3;
        D_800B0A04[6].unk_06 += 2;
    } else {
        D_800B0A04[6].unk_02 += 1;
        D_800B0A04[6].unk_06 += 3;
    }
    D_800B0A04[6].unk_0A -= 0x14;
    func_8004C250();
    spot_nozomi_still_pointers_5_init();
    func_80012D64(D_80122BF0, 0x11, 1, 2, 0);
    func_8004C46C(D_80122BF4, D_80122BF8, D_80122BFC, D_80122C00, D_80122C04, D_80122C08);
    func_8004C6B0(D_80122BE4, D_80122BE8, spot_nozomi_still_pointers_5, D_80122BEC);
    func_80011DFC();
}

void spot_nozomi_scene1_bg_load_03(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void spot_nozomi_scene2_bg_load_11(void) {
    func_800469F4(0x47A7);
    func_80011DFC();
}

void spot_nozomi_scene2_bg_load_20(void) {
    func_800469F4(0x446A);
    func_80011DFC();
}

void spot_nozomi_scene3_bg_load_11(void) {
    func_800469F4(0x4814);
    func_80011DFC();
}

void spot_nozomi_scene3_bg_load_35(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void spot_nozomi_scene4_bg_load_11(void) {
    func_800469F4(0x483E);
    func_80011DFC();
}

void spot_nozomi_scene4_bg_load_21(void) {
    func_800469F4(0x454C);
    func_80011DFC();
}

void spot_nozomi_scene5_bg_load_12(void) {
    func_800469F4(0x4876);
    func_80011DFC();
}

void spot_nozomi_scene5_bg_load_25(void) {
    func_800469F4(0x45A0);
    func_80011DFC();
}

void spot_nozomi_scene2_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotNozomiScene2StepTable tbl;

    tbl = spot_nozomi_scene2_steps;
    idx = D_800B1AF6;
    func_80078950("%d\n", idx);
    tbl.f[D_800B1AF6](0x80);
}

void spot_nozomi_scene2_se_play_09(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void spot_nozomi_scene2_assets_load_05(void) {
    func_80015D28(0x3D, 0x801B0000, 0x85E4);
    spot_nozomi_still_pointers_2_init();
    func_80011DFC();
}

void spot_nozomi_scene2_step_35(void) {
    if (func_8002328C(D_800B1746) < 2U) {
        func_8004A8EC(0);
        return;
    }
    func_8004A8EC(func_8002336C(D_800B1746));
}

void spot_nozomi_scene2_branch_on_mood_07(void) {
    u8 s;

    s = func_8002328C(D_800B1746) & 0x7F;
    if (s >= 2U) {
        D_80094714 += 5;
    }
    func_80011DFC();
}

void spot_nozomi_scene3_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotNozomiSakiAndYumiSceneStepTable tbl;

    tbl = spot_nozomi_scene3_steps;
    idx = D_800B1AF6;
    func_80078950("%d\n", idx);
    tbl.f[D_800B1AF6](0x80);
}

void spot_nozomi_scene3_assets_load_00(void) {
    func_80015D28(0x45, 0x801B0000, 0x8621);
    spot_nozomi_still_pointers_3_init();
    func_80011DFC();
}

void spot_nozomi_scene3_se_play_15(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void spot_nozomi_scene3_set_state_18(void) {
    /* Port of the DATE clone func_801572E4 (T-17400). */
    s16 i;

    ((Actor *)D_800EAFA0)[2] = ((Actor *)D_800EAFA0)[0];
    D_800EB03E = 4;
    D_800EB02A = 9;
    for (i = 0; i < 6; i++) {
        ((SpriteQuad *)D_800EC190)[i + 4].flags |= 0x80000000;
    }
    ((Actor *)D_800EAFA0)->draw_flags &= 0xFF7F;
    ((Actor *)D_800EAFA0)[1].draw_flags &= 0xFF7F;
    D_800EECD4 = 0;
    D_800B0860[6] = D_800B0860[7] = D_800B0860[8] = 0x7F;
    func_80011DFC();
}

s32 spot_nozomi_scene3_branch_19(void) {
    D_800B0860[6] += 0x10;
    D_800B0860[7] += 0x10;
    D_800B0860[8] += 0x10;
    if (D_800B0860[6] >= 0xF0U) {
        func_80011DFC();
    }
}

void spot_nozomi_scene3_set_state_20(void) {
    s16 i;

    D_800B0860[7] = 0;
    D_800B0860[6] = 0;
    /* FAKE: same-line loop setup preserves the store scheduling. T-10850 */
    D_800B0860[8] = 0; for (i = 0; i < 6; i++) {
        ((SpriteQuad *)D_800EC190)[i + 4].flags &= 0x7FFFFFFF;
    }
    ((Actor *)D_800EAFA0)->draw_flags |= 0x80;
    ((Actor *)D_800EAFA0)[1].draw_flags |= 0x80;
    D_800EECD4 = 1;
    func_80011DFC();
}

void spot_nozomi_scene4_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotYukoYuinaReiNozomiYumiAndConcertHallStepTable tbl;

    tbl = spot_nozomi_scene4_steps;
    idx = D_800B1AF6;
    func_80078950("%d\n", idx);
    tbl.f[D_800B1AF6](0x80);
}

void spot_nozomi_scene4_assets_load_00(void) {
    func_80015D28(0x45, 0x801B0000, 0x8666);
    spot_nozomi_still_pointers_4_init();
    func_80011DFC();
}

void spot_nozomi_scene4_branch_on_mood_07(void) {
    /* the u8 cast reproduces the conversion of the callee's u8 result (andi) that shifts the temporary registers */
    if (((u8)func_8002328C(D_800B1746) & 0x7F) >= 2U) {
        D_80094714 += 3;
    }
    func_80011DFC();
}

void spot_nozomi_scene5_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotMiraYukariNozomiAndSakiSceneStepTable tbl;

    tbl = spot_nozomi_scene5_steps;
    idx = D_800B1AF6;
    func_80078950("%d %d %d\n", idx, D_80094714, D_80094718);
    tbl.f[D_800B1AF6](0x80);
}

void spot_shared_assets_load_1(void) {
    func_80015D28(0x3D, 0x801B0000, 0x86AB);
    spot_nozomi_still_pointers_5_init();
    func_80011DFC();
}

void spot_nozomi_scene5_se_play_11(void) {
    func_800433D0(0x501);
    func_80011DFC();
}

void spot_nozomi_scene5_branch_on_mood_07(void) {
    u8 temp;
    temp = (u32) (func_8002328C(D_800B1746) & 0x7F);
    if (temp >= 2U) {
        D_80094714 += 3;
    }
    func_80011DFC();
}
