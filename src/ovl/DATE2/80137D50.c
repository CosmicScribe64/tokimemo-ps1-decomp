#include "common.h"
#include "ovl/DATE2.h"

void func_80137D50(void) {
    D_8013B540 = 0x801B00A4;
    D_8013B544 = 0x801B0238;
    D_8013B548 = 0x801B03C8;
    D_8013B54C = 0x801B0558;
    D_8013B550 = 0x801B0704;
    D_8013B554 = 0x801B0878;
    D_8013B558 = 0x801B0A20;
    D_8013B55C = 0x801B0B9C;
    D_8013B560 = 0x801B0D88;
    D_8013B564 = 0x801B0F24;
    D_8013B568 = 0x801B106C;
    D_8013B56C = 0x801B00CC;
    D_8013B570 = 0x801B0260;
    D_8013B574 = 0x801B03F0;
    D_8013B578 = 0x801B0580;
    D_8013B57C = 0x801B072C;
    D_8013B580 = 0x801B08A0;
    D_8013B584 = 0x801B0A48;
    D_8013B588 = 0x801B0BC4;
    D_8013B58C = 0x801B0DB0;
    D_8013B590 = 0x801B0F4C;
    D_8013B594 = 0x801B1084;
    D_8013B598 = 0x801B016C;
    D_8013B59C = 0x801B0300;
    D_8013B5A0 = 0x801B0490;
    D_8013B5A4 = 0x801B0620;
    D_8013B5A8 = 0x801B07CC;
    D_8013B5AC = 0x801B0940;
    D_8013B5B0 = 0x801B0AE8;
    D_8013B5B4 = 0x801B0C64;
    D_8013B5B8 = 0x801B0E50;
    D_8013B5BC = 0x801B0FEC;
    D_8013B5C0 = 0x801B10E4;
}

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

void func_8013856C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_801385CC();
        return;
    case 1:
        func_801388C0();
        return;
    default:
        func_80046500();
        return;
    }
}

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
