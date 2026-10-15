s32 func_80137784(s32 arg0) {
    void *sp30;
    s32 *temp_a0;
    s32 *temp_a0_2;
    s32 *temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t8;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 var_t2;
    u8 *temp_t1;
    u8 *temp_v0;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *temp_v1_3;

    temp_t1 = (arg0 * 0x24) + &D_80149208;
    var_t2 = *(u8 *)((unsigned char *)temp_t1 + (2));
    temp_v0 = D_8011ECD0 + (var_t2 * 0x44);
    if ((*(u8 *)((unsigned char *)temp_v0 + (3130)) != 0) && ((arg0 != 0) || (D_80122E94 == 0xFF))) {
        *(u8 *)((unsigned char *)temp_v0 + (3130)) = 0U;
        *(s8 *)((unsigned char *)temp_v0 + (3131)) = 0;
        if (arg0 == 0) {
            var_v0 = D_8011ECD0;
            do {
                var_v0 += 0x44;
                *(s8 *)((unsigned char *)var_v0 + (7006)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (7007)) = 0;
            } while (var_v0 != D_8011ED9C);
            sp30 = temp_t1;
            k_reset(0);
            var_t2 = *(u8 *)((unsigned char *)temp_t1 + (2));
        }
        temp_v1 = D_8011ECD0 + (arg0 * 0x44);
        *(u8 *)((unsigned char *)temp_v1 + (7279)) = 0xA4U;
        *(s8 *)((unsigned char *)temp_v1 + (7278)) = 1;
        temp_a2 = *(s32 *)((unsigned char *)temp_v1 + (6564));
        *(s16 *)((unsigned char *)temp_v1 + (7314)) = (s16) (((((s16) var_t2 % 10) << 5) - D_8014929A) + 0x1E0);
        temp_a3 = *(s32 *)((unsigned char *)temp_v1 + (7312));
        *(s16 *)((unsigned char *)temp_v1 + (7318)) = (s16) ((((s16) var_t2 / 10) * -0x10) - 0x30);
        temp_t7 = (s32) (temp_a2 - temp_a3) >> 7;
        (&D_8014982C)[arg0] = temp_t7;
        (&D_8014983C)[arg0] = (s32) (*(s32 *)((unsigned char *)temp_v1 + (6568)) - *(s32 *)((unsigned char *)temp_v1 + (7316))) >> 7;
        temp_a0 = &(&D_8014984C)[arg0];
        (*(s32 *)temp_a0) = temp_a3 - temp_a2;
        if (temp_t7 > 0) {
            *(u8 *)((unsigned char *)temp_v1 + (7279)) = (u8) (*(u8 *)((unsigned char *)temp_v1 + (7279)) | 1);
        }
        (&D_8014981C)[arg0] = 0;
        *(s8 *)((unsigned char *)temp_t1 + (3)) = 0;
        *(s8 *)((unsigned char *)temp_t1 + (4)) = 0;
        *(s16 *)((unsigned char *)temp_t1 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_t1 + (14)) = 0;
        *(s32 *)((unsigned char *)temp_t1 + (16)) = 0;
        *(s32 *)((unsigned char *)temp_t1 + (20)) = 0;
        *(s32 *)((unsigned char *)temp_t1 + (24)) = 0;
        *(s32 *)((unsigned char *)temp_t1 + (28)) = 0;
        if (temp_a0 == &D_8014984C) {
            D_8014922A = -0xA0;
            D_8014929C = 0;
        }
        goto block_26;
    }
    temp_v1_2 = D_8011ECD0 + (D_801491D8 * 0x44);
    if ((*(u8 *)((unsigned char *)temp_v1_2 + (3130)) != 0) && (D_80122E94 == 1) && (arg0 == 0)) {
        *(u8 *)((unsigned char *)temp_v1_2 + (3130)) = 0U;
        *(s8 *)((unsigned char *)temp_v1_2 + (3131)) = 0;
        var_v0_2 = D_8011ECD0;
        do {
            var_v0_2 += 0x44;
            *(s8 *)((unsigned char *)var_v0_2 + (7006)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (7007)) = 0;
        } while (var_v0_2 != D_8011ED9C);
        k_reset(0);
        D_80120952 = D_80120B72;
        D_8012097F = D_80120B9F;
        D_80120954 = 0;
        D_80120944 = 0;
        D_8012093E = 1;
        D_8012093F = 0xA4;
        D_80120962 = ((D_801491D8 << 5) - D_8014929A) + 0x1E0;
        D_80120966 = -0x30;
        temp_t8 = (s32) (D_80120674 - *(s32 *)((unsigned char *)D_8011ECD0 + (7312))) >> 7;
        D_8014983C = (s32) (D_80120678 - *(s32 *)((unsigned char *)D_8011ECD0 + (7316))) >> 7;
        D_8014984C = *(s32 *)((unsigned char *)D_8011ECD0 + (7312)) - D_80120674;
        D_8014982C = temp_t8;
        if (temp_t8 > 0) {
            D_8012093F = 0xA5;
        }
        D_8014981C = 0;
        D_8014920B = 0;
        *(s8 *)((unsigned char *)(&D_8014920B) + (1)) = 0;
        D_80149214 = 0;
        *(s16 *)((unsigned char *)(&D_80149214) + (2)) = 0;
        *(s32 *)((unsigned char *)(&D_80149214) + (4)) = 0;
        *(s32 *)((unsigned char *)(&D_80149214) + (8)) = 0;
        D_80149220 = 0;
        *(s32 *)((unsigned char *)(&D_80149220) + (4)) = 0;
        *(s16 *)((unsigned char *)(&D_80149220) + (10)) = -0xA0;
        D_8014929C = 0;
        goto block_26;
    }
    temp_a0_2 = &(&D_8014984C)[arg0];
    (*(s32 *)temp_a0_2) += (&D_8014982C)[arg0];
    temp_v1_3 = D_8011ECD0 + (arg0 * 0x44);
    *(s16 *)((unsigned char *)temp_v1_3 + (7314)) = (s16) (*(s16 *)((unsigned char *)temp_a0_2 + (2)) + *(s16 *)((unsigned char *)temp_v1_3 + (6566)));
    temp_a1 = &(&D_8014981C)[arg0];
    *(s32 *)((unsigned char *)temp_v1_3 + (7316)) = (s32) (*(s32 *)((unsigned char *)temp_v1_3 + (7316)) + (&D_8014983C)[arg0]);
    temp_t6 = *temp_a1 + 1;
    *temp_a1 = temp_t6;
    if (temp_t6 >= 0x80) {
        *(s16 *)((unsigned char *)temp_v1_3 + (6552)) = (s16) (arg0 + 0x90);
        if ((temp_a1 == &D_8014981C) && (D_80122E94 == 1)) {
            D_80120954 = (D_80149320[D_801491DC] * 6) + 5;
        } else {
            *(s16 *)((unsigned char *)temp_v1_3 + (7300)) = (s16) ((D_801491EC * 6) + 5);
        }
        *(s16 *)((unsigned char *)temp_v1_3 + (7314)) = (s16) (*(s16 *)((unsigned char *)temp_v1_3 + (6566)) - 8);
        *(s8 *)((unsigned char *)temp_v1_3 + (7278)) = 0x40;
        *(s8 *)((unsigned char *)temp_v1_3 + (7279)) = 0xA4;
        *(s32 *)((unsigned char *)temp_v1_3 + (7316)) = (s32) *(s32 *)((unsigned char *)temp_v1_3 + (6568));
        return 1;
    }
block_26:
    return 0;
}
