#include "common.h"
#include "ovl/TAIIKU.h"

void func_80141C00(void) {
    if ((func_8008667C(6) < 2) && (((u32) (D_800E6598 << 0x1E) >> 0x1F) == 1) && (D_800E6819 == 0)) {
        D_800E6819 = 1;
        func_80042808();
    } else {
        D_800E7389 = 0;
        D_800E738A = 0;
        D_80122ECC = 1;
    }
    Default_Disp();
}

void func_80141C88(void) {
    s32 temp_v0;

    temp_v0 = func_80141CDC();
    if (temp_v0 != 0) {
        if (temp_v0 == 1) {
            D_800E7389 = 0;
            D_800E738A = 0;
            D_80122ECC = 1;
            return;
        }
        func_80046500();
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80141C00", func_80141CDC);
