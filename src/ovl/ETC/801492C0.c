#include "common.h"
#include "ovl/ETC.h"

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_801492C0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149394);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149468);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_801495CC);

s32 func_8014979C(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E7208 & 0x20) {
        switch (D_800E7313) {
        case 0:
            if (D_800E6377 != 0) {
                func_80042908(2);
            } else {
                func_80042908(1);
            }
            break;
        case 1:
            if (D_800E6377 != 0) {
                func_80042908(1);
            } else {
                func_80042908(2);
            }
            break;
        case 2:
            func_80042908(3);
            break;
        }
    }
}

void func_8014988C(void) {
    if (D_8011F553 & 0x80) {
        parameter_show();
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_801498BC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149928);

s32 func_80149A08(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E7208 & 0x20) {
        switch (D_800E7313) {
        case 0:
            D_80150E98 = 0;
            func_8004284C();
            break;
        case 1:
            D_80150E98 = 1;
            func_8004284C();
            break;
        case 2:
            D_80150E98 = 2;
            func_8004284C();
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149AC4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149BC0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149D3C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149DD8);

void func_80149E68(void) {
    func_80048F64(0x60);
    D_80120650[1] = 1;
    D_80120650[2] = 4;
    *(s32 *)&D_80120650[0x38] = 0x01000000;
    D_80120650[3] = 0x84;
    *(u8 **)&D_80120650[0xC] = D_80125CA8;
    *(u8 **)&D_80120650[0x10] = D_80125CAC;
    *(u8 **)&D_80120650[0x34] = D_80125CB0;
    D_80120650[5] = 2;
    D_80120650[6] = 1;
    D_80120650[0x43] = 0x10;
    *(s16 *)&D_80120650[0x16] = 0;
    *(s16 *)&D_80120650[0x26] = -0x50;
    *(s16 *)&D_80120650[0x2A] = -0x40;
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149F48);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A180);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A258);

s32 func_8014A2C4(void) {
    switch (D_80150E9C) {
    case 10:
        return 0;
    case 8:
    case 9:
        return 1;
    case 7:
        return 2;
    case 5:
    case 6:
        return 3;
    case 3:
    case 4:
        return 4;
    case 1:
    case 2:
    default:
        return 5;
    }
}

s32 func_8014A32C(void) {
    if (D_800E6392 + D_800E6396 > 0x100) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A360);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A534);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A6D4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A7A4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A8A8);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A9A4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014AA74);

void func_8014AB5C(void) {
    if (D_800E738A == 0) {
        func_8014AA74();
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014AB88);
