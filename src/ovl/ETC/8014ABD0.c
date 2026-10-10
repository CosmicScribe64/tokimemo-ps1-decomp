#include "common.h"
#include "ovl/ETC.h"

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014ABD0);

void func_8014ACB4(void) {
    s32 temp_v0;
    s32 sp28;

    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80048E78();
    func_800438F0(1);
    func_80048390();
    func_80041584();
    func_8006BC28(0);
    func_800649D4();
    func_80064FA4(0);
    func_80063930(0);
    func_80064E84();
    func_8004E58C();
    func_80065F34(0);
    func_80057390(0);
    temp_v0 = func_8005742C(0x4055, 1);
    sp28 = temp_v0;
    if (temp_v0 != -1) {
        if ((temp_v0 == 0) || (temp_v0 == 1)) {
            func_8004284C();
            func_8004284C();
            func_80057418(1, sp28);
        }
    } else {
        func_8004284C();
    }
}

void func_8014AD94(void) {
    if (func_800460CC() & 1) {
        dec_bg_show_set(1, func_8005751C(1));
        func_8004284C();
    }
}

void func_8014ADDC(void) {
    func_8006612C("進路指導");
    func_8004EA98();
    func_8004EAD4(0);
    func_80057390(0x80);
    func_80042808();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014AE20);

s32 func_8014AE8C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80150EE0, 0, 0xF);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        func_8004284C();
        break;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014AF28);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B0D0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B1B4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B750);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014B7A4);

s32 func_8014B8CC(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1093) {
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
    if ((D_800E6280.unk_F88 & 0x20) && D_800E6280.unk_1093 != -1) {
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4);
        sndisp(D_80151358[D_800E6280.unk_1093], 0, 0xF);
        func_8004284C();
    }
}

s32 func_8014BB34(void) {
    switch (D_800E6280.unk_110D) {                           /* irregular */
    case 0:
        func_80051DBC();
        break;
    case 1:
        func_80042908(2);
        D_800E6280.unk_110D = 2;
        break;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014BB94);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8014ABD0", func_8014BC00);

s32 func_8014BDE0(void) {
    s32 pad; /* FAKE: unused local, makes the frame 0x38 and puts the spill at sp+0x2C as in the original; real source unknown. T-4010 */
    s32 sum;
    s32 n;

    sum = 0;
    n = 0;
    if (D_800E6280.unk_100.unk_02 >= 0x79) {
        n = 1;
        sum = D_800E6280.unk_100.unk_02 - 0x78;
    }
    if (D_800E6280.unk_104.unk_02 >= 0x79) {
        n += 1;
        sum += D_800E6280.unk_104.unk_02;
        sum -= 0x78;
    }
    if (D_800E6280.unk_108.unk_02 >= 0x79) {
        n += 1;
        sum += D_800E6280.unk_108.unk_02;
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
    if (D_800E6280.unk_110.unk_02 >= 0x3D) {
        n += 1;
    }
    if (D_800E6280.unk_114.unk_02 >= 0x3D) {
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
