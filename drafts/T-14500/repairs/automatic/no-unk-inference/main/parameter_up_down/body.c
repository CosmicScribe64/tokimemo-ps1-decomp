void parameter_up_down(s32 arg0, s32 arg1, s32 arg2) {
    s8 *sp4C;
    s32 temp_v0;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s3;
    s32 var_s4;
    s32 var_s5;
    s32 var_s6;
    s8 *temp_t5;
    s8 *temp_t9;
    s8 *var_s1;
    s8 *var_s1_2;
    s8 temp_v0_2;
    s8 temp_v0_3;
    s8 temp_v0_4;
    s8 temp_v0_5;

    temp_v0 = D_800E6375 & 0xF;
    var_s5 = 1;
    var_s6 = 1;
    if (temp_v0 == 3) {
        var_s5 = 2;
    } else if (((temp_v0 == 1) || (temp_v0 == 2)) && (arg0 != 5) && (arg0 != 7) && (arg0 != 6)) {
        var_s6 = 2;
    }
    if (arg1 != 0) {
        var_s4 = 4;
    } else {
        var_s4 = 1;
    }
    var_s0 = 0;
    if (arg2 != 0) {
        var_s3 = 2;
    } else {
        var_s3 = 1;
    }
    if (arg0 == 4) {
        temp_t9 = (((u16) D_800E6374 >> 0xC) * 9) + &D_800B6928;
        sp4C = temp_t9;
        var_s1 = temp_t9;
        var_s0_2 = 0;
        do {
            temp_v0_2 = *var_s1;
            if (temp_v0_2 > 0) {
                parameter_change(var_s0_2, temp_v0_2 * ((s32) ((s32) ((s32) (var_s4 << 0xC) / var_s3) / var_s5) / var_s6));
            } else {
                parameter_change(var_s0_2, temp_v0_2 << 0xC);
            }
            var_s0_2 += 1;
            var_s1 += 1;
        } while (var_s0_2 != 8);
        temp_v0_3 = *(s8 *)((unsigned char *)sp4C + (8));
        if (temp_v0_3 < 0) {
            parameter_change(8, (temp_v0_3 * ((s32) ((s32) ((s32) (var_s4 << 0xC) / var_s3) / var_s5) / var_s6)) + (var_s3 << 0x10) + 0xFFFF0000);
            return;
        }
        parameter_change(8, (temp_v0_3 << 0xC) + (var_s3 << 0x10) + 0xFFFF0000);
        return;
    }
    temp_t5 = (arg0 * 9) + &D_800B68E0;
    sp4C = temp_t5;
    var_s1_2 = temp_t5;
    do {
        temp_v0_4 = *var_s1_2;
        if (temp_v0_4 > 0) {
            parameter_change(var_s0, temp_v0_4 * ((s32) ((s32) ((s32) (var_s4 << 0xC) / var_s3) / var_s5) / var_s6));
        } else {
            parameter_change(var_s0, temp_v0_4 << 0xC);
        }
        var_s0 += 1;
        var_s1_2 += 1;
    } while (var_s0 != 8);
    temp_v0_5 = *(s8 *)((unsigned char *)sp4C + (8));
    if (temp_v0_5 < 0) {
        parameter_change(8, (temp_v0_5 * ((s32) ((s32) ((s32) (var_s4 << 0xC) / var_s3) / var_s5) / var_s6)) + (var_s3 << 0x10) + 0xFFFF0000);
        return;
    }
    parameter_change(8, (temp_v0_5 << 0xC) + (var_s3 << 0x10) + 0xFFFF0000);
}
