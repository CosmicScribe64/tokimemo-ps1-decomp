void func_801344E4(void) {
    s16 sp64;
    s16 sp5C;
    ? sp54;
    ? sp4C;
    ? *var_a2;
    ? *var_a3;
    OptActor44 *var_v0;
    OptActor44 *var_v0_2;
    OptActor44 *var_v0_3;
    OptActor44 *var_v0_4;
    s16 *var_a0;
    s16 *var_v1;
    s16 temp_t5;
    s16 temp_t7;
    s16 temp_t9;
    s16 var_a0_2;
    s16 var_a0_3;
    s16 var_a0_4;
    s16 var_a1;
    s16 var_t0;
    s16 var_v1_2;
    s16 var_v1_3;
    s16 var_v1_4;
    u8 temp_t6;
    u8 temp_t7_2;

    if (D_800E6280.unk_110D == 0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (7232)) = 3;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7246)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7250)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x7FFF;
        D_800E6280.unk_110D = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x7FFF;
    } else {
        var_v1 = &sp64;
        var_a0 = &sp5C;
        var_a1 = -0x20;
        var_a2 = &sp54;
        var_a3 = &sp4C;
        var_t0 = 0x1C;
        temp_t7 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) / 17) * 0x10;
        var_v0 = D_8011ECD0 + 0x1980;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = (s16) (((s16) D_8011ECD0->unk0 / 17) * 0x10);
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = temp_t7;
        if (temp_t7 < 0x1001) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x1000;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x1000;
            do {
                temp_t6 = *(u8 *)((unsigned char *)var_v0 + (751));
                *var_v1 = -0x78;
                *var_a0 = var_a1;
                temp_t5 = *var_a0;
                temp_t9 = *var_v1;
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (704)) = var_t0;
                var_t0 += 2;
                var_a1 += 0x28;
                var_v1 += 2;
                var_a0 += 2;
                var_a2 += 2;
                var_a3 += 2;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0x6C;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                *(s8 *)((unsigned char *)var_v0 + (683)) = (s8) (temp_t6 | 0x80);
                *(s16 *)((unsigned char *)var_v0 + (722)) = temp_t5;
                *(s16 *)((unsigned char *)var_v0 + (718)) = temp_t9;
            } while ((u32) var_v0 < (u32) (D_8011ECD0 + 0x1A4C));
            func_8004F870(1, 3, &sp64, &sp5C, &sp54, &sp4C);
            var_v0_2 = D_8011ECD0 + 0x1980;
            var_v1_2 = 0x22;
            var_a0_2 = -0x28;
            do {
                *(s16 *)((unsigned char *)var_v0_2 + (976)) = var_v1_2;
                *(u8 *)((unsigned char *)var_v0_2 + (955)) = (u8) (*(u8 *)((unsigned char *)var_v0_2 + (955)) | 0x80);
                *(s16 *)((unsigned char *)var_v0_2 + (990)) = 0;
                *(s16 *)((unsigned char *)var_v0_2 + (994)) = var_a0_2;
                if (D_800E6280.unk_F75 == 1) {
                    *(s16 *)((unsigned char *)D_8011ECD0 + (976)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (976)) + 1);
                } else {
                    *(s16 *)((unsigned char *)D_8011ECD0 + (1044)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (1044)) + 1);
                }
                var_v0_2 += 0x44;
                var_v1_2 += 2;
                var_a0_2 += 0x10;
            } while ((u32) var_v0_2 < (u32) (D_8011ECD0 + 0x1A08));
            var_v0_3 = D_8011ECD0 + 0x1980;
            var_v1_3 = 0;
            var_a0_3 = 0x26;
            do {
                *(s16 *)((unsigned char *)var_v0_3 + (1112)) = var_a0_3;
                *(u8 *)((unsigned char *)var_v0_3 + (1091)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (1091)) | 0x80);
                *(s16 *)((unsigned char *)var_v0_3 + (1126)) = 0;
                *(s16 *)((unsigned char *)var_v0_3 + (1130)) = var_v1_3;
                if ((u8) D_800E6280.unk_F70 == 0) {
                    *(s16 *)((unsigned char *)D_8011ECD0 + (1112)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (1112)) + 1);
                } else {
                    *(s16 *)((unsigned char *)D_8011ECD0 + (1180)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (1180)) + 1);
                }
                var_v0_3 += 0x44;
                var_v1_3 += 0x10;
                var_a0_3 += 2;
            } while ((u32) var_v0_3 < (u32) (D_8011ECD0 + 0x1A08));
            var_v0_4 = D_8011ECD0 + 0x1980;
            var_v1_4 = 0x2A;
            var_a0_4 = 0x28;
            do {
                temp_t7_2 = *(u8 *)((unsigned char *)var_v0_4 + (1227));
                var_v0_4 += 0x44;
                *(s16 *)((unsigned char *)var_v0_4 + (1180)) = var_v1_4;
                *(s16 *)((unsigned char *)var_v0_4 + (1198)) = var_a0_4;
                var_a0_4 += 0x10;
                var_v1_4 += 2;
                *(s8 *)((unsigned char *)var_v0_4 + (1159)) = (s8) (temp_t7_2 | 0x80);
                *(s16 *)((unsigned char *)var_v0_4 + (1194)) = 0;
            } while (var_v0_4 != (D_8011ECD0 + 0x1A08));
            func_8004284C();
        }
    }
    func_801324E8();
}
