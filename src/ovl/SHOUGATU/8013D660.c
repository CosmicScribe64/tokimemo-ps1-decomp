#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013D660);

void func_8013D6D4(void) {
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_8013D734(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x202);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_8013D7BC(void) {
    func_80046318(0xE, 0x80197000, 0xAFBC);
    func_8013C070();
    func_8004284C();
}

void func_8013D7F8(void) {
    D_80122CFC = 0x1E;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013D820);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013D92C);

void func_8013DA5C(void) {
    if (((u32) (D_800E6280.unk_1BC[14].unk_0C.w << 0x14) >> 0x1D) == 1) {
        func_8013D2A8();
        return;
    }
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013DAAC);

void func_8013DCFC(void) {
    bg_read_sub2(0x41CA);
    func_8004284C();
}

void func_8013DD24(void) {
    func_80062CD0(0x6B24);
    func_8004284C();
}

void func_8013DD4C(void) {
    func_80042808();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013DD6C);
