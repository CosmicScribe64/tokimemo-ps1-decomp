#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A0B0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A1A0);

void func_8005A2A8(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005A37C();
        break;
    case 1:
        func_8005A3E8();
        break;
    case 2:
        func_8005A668();
        break;
    case 3:
        func_8005ABD0();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    func_80065B0C(0);
    func_8006BA40();
    func_80066334();
    func_800578F4(get_last_gamen_mode());
}

void func_8005A37C(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048E78();
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_111A = 0;
    set_dec_bri(0);
    func_8004E58C();
    k_disp_start(2);
    func_8004284C();
}

void func_8005A3E8(void) {
    k_disp_inc();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A410);

void func_8005A560(void) {
    if (D_800E6280.unk_110D == 0) {
        if (func_80044E8C() == 1) {
            func_8004500C(0, 0);
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        func_80046318(0x95U, 0x801A0000, 0x9B94);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 2) {
        if (func_800460CC() & 1) {
            func_80068938(D_800E6280.unk_03E, D_800E6280.unk_03F, 0);
            D_800E6280.unk_1092 &= 3;
            D_800B5BC8 = 0xFE;
            if (D_800B5C08 < 0xE) {
                func_800676AC(D_800B5C08);
                func_8006764C(1);
            }
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A668);

void func_8005AB4C(void) {
    s32 r;

    r = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    switch (r) {
    case 0:
    case 1:
        dec_bg_show_set(0, r);
        set_dec_bri(0x80);
        D_800E6280.unk_03A = 0x80;
        func_80042808();
        break;
    case -1:
        func_8004284C();
        break;
    }
}

void func_8005ABD0(void) {
    if (func_800460CC() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            D_800E6280.unk_03A = 0x80;
            func_80042908(1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005AC70);

void func_8005AD1C(void) {
    LoadSquare(0x3F0, 0x160, 0x10, 0x18, D_800C9A60);
    load_palette(D_800C9D60, 0xF, 1, 1, 0);
}

void func_8005AD70(void) {
    if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3) {
        D_801217D0[0].unk_00 = 0x01000000;
        D_801217D0[0].unk_04 = -0xC;
        D_801217D0[0].unk_06 = -0xA;
        D_801217D0[0].unk_08 = 0x1E;
        D_801217D0[0].unk_0A = 0x18;
        D_801217D0[0].unk_0C = 0x1E;
        D_801217D0[0].unk_0E = 0xE0;
        D_801217D0[0].unk_0F = 0x60;
        D_801217D0[0].unk_10 = 0;
        D_801217D0[0].unk_12 = 0x1EF;
        D_801217D0[0].unk_14 = 0x80;
        D_801217D0[0].unk_15 = 0x80;
        D_801217D0[0].unk_16 = 0x80;
        D_801217D0[0].unk_18 = 0;
        D_801217D0[0].unk_1A = 0;
        D_801217D0[0].unk_1C = 0x1000;
        D_801217D0[0].unk_1E = 0x1000;
        D_801217D0[0].unk_20 = 0;
        D_800E6280.unk_10A5[0] = 0xA;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005AE60);

s32 func_8005AF28(void) {
    if (D_8011ECF6 < 0x22 && D_8011ECF6 >= -0xD && D_8011ECFA < 8 && D_8011ECFA >= -0xB) {
        if (D_800E6280.unk_F88 & 0x20) {
            func_80044750(0xB1);
            func_80044774(0);
            func_80046290(0x1D000185, 0x17000397, 0);
            func_80044750(0x300);
            return 1;
        }
    }
    return 0;
}

s32 func_8005AFC8(void) {
    if (D_8011ECF6 < 0x50 && D_8011ECF6 >= 0x41 && D_8011ECFA < -0x10 && D_8011ECFA >= -0x1F) {
        if (D_800E6280.unk_F88 & 0x20) {
            func_80042878(0x3A);
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B040);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B0F4);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B1A8);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B2BC);

void func_8005B39C(void) {
    k_disp_inc();
    parameter_show();
    hizuke_show();
    message_window_show();
    func_80065B0C(0);
    func_8006BA40();
    func_80066334();
    func_800578F4(0);
    if (func_8005B43C() == 0 && func_8005B2BC() == 0 && func_8005B040() == 0 && func_8005AF28() == 0) {
        func_8005AFC8();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B43C);

void func_8005B798(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005B830();
        break;
    case 1:
        func_8005B8A0();
        break;
    case 2:
        func_8005B8E0();
        break;
    }
    func_80068EC0();
    parameter_show();
    hizuke_show();
    message_window_show();
    func_80066334();
    func_8006BA40();
}
INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800AFF6C);

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800AFF78);

void func_8005B830(void) {
    func_8004E58C();
    func_8006612C(D_800AFF6C);
    set_kanji_string(-0x80, 0x32, 0, D_800AFF78, 0);
    k_disp_start(2);
    cal_sprite_init();
    func_80067E34();
    icon_disp_switch(0);
    func_8004284C();
}

void func_8005B8A0(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

void func_8005B8E0(void) {
    func_80042908(0);
    func_80042940(2);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B908);

s32 func_8005BAE0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044750(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "データが壊れています", 0);
        k_sub_disp_start(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            func_80042940(6);
        }
        break;
    }
}

s32 func_8005BB84(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044750(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードを認識できません", 0);
        k_sub_disp_start(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            func_8005C4CC(1);
            func_80042908(0);
            func_80042940(2);
        }
        break;
    }
}

s32 func_8005BC38(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044750(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードに空きがありません", 0);
        k_sub_disp_start(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            func_8005C4CC(1);
            func_80042908(0);
            func_80042940(2);
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005BCEC);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005BE8C);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C14C);

void func_8005C414(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x40) {
        func_8005C4CC(1);
        func_80042908(0);
        func_80042940(2);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            func_80042940(3);
            return;
        case 1:
            func_80042940(6);
            break;
        }
    }
}

