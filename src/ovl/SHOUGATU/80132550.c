#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132550", func_80132550);

void func_801325CC(void) {
    func_80044890(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

void func_80132634(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    if ((u32) D_800E7384++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E738A - 1) & 0xFF);
    }
}

void func_801326BC(void) {
    func_80046318(0x34, 0x80197000, 0xAF48);
    func_80132000();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132550", func_801326F8);

void func_80132864(void) {
    bg_read_sub2(0x4167);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132550", func_8013288C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132550", func_80132A34);
