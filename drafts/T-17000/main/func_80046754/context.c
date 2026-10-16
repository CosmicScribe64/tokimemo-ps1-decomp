#include "common.h"
#include "game.h"

s32 func_80046500(void) {
    func_800789E0();
    func_800412E0(0);
    main_state_set(0x12);
    func_80041584();
    func_80048DD0(0);
    back_clear_switch(1);
    dec_bg_reset();
    func_8004482C();
    tpage_buf_clear();
    func_80044750(0x7F);
    func_8007BDE8();
    func_8007B844();
    func_800452C4();
    func_80041878();
    func_80042058();
    return -1;
}

s32 func_80046590(s16 arg0, u8 arg1) {
    s32 p;
    u32 i;

    p = 1;
    if ((u32)arg1 >= 6) {
        return 0;
    }
    i = 0;
    if (arg1 != 0) {
        do {
            i += 1;
            p *= 10;
        } while (i < (u32)arg1);
    }
    return (arg0 / (p / 10)) % 10;
}

s32 func_80046684(s32 arg0) {
    s32 result;
    s32 mul;
    s32 i;

    result = 0;
    mul = 1;
    for (i = 0; i < 8; i++) {
        result += (mul * (arg0 & (0xF << (i * 4)))) >> (i * 4);
        mul *= 10;
    }
    return result;
}

void func_80046754(void *arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 *temp_v0_2;
    s32 *temp_v0_3;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t7;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v1;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_a3_3;
    s32 var_v0_2;
    s32 var_v1_2;
    u8 *temp_v0_6;
    u8 *temp_v0_7;
    u8 *var_v0;
    u8 *var_v1;

    var_a3 = 0;
    var_v1 = arg0;
    do {
        var_a2 = 0;
        var_v0 = var_v1;
loop_2:
        var_a2 += 4;
        *(s32 *)((unsigned char *)var_v0 + (64)) = 0;
        *(s32 *)((unsigned char *)var_v0 + (128)) = 0;
        *(s32 *)((unsigned char *)var_v0 + (192)) = 0;
        var_v0 += 0x100;
        *(s32 *)((unsigned char *)var_v0 + (-256)) = 0;
        if (var_a2 != 0x80) {
            goto loop_2;
        }
        var_a3 += 1;
        var_v1 += 4;
    } while (var_a3 < 0x10);
    var_a3_2 = 0;
    if ((arg2 / 16) & 0xFF) {
        do {
            var_a2_2 = 0;
loop_6:
            temp_lo = var_a2_2 * var_a2_2;
            temp_t7 = var_a2_2 * 0x10;
            var_a2_2 += 1;
            var_v1_2 = 0xF;
            var_v0_2 = 0xF * 0;
loop_7:
            if (((var_a3_2 * var_a3_2) + temp_lo) < ((var_v0_2 * var_v0_2) << 6)) {
                temp_v0 = ((u8 *)arg0) + ((var_a3_2 / 8) * 4) + (temp_t7 * 4);
                *temp_v0 |= var_v1_2 << ((var_a3_2 % 8) * 4);
            } else {
                var_v1_2 -= 1;
                var_v0_2 = 0xF - var_v1_2;
                if (var_v1_2 != 0) {
                    goto loop_7;
                }
            }
            if (var_a2_2 != 0x80) {
                goto loop_6;
            }
            var_a3_2 += 1;
        } while (var_a3_2 != 0x80);
    } else {
        var_a3_3 = 0;
        temp_v1 = arg2 & 0xF;
        do {
            temp_lo_2 = var_a3_3 * var_a3_3;
            var_a2_3 = 0;
loop_15:
            if (temp_v1 != 0) {
                if ((temp_lo_2 + (var_a2_3 * var_a2_3)) < 0x3F01) {
                    temp_v0_2 = ((u8 *)arg0) + ((var_a3_3 / 8) * 4) + (var_a2_3 * 0x10 * 4);
                    *temp_v0_2 |= temp_v1 << ((var_a3_3 % 8) * 4);
                }
            } else if ((temp_lo_2 + (var_a2_3 * var_a2_3)) >= 0x3F02) {
                temp_v0_3 = ((u8 *)arg0) + ((var_a3_3 / 8) * 4) + (var_a2_3 * 0x10 * 4);
                *temp_v0_3 |= 1 << ((var_a3_3 % 8) * 4);
            }
            temp_v0_4 = var_a2_3 + 1;
            if (temp_v1 != 0) {
                temp_v0_5 = var_a2_3 + 1;
                if ((temp_lo_2 + (temp_v0_5 * temp_v0_5)) < 0x3F01) {
                    temp_v0_6 = ((u8 *)arg0) + ((var_a3_3 / 8) * 4) + (var_a2_3 * 0x10 * 4);
                    *(s32 *)((unsigned char *)temp_v0_6 + (64)) = (s32) (*(s32 *)((unsigned char *)temp_v0_6 + (64)) | (temp_v1 << ((var_a3_3 % 8) * 4)));
                }
            } else if ((temp_lo_2 + (temp_v0_4 * temp_v0_4)) >= 0x3F02) {
                temp_v0_7 = ((u8 *)arg0) + ((var_a3_3 / 8) * 4) + (var_a2_3 * 0x10 * 4);
                *(s32 *)((unsigned char *)temp_v0_7 + (64)) = (s32) (*(s32 *)((unsigned char *)temp_v0_7 + (64)) | (1 << ((var_a3_3 % 8) * 4)));
            }
            var_a2_3 += 2;
            if (var_a2_3 != 0x80) {
                goto loop_15;
            }
            var_a3_3 += 1;
        } while (var_a3_3 != 0x80);
    }
    LoadSquare(((arg1 & 0xF) << 6) & 0xFFFF, (((arg1 & 0x10) * 0x10) + ((arg1 & 0x20) * 4)) & 0xFFFF, 0x20U, 0x80U, arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046AC8);
