/* shiori_scenes.c: src/ovl/EVENT/800F9680.c:5: girl 0 (Shiori) scenes 1-6 and 8; also holds the shared script loader and the gradient quad helper. Original object 0x800F9680-0x800FBBF0 (config/objects/EVENT.txt). */
#include "common.h"
#include "psx_types.h"
#include "ovl/EVENT.h"

void spot_yuko_scene6_bgm_start_09(void) {
    func_800433D0(0x200);
    func_80011DFC();
}

void spot_yuko_scene6_assets_load_00(void) {
    func_80015D28(0x45, 0x801B0000, 0x7B5F);
    spot_yuko_still_pointers_6_init();
    func_80011DFC();
}

void spot_yuko_scene6_special_case_18(void) {
    spot_talk_step();
    if ((u16) D_80094718 == 4) {
        func_8004CA18();
        D_800B1AF6 -= 1;
    }
}

/* FAKE: explicit colour masks and one line per trio retain the matched DATE schedule (T-20600, T-24034). */
void spot_gradient_quad_draw(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11) {
    SpotDevelopmentState *g;
    POLY_G4 *p;

    g = (SpotDevelopmentState *)D_800B0860;
    p = (POLY_G4 *)func_80019D00(0x24, D_800E9600);
    func_80067678(p);
    func_800675A8(p, arg11);
    p->r0 = (((u32)arg8 >> 16) & 0xFF); p->g0 = (((u32)arg8 >> 8) & 0xFF); p->b0 = ((u32)arg8 & 0xFF);
    p->r1 = (((u32)arg8 >> 16) & 0xFF); p->g1 = (((u32)arg8 >> 8) & 0xFF); p->b1 = ((u32)arg8 & 0xFF);
    p->r2 = (((u32)arg9 >> 16) & 0xFF); p->g2 = (((u32)arg9 >> 8) & 0xFF); p->b2 = ((u32)arg9 & 0xFF);
    p->r3 = (((u32)arg9 >> 16) & 0xFF); p->g3 = (((u32)arg9 >> 8) & 0xFF); p->b3 = ((u32)arg9 & 0xFF);
    p->x0 = g->screen_center_x[D_800E9600] + arg0;
    p->y0 = g->screen_center_y[D_800E9600] + arg1;
    p->x1 = g->screen_center_x[D_800E9600] + arg2;
    p->y1 = g->screen_center_y[D_800E9600] + arg3;
    p->x2 = g->screen_center_x[D_800E9600] + arg4;
    p->y2 = g->screen_center_y[D_800E9600] + arg5;
    p->x3 = g->screen_center_x[D_800E9600] + arg6;
    p->y3 = g->screen_center_y[D_800E9600] + arg7;
    func_800674F4(&D_800B2580[(D_800E9600 << 6) + (arg10 * 4)], (s32)p);
}

void spot_shiori_overlay_pointers_1_init(void) {
    spot_shiori_overlay_pointers_1 = 0x801D638C;
    D_80120BF4 = 0x801D6394;
    D_80120BF8 = 0x801D63B4;
    D_80120BFC = *(s16 *)0x801D63C8;
    D_80120C00 = 0x801B0000;
    D_80120C04 = 0x801D6000;
    D_80120C08 = 0x801B2000;
    D_80120C0C = 0x801B6000;
    D_80120C10 = 0x801BA000;
    D_80120C14 = 0x801BE000;
    D_80120C18 = 0x801C2000;
    D_80120C1C = 0x801C6000;
    D_80120C20 = 0x801D2000;
}

void spot_shiori_still_pointers_1_init(void) {
    spot_shiori_still_pointers_1 = 0x801D21D4;
    D_80120C28 = 0x801D21DC;
    D_80120C2C = 0x801D21FC;
    D_80120C30 = *(s16 *)0x801D2210;
    D_80120C34 = 0x801B0000;
    D_80120C38 = 0x801B2000;
    D_80120C3C = 0x801B6000;
    D_80120C40 = 0x801BA000;
    D_80120C44 = 0x801BE000;
    D_80120C48 = 0x801C2000;
    D_80120C4C = 0x801C6000;
}

