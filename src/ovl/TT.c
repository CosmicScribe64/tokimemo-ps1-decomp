#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132000);

void func_801320F0(u8 *arg0, u8 *arg1) {
    s32 dx = *(s32 *)(arg1 + 0xC) - *(s32 *)(arg0 + 0xC);
    s32 dy = *(s32 *)(arg1 + 0x10) - *(s32 *)(arg0 + 0x10);

    func_8014D260(dy >> 16, dx >> 16);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132130);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132198);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013230C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132810);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132B88);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132C24);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132CCC);

u8 *func_80132F10(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x78;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132F4C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80132F80);

void func_80132FC8(u8 *arg0, u16 arg1, s32 arg2) {
    arg0[0] = 1;
    arg0[0x51] = 0;
    arg0[0x64] = 0;
    *(u16 *)(arg0 + 2) = arg1;
    *(s16 *)(arg0 + 0x58) = 0xA;
    *(s32 *)(arg0 + 0x38) = (&D_8014E34C)[arg2];
    arg0[0x40] = 0x80;
    *(s16 *)(arg0 + 0x56) = 0;
    switch (arg1) {
    case 0x60:
        *(s16 *)(arg0 + 0x5A) = 0x3CC7;
        break;
    case 0x61:
    case 0x62:
        *(s16 *)(arg0 + 0x5A) = 0x3CC8;
        break;
    case 0x63:
        *(s16 *)(arg0 + 0x5A) = 0x3CC9;
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133058);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801330D8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133288);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801333D4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801336F8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133970);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133A60);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133AD0);

void func_80133B3C(void) {
    u8 *p = D_80158A60;
    s32 i;

    for (i = 0; i < 0x40; i++) {
        p[0x56 + i] = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133B70);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133C1C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133D14);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133E30);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80133FB0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134178);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801342CC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134470);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134630);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134768);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134874);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801349B0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134ADC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134BBC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134D0C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80134E20);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80135148);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80135360);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801356C8);

void func_80135848(s32 arg0) {
    D_80158A8C[0x3E] = ((arg0 + 0x40) & 0xFFF) / 128;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80135870);

void func_80135A54(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x50) {
        *(s32 *)(p + 0x24) = 0x500000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 4) = 0x30;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

void func_80135AC4(void) {
    u8 *p = D_80158A8C;

    switch (*(u16 *)(p + 8)) {
    case 0:
        p[0x54] = 0x80;
        *(u16 *)(p + 8) = 0x40;
        break;
    case 0x40:
        func_80142AF0(D_80150978, 1);
        p[0x54] &= 1;
        if (p[0x54] != 0) {
            *(u8 **)(p + 0x218) = D_80155B54;
            p[0x343] = 1;
            *(u16 *)(p + 8) = 0;
            *(u16 *)(p + 4) = 0x40;
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80135B5C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80135E9C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80135FA8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80136244);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801364EC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801365C4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801367C0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80136830);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801369F4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80136AD8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80136C64);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801374C0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80137654);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80137878);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80137A78);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80137C20);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80138300);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80138808);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80138B90);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80138D94);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80138E28);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013923C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801392A0);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80139770);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80139B98);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013A040);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013A0DC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013A3E0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013A610);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013A710);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013A810);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013A8B8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013A960);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013AB50);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013ABB0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013AC0C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013ADC0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013AE3C);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013B1C4);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013B4A0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013B6C8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013B928);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013BB28);

u8 *func_8013BD10(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x6C;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013BD4C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013BD80);

void func_8013BDC8(u8 *arg0) {
    arg0[0] = 1;
    *(s16 *)(arg0 + 2) = 0x20;
    *(s16 *)(arg0 + 0x52) = 0xA;
    *(s16 *)(arg0 + 0x54) = 0x3C84;
    *(s32 *)(arg0 + 0x38) = D_8014E34C;
    arg0[0x40] = 0x80;
    *(s16 *)(arg0 + 0x10) = 6;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013BE08);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013BE9C);