void func_8005C4CC(s32 arg0) {
    if (arg0 != 0) {
        memcpy(D_80125C10, D_800E6280.unk_163C, 0x40);
        D_80125C50 = D_800E6280.unk_163C[32];
        D_80125C52 = D_800E6280.unk_163C[33];
        D_800B5C08 = *(u8 *)&D_800E6280.unk_163C[34]; /* the low byte of the word stored below */
        D_80125C54 = D_800E6280.unk_163C[35];
        D_80125C55 = D_800E6280.unk_163C[36];
        return;
    }
    memcpy(D_800E6280.unk_163C, D_80125C10, 0x40);
    D_800E6280.unk_163C[32] = D_80125C50;
    D_800E6280.unk_163C[33] = D_80125C52;
    D_800E6280.unk_163C[34] = D_800B5C08;
    D_800E6280.unk_163C[35] = D_80125C54;
    D_800E6280.unk_163C[36] = D_80125C55;
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C5BC);

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800B00FC);

void func_8005C7C0(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B00FC, 0);
    k_sub_disp_start(1);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C7FC);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C968);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005CA94);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005CC40);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005CF6C);

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800B0194);

void func_8005D174(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B0194, 0);
    k_sub_disp_start(1);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D1B0);

void func_8005D31C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xD0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_800696DC(0, 1);
            func_80042940(7);
            func_8005D174();
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042940(0xFF);
        }
        break;
    }
    func_8005D1B0();
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D448);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D56C);

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original; the s32 return keeps the switch temporary copy in $v0. T-5010 */
s32 func_8005D804(void) {
    switch (*(u8 *)&D_800E6280.unk_110D) {
    default:
        D_800E6280.unk_110D += 1;
        break;
    case 0:
        if (D_800E6280.unk_03E == 0x62 && D_800E6280.unk_03F == 3 && D_800E6280.unk_040 == 1) {
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 4 && D_800E6280.unk_040 == 4) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80044750(0xB1);
            D_800E6280.unk_110D = 0xC0;
        }
        break;
    case 1:
        func_80044750(5);
        func_80044750(0xB1);
        func_80044890(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1);
        D_800E6280.unk_110D += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_80041878();
            func_80041F48();
            D_800E6280.unk_F5E = 0;
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            message_disp_switch(0);
            func_8006764C(0);
            func_8004500C(0, 0);
            func_80042878(0x14);
        }
        break;
    case 0xFF:
        func_80065F34(0);
        func_80042878(0x34);
        func_80042908(0xFF);
        func_800674B0();
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D9B4);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005DC4C);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005DDB4);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005DEA0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E018);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E150);

