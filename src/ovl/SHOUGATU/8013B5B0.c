#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B5B0", func_8013B5B0);

void func_8013B62C(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_80044750(0x202);
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B5B0", func_8013B694);

void func_8013B71C(void) {
    func_80046318(9, 0x80197000, 0xAFB3);
    func_8013B210();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B5B0", func_8013B758);

void func_8013B8CC(void) {
    func_80085B3C(7, 0);
    bg_read_sub2(0x4045);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B5B0", func_8013B900);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B5B0", func_8013B944);

void func_8013B984(void) {
    if (D_80145A10 != 0) {
        func_8004284C();
        return;
    }
    func_8013B528();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B5B0", func_8013B9C0);
