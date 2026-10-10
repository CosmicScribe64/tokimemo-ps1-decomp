#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137A60", func_80137A60);

void func_80137B14(s32 arg0) {
    func_80048F64(1);
    D_8011ED15 = 8;
    D_8011ED4C = 0;
    D_8011ED16 = 0x40;
    D_8011ED17 = 0x84;
    D_8011ED20 = D_800E7CCC;
    D_8011ED24 = D_800E7CDC;
    D_8011ED48 = D_800E7CBC;
    D_8011ED18 = 8;
    D_8011ED19 = 0x81;
    D_8011ED57 = 0xF;
    D_8011ED2C = arg0;
    D_8011ED3A = -0xAA;
    D_8011ED3E = 0x78;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137A60", func_80137BD8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137A60", func_80137CBC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137A60", func_80137D00);

void func_80137E48(void) {
    switch (D_800E738D) {
    case 0:
        if (D_8015EDEC < 8) {
            func_80137CBC(D_8015EDEC);
            D_8015EDEC += 1;
            D_800E738D += 1;
        } else {
            func_8004284C();
        }
        break;
    case 1:
        if (func_800460EC() & 4) {
            D_800E738D += 1;
        }
        break;
    case 2:
        if (func_800460EC() & 2) {
            D_800E738D = 0;
        }
        break;
    }
    func_800578F4(2);
    if ((D_800E7378 & 3) == 3) {
        D_8011ED3E -= 1;
    }
}
void func_80137F54(void) {
    func_80137CBC(8);
    func_80044750(0xB1);
    func_80042908(2);
}
