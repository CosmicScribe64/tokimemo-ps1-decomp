/* megumi_scenes.c: src/ovl/EVENT/801050D0.c:4: girl 9 (Megumi) scenes 1-6. Original object 0x801050D0-0x80106C20 (config/objects/EVENT.txt). */
#include "common.h"
#include "ovl/EVENT.h"

void spot_megumi_overlay_pointers_1_init(void) {
    spot_megumi_overlay_pointers_1 = 0x801D22F4;
    D_80122F74 = 0x801D22F8;
    D_80122F78 = 0x801D2318;
    D_80122F7C = *(s16 *)0x801D232C;
    D_80122F80 = 0x801B0000;
    D_80120C04 = 0x801D2000;
    D_80122F84 = 0x801B2000;
    D_80122F88 = 0x801B6000;
    D_80122F8C = 0x801BA000;
    D_80122F90 = 0x801BE000;
    D_80122F94 = 0x801C2000;
    D_80122F98 = 0x801C6000;
    D_80120C20 = 0x801CE000;
}

void spot_megumi_still_pointers_1_init(void) {
    spot_megumi_still_pointers_1 = 0x801CE090;
    D_80122FA0 = 0x801CE094;
    D_80122FA4 = 0x801CE0AC;
    D_80122FA8 = *(s16 *)0x801CE0C0;
    D_80122FAC = 0x801B0000;
    D_80122FB0 = 0x801B2000;
    D_80122FB4 = 0x801B6000;
    D_80122FB8 = 0x801BA000;
    D_80122FBC = 0x801BE000;
    D_80122FC0 = 0x801C2000;
    D_80122FC4 = 0x801C6000;
}

void spot_megumi_still_pointers_2_init(void) {
    spot_megumi_still_pointers_2 = 0x801D2240;
    D_80122FCC = 0x801D2248;
    D_80122FD0 = 0x801D2288;
    D_80122FD4 = *(s16 *)0x801D22A0;
    D_80122FD8 = 0x801B0000;
    D_80122FDC = 0x801B2000;
    D_80122FE0 = 0x801B6000;
    D_80122FE4 = 0x801BA000;
    D_80122FE8 = 0x801BE000;
    D_80122FEC = 0x801C2000;
    D_80122FF0 = 0x801C6000;
}

void spot_megumi_still_pointers_3_init(void) {
    spot_megumi_still_pointers_3 = 0x801CE078;
    D_80122FF8 = 0x801CE07C;
    D_80122FFC = 0x801CE08C;
    D_80123000 = *(s16 *)0x801CE098;
    D_80123004 = 0x801B0000;
    D_80123008 = 0x801B2000;
    D_8012300C = 0x801B6000;
    D_80123010 = 0x801BA000;
    D_80123014 = 0x801BE000;
    D_80123018 = 0x801C2000;
    D_8012301C = 0x801C6000;
}

void spot_megumi_still_pointers_4_init(void) {
    spot_megumi_still_pointers_4 = 0x801CE0F4;
    D_80123024 = 0x801CE0F8;
    D_80123028 = 0x801CE118;
    D_8012302C = *(s16 *)0x801CE12C;
    D_80123030 = 0x801B0000;
    D_80123034 = 0x801B2000;
    D_80123038 = 0x801B6000;
    D_8012303C = 0x801BA000;
    D_80123040 = 0x801BE000;
    D_80123044 = 0x801C2000;
    D_80123048 = 0x801C6000;
}

void spot_megumi_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_megumi_scene1_run();
        return;
    case 2:
        spot_megumi_scene2_run();
        return;
    case 3:
        spot_megumi_scene3_run();
        return;
    case 4:
        spot_megumi_scene4_run();
        return;
    case 5:
        spot_megumi_scene5_run();
        return;
    case 6:
        spot_megumi_scene6_run();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void spot_megumi_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_megumi_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_megumi_scene2_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_megumi_scene2_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_megumi_scene1_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotShioriAyakoMegumiAndMovieTheaterStepTable tbl;

    tbl = spot_megumi_scene1_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    spot_megumi_scene1_sprites_update();
}

