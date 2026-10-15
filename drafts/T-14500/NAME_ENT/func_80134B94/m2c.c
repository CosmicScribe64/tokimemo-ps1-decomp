void func_80134B94(s32 arg0) {
    s32 sp30;
    s32 sp2C;
    u8 *sp28;
    s32 var_a3;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    u8 *temp_v1;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 *var_v1_3;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t5;

    var_a3 = 0;
    var_v1 = D_8014CC74;
    sp2C = func_800AE0D0() % 15;
    if (arg0 == 0) {
        var_v0 = 0;
        do {
            temp_t3 = D_800B3644[var_v0 & 1];
            var_v0 += 1;
            if (*var_v1 == temp_t3) {
                var_a3 += 1;
            }
            var_v1 += 1;
        } while (var_v0 != 6);
        if (D_8014CC74[var_a3] == 0) {
            sp30 = var_a3;
            func_800AE090(D_8014CC74, (sp2C * 8) + D_8014CE6C, 8);
        }
        if (var_a3 == 6) {
            sp30 = var_a3;
            func_800AE090(D_8014CC74, (sp2C * 8) + D_8014CE6C, 8);
        }
        if (*D_8014CC74 == 0) {
            sp30 = var_a3;
            func_800AE090(D_8014CC74, (sp2C * 8) + D_8014CE6C, 8);
        }
    }
    if (arg0 == 1) {
        var_v1_2 = D_8014CC7C;
        var_v0_2 = 0;
        do {
            temp_t4 = D_800B3644[var_v0_2 & 1];
            var_v0_2 += 1;
            if (*var_v1_2 == temp_t4) {
                var_a3 += 1;
            }
            var_v1_2 += 1;
        } while (var_v0_2 != 6);
        if (D_8014CC7C[var_a3] == 0) {
            sp30 = var_a3;
            func_800AE090(D_8014CC7C, (sp2C * 8) + D_8014CEE4, 8);
        }
        if (var_a3 == 6) {
            sp30 = var_a3;
            func_800AE090(D_8014CC7C, (sp2C * 8) + D_8014CEE4, 8);
        }
        if (*D_8014CC7C == 0) {
            sp30 = var_a3;
            func_800AE090(D_8014CC7C, (sp2C * 8) + D_8014CEE4, 8);
        }
    }
    if (arg0 == 2) {
        var_v0_3 = 0;
        var_v1_3 = D_8014CC84;
        do {
            temp_t5 = D_800B3644[var_v0_3 & 1];
            var_v0_3 += 1;
            if (*var_v1_3 == temp_t5) {
                var_a3 += 1;
            }
            var_v1_3 += 1;
        } while (var_v0_3 != 0xA);
        temp_v1 = &D_8014CC84[var_a3];
        if (*temp_v1 == 0) {
            sp28 = temp_v1;
            func_800AE090(D_8014CC84, (sp2C * 0xC) + D_8014CF5C, 0xC);
        }
        if (temp_v1 == &D_8014CC8E) {
            func_800AE090(D_8014CC84, (sp2C * 0xC) + D_8014CF5C, 0xC);
        }
        if (*D_8014CC84 == 0) {
            func_800AE090(D_8014CC84, (sp2C * 0xC) + D_8014CF5C, 0xC);
        }
    }
}
