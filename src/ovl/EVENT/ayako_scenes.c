/* ayako_scenes.c: src/ovl/EVENT/800FDC20.c:5: girl 3 (Ayako) scenes 1-5; also the gradient triangle helper. Original object 0x800FDC20-0x800FFBE0 (config/objects/EVENT.txt). */
#include "common.h"
#include "psx_types.h"
#include "ovl/EVENT.h"

void spot_wipe_gradient_draw();


void spot_ayako_still_pointers_1_init(void) {
    spot_ayako_still_pointers_1 = 0x801CE11C;
    D_80121804 = 0x801CE120;
    D_80121808 = 0x801CE140;
    D_8012180C = *(s16 *)0x801CE154;
    D_80121810 = 0x801B0000;
    D_80121814 = 0x801B2000;
    D_80121818 = 0x801B6000;
    D_8012181C = 0x801BA000;
    D_80121820 = 0x801BE000;
    D_80121824 = 0x801C2000;
    D_80121828 = 0x801C6000;
}

void spot_ayako_still_pointers_2_init(void) {
    spot_ayako_still_pointers_2 = 0x801CE0DC;
    D_80121830 = 0x801CE0E0;
    D_80121834 = 0x801CE100;
    D_80121838 = *(s16 *)0x801CE114;
    D_8012183C = 0x801B0000;
    D_80121840 = 0x801B2000;
    D_80121844 = 0x801B6000;
    D_80121848 = 0x801BA000;
    D_8012184C = 0x801BE000;
    D_80121850 = 0x801C2000;
    D_80121854 = 0x801C6000;
}

void spot_ayako_still_pointers_3_init(void) {
    spot_ayako_still_pointers_3 = 0x801D2148;
    D_8012185C = 0x801D2150;
    D_80121860 = 0x801D2170;
    D_80121864 = *(s16 *)0x801D218C;
    D_80121868 = 0x801B0000;
    D_8012186C = 0x801B2000;
    D_80121870 = 0x801B6000;
    D_80121874 = 0x801BA000;
    D_80121878 = 0x801BE000;
    D_8012187C = 0x801C2000;
    D_80121880 = 0x801C6000;
}

void spot_ayako_still_pointers_4_init(void) {
    D_80121884 = 0x801CE11C;
    D_80121888 = 0x801CE120;
    D_8012188C = 0x801CE140;
    D_80121890 = *(s16 *)0x801CE154;
    D_80121894 = 0x801B0000;
    D_80121898 = 0x801B2000;
    D_8012189C = 0x801B6000;
    D_801218A0 = 0x801BA000;
    D_801218A4 = 0x801BE000;
    D_801218A8 = 0x801C2000;
    D_801218AC = 0x801C6000;
}

void spot_ayako_still_pointers_5_init(void) {
    spot_ayako_still_pointers_5 = 0x801CE07C;
    D_801218B4 = 0x801CE080;
    D_801218B8 = 0x801CE090;
    D_801218BC = *(s16 *)0x801CE0A4;
    D_801218C0 = 0x801B0000;
    D_801218C4 = 0x801B2000;
    D_801218C8 = 0x801B6000;
    D_801218CC = 0x801BA000;
    D_801218D0 = 0x801BE000;
    D_801218D4 = 0x801C2000;
    D_801218D8 = 0x801C6000;
}

void spot_ayako_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_ayako_scene1_run();
        return;
    case 2:
        spot_ayako_scene2_run();
        return;
    case 3:
        spot_ayako_scene3_run();
        return;
    case 4:
        spot_ayako_scene4_run();
        return;
    case 5:
        spot_ayako_scene5_run();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void spot_ayako_scene1_steps_run(void);

void spot_ayako_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_ayako_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_ayako_scene1_steps_run(void) {
    s32 pad; /* FAKE: unused local above tmp; the original frame is 4 bytes larger, real source unknown. T-4080 */
    FnTbl32 tmp;

    tmp = spot_ayako_scene1_steps;
    func_80078950("s%d\n", D_800B1AF6);
}