void func_8013BF68(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) = 0;
    *(s32 *)(arg0 + 0x24) = 0x200000;
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(s32 *)(arg0 + 0x30) = 0;
    *(s32 *)(arg0 + 0x34) = 0xC00;
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013BF9C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013C048);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013C0A8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013C1FC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013C41C);

void func_8013C6D0(void) {
    u8 *q = D_80158A6C;
    u8 *p = D_80158AA4;

    p[0] = 1;
    p[1] = 0;
    *(s16 *)(p + 0x44) = 0;
    *(s16 *)(p + 0x14) = -0x60;
    *(s16 *)(p + 0x16) = q[0xA] * 16 + 0x60;
    *(s16 *)(p + 0x3C) = 8;
    *(s16 *)(p + 0x3E) = 8;
    p[0x46] = 0;
    p[0x47] = 0x18;
    *(s16 *)(p + 0x40) = 0xA;
    *(s16 *)(p + 0x42) = 0x3C0F;
    p[0x3B] = 0x65;
    *(s16 *)(p + 0x10) = 0;
}

void func_8013C740(void) {
    *(s16 *)(D_80158AA4 + 0x16) = D_80158A6C[0xA] * 16 + 0x60;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013C764);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013C818);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013C8DC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013CA0C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013CB58);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013CC24);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013CDC0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013D060);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013D150);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013D2E0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013D3C0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013D508);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013D73C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013D988);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013DE10);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013E1E8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013E290);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013E320);

void func_8013E454(void) {
    func_800AE080(D_80158A60, 0xA8);
    func_800AE080(D_80158AB4, 0x10170);
    func_8013E320();
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013E498);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013E6A0);

void func_8013E824(void) {
    func_8004111C();
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013E850);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013EC1C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013EC64);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013ED70);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013EE88);

void func_8013EF38(void) {
    u8 *q = D_80158A68;
    u8 *p;

    *(s32 *)(q + 8) += 1;
    *(s32 *)(q + 8) &= 1;
    p = D_80158AB4 + *(s32 *)(q + 8) * 0x80B8;
    *(s32 *)(p + 4) = *(s32 *)p;
    func_8009CAAC(p + 0x8008, 0x10);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013EFAC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013F040);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013F3B4);

void func_8013F588(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x40) {
        *(s32 *)(p + 0x24) = 0x400000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 4) = 0x30;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

void func_8013F5F8(void) {
    u8 *p = D_80158A8C;

    func_80142AF0(D_80155E2C, 0);
    if (p[0x40] & 1) {
        *(u8 **)(p + 0x218) = D_80155D2C;
        *(s16 *)(p + 4) = 0x40;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013F650);

void func_8013F748(void) {
    u8 *p = D_80158A8C;
    s32 v = *(s32 *)(p + 0x20);

    if (v < (s32)0xFFE00000) {
        *(s32 *)(p + 0x20) += 0x10000;
        return;
    }
    if (v >= 0x200001) {
        *(s32 *)(p + 0x20) += 0xFFFF0000;
        return;
    }
    *(s16 *)(p + 6) = 2;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013F7B0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013F878);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013F988);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013F9E0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013FBD0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8013FF14);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801400B0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80140624);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80140920);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80140CBC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80140D84);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801410A0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80141450);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80141A1C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80141C6C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80141EBC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80142110);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80142350);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80142570);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801426F0);

void func_80142740(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 0x18; i++) {
        p[0x310 + i] = 0;
    }
}

void func_80142774(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 12; i++) {
        *(s32 *)(p + 0x218 + i * 4) = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801427A8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80142AF0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80142C70);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80142D40);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80142DA8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80142DEC);

void func_8014304C(s32 arg0, s32 arg1, s32 arg2) {
    D_80156784 = D_80156780;
    D_80156780 = func_800460DC();
    func_80045414(9, 0, 0);
    func_800462C8((arg2 + 0x7FF) / 2048, arg0, arg1);
}

u8 func_801430C4(void) {
    return D_80156784;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801430D4);

