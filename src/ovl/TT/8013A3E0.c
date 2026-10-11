#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A3E0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A610);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A3E0", func_8013A710);

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x1C];
} TtXY; /* FAKE: pad4 and fake_pad below reproduce the original's frame (locals 0x30..0x54); real locals unknown. T-8060 */

void func_8013A810(void) {
    u8 *p;
    u16 idx;
    TtXY pos;
    s32 fake_pad[2];

    p = D_80158A8C;
    idx = *(u16 *)(p + 0x3A8);
    pos.x = *(s16 *)(p + 0x14) + *(s16 *)(p + 0x252);
    pos.y = *(s16 *)(p + 0x16) + *(s16 *)(p + 0x282);
    func_8013A3E0(&pos, idx * 6 + 0x78 + D_80155C78, -1);
    pos.x = *(s16 *)(p + 0x14) + *(s16 *)(p + 0x254);
    pos.y = *(s16 *)(p + 0x16) + *(s16 *)(p + 0x284);
    func_8013A3E0(&pos, &D_80155D02, 2);
}

void func_8013A8B8(void) {
    u8 *p;
    u16 idx;
    TtXY pos;
    s32 fake_pad[2];

    p = D_80158A8C;
    idx = *(u16 *)(p + 0x3AA);
    pos.x = *(s16 *)(p + 0x14) + *(s16 *)(p + 0x256);
    pos.y = *(s16 *)(p + 0x16) + *(s16 *)(p + 0x286);
    func_8013A3E0(&pos, idx * 6 + 0x66 + D_80155C78, -1);
    pos.x = *(s16 *)(p + 0x14) + *(s16 *)(p + 0x258);
    pos.y = *(s16 *)(p + 0x16) + *(s16 *)(p + 0x288);
    func_8013A3E0(&pos, &D_80155D02, 3);
}

void func_8013A960(void) {
    u8 *p;
    s32 i;

    p = D_80158A8C;
    if (p[0] != 0) {
        for (i = 0; i < 0x18; i++) {
            p[0x42 + i * 0x14] = 0;
        }
        if (*(s16 *)(p + 0x2D8) >= 0) {
            func_8013A610();
        }
        if (*(s16 *)(p + 0x2DA) >= 0) {
            func_8013A710();
        }
        if (*(s16 *)(p + 0x2DC) >= 0) {
            func_8013A810();
        }
        if (*(s16 *)(p + 0x2DE) >= 0) {
            func_8013A8B8();
        }
        if (func_801427A8() != 0) {
            func_80133AD0(0x508);
        }
    }
}

void func_8013AA44(void) {
    u8 *p = D_80158A8C;
    s32 i = *(u16 *)(p + 0x3A4);
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
    s32 i = *(u16 *)(p + 0x3A6);
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

void func_8013AB50(void) {
    u8 *p = D_80158A8C;
    u16 i = *(u16 *)(p + 0x3A8);

    p[0xA0] = 2;
    p[0xA2] = D_80155A80[i];
    *(s16 *)(p + 0x252) = 0x1A;
    *(s16 *)(p + 0x282) = 0x24;
    p[0xB4] = 2;
    p[0xB6] = 0xE;
    *(s16 *)(p + 0x254) = 0x26;
    *(s16 *)(p + 0x284) = D_80155A78[i];
}

void func_8013ABB0(void) {
    u8 *p = D_80158A8C;
    u16 i = *(u16 *)(p + 0x3AA);

    p[0xC8] = 0;
    p[0xCA] = D_80155A8C[i];
    *(s16 *)(p + 0x256) = -0x1A;
    *(s16 *)(p + 0x286) = 0x24;
    p[0xDC] = 0;
    p[0xDE] = 0xE;
    *(s16 *)(p + 0x258) = -0x26;
    *(s16 *)(p + 0x288) = D_80155A84[i];
}

void func_8013AC0C(void) {
    u8 *p;
    s32 i;

    p = D_80158A8C;
    func_80142740();
    func_80142774();
    p[0] = 1;
    p[0x342] = 0;
    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) = 0xFFB00000;
    *(s16 *)(p + 0x14) = 0;
    *(s16 *)(p + 0x16) = -0x50;
    for (i = 0; i < 10; i++) {
        *(u8 **)(p + i * 0x14 + 0x38) = D_80151A50;
        p[i * 0x14 + 0x3C] = 0;
        p[i * 0x14 + 0x3D] = 9;
        p[i * 0x14 + 0x40] = 0x60;
        p[i * 0x14 + 0x43] = 0xF;
        p[i * 0x14 + 0x48] = 0;
        p[i + 0x310] = 1;
        *(u16 *)(p + i * 2 + 0x2A8) = 9;
    }
    for (i = 2; i < 24; i++) {
        p[0x310 + i] = 0;
    }
    p[0x312] = 1;
    p[0x313] = 1;
    p[0x314] = 1;
    p[0x315] = 1;
    p[0x316] = 1;
    p[0x317] = 1;
    p[0x318] = 1;
    p[0x8C] = 0;
    p[0x8E] = 0;
    *(s16 *)(p + 0x250) = 0;
    *(s16 *)(p + 0x280) = 0;
    *(s16 *)(p + 0x30C) = 0x80;
    *(s16 *)(p + 0x30E) = 0;
    *(s16 *)(p + 0x2D8) = 0x60;
    *(s16 *)(p + 0x2DA) = 0x60;
    *(s16 *)(p + 0x2DC) = 0x60;
    *(s16 *)(p + 0x2DE) = 0x60;
    *(s16 *)(p + 0x3A4) = 0xB;
    *(s16 *)(p + 0x3A6) = 0xB;
    *(s16 *)(p + 0x3A8) = 2;
    *(s16 *)(p + 0x3AA) = 2;
    *(s16 *)(p + 0x3AC) = 0;
    *(s32 *)(p + 0x3E4) = 0;
    p[0x343] = 0;
    p[0x344] = 0;
    p[0x345] = 0;
    p[0x346] = 0;
    *(s16 *)(p + 0x364) = 0;
    *(s16 *)(p + 0x366) = 0;
    *(s16 *)(p + 0x368) = 0;
    *(s16 *)(p + 0x36A) = 0;
    func_8013AA44();
    func_8013AACC();
    func_8013AB50();
    func_8013ABB0();
    D_8014ED50 = 0x3E50;
    *(u8 **)(p + 0x228) = D_80155C7E;
    *(u8 **)(p + 0x22C) = D_80155C84;
}

void func_8013ADC0(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x40) {
        *(s32 *)(p + 0x24) = 0x400000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(u8 **)(p + 0x218) = D_80155C78;
        *(s16 *)(p + 4) = 0x40;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

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