void spot_ayako_scene1_assets_load_03(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7F2A);
    spot_ayako_still_pointers_1_init();
    func_80011DFC();
}

void spot_ayako_scene1_init_00(void) {
    D_800B1746 = 3;
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
    spot_talk_text_table = D_801203B4;
    spot_talk_command_table = D_801203CC;
    spot_talk_voice_table = D_801203E4;
    func_80078970(D_80094784, "校舎裏");
    D_800B0A04[3].unk_06 += 2;
    func_8004C250();
    func_80011DFC();
}

void spot_ayako_scene1_still_show_05(void) {
    spot_ayako_still_pointers_1_init();
    func_80012D64(D_80121810, 0x11, 1, 2, 0);
    func_8004C46C(D_80121814, D_80121818, D_8012181C, D_80121820, D_80121824, D_80121828);
    func_8004C6B0(D_80121804, D_80121808, spot_ayako_still_pointers_1, (s32) D_8012180C);
    func_80011DFC();
}

void spot_ayako_scene2_still_show_stats_03(void) {
    s16 i;

    for (i = 0; i < 0xC; i++) {
        D_801218DC[i] = func_800789A0() % 4;
    }
    spot_talk_text_table = D_801203B8;
    spot_talk_command_table = D_801203D0;
    spot_talk_voice_table = D_801203E8;
    spot_ayako_still_pointers_2_init();
    func_80012D64(D_8012183C, 0x11, 1, 2, 0);
    func_8004C46C(D_80121840, D_80121844, D_80121848, D_8012184C, D_80121850, D_80121854);
    func_8004C6B0(D_80121830, D_80121834, spot_ayako_still_pointers_2, (s32) D_80121838);
    D_800EECEC = 2;
    D_800EAFB8 = 3;
    D_800B0A04[3].unk_02 += 2;
    D_800B0A04[3].unk_06 += 1;
    D_800B0A04[3].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_ayako_scene3_still_show_stats_03(void) {
    spot_talk_text_table = D_801203BC;
    spot_talk_command_table = D_801203D4;
    spot_talk_voice_table = D_801203EC;
    spot_ayako_still_pointers_3_init();
    func_80012D64(D_80121868, 0x11, 1, 2, 0);
    func_8004C46C(D_8012186C, D_80121870, D_80121874, D_80121878, D_8012187C, D_80121880);
    func_8004C6B0(D_8012185C, D_80121860, spot_ayako_still_pointers_3, D_80121864);
    func_800189C4(0x62);
    D_800EB029 = 9;
    D_800EB02A = 1;
    D_800EB060 = 0x41000000;
    D_800EB02B = 4;
    D_800EB034 = D_8012185C;
    D_800EB038 = D_80121860;
    D_800EB05C = spot_ayako_still_pointers_3;
    D_800EB03C = D_80121864;
    D_800EB03E = 4;
    D_800EB040 = 0;
    D_800EB02E = 1;
    D_800EB06B = 0x11;
    D_800EB04E = -0xA0;
    D_800EB052 = -0x78;
    D_800EB02F = 0;
    D_800EB02D = 0;
    D_800EB02C = 0;
    D_800B0A04[3].unk_02 -= 1;
    D_800B0A04[3].unk_06 -= 1;
    D_800B0A04[3].unk_0A += 0x14;
    func_8004C250();
    spot_reaction_tier_cells = 3;
    D_80124396 = 3;
    D_80124398 = 3;
    spot_reaction_low_cells = 2;
    D_801243A6 = 2;
    D_801243A8 = 2;
    func_80011DFC();
}

void spot_ayako_scene4_still_show_stats_03(void) {
    spot_talk_text_table = D_801203C0;
    spot_talk_command_table = D_801203D8;
    spot_talk_voice_table = D_801203F0;
    spot_ayako_still_pointers_4_init();
    func_80012D64(D_80121894, 0x11, 1, 2, 0);
    func_8004C46C(D_80121898, D_8012189C, D_801218A0, D_801218A4, D_801218A8, D_801218AC);
    func_8004C6B0(D_80121888, D_8012188C, D_80121884, D_80121890);
    D_800B0A04[3].unk_02 += 2;
    D_800B0A04[3].unk_06 += 1;
    D_800B0A04[3].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_ayako_scene5_still_show_03(void) {
    spot_talk_text_table = D_801203C4;
    spot_talk_command_table = D_801203DC;
    spot_talk_voice_table = D_801203F4;
    spot_ayako_still_pointers_5_init();
    func_80012D64(D_801218C0, 0x11, 1, 2, 0);
    func_8004C46C(D_801218C4, D_801218C8, D_801218CC, D_801218D0, D_801218D4, D_801218D8);
    func_8004C6B0(D_801218B4, D_801218B8, spot_ayako_still_pointers_5, (s32) D_801218BC);
    func_80011DFC();
}

void spot_ayako_scene1_bg_load_06(void) {
    func_800469F4(0x4015);
    func_80011DFC();
}

void spot_ayako_scene1_bg_load_18(void) {
    func_800469F4(0x462C);
    func_80011DFC();
}

void spot_ayako_scene2_bg_load_18(void) {
    func_800469F4(0x4862);
    func_80011DFC();
}

void spot_ayako_scene2_bg_load_32(void) {
    func_800469F4(0x458C);
    func_80011DFC();
}

void spot_ayako_scene3_bg_load_11(void) {
    func_800469F4(0x480C);
    func_80011DFC();
}

void spot_ayako_scene3_bg_load_28(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void spot_ayako_scene4_bg_load_10(void) {
    func_800469F4(0x479D);
    func_80011DFC();
}

void spot_ayako_scene4_bg_load_22(void) {
    func_800469F4(0x446A);
    func_80011DFC();
}

void spot_ayako_scene5_bg_load_10(void) {
    func_800469F4(0x4847);
    func_80011DFC();
}

void spot_ayako_scene5_bg_load_30(void) {
    func_800469F4(0x455F);
    func_80011DFC();
}

void spot_ayako_scene2_run(void) {
    s32 idx; /* FAKE: unused local declared before the table, the original frame has the table 4 bytes lower (T-3330) */
    FnTbl49 tbl;

    tbl = spot_ayako_scene2_steps;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6]();
    if (D_800B1AF6 == 0x17 || D_800B1AF6 == 0x18) {
        spot_ayako_scene2_sprites_update();
    }
}

s32 spot_ayako_scene2_step_23(void) {
    s16 i;
    s16 hit;
    s32 found;
    SpotLinePair ent;
    EvShorts82 tbl;

    ent = D_80121A2C;
    tbl = D_80121A34;
    if (((SpotFrameCounter *)&D_800B1AF0)->u == 0) {
        func_80015C34(ent.a, ent.b, 0xF);
        func_800433D0(0x71);
        func_800433D0(0x300);
        ((SpotFrameCounter *)&D_800B1AF0)->u += 1;
    } else if (func_80015A0C() == 9) {
        func_80011DFC();
        func_800433D0(0x202);
    } else if (((SpotDevelopmentState *)&D_800B0860)->pad_pressed & 0x400040) {
        func_80011DFC();
        func_800433D0(0xB4);
        func_800433D0(0x202);
    } else {
        found = 0;
        hit = 0;
        for (i = 0; i < 41; i++) {
            if (((SpotFrameCounter *)&D_800B1AF0)->u >= tbl.v[i][0] && tbl.v[i][1] >= ((SpotFrameCounter *)&D_800B1AF0)->u) {
                found = 1;
                if (i == 6) {
                    D_800EAFB8 = 2;
                    hit = 1;
                } else if (i == 0xC) {
                    D_800EAFB8 = 2;
                    hit = 1;
                } else if (i == 0x16) {
                    D_800EAFB8 = 0;
                    hit = 1;
                } else if (i == 0x17) {
                    D_800EAFB8 = 1;
                    hit = 1;
                } else if (i == 0x18) {
                    D_800EAFB8 = 2;
                    hit = 1;
                } else if (i == 0x19) {
                    D_800EAFB8 = 2;
                    hit = 1;
                } else if (i == 0x1A) {
                    D_800EAFB8 = 0;
                    hit = 1;
                } else if (i == 0x1D) {
                    D_800EAFB8 = 0;
                    hit = 1;
                } else if (i == 0x21) {
                    D_800EAFB8 = 0;
                    hit = 1;
                } else if (i == 0x24) {
                    D_800EAFB8 = 0;
                    hit = 1;
                } else if (i == 0x28) {
                    D_800EAFB8 = 0;
                    hit = 1;
                }
            }
        }
        if (found == 0) {
            D_800EAFB8 = 3;
        } else if (hit == 0) {
            spot_ayako_scene2_pick_maximum_input();
        }
    }
    if (func_80015A64() & 4) {
        ((SpotFrameCounter *)&D_800B1AF0)->u += 1;
    }
}

s32 spot_ayako_scene2_pick_maximum_input(void) {
    s16 i;
    s32 max;
    s32 max_idx;
    s32 avg;
    s32 q;

    if (func_80015A64() & 4) {
        func_800458E0();
    }
    max = D_800F19D0[0];
    max_idx = 0;
    if (max < D_800F19D0[1]) {
        max = D_800F19D0[1];
        max_idx = 1;
    }
    if (max < D_800F19D0[2]) {
        max = D_800F19D0[2];
        max_idx = 2;
    }
    if (max < D_800F19D0[3]) {
        max = D_800F19D0[3];
        max_idx = 3;
    }
    max = 0;
    for (i = 0; i < 4; i++) {
        if (i != max_idx) {
            /* FAKE: tautology in inner block gives max_idx a second loop-block reference so uopt assigns $a1 to max_idx and $a2 to &D_800F19D0. */
            if (max_idx == max_idx) {
            }
            if (max < D_800F19D0[i]) {
                max = D_800F19D0[i];
            }
        }
    }
    avg = (D_800F19D0[0] + D_800F19D0[1] + D_800F19D0[2] + D_800F19D0[3]) / 4;
    q = avg; /* FAKE: dead copy keeps avg in the pre-branch block so uopt does not sink the sum/divide into the if body. */
    if ((D_800EAFB8 == 0) || !(D_800B1AE4 & 3)) {
        if (avg == 0) {
            D_800EAFB8 = 3;
        } else {
            q = avg / 16;
            if (q == 0) {
                D_800EAFB8 = 3;
            } else if (q == 1) {
                D_800EAFB8 = 2;
            } else if (q == 2) {
                D_800EAFB8 = 1;
            } else if (q >= 3) {
                D_800EAFB8 = 0;
            }
        }
    }
}

s32 spot_ayako_scene2_sprites_update(void) {
    s16 i;
    s32 v;
    s32 j;
    u32 n;

    if (D_800B1AE4 % 51 == 0) {
        for (i = 0; i < 12; ) {
            D_801218DC[i] = func_800789A0() % 4;
            ++i;
        }
    }
    /* Legacy calls pass promoted ints; argument-site s16 casts add duplicate narrowing (T-24034). */
    n = 1000;
    /* FAKE: the dead copies `j = v` after each call keep `v` live across the call, so uopt promotes it to $s0 (with its
     * narrowing and the copy `move s0,tN`) and puts the divisor constants in $s1, as the original has; without them the
     * variable is folded away and `n` takes $s0. The original's real use of `v` after the calls is unknown. T-17204, T-24034 */
    v = (s16)((D_800B1AE4 * 16) % n);
    spot_wipe_gradient_draw(v, 0, 9);
    j = v;
    v = (s16)((D_800B1AE4 * 16) % n);
    spot_wipe_gradient_draw(v - 0xA, 1, 9);
    j = v;
    v = (s16)((D_800B1AE4 * 16) % n);
    spot_wipe_gradient_draw(v - 0x14, 2, 9);
    j = v;
    v = (s16)((D_800B1AE4 * 16 + 0x1F4) % n);
    spot_wipe_gradient_draw(v, 6, 9);
    j = v;
    v = (s16)((D_800B1AE4 * 16 + 0x1F4) % n);
    spot_wipe_gradient_draw(v - 0xA, 7, 9);
    j = v;
    v = (s16)((D_800B1AE4 * 16 + 0x1F4) % n);
    spot_wipe_gradient_draw(v - 0x14, 8, 9);
    j = v;
    func_8001A0CC(0, 0, 9, 3, 0);
    n = 0x320;
    v = (s16)(n - (D_800B1AE4 * 16) % n);
    spot_wipe_gradient_draw(v, 3, 0xD);
    j = v;
    v = (s16)(n - (D_800B1AE4 * 16) % n);
    spot_wipe_gradient_draw(v + 0xA, 4, 0xD);
    j = v;
    v = (s16)(n - (D_800B1AE4 * 16) % n);
    spot_wipe_gradient_draw(v + 0x14, 5, 0xD);
    j = v;
    v = (s16)(n - (D_800B1AE4 * 16 + 0x190) % n);
    spot_wipe_gradient_draw(v, 9, 0xD);
    j = v;
    v = (s16)(n - (D_800B1AE4 * 16 + 0x190) % n);
    spot_wipe_gradient_draw(v + 0xA, 0xA, 0xD);
    j = v;
    v = (s16)(n - (D_800B1AE4 * 16 + 0x190) % n);
    spot_wipe_gradient_draw(v + 0x14, 0xB, 0xD);
    j = v;
    func_8001A0CC(0, 0, 0xD, 3, 0);
}

void spot_wipe_gradient_draw(arg0, arg1, arg2)
    s16 arg0;
    s16 arg1;
    s16 arg2;
{
    /* Port of the DATE clone func_80152684 (T-17400). */
    /* FAKE: 8 unused bytes above tbl give the original's frame and tbl offset, as in the clone; real locals unknown. T-9310 */
    s32 pad[2];
    EvTbl tbl;

    tbl = D_80121AD8;
    if (arg0 < 0x140) {
        spot_gradient_tri_draw(0x140, -0x118, 0xA0 - arg0, 0x28, (0xA0 - arg0) + (s16)(arg0 / 32 + 0xA), 0x28, tbl.e[*(u8 *)&D_801218DC[arg1]].a, tbl.e[*(u8 *)&D_801218DC[arg1]].b, arg2, 1);
        return;
    }
    if (arg0 < 0x154) {
        spot_gradient_tri_draw(0x140, -0x118, -0xA0, 0x28 - (s16)((arg0 * 7 - 0x8C0) / 8), -0xA0, 0x28, tbl.e[*(u8 *)&D_801218DC[arg1]].a, tbl.e[*(u8 *)&D_801218DC[arg1]].b, arg2, 1);
        spot_gradient_tri_draw(0x140, -0x118, -0xA0, 0x28, (s16)(arg0 / 32 + 0xA) - arg0 + 0xA0, 0x28, tbl.e[*(u8 *)&D_801218DC[arg1]].a, tbl.e[*(u8 *)&D_801218DC[arg1]].b, arg2, 1);
        return;
    }
    spot_gradient_tri_draw(0x140, -0x118, -0xA0, 0x28 - (s16)((arg0 * 7 - 0x8C0) / 8), -0xA0, (0x28 - (s16)((arg0 * 7 - 0x8C0) / 8)) + (s16)(0xF - (arg0 - 0x140) / 32), tbl.e[*(u8 *)&D_801218DC[arg1]].a, tbl.e[*(u8 *)&D_801218DC[arg1]].b, arg2, 1);
}

void spot_ayako_scene2_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7F67);
    spot_ayako_still_pointers_2_init();
    func_80011DFC();
}

