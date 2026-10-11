#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    u32 pad : 1;
    u32 f : 1;
    u32 rest : 30;
} Bit1Flag;

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

void func_8013E19C(void) {
    s32 r;
    u8 a;
    u8 pad; /* FAKE: unused byte between a and b, real source unknown (frame layout) */
    u8 b;

    a = func_80051A68(7);
    r = func_80051B48(7);
    b = 0xFF;
    if (((Bit1Flag *)&D_800E6280.unk_1BC[7].unk_0C.w)->f && D_800E6280.unk_56C[52] == 0 && a == 0x80 && r == 0) {
        b = 7;
    }
    a = func_80051A68(0);
    r = func_80051B48(0);
    if (D_800E6280.unk_56C[6] == 0 && a == 0x80 && r == 0 && (b == 0xFF || (b == 7 && D_800E6280.unk_1BC[0].unk_0A < D_800E6280.unk_1BC[7].unk_0A))) {
        b = 0;
    }
    a = func_80051A68(9);
    r = func_80051B48(9);
    if (((Bit1Flag *)&D_800E6280.unk_1BC[9].unk_0C.w)->f && D_800E6280.unk_56C[66] == 0 && a == 0x80 && r == 0 &&
        (b == 0xFF || (b == 7 && D_800E6280.unk_1BC[9].unk_0A < D_800E6280.unk_1BC[7].unk_0A) || (b == 0 && D_800E6280.unk_1BC[9].unk_0A < D_800E6280.unk_1BC[0].unk_0A))) {
        b = 9;
    }
    D_80145FE4 = b;
    func_8004284C();
}
