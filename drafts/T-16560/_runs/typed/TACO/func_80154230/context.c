#include "common.h"
#include "ovl/TACO.h"

/* bulk_m2c declarations (T-11190) */
s32 func_8015431C(u32 arg0);
/* end bulk_m2c declarations */
void func_80154230(void *arg0, s32 arg1) {
    s32 *var_s0;
    s32 temp_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    u32 temp_s2;

    temp_s1 = *(s32 *)((unsigned char *)arg0 + (4));
    temp_s2 = *(u32 *)((unsigned char *)arg0 + (12));
    var_s0 = ((u8 *)arg0) + 0x24;
loop_1:
    if (func_8015431C((s32) var_s0) != 0) {
        temp_v0 = (*(s32 *)var_s0);
        if ((temp_v0 == 0x24000507) || (temp_v0 == 0x34000609)) {
            temp_v0_2 = *(s32 *)((unsigned char *)var_s0 + (8));
            var_s0 += 2;
            *var_s0 = (temp_v0_2 & 0xFFFF) + (temp_v0_2 & 0x01E00000) + ((arg1 & 0xFFFF & 0x1F) << 0x10);
        }
        var_s0 += 1;
        if ((temp_s1 == 0) || (temp_s2 >= (u32) var_s0)) {
            if ((temp_s1 == 0) && ((u32) ((temp_s2 * 4) + var_s0 + 0xC) < (u32) var_s0)) {

            } else {
                goto loop_1;
            }
        }
    }
}

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154230", func_8015443C);

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
