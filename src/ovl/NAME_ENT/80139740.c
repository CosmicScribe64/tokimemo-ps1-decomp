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
    p[0xE] = arg0 / 8 * 0x30 + D_800E6280.unk_10F8 / 32 % 3 * 0x10 + 0x80;
    p[0xF] = (arg0 & 7) * 0x10;
    *(s16 *)(p + 0xC) = 0x1F;
    *(s16 *)(p + 0x10) = arg0 * 0x10;
    *(s16 *)(p + 0x18) = 8;
    *(s16 *)(p + 0x1A) = 8;
    *(s16 *)(p + 0x12) = 0x1F8;
    D_800E6280.unk_10A5[arg0] = 2;
}

void func_80139BA0(void) {
    if (D_800E6280.unk_F88 & 0x10) {
        D_8014D0D4 += 1;
        return;
    }
    if ((D_800E6280.unk_F88 & 0x20) && (D_8011ECF6 < 0x20) && (D_8011ECF6 >= -0x3F) && (D_8011ECFA < 0x1C) && (D_8011ECFA >= 0xD)) {
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
    func_8013B51C(D_800E6280.unk_1118 - 1);
}

void func_8013B5F0(void) {
    if ((D_800E6280.unk_1109 != 0 || D_800E6280.unk_110A != 0) && D_8014D0E0 < 0x4001) {
        D_8014D0E0 = (D_8014D0E0 * 16 + 0x70) / 15;
        D_8014D0E4 += D_8014D0EC;
        D_8014D0E8 += D_8014D0F0;
        if (D_8014D0E4 >= 0xA1) {
            D_8014D0EC = -(func_800AE0D0() & 0xF);
        }
        if (D_8014D0E8 >= 0x79) {
            D_8014D0F0 = -(func_800AE0D0() & 0xF);
        }
        if (D_8014D0E4 < -0xA0) {
            D_8014D0EC = func_800AE0D0() & 0xF;
        }
        if (D_8014D0E8 < -0x78) {
            D_8014D0F0 = func_800AE0D0() & 0xF;
        }
    }
    func_80046AC8(0x1A, 0, 0x1F7, D_8014D0E0, D_8014D0E4, D_8014D0E8, 0x01DF7FCF, 2);
}

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

void func_8013C4A8(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013BB24();
        return;
    case 1:
        func_8013BE18();
        return;
    case 2:
        func_8013BF74();
        return;
    }
}
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
    if (D_800E6280.unk_F88 & 0x860) {
        func_8004284C();
    }
}

void func_8013C6FC(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "新たにゲームを始めますか？", 0);
    func_8004E788(-0x40, 0x30, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x30, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x40, 2, func_8006CAE0(), 0);
    func_8004E884(5);
    func_8004284C();
}