void spot_ayako_scene3_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl48 tbl;

    tbl = spot_ayako_scene3_steps;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800EECE0 != 0) {
        D_800EB02B |= 0x80;
        D_800EB02F = D_800F19AB;
    } else {
        D_800EB02B &= 0xFF7F;
    }
    if (D_800B1AF6 >= 0x10 && D_800B1AF6 < 0x13) {
        spot_bars_overlay_draw();
    }
}

void spot_ayako_scene3_assets_load_00(void) {
    func_80015D28(0x45, 0x801B0000, 0x7FA4);
    spot_ayako_still_pointers_3_init();
    func_80011DFC();
}

void spot_ayako_scene3_set_state_17(void) {
    D_800EB02A = 5;
    func_80011DFC();
}

void spot_ayako_scene4_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = spot_ayako_scene4_steps;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_ayako_scene4_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7FE9);
    spot_ayako_still_pointers_4_init();
    func_80011DFC();
}

void spot_ayako_scene5_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl45 tbl;

    tbl = spot_ayako_scene5_steps;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_ayako_scene5_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8026);
    spot_ayako_still_pointers_5_init();
    func_80011DFC();
}

void spot_ayako_scene5_special_case_17(void) {
    if (D_800EECBC == 1) {
        D_80094714 += 2;
        D_80094718 = 0;
        D_800B0A04[3].unk_02 += 2;
        D_800B0A04[3].unk_06 += 2;
        D_800B0A04[3].unk_0A -= 0x14;
        func_8004C250();
        func_80011DFC();
    } else {
        D_800B0A04[3].unk_02 += 3;
        D_800B0A04[3].unk_06 += 1;
        D_800B0A04[3].unk_0A -= 0x14;
        func_8004C250();
    }
    func_80011DFC();
}