void spot_shiori_still_pointers_2_init(void) {
    spot_shiori_still_pointers_2 = 0x801D21D0;
    D_80120C54 = 0x801D21D8;
    D_80120C58 = 0x801D2218;
    D_80120C5C = *(s16 *)0x801D2230;
    D_80120C60 = 0x801B0000;
    D_80120C64 = 0x801B2000;
    D_80120C68 = 0x801B6000;
    D_80120C6C = 0x801BA000;
    D_80120C70 = 0x801BE000;
    D_80120C74 = 0x801C2000;
    D_80120C78 = 0x801C6000;
}

void spot_shiori_still_pointers_3_init(void) {
    spot_shiori_still_pointers_3 = 0x801D232C;
    D_80120C80 = 0x801D2334;
    D_80120C84 = 0x801D2374;
    D_80120C88 = *(s16 *)0x801D238C;
    D_80120C8C = 0x801B0000;
    D_80120C90 = 0x801B2000;
    D_80120C94 = 0x801B6000;
    D_80120C98 = 0x801BA000;
    D_80120C9C = 0x801BE000;
    D_80120CA0 = 0x801C2000;
    D_80120CA4 = 0x801C6000;
}

void spot_shiori_tile_pointers_1_init(void) {
    spot_shiori_tile_pointers_1 = 0x801B0000;
    D_80120CAC = 0x801B2000;
    D_80120CB0 = 0x801B6000;
    D_80120CB4 = 0x801BA000;
    D_80120CB8 = 0x801BE000;
    D_80120CBC = 0x801C2000;
    D_80120CC0 = 0x801C6000;
}

void spot_shiori_still_pointers_4_init(void) {
    spot_shiori_still_pointers_4 = 0x801D22D0;
    D_80120CC8 = 0x801D22D8;
    D_80120CCC = 0x801D233C;
    D_80120CD0 = *(s16 *)0x801D2358;
    D_80120CD4 = 0x801B0000;
    D_80120CD8 = 0x801B2000;
    D_80120CDC = 0x801B6000;
    D_80120CE0 = 0x801BA000;
    D_80120CE4 = 0x801BE000;
    D_80120CE8 = 0x801C2000;
    D_80120CEC = 0x801C6000;
}

