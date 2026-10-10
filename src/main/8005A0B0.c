#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A0B0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A1A0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A2A8);

void func_8005A37C(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048E78();
    D_800E7322 = 0;
    D_800E739A = 0;
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

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A560);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A668);

void func_8005AB4C(void) {
    s32 r;

    r = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    switch (r) {
    case 0:
    case 1:
        dec_bg_show_set(0, r);
        set_dec_bri(0x80);
        D_800E62BA = 0x80;
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
            D_800E62BA = 0x80;
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
    if ((D_800E6375 & 0xF) == 3) {
        D_801217D0 = 0x01000000;
        D_801217D4 = -0xC;
        D_801217D6 = -0xA;
        D_801217D8 = 0x1E;
        D_801217DA = 0x18;
        D_801217DC = 0x1E;
        D_801217DE = 0xE0;
        D_801217DF = 0x60;
        D_801217E0 = 0;
        D_801217E2 = 0x1EF;
        D_801217E4 = 0x80;
        D_801217E5 = 0x80;
        D_801217E6 = 0x80;
        D_801217E8 = 0;
        D_801217EA = 0;
        D_801217EC = 0x1000;
        D_801217EE = 0x1000;
        D_801217F0 = 0;
        D_800E7325 = 0xA;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005AE60);

s32 func_8005AF28(void) {
    if (D_8011ECF6 < 0x22 && D_8011ECF6 >= -0xD && D_8011ECFA < 8 && D_8011ECFA >= -0xB) {
        if (D_800E7208 & 0x20) {
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
        if (D_800E7208 & 0x20) {
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

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B798);

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
    if (D_800E7208 & 0x40) {
        func_8004284C();
    }
}

void func_8005B8E0(void) {
    func_80042908(0);
    func_80042940(2);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B908);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005BAE0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005BB84);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005BC38);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005BCEC);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005BE8C);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C14C);

void func_8005C414(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E7208 & 0x40) {
        func_8005C4CC(1);
        func_80042908(0);
        func_80042940(2);
        return;
    }
    if (D_800E7208 & 0x20) {
        switch (D_800E7314) {
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
        memcpy(D_80125C10, D_800E78BC, 0x40);
        D_80125C50 = D_800E793C;
        D_80125C52 = D_800E7940;
        D_800B5C08 = D_800E7944;
        D_80125C54 = D_800E7948;
        D_80125C55 = D_800E794C;
        return;
    }
    memcpy(D_800E78BC, D_80125C10, 0x40);
    D_800E793C = D_80125C50;
    D_800E7940 = D_80125C52;
    *(s32 *)&D_800E7944 = D_800B5C08; /* read as u8 above, stored as a word */
    D_800E7948 = D_80125C54;
    D_800E794C = D_80125C55;
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C5BC);

void func_8005C7C0(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B00FC, 0);
    k_sub_disp_start(1);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C7FC);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C968);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005CA94);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005CC40);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005CF6C);

void func_8005D174(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B0194, 0);
    k_sub_disp_start(1);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D1B0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D31C);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D448);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D56C);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D804);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005D9B4);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005DC4C);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005DDB4);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005DEA0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E018);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E150);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E2C0);

void func_8005E40C(void) {
    k_disp_inc();
    if (D_800E7208 & 0x20) {
        func_80042940(0);
        D_800E7313 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E454);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E5C8);

void func_8005E7A4(void) {
    k_disp_inc();
    if (D_800E7208 & 0x20) {
        func_80042940(0);
        D_800E7313 = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005E7F0);

void func_8005E924(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E7208 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E7208 & 0x20) {
        switch (D_800E7314) {
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
    if (D_800E7208 & 0x20) {
        func_80042940(0);
        D_800E7313 = 2;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005EA38);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005EB74);

void func_8005EC78(void) {
    k_disp_inc();
    if (D_800E7208 & 0x20) {
        func_80042940(0x30);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005ECB8);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005EE00);

void func_8005EF04(void) {
    k_disp_inc();
    if (D_800E7208 & 0x20) {
        func_80042940(0x30);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005EF44);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F09C);

void func_8005F22C(void) {
    k_disp_inc();
    if (D_800E7208 & 0x20) {
        func_80042940(0x30);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F26C);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F478);

void func_8005F6E4(void) {
    k_disp_inc();
    if (D_800E7208 & 0x20) {
        func_80042940(0);
        D_800E7313 = 3;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F730);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005FA20);

void func_8005FDA0(void) {
    k_disp_inc();
    if (D_800E7208 & 0x20) {
        func_80042940(0);
        D_800E7313 = 4;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005FDEC);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005FF68);

void func_80060104(void) {
    k_disp_inc();
    if (D_800E7208 & 0x20) {
        func_80042940(0);
        D_800E7313 = 5;
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
    D_800E7313 = 6;
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", join_club);

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

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80060958);

void func_80060A84(void) {
    if (func_800460CC() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            D_800E62BA = 0x80;
            func_80042908(1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80060B24);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80060B78);

void uwasa_exit0(void) {
    func_80042878(0x31);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", uwasa0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", uwasa_main);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", pre_xmas_init);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", pre_xmas);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80060DF8);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80060EA0);

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

void pre_syogatu_init3(void) {
    func_8004E58C();
    func_8006612C(D_800B0B64);
    k_sub_reset_point_set();
    sndisp(D_800B0B6C, 0, 0xFF);
    func_80044750(0x603);
    k_disp_start(6);
    func_80042808();
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", pre_syogatu_init);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", pre_syogatu0);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", pre_syogatu1);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80061634);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80061688);
