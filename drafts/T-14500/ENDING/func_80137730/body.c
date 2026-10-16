void func_80137730(void) {
    s16 *temp_s2;
    s16 *temp_v0;
    s16 temp_v0_2;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s0_4;
    s16 var_s0_5;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    s32 var_a0;
    s32 var_a2;
    s32 var_at;
    s32 var_at_2;
    u8 *temp_a1;
    u8 *var_t0;
    u8 temp_v1;
    u8 var_v0;
    u8 *temp_v0_3;

    temp_v0 = get_ksys();
    temp_s2 = temp_v0;
    if (D_8013C35C <= 0) {

    } else {
        temp_v1 = *(u8 *)((unsigned char *)(*(s32 *)(D_8013C350 + (D_8013C358 * 4)) + D_8013C35C) + (-1));
        if ((temp_v1 == 0) || (temp_v1 == 6)) {
            var_s0 = 1;
            if (*temp_v0 > 0) {
                do {
                    *(s8 *)((unsigned char *)get_k_work(var_s0) + (7)) = 1;
                    var_s0 += 1;
                } while (*temp_s2 >= var_s0);
            }
            D_8011F524 = 0;
        } else {
            var_v1 = 0;
            if (func_800460EC() & 4) {
                var_v1 = getCDlevel();
            }
            (&D_80125C80)[D_800E7378 & 3] = var_v1;
            var_v1_2 = D_80125C80;
            var_a0 = 0;
            if (var_v1_2 < D_80125C82) {
                var_v1_2 = D_80125C82;
                var_a0 = 1;
            }
            var_s0_2 = 0;
            if (var_v1_2 < D_80125C84) {
                var_v1_2 = D_80125C84;
                var_a0 = 2;
            }
            var_a2 = 0;
            var_v1_3 = 0;
            if (var_v1_2 < D_80125C86) {
                var_a0 = 3;
            }
            do {
                if (var_s0_2 != var_a0) {
                    temp_v0_2 = (&D_80125C80)[var_s0_2];
                    if (var_v1_3 < temp_v0_2) {
                        var_v1_3 = temp_v0_2;
                    }
                }
                var_s0_2 += 1;
            } while (var_s0_2 < 4);
            temp_a1 = (D_800E71DF * 3) + &D_800B5A68;
            var_s0_3 = 0;
            var_t0 = temp_a1;
            do {
                var_at = (u8) *var_t0 < (u32) var_v1_3;
                if (var_s0_3 == 0) {
                    var_at = (u32) var_v1_3 > 8U;
                }
                if (var_at != 0) {
                    var_a2 = var_s0_3 + 1;
                }
                var_s0_3 += 1;
                var_t0 = temp_a1 + var_s0_3;
            } while (var_s0_3 < 3);
            var_v0 = 0;
            if (D_800E71DF == 0) {
                var_at_2 = var_a2 < 4;
                if (D_8011F522 == 8) {
                    var_v0 = 1;
                    goto block_28;
                }
            } else {
block_28:
                var_at_2 = var_a2 < 4;
            }
            if (var_at_2 == 0) {
                var_a2 = 3;
            }
            D_800B5A60 = var_v0;
            if (var_a2 != 0) {
                D_800B5A60 = var_v0;
                if (var_v0 != 0) {
                    var_a2 = 3 - var_a2;
                }
            }
            if ((D_8011F524 != 0) && (D_800E7378 & 1)) {

            } else if (var_v1_3 == 0) {
                D_8011F524 = 0;
            } else if (var_a2 == 0) {
                D_8011F524 = 0;
            } else if (var_a2 == 1) {
                D_8011F524 = 1;
            } else if (var_a2 == 2) {
                D_8011F524 = 2;
            } else if (var_a2 >= 3) {
                D_8011F524 = 3;
            }
            if (D_800E7200 != 0) {
                var_s0_4 = 1;
                if (*temp_s2 > 0) {
                    do {
                        temp_v0_3 = get_k_work(var_s0_4);
                        if (*(s16 *)((unsigned char *)temp_v0_3 + (2)) >= 0x48) {
                            *(s8 *)((unsigned char *)temp_v0_3 + (7)) = 1;
                        }
                        var_s0_4 += 1;
                    } while (*temp_s2 >= var_s0_4);
                }
            } else {
                var_s0_5 = 1;
                if (*temp_s2 > 0) {
                    do {
                        *(s8 *)((unsigned char *)get_k_work(var_s0_5) + (7)) = 0;
                        var_s0_5 += 1;
                    } while (*temp_s2 >= var_s0_5);
                }
            }
        }
    }
}