void spot_ayako_scene5_branch_26(void) {
    if (D_800EECBC == 0) {
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void spot_ayako_scene5_expression_24(void) {
    if (D_800EECBC == 0) {
        func_8004A734();
        return;
    }
    func_8004A764();
}

s32 spot_gradient_tri_draw(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 c0, u32 c1, s32 z, s32 trans) {
    SpotDevelopmentState *g;
    POLY_G3 *poly;

    g = (SpotDevelopmentState *)D_800B0860;
    poly = func_80019D00(0x1C, D_800E9600);
    func_80067628(poly);
    func_800675A8(poly, trans);
    /* FAKE: explicit colour masks/comma lines preserve the matched DATE schedule (T-20600, T-24034). */
    poly->r0 = (c0 >> 16) & 0xFF, poly->g0 = (c0 >> 8) & 0xFF, poly->b0 = c0 & 0xFF;
    poly->r1 = (c1 >> 16) & 0xFF, poly->g1 = (c1 >> 8) & 0xFF, poly->b1 = c1 & 0xFF;
    poly->r2 = (c1 >> 16) & 0xFF, poly->g2 = (c1 >> 8) & 0xFF, poly->b2 = c1 & 0xFF;
    poly->x0 = g->screen_center_x[D_800E9600] + x0;
    poly->y0 = g->screen_center_y[D_800E9600] + y0;
    poly->x1 = g->screen_center_x[D_800E9600] + x1;
    poly->y1 = g->screen_center_y[D_800E9600] + y1;
    poly->x2 = g->screen_center_x[D_800E9600] + x2;
    poly->y2 = g->screen_center_y[D_800E9600] + y2;
    func_800674F4(&D_800B2580[(D_800E9600 << 6) + z * 4], poly);
}
