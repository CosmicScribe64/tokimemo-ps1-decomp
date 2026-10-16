s32 func_80156064(void) {
    DisplayActor44 *var_s0;
    DisplayActor44 *var_s0_2;
    DisplayActor44 *var_s0_3;
    s16 temp_v0;
    s16 temp_v0_3;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 temp_s3;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 var_s2;

    func_8013F0F4(0x50F, 0, 1);
    var_s1 = &D_8015EBBC;
    switch (D_8015EDB0.unk_00) {
    case 0:
        var_s0 = &((DisplayActor44 *)D_8011ECD0)[4];
        do {
            var_s0 += 4;
            *(s32 *)((unsigned char *)var_s1 + (204)) = 0;
            var_s0[93].unk_01 = 5;
            *(s32 *)((unsigned char *)var_s1 + (208)) = 0;
            var_s0[94].unk_01 = 5;
            *(s32 *)((unsigned char *)var_s1 + (212)) = 0;
            var_s0[95].unk_01 = 5;
            var_s1 += 4;
            *(s32 *)((unsigned char *)var_s1 + (184)) = 0;
            var_s0[92].unk_01 = 5;
        } while (var_s0 != (&((DisplayActor44 *)D_8011ECD0)[16]));
        func_8014EBD0();
        return;
    case 1:
        ((DisplayActor44 *)D_8011ECD0)[147].unk_2A = (s16) ((*(s32 *)D_8011ECD0) - 2);
        temp_v0 = (D_8015EDB0.unk_04 << 6) + 0x1000;
        ((DisplayActor44 *)D_8011ECD0)[147].unk_1A = temp_v0;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (10024)) = temp_v0;
        D_8015EDB0.unk_04 += 1;
        var_s0_2 = &((DisplayActor44 *)D_8011ECD0)[4];
        var_s1_2 = &D_8015EBBC;
        var_s2 = 4;
        do {
            if ((*(s32 *)((unsigned char *)var_s1_2 + (200)) == 0) && (func_8013E9A8(0x2D) == 0)) {
                *(s32 *)((unsigned char *)var_s1_2 + (200)) = 1;
                temp_s3 = (func_8013E9A8(0x80) + ((DisplayActor44 *)D_8011ECD0)[147].unk_26) - 0x40;
                func_8013E97C(var_s2, temp_s3 + 0xA0, (((DisplayActor44 *)D_8011ECD0)[147].unk_2A - func_8013E9A8(0x28)) + 0x78);
                func_8013E810(var_s2, func_8013E9A8(2) + 8, 1, 1);
            }
            temp_v0_2 = *(s32 *)((unsigned char *)var_s1_2 + (200));
            temp_t4 = temp_v0_2 + 1;
            if (temp_v0_2 != 0) {
                *(s32 *)((unsigned char *)var_s1_2 + (200)) = temp_t4;
                if (temp_t4 >= 0x1F) {
                    switch (temp_t4) {
                    case 31:
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 32:
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 33:
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 34:
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 35:
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 36:
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 37:
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    }
                }
            }
            var_s0_2[96].unk_2A = (s16) (var_s0_2[96].unk_2A + 1);
            var_s2 += 1;
            temp_v0_3 = (D_8015EDB0.unk_04 << 6) + 0x1000;
            var_s1_2 += 1;
            var_s0_2 += 1;
            var_s0_2[95].unk_1A = temp_v0_3;
            *(s16 *)((unsigned char *)var_s0_2 + (6488)) = temp_v0_3;
        } while (var_s2 < 0x10);
        if (((DisplayActor44 *)D_8011ECD0)[147].unk_2A < -0xC0) {
            func_8013E810(0x33, 0xFF, 1, 0);
            var_s0_3 = &((DisplayActor44 *)D_8011ECD0)[4];
            do {
                var_s0_3 += 4;
                var_s0_3[93].unk_1A = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6352)) = 0x1000;
                var_s0_3[94].unk_1A = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6420)) = 0x1000;
                var_s0_3[95].unk_1A = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6488)) = 0x1000;
                var_s0_3[92].unk_1A = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6284)) = 0x1000;
            } while (var_s0_3 != (&((DisplayActor44 *)D_8011ECD0)[16]));
            ((DisplayActor44 *)D_8011ECD0)[147].unk_1A = 0x1000;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (10024)) = 0x1000;
            func_8014EBA8();
        }
        return;
    }
}
