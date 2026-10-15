void func_801373D0(void) {
    s32 temp_a1;
    s32 temp_fp;
    s32 temp_t5;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s0;
    s32 var_t2;
    s32 var_v0;
    u8 *temp_s1;

    if ((D_800E7380 == 0) && (D_8013CED8 = 0, (D_8013CED4 != 0))) {
        switch (D_8013CED4) {
        case 1:
            D_800E7384 = 0;
            goto block_7;
        case 2:
            D_8013CECC += 1;
            D_8013CED0 = 0;
            D_8013CED4 = 0;
            D_8013CEC8 = 1;
            k_reset(1);
            func_8004284C();
            break;
        }
    } else {
block_7:
        var_v0 = D_8013CED4;
        temp_fp = *(&D_8013CE8C + (D_8013CECC * 4));
        if (var_v0 == 0) {
            var_v0 = 1;
        }
        D_8013CED4 = var_v0;
        if (var_v0 == 1) {
            var_s0 = D_8013CEC8;
            if (var_s0 < 0xFFFF) {
                var_t2 = var_s0 * 8;
                do {
                    temp_s1 = temp_fp + var_t2;
                    temp_v1 = var_s0;
                    if (D_8013CED0 == (*(s32 *)temp_s1)) {
                        temp_v0 = *(s32 *)((unsigned char *)temp_s1 + (4));
                        if (temp_v0 == 0x0FFFFFFF) {
                            D_8013CED4 = 2;
                            var_s0 = 0xFFFF;
                        } else {
                            temp_t5 = (s32) (temp_v0 & 0xFF0000) >> 0x10;
                            temp_a1 = temp_v0 & 0xFF;
                            if (temp_a1 == 0xFE) {
                                func_801377D4(temp_t5, 0x50, 0);
                            } else {
                                func_801377D4(temp_t5, temp_a1, 1);
                            }
                            if (*(s32 *)((unsigned char *)temp_s1 + (4)) & 0xF0000000) {
                                func_80137824(D_8013C814[D_8013CECC]);
                                D_800E7380 = 1;
                                D_8013CED8 = 1;
                            }
                        }
                    } else {
                        var_s0 = 0xFFFF;
                        D_8013CEC8 = temp_v1;
                    }
                    var_s0 += 1;
                    var_t2 = var_s0 * 8;
                } while (var_s0 < 0xFFFF);
            }
        }
        D_8013CED0 += 1;
        if (D_8013CED8 == 1) {
            func_80137620();
        }
    }
}