void func_80143570(void) {
    *D_80158A74 = 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80143580);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801436BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801437F0);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80143A44);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80144094);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801441BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80144854);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80144A04);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80144CA8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80144CF0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80144D70);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80145370);

void func_801454D8(u8 *arg0) {
    D_80158A74[0x55] = 0xA;
    func_8013BE9C(arg0 + 0x14);
}

s32 func_80145508(u8 *arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    s16 x1 = *(s16 *)arg1 + *(s16 *)arg0;
    s16 y1 = *(s16 *)(arg1 + 2) + *(s16 *)(arg0 + 2);
    s16 x2 = *(s16 *)arg3 + *(s16 *)arg2;
    s16 y2 = *(s16 *)(arg3 + 2) + *(s16 *)(arg2 + 2);
    s32 dx = x1 - x2;
    s32 dy;
    s32 a;

    if (dx < 0) {
        a = -dx;
    } else {
        a = dx;
    }
    dy = y1 - y2;
    if ((u32)a < (u32)(arg3[4] + arg1[4])) {
        if (dy < 0) {
            a = -dy;
        } else {
            a = dy;
        }
        if ((u32)a < (u32)(arg3[5] + arg1[5])) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801455C8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80145E60);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80146000);

void func_80146224(void) {
    u8 *p = D_80158A6C;
    s16 *q = (s16 *)D_80158AB0;
    s32 i;

    for (i = 0; i != 0x19; i++) {
        s16 v = func_800A0140(*(u16 *)(p + 0x1E));

        q += 8;
        q[-8] = v;
        q[-7] = 0x1000;
    }
    *(u16 *)(p + 0x1E) += 0x10;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801462AC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80146348);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014687C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80146924);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80146BE0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80146C14);

void func_80146C74(u8 *arg0, s32 arg1) {
    u8 *p = D_80158A74;

    if (arg1 < 2) {
        if (arg0[0x3D] != 0) {
            *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) + 6;
        } else {
            *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) - 6;
        }
        *(s32 *)(arg0 + 0x20) = *(s16 *)(arg0 + 0x14) << 16;
        *(s16 *)(arg0 + 0x16) = *(s16 *)(p + 0x16) - 0x10;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(arg0 + 0x16) << 16;
        *(s32 *)(arg0 + 0x2C) = 0xFFFC0000;
        arg0[0x44] = 0xA0;
        arg0[0x45] = 0xD0;
        *(s16 *)(arg0 + 0x10) = 3;
        *(s16 *)(arg0 + 0x40) = 0xA;
        *(s16 *)(arg0 + 0x42) = 0x3C81;
        *(s16 *)(arg0 + 0x46) = 0;
        return;
    }
    p = D_80158A78 + (arg1 >> 1) * 0xA4 - 0xA4;
    if (arg0[0x3D] != 0) {
        *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) + 6;
    } else {
        *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) - 6;
    }
    *(s32 *)(arg0 + 0x20) = *(s16 *)(arg0 + 0x14) << 16;
    *(s16 *)(arg0 + 0x16) = *(s16 *)(p + 0x16) - 0x10;
    *(s32 *)(arg0 + 0x24) = *(s16 *)(arg0 + 0x16) << 16;
    *(s32 *)(arg0 + 0x2C) = 0xFFFC0000;
    arg0[0x44] = 0xA0;
    arg0[0x45] = 0xD0;
    *(s16 *)(arg0 + 0x10) = 3;
    *(s16 *)(arg0 + 0x40) = 0xA;
    *(s16 *)(arg0 + 0x42) = 0x3C81;
    *(s16 *)(arg0 + 0x46) = 0;
}

void func_80146DC8(u8 *arg0) {
    u32 v = (u16)++*(u16 *)(arg0 + 0x46);

    if (v >= 0x15) {
        arg0[0x44] = 0xC0;
    } else if (v >= 0xB) {
        arg0[0x44] = 0xB0;
    } else {
        arg0[0x44] = 0xA0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80146E10);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80147074);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80147150);

u8 *func_80147380(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x68;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801473BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801473E8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014742C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014743C);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801475C8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80147644);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801478D0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80147A18);

