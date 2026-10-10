#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133970);

void func_80133A60(void) {
    s32 i;

    func_80079B34();
    func_800451E0(0x8019A000);
    func_80044750(0x12);
    i = 0;
    do {
        func_80079B34();
        func_800AD950(0);
    } while (i++ < 0x10000 && func_80044F94(1) == 0);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133AD0);

void func_80133B3C(void) {
    u8 *p = D_80158A60;
    s32 i;

    for (i = 0; i < 0x40; i++) {
        p[0x56 + i] = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133B70);

u8 *func_80133BA4(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x68;
    }
    return 0;
}

void func_80133BE0(u8 *arg0, u8 *arg1) {
    s32 dx = *(s32 *)(arg1 + 0x20) - *(s32 *)(arg0 + 0x20);
    s32 dy = *(s32 *)(arg1 + 0x24) - *(s32 *)(arg0 + 0x24);

    func_8014D260(dy >> 16, dx >> 16);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133C1C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133D14);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133E30);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133FB0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134178);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_801342CC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134470);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134630);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134768);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134874);

void func_80134924(u8 *arg0) {
    s16 x;
    s16 y;

    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    x = *(s16 *)(arg0 + 0x16);
    if (x >= 0x101 || x < -0x10 || (y = *(s16 *)(arg0 + 0x14)) < -0x88 || y >= 0x89) {
        *(s16 *)(arg0 + 4) = 0;
        *(s16 *)(arg0 + 0xA) = 0;
        *(s16 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 6) = 0;
        arg0[0xF] = 0;
        arg0[0xE] = 0;
        arg0[0xD] = 0;
        arg0[0] = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_801349B0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134ADC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134BBC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134D0C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134E20);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80135148);
