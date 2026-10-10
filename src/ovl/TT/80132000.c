#include "common.h"
#include "ovl/TT.h"

void func_80132000(u8 *arg0, s8 *arg1, s8 *arg2) {
    u8 *e;
    u8 b;
    u8 c;

    if (arg0[8] & 0x80) {
        arg0[8] &= 0xFF7F;
        arg0[6] = 0;
        goto step;
    }
    if (arg0[8] & 0x20) {
        arg0[8] &= 0xFFDF;
        goto step;
    }
    if (!(arg0[8] & 0x40)) {
        if (--arg0[9] == 0) {
            arg0[6] += 1;
step: /* the three flag paths join here, as in the original control flow */
            e = arg0[6] * 3 + *(u8 **)arg0;
            b = *e;
            arg0[9] = b;
            if (b == 0xFE) {
                arg0[6] = 0;
                e = *(u8 **)arg0;
                arg0[9] = *e;
                arg0[8] |= 1;
            }
            *arg1 = (e[1] & 7) * 0x10 + 0x80;
            *arg2 = (e[1] & 0xFFF8) * 2;
            e += 2;
            c = *e;
            if (c != 0xFF) {
                arg0[4] = c;
            }
        }
    }
}

void func_801320F0(u8 *arg0, u8 *arg1) {
    s32 dx = *(s32 *)(arg1 + 0xC) - *(s32 *)(arg0 + 0xC);
    s32 dy = *(s32 *)(arg1 + 0x10) - *(s32 *)(arg0 + 0x10);

    func_8014D260(dy >> 16, dx >> 16);
}
