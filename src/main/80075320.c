#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075320);

void func_80075468(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
    default:
        func_800755B4();
        break;
    case 1:
        func_80075A64();
        break;
    }
    func_80066334();
    func_80065B0C(0);
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800754C0);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800755B4);

void func_80075A64(void) {
    if (func_800460CC() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            func_80042808();
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075AF8);

void func_80075BE8(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80075C24();
    }
    func_80066334();
    func_80065B0C(0);
}

void func_80075C24(void) {
    k_disp_inc();
    func_8006BA40();
    if (func_80076000() == 0 && func_80075C84() == 0 && func_8005B0F4() == 0) {
        func_8005B1A8();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075C84);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075FA0);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076000);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800764AC);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076630);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800767A4);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076B48);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076C7C);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076DA0);

void func_80076EC0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80076630();
        break;
    case 1:
        func_80076B48();
        break;
    case 2:
        func_80076C7C();
        break;
    case 3:
        func_80076DA0();
        break;
    }
    func_80066334();
}

void func_80076F48(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8007739C();
        break;
    case 1:
        func_80077694();
        break;
    case 2:
        func_80077900();
        break;
    }
    parameter_show();
    func_80066334();
    func_80065B0C(1);
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076FDC);

s32 func_80077330(void) {
    s32 r;

    if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3) {
        r = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    } else {
        r = dec_bg_cd_read(func_80075FA0(), 0);
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_8007739C);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077694);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077900);

void func_80077BE0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80077C50();
        break;
    case 1:
        func_80077E30();
        break;
    case 2:
        magazine_exit();
        break;
    }
    func_80066334();
}

void func_80077C50(void) {
    func_8006A044(0, 0);
    func_8004E58C();
    func_8006612C("情報誌");
    func_8006A2CC();
    icon_disp_switch(0);
    hizuke_disp_switch(0);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077CA8);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077E30);

void magazine_exit(void) {
    func_8006D138();
    hizuke_disp_switch(1);
    func_80042908(0);
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_tel);

void holiday_tel_init(void) {
    func_8004E58C();
    func_80048E78();
    func_80065F34(0);
    icon_disp_switch(0);
    telephone_class_init();
    k_sub_reset_point_set();
    gnsx(D_800E6280.unk_0D4);
    sndi((u8 *)"誰に電話かけようかな？）", 0, 0x1F);
    k_sub_disp_start(2);
    func_8004284C();
}

void holiday_tel_call(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(0);
        return;
    }
    if ((D_800E6280.unk_F88 & 0x20) && D_800E6280.unk_1093 != -1) {
        if (D_800E6280.unk_1093 == 0xD) {
            func_80042908(0);
            return;
        }
        func_80044750(0x50A);
        if (D_800E6280.unk_1093 == 0) {
            D_800E6280.unk_F5F = 0xD;
        } else if (D_800E6280.unk_1093 == 0xC) {
            D_800E6280.unk_F5F = 0xB;
        } else {
            D_800E6280.unk_F5F = D_800E6280.unk_1093 - 1;
        }
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_tel_exit);

INCLUDE_ASM("asm/nonmatchings/main/80075320", telephone_class_init);

void holiday_club_init(void) {
    func_80065F34(0);
    func_8004E58C();
    x_taku_string_set("クラブ活動をする", "クラブをやめる", 0, 2);
    x_taku_menu_set(1, 2);
    D_800E6280.unk_1094 = 0;
    k_disp_start(2);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_8007866C);

void holiday_club_exit(void) {
    func_80048E78();
    if (func_8005352C() == 1 && (u32)(D_800E6280.unk_040 + 6) / 7 == 3 && D_800E6280.unk_03F % 3U == 0 && D_800E6280.unk_041 == 6) {
        func_80042878(0x73);
        return;
    }
    func_80042908(5);
}

void holiday_taibu_club_exit(void) {
    func_80048E78();
    if (D_800E6280.unk_F88 & 0x60) {
        func_80042908(0);
    }
}

s32 holiday_club(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        holiday_club_init();
        break;
    case 1:
        func_8007866C();
        break;
    case 2:
        holiday_club_exit();
        break;
    case 3:
        holiday_taibu_club_exit();
        break;
    }
}

void holiday_club_join_exit(void) {
    func_80048E78();
    icon_disp_switch(1);
    func_80042908(5);
}

void func_8007894C(void) {
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
        holiday_club_join_exit();
        break;
    }
    cal_base_show();
}
