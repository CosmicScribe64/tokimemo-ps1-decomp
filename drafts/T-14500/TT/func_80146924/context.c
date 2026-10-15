#include "common.h"
#include "ovl/TT.h"

s32 func_80145E60(void) {
    u8 *a;
    u8 *b;
    u16 v;
    u16 k;

    a = D_80158A60;
    b = D_80158A6C;
    v = *(u16 *)(b + 8);
    if (v < *(u16 *)(a + 0x30)) {
        *(u16 *)(a + 0x3C) = *(u16 *)(a + 0x36);
        *(u16 *)(a + 0x36) = *(u16 *)(a + 0x30);
        *(u16 *)(a + 0x30) = *(u16 *)(b + 8);
        func_800AE0A0(a + 0x3E, a + 0x38, 3);
        func_800AE0A0(a + 0x38, a + 0x32, 3);
        *(u16 *)(b + 0x18) = 0;
    } else if (v < *(u16 *)(a + 0x36)) {
        *(u16 *)(a + 0x3C) = *(u16 *)(a + 0x36);
        *(u16 *)(a + 0x36) = *(u16 *)(b + 8);
        func_800AE0A0(a + 0x3E, a + 0x38, 3);
        *(u16 *)(b + 0x18) = 1;
    } else if (v < *(u16 *)(a + 0x3C)) {
        *(u16 *)(a + 0x3C) = *(u16 *)(b + 8);
        *(u16 *)(b + 0x18) = 2;
    } else {
        return 0;
    }
    a[*(u16 *)(b + 0x18) * 6 + 0x32] = 0xA;
    a[*(u16 *)(b + 0x18) * 6 + 0x33] = 0x2F;
    a[*(u16 *)(b + 0x18) * 6 + 0x34] = 0x2F;
    a[*(u16 *)(b + 0x18) * 6 + 0x35] = 0xFF;
    k = *(u16 *)(b + 0x18);
    func_801486D8(a + 0x32 + k * 6, k + 0x2B);
    return 1;
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-11820, 7 instructions (registers only): the D_80158A6C local c gets a1 and the record constant 8 (u16 stores) a0 where the original has a0/v1; regdiff: the 8 web (priority 2) is coloured before c (priority 1). The u16 view of the three record stores splits the constant 8 from the s16 one in s6. Tried: empty if (c) uses, other assignment and declaration orders. */
void func_80146000(void) {
    u8 *src;
    u8 *p;
    s32 i;
    s32 angle;
    u8 *c;
    u8 *a;
    u8 *b;

    c = D_80158A6C;
    src = D_80158AB0;
    a = D_80158AA8;
    *(s16 *)(a + 0x14A) = -0x40;
    *(s16 *)(a + 0x14C) = 0x48;
    *(s16 *)(a + 0x14E) = 0x11;
    a[0x142] = 1;
    a[0x140] = 1;
    *(u16 *)(a + 0x146) = 8;
    *(s16 *)(a + 0x150) = 0;
    a += 0x154;
    *(s16 *)(a + 0xA) = 0;
    *(s16 *)(a + 0xC) = 0x58;
    *(s16 *)(a + 0xE) = 0x26;
    a[2] = 1;
    a[0] = 1;
    *(u16 *)(a + 6) = 8;
    *(s16 *)(a + 0x10) = 0;
    b = D_80158AA4;
    b[0] = 1;
    b[1] = 0;
    *(u16 *)(b + 0x44) = 8;
    *(s16 *)(b + 0x14) = 0x34;
    *(s16 *)(b + 0x16) = *(u16 *)(c + 0x18) * 16 + 0x7E;
    *(s16 *)(b + 0x3C) = 8;
    *(s16 *)(b + 0x3E) = 8;
    b[0x46] = 0x30;
    b[0x47] = 0x10;
    *(s16 *)(b + 0x40) = 0xA;
    *(s16 *)(b + 0x42) = 0x3C0F;
    b[0x3B] = 0x65;
    *(s16 *)(b + 0x10) = 0;
    for (i = 0, angle = 0, p = D_80158AA4 + 0x1900; i < 3; i++, angle += 0x55, p += 0x64) {
        p[0] = 1;
        p[1] = 0;
        p[0x3B] = 0x80;
        *(u8 **)(p + 0x48) = src;
        p[0x4D] = 0;
        p[0x4E] = 1;
        p[0x38] = 0x40;
        p[0x39] = 0x40;
        p[0x3A] = 0x40;
        p[0x50] = 0;
        *(s16 *)(p + 0x14) = *(s16 *)(src + 4) - 0x24;
        *(s16 *)(p + 0x16) = *(s16 *)(src + 6) - 0x24;
        *(s16 *)(p + 0x14) = func_800A0140(angle) / 128;
        *(s16 *)(p + 0x16) = func_800A0070(angle) / 48 + 0x78;
        p[0x46] = (i & 0xF) * 8;
        p[0x47] = (i / 16) * 8;
        p[0x4F] = 0;
        *(s16 *)(p + 0x3C) = 8;
        *(s16 *)(p + 0x3E) = 8;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = 0x3C00;
        p[0x4C] = 0;
        *(s16 *)(p + 0x10) = 0xD;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_80146000);
#endif

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

void func_801462AC(void) {
    u8 *p;
    u8 *q;
    s32 i;

    p = D_80158A64;
    p[0x1D] += 4;
    p[0x1E] = p[0x1F] = p[0x1D];
    if (p[0x1D] >= 0x80U) {
        q = D_80158AA8;
        for (i = 0; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 0;
        }
        q = D_80158AA4;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 0;
            q += 0x64;
        }
        *(s16 *)(p + 2) = 0x40;
    }
    func_80146224();
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_80146348);

void func_8014687C(void) {
    u8 *p;
    u8 *q;
    s32 i;

    p = D_80158A64;
    if (p[0x1D] == 0x80U) {
        i = 0;
        q = D_80158AA8;
        for (; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 8;
        }
        q = D_80158AA4;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 8;
            q += 0x64;
        }
    }
    p[0x1D] -= 4;
    p[0x1E] = p[0x1F] = p[0x1D];
    if (p[0x1D] == 0) {
        p[9] = 1;
    }
    func_80146224();
}

