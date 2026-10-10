#include "common.h"
#include "ovl/NAME_ENT.h"

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80139740);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_801398EC);

void func_80139AD8(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;

    p = (u8 *)D_801217D0 + arg0 * 36;
    p[0x14] = 0x80;
    p[0x15] = 0x80;
    p[0x16] = 0x80;
    *(s16 *)(p + 4) = arg1;
    *(s16 *)(p + 6) = arg2;
    *(s32 *)p = 0;
    *(s16 *)(p + 8) = 0x10;
    *(s16 *)(p + 0xA) = 0x10;
    p[0xE] = arg0 / 8 * 0x30 + D_800E7378 / 32 % 3 * 0x10 + 0x80;
    p[0xF] = (arg0 & 7) * 0x10;
    *(s16 *)(p + 0xC) = 0x1F;
    *(s16 *)(p + 0x10) = arg0 * 0x10;
    *(s16 *)(p + 0x18) = 8;
    *(s16 *)(p + 0x1A) = 8;
    *(s16 *)(p + 0x12) = 0x1F8;
    (&D_800E7325)[arg0] = 2;
}

void func_80139BA0(void) {
    if (D_800E7208 & 0x10) {
        D_8014D0D4 += 1;
        return;
    }
    if ((D_800E7208 & 0x20) && (D_8011ECF6 < 0x20) && (D_8011ECF6 >= -0x3F) && (D_8011ECFA < 0x1C) && (D_8011ECFA >= 0xD)) {
        D_8014D0D4 += 1;
    }
}

void func_80139C24(void) {
    if (D_8014D0D4 < 0) {
        D_8014D0D4 += 3;
    }
    switch (D_8014D0D4 % 3) {
    case 0:
        func_80139F30();
        break;
    case 1:
        func_8013ACA4();
        break;
    case 2:
        func_8013A9C8();
        break;
    }
    func_80139BA0();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80139CB4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80139F30);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013A9C8);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013ACA4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013AF78);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013B0A0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013B1D8);

void func_8013B51C(s32 arg0) {
    s16 x;
    s16 y;

    x = (arg0 / 7) * 0x1F - 0x82;
    *(s16 *)&D_8011ECD0[0x26] = x;
    y = (arg0 % 7) * 0x10 - 0xB;
    *(s16 *)&D_8011ECD0[0x2A] = y;
    D_8011ECD0[0x1F9F] |= 0x80;
    *(s16 *)&D_8011ECD0[0x1FC2] = x;
    *(s16 *)&D_8011ECD0[0x1FC6] = y;
}

void func_8013B59C(void) {
    func_8004E58C();
    func_80139CB4();
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    D_8014D0D4 = 1;
    func_8013B51C(D_800E7398 - 1);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013B5F0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013B77C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013B994);

void func_8013BB10(void) {
    D_8014D0A8 = 0;
}

void func_8013BB1C(void) {
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013BB24);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013BE18);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013BF74);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C4A8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B7B4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B7C8);

void func_8013C510(void) {
    func_80056284();
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014B7B4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014B7C8, 0);
    k_disp_start(2);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C580);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B824);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B838);

void func_8013C664(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014B824, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014B838, 0);
    k_disp_start(2);
    func_8004284C();
}

void func_8013C6CC(void) {
    if (D_800E7208 & 0x860) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C6FC);

void func_8013C7C0(void) {
    if (D_800E7208 & 0x800) {
        func_8013B77C();
        func_80042878(0x14);
    } else if (D_800E7208 & 0x20) {
        func_8013B77C();
        func_80042878(0x14);
    } else if (D_800E7208 & 0x40) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C834);

void func_8013C8FC(void) {
    if (D_800E7208 & 0x20) {
        func_80056284();
        func_80042940(0);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C934);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C9F0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013CB44);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013CEC8);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013D054);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013D2AC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013D4CC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013D62C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013D6C0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B998);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B9A4);

void func_8013D7F4(void) {
    if (func_80055AFC(0) == 1) {
        func_8013BB10();
        func_800AE0B0(D_8014B998);
        func_80053D10();
        func_80044750(0x501);
        func_80042908(3);
    } else {
        func_800AE0B0(D_8014B9A4);
        func_80056284();
        func_80042908(1);
    }
    func_8013B1D8();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013D874);

