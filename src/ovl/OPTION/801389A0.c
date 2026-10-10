#include "common.h"
#include "ovl/OPTION.h"

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_801389A0);

void func_80138A24(void) {
    if (D_800E738D == 0) {
        func_80046318(0x3A, 0x80180000, 0xA1AB);
        D_800E738D += 1;
    } else if (func_800460CC() & 1) {
        func_80042808();
    }
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80138A8C);

void func_80138B4C(s32 arg0) {
    s32 i;
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    for (i = 0; i < 16; i++) {
        a[i] = 0;
        b[i] = i * 10 - 0x64;
        c[i] = 0x40;
        d[i] = 0xA;
        func_8004F870(1, 0x10, a, b, c, d);
    }
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80138C20);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80138DF8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80138FC0);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139198);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_801394F4);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139878);

void func_8013990C(void) {
    D_801220EC = rsin(D_800E7384 << 5);
    D_801220EE = rsin(D_800E7384 << 5);
    D_801220F0 = D_800E7384 << 0xC;
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    func_8013A484();
    func_80139A30();
    k_disp_inc();
    func_801394F4();
    func_801399C0();
}

void func_801399C0(void) {
    if (D_800E7208 & 0x800) {
        D_8013D3F4 += 1;
        func_8007BFB8();
    }
    func_8004B338("reverve", 0x78, 0);
    func_8004B19C(D_8013D3F4 & 1, 0x78, 0xA);
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139A30);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139E3C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139F04);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139F8C);

void func_8013A010(void) {
    func_8004500C(0, D_8013D3A8);
}

void func_8013A038(void) {
    func_8004500C(1, D_8013D3A8);
}

void func_8013A060(void) {
    if (D_8013D3B0 != 0) {
        func_8013A1BC();
        return;
    }
    func_8013A09C();
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_8013A09C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_8013A1BC);

void func_8013A304(void) {
    D_8013D3B4 += 1;
}

void func_8013A31C(void) {
    D_8013D3B0 += 1;
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_8013A334);

void func_8013A484(void) {
    func_80049A40(-0x50, -0x40, 0xB0, 0x90, 0xA, 0x1E021D, 0);
    func_80049A40(-0x96, 4, 0x50, 0x2C, 0xA, 0x101802, 0);
    dtd_on(0xA);
}

void func_8013A4FC(void) {
    func_80049A40(-0xA, -0x6E, 0x50, 0xB4, 6, 0xA08080, 2);
    dtd_on(6);
}