void func_80147CE0(void) {
    s32 i = 0;
    u8 *p = D_80158A78;

    do {
        i += 1;
        p += 0xA4;
        p[-0xA4] = 0;
    } while (i != 3);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80147D08);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80147F14);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80147FD0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801480BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148154);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148210);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148244);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148460);

void func_80148584(u16 arg0, s32 arg1) {
    u32 t = arg0;
    u8 *p = D_8015694C[arg1];

    if (t == 0x8CA0) {
        p[0] = 9;
        p[2] = 5;
        p[3] = 9;
        p[5] = 9;
        p[6] = 9;
        return;
    }
    p[0] = t / 3600;
    p[2] = (t % 3600) / 600;
    p[3] = (t % 600) / 60;
    p[5] = (t % 60) * 10 / 60;
    p[6] = t * 10 / 6 % 10;
}

void func_801486D8(s32 arg0, s32 arg1) {
    func_800AE0A0(D_8015694C[arg1], arg0, 3);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148714);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148764);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801487D0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148974);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148A04);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148B6C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148C04);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148C94);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148DB0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148DE4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80148E7C);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80149108);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801491F0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80149400);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801494D8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80149704);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_801498C8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80149B20);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80149BB0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80149D64);

void func_80149E44(void) {
    u8 *p = D_80158A74;

    *(s32 *)(p + 0x24) = func_800A0070(*(u16 *)(D_80158A74 + 0x6A)) * 0x78 + 0x9C0000;
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    *(u16 *)(p + 0x6A) += 0x10;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_80149EA0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014A000);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014A198);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014A2B4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014A7E0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014A814);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014A958);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014AB48);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014ACEC);

u8 *func_8014AF00(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x78;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014AF3C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014AF70);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014AFB8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014B088);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014B198);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014B274);

void func_8014B414(u8 *arg0) {
    if (D_80158A6C[0xB] != 0 && (u32)*(u16 *)(arg0 + 0x5E) >= 0x19) {
        if (arg0[0x60] == 0) {
            *(u16 *)(arg0 + 0x5E) = 0;
            arg0[0x60] += 1;
            func_80133D14(*(s16 *)(arg0 + 0x14), *(s16 *)(arg0 + 0x16), 0x20, 0xA0);
        }
    }
    if (arg0[0x51] != 0) {
        *(u16 *)(arg0 + 0x5E) += 1;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014B4A4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014B5A4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014B67C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014B7A8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014BA54);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014BB30);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014BC74);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014BD68);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014BE60);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014BF54);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014C0B0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014C1BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014C2F8);

void func_8014C4AC(u8 *arg0) {
    s16 v;

    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    if (arg0[0x51] != 0) {
        if ((v = *(s16 *)(arg0 + 0x14)) < -0x90 || v >= 0x91 || (v = *(s16 *)(arg0 + 0x16)) < -0x10 || v >= 0x101) {
            *(s16 *)(arg0 + 4) = 0x80;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            arg0[0x51] = 0;
        }
    } else {
        v = *(s16 *)(arg0 + 0x14);
        if (v >= -0x7F && v < 0x80) {
            v = *(s16 *)(arg0 + 0x16);
            if (v > 0 && v < 0xF0) {
                arg0[0x51] = 1;
            }
        }
    }
}

void func_8014C580(u8 *arg0) {
    func_80133AD0(0x60C, arg0);
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(u8 **)(arg0 + 0x38) = D_80151A50;
    arg0[0x3C] = 0;
    arg0[0x3D] = 2;
    arg0[0x3E] = 9;
    arg0[0x40] = 0x20;
    *(s16 *)(arg0 + 0x6E) = 0;
    *(s16 *)(arg0 + 0xA) = 0;
    *(s16 *)(arg0 + 8) = 0;
    *(s16 *)(arg0 + 6) = 0;
    arg0[0xF] = 0;
    arg0[0xE] = 0;
    arg0[0xD] = 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014C5F8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014C680);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014C84C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014CBD0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014CC04);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014CDBC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014CFEC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014D168);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT", func_8014D260);
