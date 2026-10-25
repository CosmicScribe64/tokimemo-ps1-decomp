/* yumi_scenes.c: src/ovl/EVENT/80107FC0.c:5: girl 10 (Yumi) scenes 1-7. Original object 0x80107FC0-0x8010A210 (config/objects/EVENT.txt). */
#include "common.h"
#include "ovl/EVENT.h"
#include "psx_types.h"

void spot_yumi_still_pointers_1_init(void) {
    spot_yumi_still_pointers_1 = 0x801D21BC;
    D_80123884 = 0x801D21C4;
    D_80123888 = 0x801D2204;
    D_8012388C = *(s16 *)0x801D221C;
    D_80123890 = 0x801B0000;
    D_80123894 = 0x801B2000;
    D_80123898 = 0x801B6000;
    D_8012389C = 0x801BA000;
    D_801238A0 = 0x801BE000;
    D_801238A4 = 0x801C2000;
    D_801238A8 = 0x801C6000;
}

void spot_yumi_still_pointers_2_init(void) {
    spot_yumi_still_pointers_2 = 0x801CE0F4;
    D_801238B0 = 0x801CE0F8;
    D_801238B4 = 0x801CE118;
    D_801238B8 = *(s16 *)0x801CE12C;
    D_801238BC = 0x801B0000;
    D_801238C0 = 0x801B2000;
    D_801238C4 = 0x801B6000;
    D_801238C8 = 0x801BA000;
    D_801238CC = 0x801BE000;
    D_801238D0 = 0x801C2000;
    D_801238D4 = 0x801C6000;
}

void spot_yumi_still_pointers_3_init(void) {
    spot_yumi_still_pointers_3 = 0x801CE0F4;
    D_801238DC = 0x801CE0F8;
    D_801238E0 = 0x801CE118;
    D_801238E4 = *(s16 *)0x801CE12C;
    D_801238E8 = 0x801B0000;
    D_801238EC = 0x801B2000;
    D_801238F0 = 0x801B6000;
    D_801238F4 = 0x801BA000;
    D_801238F8 = 0x801BE000;
    D_801238FC = 0x801C2000;
    D_80123900 = 0x801C6000;
}

void spot_yumi_still_pointers_4_init(void) {
    spot_yumi_still_pointers_4 = 0x801CE0F4;
    D_80123908 = 0x801CE0F8;
    D_8012390C = 0x801CE118;
    D_80123910 = *(s16 *)0x801CE12C;
    D_80123914 = 0x801B0000;
    D_80123918 = 0x801B2000;
    D_8012391C = 0x801B6000;
    D_80123920 = 0x801BA000;
    D_80123924 = 0x801BE000;
    D_80123928 = 0x801C2000;
    D_8012392C = 0x801C6000;
}

void spot_yumi_still_pointers_5_init(void) {
    spot_yumi_still_pointers_5 = 0x801D2470;
    D_80123934 = 0x801D2478;
    D_80123938 = 0x801D24B8;
    D_8012393C = *(s16 *)0x801D24D0;
    D_80123940 = 0x801B0000;
    D_80123944 = 0x801B2000;
    D_80123948 = 0x801B6000;
    D_8012394C = 0x801BA000;
    D_80123950 = 0x801BE000;
    D_80123954 = 0x801C2000;
    D_80123958 = 0x801C6000;
}

void spot_yumi_still_pointers_6_init(void) {
    spot_yumi_still_pointers_6 = 0x801D2600;
    D_80123960 = 0x801D2608;
    D_80123964 = 0x801D2674;
    D_80123968 = *(s16 *)0x801D268C;
    D_8012396C = 0x801B0000;
    D_80123970 = 0x801B2000;
    D_80123974 = 0x801B6000;
    D_80123978 = 0x801BA000;
    D_8012397C = 0x801BE000;
    D_80123980 = 0x801C2000;
    D_80123984 = 0x801C6000;
}

void spot_yumi_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_yumi_scene1_run();
        return;
    case 2:
        spot_yumi_scene2_run();
        return;
    case 3:
        spot_yumi_scene3_run();
        return;
    case 4:
        spot_yumi_scene4_run();
        return;
    case 5:
        spot_yumi_scene5_run();
        return;
    case 6:
        spot_yumi_scene6_run();
        return;
    case 7:
        spot_yumi_scene7_run();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void spot_yumi_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_yumi_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_yumi_scene2_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_yumi_scene2_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_yumi_scene7_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_yumi_scene7_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_yumi_scene1_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = spot_yumi_scene1_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_yumi_scene1_assets_load_07(void) {
    func_80015D28(0x45, 0x801B0000, 0x8A1D);
    spot_yumi_still_pointers_1_init();
    func_80011DFC();
}

