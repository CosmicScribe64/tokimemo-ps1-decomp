#include "common.h"
#include "ovl/TT.h"

u8 *func_80132F10(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x78;
    }
    return 0;
}

void func_80132F4C(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A9C; i < 0x20; i++, p += 0x78) {
        *p = 0;
    }
}

void func_80132F80(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A9C; i < 0x20; i++, p += 0x78) {
        *p = 0;
        *(s16 *)(p + 0x10) = 0xC;
    }
}

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

/* FAKE: the two byte stores of each case share one source line; as1 schedules them as a unit, which gives the original's store order. Real source unknown. T-8010 */
void func_80133058(u8 *arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
    switch (arg1) {
    case 0x6E:
        arg0[0x5C] = 0xF0; arg0[0x5D] = 0xA0;
        *(u16 *)(arg0 + 0x5A) = 0x3C84;
        break;
    case 0x6F:
        arg0[0x5C] = 0xF0; arg0[0x5D] = 0xB0;
        *(u16 *)(arg0 + 0x5A) = 0x3C84;
        break;
    case 0x6D:
        arg0[0x5C] = 0xF0; arg0[0x5D] = 0xE0;
        *(u16 *)(arg0 + 0x5A) = 0x3C8D;
        break;
    }
}

/* T-11000 near miss: id slot at sp+0x50 instead of 0x54, beq operand order of the hoisted -1 (s3, v0), call-argument/store scheduling in the record init; about 47 listing rows. */
#ifdef NON_MATCHING
void func_801330D8(void) {
    u8 *g;
    u8 *a;
    s16 t;
    s32 id;
    s32 x;
    s32 m;

    m = -1;
    g = (u8 *)D_80158A70;
    while (1) {
        t = *(s16 *)(*(u8 **)(g + 8) + 2);
        if (m == t || *(u16 *)(g + 2) < (u32)t || (a = func_80132F10(D_80158A9C, D_80158A9C + 0xF00)) == 0) {
            break;
        }
        *(u16 *)(a + 2) = *(u16 *)(*(u8 **)(g + 8) + 4);
        switch (*(u16 *)(a + 2)) {
        case 0x60:
            id = 0xE;
            break;
        case 0x61:
            id = 0xF;
            break;
        case 0x62:
            id = 0x10;
            break;
        case 0x63:
            id = 0x11;
            break;
        }
        *(s32 *)(a + 0x20) = *(s16 *)(*(u8 **)(g + 8)) << 16;
        x = *(s16 *)(*(u8 **)(g + 8) + 2) << 16;
        *(s32 *)(a + 0x1C) = x;
        *(s32 *)(a + 0x28) = 0;
        *(s32 *)(a + 0x24) = *(s32 *)g - x;
        *(s32 *)(a + 0x2C) = 0;
        *(s16 *)(a + 0x5E) = 0;
        a[0x60] = 0;
        *(s16 *)(a + 0x14) = *(s16 *)(a + 0x22);
        *(s16 *)(a + 0x16) = *(s16 *)(a + 0x26);
        func_80132FC8(a, *(u16 *)(a + 2), id);
        *(s16 *)(a + 4) = 0x40;
        a[0x40] |= 0x40;
        *(s16 *)(a + 10) = 0;
        *(s16 *)(a + 8) = 0;
        *(s16 *)(a + 6) = 0;
        a[15] = 0;
        a[14] = 0;
        a[13] = 0;
        *(u8 **)(g + 8) += 6;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_801330D8);
#endif

void func_80133288(u8 *arg0) {
    u8 *q;
    s16 y;
    s16 x;
    s32 d;
    s32 a;

    q = D_80158A74;
    *(s32 *)(arg0 + 0x24) = *D_80158A70 - *(s32 *)(arg0 + 0x1C);
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    if (*(u16 *)(arg0 + 4) == 0x40) {
        if (*(u16 *)(arg0 + 0x5E) > 0x28 && arg0[0x60] == 0) {
            arg0[0x40] &= 0xFFBF;
        }
        if (arg0[0x40] & 1) {
            y = *(s16 *)(arg0 + 0x16);
            arg0[0x40] = 0xC0;
            *(u16 *)(arg0 + 0x5E) = 0;
            if (y < 0xC0) {
                x = *(s16 *)(arg0 + 0x14);
                d = *(s16 *)(q + 0x14) - x;
                if (d < 0) {
                    a = -d;
                } else {
                    a = d;
                }
                if (a < 0x20) {
                    d = *(s16 *)(q + 0x16) - y;
                    if (d < 0) {
                        a = -d;
                    } else {
                        a = d;
                    }
                    if (a >= 0x20) {
                        goto call;
                    }
                } else {
call:
                    arg0[0x60] += 1;
                    func_80133D14(x, y, 0x20, 0xA0);
                }
            }
        }
        *(u16 *)(arg0 + 0x5E) += 1;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_801333D4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_801336F8);
