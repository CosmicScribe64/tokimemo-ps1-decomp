#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013A950(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
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

void func_8013AC7C(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "教室");
    func_8004284C();
}

void func_8013ACC0(void) {
    D_80144E08 = 0xC;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "グランド");
    func_8004284C();
}

void func_8013AD08(void) {
    D_80144E08 = 0xF;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "グランド");
    func_8004284C();
}

void func_8013AD50(void) {
    if (((u8) D_800E6280.unk_03F >= 6U) && ((u8) D_800E6280.unk_03F < 0xAU)) {
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
    if (((u32) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 7) {
        D_80144E08 = 6;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013AE0C);

void func_8013AE6C(void) {
    if (((u32) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) != ((u32) D_800E6280.unk_0F4.h >> 0xC)) {
        D_80144E08 = 4;
    }
    func_8004284C();
}

void func_8013AEB4(void) {
    if (((u32) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == ((u32) D_800E6280.unk_0F4.h >> 0xC)) {
        D_800E6280.unk_110A += 0xD;
        return;
    }
    func_8004284C();
}
