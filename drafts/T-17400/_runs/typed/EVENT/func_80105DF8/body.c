void func_80105DF8(void) {
    void *sp54;
    void *sp4C;
    s16 temp_a1;
    s16 temp_a1_2;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 temp_v0_6;
    s16 var_s1;
    s16 var_t5;
    s16 var_v1;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_v0_3;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_t6;
    s32 var_v0;
    u8 *temp_s0;
    u8 *var_a0;
    u8 *var_a2;

    var_s1 = 4;
    var_t6 = 4 * 8;
    do {
        temp_s0 = ((var_t6 + var_s1) * 4) + ((Rec24 *)D_800EC190);
        *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) + 0xA);
        var_v0 = var_s1 * 2;
        if (!(func_800789A0() & 3)) {
            temp_a1 = *(s16 *)((unsigned char *)temp_s0 + (6));
            if (temp_a1 >= -0x52) {
                var_at = temp_a1 < -0x39;
                if (temp_a1 < -0x39) {
                    if (*(s16 *)((unsigned char *)temp_s0 + (4)) >= -0x57) {
                        goto block_5;
                    }
                    goto block_18;
                }
                goto block_6;
            }
block_5:
            var_at = temp_a1 < -0x39;
block_6:
            if (var_at == 0) {
                var_at_2 = temp_a1 < -0xB;
                if (temp_a1 < -0x1A) {
                    if (*(s16 *)((unsigned char *)temp_s0 + (4)) >= -0x77) {
                        goto block_9;
                    }
                    goto block_18;
                }
                goto block_10;
            }
block_9:
            var_at_2 = temp_a1 < -0xB;
block_10:
            if (var_at_2 == 0) {
                var_at_3 = temp_a1 < 5;
                if (temp_a1 < 3) {
                    temp_v0 = *(s16 *)((unsigned char *)temp_s0 + (4));
                    if ((temp_v0 < -0x8D) || (temp_v0 >= -0x6F)) {
                        goto block_14;
                    }
                    goto block_18;
                }
                goto block_15;
            }
block_14:
            var_at_3 = temp_a1 < 5;
block_15:
            var_v0 = var_s1 * 2;
            if (var_at_3 == 0) {
                temp_v0_2 = *(s16 *)((unsigned char *)temp_s0 + (4));
                if ((temp_v0_2 >= -0x7F) && (temp_v0_2 < -0x5C)) {
block_18:
                    temp_v0_3 = var_s1 * 2;
                    var_a2 = &D_80120CF0 + temp_v0_3;
                    var_v1 = *(s16 *)((unsigned char *)var_a2 + (-8));
                    if ((var_v1 != 0) || (var_a0 = temp_v0_3 + &D_80120D10, (*(s16 *)((unsigned char *)var_a0 + (-8)) != 0))) {
                        goto block_20;
                    }
                    goto block_23;
                }
block_20:
                var_v0 = var_s1 * 2;
                goto block_21;
            }
            goto block_21;
        }
block_21:
        var_a2 = &D_80120CF0 + var_v0;
        var_v1 = *(s16 *)((unsigned char *)var_a2 + (-8));
        temp_a1_2 = *(s16 *)((unsigned char *)temp_s0 + (6));
        var_a0 = var_v0 + &D_80120D10;
        if ((var_v1 != 0) || (temp_v0_4 = *(s16 *)((unsigned char *)var_a0 + (-8)), (temp_v0_4 != 0))) {
block_23:
            *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) - 0xA);
            if (var_v1 == 0) {
                temp_v0_5 = *(s16 *)((unsigned char *)var_a0 + (-8));
                if (temp_v0_5 == 0) {
                    if (*(s16 *)((unsigned char *)temp_s0 + (6)) < -0x44) {
                        *(u8 *)((unsigned char *)temp_s0 + (15)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (15)) + 8);
                    }
                    *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (4)) - 0xC);
                    *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) + 0x10);
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = 8U;
                    *(s16 *)((unsigned char *)temp_s0 + (8)) = 0x18;
                    *(s16 *)((unsigned char *)temp_s0 + (10)) = 8;
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_5 + 1);
                } else {
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (var_v1 + 1);
                }
            } else {
                if (var_v1 == 4) {
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = 0U;
                    *(u8 *)((unsigned char *)temp_s0 + (15)) = 0U;
                    *(s16 *)((unsigned char *)temp_s0 + (8)) = 8;
                    *(s16 *)((unsigned char *)temp_s0 + (10)) = 0x18;
                    sp4C = var_a0;
                    sp54 = var_a2;
                    temp_hi = func_800789A0(var_a0, *(s16 *)((unsigned char *)temp_s0 + (6)), var_a2) % 84;
                    *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x78;
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (-8)) = 0;
                    var_t5 = temp_hi - 0x9F;
                    goto block_42;
                }
                temp_v0_6 = *(s16 *)((unsigned char *)var_a0 + (-8));
                if (var_v1 == 5) {
                    if (temp_v0_6 == 0) {
                        *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_6 + 1);
                        *(u8 *)((unsigned char *)temp_s0 + (14)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (14)) - 0x18);
                    } else {
                        *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                        *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (var_v1 + 1);
                    }
                } else if (temp_v0_6 == 0) {
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_6 + 1);
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (14)) + 0x18);
                } else {
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (var_v1 + 1);
                }
            }
        } else if ((temp_a1_2 >= 0x19) && (var_v1 == 0) && (temp_v0_4 == 0)) {
            temp_hi_2 = func_800789A0(var_a0, temp_a1_2, var_a2) % 84;
            *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x78;
            var_t5 = temp_hi_2 - 0x9F;
block_42:
            *(s16 *)((unsigned char *)temp_s0 + (4)) = var_t5;
        }
        var_s1 += 1;
        var_t6 = var_s1 * 8;
    } while (var_s1 < 0xE);
}
