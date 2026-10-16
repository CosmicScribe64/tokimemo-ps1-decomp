s32 func_8013842C(void) {
    s32 temp_a1;
    s32 temp_fp;
    s32 temp_t5;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s0;
    s32 var_t2;
    s32 var_v0;
    u8 *temp_s1;

    if ((D_800E6280.unk_1100 == 0) && (D_8013D9A4 = 0, (D_8013D9A0 != 0))) {
        switch (D_8013D9A0) {
        case 1:
            D_800E6280.unk_1104.w = 0;
            goto block_7;
        case 2:
            D_8013D998 += 1;
            D_8013D99C = 0;
            D_8013D9A0 = 0;
            D_8013D994 = 1;
            k_reset(1);
            func_8004284C();
            break;
        }
    } else {
block_7:
        var_v0 = D_8013D9A0;
        temp_fp = *(&D_8013D90C + (D_8013D998 * 4));
        if (var_v0 == 0) {
            var_v0 = 1;
        }
        D_8013D9A0 = var_v0;
        if (var_v0 == 1) {
            var_s0 = D_8013D994;
            if (var_s0 < 0xFFFF) {
                var_t2 = var_s0 * 8;
                do {
                    temp_s1 = temp_fp + var_t2;
                    temp_v1 = var_s0;
                    if (D_8013D99C == (*(s32 *)temp_s1)) {
                        temp_v0 = *(s32 *)((unsigned char *)temp_s1 + (4));
                        if (temp_v0 == 0x0FFFFFFF) {
                            D_8013D9A0 = 2;
                            var_s0 = 0xFFFF;
                        } else {
                            temp_t5 = (s32) (temp_v0 & 0xFF0000) >> 0x10;
                            temp_a1 = temp_v0 & 0xFF;
                            if (temp_a1 == 0xFE) {
                                func_80138810(temp_t5, 0x50, 0);
                            } else {
                                func_80138810(temp_t5, temp_a1, 1);
                            }
                            if (*(s32 *)((unsigned char *)temp_s1 + (4)) & 0xF0000000) {
                                func_80138860(*(&D_8013D0C0 + (D_8013D998 * 4)));
                                D_800E6280.unk_1100 = 1;
                                D_8013D9A4 = 1;
                            }
                        }
                    } else {
                        var_s0 = 0xFFFF;
                        D_8013D994 = temp_v1;
                    }
                    var_s0 += 1;
                    var_t2 = var_s0 * 8;
                } while (var_s0 < 0xFFFF);
            }
        }
        D_8013D99C += 1;
        if (D_8013D9A4 == 1) {
            func_8013867C();
        }
    }
}
