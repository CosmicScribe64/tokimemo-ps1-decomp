void func_801344E4(void) {
    s16 sp64[4];
    s16 sp5C[4];
    s16 sp54[4];
    s16 sp4C[4];
    s16 *var_a2;
    s16 *var_a3;
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
        ((OptActor44 *)D_8011ECD0)[106].unk_18 = 3;
        ((OptActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0;
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = 0x7FFF;
        D_800E6280.unk_110D = 1;
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = 0x7FFF;
    } else {
        var_v1 = sp64;
        var_a0 = sp5C;
        var_a1 = -0x20;
        var_a2 = sp54;
        var_a3 = sp4C;
        var_t0 = 0x1C;
        temp_t7 = ((s16) ((OptActor44 *)D_8011ECD0)[106].unk_1A / 17) * 0x10;
        var_v0 = &((OptActor44 *)D_8011ECD0)[96];
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = (s16) (((s16) ((OptActor44 *)D_8011ECD0)->unk0 / 17) * 0x10);
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = temp_t7;
        if (temp_t7 < 0x1001) {
            ((OptActor44 *)D_8011ECD0)[106].unk_1C = 0x1000;
            ((OptActor44 *)D_8011ECD0)[106].unk_1A = 0x1000;
            do {
                temp_t6 = var_v0[11].unk_03;
                *var_v1 = -0x78;
                *var_a0 = var_a1;
                temp_t5 = *var_a0;
                temp_t9 = *var_v1;
                var_v0 += 1;
                var_v0[10].unk_18 = var_t0;
                var_t0 += 2;
                var_a1 += 0x28;
                var_v1 += 1;
                var_a0 += 1;
                var_a2 += 1;
                var_a3 += 1;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0x6C;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                var_v0[10].unk_03 = (s8) (temp_t6 | 0x80);
                var_v0[10].unk_2A = temp_t5;
                var_v0[10].unk_26 = temp_t9;
            } while ((u32) var_v0 < (u32) (&((OptActor44 *)D_8011ECD0)[99]));
            menu_set(1, 3, sp64, sp5C, sp54, sp4C);
            var_v0_2 = &((OptActor44 *)D_8011ECD0)[96];
            var_v1_2 = 0x22;
            var_a0_2 = -0x28;
            do {
                var_v0_2[14].unk_18 = var_v1_2;
                var_v0_2[14].unk_03 = (u8) (var_v0_2[14].unk_03 | 0x80);
                var_v0_2[14].unk_26 = 0;
                var_v0_2[14].unk_2A = var_a0_2;
                if (D_800E6280.unk_F75 == 1) {
                    ((OptActor44 *)D_8011ECD0)[14].unk_18 = (s16) (((OptActor44 *)D_8011ECD0)[14].unk_18 + 1);
                } else {
                    ((OptActor44 *)D_8011ECD0)[15].unk_18 = (s16) (((OptActor44 *)D_8011ECD0)[15].unk_18 + 1);
                }
                var_v0_2 += 1;
                var_v1_2 += 2;
                var_a0_2 += 0x10;
            } while ((u32) var_v0_2 < (u32) (&((OptActor44 *)D_8011ECD0)[98]));
            var_v0_3 = &((OptActor44 *)D_8011ECD0)[96];
            var_v1_3 = 0;
            var_a0_3 = 0x26;
            do {
                var_v0_3[16].unk_18 = var_a0_3;
                var_v0_3[16].unk_03 = (u8) (var_v0_3[16].unk_03 | 0x80);
                var_v0_3[16].unk_26 = 0;
                var_v0_3[16].unk_2A = var_v1_3;
                if ((u8) D_800E6280.unk_F70 == 0) {
                    ((OptActor44 *)D_8011ECD0)[16].unk_18 = (s16) (((OptActor44 *)D_8011ECD0)[16].unk_18 + 1);
                } else {
                    ((OptActor44 *)D_8011ECD0)[17].unk_18 = (s16) (((OptActor44 *)D_8011ECD0)[17].unk_18 + 1);
                }
                var_v0_3 += 1;
                var_v1_3 += 0x10;
                var_a0_3 += 2;
            } while ((u32) var_v0_3 < (u32) (&((OptActor44 *)D_8011ECD0)[98]));
            var_v0_4 = &((OptActor44 *)D_8011ECD0)[96];
            var_v1_4 = 0x2A;
            var_a0_4 = 0x28;
            do {
                temp_t7_2 = var_v0_4[18].unk_03;
                var_v0_4 += 1;
                var_v0_4[17].unk_18 = var_v1_4;
                var_v0_4[17].unk_2A = var_a0_4;
                var_a0_4 += 0x10;
                var_v1_4 += 2;
                var_v0_4[17].unk_03 = (s8) (temp_t7_2 | 0x80);
                var_v0_4[17].unk_26 = 0;
            } while (var_v0_4 != (&((OptActor44 *)D_8011ECD0)[98]));
            func_8004284C();
        }
    }
    func_801324E8();
}
