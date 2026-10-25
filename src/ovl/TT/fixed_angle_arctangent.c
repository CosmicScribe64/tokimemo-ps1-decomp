/* fixed_angle_arctangent.c: src/ovl/TT/8014D260.c:5: lookup-table arctangent with octant and sign restoration; body 11: neg_x = 0; 12: neg_y = 0;. Original object 0x8014D260-0x8014D3F0 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/tt.h"

/* PsyQ LIBGTE ratan2, built with gcc 2.7.2-psx (config/toolchains.txt, T-24086). */
s32 func_8014D260(s32 y, s32 x) {
    s32 neg_x;
    s32 neg_y;
    s32 q;
    s32 v;

    neg_x = 0;
    neg_y = 0;
    if (x < 0) {
        neg_x = 1;
        x = -x;
    }
    if (y < 0) {
        neg_y = 1;
        y = -y;
    }
    if (x == 0 && y == 0) {
        return 0;
    }
    if (y < x) {
        if (y & 0x7FE00000) {
            q = y / (x >> 10);
        } else {
            q = (y << 10) / x;
        }
        v = D_80159B20[q];
    } else {
        if (x & 0x7FE00000) {
            q = x / (y >> 10);
        } else {
            q = (x << 10) / y;
        }
        v = 1024 - D_80159B20[q];
    }
    if (neg_x) {
        v = 2048 - v;
    }
    if (neg_y) {
        v = -v;
    }
    return v;
}
