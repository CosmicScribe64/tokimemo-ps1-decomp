#include "common.h"
#include "ovl/TT.h"

u8 *func_80147380(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x68;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80147380", func_801473BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80147380", func_801473E8);

void func_8014742C(u8 *arg0, u16 arg1) {
    arg0[0] = 1;
    *(u16 *)(arg0 + 2) = arg1;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80147380", func_8014743C);

void func_8014750C(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    if (*(s16 *)(arg0 + 0x16) >= 0x101) {
        arg0[0] = 0;
        *(s16 *)(arg0 + 4) = 0;
        arg0[0xC] = 0;
        *(s16 *)(arg0 + 0xA) = 0;
        *(s16 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 6) = 0;
        arg0[0xF] = 0;
        arg0[0xE] = 0;
        arg0[0xD] = 0;
        return;
    }
    if (*(s16 *)(arg0 + 0x52) < 0xA01) {
        arg0[0x54] = 1;
    } else if (*(s16 *)(arg0 + 0x52) >= 0x1000) {
        *(s8 *)(arg0 + 0x54) = -1;
    }
    *(s16 *)(arg0 + 0x52) += *(s8 *)(arg0 + 0x54) * 0x30;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80147380", func_801475C8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80147380", func_80147644);
