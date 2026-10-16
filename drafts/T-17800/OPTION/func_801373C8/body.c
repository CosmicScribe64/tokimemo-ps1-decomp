s32 func_801373C8(void) {
    s16 sp74[6];
    s16 sp68[6];
    s16 sp5C[6];
    s16 sp50[6];
    s16 sp5E;
    s16 *var_a3;
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
        var_a0 = sp68;
        var_a1 = 8;
        var_a2 = sp5C;
        var_a3 = sp50;
        var_t0 = 0x3C;
        temp_t6 = ((s16) ((OptActor44 *)D_8011ECD0)[159].unk_1A / 17) * 0x10;
        ((OptActor44 *)D_8011ECD0)[159].unk_1A = temp_t6;
        var_v0 = &((OptActor44 *)D_8011ECD0)[96];
        ((OptActor44 *)D_8011ECD0)[159].unk_1C = (s16) (((s16) ((OptActor44 *)D_8011ECD0)[159].unk_1C / 17) * 0x10);
        if (temp_t6 < 0x1001) {
            ((OptActor44 *)D_8011ECD0)[159].unk_1A = 0x1000;
            ((OptActor44 *)D_8011ECD0)[159].unk_1C = 0x1000;
            var_v1 = sp74;
            do {
                temp_t9 = var_v0[15].unk_03;
                *var_v1 = -0x50;
                *var_a0 = var_a1;
                temp_t8 = *var_a0;
                temp_t7 = *var_v1;
                var_v0 += 1;
                var_v0[14].unk_18 = var_t0;
                var_t0 += 2;
                var_a1 += 0x10;
                var_v1 += 1;
                var_a0 += 1;
                var_a2 += 1;
                var_a3 += 1;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0xC4;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                var_v0[14].unk_03 = (s8) (temp_t9 | 0x80);
                var_v0[14].unk_2A = temp_t8;
                var_v0[14].unk_26 = temp_t7;
            } while (var_v0 != (&((OptActor44 *)D_8011ECD0)[98]));
            ((OptActor44 *)D_8011ECD0)[111].unk_03 = 0;
            ((OptActor44 *)D_8011ECD0)[112].unk_03 = 0;
            if (D_800B3220 == 0) {
                if (!(D_800E7D4F & 1)) {
                    sp5C[0] = 0;
                }
                if (!(D_800E7D50 & 1)) {
                    sp5E = 0;
                }
            }
            menu_set(1, 2, sp74, sp68, sp5C, sp50);
        }
    }
    if (temp_v1 == 0) {
        ((OptActor44 *)D_8011ECD0)[159].unk_18 = 7;
        ((OptActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        D_800E6280.unk_110D = temp_v1 + 1;
    } else if (temp_v1 == 1) {
        ((OptActor44 *)D_8011ECD0)[107].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[107].unk_03 & 0xFF7F);
        ((OptActor44 *)D_8011ECD0)[108].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[108].unk_03 & 0xFF7F);
        ((OptActor44 *)D_8011ECD0)[109].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[109].unk_03 & 0xFF7F);
        ((OptActor44 *)D_8011ECD0)[110].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[110].unk_03 & 0xFF7F);
        temp_t7_2 = ((s16) ((OptActor44 *)D_8011ECD0)[106].unk_1A / 9) * 8;
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = temp_t7_2;
        if (temp_t7_2 < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        func_8004284C();
    }
    ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0;
    ((OptActor44 *)D_8011ECD0)[159].unk_03 = 0;
    ((OptActor44 *)D_8011ECD0)[159].unk_1A = 0x1000;
    ((OptActor44 *)D_8011ECD0)[159].unk_1C = 0x1000;
    return 0;
}