void func_8005E2C0(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを遅くしました", 0);
            k_speed_set(3);
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを普通にしました", 0);
            k_speed_set(1);
            func_8004284C();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを速くしました", 0);
            k_speed_set(0);
            func_8004284C();
            break;
        }
    }
}

void func_8005E40C(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E454);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E5C8);

void func_8005E7A4(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E7F0);

void func_8005E924(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            func_80042940(0x33);
            return;
        case 1:
            func_80042940(0x36);
            return;
        case 2:
            func_80042940(0x39);
            break;
        }
    }
}

void func_8005E9EC(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 2;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005EA38);

void func_8005EB74(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "右ボタンで決定にしました", 0);
            D_800E6280.unk_F75 = 1;
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "左ボタンで決定にしました", 0);
            D_800E6280.unk_F75 = 0;
            func_8004284C();
            break;
        }
    }
}

void func_8005EC78(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x30);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005ECB8);

void func_8005EE00(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "シングルクリックにしました", 0);
            D_800E6280.unk_F70 = 0;
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "ダブルクリックにしました", 0);
            D_800E6280.unk_F70 = 1;
            func_8004284C();
            break;
        }
    }
}

void func_8005EF04(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x30);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005EF44);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F09C);

void func_8005F22C(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x30);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F26C);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F478);

void func_8005F6E4(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 3;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F730);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005FA20);

void func_8005FDA0(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 4;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005FDEC);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005FF68);

void func_80060104(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 5;
    }
}

void func_80060150(void) {
    func_80042908(0);
    func_80042940(2);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80060178);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_800603C0);

void func_80060504(void) {
    func_80042940(0);
    D_800E6280.unk_1093 = 6;
}

void join_club(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        join_club_init();
        break;
    case 1:
        join_club_select();
        break;
    case 2:
        join_club_message();
        break;
    case 3:
        join_club_exit();
        break;
    }
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    func_8006BA40();
}

void join_club_init(void) {
    func_80065F34(0);
    icon_disp_switch(0);
    func_8004E58C();
    func_8006B648();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", join_club_select);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", join_club_message);

void join_club_exit(void) {
    func_80048E78();
    func_80042878(0x30);
}

void func_80060958(void) {
    s32 r;

    func_8004E58C();
    func_8006612C("自宅");
    gnsx(D_800E6280.unk_0D4);
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    func_8006BC28(0);
    func_8006509C();
    icon_disp_switch(1);
    func_8006CE84();
    icon_can_use_set(0, 0);
    icon_can_use_set(D_800E6280.unk_F68.w & 0xF, 1);
    tpage_buf_clear();
    sndisp("最近女の子の間で 変な噂が流れているらしい…）", 0, 0x1F);
    k_disp_start(1);
    r = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    if (r != -1) {
        if (r == 0 || r == 1) {
            dec_bg_show_set(0, r);
            set_dec_bri(0x80);
            func_80042808();
        }
    } else {
        func_8004284C();
    }
    k_disp_start(2);
}

void func_80060A84(void) {
    if (func_800460CC() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            D_800E6280.unk_03A = 0x80;
            func_80042908(1);
        }
    }
}

s32 func_80060B24(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80060958();
        break;
    case 1:
        func_80060A84();
        break;
    }
}

void func_80060B78(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        xa_wait();
        return;
    default:
    case 1:
        func_8004284C();
        return;
    }
}
void uwasa_exit0(void) {
    func_80042878(0x31);
}

s32 uwasa0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80060B78();
        break;
    case 1:
        uwasa_exit0();
        break;
    }
}