void func_8013C7C0(void) {
    if (D_800E6280.unk_F88 & 0x800) {
        func_8013B77C();
        func_80042878(0x14);
    } else if (D_800E6280.unk_F88 & 0x20) {
        func_8013B77C();
        func_80042878(0x14);
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

void func_8013C834(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "メモリーカードを", 0);
    func_8004E788(-0x40, 0x30, 1, "差し込み口１", 0);
    func_8004E788(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
    func_8004E788(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
    func_8004E788(-0x40, 0x40, 1, func_8006CA9C(), 0);
    func_80053D10();
    func_8004E884(5);
    func_8004284C();
}

void func_8013C8FC(void) {
    if (D_800E6280.unk_F88 & 0x20) {
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

void func_8013D62C(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    func_800AE100(buf, "番アルバムを");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "ロードしています。", 0);
    func_8004E884(2);
    func_8004284C();
}

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

void func_8013D874(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "このゲームのデータがありません", 0);
    func_8004E788(-0x40, 0x30, 0xF, "新たにアルバムを作成し、", 0);
    func_8004E788(-0x40, 0x40, 0xF, "ゲームをプレイしますか？", 0);
    func_8004E788(-0x40, 0x50, 0xF, "　　はい…　　いいえ…　", 0);
    func_8004E788(6, 0x50, 1, func_8006CA9C(), 0);
    func_8004E788(0x5A, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}

void func_8013D954(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x44);
    } else if (D_800E6280.unk_F88 & 0x40) {
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
    if (D_800E6280.unk_F88 & 0x820) {
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
    if (D_800E6280.unk_F88 & 0x860) {
        func_8004284C();
    }
}

void func_8013DC84(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "システムファイルを作成しますか", 0);
    func_8004E788(-0x40, 0x30, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x30, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x40, 2, func_8006CAE0(), 0);
    func_8004E884(5);
    func_8004284C();
}

void func_8013DD48(void) {
    if (D_800E6280.unk_F88 & 0x820) {
        func_80042940(0x4C);
    } else if (D_800E6280.unk_F88 & 0x40) {
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
    if (D_800E6280.unk_F88 & 0x860) {
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
    if (D_800E6280.unk_F88 & 0x40) {
        func_80044750(0x501);
        func_80042940(0);
    } else if (D_800E6280.unk_F88 & 0x20) {
        if (D_800E8BEE) {
            func_80042940(0xA6);
        } else {
            func_80042940(0xA7);
        }
    }
}

/* Packed date word: day in bits 0-3, month in bits 4-8, week in bits 9-10. */
typedef struct NameDate {
    u32 day : 4;
    u32 month : 5;
    u32 week : 2;
    u32 rest : 21;
} NameDate;

void func_8013E71C(void) {
    func_800AE0A0(D_8014D070, &D_800E6280.unk_0D4, 8);
    func_800AE0A0(D_8014D078, &D_800E6280.unk_0DC, 8);
    func_800AE0A0(D_8014D080, &D_800E6280.unk_0E4, 0xC);
    D_8014D08C = ((NameDate *)&D_800E6280.unk_0F8)->day - 1;
    D_8014D090 = ((NameDate *)&D_800E6280.unk_0F8)->month - 1;
    D_8014D094 = (((NameDate *)&D_800E6280.unk_0F8)->week + 3) & 3;
    D_8014D098 = ((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->day - 1;
    D_8014D09C = ((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->month - 1;
    D_8014D0A0 = (((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->week + 3) & 3;
    D_8014D0A4 = 1;
}

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
    if (D_800E6280.unk_F88 & 0x20) {
        func_80044750(0x501);
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        D_800E6280.unk_110D = 2;
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
    if (D_800E6280.unk_F88 & 0x20) {
        func_80044750(0x501);
        if (D_800E8BEE) {
            func_80042940(0xE4);
        } else {
            func_80042940(0xE5);
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013FE40);

void func_8013FF98(void) {
    if (D_800E6280.unk_110D == 0) {
        if (func_80054704(D_800E6280.unk_1118) == 1) {
            func_800AE0B0("DELETE OK");
            D_800E7D14[D_800E6280.unk_1118] = 0;
            func_80042940(0);
            D_800E6280.unk_110D = 2;
            return;
        }
        func_80053D10();
        func_80056284();
        func_80042908(1);
    }
}

void func_80140024(void) {
    func_80042940(0);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140044);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140430);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140574);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8014068C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140B20);

void func_80140BA4(void) {
    if (D_800E6280.unk_110D++ >= 0x41U) {
        func_8013B77C();
        func_80057390(0);
        if (D_800E6280.unk_03E == 0x62 && D_800E6280.unk_03F == 3 && D_800E6280.unk_040 == 1) {
            func_8013E71C();
            func_80041878();
            func_80041F48();
            D_800E6280.unk_F5E = 0;
            func_80042878(0x14);
        } else if (D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 4 && D_800E6280.unk_040 == 4) {
            func_8013E71C();
            func_80041878();
            func_80041F48();
            D_800E6280.unk_F5E = 0;
            func_80042878(0x14);
        } else {
            func_800674B0();
            func_80042878(0x34);
            func_80042908(0xFF);
        }
    }
    D_8011ECA4 = 1;
}

s32 func_80140CCC(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014068C();
        break;
    case 1:
        func_80140BA4();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BEB0);

void func_80140D20(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BEB0, 0);
    k_disp_start(2);
    func_8004284C();
}

/* NON_MATCHING: T-8050, the original loads the end symbol address twice (hoisted copy in the loop, fresh one after it) */
#ifdef NON_MATCHING
void func_80140D6C(void) {
    u8 *p;

    if (D_800E6280.unk_F88 & 0x860) {
        for (p = (u8 *)&D_800E7D10; p != &D_800E7D1F; p++) {
            if (p[4] == 1) {
                break;
            }
        }
        if (p == &D_800E7D1F) {
            func_8004284C();
            return;
        }
        func_80042908(2);
        if (D_800E8BEE != 0) {
            func_80042940(1);
            return;
        }
        func_80042940(0x20);
    }
}
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140D6C);
#endif

void func_80140E18(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "別のメモリーカードを差し込み口", 0);
    func_8004E788(-0x40, 0x30, 0xF, "１にセットするか、他のゲームの", 0);
    func_8004E788(-0x40, 0x40, 0xF, "データを消去してください。", 0);
    func_8004E884(2);
    func_8004284C();
}

void func_80140E9C(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_80042908(1);
        func_80042940(6);
    }
}

void func_80140ED4(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80140D20();
        return;
    case 1:
        func_80140D6C();
        return;
    case 2:
        func_80140E18();
        return;
    case 3:
        func_80140E9C();
        return;
    }
}

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
    if (D_800E6280.unk_110A == 0) {
        func_80142028();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8014231C);

void func_80142560(void) {
    if ((D_800E6280.unk_1109 | D_800E6280.unk_110A) != 0) {
        D_801230D0 -= 1;
    }
    if (D_801230D0 > 0) {
        func_8004AC18(D_801230D0);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_801425C0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80142880);

s32 func_801429BC(void) {
    if (D_800E6280.unk_F88 & 0x20) {
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