void func_80146924(void) {
    u8 *sp3C;
    u8 sp37;
    u16 temp_t5;
    u16 temp_t8;
    u16 temp_v0;
    u16 temp_v0_2;
    u8 *temp_s0;
    u8 *temp_v1;

    temp_s0 = D_80158A64;
    sp3C = D_80158A6C;
    temp_v0 = *(u16 *)((unsigned char *)temp_s0 + (2));
    switch (temp_v0) {
    case 0x0:
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750(4);
        func_80044750(0x206);
        *(s8 *)((unsigned char *)temp_s0 + (28)) = 1;
        func_8014CBD0();
        func_80148210();
        func_80143570();
        func_8014A7E0();
        func_80146BE0();
        func_80148DB0();
        func_80147CE0();
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        func_8013BD4C();
        func_801473BC();
        func_801426F0();
        *(s8 *)((unsigned char *)temp_s0 + (17)) = 0;
        if (func_80145E60() != 0) {
            *(s8 *)((unsigned char *)temp_s0 + (29)) = 0;
            *(s8 *)((unsigned char *)temp_s0 + (30)) = 0;
            *(s8 *)((unsigned char *)temp_s0 + (31)) = 0;
            func_80146000();
            func_80132198(2, 6, 4);
            func_8013230C(0xC, 1);
            *(s16 *)((unsigned char *)sp3C + (30)) = 0;
        } else {
            *(u8 *)((unsigned char *)temp_s0 + (9)) = 1U;
        }
        *(u16 *)((unsigned char *)temp_s0 + (38)) = 0xE10U;
        *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x20;
        break;
    case 0x20:
        func_801462AC();
        break;
    case 0x40:
        func_80146224();
        func_80146348();
        if ((*(u8 *)((unsigned char *)temp_s0 + (10)) != 0) || (temp_t5 = *(u16 *)((unsigned char *)temp_s0 + (38)) - 1, *(u16 *)((unsigned char *)temp_s0 + (38)) = temp_t5, ((temp_t5 & 0xFFFF) == 0))) {
            *D_80158AA4 = 0;
            *(u8 *)((unsigned char *)temp_s0 + (10)) = 0U;
            *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x80;
            *(u16 *)((unsigned char *)temp_s0 + (40)) = 0x3CU;
            temp_v0_2 = *(u16 *)((unsigned char *)sp3C + (24));
            temp_v1 = (*(u16 *)((unsigned char *)sp3C + (24)) * 0x3C) + D_80158AA8;
            switch (temp_v0_2) {
            case 0:
                sp37 = 2;
                break;
            case 1:
                sp37 = 6;
                break;
            case 2:
                sp37 = 4;
                break;
            }
            *(u8 *)((unsigned char *)temp_v1 + (42)) = sp37;
            *(u8 *)((unsigned char *)temp_v1 + (22)) = sp37;
            *(u8 *)((unsigned char *)temp_v1 + (2)) = sp37;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x70;
        }
        break;
    case 0x70:
        func_80146224();
        temp_t8 = *(u16 *)((unsigned char *)temp_s0 + (40)) - 1;
        *(u16 *)((unsigned char *)temp_s0 + (40)) = temp_t8;
        if (!(temp_t8 & 0xFFFF)) {
            *(s8 *)((unsigned char *)temp_s0 + (29)) = 0x80;
            *(s8 *)((unsigned char *)temp_s0 + (30)) = 0x80;
            *(s8 *)((unsigned char *)temp_s0 + (31)) = 0x80;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x80;
        }
        break;
    case 0x80:
        func_8014687C();
        break;
    }
    if (*(u8 *)((unsigned char *)temp_s0 + (9)) != 0) {
        (*(s32 *)temp_s0) = 7;
        *(s16 *)((unsigned char *)temp_s0 + (6)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (2)) = 0;
        *(s8 *)((unsigned char *)temp_s0 + (11)) = 0;
        *(u8 *)((unsigned char *)temp_s0 + (10)) = 0U;
        *(u8 *)((unsigned char *)temp_s0 + (9)) = 0U;
    }
    func_80148714((s32) ((u16) *(u16 *)((unsigned char *)temp_s0 + (38)) / 60U), 0x26);
}
