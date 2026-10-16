#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_tpage_show);

/* bulk_m2c declarations (T-11190) */
void move_255_line(s32 arg0, s32 arg1, s32 arg2);
/* end bulk_m2c declarations */
void srn_vram_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp94;
    s32 sp84;
    s32 sp80;
    s32 sp74;
    s32 sp70;
    s32 sp58;
    s32 *var_s2;
    s32 temp_lo;
    s32 temp_t9;
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    s32 var_t1;
    s32 var_t8;
    s32 var_v1;
    u8 temp_a1;
    u8 temp_fp;
    u8 temp_s4;
    u8 temp_s5;
    u8 temp_t8;
    u8 var_t0;
    u8 *temp_v0;

    func_8009C674(0);
    temp_v0 = (arg0 * 0x28) + D_80121750;
    temp_t8 = *(u8 *)((unsigned char *)temp_v0 + (14));
    temp_fp = *(u8 *)((unsigned char *)temp_v0 + (15));
    temp_s4 = *(u8 *)((unsigned char *)temp_v0 + (16));
    temp_s5 = *(u8 *)((unsigned char *)temp_v0 + (17));
    var_t0 = *(u8 *)((unsigned char *)temp_v0 + (19));
    temp_a1 = *(u8 *)((unsigned char *)temp_v0 + (21));
    var_s2 = *(s32 **)((unsigned char *)temp_v0 + (8));
    sp94 = (s32) temp_t8;
    sp84 = (s32) *(u8 *)((unsigned char *)temp_v0 + (18));
    if ((temp_fp == 0) || (temp_t8 == 0)) {
        printf("ERROR", temp_a1);
        return;
    }
    var_s3 = *(s32 *)((unsigned char *)temp_v0 + (28));
    var_s1 = 0;
    temp_lo = (s32) (*(u8 *)((unsigned char *)temp_v0 + (20)) * temp_s4) / sp94;
    var_t1 = temp_lo;
    var_v1 = (s32) (temp_a1 * temp_s5) / (s32) temp_fp;
    sp70 = var_v1;
    if (var_v1 > 0) {
        do {
            var_s0 = 0;
            if (temp_lo > 0) {
                var_t8 = -1;
                do {
                    if ((((var_t8 * temp_s4) + (sp84 * sp94)) < arg2) && ((((var_s1 - 1) * temp_s5) + (var_t0 * temp_fp)) < arg3)) {
                        temp_t9 = *var_s2 & 0xFFFF;
                        if (temp_t9 != 0) {
                            sp58 = var_v1;
                            sp80 = (s32) var_t0;
                            sp74 = var_t1;
                            mod_trans_vram(0, temp_t9, arg1, temp_s4 * var_s0, temp_s5 * var_s1);
                        }
                    }
                    var_s3 -= 1;
                    var_s2 += 1;
                    if (var_s3 == 0) {
                        var_s1 = var_v1;
                        var_s0 = temp_lo;
                    }
                    var_s0 += 1;
                    var_t8 = var_s0 - 1;
                } while (var_s0 < var_t1);
            }
            var_s1 += 1;
        } while (var_s1 < sp70);
    }
    if ((u8) D_800B3DB0 != 0) {
        move_255_line(arg1, arg1 + 0xB, 0xF);
    }
}

void move_255_line(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;

    for (i = arg0; i <= arg1; i++) {
        MoveSquare((i * 0x40 + 0x3F) & 0xFFFF, 0, 1, 0xFF, ((arg2 & 0xF) << 6) + i - arg0, ((arg2 & 0x10) * 0x10) & 0xFFFF);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_trans_vram);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", get_fnt);

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
