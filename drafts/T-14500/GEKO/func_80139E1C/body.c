void func_80139E1C(void) {
    void *sp4C;
    s16 temp_a0;
    s16 temp_a0_2;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 var_s2;
    s16 var_t4;
    s16 var_v1;
    s32 temp_v0;
    s32 var_t6;
    s32 var_v0;
    u8 *temp_s0;
    u8 *var_a1;
    u8 *var_s1;

    var_s2 = 4;
    var_t6 = 4 * 8;
    do {
        temp_s0 = ((var_t6 + var_s2) * 4) + D_801217D0;
        *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) + 0xA);
        var_v0 = var_s2 * 2;
        if (!(func_800AE0D0() & 3)) {
            temp_a0 = *(s16 *)((unsigned char *)temp_s0 + (6));
            temp_v0 = var_s2 * 2;
            var_a1 = &D_801463E0 + temp_v0;
            if ((temp_a0 < 0x11) || (var_v1 = *(s16 *)((unsigned char *)var_a1 + (-8)), (var_v1 != 0)) || (var_s1 = temp_v0 + &D_80146400, var_t4 = temp_a0 - 0xA, (*(s16 *)((unsigned char *)var_s1 + (-8)) != 0))) {
                var_v0 = var_s2 * 2;
                goto block_6;
            }
            goto block_9;
        }
block_6:
        var_a1 = &D_801463E0 + var_v0;
        var_v1 = *(s16 *)((unsigned char *)var_a1 + (-8));
        temp_a0_2 = *(s16 *)((unsigned char *)temp_s0 + (6));
        var_s1 = var_v0 + &D_80146400;
        if ((var_v1 != 0) || (temp_v0_2 = *(s16 *)((unsigned char *)var_s1 + (-8)), (temp_v0_2 != 0))) {
            var_t4 = temp_a0_2 - 0xA;
block_9:
            *(s16 *)((unsigned char *)temp_s0 + (6)) = var_t4;
            if (var_v1 == 0) {
                if (*(s16 *)((unsigned char *)var_s1 + (-8)) == 0) {
                    if (*(s16 *)((unsigned char *)temp_s0 + (4)) < -0x57) {
                        *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) ((func_800AE0D0() % 4) + 0x1C);
                    } else {
                        *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) ((func_800AE0D0() % 15) + 0x11);
                    }
                    *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (4)) - 0xC);
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = 8U;
                    *(s16 *)((unsigned char *)temp_s0 + (8)) = 0x18;
                    *(s16 *)((unsigned char *)temp_s0 + (10)) = 8;
                    *(s16 *)((unsigned char *)var_s1 + (-8)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (-8)) + 1);
                } else {
                    *(s16 *)((unsigned char *)var_s1 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a1 + (-8)) = (s16) (var_v1 + 1);
                }
            } else if (var_v1 == 5) {
                *(u8 *)((unsigned char *)temp_s0 + (14)) = 0U;
                *(s16 *)((unsigned char *)temp_s0 + (8)) = 8;
                *(s16 *)((unsigned char *)temp_s0 + (10)) = 0x18;
                sp4C = var_a1;
                *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) ((func_800AE0D0() % 167) - 0x9F);
                temp_v1 = *(s16 *)((unsigned char *)temp_s0 + (4));
                if (temp_v1 >= -0x57) {
                    *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) (temp_v1 + 0x99);
                }
                *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x5F;
                *(s16 *)((unsigned char *)var_a1 + (-8)) = 0;
                *(s16 *)((unsigned char *)var_s1 + (-8)) = 0;
            } else {
                temp_v0_3 = *(s16 *)((unsigned char *)var_s1 + (-8));
                if (var_v1 == 4) {
                    if (temp_v0_3 == 0) {
                        *(s16 *)((unsigned char *)var_s1 + (-8)) = (s16) (temp_v0_3 + 1);
                        *(u8 *)((unsigned char *)temp_s0 + (14)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (14)) - 0x18);
                    } else {
                        *(s16 *)((unsigned char *)var_s1 + (-8)) = 0;
                        *(s16 *)((unsigned char *)var_a1 + (-8)) = (s16) (var_v1 + 1);
                    }
                } else if (temp_v0_3 == 0) {
                    *(s16 *)((unsigned char *)var_s1 + (-8)) = (s16) (temp_v0_3 + 1);
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (14)) + 0x18);
                } else {
                    *(s16 *)((unsigned char *)var_s1 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a1 + (-8)) = (s16) (var_v1 + 1);
                }
            }
        } else if ((temp_a0_2 >= 0x11) && (var_v1 == 0) && (temp_v0_2 == 0)) {
            *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) ((func_800AE0D0() % 167) - 0x9F);
            temp_v1_2 = *(s16 *)((unsigned char *)temp_s0 + (4));
            if (temp_v1_2 >= -0x57) {
                *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) (temp_v1_2 + 0x99);
            }
            *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x5F;
        }
        var_s2 += 1;
        var_t6 = var_s2 * 8;
    } while (var_s2 < 0x13);
}
