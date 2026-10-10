#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 func_80136270(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80046318(0x6F, 0x80180000, 0x4C59);
        D_800E6280.unk_110D += 1;
    } else if ((D_800E6280.unk_110D == 1) && (func_800460CC() & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80136270", func_801362EC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80136270", func_801363BC);

void func_801364B4(void) {
    func_8004500C(1, 0x201);
    func_80136578();
    D_80120696 = 1;
    func_8004284C();
}

void func_801364F0(void) {
    func_80136904();
    func_801369AC();
    D_800E6280.unk_1104.u += 1;
    if (D_80122EAC == 3) {
        if (D_800E6280.unk_1104.u >= 0x25B) {
            func_8004284C();
        }
    } else if (!(D_80120652 & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80136270", func_80136578);

void func_80136904(void) {
    if (D_80122EAC == 3) {
        switch (D_800E6280.unk_1104.u) {
        case 0x14C:
            func_80044750(0x501);
            return;
        case 0x1BF:
            func_80044750(0x502);
            return;
        }
    } else {
        switch (D_800E6280.unk_1104.u) {
        case 0x157:
            func_80044750(0x501);
            return;
        case 0x1CA:
            func_80044750(0x502);
            return;
        }
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80136270", func_801369AC);
