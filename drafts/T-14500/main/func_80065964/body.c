s32 func_80065964(s32 arg0, s32 arg1, s32 arg2) {
    void *sp28;                                     /* compiler-managed */
    s32 sp24;
    s32 sp20;
    s16 temp_a2;
    s16 temp_s1;
    s32 temp_t2;
    s32 temp_t4;
    s32 temp_t5;
    s32 var_a1;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_v1;
    u8 *temp_s0;
    u8 *temp_t1;
    u8 *temp_t8;

    temp_t8 = (arg0 * 4) + D_80125C10;
    sp28 = temp_t8;
    var_v1 = 1;
    temp_t2 = (*(s32 *)temp_t8) + arg1;
    sp24 = temp_t2;
    temp_t4 = temp_t2 + 0x13;
    temp_t1 = *(s16 *)((unsigned char *)temp_t8 + (2)) + arg2;
    sp28 = temp_t1;
    temp_t5 = temp_t1 + 0xF;
    sp20 = temp_t4;
    sp28 = temp_t5;
    if (temp_t2 < -0xA0) {
        return 0;
    }
    if (temp_t4 >= 0xA1) {
        return 0;
    }
    if ((s32) temp_t1 < -0x78) {
        return 0;
    }
    var_a1 = 0;
    if ((s32) temp_t1 >= 0x79) {
        return 0;
    }
    do {
        if (var_a1 != arg0) {
            temp_s0 = (var_a1 * 4) + D_80125C10;
            temp_a2 = (*(s32 *)temp_s0);
            temp_s1 = *(s16 *)((unsigned char *)temp_s0 + (2));
            if ((temp_t2 >= temp_a2) && (temp_t2 < (temp_a2 + 0x14)) && ((s32) temp_t1 >= temp_s1)) {
                var_at = temp_t4 < temp_a2;
                if ((s32) temp_t1 < (temp_s1 + 0x10)) {
                    var_v1 = 0;
                    goto block_16;
                }
            } else {
block_16:
                var_at = temp_t4 < temp_a2;
            }
            if ((var_at == 0) && (temp_t4 < (temp_a2 + 0x14)) && ((s32) temp_t1 >= temp_s1)) {
                var_at_2 = temp_t2 < temp_a2;
                if ((s32) temp_t1 < (temp_s1 + 0x10)) {
                    var_v1 = 0;
                    goto block_22;
                }
            } else {
block_22:
                var_at_2 = temp_t2 < temp_a2;
            }
            if ((var_at_2 == 0) && (temp_t2 < (temp_a2 + 0x14)) && (temp_t5 >= temp_s1)) {
                var_at_3 = temp_t4 < temp_a2;
                if (temp_t5 < (temp_s1 + 0x10)) {
                    var_v1 = 0;
                    goto block_28;
                }
            } else {
block_28:
                var_at_3 = temp_t4 < temp_a2;
            }
            if ((var_at_3 == 0) && (temp_t4 < (temp_a2 + 0x14)) && (temp_t5 >= temp_s1) && (temp_t5 < (temp_s1 + 0x10))) {
                var_v1 = 0;
            }
        }
        var_a1 += 1;
    } while (var_a1 != 0xF);
    return var_v1;
}
