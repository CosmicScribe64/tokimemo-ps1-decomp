extern u8 D_8012093E;
extern s16 D_8014929A;
extern ? D_801492A4;
extern u8 D_801492A8;

s32 func_801365C8(void) {
    u8 *sp38;
    TaiikuSlots *var_a1;
    s16 temp_v0_3;
    s16 var_s2;
    s16 var_t6;
    s32 var_at;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    s32 var_v1_2;
    u32 temp_v1;
    u8 *var_a0;
    u8 *var_a0_2;
    u8 temp_t7;
    u8 temp_v0;
    u8 temp_v0_2;
    void *var_v1;

    var_a0 = &D_801492A8;
    var_s0 = 0;
    do {
        temp_t7 = *var_a0 + 1;
        temp_v1 = temp_t7 & 0xFF;
        *var_a0 = temp_t7;
        if (temp_v1 >= 0x79U) {
            *var_a0 = 0;
        } else if (temp_v1 == 0xA) {
            sp38 = var_a0;
            *(&D_801492A4 + var_s0) = (func_800AE0D0() % 20) + 0xA;
        } else if (temp_v1 == 0x1B) {
            *(&D_801492A4 + var_s0) = 0xFF;
        } else {
            if (temp_v1 >= 0x13U) {
                temp_v0 = *(&D_801492A4 + var_s0);
                var_at = temp_v1 < 0xBU;
                if (temp_v0 != 0xFF) {
                    var_v1 = D_8011ECD0 + (temp_v0 * 0x44);
                    var_t6 = *(s16 *)((unsigned char *)var_v1 + (3170)) + 1;
                    goto block_14;
                }
                goto block_11;
            }
            var_at = temp_v1 < 0xBU;
block_11:
            if (var_at == 0) {
                temp_v0_2 = *(&D_801492A4 + var_s0);
                if (temp_v0_2 != 0xFF) {
                    var_v1 = D_8011ECD0 + (temp_v0_2 * 0x44);
                    var_t6 = *(s16 *)((unsigned char *)var_v1 + (3170)) - 1;
block_14:
                    *(s16 *)((unsigned char *)var_v1 + (3170)) = var_t6;
                }
            }
        }
        var_s0 += 1;
        var_a0 += 1;
    } while (var_s0 < 2);
    var_a0_2 = D_8011ECD0;
    var_s0_2 = 0;
    var_s1 = 0;
    var_s2 = 0x1E0 - D_8014929A;
    do {
        var_v1_2 = 0;
        if (var_s1 >= 0x140) {
            var_s1 -= 0x140;
            var_s2 -= 0x140;
        }
        *(s16 *)((unsigned char *)var_a0_2 + (3166)) = var_s2;
        if (var_s0_2 >= 0xA) {
            if (var_s0_2 < 0x14) {
                *(s16 *)((unsigned char *)var_a0_2 + (3166)) = (s16) (*(s16 *)((unsigned char *)var_a0_2 + (3166)) + 0x10);
            }
        }
        if (var_s0_2 == 0x13) {
            *(s8 *)((unsigned char *)var_a0_2 + (3130)) = 0;
            goto block_29;
        }
        temp_v0_3 = *(s16 *)((unsigned char *)var_a0_2 + (3166));
        if ((temp_v0_3 >= -0xCF) && (temp_v0_3 < 0x141)) {
            *(s8 *)((unsigned char *)var_a0_2 + (3130)) = 1;
            *(s8 *)((unsigned char *)var_a0_2 + (3131)) = 0xA4;
        } else {
            *(s8 *)((unsigned char *)var_a0_2 + (3130)) = 0;
block_29:
            *(s8 *)((unsigned char *)var_a0_2 + (3131)) = 0;
        }
        var_a1 = &D_80149208;
loop_31:
        if ((var_s0_2 == D_801491D8) && (D_8012093E != 0) && (D_80122E94 == 1)) {
            *(s8 *)((unsigned char *)var_a0_2 + (3130)) = 0;
            goto block_41;
        }
        if ((var_s0_2 == var_a1->pad00[2]) && (*(u8 *)((unsigned char *)(D_8011ECD0 + (var_v1_2 * 0x44)) + (7278)) != 0)) {
            if (var_v1_2 == 0) {
                if (D_80122E94 == 0xFF) {
                    *(s8 *)((unsigned char *)var_a0_2 + (3130)) = 0;
                    goto block_41;
                }
            } else {
                *(s8 *)((unsigned char *)var_a0_2 + (3130)) = 0;
block_41:
                *(s8 *)((unsigned char *)var_a0_2 + (3131)) = 0;
            }
        }
        if ((var_s0_2 == (s32) var_a1->pad00[2]) && (*(u8 *)((unsigned char *)(D_8011ECD0 + (var_v1_2 * 0x44)) + (7278)) == 0)) {
            *(s8 *)((unsigned char *)var_a0_2 + (3130)) = 1;
            *(s8 *)((unsigned char *)var_a0_2 + (3131)) = 0xA4;
        }
        var_v1_2 += 1;
        var_a1 += 0x24;
        if (var_v1_2 != 4) {
            goto loop_31;
        }
        var_s0_2 += 1;
        var_a0_2 += 0x44;
        var_s1 += 0x20;
        var_s2 += 0x20;
    } while (var_s0_2 != 0x1E);
    return (s32) var_a1->pad00[2];
}
