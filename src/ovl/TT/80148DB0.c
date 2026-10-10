#include "common.h"
#include "ovl/TT.h"

void func_80148DB0(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A88;
    for (; i < 8; i++) {
        *p = 0;
        p += 0x58;
    }
}

void func_80148DE4(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A88; i < 8; i++, p += 0x58) {
        p[0] = 0;
        *(s16 *)(p + 0x10) = 0xB;
        *(s16 *)(p + 0x3E) = 0xA;
        *(s16 *)(p + 0x40) = 0x3C8B;
        p[0x42] = 0xF0;
        p[0x43] = 0xD0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80148E7C);

void func_80149098(u8 *arg0) {
    if ((u32)(u16)++*(u16 *)(arg0 + 0x46) >= 0x79) {
        arg0[0] = 0;
    }
}

void func_801490C0(u8 *arg0) {
    *(s32 *)(arg0 + 0x24) = *D_80158A70 - *(s32 *)(arg0 + 0x1C);
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149108);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_801491F0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149400);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_801494D8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149704);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_801498C8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149B20);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149BB0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149D64);

void func_80149E44(void) {
    u8 *p = D_80158A74;

    *(s32 *)(p + 0x24) = func_800A0070(*(u16 *)(D_80158A74 + 0x6A)) * 0x78 + 0x9C0000;
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    *(u16 *)(p + 0x6A) += 0x10;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149EA0);

void func_8014A000(void) {
    s32 i;
    u8 *a64;
    u8 *a6C;
    u8 *a74;
    u8 *p;
    u8 *aA8;

    a64 = D_80158A64;
    a6C = D_80158A6C;
    a74 = D_80158A74;
    p = D_80158AA4 + 0xFA0;
    aA8 = D_80158AA8;
    if (*(u16 *) (a6C + 0x10) >= 0x2C) {
        *(u16 *) (a6C + 0x10) = 0;
        *(u16 *) (a64 + 2) += 1;
    }
    switch (*(u16 *) (a6C + 0x10)) {
    case 8:
        a74[0x3E] = 8;
        break;
    case 16:
        a74[0x3E] = 9;
        break;
    case 22:
        a74[0x3E] = 0xA;
        break;
    case 28:
        a74[0x3E] = 0xB;
        break;
    case 34:
        a74[0x3E] = 0xC;
        break;
    case 40:
        a74[0x3E] = 0xD;
        break;
    case 42:
        a74[0x3E] = 0xE;
        break;
    case 44:
        a74[0x3E] = 0xF;
        break;
    }
    for (i = 0; i < 4; i++) {
        *(s32 *) (p + 0x24) += 0xFFFC0000;
        *(s16 *) (p + 0x16) = *(s16 *) (p + 0x26);
        p += 0x64;
    }
    for (i = 0; i < 7; i++) {
        *(s16 *) (aA8 + 0xC) += 4;
        aA8 += 0x14;
    }
    D_80158AA8[0x78] = 0;
    *(s16 *) (D_80158AA4 + 0x16) += 4;
    *(u16 *) (a6C + 0x10) += 1;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014A198);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014A2B4);

void func_8014A7E0(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A80; i < 12; i++, p += 0x54) {
        *p = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014A814);

void func_8014A8F8(u8 *arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
    switch (arg1) {
    case 8:
    default:
        arg0[0x42] = 0x80;
        arg0[0x43] = 0xC0;
        break;
    case 9:
        arg0[0x42] = 0x90;
        arg0[0x43] = 0xC0;
        break;
    case 10:
        arg0[0x42] = 0x80;
        arg0[0x43] = 0xD0;
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014A958);

void func_8014AAE8(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    if (*(s16 *)(arg0 + 0x16) < 0) {
        *(s16 *)(arg0 + 4) = 0xA0;
        *(s16 *)(arg0 + 0xA) = 0;
        *(s16 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 6) = 0;
        arg0[0xF] = 0;
        arg0[0xE] = 0;
        arg0[0xD] = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014AB48);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014ACEC);
