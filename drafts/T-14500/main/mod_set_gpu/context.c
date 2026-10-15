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

void mod_set_gpu(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    u32 sp9C;
    s32 sp78;
    u8 *sp6C;
    s32 sp60;
    s32 sp5C;
    s16 var_s4;
    s32 *var_s6;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_s3;
    u32 temp_s1;
    u32 temp_s2;
    u32 temp_s7;
    u32 var_a2;
    u32 var_s5;
    u8 temp_t9;
    u8 var_v0;
    u8 var_v1;
    u8 *temp_t8;

    temp_v0 = func_8004901C();
    temp_t8 = (arg0 * 0x14) + D_801217A0;
    sp6C = temp_t8;
    temp_t9 = *(u8 *)((unsigned char *)temp_t8 + (8));
    switch (temp_t9) {
    case 0:
        sp78 = 1;
        break;
    case 1:
        sp78 = 2;
        break;
    case 2:
        sp78 = 2;
        break;
    case 3:
        sp78 = 1;
        break;
    case 4:
        sp78 = 1;
        break;
    case 5:
        sp78 = 1;
        break;
    default:
        sp78 = 1;
        break;
    }
    var_s3 = 0xFFFF;
    var_v1 = *(u8 *)((unsigned char *)sp6C + (13));
    var_v0 = *(u8 *)((unsigned char *)sp6C + (12));
    var_a2 = 0;
    var_s6 = (var_v1 * arg1 * var_v0 * 4) + *(s32 *)((unsigned char *)sp6C + (4));
    if (var_v1 != 0) {
        do {
            var_s5 = 0;
            if (var_v0 != 0) {
                sp5C = arg6 * 4;
                sp60 = (var_a2 * 8) + arg3 + 0x168;
                sp9C = var_a2;
                var_s4 = arg2 + 0xA0;
                do {
                    temp_v0_2 = *var_s6;
                    temp_s1 = (u32) (temp_v0_2 & 0xFF0000) >> 0x10;
                    temp_s2 = (u32) (temp_v0_2 & 0xFF000000) >> 0x18;
                    temp_s7 = (u32) (temp_v0_2 & 0xFF00) >> 8;
                    temp_v0_3 = GetWorkBase(0x10, D_8011ECA0);
                    func_8009F07C(temp_v0_3);
                    SetSemiTrans(temp_v0_3, 0);
                    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = var_s4;
                    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (sp60 - (D_8011ECA0 * 0xF0));
                    *(s8 *)((unsigned char *)temp_v0_3 + (13)) = (s8) (((u32) (temp_s1 & 0xF0) >> 4) * 8);
                    *(s8 *)((unsigned char *)temp_v0_3 + (12)) = (s8) ((temp_s1 & 0xF) * 8);
                    *(s8 *)((unsigned char *)temp_v0_3 + (6)) = (s8) temp_v0;
                    *(s8 *)((unsigned char *)temp_v0_3 + (5)) = (s8) temp_v0;
                    *(s8 *)((unsigned char *)temp_v0_3 + (4)) = (s8) temp_v0;
                    temp_v0_4 = search_tpage_multi(*(s32 *)((*(s32 *)sp6C) + (temp_s2 * 4)), sp78);
                    if (var_s3 != temp_v0_4) {
                        if (var_s3 != 0xFFFF) {
                            goto_tpage((var_s3 & 0xF) << 6, (var_s3 & 0x10) * 0x10, arg6 + 1, 0, 0);
                        }
                        var_s3 = temp_v0_4 & 0xFFFF & 0xFFFF;
                    }
                    *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(arg4 + (temp_s7 * 0x10), arg5);
                    AddPrim(&*(s32 *)((unsigned char *)(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp5C]) + (4)), temp_v0_3);
                    var_s5 += 1;
                    var_v0 = *(u8 *)((unsigned char *)sp6C + (12));
                    var_s4 += 8;
                    var_s6 += 4;
                } while (var_s5 < var_v0);
                var_v1 = *(u8 *)((unsigned char *)sp6C + (13));
                var_a2 = sp9C;
            }
            var_a2 += 1;
        } while (var_a2 < var_v1);
    }
    goto_tpage((var_s3 & 0xF) << 6, (var_s3 & 0x10) * 0x10, arg6 + 1, 0, 0);
}

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
