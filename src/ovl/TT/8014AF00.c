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

void func_8014AFB8(u8 *arg0, u16 arg1, s32 arg2) {
    arg0[0] = 1;
    arg0[0x51] = 0;
    *(u16 *)(arg0 + 2) = arg1;
    *(s16 *)(arg0 + 0x58) = 0xA;
    *(s32 *)(arg0 + 0x38) = (&D_8014E34C)[arg2];
    arg0[0x40] = 0x80;
    switch (arg1) {
    case 0x40:
        *(s16 *)(arg0 + 0x5A) = 0x3CC0;
        return;
    case 0x41:
        *(s16 *)(arg0 + 0x5A) = 0x3CC1;
        return;
    case 0x42:
        *(s16 *)(arg0 + 0x5A) = 0x3CC1;
        return;
    case 0x43:
        *(s16 *)(arg0 + 0x5A) = 0x3CC2;
        return;
    case 0x44:
        *(s16 *)(arg0 + 0x5A) = 0x3CC3;
        return;
    case 0x45:
        *(s16 *)(arg0 + 0x5A) = 0x3CC5;
        return;
    case 0x46:
        *(s16 *)(arg0 + 0x5A) = 0x3CC9;
        return;
    case 0x47:
        *(s16 *)(arg0 + 0x5A) = 0x3CC6;
        return;
    case 0x48:
        *(s16 *)(arg0 + 0x5A) = 0x3CC5;
        return;
    }
}

void func_8014B088(u8 arg0) {
    u32 size;
    s32 i;
    u8 *p;
    s32 a2;

    size = 0x40;
    /* Wide local preserves the u16 call conversion. T-9570 */
    p = D_80158A98;
    i = 0;
    do {
        p = func_8014AF00(p, p + 0x1E00);
        if (p == 0) {
            return;
        }
        if (arg0 != 0) {
            a2 = 4;
            *(s32 *)(p + 0x20) = 0x500000;
        } else {
            a2 = 3;
            *(s32 *)(p + 0x20) = 0xFFB00000;
        }
        *(s32 *)(p + 0x24) = (-0x10 - i * 0x10) << 16;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x20000;
        p[0x63] = arg0;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        func_8014AFB8(p, size, a2);
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = size;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 6);
}

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

