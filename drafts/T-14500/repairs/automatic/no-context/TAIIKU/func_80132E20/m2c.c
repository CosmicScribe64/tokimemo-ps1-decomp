s32 func_800AE0C0(s16);                             /* extern */
extern ? D_8011ECD0;
extern ? D_8011ED14;
extern s8 D_8011ED16;
extern s8 D_8011ED17;
extern s16 D_8011ED1C;
extern s16 D_8011ED2C;
extern s16 D_8011ED3A;
extern ? D_8011F2EC;
extern ? D_80148FB8;

void func_80132E20(s16 arg0, s32 arg1) {
    ? *var_s4;
    ? *var_s4_2;
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
    void *temp_t8;

    M2C_MEMCPY_ALIGNED(&sp78[0], &D_80148FB8, 0x28);
    temp_t7 = &sp78[0] + 0x24;
    temp_t8 = &D_80148FB8 + 0x24;
    var_s4 = &D_8011ECD0;
    var_s5 = 0;
    *(s32 *)((unsigned char *)temp_t7 + (4)) = (s32) *(s32 *)((unsigned char *)temp_t8 + (4));
    *(u16 *)((unsigned char *)temp_t7 + (8)) = (u16) *(u16 *)((unsigned char *)temp_t8 + (8));
    do {
        var_s1 = 0;
        if (arg1 > 0) {
            temp_a0 = arg1 & 3;
            temp_t0 = &(&sp78[0])[var_s5];
            if (temp_a0 != 0) {
                var_s0 = &(&spA8[0])[0];
                var_v0 = ((*temp_t0 + (0 << 0xA)) - arg0) - 0xA0;
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
                temp_a3 = (*temp_t0 + (var_s1 * 0x500)) - arg0;
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
                temp_s2 = func_800AE0C0(*var_s0_3);
                var_s1 += 1;
                if (temp_s2 < func_800AE0C0(spA8[0])) {
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
