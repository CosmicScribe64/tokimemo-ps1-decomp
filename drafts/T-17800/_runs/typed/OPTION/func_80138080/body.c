s32 func_80138080(void) {
    s16 spB4[16];
    s16 sp94[16];
    s16 sp74[16];
    s16 sp54[16];
    s16 *var_a0;
    s16 *var_a1;
    s16 *var_a3;
    OptActor44 *var_v1;
    s16 *var_a2;
    s16 temp_t6;
    s16 temp_t8;
    s16 var_t0;
    s32 *var_t1;
    s32 var_v0;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110D;
    if (temp_v1 == 1) {
        var_t1 = &D_800E7D10;
        var_a0 = spB4;
        var_a1 = sp94;
        var_a2 = sp74;
        var_a3 = sp54;
        var_t0 = 0x5C;
        temp_t6 = ((s16) ((OptActor44 *)D_8011ECD0)[159].unk_1A / 17) * 0x10;
        ((OptActor44 *)D_8011ECD0)[159].unk_1A = temp_t6;
        var_v0 = 0;
        ((OptActor44 *)D_8011ECD0)[159].unk_1C = (s16) (((s16) ((OptActor44 *)D_8011ECD0)[159].unk_1C / 17) * 0x10);
        if (temp_t6 < 0x1001) {
            ((OptActor44 *)D_8011ECD0)[159].unk_1A = 0x1000;
            ((OptActor44 *)D_8011ECD0)[159].unk_1C = 0x1000;
            var_v1 = &((OptActor44 *)D_8011ECD0)[96];
            do {
                var_v1[15].unk_18 = var_t0;
                var_t0 += 2;
                var_a0 += 1;
                var_a1 += 1;
                var_a3 += 1;
                var_t1 += 1;
                var_v1 += 1;
                *var_a2 = 0x50;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                var_v1[14].unk_03 = 0;
                *(s16 *)((unsigned char *)var_a0 + (-2)) = (s16) (((var_v0 / 7) << 7) - 0x78);
                var_v1[14].unk_26 = (s16) *(s16 *)((unsigned char *)var_a0 + (-2));
                *(s16 *)((unsigned char *)var_a1 + (-2)) = (s16) (((var_v0 % 7) * 0x12) - 0x48);
                var_v1[14].unk_2A = (s16) *(s16 *)((unsigned char *)var_a1 + (-2));
                if ((*(u8 *)((unsigned char *)var_t1 + (19)) == 0) && (D_800E6280.unk_53C[var_v0] == 0)) {
                    *var_a2 = 0;
                }
                var_v0 += 1;
                var_a2 += 1;
            } while (var_v0 != 0xE);
            menu_set(1, 0xE, spB4, sp94, sp74, sp54);
        }
    }
    if (temp_v1 == 0) {
        ((OptActor44 *)D_8011ECD0)[159].unk_18 = 8;
        ((OptActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        D_800E6280.unk_110D = temp_v1 + 1;
    } else if (temp_v1 == 1) {
        ((OptActor44 *)D_8011ECD0)[107].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[107].unk_03 & 0xFF7F);
        ((OptActor44 *)D_8011ECD0)[108].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[108].unk_03 & 0xFF7F);
        ((OptActor44 *)D_8011ECD0)[109].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[109].unk_03 & 0xFF7F);
        ((OptActor44 *)D_8011ECD0)[110].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[110].unk_03 & 0xFF7F);
        temp_t8 = ((s16) ((OptActor44 *)D_8011ECD0)[106].unk_1C / 9) * 8;
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = temp_t8;
        if (temp_t8 < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        k_disp_switch(6, 0);
        set_kanji_string(-0x80, -0x58, 0, (s32) "エンディング", 0);
        k_sub_disp_start(1);
        func_8004284C();
    }
    ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0;
    ((OptActor44 *)D_8011ECD0)[159].unk_03 = 0;
    ((OptActor44 *)D_8011ECD0)[159].unk_1A = 0x1000;
    ((OptActor44 *)D_8011ECD0)[159].unk_1C = 0x1000;
    return 0;
}