void func_8013D954(void) {
    if (D_800E7208 & 0x20) {
        func_80042940(0x44);
    } else if (D_800E7208 & 0x40) {
        func_8004284C();
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA28);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA44);

void func_8013D9A0(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BA28, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BA44, 0);
    k_disp_start(2);
    func_8004284C();
}

void func_8013DA08(void) {
    if (D_800E7208 & 0x820) {
        func_8013B77C();
        func_80042878(0x14);
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA60);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA78);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA88);

void func_8013DA40(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BA60, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BA78, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BA88, 0);
    k_disp_start(3);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013DAC4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAA4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAC4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAE0);

void func_8013DBD0(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BAA4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BAC4, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BAE0, 0);
    k_disp_start(3);
    func_8004284C();
}

void func_8013DC54(void) {
    if (D_800E7208 & 0x860) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013DC84);

void func_8013DD48(void) {
    if (D_800E7208 & 0x820) {
        func_80042940(0x4C);
    } else if (D_800E7208 & 0x40) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013DD94);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013DF88);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BB8C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBA8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBC0);

void func_8013E30C(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BB8C, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BBA8, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BBC0, 0);
    k_disp_start(3);
    func_8004284C();
}

void func_8013E390(void) {
    if (D_800E7208 & 0x860) {
        func_8004284C();
    }
    func_80142880();
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBD4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBEC);

void func_8013E3C8(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BBD4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BBEC, 0);
    k_disp_start(2);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013E430);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013E59C);

void func_8013E6A8(void) {
    if (D_800E7208 & 0x40) {
        func_80044750(0x501);
        func_80042940(0);
    } else if (D_800E7208 & 0x20) {
        if (D_800E8BEE) {
            func_80042940(0xA6);
        } else {
            func_80042940(0xA7);
        }
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013E71C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013E80C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013E9FC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013EBEC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013EE7C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCB4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCCC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCE0);

void func_8013F058(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BCB4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BCCC, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BCE0, 0);
    k_disp_start(3);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F0DC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F208);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F454);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F5B0);

void func_8013F6BC(void) {
    if (D_800E7208 & 0x20) {
        func_80044750(0x501);
        func_8004284C();
    } else if (D_800E7208 & 0x40) {
        func_80042940(0);
        D_800E738D = 2;
    }
}

void func_8013F71C(void) {
    if (D_800E8BEE) {
        func_80042940(0xC7);
    } else {
        func_80042940(0xC8);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F758);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F984);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BDE8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BE00);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BE10);

void func_8013FB08(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BDE8, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BE00, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BE10, 0);
    k_disp_start(3);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013FB8C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013FCBC);

void func_8013FDC8(void) {
    if (D_800E7208 & 0x20) {
        func_80044750(0x501);
        if (D_800E8BEE) {
            func_80042940(0xE4);
        } else {
            func_80042940(0xE5);
        }
    } else if (D_800E7208 & 0x40) {
        func_80042940(0);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013FE40);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013FF98);

void func_80140024(void) {
    func_80042940(0);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140044);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140430);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140574);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8014068C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140B20);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140BA4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140CCC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BEB0);

void func_80140D20(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BEB0, 0);
    k_disp_start(2);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140D6C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140E18);

void func_80140E9C(void) {
    if (D_800E7208 & 0x860) {
        func_80042908(1);
        func_80042940(6);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140ED4);

void func_80140F54(void) {
    if (D_800E8BEE) {
        func_80042940(1);
    } else {
        func_80042940(2);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140F90);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80141820);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80141FA4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80142028);

void func_801422F0(void) {
    if (D_800E738A == 0) {
        func_80142028();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8014231C);

void func_80142560(void) {
    if ((D_800E7389 | D_800E738A) != 0) {
        D_801230D0 -= 1;
    }
    if (D_801230D0 > 0) {
        func_8004AC18(D_801230D0);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_801425C0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80142880);

s32 func_801429BC(void) {
    if (D_800E7208 & 0x20) {
        if (D_8011ECF6 < -0x50 && D_8011ECF6 >= -0x8F) {
            if (D_8011ECFA < -0x18 && D_8011ECFA >= -0x27) {
                func_80044750(0x50F);
                func_80042908(0xD0);
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80142A3C);
