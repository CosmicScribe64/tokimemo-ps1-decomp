#include "common.h"
#include "ovl/TACO.h"

/* NON_MATCHING: T-16562, 8 differing lines (earlier T-10510); IDO folds the entry copy of the pointer parameter (move s0,a0 at entry in the original) */
#ifdef NON_MATCHING
void func_80154230(arg0, arg1)
u32 *arg0;
u16 arg1;
{
    u32 command0;
    u32 command1;
    u32 command;
    u32 word;
    s32 count;
    u32 limit;

    count = arg0[1];
    limit = arg0[3];
    command0 = 0x24000507;
    command1 = 0x34000609;
    arg0 += 9;
loop_1:
    if (func_8015431C((u32)arg0) != 0) {
        command = arg0[0];
        if (command0 == command || command1 == command) {
            word = arg0[2];
            arg0 += 2;
            *arg0 = (word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10);
        }
        arg0++;
        if (count == 0 || limit >= (u32)arg0) {
            if (count == 0 && (u32)(limit * 4 + (u32)arg0 + 0xC) < (u32)arg0) {
            } else {
                goto loop_1;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154230", func_80154230);
#endif

s32 func_8015431C(u32 arg0) {
    if (arg0 < 0x8018A000U) {
        return 0;
    }
    if (arg0 >= 0x801B8001U) {
        return 0;
    }
    return 1;
}

void func_8015435C(s32 arg0, u16 arg1) {
    u32 **bounds;
    u32 command;
    u32 command0;
    u32 command1;
    u32 command2;
    s32 word;
    u32 *p;

    /* Unsigned command constants preserve the original comparison order. T-10030. */
    command0 = 0x24000507;
    command1 = 0x34000609;
    command2 = 0x24000014;
    bounds = (u32 **)(D_8015F290 + arg0);
    p = bounds[0];
loop_1:
    if (func_8015431C((u32)p) != 0) {
        command = p[0];
        if (command0 == command || command1 == command || command2 == command) {
            word = p[2];
            p += 2;
            *p = (s32) ((word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10));
        }
        p++;
        if ((u32) p < (u32) bounds[1]) {
            goto loop_1;
        }
    }
}

/* NON_MATCHING: T-16562, 9 differing lines; entry copy `move s0,a0` before the loads of count and limit (T-0018 family of func_80154230, row in wiki/data/t0018-cases.md) */
#ifdef NON_MATCHING
void func_8015443C(arg0, arg1, arg2, arg3)
u32 *arg0;
u16 arg1;
s32 arg2;
s32 arg3;
{
    s32 pad[2]; /* FAKE: the original frame has 8 more bytes above interval; real source unknown. T-16562 */
    s32 interval;
    u32 v;
    s32 count;
    u32 limit;
    s32 i;
    u32 word;
    u32 command;
    u32 *p;

    v = D_8015EDB4[20].unk2;
    count = arg0[1];
    limit = arg0[3];
    i = 0;
    p = arg0 + 9;
    if (v < 0x10) {
        interval = func_800AE0D0() % 150 + 100;
    } else if (v >= 0x10 && v < 0x20) {
        interval = func_800AE0D0() % 100 + 50;
    } else if (v >= 0x20 && v < 0x30) {
        interval = func_800AE0D0() % 50 + 30;
    } else if (v >= 0x30 && v < 0x40) {
        interval = func_800AE0D0() % 30 + 20;
    } else if (v >= 0x40 && v < 0x64) {
        interval = func_800AE0D0() % 10 + 5;
    }
loop_1:
    if (func_8015431C((u32)p) != 0) {
        command = p[0];
        if (command == 0x24000507 || command == 0x34000609) {
            if (D_8015EDB4[20].unk2 < 0x50) {
                if (i >= interval) {
                    word = p[2];
                    p[1] = (p[1] & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10));
                    p += 2;
                    *p = (word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10);
                    i = 0;
                }
                i++;
            } else {
                interval = func_800AE0D0() % 70;
                if (interval == 0) {
                    word = p[2];
                    p[1] = (p[1] & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10));
                    p += 2;
                    *p = (word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10);
                }
            }
        }
        p++;
        if (count == 0 || limit >= (u32)p) {
            if (count == 0 && (u32)(limit * 4 + (u32)p + 0xC) < (u32)p) {
            } else {
                goto loop_1;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154230", func_8015443C);
#endif

/* NON_MATCHING: T-16562, 8 differing lines (earlier T-10310); same entry copy of the pointer parameter as func_80154230 */
#ifdef NON_MATCHING
void func_801546FC(arg0, arg1, arg2, arg3)
u32 *arg0;
u16 arg1;
s32 arg2;
s32 arg3;
{
    u32 command0;
    u32 command1;
    u32 command;
    u32 word;
    s32 count;
    u32 limit;

    count = arg0[1];
    limit = arg0[3];
    command0 = 0x24000507;
    command1 = 0x34000609;
    arg0 += 9;
loop_1:
    if (func_8015431C((u32)arg0) != 0) {
        command = arg0[0];
        if (command0 == command || command1 == command) {
            word = arg0[2];
            arg0[1] = (arg0[1] & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10));
            arg0 += 2;
            *arg0 = (word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10);
        }
        arg0++;
        if (count == 0 || limit >= (u32)arg0) {
            if (count == 0 && (u32)(limit * 4 + (u32)arg0 + 0xC) < (u32)arg0) {
            } else {
                goto loop_1;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154230", func_801546FC);
#endif

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154230", func_80154820);
