#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_tpage_show);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_vram_set);

void move_255_line(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;

    for (i = arg0; i <= arg1; i++) {
        MoveSquare((i * 0x40 + 0x3F) & 0xFFFF, 0, 1, 0xFF, ((arg2 & 0xF) << 6) + i - arg0, ((arg2 & 0x10) * 0x10) & 0xFFFF);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_trans_vram);

void get_fnt(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5) {
    s32 sp8C[3];
    s32 *var_s2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s3;
    u8 temp_t5;
    u8 temp_t7;
    u8 *var_a0;
    u8 *var_a1;
    u8 *var_v0;
    u8 *var_v1;

    var_s0 = 0;
    if (arg4 > 0) {
        temp_lo = 0x80 / arg4;
        var_s1 = 0;
        do {
            memcpy((var_s1 / arg3) + arg0, ((s32) ((arg2 % temp_lo) * arg4) / arg3) + arg1 + ((s32) ((((arg2 / temp_lo) * arg4) + var_s0) << 7) / arg3), arg4 / arg3);
            var_s0 += 1;
            var_s1 += arg4;
        } while (var_s0 != arg4);
    }
    if (arg5 & 1) {
        memcpy((s32) &sp8C, arg0, 0x100);
        var_s1_2 = arg4 - 1;
        if (arg4 > 0) {
            var_s2 = &sp8C;
            var_s3 = 1;
            do {
                memcpy((var_s1_2 * arg4) + arg0, (s32) var_s2, arg4);
                var_s1_2 -= 1;
                var_s2 += arg4;
                var_s3 += 1;
            } while (var_s3 != arg4);
        }
    }
    if (arg5 & 2) {
        var_s0_2 = 0;
        memcpy((s32) &sp8C, arg0, 0x100);
        if (arg4 > 0) {
            if (!(arg4 & 1) || (var_s0_2 = 1, *(u8 *)((unsigned char *)arg0 + (4)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (3)), *(u8 *)((unsigned char *)arg0 + (5)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (2)), *(u8 *)((unsigned char *)arg0 + (6)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (1)), *(u8 *)((unsigned char *)arg0 + (7)) = (u8) (*(s32 *)&sp8C), (*(s32 *)arg0) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (7)), *(u8 *)((unsigned char *)arg0 + (1)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (6)), *(u8 *)((unsigned char *)arg0 + (2)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (5)), *(u8 *)((unsigned char *)arg0 + (3)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (4)), (arg4 != 1))) {
                temp_lo_2 = var_s0_2 * arg4;
                var_v0 = arg0 + temp_lo_2;
                var_v1 = temp_lo_2 + &sp8C;
                temp_lo_3 = arg4 * (var_s0_2 + 1);
                var_a0 = arg0 + temp_lo_3;
                var_a1 = temp_lo_3 + &sp8C;
                do {
                    var_s0_2 += 2;
                    *(u8 *)((unsigned char *)var_v0 + (4)) = (u8) *(u8 *)((unsigned char *)var_v1 + (3));
                    *(u8 *)((unsigned char *)var_v0 + (5)) = (u8) *(u8 *)((unsigned char *)var_v1 + (2));
                    *(u8 *)((unsigned char *)var_v0 + (6)) = (u8) *(u8 *)((unsigned char *)var_v1 + (1));
                    *(u8 *)((unsigned char *)var_v0 + (7)) = (u8) (*(s32 *)var_v1);
                    (*(s32 *)var_v0) = (u8) *(u8 *)((unsigned char *)var_v1 + (7));
                    *(u8 *)((unsigned char *)var_v0 + (1)) = (u8) *(u8 *)((unsigned char *)var_v1 + (6));
                    *(u8 *)((unsigned char *)var_v0 + (2)) = (u8) *(u8 *)((unsigned char *)var_v1 + (5));
                    temp_t5 = *(u8 *)((unsigned char *)var_v1 + (4));
                    var_v1 += arg4 * 2;
                    *(u8 *)((unsigned char *)var_v0 + (3)) = temp_t5;
                    var_v0 += arg4 * 2;
                    *(u8 *)((unsigned char *)var_a0 + (4)) = (u8) *(u8 *)((unsigned char *)var_a1 + (3));
                    *(u8 *)((unsigned char *)var_a0 + (5)) = (u8) *(u8 *)((unsigned char *)var_a1 + (2));
                    *(u8 *)((unsigned char *)var_a0 + (6)) = (u8) *(u8 *)((unsigned char *)var_a1 + (1));
                    *(u8 *)((unsigned char *)var_a0 + (7)) = (u8) (*(s32 *)var_a1);
                    (*(s32 *)var_a0) = (u8) *(u8 *)((unsigned char *)var_a1 + (7));
                    *(u8 *)((unsigned char *)var_a0 + (1)) = (u8) *(u8 *)((unsigned char *)var_a1 + (6));
                    *(u8 *)((unsigned char *)var_a0 + (2)) = (u8) *(u8 *)((unsigned char *)var_a1 + (5));
                    temp_t7 = *(u8 *)((unsigned char *)var_a1 + (4));
                    var_a1 += arg4 * 2;
                    *(u8 *)((unsigned char *)var_a0 + (3)) = temp_t7;
                    var_a0 += arg4 * 2;
                } while (var_s0_2 != arg4);
            }
        }
    }
}

