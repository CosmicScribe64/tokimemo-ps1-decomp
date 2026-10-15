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

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", get_fnt);

/* FAKE: taking the address of the argument forces the home-slot spill (sw a0,0(sp)); real source unknown. T-0400 */
void func_8004DAC4(s32 arg0) {
    s32 *p = &arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set_gpu);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set);

/* bulk_m2c declarations (T-11190) */
extern u8 D_8012175C;
extern s32 NULL;
/* end bulk_m2c declarations */
s32 srn_set(s32 arg0) {
    s32 spAC;
    s32 sp94;
    s32 sp88;
    s32 sp7C;
    s32 sp78;
    s32 sp64;
    s32 sp60;
    s16 temp_s7;
    s16 temp_t1;
    s16 var_s3;
    s32 *var_s1;
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t3;
    s32 temp_t6;
    s32 var_at;
    s32 var_s0;
    s32 var_s2;
    u8 temp_a1;
    u8 temp_s4;
    u8 temp_s5;
    u8 temp_s6;
    u8 *var_v0;

    var_v0 = (arg0 * 0x28) + D_80121750;
    temp_a1 = *(u8 *)((unsigned char *)var_v0 + (15));
    temp_s4 = *(u8 *)((unsigned char *)var_v0 + (14));
    temp_s5 = *(u8 *)((unsigned char *)var_v0 + (16));
    temp_s6 = *(u8 *)((unsigned char *)var_v0 + (18));
    temp_s7 = *(s16 *)((unsigned char *)var_v0 + (2));
    temp_t1 = *(s16 *)((unsigned char *)var_v0 + (6));
    var_s1 = *(s32 **)((unsigned char *)var_v0 + (8));
    spAC = (s32) *(u8 *)((unsigned char *)var_v0 + (17));
    if ((temp_a1 == 0) || (temp_s4 == 0)) {
        var_v0 = (void *)-1;
    } else {
        var_s2 = *(s32 *)((unsigned char *)var_v0 + (28));
        sp88 = 0;
        sp7C = (s32) *(u16 *)((unsigned char *)var_v0 + (22));
        sp78 = (s32) *(u16 *)((unsigned char *)var_v0 + (24));
        temp_lo = (s32) (*(u8 *)((unsigned char *)var_v0 + (20)) * temp_s5) / (s32) temp_s4;
        sp64 = temp_lo;
        temp_lo_2 = (s32) (*(u8 *)((unsigned char *)var_v0 + (21)) * spAC) / (s32) temp_a1;
        sp94 = temp_lo_2;
        if (temp_lo_2 > 0) {
            var_s3 = temp_t1;
            sp60 = temp_t1 + (temp_a1 * *(u8 *)((unsigned char *)var_v0 + (19)));
loop_5:
            if ((sp60 >= -0x77) && (var_s0 = 0, (sp64 > 0))) {
                var_at = var_s3 < 0x79;
loop_8:
                if (var_at == 0) {
                    var_v0 = NULL;
                } else {
                    temp_a2 = (temp_s5 * var_s0) + temp_s7;
                    if ((temp_a2 + (temp_s6 * temp_s4)) >= -0x9F) {
                        temp_t6 = *var_s1 & 0xFFFF;
                        var_v0 = (void *) temp_t6;
                        if (temp_t6 != 0) {
                            var_v0 = mod_set_gpu(0, temp_t6, temp_a2, (s32) var_s3, sp7C, sp78, (s32) D_8012175C);
                        }
                    }
                    var_s2 -= 1;
                    var_s1 += 4;
                    if (var_s2 == 0) {
                        var_v0 = NULL;
                    } else {
                        var_s0 += 1;
                        var_at = var_s3 < 0x79;
                        if (var_s0 == temp_lo) {
                            goto block_16;
                        }
                        goto loop_8;
                    }
                }
            } else {
block_16:
                temp_t3 = sp88 + 1;
                sp60 += spAC;
                sp88 = temp_t3;
                var_s3 += spAC;
                if (temp_t3 < sp94) {
                    goto loop_5;
                }
            }
        }
    }
    return (s32) var_v0;
}

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
