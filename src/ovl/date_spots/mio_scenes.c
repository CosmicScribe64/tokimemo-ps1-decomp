/* mio_scenes.c: src/ovl/EVENT/801011A0.c:4: girl 1 (Mio) scenes 1-5. Original object 0x801011A0-0x801024B0 (config/objects/EVENT.txt). */
#include "common.h"
#include "ovl/date_spots.h"

void spot_mio_still_pointers_1_init(void) {
    spot_mio_still_pointers_1 = 0x801CE0EC;
    D_801221D4 = 0x801CE0F0;
    D_801221D8 = 0x801CE110;
    D_801221DC = *(s16 *)0x801CE124;
    D_801221E0 = 0x801B0000;
    D_801221E4 = 0x801B2000;
    D_801221E8 = 0x801B6000;
    D_801221EC = 0x801BA000;
    D_801221F0 = 0x801BE000;
    D_801221F4 = 0x801C2000;
    D_801221F8 = 0x801C6000;
}

void spot_mio_still_pointers_2_init(void) {
    spot_mio_still_pointers_2 = 0x801D2230;
    D_80122200 = 0x801D2238;
    D_80122204 = 0x801D2278;
    D_80122208 = *(s16 *)0x801D2290;
    D_8012220C = 0x801B0000;
    D_80122210 = 0x801B2000;
    D_80122214 = 0x801B6000;
    D_80122218 = 0x801BA000;
    D_8012221C = 0x801BE000;
    D_80122220 = 0x801C2000;
    D_80122224 = 0x801C6000;
}

void spot_mio_still_pointers_3_init(void) {
    spot_mio_still_pointers_3 = 0x801D21D0;
    D_8012222C = 0x801D21D8;
    D_80122230 = 0x801D2218;
    D_80122234 = *(s16 *)0x801D2230;
    D_80122238 = 0x801B0000;
    D_8012223C = 0x801B2000;
    D_80122240 = 0x801B6000;
    D_80122244 = 0x801BA000;
    D_80122248 = 0x801BE000;
    D_8012224C = 0x801C2000;
    D_80122250 = 0x801C6000;
}

void spot_mio_still_pointers_4_init(void) {
    spot_mio_still_pointers_4 = 0x801CE074;
    D_80122258 = 0x801CE078;
    D_8012225C = 0x801CE088;
    D_80122260 = *(s16 *)0x801CE094;
    D_80122264 = 0x801B0000;
    D_80122268 = 0x801B2000;
    D_8012226C = 0x801B6000;
    D_80122270 = 0x801BA000;
    D_80122274 = 0x801BE000;
    D_80122278 = 0x801C2000;
    D_8012227C = 0x801C6000;
}

void spot_mio_still_pointers_5_init(void) {
    spot_mio_still_pointers_5 = 0x801CE148;
    D_80122284 = 0x801CE14C;
    D_80122288 = 0x801CE170;
    D_8012228C = *(s16 *)0x801CE184;
    D_80122290 = 0x801B0000;
    D_80122294 = 0x801B2000;
    D_80122298 = 0x801B6000;
    D_8012229C = 0x801BA000;
    D_801222A0 = 0x801BE000;
    D_801222A4 = 0x801C2000;
    D_801222A8 = 0x801C6000;
}

void spot_mio_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_mio_scene1_run();
        break;
    case 2:
        spot_mio_scene2_run();
        break;
    case 3:
        spot_mio_scene3_run();
        break;
    case 4:
        spot_mio_scene4_run();
        break;
    case 5:
        spot_mio_scene5_run();
        break;
    default:
        func_80015FE0();
        break;
    }
}

void spot_mio_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_mio_scene1_steps_run();
        return;
    }
    func_80015FE0();
}

void spot_mio_scene1_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotShioriYuinaMioAndGameCenterStepTable tbl;

    tbl = spot_mio_scene1_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_mio_scene1_assets_load_03(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8287);
    spot_mio_still_pointers_1_init();
    func_80011DFC();
}

void spot_mio_scene1_init_00(void) {
    D_800B1746 = 1;
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
    spot_talk_text_table = D_80120330;
    spot_talk_command_table = D_80120344;
    spot_talk_voice_table = D_80120358;
    func_80078970(D_80094784, "廊下");
    D_800B0A04[1].unk_02 += 1;
    D_800B0A04[1].unk_06 += 2;
    func_8004C250();
    func_80011DFC();
}