/* FAKE: taking the address of the argument forces the home-slot spill (sw a0,0(sp)); real source unknown. T-0400 */
void func_8004DAC4(s32 arg0) {
    s32 *p = &arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set_gpu);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_set);

void srn_init(s32 arg0, u32 *arg1, s32 arg2) {
    u8 *p;

    p = D_80121750 + arg0 * 0x28;
    p[0xD] = (arg1[0] & 0xFF00) >> 8;
    *(s16 *)(p + 2) = 0;
    *(s16 *)(p + 6) = 0;
    p[0xC] = 0;
    *(u32 **)(p + 8) = arg1 + 5;
    p[0xE] = (arg1[1] & 0xFF000000) >> 24;
    p[0xF] = (arg1[1] & 0xFF0000) >> 16;
    p[0x10] = (arg1[1] & 0xFF00) >> 8;
    /* FAKE: the no-op "& 0xFF" consumes one IDO temp number, as in func_8004E44C. T-4070 */
    p[0x11] = arg1[1] & 0xFF;
    p[0x12] = (arg1[2] & 0xFF000000) >> 24;
    p[0x13] = (arg1[2] & 0xFF0000) >> 16;
    p[0x14] = (arg1[2] & 0xFF00) >> 8;
    p[0x15] = arg1[2] & 0xFF; /* FAKE: same no-op mask as above. T-4070 */
    *(s16 *)(p + 0x16) = 0;
    *(s16 *)(p + 0x18) = 0x1E0;
    *(u32 *)(p + 0x1C) = arg1[4];
    *(u32 *)(p + 0x20) = arg1[3];
    *(u8 **)(p + 0x24) = D_801217A0 + arg2 * 0x14;
}

void func_8004E44C(s32 arg0, u32 *arg1, s32 arg2) {
    u8 *p;

    p = D_801217A0 + arg0 * 0x14;
    *(s32 *)p = arg2;
    *(u32 **)(p + 4) = arg1 + 4;
    p[8] = (arg1[0] & 0xFF000000) >> 24;
    *(s16 *)(p + 0xA) = (arg1[0] & 0xFFFF) >> 24;
    p[0xC] = (arg1[1] & 0xFF000000) >> 24;
    p[0xD] = (arg1[1] & 0xFF0000) >> 16;
    p[0xE] = (arg1[1] & 0xFF00) >> 8;
    /* FAKE: the no-op "& 0xFF" consumes one IDO temp number, as in the original. T-2090 */
    p[0xF] = arg1[1] & 0xFF;
    *(s16 *)(p + 0x10) = ((u8 *)arg1)[8];
    *(s16 *)(p + 0x12) = ((u8 *)arg1)[0xC];
}
