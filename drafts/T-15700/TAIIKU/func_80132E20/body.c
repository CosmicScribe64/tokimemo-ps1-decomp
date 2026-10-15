void func_80132E20(s32 arg0, s32 arg1) {
    s16 sp78[23];
    s32 spA8[16];
    typedef struct { s32 w[10]; } M2cBlock28; /* m2c memcpy of 40 bytes */
    s32 *var_s4_2;
    s16 *temp_t0;
    s16 *temp_t7;
    s16 *var_s0;
    s16 *var_s0_2;
    s16 *var_s0_3;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_a0;
    s16 var_a1;
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v1;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_s2;
    s32 var_s1;
    s32 var_s5;
    u8 *var_s4;
    u8 *temp_t8;

    *(RepairShort23 *)sp78 = *(RepairShort23 *)&D_80148FB8;
    var_s4 = D_8011ECD0;
    var_s5 = 0;
    do {
        var_s1 = 0;
        if (arg1 > 0) {
            temp_a0 = arg1 & 3;
            temp_t0 = &(&sp78[0])[var_s5];
            if (temp_a0 != 0) {
                var_s0 = &(&spA8[0])[0];
                var_v0 = ((*temp_t0 + (0 << 0xA)) - (s16) arg0) - 0xA0;
                do {
                    *var_s0 = var_v0;
                    var_s1 += 1;
                    var_v0 += 0x500;
                    var_s0 += 2;
                } while (temp_a0 != var_s1);
                if (var_s1 != arg1) {
                    goto block_6;
                }
            } else {
block_6:
                temp_a3 = (*temp_t0 + (var_s1 * 0x500)) - (s16) arg0;
                var_s0_2 = &(&spA8[0])[var_s1];
                var_v0_2 = temp_a3 - 0xA0;
                var_v1 = temp_a3 + 0x460;
                var_a0 = temp_a3 + 0x960;
                var_a1 = temp_a3 + 0xE60;
                do {
                    *(s16 *)((unsigned char *)var_s0_2 + (2)) = var_v1;
                    *(s16 *)((unsigned char *)var_s0_2 + (4)) = var_a0;
                    *(s16 *)((unsigned char *)var_s0_2 + (6)) = var_a1;
                    var_s0_2 += 8;
                    *(s16 *)((unsigned char *)var_s0_2 + (-8)) = var_v0_2;
                    var_v0_2 += 0x1400;
                    var_a1 += 0x1400;
                    var_a0 += 0x1400;
                    var_v1 += 0x1400;
                } while (var_s0_2 != &(&spA8[0])[arg1]);
            }
            var_s1 = 0;
        }
        var_s0_3 = &spA8[0];
        if (arg1 > 0) {
            do {
                temp_s2 = func_800AE0C0((s32) *var_s0_3);
                var_s1 += 1;
                if (temp_s2 < func_800AE0C0((s32) spA8[0])) {
                    spA8[0] = *var_s0_3;
                }
                var_s0_3 += 2;
            } while (var_s1 != arg1);
        }
        var_s5 += 1;
        var_s4 += 0x44;
        *(s16 *)((unsigned char *)var_s4 + (38)) = spA8[0];
    } while (var_s5 < 0x17);
    var_s4_2 = &D_8011ED14;
    if ((D_8011ED3A < 0xB1) && (D_8011ED3A >= -0x127)) {
        D_8011ED16 = 1;
        D_8011ED17 = 0xA4;
    } else {
        D_8011ED16 = 0;
        D_8011ED17 = 0;
        D_8011ED2C = 0;
        D_8011ED1C = 0;
    }
    do {
        temp_v0 = *(s16 *)((unsigned char *)var_s4_2 + (106));
        if ((temp_v0 < 0xB1) && (temp_v0 >= -0x127)) {
            *(s8 *)((unsigned char *)var_s4_2 + (70)) = 1;
            *(s8 *)((unsigned char *)var_s4_2 + (71)) = 0xA4;
        } else {
            *(s8 *)((unsigned char *)var_s4_2 + (70)) = 0;
            *(s8 *)((unsigned char *)var_s4_2 + (71)) = 0;
            *(s16 *)((unsigned char *)var_s4_2 + (92)) = 0;
            *(s16 *)((unsigned char *)var_s4_2 + (76)) = 0;
        }
        temp_v0_2 = *(s16 *)((unsigned char *)var_s4_2 + (174));
        if ((temp_v0_2 < 0xB1) && (temp_v0_2 >= -0x127)) {
            *(s8 *)((unsigned char *)var_s4_2 + (138)) = 1;
            *(s8 *)((unsigned char *)var_s4_2 + (139)) = 0xA4;
        } else {
            *(s8 *)((unsigned char *)var_s4_2 + (138)) = 0;
            *(s8 *)((unsigned char *)var_s4_2 + (139)) = 0;
            *(s16 *)((unsigned char *)var_s4_2 + (160)) = 0;
            *(s16 *)((unsigned char *)var_s4_2 + (144)) = 0;
        }
        var_s4_2 += 0x88;
    } while (var_s4_2 != &D_8011F2EC);
}
