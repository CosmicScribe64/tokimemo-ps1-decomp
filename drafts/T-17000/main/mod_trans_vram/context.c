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

void mod_trans_vram(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u32 sp194;
    s32 sp88[1];
    u8 sp87;
    s32 sp78;
    u8 *sp74;
    s32 *var_s2;
    s32 temp_t0;
    s32 var_a1;
    s32 var_s1;
    u32 var_a0;
    u32 var_s0;
    u8 temp_t3;
    u8 var_v0;
    u8 var_v1;
    u8 *temp_t8;

    temp_t8 = (arg0 * 0x14) + D_801217A0;
    sp74 = temp_t8;
    if (*(u8 *)((unsigned char *)temp_t8 + (14)) == 8) {
        var_a1 = 8;
    } else {
        sp78 = 0x10;
        var_a1 = 0x10;
    }
    temp_t3 = *(u8 *)((unsigned char *)sp74 + (8));
    switch (temp_t3) {
    case 0:
        sp87 = 1;
        break;
    case 1:
        sp87 = 2;
        break;
    case 2:
        sp87 = 2;
        break;
    case 3:
        sp87 = 1;
        break;
    case 4:
        sp87 = 1;
        break;
    case 5:
        sp87 = 1;
        break;
    default:
        sp87 = 1;
        break;
    }
    var_a0 = 0;
    var_v1 = *(u8 *)((unsigned char *)sp74 + (13));
    var_v0 = *(u8 *)((unsigned char *)sp74 + (12));
    var_s2 = (var_v1 * arg1 * var_v0 * 4) + *(s32 *)((unsigned char *)sp74 + (4));
    if (var_v1 != 0) {
        do {
            var_s0 = 0;
            if (var_v0 != 0) {
                var_s1 = 0;
                sp194 = var_a0;
                sp78 = var_a1;
                do {
                    temp_t0 = *var_s2;
                    get_fnt((s32) &sp88, *(s32 *)((*(s32 *)sp74) + (((u32) (temp_t0 & 0xFF000000) >> 0x18) * 4)), (s32) ((u32) (temp_t0 & 0xFF0000) >> 0x10), (s32) sp87, sp78, temp_t0 & 0xFF);
                    LoadSquare((((arg2 & 0xF) << 6) + ((u32) (arg3 / 2) / sp87) + ((u32) (var_s1 / 2) / sp87)) & 0xFFFF, (((arg2 & 0x10) * 0x10) + ((arg2 & 0x20) * 4) + arg4 + (var_a0 * var_a1)) & 0xFFFF & 0xFFFF, ((u32) (var_a1 / 2) / sp87) & 0xFFFF & 0xFFFF, var_a1 & 0xFFFF & 0xFFFF, &sp88);
                    var_v0 = *(u8 *)((unsigned char *)sp74 + (12));
                    var_s0 += 1;
                    var_s2 += 1;
                    var_s1 += sp78;
                } while (var_s0 < var_v0);
                var_v1 = *(u8 *)((unsigned char *)sp74 + (13));
                var_a0 = sp194;
                var_a1 = sp78;
            }
            var_a0 += 1;
        } while (var_a0 < var_v1);
    }
}

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
