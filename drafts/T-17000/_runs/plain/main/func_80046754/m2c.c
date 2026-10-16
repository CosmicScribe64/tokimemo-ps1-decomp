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
    void *temp_v0_6;
    void *temp_v0_7;
    void *var_v0;
    void *var_v1;

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
                temp_v0 = arg0 + ((var_a3_2 / 8) * 4) + (temp_t7 * 4);
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
                    temp_v0_2 = arg0 + ((var_a3_3 / 8) * 4) + (var_a2_3 * 0x10 * 4);
                    *temp_v0_2 |= temp_v1 << ((var_a3_3 % 8) * 4);
                }
            } else if ((temp_lo_2 + (var_a2_3 * var_a2_3)) >= 0x3F02) {
                temp_v0_3 = arg0 + ((var_a3_3 / 8) * 4) + (var_a2_3 * 0x10 * 4);
                *temp_v0_3 |= 1 << ((var_a3_3 % 8) * 4);
            }
            temp_v0_4 = var_a2_3 + 1;
            if (temp_v1 != 0) {
                temp_v0_5 = var_a2_3 + 1;
                if ((temp_lo_2 + (temp_v0_5 * temp_v0_5)) < 0x3F01) {
                    temp_v0_6 = arg0 + ((var_a3_3 / 8) * 4) + (var_a2_3 * 0x10 * 4);
                    *(s32 *)((unsigned char *)temp_v0_6 + (64)) = (s32) (*(s32 *)((unsigned char *)temp_v0_6 + (64)) | (temp_v1 << ((var_a3_3 % 8) * 4)));
                }
            } else if ((temp_lo_2 + (temp_v0_4 * temp_v0_4)) >= 0x3F02) {
                temp_v0_7 = arg0 + ((var_a3_3 / 8) * 4) + (var_a2_3 * 0x10 * 4);
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
