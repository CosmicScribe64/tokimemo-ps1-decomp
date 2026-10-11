#include "common.h"
#include "ovl/OPTION.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D360);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D364);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D368);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D36C);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D370);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D374);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D378);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D37C);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D380);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D39C);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3A0);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3A4);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3A8);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3B0);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3B4);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3B8);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3BC);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3CC);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3DC);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3E0);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3F0);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3F4);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3F8);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D798);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D880);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D968);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013DD58);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E148);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E244);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E340);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E43C);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E538);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E634);
s32 D_8013E730 = 0;
s32 D_8013E734 = 0;
s32 D_8013E738 = 0;
s32 D_8013E73C = 0;

void func_801389A0(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80138A24();
        break;
    case 1:
        func_80138A8C();
        break;
    default:
        func_80046500();
        break;
    }
    func_80066C08(0);
    func_80066334();
}

void func_80138A24(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80046318(0x3A, 0x80180000, 0xA1AB);
        D_800E6280.unk_110D += 1;
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
    D_801220EC = rsin(D_800E6280.unk_1104.w << 5);
    D_801220EE = rsin(D_800E6280.unk_1104.w << 5);
    D_801220F0 = D_800E6280.unk_1104.w << 0xC;
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
    if (D_800E6280.unk_F88 & 0x800) {
        D_8013D3F4 += 1;
        func_8007BFB8();
    }
    func_8004B338("reverve", 0x78, 0);
    func_8004B19C(D_8013D3F4 & 1, 0x78, 0xA);
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139A30);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139E3C);

void func_80139F04(void) {
    if (D_800E6280.unk_F80 & 1) {
        D_8013E730 -= 1;
    }
    if (D_800E6280.unk_F80 & 4) {
        D_8013E730 += 1;
    }
    if (D_8013E730 < 0) {
        D_8013E730 = 0x20;
    }
    if (D_8013E730 >= 0x21) {
        D_8013E730 = 0;
    }
    func_80044750((D_8013E730 | 0x500) & 0xFFFF);
}

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

void func_8013A334(void) {
    func_8004AE54(D_8013D3B8, 0x50, -0x3C, 6);
    func_8004AE54(D_800E6280.unk_F5F & 0xF, -0x50, -0x3C, 6);
    func_8004AE54(D_8013D3B0, 0x50, -0x28, 6);
    func_8004AE54(D_8013D3B4, 0x50, -0x14, 6);
    func_8004AE54(D_8013D3A4, 0x50, 0, 6);
    func_8004B358(D_8013D3BC, -0x96, 8, 6);
    func_8004AE54(D_8013D3A0, 0x50, 0x14, 6);
    func_8004B358(D_8013D3CC, -0x96, 0x14, 6);
    func_8004AE54(D_8013D39C, -0x50, 0xA - D_8013D39C * 0xA, 6);
    func_8004AE54(D_8013E730, 0x50, 0x28, 6);
    func_8004B358(D_8013D3E0, -0x96, 0x20, 6);
    func_8004AE54(D_8013D3A8, 0x50, 0x3C, 6);
}

void func_8013A484(void) {
    func_80049A40(-0x50, -0x40, 0xB0, 0x90, 0xA, 0x1E021D, 0);
    func_80049A40(-0x96, 4, 0x50, 0x2C, 0xA, 0x101802, 0);
    dtd_on(0xA);
}

void func_8013A4FC(void) {
    func_80049A40(-0xA, -0x6E, 0x50, 0xB4, 6, 0xA08080, 2);
    dtd_on(6);
}
