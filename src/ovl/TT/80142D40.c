#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80142D40);

void func_80142DA8(void *arg0, s16 arg1, s16 arg2) {
    RECT r;

    /* FAKE: w and h on one source line; IDO then stores h before w (line-based scheduling). T-6070 */
    r.w = 0x20; r.h = 0x80;
    r.x = arg1;
    r.y = arg2;
    func_8009C884(&r, arg0);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80142DEC);

void func_8014304C(s32 arg0, s32 arg1, s32 arg2) {
    D_80156784 = D_80156780;
    D_80156780 = func_800460DC();
    func_80045414(9, 0, 0);
    func_800462C8((arg2 + 0x7FF) / 2048, arg0, arg1);
}

u8 func_801430C4(void) {
    return D_80156784;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801430D4);

void func_80143570(void) {
    *D_80158A74 = 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80143580);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801436BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801437F0);

void func_80143970(void) {
}

void func_80143978(void) {
    u8 *p = D_80158A74;
    u8 *q = D_80158A6C;

    switch (*(u16 *)(p + 6)) {
    case 0:
        p[0x52] = 1;
        *(u16 *)(p + 0x50) |= 2;
        p[0x56] = 1;
        p[0x5A] = 0;
        p[0x5B] = 0;
        *(u16 *)(p + 6) = 0x40;
        break;
    case 0x40:
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0xFFFC0000;
        if (*(s32 *)(p + 0x24) < (s32)0xFFF00000) {
            *(s32 *)(p + 0x2C) = 0;
            *(u16 *)(p + 6) = 0x50;
            *(u16 *)(q + 0x10) = 0;
        }
        break;
    case 0x50:
        if ((u32)*(u16 *)(D_80158A6C + 0x10) >= 0x5B) {
            *(u16 *)(p + 6) = 0;
            p[0xD] = 1;
        }
        *(u16 *)(q + 0x10) += 1;
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80143A44);

void func_80143FE8(void) {
    u8 *p = D_80158A74;
    s32 v;

    *(s32 *)(p + 0x20) += *(s32 *)(p + 0x28);
    *(s32 *)(p + 0x24) += *(s32 *)(p + 0x2C);
    if (p[0x56] == 0) {
        if (*(s32 *)(p + 0x20) < (s32)0xFF880000) {
            *(s32 *)(p + 0x20) = -0x780000;
        } else if (*(s32 *)(p + 0x20) >= 0x780001) {
            *(s32 *)(p + 0x20) = 0x780000;
        }
        v = *(s32 *)(p + 0x24);
        if (v < 0x380000) {
            *(s32 *)(p + 0x24) = 0x380000;
        } else if (v >= 0xE00001) {
            *(s32 *)(p + 0x24) = 0xE00000;
        }
    }
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80144094);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801441BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80144854);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80144A04);

void func_80144CA8(void) {
    u8 *q;
    u8 *p;
    u32 i;

    q = D_80158A6C;
    for (i = 0, p = D_80158AA4 + 0x64; i < (u32)(2 - q[0xC]); i++) {
        *p = 0;
        p -= 0x64;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80144CF0);
