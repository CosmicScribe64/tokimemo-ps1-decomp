s32 func_80075C84(void) {
    void *sp28;
    s32 var_a2;
    s32 var_v0;
    s32 var_v1;
    u32 temp_hi;
    u32 var_v1_2;
    u8 *var_a0;
    u8 *var_t5;

    if ((D_8011ECF6 < -0x40) && (D_8011ECF6 >= -0x4F) && (D_8011ECFA < 0x18) && (D_8011ECFA >= -0xF)) {
        var_v1 = D_800E7208 & 0x70;
        switch (var_v1) {
        case 0x20:
            *(&D_800B6990 + D_800B6998) = 0;
            D_800B6998 += 1;
            func_80044750(0x505);
block_11:
            var_v1 = D_800E7208 & 0x70;
            break;
        case 0x40:
            *(&D_800B6990 + D_800B6998) = 1;
            D_800B6998 += 1;
            func_80044750(0x506);
            goto block_11;
        case 0x10:
            *(&D_800B6990 + D_800B6998) = 2;
            D_800B6998 += 1;
            func_80044750(0x507);
            goto block_11;
        }
        var_a2 = 0;
        if ((u8) D_800B6998 >= 7U) {
            D_800B6998 = 0;
        }
        var_t5 = &D_800B3778;
        if (var_v1 != 0) {
loop_16:
            var_v0 = 0;
            var_a0 = var_t5;
            var_v1_2 = D_800B6998 + 7;
loop_17:
            temp_hi = var_v1_2 % 7U;
            var_v1_2 += 1;
            if (*(&D_800B6990 + temp_hi) == *var_a0) {
                if (var_v0 == 6) {
                    var_v0 += 1;
                    if (((u32) (D_800E6280.unk_1BC[var_a2].unk_0C.w << 0x1C) >> 0x1F) != 0) {
                        if (var_a2 != 0xB) {
                            if (var_a2 == 0xC) {
                                D_801230D0 = 0xB;
                                D_800E71DF = 0xB;
                                D_800E7313 = 0xC;
                            } else {
                                D_801230D0 = var_a2;
                                D_800E71DF = (s8) var_a2;
                                D_800E7313 = var_a2 + 1;
                            }
                        }
                        sp28 = &D_800E6280 + (var_a2 * 0x38);
                        D_800E6280.unk_1BC[var_a2] = (s32) (D_800E6280.unk_1BC[var_a2] + (rand() & 0x7FFF));
                        if (D_800E6376 & 1) {
                            D_800E639E += 0x20;
                        }
                        func_80042908(0x10);
                        func_80042940(2);
                        D_800E738D = 0xB;
                        func_8004E58C();
                        icon_disp_switch(0);
                        func_80065F34(0);
                        telephone_class_init();
                        k_sub_reset_point_set();
                        k_sub_disp_start(0);
                        return 1;
                    }
                    goto block_28;
                }
                var_v0 += 1;
block_28:
                var_a0 += 1;
                if (var_v0 == 7) {
                    goto block_29;
                }
                goto loop_17;
            }
block_29:
            var_a2 += 1;
            var_t5 += 7;
            if (var_a2 == 0xD) {
                goto block_30;
            }
            goto loop_16;
        }
        goto block_30;
    }
block_30:
    return 0;
}