void spot_megumi_scene1_step_33(void) {
    s16 i;
    SpriteQuad *p;

    D_800B1C1C = 1;
    func_800137AC(0x140, 0, 0x40, 0x80, D_80120C20);
    spot_megumi_overlay_pointers_1_init();
    func_80012D64(D_80120C04, 0x12, 1, 3, 0);
    for (i = 4; i < 0xE; i++) {
        p = (SpriteQuad *)(D_800EC190 + i * 0x24);
        p->flags = 0xC1000000;
        p->x = func_800789A0() % 84 - 0x9F;
        p->y = func_800789A0() % 130 - 0x5F;
        p->w = 8;
        p->h = 0x18;
        p->unk_0C = 5;
        p->unk_0E = 0;
        p->unk_0F = 0;
        p->unk_10 = 0;
        p->unk_12 = 0x1F2;
        p->tint_r = 0;
        p->tint_g = 0;
        p->tint_b = 0;
        p->anchor_x = 0;
        p->anchor_y = 0;
        p->unk_1C = 0x1000;
        p->unk_1E = 0x1000;
        p->unk_20 = 0;
        D_800B188D[i] = 0x8C;
        ((s16 *)&D_80120CE8)[i] = 0;
        D_80120D08[i] = 0;
    }
    func_80011DFC();
}

void spot_megumi_scene1_assets_load_01(void) {
    func_80015D28(0x45, 0x801B0000, 0x86E8);
    spot_megumi_overlay_pointers_1_init();
    func_80011DFC();
}

void spot_megumi_scene1_still_show_11(void) {
    spot_megumi_overlay_pointers_1_init();
    func_80012D64(D_80122F80, 0x11, 1, 2, 0);
    func_8004C46C(D_80122F84, D_80122F88, D_80122F8C, D_80122F90, D_80122F94, D_80122F98);
    func_8004C6B0(D_80122F74, D_80122F78, spot_megumi_overlay_pointers_1, (s32) D_80122F7C);
    func_80011DFC();
}

void spot_megumi_scene2_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotMegumiScene2StepTable tbl;

    tbl = spot_megumi_scene2_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_megumi_scene1_branch_22(void) {
    if (D_800B0A04[D_800B1746].unk_06 < 0x50) {
        D_80094714 += 2;
    }
    func_80011DFC();
}

void spot_megumi_scene1_branch_25(void) {
    if (D_800B0A04[D_800B1746].unk_06 < 0x50) {
        D_800B1AF6 += 0xE;
    }
    func_80011DFC();
}