void spot_mio_scene1_still_show_05(void) {
    spot_mio_still_pointers_1_init();
    func_80012D64(D_801221E0, 0x11, 1, 2, 0);
    func_8004C46C(D_801221E4, D_801221E8, D_801221EC, D_801221F0, D_801221F4, D_801221F8);
    func_8004C6B0(D_801221D4, D_801221D8, spot_mio_still_pointers_1, (s32) D_801221DC);
    func_80011DFC();
}

void spot_mio_scene2_set_state_01(void) {
    spot_ride = SPOT_RIDE_SCREAM_RIDE;
    spot_talk_text_table = D_80120334;
    spot_talk_command_table = D_80120348;
    spot_talk_voice_table = D_8012035C;
    func_80011DFC();
}

void spot_mio_scene2_still_show_23(void) {
    spot_mio_still_pointers_2_init();
    func_80012D64(D_8012220C, 0x11, 1, 2, 0);
    func_8004C46C(D_80122210, D_80122214, D_80122218, D_8012221C, D_80122220, D_80122224);
    func_8004C6B0(D_80122200, D_80122204, spot_mio_still_pointers_2, (s32) D_80122208);
    func_80011DFC();
}

void spot_mio_scene3_still_show_stats_03(void) {
    spot_talk_text_table = D_80120338;
    spot_talk_command_table = D_8012034C;
    spot_talk_voice_table = D_80120360;
    spot_mio_still_pointers_3_init();
    func_80012D64(D_80122238, 0x11, 1, 2, 0);
    func_8004C46C(D_8012223C, D_80122240, D_80122244, D_80122248, D_8012224C, D_80122250);
    func_8004C6B0(D_8012222C, D_80122230, spot_mio_still_pointers_3, D_80122234);
    D_800B0A04[1].unk_02 += 1;
    D_800B0A04[1].unk_06 += 2;
    D_800B0A04[1].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_mio_scene4_still_show_stats_03(void) {
    spot_talk_text_table = D_8012033C;
    spot_talk_command_table = D_80120350;
    spot_talk_voice_table = D_80120364;
    spot_mio_still_pointers_4_init();
    func_80012D64(D_80122264, 0x11, 1, 2, 0);
    func_8004C46C(D_80122268, D_8012226C, D_80122270, D_80122274, D_80122278, D_8012227C);
    func_8004C6B0(D_80122258, D_8012225C, spot_mio_still_pointers_4, D_80122260);
    D_800EAFD8 |= 0x80000000;
    D_800EAFB6 = 0;
    D_800B0A04[1].unk_02 += 2;
    D_800B0A04[1].unk_06 += 2;
    D_800B0A04[1].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_mio_scene5_still_show_stats_03(void) {
    spot_talk_text_table = D_80120340;
    spot_talk_command_table = D_80120354;
    spot_talk_voice_table = D_80120368;
    func_80012D64(D_80122290, 0x11, 1, 2, 0);
    func_80012D64(D_80122290, 0x12, 1, 2, 1);
    func_8004C46C(D_80122294, D_80122298, D_8012229C, D_801222A0, D_801222A4, D_801222A8);
    func_8004C6B0(D_80122284, D_80122288, spot_mio_still_pointers_5, D_8012228C);
    D_800B0A04[1].unk_02 += 3;
    D_800B0A04[1].unk_06 += 2;
    D_800B0A04[1].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_mio_scene1_bg_load_17(void) {
    func_800469F4(0x4657);
    func_80011DFC();
}

void spot_mio_scene1_bg_load_06(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void spot_mio_scene2_bg_load_03(void) {
    func_800469F4(0x42D6);
    func_80011DFC();
}

void spot_mio_scene2_bg_load_26(void) {
    func_800469F4(0x474F);
    func_80011DFC();
}

void spot_mio_scene2_bg_load_48(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void spot_mio_scene3_bg_load_10(void) {
    func_800469F4(0x4804);
    func_80011DFC();
}

void spot_mio_scene3_bg_load_20(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void spot_mio_scene4_bg_load_05(void) {
    func_800469F4(0x44C7);
    func_80011DFC();
}

void spot_mio_scene4_bg_load_14(void) {
    func_800469F4(0x47BD);
    func_80011DFC();
}

void spot_mio_scene4_bg_load_27(void) {
    func_800469F4(0x44BD);
    func_80011DFC();
}

void spot_mio_scene5_bg_load_09(void) {
    func_800469F4(0x4779);
    func_80011DFC();
}

void spot_mio_scene5_bg_load_17(void) {
    func_800469F4(0x4376);
    func_80011DFC();
}

void spot_mio_scene5_bg_load_24(void) {
    func_800469F4(0x436D);
    func_80011DFC();
}

void spot_mio_scene2_run(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    SpotMioScene2StepTable tbl;

    tbl = spot_mio_scene2_steps;
    func_80078950(" %d %d\n", D_800B1AF6, (&spot_reaction_low_cells)[D_800EECBC]);
    tbl.f[D_800B1AF6](0x80);
}

void spot_mio_unused_assets_load_1(void) {
    func_80015D28(0x45, 0x801B0000, 0x82C4);
    spot_mio_still_pointers_2_init();
    func_80011DFC();
}

void spot_mio_scene2_face_step_15(void) {
    if (D_800EECBC == 1) {
        func_8004A8EC(2);
        func_80033E88();
        func_80011EC4(0x2E);
        return;
    }
    D_800B0E31 = 1;
    spot_reaction_tier_cells = 3;
    D_80124396 = 3;
    D_80124398 = 3;
    spot_reaction_low_cells = 2;
    D_801243A6 = 2;
    D_801243A8 = 2;
    D_800B0A04[1].unk_02 -= 2;
    D_800B0A04[1].unk_06 -= 1;
    D_800B0A04[1].unk_0A += 0x14;
    func_8004C250();
    func_80011DFC();
}

void spot_mio_scene2_label_set_08(void) {
    func_80078970(D_800947C4, "絶叫マシーンビビール");
    func_80011DFC();
}

void spot_mio_scene3_run(void) {
    s32 idx;
    SpotShioriYuinaMioAndGameCenterStepTable tbl;

    tbl = spot_mio_scene3_steps;
    func_80078950(" %d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800B1AF6 >= 0xF && D_800B1AF6 < 0x12) {
        spot_bars_overlay_draw();
    }
}

void spot_mio_scene3_assets_load_00(void) {
    func_80015D28(0x45, 0x801B0000, 0x8309);
    spot_mio_still_pointers_3_init();
    func_80011DFC();
}

void spot_mio_scene4_run(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    SpotMioScene4StepTable tbl;

    tbl = spot_mio_scene4_steps;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void spot_mio_scene4_assets_load_00(void) {
    func_80015D28(0x3D, 0x801B0000, 0x834E);
    spot_mio_still_pointers_4_init();
    func_80011DFC();
}

void spot_mio_scene5_run(void) {
    /* FAKE: unused local reserves the 4 bytes above tbl in the 0x108 frame (T-3330 layout); T-24105 */
    s32 pad;
    s16 i;
    SpotMioScene5StepTable tbl;

    tbl = spot_mio_scene5_steps;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    tbl.f[D_800B1AF6](0x80);
    if ((D_800B1AF6 == 0xE) && !(D_800B1AE4 & 1)) {
        if (D_800F19AB == 0x80) {
            D_800F19AB = 0x60;
        } else {
            D_800F19AB = 0x80;
        }
        ((Actor *)D_800E9620)[96].brightness = D_800F19AB;
        ((Actor *)D_800E9620)[97].brightness = D_800F19AB;
        for (i = 0; i < 6; i++) {
            ((SpriteQuad *)D_800EC190)[i + 2].tint_r = ((SpriteQuad *)D_800EC190)[i + 2].tint_g = ((SpriteQuad *)D_800EC190)[i + 2].tint_b = D_800F19AB;
        }
    }
}

void spot_mio_scene5_assets_load_01(void) {
    func_80015D28(0x3D, 0x801B0000, 0x838B);
    spot_mio_still_pointers_5_init();
    func_80011DFC();
}

void spot_mio_scene5_branch_35(void) {
    if (D_800EECBC != 0) {
        D_80094714 += 1;
    } else {
        D_800B0A04[1].unk_0C.b14 = 1;
    }
    func_80011DFC();
}

void spot_mio_scene2_step_18(void) {
    spot_ride_movie_run();
}
