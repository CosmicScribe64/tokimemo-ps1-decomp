/* NON_MATCHING: T-10720/T-11800, 7 lines (registers only): the hoisted loop constants 0x40/0x20 sit in t0/t1 where the original has t1/t0 (unconstrained ranges coloured in bit order; 0x40 is created first). T-11800 (order_search): writing the hit-branch stores as 0x20, 0x1C, 0x44, then p[0x64] fixes the li/sh order (was 11 lines). Tried: do/while loops, for (i = 0, p = ...) heads. */
void func_80148E7C(arg0, arg1)
u8 *arg0;
s32 arg1;
{
    u8 *src;
    s32 *pp;
    u8 *p;
    s32 i;
    s32 d;
    s32 dx;
    s32 e;

    pp = D_80158A70;
    src = D_80158A74;
    if (arg1 < 2) {
        *(s32 *)(arg0 + 0x20) = *(s16 *)(src + 0x14) << 16;
        *(s32 *)(arg0 + 0x1C) = *pp - ((*(s16 *)(src + 0x16) - 0x60) << 16);
        *(u16 *)(arg0 + 0x44) = 0xFFFF;
        i = 0;
        p = D_80158A9C;
        for (; i != 0x20; i++, p += 0x78) {
            if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && p[0x64] == 0) {
                d = *(s16 *)(src + 0x16) - *(s16 *)(p + 0x16);
                if (d < 0x60 && d >= 0x11) {
                    e = *(s16 *)(p + 0x14) - *(s16 *)(src + 0x14);
                    if (e < 0) {
                        dx = -e;
                    } else {
                        dx = e;
                    }
                    if (dx < 0x20) {
                        *(s32 *)(arg0 + 0x20) = *(s32 *)(p + 0x20);
                        *(s32 *)(arg0 + 0x1C) = *(s32 *)(p + 0x1C);
                        *(s16 *)(arg0 + 0x44) = i;
                        p[0x64] = 1;
                        break;
                    }
                }
            }
        }
    } else {
        src = D_80158A78 + (arg1 >> 1) * 0xA4 - 0xA4;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(src + 0x14) << 16;
        *(s32 *)(arg0 + 0x1C) = *pp - ((*(s16 *)(src + 0x16) - 0x60) << 16);
        *(u16 *)(arg0 + 0x44) = 0xFFFF;
        i = 0;
        p = D_80158A9C;
        for (; i != 0x20; i++, p += 0x78) {
            if (p[0] != 0 && p[0x64] == 0) {
                d = *(s16 *)(src + 0x16) - *(s16 *)(p + 0x16);
                if (d < 0x60 && d >= 0x11) {
                    e = *(s16 *)(p + 0x14) - *(s16 *)(src + 0x14);
                    if (e < 0) {
                        dx = -e;
                    } else {
                        dx = e;
                    }
                    if (dx < 0x20) {
                        *(s32 *)(arg0 + 0x20) = *(s32 *)(p + 0x20);
                        *(s32 *)(arg0 + 0x1C) = *(s32 *)(p + 0x1C);
                        *(s16 *)(arg0 + 0x44) = i;
                        p[0x64] = 1;
                        break;
                    }
                }
            }
        }
    }
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(s16 *)(arg0 + 0x46) = 0;
}
