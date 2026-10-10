#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 func_80135160(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80046318(0x61, 0x80180000, 0x4E4E);
        D_800E6280.unk_110D += 1;
    } else if ((D_800E6280.unk_110D == 1) && (func_800460CC() & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_801351DC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_801352F4);

void func_8013540C(void) {
    func_8004500C(1, 0x201);
    func_80135630();
    func_8004284C();
}

void func_80135440(void) {
    if (D_800E6280.unk_1104.u == 0) {
        D_80120652 = 5;
    }
    D_800E6280.unk_1104.u += 1;
    if (D_80122EAC == 3) {
        if (D_800E6280.unk_1104.u >= 0x1C2) {
            func_8004284C();
        }
    } else {
        if (!(D_80120652 & 1)) {
            func_8004284C();
        }
    }
}

s32 func_801354C8(void) {
    if (D_800E6280.unk_1104.u == 0) {
        *(u8 *)&D_800E6280.unk_110B = 0x80;
        D_80120653 = 0;
        D_801206DB = 0;
        D_8012069B = 0x80;
        D_80120697 = 0x80;
        D_80120696 = 5;
    }
    func_80066334();
    D_800E6280.unk_1104.u += 1;
    if (D_800E6280.unk_1104.u % 9U == 0) {
        if ((D_801206D7 += 1) >= 0x15U) {
            D_801206D7 = 0x11;
        }
    }
    if (!(D_80120696 & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_801355AC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_80135630);