void func_8014B7A8(u8 *arg0) {
    s16 idx;
    u8 *e;

    switch (*(u16 *)(arg0 + 6)) {
    case 0:
        if (arg0[0x63] != 0) {
            arg0[0x5C] = 0xD0;
            arg0[0x5D] = 0x30;
            arg0[0x3C] = 0;
            if (*(s32 *)(arg0 + 0x20) < 0x80001) {
                *(u16 *)(arg0 + 6) = 1;
                *(u16 *)(arg0 + 0x6E) = 0x1C00;
            }
        } else {
            arg0[0x5C] = 0xD0;
            arg0[0x5D] = 0x30;
            arg0[0x3C] = 2;
            if (*(s32 *)(arg0 + 0x20) >= -0x80000) {
                *(u16 *)(arg0 + 6) = 1;
                *(u16 *)(arg0 + 0x6E) = 0xC00;
            }
        }
        break;
    case 1:
        if (arg0[0x63] != 0) {
            *(s32 *)(arg0 + 0x20) = func_800A0140(*(u16 *)(arg0 + 0x6E)) * 0x300;
            *(s32 *)(arg0 + 0x24) = func_800A0070(*(u16 *)(arg0 + 0x6E)) * 0x300 + 0x680000;
            *(s32 *)(arg0 + 0x28) = 0;
            if (*(u16 *)(arg0 + 0x6E) < 0xC00) {
                *(u16 *)(arg0 + 6) = 2;
            }
            *(u16 *)(arg0 + 0x6E) -= 0x40;
            idx = ((*(u16 *)(arg0 + 0x6E) + 0x100) & 0xFFF) / 512 * 3;
        } else {
            *(s32 *)(arg0 + 0x20) = func_800A0140(*(u16 *)(arg0 + 0x6E)) * 0x300;
            *(s32 *)(arg0 + 0x24) = func_800A0070(*(u16 *)(arg0 + 0x6E)) * 0x300 + 0x680000;
            *(s32 *)(arg0 + 0x28) = 0;
            if (*(u16 *)(arg0 + 0x6E) > 0x1C00) {
                *(u16 *)(arg0 + 6) = 2;
            }
            *(u16 *)(arg0 + 0x6E) += 0x40;
            idx = ((*(u16 *)(arg0 + 0x6E) + 0x900) & 0xFFF) / 512 * 3;
        }
        e = D_80158AC0 + idx;
        arg0[0x5C] = e[0];
        arg0[0x5D] = e[1];
        arg0[0x3C] = e[2];
        break;
    case 2:
        if (arg0[0x63] != 0) {
            *(u16 *)(arg0 + 0x6E) = 0;
            *(s32 *)(arg0 + 0x28) = 0xFFFC0000;
            arg0[0x5C] = 0xD0;
            arg0[0x5D] = 0x30;
            arg0[0x3C] = 0;
        } else {
            *(u16 *)(arg0 + 0x6E) = 0;
            *(s32 *)(arg0 + 0x28) = 0x40000;
            arg0[0x5C] = 0xD0;
            arg0[0x5D] = 0x30;
            arg0[0x3C] = 2;
        }
        break;
    }
    if (*(u16 *)(arg0 + 0x5E) > 0x3C) {
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

void func_8014BB30(TtActor78 *arg0) {
    s32 value;
    u8 flag;

    if ((s32)(func_800A0070(arg0->unk_24.w / 3072) << 9) < 0) {
        value = -(func_800A0070(arg0->unk_24.w / 3072) << 9);
    } else {
        value = func_800A0070(arg0->unk_24.w / 3072) << 9;
    }
    arg0->unk_20.w = (value + 0x100000) * (arg0->unk_50 - 2);
    if (((TtActorTail10 *)arg0->pad_5E)->unk_00 >= 0x19U) {
        flag = ((TtActorTail10 *)arg0->pad_5E)->unk_02;
        if (flag == 0) {
            ((TtActorTail10 *)arg0->pad_5E)->unk_00 = 0;
            ((TtActorTail10 *)arg0->pad_5E)->unk_02 = flag + 1;
            func_80133D14(arg0->unk_14, arg0->unk_16, 0x20, 0xA0);
        }
    }
    if (arg0->unk_51 != 0) {
        ((TtActorTail10 *)arg0->pad_5E)->unk_00 += 1;
    }
}

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

void func_8014BF54(void *arg0) {
    s32 shifted;
    s32 factor;
    s32 value;
    u16 state;
    u16 counter;
    u8 flag;

    state = *(u16 *)((u8 *)arg0 + 0x6);
    switch (state) {
    case 0:
        factor = *(u8 *)((u8 *)arg0 + 0x50) - 2;
        value = *(s32 *)((u8 *)arg0 + 0x24);
        shifted = value >> 0x10;
        *(s32 *)((u8 *)arg0 + 0x20) = (s32) ((factor << 0x15) + (factor * shifted * shifted * 0x30));
        if (value < 0x540000) {
            *(s32 *)((u8 *)arg0 + 0x2c) = 0;
            *(u16 *)((u8 *)arg0 + 0x6) = 1U;
            *(u16 *)((u8 *)arg0 + 0x6e) = 0x400U;
        }
        *(u8 *)((u8 *)arg0 + 0x3c) = 1;
        break;
    case 1:
        *(s32 *)((u8 *)arg0 + 0x24) = (s32) ((func_800A0140((s32) *(u16 *)((u8 *)arg0 + 0x6e)) << 9) + 0x500000);
        counter = *(u16 *)((u8 *)arg0 + 0x6e);
        if (counter >= 0xC81U) {
            *(s32 *)((u8 *)arg0 + 0x2c) = 0x20000;
            *(u16 *)((u8 *)arg0 + 0x6e) = 0U;
            *(u16 *)((u8 *)arg0 + 0x6) = 2U;
            counter = *(u16 *)((u8 *)arg0 + 0x6e);
        }
        *(u16 *)((u8 *)arg0 + 0x6e) = (u16) (counter + 0x30);
        *(u8 *)((u8 *)arg0 + 0x3c) = 0;
        break;
    case 2:
        if ((u16) *(u16 *)((u8 *)arg0 + 0x5e) >= 5U) {
            flag = *(u8 *)((u8 *)arg0 + 0x60);
            if (flag == 0) {
                *(u16 *)((u8 *)arg0 + 0x5e) = 0U;
                *(u8 *)((u8 *)arg0 + 0x60) = (u8) (flag + 1);
                func_80133D14((s32) *(s16 *)((u8 *)arg0 + 0x14), (s32) *(s16 *)((u8 *)arg0 + 0x16), 0x20, 0xA0);
            }
        }
        if (*(u8 *)((u8 *)arg0 + 0x51) != 0) {
            *(u16 *)((u8 *)arg0 + 0x5e) = (u16) (*(u16 *)((u8 *)arg0 + 0x5e) + 1);
        }
        break;
    }
}

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

void func_8014C2F8(void) {
    TtCmdStream *g;

    g = (TtCmdStream *)D_80158A70;
    while (1) {
        if (*(s16 *)g->cmd == -1) {
            return;
        }
        if (g->time < (u32) * (s16 *)g->cmd) {
            return;
        }
        switch (*(u16 *)(g->cmd + 2)) {
        case 0xC0:
            func_8014B088(g->cmd[4]);
            break;
        case 0xC1:
            func_8014B274(g->cmd[4]);
            break;
        case 0xC2:
            func_8014B4A4(g->cmd[4]);
            break;
        case 0xC3:
            func_8014B67C(g->cmd[4]);
            break;
        case 0xC4:
            func_8014BA54(g->cmd[4]);
            break;
        case 0xC5:
            func_8014BC74(g->cmd[4]);
            break;
        case 0xC6:
            func_8014BE60(g->cmd[4]);
            break;
        case 0xC7:
            func_8014C0B0(g->cmd[4]);
            break;
        case 0x30:
        case 0x31:
            func_8014743C((s8)g->cmd[4], *(u16 *)(g->cmd + 2), g->cmd[5]);
            break;
        }
        g->cmd += 6;
    }
}

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

/* NON_MATCHING: T-10850, 16 differing listing lines; regdiff evidence in the ticket scratch. */
#ifdef NON_MATCHING
/* FAKE: shared zero local preserves constant allocation. T-10850 */
void func_8014C5F8(u8 *arg0) {
    u8 *p;
    s32 zero;

    p = (u8 *)func_8014AF00(D_80158A98, D_80158A98 + 0x1E00);
    if (p != 0) {
        zero = 0;
        p[0] = 1;
        *(s16 *)(p + 4) = 0x71;
        p[0xD] = p[0xE] = zero;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        *(s32 *)(p + 0x20) = *(s32 *)(arg0 + 0xC);
        *(s32 *)(p + 0x24) = *(s32 *)(arg0 + 0x10);
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        func_8014C580(p);
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014C5F8);
#endif

/* FAKE: permuter source layout preserves branch operand order; original source unknown. T-9710. */
void func_8014C680(void)
{
    s32 var_s1;
    u16 temp_v0;

    TtActor78 *var_s0;
    func_8014C2F8();
    var_s0 = (TtActor78 *) D_80158A98;
    var_s1 = 0;
    do
    {
        if (var_s0->unk_00 != 0)
        {
            temp_v0 = var_s0->unk_04;
            switch (temp_v0)
            {
            case 0x71:
                break;

            case 0x40:
                switch (var_s0->unk_02)
                {
                case 0x40:
                    func_8014B198((u8 *) var_s0);
                    break;

                case 0x41:

                case 0x42:
                    func_8014B414((u8 *) var_s0);
                    break;

                case 0x43:
                    func_8014B5A4((u8 *) var_s0);
                    break;

                case 0x44:
                    func_8014B7A8((s32) var_s0);
                    break;

                case 0x45:
                    func_8014BB30((s32) var_s0);
                    break;

                case 0x46:
                    func_8014BD68((u8 *) var_s0);
                    break;

                case 0x47:
                    func_8014BF54((void *) var_s0);
                    break;

                case 0x48:
                    func_8014C1BC((u8 *) var_s0);
                    break;

                }

                break;

            case 0x70:
                func_8014C580((u8 *) var_s0);
                var_s0->unk_04 = 0x71U;
                var_s0->unk_0A = 0;
                var_s0->unk_08 = 0;
                var_s0->unk_06 = 0;
                var_s0->unk_0F = 0;
                var_s0->unk_0E = 0;
                var_s0->unk_0D = 0;
                break;

            case 0x80:
                var_s0->unk_00 = 0;
                var_s0->unk_51 = 0;
                break;

            }

            func_8014C4AC((u8 *) var_s0);

            if (0x40 == var_s0->unk_04)
            {
                func_80132000((s32) (&var_s0->unk_38), (s32) (&var_s0->unk_5C), (s32) (&var_s0->unk_5D));
            }
            else
            if (0x71 == var_s0->unk_04)
            {
                func_801478D0((s32) (&var_s0->unk_38));
                if (var_s0->unk_40.b.lo & 1)
                {
                    var_s0->unk_00 = 0;
                }
            }
        }
        var_s1 += 1;
        var_s0 += 1;
    }
    while (var_s1 != 0x40);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014C84C);

void func_8014CBD0(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158AA4; i < 0x80; i++, p += 0x64) {
        *p = 0;
    }
}

/* NON_MATCHING: T-9380, one instruction: the first ordering-table address is `addu t9,t8,v0` here, `addu t9,v0,t8` in the original (all else matches; TtOtTag is the 24-bit address field of a GPU tag) */
#ifdef NON_MATCHING
void func_8014CC04(u8 *arg0) {
    u8 *p;
    u8 *v;
    u32 r;
    u32 g;
    u32 b;

    p = *(s32 *)(D_80158A68 + 8) * 0x80B8 + D_80158AB4;
    v = *(u8 **)(p + 4);
    *(u8 **)(p + 4) = v + 0x10;
    v[3] = 3;
    v[7] = arg0[0x3B];
    if (*(u16 *)(arg0 + 0x44) & 8) {
        r = (arg0[0x38] * D_80158A64[0x1D]) >> 7;
        if ((s32)r >= 0x100) {
            r = 0xFF;
        }
        g = (arg0[0x39] * D_80158A64[0x1E]) >> 7;
        if ((s32)g >= 0x100) {
            g = 0xFF;
        }
        b = (arg0[0x3A] * D_80158A64[0x1F]) >> 7;
        if ((s32)b >= 0x100) {
            b = 0xFF;
        }
        v[4] = r;
        v[5] = g;
        v[6] = b;
    } else {
        v[4] = arg0[0x38];
        v[5] = arg0[0x39];
        v[6] = arg0[0x3A];
    }
    *(s16 *)(v + 8) = *(s16 *)(arg0 + 0x14);
    *(s16 *)(v + 0xA) = *(s16 *)(arg0 + 0x16);
    *(s16 *)(v + 0xC) = *(s16 *)(arg0 + 0x3C);
    *(s16 *)(v + 0xE) = *(s16 *)(arg0 + 0x3E);
    ((TtOtTag *)v)->addr = *(u32 *)(p + *(u16 *)(arg0 + 0x10) * 4 + 0x8008);
    ((TtOtTag *)(p + *(u16 *)(arg0 + 0x10) * 4 + 0x8008))->addr = (u32)v;
}
#else
void func_8014CC04(TtActor78 *arg0) {
    TtDrawBuffer80B8 *p;
    TILE *v;
    u32 r;
    u32 g;
    u32 b;

    p = ((TtDrawSelect0C *)D_80158A68)->index + (TtDrawBuffer80B8 *)D_80158AB4;
    v = (TILE *)p->cursor;
    p->cursor = (DR_MODE *)(v + 1);
    ((TtOtTag *)v)->len = 3;
    v->code = arg0->unk_3B;
    if (arg0->unk_44 & 8) {
        r = (arg0->unk_38 * D_80158A64[0x1D]) >> 7;
        if ((s32)r >= 0x100) {
            r = 0xFF;
        }
        g = (arg0->unk_39 * D_80158A64[0x1E]) >> 7;
        if ((s32)g >= 0x100) {
            g = 0xFF;
        }
        b = (arg0->unk_3A * D_80158A64[0x1F]) >> 7;
        if ((s32)b >= 0x100) {
            b = 0xFF;
        }
        v->r0 = r;
        v->g0 = g;
        v->b0 = b;
    } else {
        v->r0 = arg0->unk_38;
        v->g0 = arg0->unk_39;
        v->b0 = arg0->unk_3A;
    }
    v->x0 = arg0->unk_14;
    v->y0 = arg0->unk_16;
    v->w = arg0->unk_3C;
    v->h = arg0->unk_3E;
    ((TtOtTag *)v)->addr = *(u32 *)&p->ordering[arg0->unk_10];
    (&p->ordering[arg0->unk_10])->addr = (u32)v;
}
#endif

void func_8014CDBC(u8 *arg0) {
    DR_MODE *m;
    TtDrawBuffer80B8 *buffer;
    TtSpritePacket20 *q;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    q = (TtSpritePacket20 *)buffer->cursor;
    buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
    q->tag.len = 4;
    if (*(u16 *)(arg0 + 0x44) & 8) {
        TtCore30 *core;

        core = (TtCore30 *)D_80158A64;
        q->code = arg0[0x3B] & 0xFFFE;
        q->r = core->unk_1D;
        q->g = core->unk_1E;
        q->b = core->unk_1F;
    } else {
        q->code = arg0[0x3B];
        q->r = arg0[0x38];
        q->g = arg0[0x39];
        q->b = arg0[0x3A];
    }
    q->x = *(s16 *)(arg0 + 0x14);
    q->y = *(s16 *)(arg0 + 0x16);
    q->w = *(s16 *)(arg0 + 0x3C);
    q->h = *(s16 *)(arg0 + 0x3E);
    q->u = arg0[0x46];
    q->v = arg0[0x47];
    q->clut = *(u16 *)(arg0 + 0x42);
    q->tag.addr = buffer->ordering[*(u16 *)(arg0 + 0x10)].addr;
    buffer->ordering[*(u16 *)(arg0 + 0x10)].addr = (u32)q;
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, *(u16 *)(arg0 + 0x40), 0);
    ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(arg0 + 0x10)].addr;
    buffer->ordering[*(u16 *)(arg0 + 0x10)].addr = (u32)m;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8014AF00", func_8014CFEC);

/* FAKE: `if (1)` after the second case and the bound in the local n give the original's constant registers (the bound 0x80 apart from case 0x80); found with the permuter, T-9380 */
void func_8014D168(void) {
    s32 i;
    s32 n;
    u8 *p;

    p = D_80158AA4;
    i = 0;
    n = 0x80;
    do {
        if ((p[0] != 0) && (p[1] == 0)) {
            switch (p[0x3B] & ~3) {
            case 0x60:
                func_8014CC04(p);
                break;
            case 0x64:
                func_8014CDBC(p);
                if (1) {
                    break;
                }
            case 0x80:
                func_8014CFEC(p);
                break;
            }
        }
        i += 1;
        p += 0x64;
    } while (i != n);
}
