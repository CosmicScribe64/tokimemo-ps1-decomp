#include "common.h"
#include "ovl/BUNKA_SD.h"

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80136270", func_80136270);

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
