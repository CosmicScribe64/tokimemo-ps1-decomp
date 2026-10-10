#include "common.h"
#include "ovl/SHOUGATU.h"

void func_801397D0(void) {
    D_801453B0 = (u8 *)0x801E60CC;
    D_801453B4 = (u8 *)0x801E60D0;
    D_801453B8 = (u8 *)0x801E60E4;
    D_801453BC = *(s16 *)0x801E60F0;
    D_801453C0 = (u8 *)0x801AE000;
    D_801453C4 = (u8 *)0x801B0000;
    D_801453C8 = (u8 *)0x801B2000;
    D_801453CC = (u8 *)0x801B6000;
    D_801453D0 = (u8 *)0x801BA000;
    D_801453D4 = (u8 *)0x801BE000;
    D_801453D8 = (u8 *)0x801C2000;
    D_801453DC = (u8 *)0x801C6000;
    D_801453E0 = (u8 *)0x801CE000;
    D_801453E4 = (u8 *)0x801D2000;
    D_801453E8 = (u8 *)0x801D6000;
    D_801453EC = (u8 *)0x801DA000;
    D_801453F0 = (u8 *)0x801DE000;
    D_801453F4 = (u8 *)0x801E2000;
}

void func_801398F0(void) {
    switch (D_80144E14) {
    case 0:
        func_80139980();
        return;
    case 1:
        func_80139A08();
        return;
    case 2:
        func_80139A90();
        return;
    case 3:
        func_80139B0C();
        return;
    default:
        func_80139B94();
        return;
    }
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139980);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139A08);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139A90);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139B0C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139B94);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139C10);

void func_80139C68(void) {
    D_800E71DF = 1;
    func_80137AB4();
}

void func_80139C90(void) {
    func_80046318(0x71, 0x801AE000, 0x8768);
    func_801397D0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139CCC);

void func_80139D80(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "図書室");
    func_8004284C();
}

void func_80139DC4(void) {
    D_80144E08 = 3;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "図書室");
    func_8004284C();
}

void func_80139E0C(void) {
    D_80144E08 = 6;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "図書室");
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139E54);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801397D0", func_80139EC8);

void func_8013A098(void) {
    bg_read_sub2(0x40BC);
    func_8004284C();
}

void func_8013A0C0(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_8013A0E8(void) {
    bg_read_sub2(0x40BC);
    func_8004284C();
}

void func_8013A110(void) {
    bg_read_sub2(0x40B2);
    func_8004284C();
}

void func_8013A138(void) {
    bg_read_sub2(0x46C4);
    func_8004284C();
}

void func_8013A160(void) {
    func_80062CD0(0x5D6E);
    func_8004284C();
}
