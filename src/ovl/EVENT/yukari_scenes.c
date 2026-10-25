/* yukari_scenes.c: src/ovl/EVENT/801024B0.c:5: girl 5 (Yukari) scenes 1-6. Original object 0x801024B0-0x80103B60 (config/objects/EVENT.txt). */
#define MAIN_API_OVERRIDE_func_8004C6B0 /* s32 return (like DATE sibling scene_anim_show in src/main/scene_library.c): consumes $v0 in ugen for spot_yukari_scene4_still_show_stats_03 (T-24090) */
#include "common.h"
#include "ovl/EVENT.h"

s32 func_8004C6B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void spot_yukari_still_pointers_1_init(void) {
    spot_yukari_still_pointers_1 = 0x801CE0F4;
    D_80122684 = 0x801CE0F8;
    D_80122688 = 0x801CE118;
    D_8012268C = *(s16 *)0x801CE12C;
    D_80122690 = 0x801B0000;
    D_80122694 = 0x801B2000;
    D_80122698 = 0x801B6000;
    D_8012269C = 0x801BA000;
    D_801226A0 = 0x801BE000;
    D_801226A4 = 0x801C2000;
    D_801226A8 = 0x801C6000;
}

void spot_yukari_still_pointers_2_init(void) {
    spot_yukari_still_pointers_2 = 0x801CE07C;
    D_801226B0 = 0x801CE080;
    D_801226B4 = 0x801CE090;
    D_801226B8 = *(s16 *)0x801CE0A4;
    D_801226BC = 0x801B0000;
    D_801226C0 = 0x801B2000;
    D_801226C4 = 0x801B6000;
    D_801226C8 = 0x801BA000;
    D_801226CC = 0x801BE000;
    D_801226D0 = 0x801C2000;
    D_801226D4 = 0x801C6000;
}

void spot_yukari_still_pointers_3_init(void) {
    spot_yukari_still_pointers_3 = 0x801CE31C;
    D_801226DC = 0x801CE320;
    D_801226E0 = 0x801CE39C;
    D_801226E4 = *(s16 *)0x801CE3BC;
    D_801226E8 = 0x801B0000;
    D_801226EC = 0x801B2000;
    D_801226F0 = 0x801B6000;
    D_801226F4 = 0x801BA000;
    D_801226F8 = 0x801BE000;
    D_801226FC = 0x801C2000;
    D_80122700 = 0x801C6000;
}

void spot_yukari_still_pointers_4_init(void) {
    spot_yukari_still_pointers_4 = 0x801CE124;
    D_80122708 = 0x801CE128;
    D_8012270C = 0x801CE148;
    D_80122710 = *(s16 *)0x801CE15C;
    D_80122714 = 0x801B0000;
    D_80122718 = 0x801B2000;
    D_8012271C = 0x801B6000;
    D_80122720 = 0x801BA000;
    D_80122724 = 0x801BE000;
    D_80122728 = 0x801C2000;
    D_8012272C = 0x801C6000;
}

void spot_yukari_tile_pointers_1_init(void) {
    spot_yukari_tile_pointers_1 = 0x801B0000;
    D_80122734 = 0x801B2000;
    D_80122738 = 0x801B6000;
    D_8012273C = 0x801BA000;
    D_80122740 = 0x801BE000;
    D_80122744 = 0x801C2000;
    D_80122748 = 0x801C6000;
}

void spot_yukari_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_yukari_scene1_run();
        return;
    case 2:
        spot_yukari_scene2_run();
        return;
    case 3:
        spot_yukari_scene3_run();
        return;
    case 4:
        spot_yukari_scene4_run();
        return;
    case 5:
        spot_yukari_scene5_run();
        return;
    case 6:
        spot_yukari_scene6_run();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void spot_yukari_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_yukari_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_yukari_scene2_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_yukari_scene2_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_yukari_scene1_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = *(FnTbl32 *)&D_8012277C;
    func_80078950("s%d %d\n", D_800B1AF6, D_800E9E63);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_yukari_scene1_assets_load_10(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8439);
    spot_yukari_still_pointers_1_init();
    func_80011DFC();
}

void spot_yukari_scene2_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = spot_yukari_scene2_steps;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx]();
}

void spot_yukari_shared_init_1(void) {
    D_800B1746 = 5;
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
    spot_talk_text_table = D_80120444;
    spot_talk_command_table = D_80120460;
    spot_talk_voice_table = D_8012047C;
    func_80078970(D_80094784, "廊下");
    if (D_800EECB0 == 1) {
        D_800B0B0E[0] += 1;
        D_800B0B0E[9] -= 1;
        func_8004C250();
    }
    func_80011DFC();
}

