#include "common.h"
#include "ovl/OLH.h"

void func_80132420(void) {
    switch (D_800E738A) {
    case 0x0:
        func_801324EC();
        break;
    case 0x1:
        func_80132614();
        break;
    case 0x10:
        func_80132744();
        break;
    case 0x20:
        func_80132884();
        break;
    case 0x30:
        func_80132954();
        break;
    case 0x40:
        func_80132AE8();
        break;
    case 0x50:
        func_80132BD4();
        break;
    case 0x60:
        func_80132CDC();
        break;
    case 0x70:
        func_80132E54();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_801324EC);

void func_80132614(void) {
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

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132744);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132884);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132954);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132AE8);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132BD4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132CDC);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132E54);