void spot_shiori_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_shiori_scene1_run();
        return;
    case 2:
        spot_shiori_scene2_run();
        return;
    case 3:
        spot_shiori_scene3_run();
        return;
    case 4:
        spot_shiori_scene4_run();
        return;
    case 5:
        spot_shiori_scene5_run();
        return;
    case 6:
        spot_shiori_scene6_run();
        return;
    case 8:
        spot_shiori_scene8_run();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void spot_shiori_scene1_run(void) {
    if (D_800B1AF5 == SPOT_GIRL_STAGE_RUN) {
        spot_shiori_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_shiori_scene1_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotShioriYuinaMioAndGameCenterStepTable tbl;

    tbl = spot_shiori_scene1_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    spot_shiori_scene1_sprites_update();
}

void spot_se_pair_step(void) {
    func_800433D0(0x505);
    func_800433D0(0x506);
    func_80011DFC();
}

void spot_shiori_scene1_vram_load_31(void) {
    func_800137AC(0x140, 0, 0x40, 0x80, D_80120C20);
    D_800B1C1C = 1;
    func_80011DFC();
}

void spot_shiori_scene1_assets_load_03(void) {
    func_80015D28(0x4D, 0x801B0000, 0x7BA4);
    spot_shiori_overlay_pointers_1_init();
    func_80011DFC();
}

void spot_script_load_step(void) {
    switch (D_800B1746) {
    case 0:
        func_80015D28(6, 0x80198000, 0xA191);
        break;
    case 1:
        func_80015D28(3, 0x80198000, 0xA1A4);
        break;
    case 2:
        func_80015D28(4, 0x80198000, 0xA197);
        break;
    case 3:
        func_80015D28(5, 0x80198000, 0xA19B);
        break;
    case 4:
        func_80015D28(6, 0x80198000, 0xA1B7);
        break;
    case 5:
        func_80015D28(5, 0x80198000, 0xA1A7);
        break;
    case 6:
        func_80015D28(6, 0x80198000, 0xA1AC);
        break;
    case 7:
        func_80015D28(4, 0x80198000, 0xA1A0);
        break;
    case 8:
        func_80015D28(4, 0x80198000, 0xA18D);
        break;
    case 9:
        func_80015D28(5, 0x80198000, 0xA1B2);
        break;
    case 10:
        func_80015D28(6, 0x80198000, 0xA1C4);
        break;
    case 11:
    case 12:
    case 13:
        func_80015D28(7, 0x80198000, 0xA1BD);
        break;
    default:
        func_80015FE0();
        break;
    }
    spot_script_sections_init();
    func_80011DFC();
}

void spot_shiori_scene1_still_fade_out_35(void) {
    s16 i;
    SpriteQuad *records;
    s32 before;
    s32 after;

    before = D_800B1AF6;
    func_8004C984();
    records = (SpriteQuad *)D_800EC190;
    for (i = 4; i < 19; i++) {
        if (D_800F19AB < 96) {
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

void spot_shiori_scene1_still_fade_in_32(void) {
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
        if (D_800F19AB < 97) {
            records[i].tint_r = records[i].tint_g = records[i].tint_b = D_800F19AB;
        }
    }
}

/* FAKE: s32 return type without a return statement keeps $v0 live so as1 fills the beqz delay slot from the target block. T-24081 */
s32 spot_shiori_scene1_sprites_update(void) {
    s16 i;

    for (i = 4; i < 0x13; i++) {
        ((SpriteQuad *)D_800EC190)[i].y += 0xA;
        if (((func_800789A0() & 3) == 0 && ((SpriteQuad *)D_800EC190)[i].y >= 0x11 && D_80120CF0[i - 4] == 0 && D_80120D10[i - 4] == 0) ||
            D_80120CF0[i - 4] != 0 || D_80120D10[i - 4] != 0) {
            ((SpriteQuad *)D_800EC190)[i].y -= 0xA;
            if (D_80120CF0[i - 4] == 0) {
                if (D_80120D10[i - 4] == 0) {
                    if (((SpriteQuad *)D_800EC190)[i].x < -0x57) {
                        ((SpriteQuad *)D_800EC190)[i].y = func_800789A0() % 4 + 0x1C;
                    } else {
                        ((SpriteQuad *)D_800EC190)[i].y = func_800789A0() % 15 + 0x11;
                    }
                    ((SpriteQuad *)D_800EC190)[i].x -= 0xC;
                    /* FAKE: SpotSpriteQuad signed views of unk_0E/unk_0A share constants 8 and 0x18 with unk_08 in $s3/$s5 while preserving ixa addressing. T-24081 */
                    ((SpotSpriteQuad *)D_800EC190)[i].unk_0E = 8;
                    ((SpriteQuad *)D_800EC190)[i].w = 0x18;
                    ((SpotSpriteQuad *)D_800EC190)[i].unk_0A = 8;
                    D_80120D10[i - 4] += 1;
                } else {
                    D_80120D10[i - 4] = 0;
                    D_80120CF0[i - 4] += 1;
                }
            } else if (D_80120CF0[i - 4] == 5) {
                ((SpriteQuad *)D_800EC190)[i].unk_0E = 0;
                ((SpriteQuad *)D_800EC190)[i].w = 8;
                ((SpotSpriteQuad *)D_800EC190)[i].unk_0A = 0x18;
                ((SpriteQuad *)D_800EC190)[i].x = func_800789A0() % 167 - 0x9F;
                if (((SpriteQuad *)D_800EC190)[i].x >= -0x57) {
                    ((SpriteQuad *)D_800EC190)[i].x += 0x99;
                }
                ((SpriteQuad *)D_800EC190)[i].y = -0x5F;
                D_80120CF0[i - 4] = 0;
                D_80120D10[i - 4] = 0;
            } else if (D_80120CF0[i - 4] == 4) {
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
        } else if (((SpriteQuad *)D_800EC190)[i].y >= 0x11 && D_80120CF0[i - 4] == 0 && D_80120D10[i - 4] == 0) {
            ((SpriteQuad *)D_800EC190)[i].x = func_800789A0() % 167 - 0x9F;
            if (((SpriteQuad *)D_800EC190)[i].x >= -0x57) {
                ((SpriteQuad *)D_800EC190)[i].x += 0x99;
            }
            ((SpriteQuad *)D_800EC190)[i].y = -0x5F;
        }
    }
}

void spot_shiori_scene2_run(void) {
    if (D_800B1AF5 == 0) {
        spot_shiori_scene2_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_shiori_scene2_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotShioriAndMiharuSceneStepTable tbl;

    tbl = spot_shiori_scene2_steps;
    idx = D_800B1AF6;
    tbl.f[idx]();
    if (D_800B1AF6 >= 0xDU && D_800B1AF6 < 0x12U) {
        func_8003C2C0();
    }
}

void spot_shiori_scene8_run(void) {
    if (D_800B1AF5 == 0) {
        spot_shiori_scene8_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_shiori_scene8_steps_run(void) {
    u8 index;
    SpotShioriScene8StepTable table;

    table = spot_shiori_scene8_steps;
    index = D_800B1AF6;
    table.callback[index](0x80);
    index = D_800B1AF6;
    if (func_8004C90C == table.callback[index]) {
        D_800EB02B |= 0x80;
    }
    if (func_8004C984 == table.callback[index]) {
        D_800EB02B &= 0xFF7F;
    }
    spot_shiori_scene8_face_update();
}

void spot_shiori_scene8_assets_load_12(void) {
    func_80015D28(0x45, 0x801B0000, 0x7D4A);
    spot_shiori_still_pointers_4_init();
    func_80011DFC();
}

void spot_shiori_scene8_sound_bank_load_01(void) {
    func_800143DC(1, 0xB176, 0xB166, 0xBAF6, 0xBAA6, 0xBAA1);
    func_80011DFC();
}

void spot_shiori_scene8_se_play_21(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void spot_shiori_scene8_portrait_read_07(void) {
    func_800335E0(0x5714);
    func_80011DFC();
}

void spot_shiori_scene8_face_update(void) {
    if ((u8) D_800B1AF6 >= 0x1BU) {
        D_800EB02F = D_800EAFA7;
    }
    if (((u8) D_800B1AF6 >= 0x1BU) && (((u32) D_800B1AE4 % 300U) == 0x14)) {
        D_800EB02A |= 3;
    }
}

void spot_shiori_scene1_init_00(void) {
    D_800B1746 = 0;
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
    func_8004CDDC();
    func_80045B14();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    func_8004BC20(D_800B1746);
    spot_script_sections_init();
    spot_talk_text_table = spot_script_sections;
    spot_talk_command_table = D_801202F0;
    spot_talk_voice_table = D_80120310;
    func_80078970(D_80094784, "校門前");
    if (D_800EECB0 == 1) {
        D_800B0A04[0].unk_02 += 1;
        D_800B0A04[0].unk_06 += 2;
        func_8004C250();
    }
    func_80011DFC();
}

void spot_shiori_scene1_still_show_06(void) {
    s16 i;
    SpriteQuad *p;
    u16 clut;

    spot_shiori_overlay_pointers_1_init();
    func_80012D64(D_80120C00, 0x11, 1, 2, 0);
    func_8004C46C(D_80120C08, D_80120C0C, D_80120C10, D_80120C14, D_80120C18, D_80120C1C);
    func_8004C6B0(D_80120BF4, D_80120BF8, spot_shiori_overlay_pointers_1, D_80120BFC);
    func_80012D64(D_80120C04, 0x12, 1, 3, 0);
    for (i = 4; i < 0x13; i++) {
        p = (SpriteQuad *)(D_800EC190 + i * 0x24);
        /* FAKE: u16 clut local before unk_04 call shifts the i * 2 CSE temporary to $v0. T-24081 */
        clut = 0x1F2;
        p->flags = 0xC1000000;
        p->x = func_800789A0() % 320 - 0x9F;
        p->y = -0x78;
        p->w = 8;
        /* FAKE: s16 view of the u16 member shares constant 8 with unk_08 in $s2. T-24081 */
        *(s16 *)&p->h = 8;
        p->unk_0C = 5;
        p->unk_0E = 0;
        p->unk_0F = 0;
        p->unk_10 = 0;
        p->unk_12 = clut;
        p->tint_r = 0;
        p->tint_g = 0;
        p->tint_b = 0;
        p->anchor_x = 0;
        p->anchor_y = 0;
        p->unk_1C = 0x1000;
        p->unk_1E = 0x1000;
        p->unk_20 = 0;
        D_800B188D[i] = 0xC;
        ((s16 *)&D_80120CE8)[i] = 0;
        D_80120D08[i] = 0;
    }
    func_80011DFC();
}

void spot_shiori_scene2_init_00(void) {
    D_800B1746 = 0;
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
    func_8003535C();
    func_8003580C();
    func_8004C360();
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    spot_script_sections_init();
    spot_talk_text_table = D_801202D4;
    spot_talk_command_table = D_801202F4;
    spot_talk_voice_table = D_80120314;
    func_80078970(D_80094784, "自宅前");
    D_800B096E -= 1;
    func_8004C250();
    func_80011DFC();
}

void spot_shiori_scene8_special_case_00(void) {
    spot_bios_srand(D_800B1AE4);
    D_800B1746 = 0;
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
    func_8004CDDC();
    func_80078970(D_80094784, "詩織の部屋");
    func_80078970(D_800947C4, "自宅前");
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    func_8004BC20(D_800B1746);
    spot_script_sections_init();
    spot_talk_text_table = D_801202EC;
    spot_talk_command_table = D_8012030C;
    spot_talk_voice_table = D_8012032C;
    D_800B1C70 = -1;
    D_800B1CF0 = -1;
    func_80011DFC();
}

void spot_shiori_scene8_still_show_27(void) {
    spot_shiori_still_pointers_4_init();
    func_80012D64(D_80120CD4, 0x11, 1, 2, 0);
    func_8004C46C(D_80120CD8, D_80120CDC, D_80120CE0, D_80120CE4, D_80120CE8, D_80120CEC);
    func_8004C6B0(D_80120CC8, D_80120CCC, spot_shiori_still_pointers_4, D_80120CD0);
    ((Actor *)D_800EAFA0)[1].unk_04 = 8;
    ((Actor *)D_800EAFA0)->unk_04 = 8;
    ((Actor *)D_800EAFA0)[2] = ((Actor *)D_800EAFA0)[0];
    ((Actor *)D_800EAFA0)[2].sequence_slot = 4;
    func_80011DFC();
}

void spot_shiori_scene3_still_show_stats_04(void) {
    spot_talk_text_table = D_801202D8;
    spot_talk_command_table = D_801202F8;
    spot_talk_voice_table = D_80120318;
    func_80012D64(D_80120C34, 0x11, 1, 2, 0);
    func_8004C46C(D_80120C38, D_80120C3C, D_80120C40, D_80120C44, D_80120C48, D_80120C4C);
    func_8004C6B0(D_80120C28, D_80120C2C, spot_shiori_still_pointers_1, D_80120C30);
    D_800B0A04[0].unk_02 += 2;
    D_800B0A04[0].unk_0A -= 0x1C;
    func_8004C250();
    func_80011DFC();
}

void spot_shiori_scene4_still_show_stats_05(void) {
    spot_talk_text_table = D_801202DC;
    spot_talk_command_table = D_801202FC;
    spot_talk_voice_table = D_8012031C;
    func_80012D64(D_80120C60, 0x11, 1, 2, 0);
    func_8004C46C(D_80120C64, D_80120C68, D_80120C6C, D_80120C70, D_80120C74, D_80120C78);
    func_8004C6B0(D_80120C54, D_80120C58, spot_shiori_still_pointers_2, D_80120C5C);
    D_800B0A04[0].unk_02 += 3;
    D_800B0A04[0].unk_06 += 2;
    D_800B0A04[0].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_shiori_scene5_still_show_stats_03(void) {
    spot_talk_text_table = D_801202E0;
    spot_talk_command_table = D_80120300;
    spot_talk_voice_table = D_80120320;
    func_80012D64(D_80120C8C, 0x11, 1, 2, 0);
    func_8004C46C(D_80120C90, D_80120C94, D_80120C98, D_80120C9C, D_80120CA0, D_80120CA4);
    func_8004C6B0(D_80120C80, D_80120C84, spot_shiori_still_pointers_3, D_80120C88);
    D_800B0A04[0].unk_02 += 1;
    D_800B0A04[0].unk_06 += 1;
    D_800B0A04[0].unk_0A -= 0x14;
    func_8004C250();
    D_800EECE4 = 0x16;
    func_80011DFC();
}

void spot_shiori_scene6_still_show_stats_05(void) {
    spot_talk_text_table = D_801202E4;
    spot_talk_command_table = D_80120304;
    spot_talk_voice_table = D_80120324;
    func_80012D64(spot_shiori_tile_pointers_1, 0x11, 1, 2, 0);
    func_8004C46C(D_80120CAC, D_80120CB0, D_80120CB4, D_80120CB8, D_80120CBC, D_80120CC0);
    func_8004C6B0(0, 0, 0, 0);
    D_800B0A04[0].unk_02 += 1;
    D_800B0A04[0].unk_06 += 2;
    D_800B0A04[0].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_shiori_scene1_bg_load_10(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void spot_shiori_scene1_bg_load_25(void) {
    func_800469F4(0x461A);
    func_80011DFC();
}

void spot_shiori_scene2_bg_load_05(void) {
    func_800469F4(0x40E8);
    func_80011DFC();
}

void spot_shiori_scene3_bg_load_10(void) {
    func_800469F4(0x4700);
    func_80011DFC();
}

void spot_shiori_scene4_bg_load_12(void) {
    func_800469F4(0x4713);
    func_80011DFC();
}

void spot_shiori_scene4_bg_load_24(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void spot_shiori_scene5_bg_load_07(void) {
    func_800469F4(0x42CD);
    func_80011DFC();
}

void spot_shiori_scene5_bg_load_19(void) {
    func_800469F4(0x4746);
    func_80011DFC();
}

void spot_shiori_scene5_bg_load_39(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void spot_shiori_scene6_bg_load_09(void) {
    func_800469F4(0x4831);
    func_80011DFC();
}

void spot_shiori_scene6_bg_load_23(void) {
    func_800469F4(0x4538);
    func_80011DFC();
}

void spot_shiori_scene8_bg_load_03(void) {
    func_800469F4(0x410B);
    func_80011DFC();
}

void spot_shiori_scene8_bg_load_22(void) {
    func_800469F4(0x4691);
    func_80011DFC();
}

void spot_shiori_scene8_bg_load_36(void) {
    func_800469F4(0x40F1);
    func_80011DFC();
}

void spot_talk_reset_step(void) {
    D_800EECBC = 0;
    spot_face_cells = 1;
    D_8012438E = 1;
    D_80124390 = 1;
    spot_reaction_low_cells = 0;
    D_801243A6 = 0;
    D_801243A8 = 0;
    spot_reaction_tier_cells = 1;
    D_80124396 = 1;
    D_80124398 = 1;
    spot_reaction_cells = 0;
    D_8012439E = 0;
    D_801243A0 = 0;
    func_80011DFC();
    D_80094714 = 0;
    D_80094718 = 0;
    spot_script_sections_init();
}

void spot_shiori_scene3_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotShioriAyakoMegumiAndMovieTheaterStepTable tbl;

    tbl = spot_shiori_scene3_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_shiori_scene3_assets_load_00(void) {
    func_80015D28(0x45, 0x801B0000, 0x7BF1);
    spot_shiori_still_pointers_1_init();
    func_80011DFC();
}

void spot_shiori_scene4_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotShioriAndYumiSceneStepTable tbl;

    tbl = spot_shiori_scene4_steps;
    idx = D_800B1AF6;
    tbl.f[idx]();
}

void spot_shiori_scene4_assets_load_02(void) {
    func_80015D28(0x45, 0x801B0000, 0x7C36);
    spot_shiori_still_pointers_2_init();
    func_80011DFC();
}

void spot_shiori_scene5_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotShioriScene5StepTable tbl;

    tbl = spot_shiori_scene5_steps;
    idx = D_800B1AF6;
    tbl.f[idx]();
}

void spot_shiori_scene5_special_case_25(void) {
    spot_talk_step();
    if ((u16) D_80094718 == 2) {
        func_8004CA48();
        D_800B1AF6 -= 1;
    }
}

void spot_shiori_scene5_assets_load_00(void) {
    func_80015D28(0x45, 0x801B0000, 0x7C7B);
    spot_shiori_still_pointers_3_init();
    func_80011DFC();
}

void spot_shiori_scene5_label_set_11(void) {
    func_80078970(D_800947C4, "観覧車");
    func_80011DFC();
}

void spot_shiori_scene6_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotShioriAyakoMegumiAndMovieTheaterStepTable tbl;

    tbl = spot_shiori_scene6_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_shiori_scene6_se_play_14(void) {
    func_800433D0(0x502);
    func_80011DFC();
}

void spot_shiori_scene6_face_step_21(void) {
    if ((u16) D_80094718 == 5) {
        func_8004A8EC(1);
        func_80033E88();
    } else if ((u16) D_80094718 == 9) {
        func_8004A8EC(2);
        func_80033E88();
    } else if ((u16) D_80094718 == 0xB) {
        func_8004A8EC(func_8002336C(D_800B1746));
        func_80033E88();
    }
    spot_talk_step();
}

void spot_shiori_scene6_assets_load_02(void) {
    func_80015D28(0x35, 0x801B0000, 0x7CC0);
    spot_shiori_tile_pointers_1_init();
    func_80011DFC();
}

void spot_shiori_unused_step_1(void) {
    func_80011E28(0x52);
}

void spot_shiori_unused_bg_load_2(void) {
    func_800469F4(0x416E);
    func_80011DFC();
}

void spot_shiori_unused_bg_load_3(void) {
    func_800469F4(0x4164);
    func_80011DFC();
}

void spot_shiori_unused_portrait_read_4(void) {
    switch (D_800B1746) {
    case 0:
        func_800335E0(0x5606);
        break;
    case 1:
        func_800335E0(0x5D95);
        break;
    case 2:
        func_800335E0(0x52AF);
        break;
    case 3:
        func_800335E0(0x5930);
        break;
    case 4:
        func_800335E0(0x668C);
        break;
    case 5:
        func_800335E0(0x6038);
        break;
    case 6:
        func_800335E0(0x6281);
        break;
    case 7:
        func_800335E0(0x5BA6);
        break;
    case 8:
        func_800335E0(0x50ED);
        break;
    case 9:
        func_800335E0(0x6443);
        break;
    case 10:
        func_800335E0(0x6821);
        break;
    }
    func_80011DFC();
}