void spot_yukari_scene1_still_show_20(void) {
    spot_yukari_still_pointers_1_init();
    func_80012D64(D_80122690, 0x11, 1, 2, 0);
    func_8004C46C(D_80122694, D_80122698, D_8012269C, D_801226A0, D_801226A4, D_801226A8);
    func_8004C6B0(D_80122684, D_80122688, spot_yukari_still_pointers_1, (s32) D_8012268C);
    ((Actor *)D_800EAFA0)->unk_04 = ((Actor *)D_800EAFA0)[1].unk_04 = 8;
    func_80011DFC();
}

void spot_yukari_scene2_step_01(void) {
    spot_talk_text_table = D_80120448;
    spot_talk_command_table = D_80120464;
    spot_talk_voice_table = D_80120480;
    func_80078970(D_80094784, "廊下");
    D_800B0A04[5].unk_02 += 1;
    func_8004C250();
    func_80011DFC();
}

void spot_yukari_scene3_still_show_stats_03(void) {
    spot_talk_text_table = D_8012044C;
    spot_talk_command_table = D_80120468;
    spot_talk_voice_table = D_80120484;
    spot_yukari_still_pointers_2_init();
    func_80012D64(D_801226BC, 0x11, 1, 2, 0);
    func_8004C46C(D_801226C0, D_801226C4, D_801226C8, D_801226CC, D_801226D0, D_801226D4);
    func_8004C6B0(D_801226B0, D_801226B4, spot_yukari_still_pointers_2, D_801226B8);
    D_800B0A04[5].unk_02 += 2;
    D_800B0A04[5].unk_06 += 1;
    D_800B0A04[5].unk_0A -= 0xA;
    func_8004C250();
    func_80011DFC();
}

