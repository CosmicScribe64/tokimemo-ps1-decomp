#include "common.h"
#include "ovl/TACO.h"

/* NON_MATCHING: T-10510, 9 lines: IDO folds the entry copy of the pointer parameter (move s0,a0 at entry in the original) */
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

void func_8015443C(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp4C;
    s32 *var_a0;
    s32 *var_s0;
    s32 temp_s3;
    s32 temp_s7;
    s32 temp_t2;
    s32 temp_t6;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_s1;
    u32 temp_s4;
    u8 temp_v0;

    temp_v0 = D_8015EDB4[20].unk2;
    temp_s3 = *(s32 *)((unsigned char *)arg0 + (4));
    temp_s4 = *(u32 *)((unsigned char *)arg0 + (12));
    temp_s7 = arg1 & 0xFFFF;
    var_s1 = 0;
    var_s0 = ((u8 *)arg0) + 0x24;
    if (temp_v0 < 0x10U) {
        var_a1 = (func_800AE0D0() % 150) + 0x64;
        goto block_17;
    }
    if (temp_v0 >= 0x10U) {
        var_at = temp_v0 < 0x20U;
        if (temp_v0 < 0x20U) {
            var_a1 = (func_800AE0D0() % 100) + 0x32;
            goto block_17;
        }
        goto block_6;
    }
    var_at = temp_v0 < 0x20U;
block_6:
    if (var_at == 0) {
        var_at_2 = temp_v0 < 0x30U;
        if (temp_v0 < 0x30U) {
            var_a1 = (func_800AE0D0() % 50) + 0x1E;
            goto block_17;
        }
        goto block_10;
    }
    var_at_2 = temp_v0 < 0x30U;
block_10:
    if (var_at_2 == 0) {
        var_at_3 = temp_v0 < 0x40U;
        if (temp_v0 < 0x40U) {
            var_a1 = (func_800AE0D0() % 30) + 0x14;
            goto block_17;
        }
        goto block_14;
    }
    var_at_3 = temp_v0 < 0x40U;
block_14:
    if (var_at_3 == 0) {
        var_a1_2 = sp4C;
        if (temp_v0 < 0x64U) {
            var_a1 = (func_800AE0D0() % 10) + 5;
block_17:
            sp4C = var_a1;
            goto block_18;
        }
    } else {
block_18:
        var_a1_2 = sp4C;
    }
    var_a0 = var_s0;
loop_20:
    sp4C = var_a1_2;
    if (func_8015431C((s32) var_a0) != 0) {
        temp_v0_2 = (*(s32 *)var_s0);
        if ((temp_v0_2 == 0x24000507) || (temp_v0_2 == 0x34000609)) {
            if ((u8) D_8015EDB4[20].unk2 < 0x50U) {
                if (var_s1 >= var_a1_2) {
                    temp_v0_3 = *(s32 *)((unsigned char *)var_s0 + (8));
                    temp_t2 = (temp_v0_3 & 0xFFFF) + (temp_v0_3 & 0x01E00000) + ((temp_s7 & 0x1F) << 0x10);
                    *(s32 *)((unsigned char *)var_s0 + (4)) = (s32) ((*(s32 *)((unsigned char *)var_s0 + (4)) & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10)));
                    var_s0 += 2;
                    *var_s0 = temp_t2;
                    var_s1 = 0;
                }
                var_s1 += 1;
            } else {
                var_a1_2 = func_800AE0D0() % 70;
                if (var_a1_2 == 0) {
                    temp_v0_4 = *(s32 *)((unsigned char *)var_s0 + (8));
                    temp_t6 = (temp_v0_4 & 0xFFFF) + (temp_v0_4 & 0x01E00000) + ((temp_s7 & 0x1F) << 0x10);
                    *(s32 *)((unsigned char *)var_s0 + (4)) = (s32) ((*(s32 *)((unsigned char *)var_s0 + (4)) & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10)));
                    var_s0 += 2;
                    *var_s0 = temp_t6;
                }
            }
        }
        var_s0 += 1;
        if ((temp_s3 == 0) || (temp_s4 >= (u32) var_s0)) {
            var_a0 = var_s0;
            if ((temp_s3 == 0) && (var_a0 = var_s0, (((u32) ((temp_s4 * 4) + var_s0 + 0xC) < (u32) var_s0) != 0))) {

            } else {
                goto loop_20;
            }
        }
    }
}

/* NON_MATCHING: T-10310, near miss */
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