void spot_megumi_scene1_branch_39(void) {
    if (D_800B0A04[D_800B1746].unk_06 >= 0x50) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void spot_megumi_unused_set_state_1(void) {
    D_80094714 = (u16) D_80094714 + D_800EECBC;
    func_80011DFC();
}

void spot_megumi_scene1_still_fade_out_37(void) {
    s16 i;
    SpriteQuad *records;
    s32 before;
    s32 after;

    before = D_800B1AF6;
    func_8004C984();
    records = (SpriteQuad *)D_800EC190;
    for (i = 4; i < 19; i++) {
        if (D_800F19AB < 112) {
            records[i].tint_r = records[i].tint_g = records[i].tint_b = D_800F19AB;
        }
    }
    after = D_800B1AF6;
    if (before != after) {
        for (i = 4; i < 19; i++) {
            records[i].flags |= 0x80000000;
        }
    }
}

void spot_megumi_scene1_still_fade_in_35(void) {
    s16 i;
    SpriteQuad *records;

    if (D_800B1AF0 == 0) {
        records = (SpriteQuad *)D_800EC190;
        for (i = 4; i < 19; i++) {
            records[i].flags &= 0x7FFFFFFF;
        }
    }
    func_8004C90C();
    records = (SpriteQuad *)D_800EC190;
    for (i = 4; i < 19; i++) {
        if (D_800F19AB < 113) {
            records[i].tint_r = records[i].tint_g = records[i].tint_b = D_800F19AB;
        }
    }
}

void spot_megumi_scene1_init_00(void) {
    D_800B1746 = 9;
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
    spot_talk_text_table = D_80120570;
    spot_talk_command_table = D_8012058C;
    spot_talk_voice_table = D_801205A8;
    func_80078970(D_80094784, "廊下");
    if (D_800EECB0 == 1) {
        if (D_800B0A04[9].unk_06 >= 0x50) {
            D_800B0A04[9].unk_02 += 1;
        } else {
            D_800B0A04[9].unk_06 += 1;
        }
        func_8004C250();
    }
    func_80011DFC();
}

void spot_megumi_scene1_sprites_update(void) {
    s16 i;

    for (i = 4; i < 0xE; i++) {
        ((SpriteQuad *)D_800EC190)[i].y += 0xA;
        if (((func_800789A0() & 3) == 0 &&
             ((((SpriteQuad *)D_800EC190)[i].y >= -0x52 && ((SpriteQuad *)D_800EC190)[i].y < -0x39 && ((SpriteQuad *)D_800EC190)[i].x < -0x57) ||
              (((SpriteQuad *)D_800EC190)[i].y >= -0x39 && ((SpriteQuad *)D_800EC190)[i].y < -0x1A && ((SpriteQuad *)D_800EC190)[i].x < -0x77) ||
              (((SpriteQuad *)D_800EC190)[i].y >= -0xB && ((SpriteQuad *)D_800EC190)[i].y < 3 && ((SpriteQuad *)D_800EC190)[i].x >= -0x8D && ((SpriteQuad *)D_800EC190)[i].x < -0x6F) ||
              (((SpriteQuad *)D_800EC190)[i].y >= 5 && ((SpriteQuad *)D_800EC190)[i].x >= -0x7F && ((SpriteQuad *)D_800EC190)[i].x < -0x5C)) &&
             D_80120CF0[i - 4] == 0 && D_80120D10[i - 4] == 0) ||
            D_80120CF0[i - 4] != 0 || D_80120D10[i - 4] != 0) {
            ((SpriteQuad *)D_800EC190)[i].y -= 0xA;
            if (D_80120CF0[i - 4] == 0) {
                if (D_80120D10[i - 4] == 0) {
                    if (((SpriteQuad *)D_800EC190)[i].y < -0x44) {
                        ((SpriteQuad *)D_800EC190)[i].unk_0F += 8;
                    }
                    ((SpriteQuad *)D_800EC190)[i].x -= 0xC;
                    ((SpriteQuad *)D_800EC190)[i].y += 0x10;
                    ((SpotSpriteQuad *)D_800EC190)[i].unk_0E = 8;
                    ((SpriteQuad *)D_800EC190)[i].w = 0x18;
                    ((SpotSpriteQuad *)D_800EC190)[i].unk_0A = 8;
                    D_80120D10[i - 4] += 1;
                } else {
                    D_80120D10[i - 4] = 0;
                    D_80120CF0[i - 4] += 1;
                }
            } else if (D_80120CF0[i - 4] == 4) {
                ((SpriteQuad *)D_800EC190)[i].unk_0E = 0;
                ((SpriteQuad *)D_800EC190)[i].unk_0F = 0;
                ((SpriteQuad *)D_800EC190)[i].w = 8;
                ((SpotSpriteQuad *)D_800EC190)[i].unk_0A = 0x18;
                /* FAKE: same-line stores place mfhi after the zero stores as in the original. T-24085 */
                ((SpriteQuad *)D_800EC190)[i].x = func_800789A0() % 84 - 0x9F; ((SpriteQuad *)D_800EC190)[i].y = -0x78; D_80120D10[i - 4] = 0; D_80120CF0[i - 4] = 0;
            } else if (D_80120CF0[i - 4] == 5) {
                if (D_80120D10[i - 4] == 0) {
                    ((SpriteQuad *)D_800EC190)[i].unk_0E -= 0x18;
                    D_80120D10[i - 4] += 1;
                } else {
                    D_80120D10[i - 4] = 0;
                    D_80120CF0[i - 4] += 1;
                }
            } else {
                if (D_80120D10[i - 4] == 0) {
                    ((SpriteQuad *)D_800EC190)[i].unk_0E += 0x18;
                    D_80120D10[i - 4] += 1;
                } else {
                    D_80120D10[i - 4] = 0;
                    D_80120CF0[i - 4] += 1;
                }
            }
        } else if (((SpriteQuad *)D_800EC190)[i].y >= 0x19 && D_80120CF0[i - 4] == 0 && D_80120D10[i - 4] == 0) {
            /* FAKE: same-line store places mfhi after unk_06 = -0x78 as in the original. T-24085 */
            ((SpriteQuad *)D_800EC190)[i].x = func_800789A0() % 84 - 0x9F; ((SpriteQuad *)D_800EC190)[i].y = -0x78;
        }
    }
}

void spot_megumi_scene2_init_00(void) {
    D_800B1746 = 9;
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
    spot_talk_text_table = D_80120574;
    spot_talk_command_table = D_80120590;
    spot_talk_voice_table = D_801205AC;
    func_80011DFC();
}

void spot_megumi_scene3_still_show_stats_05(void) {
    spot_talk_text_table = D_80120578;
    spot_talk_command_table = D_80120594;
    spot_talk_voice_table = D_801205B0;
    func_80078970(D_80094784, "遊園地");
    D_800B0A04[9].unk_02 += 1;
    D_800B0A04[9].unk_06 += 1;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    spot_megumi_still_pointers_1_init();
    func_80012D64(D_80122FAC, 0x11, 1, 2, 0);
    func_8004C46C(D_80122FB0, D_80122FB4, D_80122FB8, D_80122FBC, D_80122FC0, D_80122FC4);
    func_8004C6B0(D_80122FA0, D_80122FA4, spot_megumi_still_pointers_1, D_80122FA8);
    func_80011DFC();
}

void spot_megumi_scene4_still_show_stats_05(void) {
    spot_talk_text_table = D_8012057C;
    spot_talk_command_table = D_80120598;
    spot_talk_voice_table = D_801205B4;
    D_800B0A04[9].unk_02 += 2;
    D_800B0A04[9].unk_06 += 1;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    spot_megumi_still_pointers_2_init();
    func_80012D64(D_80122FD8, 0x11, 1, 2, 0);
    func_8004C46C(D_80122FDC, D_80122FE0, D_80122FE4, D_80122FE8, D_80122FEC, D_80122FF0);
    func_8004C6B0(D_80122FCC, D_80122FD0, spot_megumi_still_pointers_2, D_80122FD4);
    func_80011DFC();
}

void spot_megumi_scene5_still_show_stats_03(void) {
    spot_talk_text_table = D_80120580;
    spot_talk_command_table = D_8012059C;
    spot_talk_voice_table = D_801205B8;
    D_800B0A04[9].unk_02 += 1;
    D_800B0A04[9].unk_06 += 1;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    spot_megumi_still_pointers_3_init();
    func_80012D64(D_80123004, 0x11, 1, 2, 0);
    func_8004C46C(D_80123008, D_8012300C, D_80123010, D_80123014, D_80123018, D_8012301C);
    func_8004C6B0(D_80122FF8, D_80122FFC, spot_megumi_still_pointers_3, D_80123000);
    func_80011DFC();
}

void spot_megumi_scene6_still_show_stats_03(void) {
    spot_talk_text_table = D_80120584;
    spot_talk_command_table = D_801205A0;
    spot_talk_voice_table = D_801205BC;
    D_800B0A04[9].unk_02 += 3;
    D_800B0A04[9].unk_06 += 2;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    spot_megumi_still_pointers_4_init();
    func_80012D64(D_80123030, 0x11, 1, 2, 0);
    func_8004C46C(D_80123034, D_80123038, D_8012303C, D_80123040, D_80123044, D_80123048);
    func_8004C6B0(D_80123024, D_80123028, spot_megumi_still_pointers_4, D_8012302C);
    func_80011DFC();
}

void spot_megumi_scene1_bg_load_07(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void spot_megumi_scene1_bg_load_27(void) {
    func_800469F4(0x4622);
    func_80011DFC();
}

void spot_megumi_scene2_bg_load_03(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void spot_megumi_scene3_bg_load_09(void) {
    func_800469F4(0x475E);
    func_80011DFC();
}

void spot_megumi_scene3_bg_load_30(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void spot_megumi_scene4_bg_load_15(void) {
    func_800469F4(0x4737);
    func_80011DFC();
}

void spot_megumi_scene5_bg_load_07(void) {
    func_800469F4(0x47F3);
    func_80011DFC();
}

void spot_megumi_scene5_bg_load_21(void) {
    func_800469F4(0x44F6);
    func_80011DFC();
}

void spot_megumi_scene6_bg_load_18(void) {
    func_800469F4(0x44E5);
    func_80011DFC();
}

void spot_megumi_scene3_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotYukoMegumiYumiAndSeaStepTable tbl;

    tbl = spot_megumi_scene3_steps;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_megumi_scene3_assets_load_02(void) {
    func_80015D28(0x3D, 0x801B0000, 0x872D);
    spot_megumi_still_pointers_1_init();
    func_80011DFC();
}

void spot_megumi_scene4_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotMegumiScene4StepTable tbl;

    tbl = spot_megumi_scene4_steps;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_megumi_scene4_assets_load_02(void) {
    func_80015D28(0x45, 0x801B0000, 0x876A);
    spot_megumi_still_pointers_2_init();
    func_80011DFC();
}

void spot_megumi_scene4_se_play_25(void) {
    func_800433D0(0x501);
    func_80011DFC();
}

void spot_megumi_scene4_se_play_27(void) {
    func_800433D0(0x503);
    func_80011DFC();
}

void spot_megumi_scene4_se_play_07(void) {
    func_800433D0(0x504);
    func_80011DFC();
}

void spot_megumi_scene4_se_play_21(void) {
    func_800433D0(0x505);
    func_80011DFC();
}

void spot_megumi_scene4_se_play_23(void) {
    func_800433D0(0x506);
    func_80011DFC();
}

void spot_megumi_scene5_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotMegumiPlanetariumAndLibraryStepTable tbl;

    tbl = spot_megumi_scene5_steps;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_megumi_scene5_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x87AF);
    spot_megumi_still_pointers_3_init();
    func_80011DFC();
}

void spot_megumi_scene6_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotMegumiPlanetariumAndLibraryStepTable tbl;

    tbl = spot_megumi_scene6_steps;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_megumi_scene6_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x87EC);
    spot_megumi_still_pointers_4_init();
    func_80011DFC();
}