void spot_yukari_scene4_still_show_stats_03(void) {
    Actor *m = (Actor *)D_800E9620;
    s16 i;

    spot_talk_text_table = D_80120450;
    spot_talk_command_table = D_8012046C;
    spot_talk_voice_table = D_80120488;
    D_800B0A04[5].unk_02 += 2;
    D_800B0A04[5].unk_06 += 1;
    D_800B0A04[5].unk_0A -= 0x14;
    func_8004C250();
    spot_yukari_still_pointers_3_init();
    func_80012D64(D_801226E8, 0x11, 1, 2, 0);
    func_8004C46C(D_801226EC, D_801226F0, D_801226F4, D_801226F8, D_801226FC, D_80122700);
    func_8004C6B0(D_801226DC, D_801226E0, spot_yukari_still_pointers_3, D_801226E4);
    m[96].unk_04 = m[97].unk_04 = 8;
    for (i = 0; i < 8; i++) {
        D_8012274C[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        m[98 + i] = ((Actor *)D_800EAFA0)[0];
        m[98 + i].sequence_slot = 5;
        m[98 + i].x = D_80122754[i] - 0xA0;
        m[98 + i].y = D_80122764[i] - 0x78;
        D_80122774[i] = func_800789A0() % 120;
    }
    m[98].sequence_slot = 4;
    func_80011DFC();
}

void spot_yukari_scene5_still_show_stats_03(void) {
    spot_talk_text_table = D_80120454;
    spot_talk_command_table = D_80120470;
    spot_talk_voice_table = D_8012048C;
    spot_yukari_still_pointers_4_init();
    func_80012D64(D_80122714, 0x11, 1, 2, 0);
    func_8004C46C(D_80122718, D_8012271C, D_80122720, D_80122724, D_80122728, D_8012272C);
    func_8004C6B0(D_80122708, D_8012270C, spot_yukari_still_pointers_4, D_80122710);
    D_800B0A04[5].unk_02 += 3;
    D_800B0A04[5].unk_06 += 2;
    D_800B0A04[5].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_yukari_scene6_still_show_stats_03(void) {
    spot_talk_text_table = D_80120458;
    spot_talk_command_table = D_80120474;
    spot_talk_voice_table = D_80120490;
    spot_yukari_tile_pointers_1_init();
    func_80012D64(spot_yukari_tile_pointers_1, 0x11, 1, 2, 0);
    func_8004C46C(D_80122734, D_80122738, D_8012273C, D_80122740, D_80122744, D_80122748);
    func_8004C6B0(0, 0, 0, 0);
    D_800B0A04[5].unk_02 += 1;
    D_800B0A04[5].unk_06 += 1;
    D_800B0A04[5].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_yukari_scene1_bg_load_03(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void spot_yukari_scene1_bg_load_21(void) {
    func_800469F4(0x463D);
    func_80011DFC();
}

void spot_yukari_scene2_bg_load_02(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void spot_yukari_scene3_bg_load_12(void) {
    func_800469F4(0x4835);
    func_80011DFC();
}

void spot_yukari_scene3_bg_load_20(void) {
    func_800469F4(0x454C);
    func_80011DFC();
}

void spot_yukari_scene4_bg_load_14(void) {
    func_800469F4(0x47FC);
    func_80011DFC();
}

void spot_yukari_scene4_bg_load_29(void) {
    func_800469F4(0x450A);
    func_80011DFC();
}

void spot_yukari_scene5_bg_load_12(void) {
    func_800469F4(0x4726);
    func_80011DFC();
}

void spot_yukari_scene5_bg_load_20(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void spot_yukari_scene6_bg_load_17(void) {
    func_800469F4(0x47D1);
    func_80011DFC();
}

void spot_yukari_scene6_bg_load_25(void) {
    func_800469F4(0x44DB);
    func_80011DFC();
}

void spot_yukari_scene6_bg_load_39(void) {
    func_800469F4(0x44D1);
    func_80011DFC();
}

void spot_yukari_scene3_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = spot_yukari_scene3_steps;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_yukari_scene3_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8476);
    spot_yukari_still_pointers_2_init();
    func_80011DFC();
}

/* FAKE: s32 return type (implicit int from forward call at line 85) keeps $v0 live at epilogue, and unused pad local places tbl at sp+0x28 in the 0xE0 frame (T-3330); T-24105 */
s32 spot_yukari_scene4_run(void) {
    s32 pad;
    s16 i;
    EvFnTbl44 tbl;

    tbl = spot_yukari_scene4_steps;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
    if (tbl.f[D_800B1AF6] == func_8004C984) {
        for (i = 0; i < 8; i++) {
            ((Actor *)D_800E9620)[i + 98].brightness = ((Actor *)D_800E9620)[96].brightness;
        }
        if (((Actor *)D_800E9620)[96].brightness < 9) {
            for (i = 0; i < 8; i++) {
                ((Actor *)D_800E9620)[i + 98].draw_flags &= 0x7F;
            }
        }
    }
    if ((D_800B1AF6 >= 0x11) && (D_800B1AF6 < 0x14)) {
        spot_yukari_scene4_sprites_update();
    }
}

void spot_yukari_scene4_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x84B3);
    spot_yukari_still_pointers_3_init();
    func_80011DFC();
}

void spot_yukari_scene4_se_play_11(void) {
    func_800433D0(0x502);
    func_80011DFC();
}

void spot_yukari_scene4_sprites_update(void) {
    ActorFixed *m = (ActorFixed *)D_800E9620;
    u8 *st;
    EvPairs8 sp68;
    s16 i;

    sp68 = D_80122990;
    for (i = 0; i < 8; i++) {
        st = i + D_8012274C;
        m[98 + i].unk_07 = ((Actor *)D_800EAFA0)[0].brightness;
        if (*st == 0) {
            D_80122774[i]++;
        }
        if (*st == 0 && D_80122774[i] >= 0x79 && (u32)(0xB4 - D_80122774[i]) < (u32)(func_800789A0() % 60)) {
            m[98 + i].draw_flags |= 0x80;
            m[98 + i].flags = 5;
            *st = 1;
        }
        if (i == 0) {
            if (*st == 1 && m[98 + i].frame == 7 && m[98 + i].unk_08 == 0) {
                m[98 + i].dy = 0;
                *st = 2;
            }
        } else if (*st == 1 && m[98 + i].frame == 3 && m[98 + i].unk_08 == 7) {
            m[98 + i].dy = 0;
            *st = 2;
        }
        if (*st == 2) {
            m[98 + i].dy += 0x4000;
            m[98 + i].y.w += m[98 + i].dy;
            if (sp68.v[i][1] - 0x78 < m[98 + i].y.h[1]) {
                m[98 + i].draw_flags &= 0x7F;
                m[98 + i].flags = 0;
                m[98 + i].y.h[1] = D_80122764[i] - 0x78;
                m[98 + i].y.h[0] = 0;
                m[98 + i].frame = 0;
                m[98 + i].unk_08 = 0;
                *st = 0;
                D_80122774[i] = 0;
            }
        }
    }
}

void spot_yukari_scene5_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = spot_yukari_scene5_steps;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_yukari_scene5_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x84F0);
    spot_yukari_still_pointers_4_init();
    func_80011DFC();
}

void spot_yukari_scene6_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl56 tbl;

    tbl = spot_yukari_scene6_steps;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}
