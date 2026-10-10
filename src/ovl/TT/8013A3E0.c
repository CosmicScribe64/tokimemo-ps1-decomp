#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A3E0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A610);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A710);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A810);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A8B8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A960);

void func_8013AA44(void) {
    u8 *p = D_80158A8C;
    u16 i = *(u16 *)(p + 0x3A4);
    s32 r = i % 3;

    p[0x3C] = D_801559F0[i];
    p[0x3E] = D_80155A08[i];
    *(s16 *)(p + 0x248) = D_80155A20[r];
    *(s16 *)(p + 0x278) = D_80155A28[r];
    p[0x50] = 2;
    p[0x52] = D_80155A30[r];
    *(s16 *)(p + 0x24A) = 0x15;
    *(s16 *)(p + 0x27A) = 9;
}

void func_8013AACC(void) {
    u8 *p = D_80158A8C;
    u16 i = *(u16 *)(p + 0x3A6);
    s32 r = i % 3;

    p[0x64] = D_80155A34[i];
    p[0x66] = D_80155A4C[i];
    *(s16 *)(p + 0x24C) = D_80155A64[r];
    *(s16 *)(p + 0x27C) = D_80155A6C[r];
    p[0x78] = 0;
    p[0x7A] = D_80155A74[r];
    *(s16 *)(p + 0x24E) = -0x15;
    *(s16 *)(p + 0x27E) = 9;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013AB50);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013ABB0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013AC0C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013ADC0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013AE3C);

void func_8013B0E0(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = func_800A0070(*(s32 *)(p + 0x3E4)) * 0x500;
    *(s32 *)(p + 0x24) = (func_800A0140(*(s32 *)(p + 0x3E4) * 4) << 7) + 0x380000;
    *(s32 *)(p + 0x3E4) += 0x10;
}

void func_8013B150(void) {
    u8 *p = D_80158A8C;

    switch (*(u16 *)(p + 0x30E)) {
    case 0:
        func_8013AE3C();
        break;
    case 1:
        func_8013B0E0();
        break;
    }
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013B1C4);

void func_8013B41C(void) {
    u8 *p = D_80158A8C;
    s16 v = *(s16 *)(p + 0x16);

    if (v >= 0x3F) {
        *(u16 *)(p + 0x3A4) = 0xB;
        *(u16 *)(p + 0x3A6) = 0xB;
        return;
    }
    if (v >= 0x3B) {
        *(u16 *)(p + 0x3A4) = 0xA;
        *(u16 *)(p + 0x3A6) = 0xA;
        return;
    }
    if (v >= 0x37) {
        *(u16 *)(p + 0x3A4) = 9;
        *(u16 *)(p + 0x3A6) = 9;
        return;
    }
    if (v >= 0x33) {
        *(u16 *)(p + 0x3A4) = 8;
        *(u16 *)(p + 0x3A6) = 8;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013B4A0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013B6C8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013B928);

void func_8013BAD0(void) {
    switch (*(u16 *)(D_80158A8C + 0x30E)) {
    case 0:
        func_8013B6C8();
        break;
    case 1:
        func_8013B928();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013BB28);
