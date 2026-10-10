#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_801374C0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137654);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137878);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137A78);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137C20);

void func_80138178(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x50) {
        *(s32 *)(p + 0x24) = 0x500000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
        *(s16 *)(p + 4) = 0x30;
    }
}

void func_801381E8(void) {
    u8 *p = D_80158A8C;

    func_80142AF0(D_80152C54, 0);
    if (p[0x40] & 1) {
        *(u8 **)(p + 0x218) = D_80155C58;
        *(s16 *)(p + 4) = 0x40;
    }
}

void func_80138240(void) {
    u8 *b = D_80158A8C;
    u8 *a = D_80158A74;
    s32 d = *(s16 *)(a + 0x14) - *(s16 *)(b + 0x14);
    s32 ad;
    s16 y;

    if (d < 0) {
        ad = -d;
    } else {
        ad = d;
    }
    if (ad < 2) {
        *(s32 *)(b + 0x20) = *(s16 *)(a + 0x14) << 16;
    } else if (d < 0) {
        *(s32 *)(b + 0x20) -= 0x4000;
    } else {
        *(s32 *)(b + 0x20) += 0x4000;
    }
    y = *(s16 *)(b + 0x22);
    if (y >= 0x51) {
        *(s32 *)(b + 0x20) = 0x500000;
        y = *(s16 *)(b + 0x22);
    } else if (y < -0x50) {
        *(s32 *)(b + 0x20) = 0xFFB00000;
        y = *(s16 *)(b + 0x22);
    }
    *(s16 *)(b + 0x14) = y;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138300);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138808);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138B90);

void func_80138D10(void) {
    switch (*(u16 *)(D_80158A8C + 0x30E)) {
    case 1:
        func_80142AF0(D_80152CA4, 6);
        break;
    case 2:
        func_80142AF0(D_80152CB0, 6);
        break;
    case 3:
        func_80142AF0(D_80152CC4, 6);
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138D94);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138E28);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_8013923C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_801392A0);

void func_801396C4(void) {
    func_80138300();
}

void func_801396E4(void) {
    u8 *p = D_80158A8C;
    u8 *q;

    if ((u32)*(u16 *)(p + 0x366) >= 0x18) {
        q = D_80158AAC;
        func_80133D14(*(s16 *)(q + 0x3A), *(s16 *)(q + 0xBA), 0x18, 0xA8);
        q += 0xEFC;
        func_80133D14(*(s16 *)(q + 0x3A), *(s16 *)(q + 0xBA), 0x18, 0xA8);
        *(u16 *)(p + 0x366) = 0;
        return;
    }
    *(u16 *)(p + 0x366) += 1;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80139770);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80139B98);
