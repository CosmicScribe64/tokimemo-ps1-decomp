#include "common.h"
#include "ovl/SHUGAKU.h"

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132000);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132274);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_801322F0);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132364);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_8013243C);

void func_801325C0(void) {
    func_80044890(0, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        if (D_800E7389 == 6) {
            func_80044750(0x200);
        }
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132638);

void func_801326C0(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80132000();
    func_8004284C();
}

void func_801326F8(void) {
    func_80133430();
    D_8013B06C = D_8013BE74;
    D_8013B070 = D_8013BEA8;
    D_8013B074 = D_8013BEDC;
    D_8013B084 = 1;
    D_8013B088 = 0;
    func_8004284C();
}

void func_80132760(void) {
    if (D_800E69A1 != 6) {
        func_80085B3C(0xA, 0x29);
    } else {
        switch (D_800CA2CC) {                       /* irregular */
        case 0:
            func_80085B3C(0xA, 0x2A);
            break;
        case 1:
            func_80085B3C(0xA, 0x2B);
            break;
        default:
            func_80085B3C(0xA, 0x2C);
            break;
        }
    }
    func_80132000();
    func_8013282C();
    if ((D_800B593C != 0) && (D_80122CF8 != 0)) {
        D_800CA2F0 = 1;
        return;
    }
    D_800CA2F0 = 0;
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_8013282C);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132B50);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132BCC);

void func_80132C1C(void) {
    if (D_800E69A1 == 6) {
        D_800E738A += 6;
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132C64);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132D14);

void func_80132DAC(void) {
    D_800E71DF = D_800E69DD;
    func_800847B8(D_800E69DD);
    func_8004284C();
}

void func_80132DE0(void) {
    if (D_800E69A1 != 6) {
        func_801386C4();
        return;
    }
    func_8004284C();
}

void func_80132E20(void) {
    D_8013B084 = D_8013B08C;
    D_8013B06C = D_8013B078;
    D_8013B070 = D_8013B07C;
    D_8013B074 = D_8013B080;
    func_80132E84();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132E84);

void func_80132F44(void) {
    D_800CA134 = &D_8013B084;
    D_800CA138 = &D_8013B088;
    D_800CA13C = D_8013B06C;
    D_800CA140 = D_8013B070;
    D_800CA144 = D_8013B074;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80132FC0(void) {
    D_800CA134 = &D_8013B084;
    D_800CA138 = &D_8013B088;
    D_800CA13C = D_8013B06C;
    D_800CA140 = D_8013B070;
    D_800CA144 = D_8013B074;
    func_80082764(0xFF, 1, 0);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80133038);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_801330E8);

void func_80133298(void) {
    D_8013B084 = (D_80122CDC * 2) + 0xD;
    D_800CA2F0 = 2;
    func_8004284C();
}

void func_801332D8(void) {
    get_g_zyotai_s(D_800E69DD);
    if (D_800E62BF == (D_800E6378 & 0xF) && D_800E62C0 == ((u32)(D_800E6378 << 0x17) >> 0x1B)) {
        func_80042908(7);
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80133348);
