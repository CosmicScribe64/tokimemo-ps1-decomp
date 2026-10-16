? func_8004E93C(?, ?);                              /* extern */

s32 func_801378DC(void) {
    ? spB8;
    ? sp98;
    s16 sp78;
    ? sp58;
    ? *var_a0;
    ? *var_a1;
    ? *var_a3;
    OptActor44 *var_v1;
    s16 *var_a2;
    s16 temp_t6;
    s16 temp_t8;
    s16 var_t0;
    s32 *var_t1;
    s32 var_v0;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110D;
    if (temp_v1 != 0) {
        var_t1 = &D_800E7D10;
        var_a0 = &spB8;
        var_a1 = &sp98;
        var_a2 = &sp78;
        var_a3 = &sp58;
        var_t0 = 0x40;
        temp_t6 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) / 17) * 0x10;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) = temp_t6;
        var_v0 = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) = (s16) (((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) / 17) * 0x10);
        if (temp_t6 < 0x1001) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) = 0x1000;
            *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) = 0x1000;
            var_v1 = D_8011ECD0 + 0x1980;
            do {
                *(s16 *)((unsigned char *)var_v1 + (1044)) = var_t0;
                var_t0 += 2;
                var_a0 += 2;
                var_a1 += 2;
                var_a3 += 2;
                var_t1 += 1;
                var_v1 += 0x44;
                *var_a2 = 0x80;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                *(s8 *)((unsigned char *)var_v1 + (955)) = 0;
                *(s16 *)((unsigned char *)var_a0 + (-2)) = (s16) (((var_v0 / 7) << 7) - 0x78);
                *(s16 *)((unsigned char *)var_v1 + (990)) = (s16) *(s16 *)((unsigned char *)var_a0 + (-2));
                *(s16 *)((unsigned char *)var_a1 + (-2)) = (s16) (((var_v0 % 7) * 0x12) - 0x48);
                *(s16 *)((unsigned char *)var_v1 + (994)) = (s16) *(s16 *)((unsigned char *)var_a1 + (-2));
                if ((*(u8 *)((unsigned char *)var_t1 + (19)) == 0) && (D_800E6280.unk_53C[var_v0] == 0)) {
                    *var_a2 = 0;
                }
                var_v0 += 1;
                var_a2 += 2;
            } while (var_v0 != 0xE);
            func_8004F870(1, 0xE, &spB8, &sp98, &sp78, &sp58);
        }
    }
    if (temp_v1 == 0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (10836)) = 8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10850)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10854)) = -0x78;
        D_800E6280.unk_110D = temp_v1 + 1;
    } else if (temp_v1 == 1) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (7279)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7279)) & 0xFF7F);
        *(u8 *)((unsigned char *)D_8011ECD0 + (7347)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7347)) & 0xFF7F);
        *(u8 *)((unsigned char *)D_8011ECD0 + (7415)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7415)) & 0xFF7F);
        *(u8 *)((unsigned char *)D_8011ECD0 + (7483)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7483)) & 0xFF7F);
        temp_t8 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) / 9) * 8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = temp_t8;
        if (temp_t8 < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        func_8004E93C(6, 0);
        func_8004E788(-0x80, -0x58, 0, (s32) "フリートーク", 0);
        func_8004EAD4(1);
        func_8004284C();
    }
    *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (10815)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) = 0x1000;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) = 0x1000;
    return 0;
}
