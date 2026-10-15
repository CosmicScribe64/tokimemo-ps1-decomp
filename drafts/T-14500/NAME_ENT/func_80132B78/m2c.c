extern s32 D_800E7208;

void func_80132B78(s32 arg0) {
    u8 *sp34;
    s32 temp_t1;
    s32 temp_t5;
    s32 temp_t8;
    s32 temp_v0;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_v0;
    s32 var_v0_2;
    u8 *var_v0_3;
    u8 var_a3;

    if ((arg0 == 0) && ((u8) D_8014CC70 >= 3U)) {
        D_8014CC70 = 0xC;
    }
    var_a3 = D_8014CC70;
    if ((arg0 == 1) && (var_a3 >= 6U)) {
        var_a3 = 0xC;
    }
    if ((arg0 == 2) && (var_a3 >= 0xBU)) {
        var_a3 = 0xC;
    }
    if (D_800E7208 & 0x20) {
        var_s0 = 0;
        if (arg0 == 0) {
            var_v0 = 0;
            var_a1 = -0x58;
            do {
                var_a1 += 0x10;
                if ((D_8011ECF6 < var_a1) && ((var_v0 - 0x68) < D_8011ECF6) && (D_8011ECFA < -0x46) && (D_8011ECFA >= -0x55)) {
                    var_a3 = var_s0 & 0xFF;
                }
                var_s0 += 1;
                var_v0 += 0x10;
            } while (var_s0 != 3);
        } else {
            var_s0_2 = 0;
            if (arg0 == 1) {
                var_v0_2 = 0;
                var_a1_2 = -0x20;
                do {
                    var_a1_2 += 0x10;
                    if ((D_8011ECF6 < var_a1_2) && ((var_v0_2 - 0x30) < D_8011ECF6) && (D_8011ECFA < -0x46) && (D_8011ECFA >= -0x55)) {
                        var_a3 = (var_s0_2 + 3) & 0xFF;
                    }
                    var_s0_2 += 1;
                    var_v0_2 += 0x10;
                } while (var_s0_2 != 3);
            } else if (arg0 == 2) {
                temp_v0 = 1 * 0x10;
                if ((D_8011ECF6 < 0x58) && ((D_8011ECF6 - 0x48) > 0) && (D_8011ECFA < -0x46) && (D_8011ECFA >= -0x55)) {
                    var_a3 = 6;
                }
                temp_t5 = temp_v0 + 0x68;
                if ((D_8011ECF6 < (temp_v0 + 0x58)) && (temp_v0 < (D_8011ECF6 - 0x48)) && (D_8011ECFA < -0x46)) {
                    var_at = D_8011ECF6 < temp_t5;
                    if (D_8011ECFA >= -0x55) {
                        var_a3 = 7 & 0xFF;
                        goto block_39;
                    }
                } else {
block_39:
                    var_at = D_8011ECF6 < temp_t5;
                }
                temp_t8 = temp_v0 + 0x78;
                if ((var_at != 0) && (temp_v0 < (D_8011ECF6 - 0x58)) && (D_8011ECFA < -0x46)) {
                    var_at_2 = D_8011ECF6 < temp_t8;
                    if (D_8011ECFA >= -0x55) {
                        var_a3 = 8 & 0xFF;
                        goto block_45;
                    }
                } else {
block_45:
                    var_at_2 = D_8011ECF6 < temp_t8;
                }
                temp_t1 = temp_v0 + 0x88;
                if ((var_at_2 != 0) && (temp_v0 < (D_8011ECF6 - 0x68)) && (D_8011ECFA < -0x46)) {
                    var_at_3 = D_8011ECF6 < temp_t1;
                    if (D_8011ECFA >= -0x55) {
                        var_a3 = 9 & 0xFF;
                        goto block_51;
                    }
                } else {
block_51:
                    var_at_3 = D_8011ECF6 < temp_t1;
                }
                if ((var_at_3 != 0) && (temp_v0 < (D_8011ECF6 - 0x78)) && (D_8011ECFA < -0x46) && (D_8011ECFA >= -0x55)) {
                    var_a3 = 0xA & 0xFF;
                }
            }
        }
    }
    var_v0_3 = D_8011ECD0;
    var_s0_3 = 0;
    do {
        if (var_s0_3 == var_a3) {
            D_8014CC70 = var_a3;
            if (*(s16 *)((unsigned char *)var_v0_3 + (6688)) == 0xD) {
                *(s16 *)((unsigned char *)var_v0_3 + (6688)) = 0;
            }
            sp34 = var_v0_3;
            func_8013664C(var_s0_3 + 0x62, M2C_ERROR(/* Read from unset register $a1 */));
        } else {
            *(s8 *)((unsigned char *)var_v0_3 + (6666)) = 0x40;
            *(s16 *)((unsigned char *)var_v0_3 + (6688)) = 0xD;
            D_8014CC70 = var_a3;
        }
        var_s0_3 += 1;
        var_a3 = D_8014CC70;
        var_v0_3 += 0x44;
    } while (var_s0_3 != 0xB);
}
