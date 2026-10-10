#include "common.h"
#include "ovl/NAME_ENT.h"

void func_80132000(s32 arg0) {
    s32 rem;

    rem = arg0 % 15;
    func_800AE090(D_8014CC74, D_8014CE6C + rem * 8, 8);
    func_800AE090(D_8014CC7C, D_8014CEE4 + rem * 8, 8);
    func_800AE090(D_8014CC84, D_8014CF5C + rem * 0xC, 0xC);
}

void func_8013209C(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80132134();
        break;
    case 1:
        func_80132E80();
        break;
    case 2:
        func_80138488();
        break;
    }
    func_800578F4(2);
    func_8004B19C(D_801220D0, 0, 0x50);
    func_8004B19C(D_801220F4, 0, 0x5A);
}

void func_80132134(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132198();
        return;
    case 1:
        func_80132320();
        return;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132198);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132320);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801324AC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132A38);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132B78);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132E80);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B140);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B150);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B15C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B168);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B174);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B180);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B18C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B198);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1AC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1B8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1C0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1CC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1E4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1FC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B208);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B214);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B220);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B22C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B238);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B244);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B250);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B25C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B268);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B270);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B27C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B284);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B290);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B29C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2A8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2B4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2C0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2CC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2E4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2FC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B308);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B314);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B320);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B32C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B338);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B344);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B350);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B35C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B368);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B374);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B380);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B38C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B398);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3B0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3BC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3C8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3D0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3E0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3E8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B400);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B410);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B420);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B430);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B440);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B448);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B450);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B458);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B460);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B468);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B470);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B478);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B480);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B484);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B48C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B494);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B49C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B4A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B4AC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132FD4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801330E8);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801332FC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80133510);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801338DC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80133D94);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80133E54);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80133FC4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80134110);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801341A8);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80134248);

void func_80134B18(void) {
    k_sub_reset();
    set_kanji_string(-0x68, -0x58, 0xE, D_8014CC74, 0);
    set_kanji_string(-0x30, -0x58, 0xE, D_8014CC7C, 0);
    set_kanji_string(0x48, -0x58, 0xE, D_8014CC84, 0);
    k_sub_disp_start(3);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80134B94);

void func_80134EB8(void) {
    if ((D_8014CCC0 >= 0) && (D_8014CCC4 >= 0)) {
        D_80120CD6 = D_8014CCC0 * 0x10 - 0x71;
        D_80120CDA = D_8014CCC4 * 0x10 - 0x29;
        return;
    }
    if ((D_8014CCB8 >= 0) && (D_8014CCBC >= 0) && (D_8014CCB8 % 6 != 5)) {
        D_80120CD6 = D_8014CCB8 * 0x10 - 0x71;
        D_80120CDA = D_8014CCBC * 0x10 + 0x3F;
        return;
    }
    D_80120CD6 = 0x100;
    D_80120CDA = 0x100;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80134F6C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135168);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135434);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135650);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801357F4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135A98);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135BB4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135E58);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135F74);

void func_8013621C(s32 arg0) {
    s32 color;

    if (arg0 != 0) {
        color = 0x90002;
    } else {
        color = 0x8081F;
    }
    func_80049A40(-0x9A, -0x6A, 0x130, 0x20, 4, color, 0x83);
    func_80049A40(-0x7C, -0x3D, 0x118, 0x9E, 4, color, 0x83);
    func_80049A40(-0x9D, 0x50, 0x1C, 0x10, 4, color, 0x83);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801362D0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_8013635C);

void func_8013664C(s32 arg0) {
    u8 *p = D_8011ECD0 + arg0 * 0x44;

    if (!(p[2] & 1)) {
        *(s16 *)(p + 0x18) = 0;
        p[2] = 1;
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80136688);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80136808);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801372A0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80137608);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80137708);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801377E8);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80137AA0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80137B84);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80137CA0);

void func_80137EE4(void) {
    u8 *p;
    s32 i;

    func_80048E78();
    for (i = 0; i < 2; i++) {
        p = D_8011ECD0 + i * 0x44;
        p[0x1981] = 2;
        p[0x1982] = 0x40;
        *(s32 *)(p + 0x19B8) = 0;
        p[0x1983] = 0x84;
        *(s32 *)(p + 0x198C) = D_800E6280.unk_1A4C;
        *(s32 *)(p + 0x1990) = D_800E6280.unk_1A5C;
        *(s32 *)(p + 0x19B4) = D_800E6280.unk_1A3C;
        p[0x1984] = 8;
        p[0x1985] = 0x81;
        p[0x19C3] = 0x10;
        *(s16 *)(p + 0x19A6) = 0x190;
    }
    D_80120668 = 2;
    D_80120676 = 0;
    D_8012067A = 0;
    D_801206AC = 6;
    D_801206BA = -0xA;
    D_801206BE = 0x58;
    D_80120695 = 3;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80137FC4);

void func_80138268(s32 arg0) {
    s32 color;

    if (arg0 != 0) {
        color = 0x90002;
    } else {
        color = 0x8081F;
    }
    if (arg0 != 0) {
        func_80049A40(-0x7E, -0x3D, 0x11A, 0x58, 4, color, 0x83);
    } else {
        func_80049A40(-0x7E, -0x3D, 0x11A, 0x76, 4, color, 0x83);
    }
    func_80049A40(-0x28, 0x3B, 0x7C, 0x26, 4, color, 0x83);
}

void func_80138330(void) {
}

void func_80138338(void) {
    s32 pad; /* FAKE: unused local, the original frame has the spill slots 4 bytes higher (T-3330) */
    s32 x;
    s32 y;

    x = D_8011ECF6;
    y = D_8011ECFA;
    func_8006BA40();
    if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
        D_801206BA = -0xA;
    }
    if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
        D_801206BA = 0x2E;
    }
    if (D_800E6280.unk_F88 & 0x800) {
        func_80044750(0x501);
        func_80042808();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    } else if (D_800E6280.unk_F88 & 0x20) {
        if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
            func_80044750(0x501);
            func_80042808();
        }
        if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
            func_80042940(0);
        }
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80138488);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801385A8);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80138634);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80138714);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801387F4);

void func_80138A7C(void) {
    s32 pad; /* FAKE: unused local, the original frame has the spill slots 4 bytes higher (T-3330) */
    s32 x;
    s32 y;

    x = D_8011ECF6;
    y = D_8011ECFA;
    func_8006BA40();
    if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
        D_801206BA = -0xA;
    }
    if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
        D_801206BA = 0x2E;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
            func_80044750(0x501);
            func_8004284C();
        }
        if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
            func_80042940(0);
        }
    } else if (D_800E6280.unk_F88 & 0x800) {
        func_80044750(0x501);
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    }
}

void func_80138BCC(void) {
    func_80044750(5);
    func_80044750(0xC1);
    func_80044750(3);
    func_80048E78();
    func_80138F60();
    func_8004E58C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80138C1C);

void func_80138F00(void) {
    D_800E6280.unk_1A3C = 0x801AAF58;
    D_800E6280.unk_1A4C = 0x801AAF5C;
    D_800E6280.unk_1A5C = 0x801AAFD0;
    D_800E6280.unk_1A6C = 0x80190000;
    D_800E6280.unk_1A84 = 0x801AA200;
    D_800E6280.unk_1A7C = 0x80192200;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80138F60);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80139440);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80139684);
