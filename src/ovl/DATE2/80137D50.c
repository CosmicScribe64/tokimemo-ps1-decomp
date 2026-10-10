#include "common.h"
#include "ovl/DATE2.h"

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_80137D50);

void func_80137F64(void) {
    func_80083808();
    func_80138048();
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
    func_80047560();
}

void func_80137FCC(void) {
    D_800CA134 = &D_8013B5F8;
    D_800CA138 = &D_8013B5FC;
    D_800CA13C = D_8013B5EC;
    D_800CA140 = D_8013B5F0;
    D_800CA144 = D_8013B5F4;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_80138048);

void func_801380C4(void) {
    func_80046318(2, 0x801E8000, 0xBD02);
    func_80138100();
    func_8004284C();
}

void func_80138100(void) {
    D_8013B5D4 = 0x801E80D0;
    D_8013B5D8 = 0x801E82C0;
    D_8013B5DC = 0x801E8444;
    D_8013B5E0 = 0x801E8700;
}

void func_80138144(void) {
    func_80044750(0x202);
    D_80122D20 = 0;
    func_8004284C();
}

void func_80138170(void) {
    if (D_80122CDC != 0) {
        func_80083440(2);
    }
    func_8004284C();
}

void func_801381A4(void) {
    func_80042878(0x50);
    func_80042908(1);
    func_80042940(0x11);
}

void func_801381D4(void) {
    func_80046318(3, 0x801B0000, 0xAF39);
    func_80137D50();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_8013820C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_8013850C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_8013856C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_801385CC);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_801388C0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_80138A3C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_80138A90);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_80138BEC);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_80138E0C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_80138F8C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_801391A0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_801393B4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_801395C8);
