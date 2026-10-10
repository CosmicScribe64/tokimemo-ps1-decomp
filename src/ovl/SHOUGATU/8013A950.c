#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013A950(void) {
    switch (D_80144E14) {
    case 0:
        func_8013A9B0();
        return;
    case 1:
        func_8013AA24();
        return;
    default:
        func_8013AAAC();
        return;
    }
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013A9B0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013AA24);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013AAAC);

void func_8013AB34(void) {
    func_80062CD0(0x66EC);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013AB5C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013AC7C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013ACC0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013AD08);

void func_8013AD50(void) {
    if (((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 0xAU)) {
        bg_read_sub2(0x404D);
    } else {
        bg_read_sub2(0x4055);
    }
    func_8004284C();
}

void func_8013ADA4(void) {
    bg_read_sub2(0x4097);
    func_8004284C();
}

void func_8013ADCC(void) {
    if (((u32) D_800E652A >> 4) == 7) {
        D_80144E08 = 6;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013AE0C);

void func_8013AE6C(void) {
    if (((u32) D_800E652A >> 4) != ((u32) D_800E6374 >> 0xC)) {
        D_80144E08 = 4;
    }
    func_8004284C();
}

void func_8013AEB4(void) {
    if (((u32) D_800E652A >> 4) == ((u32) D_800E6374 >> 0xC)) {
        D_800E738A += 0xD;
        return;
    }
    func_8004284C();
}
