s32 func_80154760(void) {
    DisplayActor44 *temp_s0;
    DisplayActor44 *temp_v0;
    DisplayActor44 *var_s0;
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_t8;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s3;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;

    switch (D_8015EDB0.unk_00) {
    case 0:
        switch (D_8015EBAC) {
        default:
            var_v1 = 0x3B;
            break;
        case 0:
            if (*D_8015EBCC == 1) {
                D_8015ED14 = 1;
            }
            var_v1 = 0x3B;
            break;
        case 1:
            if (D_8015EBD0 == 1) {
                D_8015ED14 = 1;
            }
            var_v1 = 0x3C;
            break;
        case 2:
            var_v1 = 0x3D;
            if (D_8015EBD4 == 1) {
                D_8015ED14 = 1;
            }
            break;
        }
        temp_v0 = &((DisplayActor44 *)D_8011ECD0)[var_v1];
        temp_v0[96].unk_03 = (u8) (temp_v0[96].unk_03 ^ 0x80);
        D_8015EC70 = var_v1;
        load_palette(D_8015EEE4, 0x18, 1, 3, 0xA);
        load_palette(D_8015EEE8, 0x19, 1, 3, 1);
        load_palette(D_8015EEE8, 0x1A, 1, 3, 0);
        ((DisplayActor44 *)D_8011ECD0)[136].unk_43 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[139].unk_43 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[137].unk_43 = 0x19;
        ((DisplayActor44 *)D_8011ECD0)[140].unk_43 = 0x19;
        ((DisplayActor44 *)D_8011ECD0)[138].unk_43 = 0x1A;
        ((DisplayActor44 *)D_8011ECD0)[141].unk_43 = 0x1A;
        var_s0 = ((DisplayActor44 *)D_8011ECD0);
        var_s3 = 0;
        do {
            var_s0[136].unk_38 = 0x51000000;
            var_s0[139].unk_38 = 0x41000000;
            var_s0[136].unk_07 = 0x80;
            var_s0[139].unk_07 = 0xFF;
            temp_s1 = var_s3 + 0x28;
            func_8013E97C(temp_s1, D_8015EBFC.unk_2C, D_8015EBFC.unk_30);
            temp_s2 = var_s3 + 0x2B;
            func_8013E97C(temp_s2, D_8015EBFC.unk_2C, D_8015EBFC.unk_30);
            temp_t8 = D_8015EDC4 & 0xFFFF0000;
            switch (temp_t8) {
            case 0x100000:
                func_8013E810(temp_s1, 5, 1, 1);
                func_8013E810(temp_s2, 5, 1, 1);
                break;
            case 0x200000:
                func_8013E810(temp_s1, 6, 1, 1);
                func_8013E810(temp_s2, 6, 1, 1);
                break;
            case 0x400000:
                func_8013E810(temp_s1, 7, 1, 1);
                func_8013E810(temp_s2, 7, 1, 1);
                break;
            default:
            case 0x8000000:
                func_8013E810(temp_s1, 8, 1, 1);
                func_8013E810(temp_s2, 8, 1, 1);
                break;
            }
            var_s3 += 1;
            var_s0 += 1;
        } while (var_s3 < 3);
        if (D_8015ED14 == 1) {
            ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[136].unk_03 | 1);
            temp_s0 = &((DisplayActor44 *)D_8011ECD0)[2];
            ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[137].unk_03 | 1);
            temp_s0[137].unk_03 = (u8) (temp_s0[137].unk_03 | 1);
            temp_s0[138].unk_03 = (u8) (temp_s0[138].unk_03 | 1);
            temp_s0[139].unk_03 = (u8) (temp_s0[139].unk_03 | 1);
            temp_s0[136].unk_03 = (u8) (temp_s0[136].unk_03 | 1);
        }
        func_8013F0F4(0x50B, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        var_s0_2 = (s32) (func_800A0070((s32) (D_8015EDB0.unk_00 << 0xB) / 1000) * 8) >> 0xC;
        if (var_s0_2 >= 8) {
            var_s0_2 = 7;
        }
        func_8013E97C(0x28, D_8015EBFC.unk_2C - var_s0_2, D_8015EBFC.unk_30);
        func_8013E97C(0x2B, D_8015EBFC.unk_2C - var_s0_2, D_8015EBFC.unk_30);
        func_8013E97C(0x2A, D_8015EBFC.unk_2C + var_s0_2, D_8015EBFC.unk_30);
        func_8013E97C(0x2D, D_8015EBFC.unk_2C + var_s0_2, D_8015EBFC.unk_30);
        var_v1_2 = D_8015EDB0.unk_08 + 0xA;
        if (var_v1_2 >= 0x1F5) {
            var_v1_2 += 0xA;
        }
        D_8015EDB0.unk_08 = var_v1_2;
        if (var_v1_2 >= 0x3E9) {
            func_8013F0F4(0x50B, 1, 0);
            func_8014EBD0();
        }
        break;
    case 2:
        var_s0_3 = (s32) (func_800A0070((s32) (D_8015EDB0.unk_00 << 0xB) / 1000) * 0x10) >> 0xC;
        if (var_s0_3 >= 0x10) {
            var_s0_3 = 0xF;
        }
        func_8013E97C(0x28, D_8015EBFC.unk_2C + var_s0_3, D_8015EBFC.unk_30);
        func_8013E97C(0x2B, D_8015EBFC.unk_2C + var_s0_3, D_8015EBFC.unk_30);
        func_8013E97C(0x2A, D_8015EBFC.unk_2C - var_s0_3, D_8015EBFC.unk_30);
        func_8013E97C(0x2D, D_8015EBFC.unk_2C - var_s0_3, D_8015EBFC.unk_30);
        var_v1_3 = D_8015EDB0.unk_08 + 0xA;
        if (var_v1_3 >= 0x1F5) {
            var_v1_3 += 0xA;
        }
        D_8015EDB0.unk_08 = var_v1_3;
        if (var_v1_3 >= 0x3E9) {
            func_8013F0F4(0x50B, 1, 0);
            func_8014EBD0();
        }
        break;
    case 3:
        var_s0_4 = (s32) (func_800A0070((s32) (D_8015EDB0.unk_00 << 0xB) / 1000) << 5) >> 0xC;
        if (var_s0_4 >= 0x20) {
            var_s0_4 = 0x1F;
        }
        func_8013E97C(0x28, D_8015EBFC.unk_2C - var_s0_4, D_8015EBFC.unk_30);
        func_8013E97C(0x2B, D_8015EBFC.unk_2C - var_s0_4, D_8015EBFC.unk_30);
        func_8013E97C(0x2A, D_8015EBFC.unk_2C + var_s0_4, D_8015EBFC.unk_30);
        func_8013E97C(0x2D, D_8015EBFC.unk_2C + var_s0_4, D_8015EBFC.unk_30);
        var_v1_4 = D_8015EDB0.unk_08 + 0xA;
        if (var_v1_4 >= 0x1F5) {
            var_v1_4 += 0xA;
        }
        D_8015EDB0.unk_08 = var_v1_4;
        if (var_v1_4 >= 0x3E9) {
            func_8014EBA8();
        }
        break;
    }
}
