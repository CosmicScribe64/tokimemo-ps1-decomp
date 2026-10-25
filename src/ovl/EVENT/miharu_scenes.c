/* miharu_scenes.c: src/ovl/EVENT/8010A210.c:4: girl 11 (Miharu) scenes 1-5; also the shared bar overlay helper. Original object 0x8010A210-0x8010B360 (config/objects/EVENT.txt). */
#include "common.h"
#include "ovl/EVENT.h"

void spot_miharu_tile_pointers_1_init(void) {
    spot_miharu_tile_pointers_1 = 0x801B0000;
    D_80123E94 = 0x801B2000;
    D_80123E98 = 0x801B6000;
    D_80123E9C = 0x801BA000;
    D_80123EA0 = 0x801BE000;
    D_80123EA4 = 0x801C2000;
    D_80123EA8 = 0x801C6000;
}

void spot_miharu_still_pointers_1_init(void) {
    spot_miharu_still_pointers_1 = 0x801CE150;
    D_80123EB0 = 0x801CE154;
    D_80123EB4 = 0x801CE1A4;
    D_80123EB8 = *(s16 *)0x801CE1BC;
    D_80123EBC = 0x801B0000;
    D_80123EC0 = 0x801B2000;
    D_80123EC4 = 0x801B6000;
    D_80123EC8 = 0x801BA000;
    D_80123ECC = 0x801BE000;
    D_80123ED0 = 0x801C2000;
    D_80123ED4 = 0x801C6000;
}

void spot_miharu_scene_select(void) {
    switch (D_800EECB0) {
    case 1:
        spot_miharu_scene1_run();
        break;
    case 2:
        spot_miharu_scene2_run();
        break;
    case 3:
        spot_miharu_scene3_run();
        break;
    case 4:
        spot_miharu_scene4_run();
        break;
    case 5:
        spot_miharu_scene5_run();
        break;
    default:
        func_80015FE0();
        break;
    }
}

void spot_miharu_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_miharu_scene1_steps_run();
    } else {
        func_80015FE0();
    }
}

void spot_miharu_scene2_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_miharu_scene2_steps_run();
    } else {
        func_80015FE0();
    }
}

void spot_miharu_scene4_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_miharu_scene4_steps_run();
    } else {
        func_80015FE0();
    }
}

void spot_miharu_scene1_steps_run(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = spot_miharu_scene1_steps;
    func_80078950("s%d %d\n", D_800B1AF6, D_80094714);
    tbl.f[D_800B1AF6]();
}