void spot_yumi_scene2_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl21 tbl;

    tbl = spot_yumi_scene2_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_yumi_scene1_branch_19(void) {
    if (D_800B0E6A != 0) {
        D_80094714 = (u16) D_80094714 + 5;
    }
    func_80011DFC();
}

void spot_yumi_scene1_branch_35(void) {
    if (D_800B0E6A != 0) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void spot_yumi_scene2_branch_on_mood_14(void) {
    if (((u8)func_8002328C(D_800B1746) & 0x7F) >= 2U) {
        D_80094714 += 2;
    }
    func_80011DFC();
}

void spot_yumi_scene7_steps_run(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl55 tbl;

    tbl = spot_yumi_scene7_steps;
    tbl.f[D_800B1AF6](0x80);
    if ((u8)D_80123988 != 0 && D_800B1AF6 >= 0x1B) {
        spot_yumi_scene7_flash_draw();
    }
    if (D_8012398C != 0 && D_800B1AF6 >= 0x1B) {
        spot_yumi_scene7_sprites_update();
    }
    func_80078950("flag%x\n", D_800EB02B);
}

void spot_yumi_scene7_assets_load_03(void) {
    func_80015D28(0x45, 0x801B0000, 0x8B5E);
    spot_yumi_still_pointers_6_init();
    func_80011DFC();
}

void spot_yumi_scene7_se_play_31(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void spot_yumi_scene7_sound_bank_load_01(void) {
    func_800143DC(1, 0xB60A, 0xB605, 0xC906, 0xC8B8, 0xC8B3);
    func_80011DFC();
}

void spot_yumi_scene7_special_case_38(void) {
    spot_talk_step();
    if (D_800EECC0 != 0) {
        D_800EECC0 = 0;
        D_800EECCC = 0;
        D_800E96AB &= 0xFF7F;
        D_80094714 = (u16) D_80094714 + 1;
        D_80094718 = 0;
        func_80011DFC();
    }
}

void spot_yumi_scene7_set_state_43(void) {
    D_8008093C = 0xF;
    func_80011DFC();
}

/* FAKE: an implicit s32 return keeps the narrowed phase in v0; the void form
 * uses t0. Callers ignore the return value. T-9720. */
s32 spot_yumi_scene7_branch_27(void) {
    s16 phase;

    if (D_800B1AF0 == 0) {
        D_800EB02B |= 0x80;
        D_800EB044 = 0;
        D_800EB042 = 0;
    }
    phase = D_800EB042 += 0x80;
    D_800EB044 += 0x80;
    if (phase >= 0x1000) {
        D_800EB02B &= 0xFF7F;
        D_80123988 = 1;
        func_80011DFC();
    }
    D_800B1AF0 += 1;
}

void spot_yumi_scene7_flash_draw(void) {
    spot_yumi_scene7_gradient_quad_draw(-0xA0, -0x78, 0x140, 0xA0, 0xFFFFFF, 0xA, 0);
    func_8001A16C(9);
}

/* FAKE: the explicit "& 0xFF" masks and one source line per colour trio, as in the matched main sibling
 * _sprite_set_box_shade_tarao (T-17001; one colour instead of two): the masks leave the shifts in ugen
 * temporaries (t6, t7) and as1 schedules the stores by source line. The real source is unknown. T-24017 */
void spot_yumi_scene7_gradient_quad_draw(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    SpotDevelopmentState *gs;
    POLY_G4 *p;

    p = (POLY_G4 *)func_80019D00(0x24, D_800E9600);
    func_80067678(p);
    func_800675A8(p, arg6);
    gs = (SpotDevelopmentState *)D_800B0860;
    p->r0 = (arg4 >> 16) & 0xFF; p->g0 = (arg4 >> 8) & 0xFF; p->b0 = arg4 & 0xFF;
    p->r1 = (arg4 >> 16) & 0xFF; p->g1 = (arg4 >> 8) & 0xFF; p->b1 = arg4 & 0xFF;
    p->r2 = (arg4 >> 16) & 0xFF; p->g2 = (arg4 >> 8) & 0xFF; p->b2 = arg4 & 0xFF;
    p->r3 = (arg4 >> 16) & 0xFF; p->g3 = (arg4 >> 8) & 0xFF; p->b3 = arg4 & 0xFF;
    p->x0 = gs->screen_center_x[D_800E9600] + arg0;
    p->y0 = gs->screen_center_y[D_800E9600] + arg1;
    p->x1 = gs->screen_center_x[D_800E9600] + arg0 + arg2;
    p->y1 = gs->screen_center_y[D_800E9600] + arg1;
    p->x2 = gs->screen_center_x[D_800E9600] + arg0;
    p->y2 = gs->screen_center_y[D_800E9600] + arg1 + arg3;
    p->x3 = gs->screen_center_x[D_800E9600] + arg0 + arg2;
    p->y3 = gs->screen_center_y[D_800E9600] + arg1 + arg3;
    func_800674F4(D_800B2580 + (D_800E9600 << 6) + arg5 * 4, (s32)p);
}


void spot_yumi_scene7_step_37(void) {
    s16 i;

    D_8008093C = 0xA;
    func_8004C808(1);
    D_800EECE0 = 1;
    D_800EAFA7 = 0x80;
    D_800EAFEB = 0x80;
    for (i = 0x64; i < 0x6A; i++) {
        func_8004C060(i, 0x80);
    }
    D_80123988 = 0;
    for (i = 0; i < 0xA; i++) {
        ((Actor *)D_800E9620)[i + 99].draw_flags |= 0x80;
    }
    D_8012398C = 1;
    func_80011DFC();
}

/* FAKE: byte-offset view of the D_800E9620 display records (Actor, records 99..108); with the
 * record fields the loop allocates one more register. T-20900 */
void spot_yumi_scene7_sprites_update(void) {
    s16 i;
    s32 any;

    for (i = 0; i < 0xA; i++) {
        if ((u32) D_8012398C >= (u32) (i * 0x1E)) {
            *(s16 *) &D_800E9620[i * 0x44 + 0x1A7A] = -1;
            *(s16 *) &D_800E9620[i * 0x44 + 0x1A7E] = 1;
        }
        *(s32 *) &D_800E9620[i * 0x44 + 0x1A70] += *(s32 *) &D_800E9620[i * 0x44 + 0x1A78];
        *(s32 *) &D_800E9620[i * 0x44 + 0x1A74] += *(s32 *) &D_800E9620[i * 0x44 + 0x1A7C];
        if (*(s16 *) &D_800E9620[i * 0x44 + 0x1A76] > 0xA0) {
            D_800E9620[i * 0x44 + 0x1A4F] &= ~0x80;
        }
    }
    any = 0;
    i = 0;
    do {
        if (D_800E9620[i * 0x44 + 0x1A4F] & 0x80) {
            any = 1;
        }
        i++;
    } while (i < 0xA);
    if (any == 0 && D_800B1AE4 % 60 == func_800789A0() % 60) {
        func_800433D0((func_800789A0() % 7 + 0x501) & 0xFFFF);
    }
    D_8012398C += 1;
}

void spot_yumi_scene1_init_00(void) {
    D_800B1746 = 0xA;
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
    spot_talk_text_table = D_801205C4;
    spot_talk_command_table = D_801205E0;
    spot_talk_voice_table = D_801205FC;
    func_80078970(D_80094784, "廊下");
    if (D_800EECB0 == 1) {
        D_800B0A04[10].unk_02 += 1;
        D_800B0A04[10].unk_06 += 2;
        *(s16 *)((u8 *)D_800B0A04 - 0xB6) -= 5; /* FAKE: this is D_800B094E; addressing it from the table keeps IDO from hoisting its load */
        *(s16 *)((u8 *)D_800B0A04 - 0x9A) += 5; /* FAKE: this is D_800B096A, same reason */
        func_8004C250();
    }
    func_80011DFC();
}

void spot_yumi_scene1_still_show_09(void) {
    spot_yumi_still_pointers_1_init();
    func_80012D64(D_80123890, 0x11, 1, 2, 0);
    func_8004C46C(D_80123894, D_80123898, D_8012389C, D_801238A0, D_801238A4, D_801238A8);
    func_8004C6B0(D_80123884, D_80123888, spot_yumi_still_pointers_1, (s32) D_8012388C);
    func_80011DFC();
}

void spot_yumi_scene2_init_00(void) {
    D_800B1746 = 0xA;
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
    spot_talk_text_table = D_801205C8;
    spot_talk_command_table = D_801205E4;
    spot_talk_voice_table = D_80120600;
    if (((u8)func_8002328C(0xA) & 0x7F) < 2U) {
        D_800B0A04[10].unk_02 += 1;
    }
    D_800B0A04[10].unk_06 += 2;
    func_8004C250();
    func_80011DFC();
}

void spot_yumi_scene7_init_00(void) {
    D_800B1746 = 0xA;
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
    D_80123988 = 0;
    D_8012398C = 0;
    func_8004BC20(D_800B1746);
    spot_script_sections_init();
    spot_talk_text_table = D_801205DC;
    spot_talk_command_table = D_801205F8;
    spot_talk_voice_table = D_80120614;
    func_80078970(D_80094784, "玄関");
    func_80078970(D_800947C4, "自宅前");
    func_80011DFC();
}

void spot_yumi_scene7_still_show_07(void) {
    /* FAKE: unused 8-byte local places t0..t2 at the original stack offsets; real source unknown. T-24092 */
    s32 pad[2];
    EvShorts10 t0;
    EvShorts10 t1;
    EvShorts10 t2;
    s16 i;

    t0 = D_80123B7C;
    t1 = D_80123B90;
    t2 = D_80123BA4;
    spot_yumi_still_pointers_6_init();
    func_80012D64(D_8012396C, 0x11, 1, 2, 0);
    func_8004C46C(D_80123970, D_80123974, D_80123978, D_8012397C, D_80123980, D_80123984);
    for (i = 0; i < 6; i++) {
        D_800B0860[i + 0x1091] = 0xC;
    }
    func_8004C6B0(D_80123960, D_80123964, spot_yumi_still_pointers_6, D_80123968);
    ((Actor *)D_800EAFA0)[1].unk_04 = 8;
    ((Actor *)D_800EAFA0)->unk_04 = 8;
    ((Actor *)D_800EAFA0)[1].unk_01 = 0xC;
    ((Actor *)D_800EAFA0)->unk_01 = 0xC;
    ((Actor *)D_800EAFA0)[2] = ((Actor *)D_800EAFA0)[0];
    ((Actor *)D_800EAFA0)[2].unk_01 = 0xB;
    ((Actor *)D_800EAFA0)[2].flags = 0x40;
    ((Actor *)D_800EAFA0)[2].unk_18 = 0x14;
    ((Actor *)D_800EAFA0)[2].x = -6;
    ((Actor *)D_800EAFA0)[2].y = -0x30;
    ((Actor *)D_800EAFA0)[2].brightness = 0x80;
    for (i = 0; i < 0xA; i++) {
        ((Actor *)D_800E9620)[i + 99] = ((Actor *)D_800EAFA0)[2];
        ((Actor *)D_800E9620)[i + 99].unk_18 = t0.v[i];
        ((Actor *)D_800E9620)[i + 99].x = t1.v[i] - 0xA0;
        ((Actor *)D_800E9620)[i + 99].y = t2.v[i] - 0x78;
    }
    func_80011DFC();
}

void spot_yumi_scene3_still_show_stats_05(void) {
    spot_talk_text_table = D_801205CC;
    spot_talk_command_table = D_801205E8;
    spot_talk_voice_table = D_80120604;
    D_800B0A04[10].unk_02 += 1;
    D_800B0A04[10].unk_06 += 1;
    D_800B0A04[10].unk_0A -= 0xA;
    func_8004C250();
    spot_yumi_still_pointers_2_init();
    func_80012D64(D_801238BC, 0x11, 1, 2, 0);
    func_8004C46C(D_801238C0, D_801238C4, D_801238C8, D_801238CC, D_801238D0, D_801238D4);
    func_8004C6B0(D_801238B0, D_801238B4, spot_yumi_still_pointers_2, D_801238B8);
    func_80011DFC();
}

void spot_yumi_scene4_still_show_stats_03(void) {
    spot_talk_text_table = D_801205D0;
    spot_talk_command_table = D_801205EC;
    spot_talk_voice_table = D_80120608;
    func_80078970(D_800947C4, "池");
    D_800B0A04[10].unk_02 += 3;
    D_800B0A04[10].unk_06 += 2;
    D_800B0A04[10].unk_0A -= 0x14;
    func_8004C250();
    spot_yumi_still_pointers_3_init();
    func_80012D64(D_801238E8, 0x11, 1, 2, 0);
    func_8004C46C(D_801238EC, D_801238F0, D_801238F4, D_801238F8, D_801238FC, D_80123900);
    func_8004C6B0(D_801238DC, D_801238E0, spot_yumi_still_pointers_3, D_801238E4);
    func_80011DFC();
}

void spot_yumi_scene5_still_show_stats_03(void) {
    spot_talk_text_table = D_801205D4;
    spot_talk_command_table = D_801205F0;
    spot_talk_voice_table = D_8012060C;
    D_800B0A04[10].unk_06 += 2;
    D_800B0A04[10].unk_0A -= 0xA;
    func_8004C250();
    spot_yumi_still_pointers_4_init();
    func_80012D64(D_80123914, 0x11, 1, 2, 0);
    func_8004C46C(D_80123918, D_8012391C, D_80123920, D_80123924, D_80123928, D_8012392C);
    func_8004C6B0(D_80123908, D_8012390C, spot_yumi_still_pointers_4, D_80123910);
    func_80011DFC();
}

void spot_yumi_scene6_still_show_stats_05(void) {
    spot_talk_text_table = D_801205D8;
    spot_talk_command_table = D_801205F4;
    spot_talk_voice_table = D_80120610;
    D_800B0A04[10].unk_02 += 2;
    D_800B0A04[10].unk_06 += 1;
    D_800B0A04[10].unk_0A -= 0x14;
    func_8004C250();
    spot_yumi_still_pointers_5_init();
    func_80012D64(D_80123940, 0x11, 1, 2, 0);
    func_8004C46C(D_80123944, D_80123948, D_8012394C, D_80123950, D_80123954, D_80123958);
    func_8004C6B0(D_80123934, D_80123938, spot_yumi_still_pointers_5, D_8012393C);
    func_80011DFC();
}

void spot_yumi_scene1_bg_load_03(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void spot_yumi_scene1_bg_load_23(void) {
    func_800469F4(0x4609);
    func_80011DFC();
}

void spot_yumi_scene1_bg_load_36(void) {
    func_800469F4(0x401E);
    func_80011DFC();
}

void spot_yumi_scene2_bg_load_03(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void spot_yumi_scene3_bg_load_10(void) {
    func_800469F4(0x4767);
    func_80011DFC();
}

void spot_yumi_scene3_bg_load_24(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void spot_yumi_scene4_bg_load_08(void) {
    func_800469F4(0x428D);
    func_80011DFC();
}

void spot_yumi_scene4_bg_load_18(void) {
    func_800469F4(0x472E);
    func_80011DFC();
}

void spot_yumi_scene4_bg_load_26(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void spot_yumi_scene5_bg_load_10(void) {
    func_800469F4(0x485A);
    func_80011DFC();
}

void spot_yumi_scene5_bg_load_25(void) {
    func_800469F4(0x4571);
    func_80011DFC();
}

void spot_yumi_scene6_bg_load_12(void) {
    func_800469F4(0x47E1);
    func_80011DFC();
}

void spot_yumi_scene6_bg_load_24(void) {
    func_800469F4(0x44DB);
    func_80011DFC();
}

void spot_yumi_scene6_bg_load_35(void) {
    func_800469F4(0x44D1);
    func_80011DFC();
}

void spot_yumi_scene7_bg_load_09(void) {
    func_800469F4(0x40E0);
    func_80011DFC();
}

void spot_yumi_scene7_bg_load_labeled_21(void) {
    func_800433D0(0x200);
    func_800469F4(0x40F9);
    func_80011DFC();
}

void spot_yumi_scene7_bg_load_33(void) {
    func_800469F4(0x469A);
    func_80011DFC();
}

void spot_yumi_scene3_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = spot_yumi_scene3_steps;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_yumi_scene3_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8A62);
    spot_yumi_still_pointers_2_init();
    func_80011DFC();
}

void spot_yumi_scene4_run(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl44 tbl;

    tbl = spot_yumi_scene4_steps;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
    if (D_800EECE0 != 0 && D_800B1AE4 % 120 == 0) {
        spot_yumi_scene4_se_play();
    }
}

void spot_yumi_scene4_se_play(void) {
    func_800433D0(0x500);
}

void spot_yumi_scene4_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8A9F);
    spot_yumi_still_pointers_3_init();
    func_80011DFC();
}

void spot_yumi_scene5_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = spot_yumi_scene5_steps;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_yumi_scene5_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8ADC);
    spot_yumi_still_pointers_4_init();
    func_80011DFC();
}

void spot_yumi_scene6_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl52 tbl;

    tbl = spot_yumi_scene6_steps;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x3C);
}

void spot_yumi_scene6_se_play_08(void) {
    func_800433D0(0x501);
    func_80011DFC();
}

void spot_yumi_scene6_se_play_19(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void spot_yumi_scene6_assets_load_00(void) {
    func_80015D28(0x45, 0x801B0000, 0x8B19);
    spot_yumi_still_pointers_5_init();
    func_80011DFC();
}
