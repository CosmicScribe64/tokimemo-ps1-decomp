#include "common.h"
#include "ovl/BUNKA_SD.h"

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_80135160);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_801351DC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_801352F4);

void func_8013540C(void) {
    func_8004500C(1, 0x201);
    func_80135630();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_80135440);

s32 func_801354C8(void) {
    if (D_800E7384 == 0) {
        *(u8 *)&D_800E738B = 0x80;
        D_80120653 = 0;
        D_801206DB = 0;
        D_8012069B = 0x80;
        D_80120697 = 0x80;
        D_80120696 = 5;
    }
    func_80066334();
    D_800E7384 += 1;
    if (D_800E7384 % 9U == 0) {
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