void spot_miharu_scene1_se_play_09(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void spot_miharu_scene2_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = spot_miharu_scene2_steps;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void spot_miharu_scene2_assets_read_09(void) {
    func_80015D28(0, 0x801B0000, 0);
    func_80011DFC();
}

void spot_miharu_scene2_still_show_11(void) {
    func_80012D64(spot_miharu_tile_pointers_1, 0x11, 1, 2, 0);
    func_8004C46C(D_80123E94, D_80123E98, D_80123E9C, D_80123EA0, D_80123EA4, D_80123EA8);
    func_8004C6B0(0, 0, 0, 0);
    func_80011DFC();
}

/* FAKE: separate flag-word reads and buffer bit-field writes preserve the
 * original word reload after the byte store. The full SDK aggregate is still
 * unknown. T-9920. */
void spot_miharu_scene1_branch_15(void) {
    struct SeqFlags {
        unsigned int lo : 9;
        unsigned int count : 3;
        unsigned int hi : 20;
    };
    struct SeqBits {
        u8 pad[0x3EC];
        unsigned int lo : 9;
        unsigned int count : 3;
        unsigned int hi : 20;
    };
    struct SeqBits *seq;
    s32 value;

    seq = (struct SeqBits *)D_800B0860;
    value = ((struct SeqFlags *)(D_800B0860 + 0x3EC))->count;
    if (value >= 5U) {
        seq->count = 4;
        value = ((struct SeqFlags *)(D_800B0860 + 0x3EC))->count;
    }
    D_80094714 += value * 2;
    seq->count++;
    func_80011DFC();
}

void spot_miharu_scene2_branch_21(void) {
    u8 value;

    /* FAKE: byte alias retains the loaded address in V0. T-9770 */
    value = *(u8 *)&D_800B0E72;
    if (value >= 2U) {
        value = 1;
    }
    D_80094714 += value;
    /* FAKE: empty condition delays the final store address load. T-9770 */
    if ((value && value) && value) {
    }
    D_800B0E72 = value + 1;
    func_80011DFC();
}

void spot_miharu_shared_init_1(void) {
    D_800B1746 = 0xB;
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
    func_80078970(D_80094748, D_800B0929);
    func_80078970(D_80094764, "謎の女");
    func_8004CDDC();
    spot_script_sections_init();
    spot_talk_text_table = D_80120624;
    spot_talk_command_table = D_80120644;
    spot_talk_voice_table = *(s32 *)&D_80120664;
    func_80078970(D_80094784, "廊下");
    func_80011DFC();
}

void spot_miharu_scene2_init_01(void) {
    s16 var_v0;
    SpotDevelopmentState *g;

    D_800B1746 = 0xB;
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
    func_80078970(D_80094748, D_800B0929);
    func_8004CDDC();
    spot_talk_text_table = D_80120628;
    spot_talk_command_table = D_80120648;
    spot_talk_voice_table = *(s32 *)&D_80120668;
    if (D_800EECB0 == 1) {
        D_800B096E -= 5;
        g = (SpotDevelopmentState *)D_800B0860;
        for (var_v0 = 0; var_v0 < 0xA; var_v0++) {
            if (((SpotLegacyGirlFlagBits *)&g->girls[var_v0].unk_0C)->bit1) {
                g->girls[var_v0].unk_02 -= 1;
            }
        }
        func_8004C250();
    }
    func_80011DFC();
}

void spot_miharu_shared_set_state_1(void) {
    spot_talk_text_table = D_8012062C;
    spot_talk_command_table = D_8012064C;
    spot_talk_voice_table = D_8012066C;
    func_80011DFC();
}

void spot_miharu_scene4_init_00(void) {
    D_800B1746 = 0xB;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_800201FC(1);
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_80078970(D_80094748, D_800B0929);
    func_80078970(D_80094764, "館林");
    func_80078970(D_8009476C, "館林さん");
    func_8004CDDC();
    spot_script_sections_init();
    spot_talk_text_table = D_80120630;
    spot_talk_command_table = D_80120650;
    spot_talk_voice_table = D_80120670;
    func_80078970(D_80094784, "きらめき中央公園");
    func_80078970(D_800947C4, "並木道");
    func_80011DFC();
}

void spot_miharu_scene4_still_show_07(void) {
    spot_miharu_still_pointers_1_init();
    func_80012D64(D_80123EBC, 0x11, 1, 2, 0);
    func_8004C46C(D_80123EC0, D_80123EC4, D_80123EC8, D_80123ECC, D_80123ED0, D_80123ED4);
    func_8004C6B0(D_80123EB0, D_80123EB4, spot_miharu_still_pointers_1, (s32) D_80123EB8);
    ((Actor *)D_800EAFA0)->unk_04 = ((Actor *)D_800EAFA0)[1].unk_04 = 8;
    func_80011DFC();
}

void spot_miharu_scene1_bg_load_01(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void spot_miharu_scene2_bg_load_02(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void spot_miharu_scene2_bg_load_14(void) {
    func_800469F4(0x4678);
    func_80011DFC();
}

void spot_miharu_scene3_run(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = spot_miharu_scene3_steps;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void spot_miharu_scene4_bg_load_09(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void spot_miharu_scene4_bg_load_26(void) {
    func_800469F4(0x4296);
    func_80011DFC();
}

void spot_miharu_scene4_bg_load_38(void) {
    func_800469F4(0x473D);
    func_80011DFC();
}

void spot_miharu_scene4_steps_run(void) {
    u8 index;
    FnTbl56 table;

    table = spot_miharu_scene4_steps;
    index = D_800B1AF6;
    table.f[index](0x80);
    index = D_800B1AF6;
    if (index >= 0x1E && index < 0x25) {
        func_8003C2C0();
    } else if (index == 0x2B) {
        spot_bars_overlay_draw();
    }
}

void spot_miharu_scene4_assets_load_03(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8BD8);
    spot_miharu_still_pointers_1_init();
    func_80011DFC();
}

void spot_miharu_scene4_sound_bank_load_01(void) {
    func_800143DC(1, 0xB19B, 0xB178, 0xBE66, 0xBE24, 0xBE21);
    func_80011DFC();
}

void spot_miharu_shared_portrait_read_1(void) {
    func_800335E0(0x692F);
    func_80011DFC();
}

void spot_bars_overlay_draw(void) {
    func_8001A1EC(0x120, -0xF8, 0xA0, 0x3C, -0xA0, 0x3C, 9, 0x808080, 0);
    func_8001A1EC(0x120, -0xF8, 0x40, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0);
    func_8001A1EC(0x120, -0xF8, 0x10, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x40);
    func_8001A1EC(0x120, -0xF8, -0x1C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x63);
    func_8001A1EC(0x120, -0xF8, -0x50, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x24);
    func_8001A1EC(0x120, -0xF8, -0x8C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x73);
    func_8001A1EC(0x120, -0xF8, -0xF0, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x11);
    func_8001A1EC(0x120, -0xF8, -0x140, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x50);
    func_8001A1EC(0x120, -0xF8, -0x30, 0x3C, 0, 0xB4, 9, 0x808080, 0);
    func_8001A0CC(0, 0, 9, 1, 0);
}

void spot_miharu_scene4_special_case_32(void) {
    spot_talk_step();
    D_80094714 = (u16) D_80094714 + 1;
    D_80094718 = 0;
    func_80011DFC();
}

void spot_miharu_scene4_step_35(void) {
    func_800201FC(1);
    D_800EECC0 = 0;
    D_800EECCC = 0;
    func_80011DFC();
}

void spot_miharu_scene5_run(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = spot_miharu_scene5_steps;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void spot_miharu_scene5_portrait_read_01(void) {
    func_800335E0(0x692F);
    func_80011DFC();
}
