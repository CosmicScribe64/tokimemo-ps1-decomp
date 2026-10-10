#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013A500(void) {
    if (D_80144E14 == 0) {
        func_8013A53C();
        return;
    }
    func_8013A600();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A500", func_8013A53C);

void func_8013A5B0(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_8013A5D8(void) {
    func_80044750(0x503);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A500", func_8013A600);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A500", func_8013A688);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A500", func_8013A7B0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A500", func_8013A7F4);

void func_8013A83C(void) {
    bg_read_sub2(0x42D8);
    func_8004284C();
}

void func_8013A864(void) {
    bg_read_sub2(0x40A0);
    func_8004284C();
}

void func_8013A88C(void) {
    func_80062CD0(0x6449);
    func_8004284C();
}

void func_8013A8B4(void) {
    func_80062CD0(0x625A);
    func_8004284C();
}

void func_8013A8DC(void) {
    if (((u32) D_800E6374 >> 0xC) == 9) {
        D_80144E08 = 5;
    }
    func_8004284C();
}

void func_8013A91C(void) {
    D_80144E08 = 3;
    func_8004284C();
}
