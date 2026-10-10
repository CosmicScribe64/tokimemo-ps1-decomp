#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013DD90(void) {
    D_80145FD0 = 0x801A6558;
    D_80145FD4 = 0x801A655C;
    D_80145FD8 = 0x801A6580;
    D_80145FE0 = 0x801A0000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013DD90", func_8013DDD0);

void func_8013DEBC(void) {
    func_80044750(0x204);
    func_8004284C();
}

void func_8013DEE4(void) {
    D_80120653 |= 0x80;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013DD90", func_8013DF14);

void func_8013E0C8(void) {
    bg_read_sub2(0x41D4);
    func_8004284C();
}

void func_8013E0F0(void) {
    D_80145F2C += D_800E6280.unk_1BC[12].unk_0C.f.f9 * 3 - 3;
    func_8004284C();
}

void func_8013E13C(void) {
    func_80062CD0(0x5855);
    func_8004284C();
}

void func_8013E164(void) {
    func_80046318(0xD, 0x801A0000, 0xBD9F);
    func_8013DD90();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013DD90", func_8013E19C);
