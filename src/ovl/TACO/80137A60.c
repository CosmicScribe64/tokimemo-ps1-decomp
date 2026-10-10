#include "common.h"
#include "ovl/TACO.h"

/* No return value on purpose: implicit-int function, see func_80137BD8. T-6070 */
s32 func_80137A60(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80137BD8();
        break;
    case 1:
        func_80137D00();
        break;
    case 2:
        func_80137E48();
        break;
    case 3:
        func_80137F54();
        break;
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_80044750(0x74);
        func_80044750(0xB1);
        func_80042908(2);
    }
}

void func_80137B14(s32 arg0) {
    func_80048F64(1);
    D_8011ED15 = 8;
    D_8011ED4C = 0;
    D_8011ED16 = 0x40;
    D_8011ED17 = 0x84;
    D_8011ED20 = D_800E6280.unk_1A4C;
    D_8011ED24 = D_800E6280.unk_1A5C;
    D_8011ED48 = D_800E6280.unk_1A3C;
    D_8011ED18 = 8;
    D_8011ED19 = 0x81;
    D_8011ED57 = 0xF;
    D_8011ED2C = arg0;
    D_8011ED3A = -0xAA;
    D_8011ED3E = 0x78;
}

/* No return value on purpose: the original is an implicit-int function (the `or v0,v1,zero` in the first delay slot). T-6070 */
s32 func_80137BD8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_800450F4(0, 0x201);
        func_80059048();
        func_800573AC();
        func_80046318(9U, 0x80162000, 0xB380);
        func_8004E9F4(0);
        D_8015EDEC = 0;
        func_80048DAC(1);
        func_800438DC(1, 0);
        func_80137B14(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_800460CC() & 1) {
            func_800536AC(0x2C0, 0, 0x140, 0xF0);
            func_80135944();
            func_8004284C();
        }
        break;
    }
}

void func_80137CBC(s32 arg0) {
    s32 *p;
    s32 i;

    i = arg0; /* FAKE: copy of the argument reproduces the original's `move t6,a0`; real source unknown. T-6070 */
    p = &D_8015F440[i * 2];
    func_80046290(p[0], p[1], 8);
    func_80044750(0x300);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137A60", func_80137D00);

void func_80137E48(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        if (D_8015EDEC < 8) {
            func_80137CBC(D_8015EDEC);
            D_8015EDEC += 1;
            D_800E6280.unk_110D += 1;
        } else {
            func_8004284C();
        }
        break;
    case 1:
        if (func_800460EC() & 4) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        if (func_800460EC() & 2) {
            D_800E6280.unk_110D = 0;
        }
        break;
    }
    func_800578F4(2);
    if ((D_800E6280.unk_10F8 & 3) == 3) {
        D_8011ED3E -= 1;
    }
}
void func_80137F54(void) {
    func_80137CBC(8);
    func_80044750(0xB1);
    func_80042908(2);
}
