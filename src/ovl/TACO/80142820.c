#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_80142820);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_801428DC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_80142AF0);

typedef struct {
    u32 pad0 : 1;
    u32 flag : 1;
    u32 pad1 : 30;
} TcBits;

void func_80142DE0(void) {
    func_8004E58C();
    func_800573AC();
    func_800438DC(1, 0);
    func_80048DAC(1);
    *(s32 *) &D_800E6280.unk_1095[3] = 0x9F;
    if (((TcBits *) &D_800E7D50)->flag == 1) {
        func_80048EB8(0);
        func_80048390();
        func_80042878(D_800E6280.unk_720);
        func_80042908(D_800E6280.unk_721);
        func_80042940(D_800E6280.unk_722);
        func_8006BC28(0);
        func_8006C848(0);
        func_80064E84();
        func_800649D4();
        func_80064E48(1);
        func_8006492C(1);
        return;
    }
    func_80042878(0x12);
}

void func_80142EC4(void) {
    func_80059E00();
}

void func_80142EE4(void) {
    func_80059BE8();
    func_8004284C();
}

void func_80142F0C(void) {
    func_8004ADE4();
    func_8004284C();
}

u8 func_80142F34(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80046318(0x95, 0x801A0000, 0x9B94);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_800460CC() & 1) {
            func_80068938(D_800E6280.unk_03E, D_800E6280.unk_03F, 0);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_8004284C();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_80142FE8);