void uwasa_main(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80060B24();
        break;
    case 1:
        uwasa0();
        break;
    }
    func_80065B0C(1);
    parameter_show();
    hizuke_show();
    message_window_show();
    func_80066334();
    func_800578F4(0);
    func_80066C08(1);
}
s32 pre_xmas_init(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8004E58C();
        func_80048E78();
        func_80041584();
        func_80066104("自宅");
        gnsx(D_800E6280.unk_0D4);
        parameter_show_init();
        parameter_show_init();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(1);
        icon_disp_switch(0);
        tpage_buf_clear();
        sndisp("今日は伊集院の家で、 クリスマスパーティだ。 会場に行こうかな）", 0, 0x1F);
        k_disp_start(6);
        func_8006BC28(0);
        func_8005AB4C();
        return;
    case 1:
        func_8005ABD0();
        return;
    }
}

s32 pre_xmas(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        xa_wait();
        break;
    case 1:
        func_80042878(0x52);
        break;
    }
}

void func_80060DF8(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        pre_xmas_init();
        break;
    case 1:
        pre_xmas();
        break;
    }
    parameter_show();
    hizuke_show();
    message_window_show();
    func_8006BA40();
    func_80066334();
    if (*D_8011F3FF & 0x80) {
        func_80067870();
    }
    func_800578F4(0);
    func_80066C08(1);
}

s32 func_80060EA0(void) {
    typedef struct { u32 pad0 : 1; u32 flag : 1; u32 rest : 30; } Bits;
    s32 pad[2]; /* FAKE: unused, gives the 0x48 frame and the n/tbl slots. T-7020 */
    u8 tbl[12];
    s32 i;
    s32 n;

    n = 0;
    get_h_yuukou_table(tbl);
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 11; i++) if (tbl[i] >= 0x4DU && ((Bits *) &D_800E6280.unk_1BC[i].unk_0C)->flag) n++;
    return n;
}

void func_80061044(void) {
    if (func_80060EA0() != 0) {
        set_dec_bri(0);
        func_8004E58C();
        func_80057640(0xA0);
        func_80048E78();
        func_80041584();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(0);
        func_8006764C(0);
        func_80044750(0xC1);
        icon_disp_switch(0);
        tpage_buf_clear();
        func_8006BC28(0);
        func_80044890(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
        func_8004284C();
        return;
    }
    set_dec_bri(0);
    func_80042878(0x53);
}

void pre_syogatu_init1(void) {
    s32 t;

    if (func_80044E8C() == 1) {
        t = dec_bg_cd_read(0x4167, 1);
        if (t != -1) {
            if (t == 0 || t == 1) {
                dec_bg_show_set(1, t);
                set_dec_bri(0x80);
                func_8004284C();
                func_8004284C();
            }
        } else {
            func_8004284C();
        }
    }
    func_80057710();
}

void pre_syogatu_init2(void) {
    if (func_800460CC() & 1) {
        dec_bg_show_set(1, func_8005751C(1));
        set_dec_bri(0x80);
        func_8004284C();
    }
}

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800B0B64);

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800B0B6C);

void pre_syogatu_init3(void) {
    func_8004E58C();
    func_8006612C(D_800B0B64);
    k_sub_reset_point_set();
    sndisp(D_800B0B6C, 0, 0xFF);
    func_80044750(0x603);
    k_disp_start(6);
    func_80042808();
}

s32 pre_syogatu_init(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80061044();
        break;
    case 1:
        pre_syogatu_init1();
        break;
    case 2:
        pre_syogatu_init2();
        break;
    case 3:
        pre_syogatu_init3();
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", pre_syogatu0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", pre_syogatu1);

s32 func_80061634(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        pre_syogatu0();
        break;
    case 1:
        pre_syogatu1();
        break;
    }
}

void func_80061688(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        pre_syogatu_init();
        break;
    case 1:
        func_80061634();
        break;
    }
    hizuke_show();
    message_window_show();
    func_8006BA40();
    func_80066334();
    func_800578F4(1);
    func_80066C08(1);
}