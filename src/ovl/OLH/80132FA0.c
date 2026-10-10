#include "common.h"
#include "ovl/OLH.h"

void func_80132FA0(void) {
    switch (D_800E738A) {
    case 0x0:
        func_8013306C();
        break;
    case 0x1:
        func_801331C8();
        break;
    case 0x10:
        func_801332F8();
        break;
    case 0x20:
        func_801333E4();
        break;
    case 0x30:
        func_80133508();
        break;
    case 0x40:
        func_8013362C();
        break;
    case 0x50:
        func_8013376C();
        break;
    case 0x60:
        func_801338C8();
        break;
    case 0x70:
        func_80133A24();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_8013306C);

void func_801331C8(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E7208 & 0x20) {
        if (D_800E7313 != -1) {
            func_80044750(0x501);
            switch (*(u8 *)&D_800E7313) {
            case 0:
                func_80042940(0x10);
                break;
            case 1:
                func_80042940(0x20);
                break;
            case 2:
                func_80042940(0x30);
                break;
            case 3:
                func_80042940(0x40);
                break;
            case 4:
                func_80042940(0x50);
                break;
            case 5:
                func_80042940(0x60);
                break;
            case 6:
                func_80042940(0x70);
                break;
            case 7:
                func_80042908(0);
                break;
            }
        }
    } else if (D_800E7208 & 0x40) {
        func_80042908(0);
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_801332F8);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_801333E4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_80133508);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_8013362C);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_8013376C);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_801338C8);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_80133A24);
