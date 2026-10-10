#include "common.h"
#include "ovl/ETC.h"

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014ABD0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014ACB4);

void func_8014AD94(void) {
    if (func_800460CC() & 1) {
        dec_bg_show_set(1, func_8005751C(1));
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014ADDC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014AE20);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014AE8C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014AF28);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B0D0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B1B4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B750);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B7A4);

s32 func_8014B8CC(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E7208 & 0x20) {
        switch (D_800E7313) {
        case 0:
            func_80042908(4);
            break;
        case 1:
            func_80042908(5);
            break;
        case 2:
            func_80042908(3);
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B978);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B9CC);

void func_8014BA8C(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if ((D_800E7208 & 0x20) && D_800E7313 != -1) {
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(D_80151358[D_800E7313], 0, 0xF);
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014BB34);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014BB94);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014BC00);

s32 func_8014BDE0(void) {
    s32 pad; /* FAKE: unused local, makes the frame 0x38 and puts the spill at sp+0x2C as in the original; real source unknown. T-4010 */
    s32 sum;
    s32 n;

    sum = 0;
    n = 0;
    if (D_800E6382 >= 0x79) {
        n = 1;
        sum = D_800E6382 - 0x78;
    }
    if (D_800E6386 >= 0x79) {
        n += 1;
        sum += D_800E6386;
        sum -= 0x78;
    }
    if (D_800E638A >= 0x79) {
        n += 1;
        sum += D_800E638A;
        sum -= 0x78;
    }
    if ((n == 3) && (sum >= 0xC9)) {
        n = 3;
    } else if (n != 0) {
        n -= 1;
    }
    func_800AE0B0("thl %d \n", n);
    return 3 - n;
}

s32 func_8014BE98(void) {
    s32 n;

    n = -func_8014BDE0() * 2 + 6;
    if (D_800E6392 >= 0x3D) {
        n += 1;
    }
    if (D_800E6396 >= 0x3D) {
        n += 1;
    }
    if (n >= 7) {
        return 0;
    }
    if (n >= 5) {
        return 1;
    }
    if (n >= 3) {
        return 2;
    }
    return 3;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014BF2C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014C018);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014C06C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014C250);

void func_8014C2A4(void) {
    func_80072B5C(1);
}
