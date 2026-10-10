#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801335A0", func_801335A0);

void func_80133694(void) {
    s32 i;

    if (D_8015E220 != 0) {
        D_8015E220 -= 1;
    }
    if (D_8015E221 != 0) {
        D_8015E221 -= 1;
    }
    for (i = 0; i < 8; i++) {
        if (D_8015E222[i] != 0) {
            D_8015E222[i] -= 1;
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801335A0", func_80133738);
