s32 func_801373C8(void) {
    s16 sp74;
    s16 sp68;
    s16 sp5E;
    s16 sp5C;
    ? sp50;
    ? *var_a3;
    OptActor44 *var_v0;
    s16 *var_a0;
    s16 *var_a2;
    s16 *var_v1;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t8;
    s16 var_a1;
    s16 var_t0;
    u8 temp_t9;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110D;
    if (temp_v1 != 0) {
        var_a0 = &sp68;
        var_a1 = 8;
        var_a2 = &sp5C;
        var_a3 = &sp50;
        var_t0 = 0x3C;
        temp_t6 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) / 17) * 0x10;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) = temp_t6;
        var_v0 = D_8011ECD0 + 0x1980;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) = (s16) (((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) / 17) * 0x10);
        if (temp_t6 < 0x1001) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) = 0x1000;
            *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) = 0x1000;
            var_v1 = &sp74;
            do {
                temp_t9 = *(u8 *)((unsigned char *)var_v0 + (1023));
                *var_v1 = -0x50;
                *var_a0 = var_a1;
                temp_t8 = *var_a0;
                temp_t7 = *var_v1;
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (976)) = var_t0;
                var_t0 += 2;
                var_a1 += 0x10;
                var_v1 += 2;
                var_a0 += 2;
                var_a2 += 2;
                var_a3 += 2;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0xC4;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                *(s8 *)((unsigned char *)var_v0 + (955)) = (s8) (temp_t9 | 0x80);
                *(s16 *)((unsigned char *)var_v0 + (994)) = temp_t8;
                *(s16 *)((unsigned char *)var_v0 + (990)) = temp_t7;
            } while (var_v0 != (D_8011ECD0 + 0x1A08));
            *(s8 *)((unsigned char *)D_8011ECD0 + (7551)) = 0;
            *(s8 *)((unsigned char *)D_8011ECD0 + (7619)) = 0;
            if (D_800B3220 == 0) {
                if (!(D_800E7D4F & 1)) {
                    sp5C = 0;
                }
                if (!(D_800E7D50 & 1)) {
                    sp5E = 0;
                }
            }
            func_8004F870(1, 2, &sp74, &sp68, &sp5C, &sp50);
        }
    }
    if (temp_v1 == 0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (10836)) = 7;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10850)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10854)) = -0x78;
        D_800E6280.unk_110D = temp_v1 + 1;
    } else if (temp_v1 == 1) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (7279)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7279)) & 0xFF7F);
        *(u8 *)((unsigned char *)D_8011ECD0 + (7347)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7347)) & 0xFF7F);
        *(u8 *)((unsigned char *)D_8011ECD0 + (7415)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7415)) & 0xFF7F);
        *(u8 *)((unsigned char *)D_8011ECD0 + (7483)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7483)) & 0xFF7F);
        temp_t7_2 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) / 9) * 8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = temp_t7_2;
        if (temp_t7_2 < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        func_8004284C();
    }
    *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (10815)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) = 0x1000;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) = 0x1000;
    return 0;
}
