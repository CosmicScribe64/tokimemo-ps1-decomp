void func_80136688(void) {
    s16 sp78;
    s16 sp6C;
    ? sp60;
    ? sp54;
    ? *var_a2;
    ? *var_a3;
    OptActor44 *var_v0;
    s16 *var_a0;
    s16 *var_v1;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t8;
    s16 var_a1;
    s16 var_t0;
    u32 temp_hi;
    u8 temp_t9;

    if (D_800E6280.unk_110D == 0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (7232)) = 5;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7246)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7250)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x7FFF;
        D_800E6280.unk_110D = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x7FFF;
    } else {
        var_v1 = &sp78;
        var_a0 = &sp6C;
        var_a1 = 0x18;
        var_a2 = &sp60;
        var_a3 = &sp54;
        var_t0 = 0x2E;
        temp_t6 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) / 17) * 0x10;
        var_v0 = D_8011ECD0 + 0x1980;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = (s16) (((s16) D_8011ECD0->unk0 / 17) * 0x10);
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = temp_t6;
        if (temp_t6 < 0x1001) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x1000;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x1000;
            do {
                temp_t9 = *(u8 *)((unsigned char *)var_v0 + (751));
                *var_v1 = 0;
                *var_a0 = var_a1;
                temp_t8 = *var_a0;
                temp_t7 = *var_v1;
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (704)) = var_t0;
                var_t0 += 2;
                var_a1 += 0x18;
                var_v1 += 2;
                var_a0 += 2;
                var_a2 += 2;
                var_a3 += 2;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0x80;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                *(s8 *)((unsigned char *)var_v0 + (683)) = (s8) (temp_t9 | 0x80);
                *(s16 *)((unsigned char *)var_v0 + (722)) = temp_t8;
                *(s16 *)((unsigned char *)var_v0 + (718)) = temp_t7;
            } while (var_v0 != (D_8011ECD0 + 0x1A4C));
            func_8004F870(1, 3, &sp78, &sp6C, &sp60, &sp54);
            temp_hi = (u8) D_800E6280.unk_03C % 3U;
            switch (temp_hi) {                      /* irregular */
            case 1:
                *(s16 *)((unsigned char *)D_8011ECD0 + (7300)) = (s16) (D_8011ECD0->unk0 + 1);
                D_800E6280.unk_1094 = 0;
                break;
            case 0:
                D_8011ECD0->unk0 = (s16) (D_8011ECD0->unk0 + 1);
                D_800E6280.unk_1094 = 1;
                break;
            default:
            case 2:
                *(s16 *)((unsigned char *)D_8011ECD0 + (7436)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (7436)) + 1);
                D_800E6280.unk_1094 = 2;
                break;
            }
            func_8004284C();
        }
    }
    func_801324E8();
}
