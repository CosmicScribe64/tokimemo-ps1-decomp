#include "common.h"
#include "ovl/TT.h"

u8 *func_8014AF00(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x78;
    }
    return 0;
}

void func_8014AF3C(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A98; i < 0x40; i++, p += 0x78) {
        *p = 0;
    }
}

void func_8014AF70(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A98; i < 0x40; i++, p += 0x78) {
        *p = 0;
        *(s16 *)(p + 0x10) = 4;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014AFB8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014B088);

void func_8014B198(u8 *arg0) {
    u8 *g;
    u8 t;

    g = D_80158A6C;
    if (arg0[0x63] != 0) {
        *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x24) / -64;
    } else {
        *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x24) / 64;
    }
    if ((g[0xB] != 0) && (*(u16 *)(arg0 + 0x5E) >= 0x19U)) {
        t = arg0[0x60];
        if (t == 0) {
            *(u16 *)(arg0 + 0x5E) = 0;
            arg0[0x60] = t + 1;
            func_80133D14(*(s16 *)(arg0 + 0x14), *(s16 *)(arg0 + 0x16), 0x20, 0xA0);
        }
    }
    if (arg0[0x51] != 0) {
        *(u16 *)(arg0 + 0x5E) = *(u16 *)(arg0 + 0x5E) + 1;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014B274);

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

void func_8014B4A4(u8 arg0) {
    u8 *p = D_80158A98;
    s32 i = 0;
    s32 v;

    do {
        p = func_8014AF00(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        if (arg0 != 0) {
            v = 8;
            *(s32 *)(p + 0x20) = 0x500000;
        } else {
            v = 7;
            *(s32 *)(p + 0x20) = 0xFFB00000;
        }
        *(s32 *)(p + 0x24) = (-0x10 - i * 0x10) << 16;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x10000;
        p[0x63] = arg0;
        func_8014AFB8(p, 0x43, v);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 4);
}

void func_8014B5A4(u8 *arg0) {
    if (arg0[0x63] != 0) {
        *(s32 *)(arg0 + 0x20) = -func_800A0070(*(s16 *)(arg0 + 0x16) << 5) * 0x600;
    } else {
        *(s32 *)(arg0 + 0x20) = func_800A0070(*(s16 *)(arg0 + 0x16) << 5) * 0x600;
    }
    if ((u32)*(u16 *)(arg0 + 0x5E) >= 0x11) {
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

void func_8014B67C(u8 arg0) {
    u8 *p = D_80158A98;
    s32 i = 0;

    do {
        p = func_8014AF00(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        if (arg0 != 0) {
            *(s32 *)(p + 0x20) = (i * 0x10 + 0x90) << 16;
            *(s32 *)(p + 0x28) = 0xFFFC0000;
        } else {
            *(s32 *)(p + 0x20) = (-0x90 - i * 0x10) << 16;
            *(s32 *)(p + 0x28) = 0x40000;
        }
        *(s32 *)(p + 0x24) = 0x380000;
        *(s32 *)(p + 0x2C) = 0;
        p[0x63] = arg0;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        func_8014AFB8(p, 0x44, 9);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        p[-0x38] = p[-0x38] | 0x40;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 6);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014B7A8);

void func_8014BA54(s32 arg0) {
    u8 *p = D_80158A98;
    s32 i = 0;

    do {
        p = func_8014AF00(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        *(s32 *)(p + 0x20) = 0;
        *(s32 *)(p + 0x24) = 0xFFF00000;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x20000;
        p[0x50] = i;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        func_8014AFB8(p, 0x45, 0xA);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 5);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014BB30);

void func_8014BC74(s32 arg0) {
    u8 *p = D_80158A98;
    s32 i = 0;

    do {
        p = func_8014AF00(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        *(s32 *)(p + 0x20) = (i << 21) + 0xFFC00000;
        *(s32 *)(p + 0x24) = 0xFFF00000;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x20000;
        *(s16 *)(p + 0x6E) = 0;
        p[0x50] = i;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        func_8014AFB8(p, 0x46, 0xB);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 5);
}

void func_8014BD68(u8 *arg0) {
    u32 t;

    *(s32 *)(arg0 + 0x20) = (arg0[0x50] - 2) * (func_800A0140(*(u16 *)(arg0 + 0x6E) * 0x1E) << 9);
    t = *(u16 *)(arg0 + 0x6E);
    *(s32 *)(arg0 + 0x24) = (t - 0x10) << 16;
    *(s32 *)(arg0 + 0x24) += (func_800A0070(t * 0x1E) << 9) * (arg0[0x50] - 2);
    *(u16 *)(arg0 + 0x6E) += 1;
    if ((u32)*(u16 *)(arg0 + 0x5E) >= 0x19) {
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

void func_8014BE60(s32 arg0) {
    u8 *p = D_80158A98;
    s32 i = 0;

    do {
        p = func_8014AF00(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        *(s32 *)(p + 0x20) = (i << 21) + 0xFFC00000;
        *(s32 *)(p + 0x24) = 0x01000000;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0xFFFE0000;
        *(s16 *)(p + 0x6E) = 0;
        p[0x50] = i;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        func_8014AFB8(p, 0x47, 0xC);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 5);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014BF54);

void func_8014C0B0(u8 arg0) {
    u8 *p = D_80158A98;
    s32 i = 0;

    do {
        p = func_8014AF00(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        if (arg0 != 0) {
            *(s32 *)(p + 0x20) = 0x500000;
        } else {
            *(s32 *)(p + 0x20) = 0xFFB00000;
        }
        *(s32 *)(p + 0x24) = (-0x10 - i * 0x10) << 16;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x20000;
        p[0x63] = arg0;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        func_8014AFB8(p, 0x48, 0xD);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 6);
}

void func_8014C1BC(u8 *arg0) {
    switch (*(u16 *)(arg0 + 6)) {
    case 0:
        if (*(s32 *)(arg0 + 0x24) >= *(s32 *)(D_80158A74 + 0x24)) {
            *(s32 *)(arg0 + 0x2C) = 0;
            if (arg0[0x63] != 0) {
                *(s32 *)(arg0 + 0x28) = 0xFFFE0000;
            } else {
                *(s32 *)(arg0 + 0x28) = 0x20000;
            }
            *(u16 *)(arg0 + 6) = 1;
        }
        break;
    case 1:
        if (arg0[0x63] != 0) {
            if (*(s32 *)(D_80158A74 + 0x20) >= *(s32 *)(arg0 + 0x20)) {
                *(s32 *)(arg0 + 0x28) = 0;
                *(s32 *)(arg0 + 0x2C) = 0xFFFE0000;
            }
        } else if (*(s32 *)(arg0 + 0x20) >= *(s32 *)(D_80158A74 + 0x20)) {
            *(s32 *)(arg0 + 0x28) = 0;
            *(s32 *)(arg0 + 0x2C) = 0xFFFE0000;
        }
        break;
    }
    if ((u32)*(u16 *)(arg0 + 0x5E) >= 0x19) {
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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014C2F8);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014C5F8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014C680);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014C84C);

void func_8014CBD0(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158AA4; i < 0x80; i++, p += 0x64) {
        *p = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014CC04);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014CDBC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014CFEC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014D168);
