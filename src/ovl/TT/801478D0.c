#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801478D0", func_801478D0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801478D0", func_80147A18);

void func_80147CE0(void) {
    s32 i = 0;
    u8 *p = D_80158A78;

    do {
        i += 1;
        p += 0xA4;
        p[-0xA4] = 0;
    } while (i != 3);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801478D0", func_80147D08);

void func_80147F14(u8 *arg0) {
    s32 i;
    u8 *q;

    q = D_80158A74;
    if (arg0 == D_80158A78) {
        *(s16 *)(arg0 + 0x62) = *(s16 *)(q + 0x14);
        *(s16 *)(arg0 + 0x82) = *(s16 *)(q + 0x16);
    } else {
        *(s16 *)(arg0 + 0x62) = *(s16 *)(arg0 - 0x54);
        *(s16 *)(arg0 + 0x82) = *(s16 *)(arg0 - 0x34);
    }
    for (i = 1; i < 10; i++) {
        *(s16 *)(arg0 + 0x4E + i * 2) = *(s16 *)(arg0 + 0x50 + i * 2);
        *(s16 *)(arg0 + 0x6E + i * 2) = *(s16 *)(arg0 + 0x70 + i * 2);
    }
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x50);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x70);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801478D0", func_80147FD0);

void func_801480BC(void) {
    s32 i;
    u8 *p;
    u8 *a;
    u8 *b;

    p = D_80158A74;
    if (p[0x53] != 0) {
        a = D_80158A78;
        i = 0;
        b = D_80158A7C + 0xC0;
        for (; i < 3; i++) {
            func_80147F14(a);
            b[0x3E] = p[0x5A] + 0x38;
            a += 0xA4;
            b += 0xC0;
            b[-0x22] = p[0x5B] + 0x3D;
        }
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801478D0", func_80148154);
