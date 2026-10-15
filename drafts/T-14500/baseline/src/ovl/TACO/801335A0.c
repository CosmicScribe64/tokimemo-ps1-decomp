#include "common.h"
#include "ovl/TACO.h"

s32 func_801335A0(a, b)
s32 a;
s32 b;
{
    s32 v;
    u8 *p;

    if (D_8015EDB0 != 0) {
        if (a == 1 && (b & 0xFF) < 0xA) {
            v = (b & 0xFF) & 0xF;
            p = &D_8015E220 + v;
            if (*p != 0) {
                return 0;
            }
            *p = func_800AE0D0() % (u32)(D_8015E22C[v] * 2 + 1);
        }
        v = b & 0xFF;
    } else {
        v = b & 0xFF;
    }
    if (a & 1) {
        func_80044750((v | 0x500) & 0xFFFF);
    } else {
        func_80044750((v | 0x600) & 0xFFFF);
    }
}

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
