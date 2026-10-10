#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80144D70", func_80144D70);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80144D70", func_80145370);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80144D70", func_801455C8);
