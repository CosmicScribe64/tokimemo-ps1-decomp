s32 func_80154760(void) {
    s32 i;
    s32 idx;
    s32 v;

    switch (D_8015EDB0.unk_00) {
    case 0:
        switch (D_8015EBAC) {
        default:
            idx = 0x3B;
            break;
        case 0:
            if (D_8015EBCC[0] == 1) {
                D_8015ED14 = 1;
            }
            idx = 0x3B;
            break;
        case 1:
            if (D_8015EBCC[1] == 1) {
                D_8015ED14 = 1;
            }
            idx = 0x3C;
            break;
        case 2:
            if (D_8015EBCC[2] == 1) {
                D_8015ED14 = 1;
            }
            idx = 0x3D;
            break;
        }
        ((DisplayActor44 *)D_8011ECD0)[idx + 96].unk_03 ^= 0x80;
        D_8015EC70 = idx;
        load_palette(D_8015EEE4, 0x18, 1U, 3, 0xA);
        load_palette(D_8015EEE8, 0x19, 1, 3, 1);
        load_palette(D_8015EEE8, 0x1A, 1, 3, 0);
        ((DisplayActor44 *)D_8011ECD0)[136].unk_43 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[139].unk_43 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[137].unk_43 = 0x19;
        ((DisplayActor44 *)D_8011ECD0)[140].unk_43 = 0x19;
        ((DisplayActor44 *)D_8011ECD0)[138].unk_43 = 0x1A;
        ((DisplayActor44 *)D_8011ECD0)[141].unk_43 = 0x1A;
        for (i = 0; i < 3; i++) {
            ((DisplayActor44 *)D_8011ECD0)[136 + i].unk_38 = 0x51000000;
            ((DisplayActor44 *)D_8011ECD0)[139 + i].unk_38 = 0x41000000;
            ((DisplayActor44 *)D_8011ECD0)[136 + i].unk_07 = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[139 + i].unk_07 = 0xFF;
            func_8013E97C(i + 0x28, D_8015EC28, D_8015EC2C);
            func_8013E97C(i + 0x2B, D_8015EC28, D_8015EC2C);
            switch (D_8015EDC4 & 0xFFFF0000) {
            case 0x100000:
                func_8013E810(i + 0x28, 5, 1, 1);
                func_8013E810(i + 0x2B, 5, 1, 1);
                break;
            case 0x200000:
                func_8013E810(i + 0x28, 6, 1, 1);
                func_8013E810(i + 0x2B, 6, 1, 1);
                break;
            case 0x400000:
                func_8013E810(i + 0x28, 7, 1, 1);
                func_8013E810(i + 0x2B, 7, 1, 1);
                break;
            default:
            case 0x8000000:
                func_8013E810(i + 0x28, 8, 1, 1);
                func_8013E810(i + 0x2B, 8, 1, 1);
                break;
            }
        }
        if (D_8015ED14 == 1) {
            ((DisplayActor44 *)D_8011ECD0)[136].unk_03 |= 1;
            i = 2;
            ((DisplayActor44 *)D_8011ECD0)[137].unk_03 |= 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 137].unk_03 |= 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 138].unk_03 |= 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 139].unk_03 |= 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_03 |= 1;
        }
        func_8013F0F4(0x50B, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        v = func_800A0070(D_8015EDB8 * 0x800 / 1000) * 8 >> 12;
        if (v >= 8) {
            v = 7;
        }
        func_8013E97C(0x28, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 + v, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 + v, D_8015EC2C);
        D_8015EDB8 += 0xA;
        if (D_8015EDB8 >= 0x1F5) {
            D_8015EDB8 += 0xA;
        }
        if (D_8015EDB8 >= 0x3E9) {
            func_8013F0F4(0x50B, 1, 0);
            func_8014EBD0();
        }
        break;
    case 2:
        v = func_800A0070(D_8015EDB8 * 0x800 / 1000) * 16 >> 12;
        if (v >= 16) {
            v = 15;
        }
        func_8013E97C(0x28, D_8015EC28 + v, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 + v, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 - v, D_8015EC2C);
        D_8015EDB8 += 0xA;
        if (D_8015EDB8 >= 0x1F5) {
            D_8015EDB8 += 0xA;
        }
        if (D_8015EDB8 >= 0x3E9) {
            func_8013F0F4(0x50B, 1, 0);
            func_8014EBD0();
        }
        break;
    case 3:
        v = func_800A0070(D_8015EDB8 * 0x800 / 1000) * 32 >> 12;
        if (v >= 32) {
            v = 31;
        }
        func_8013E97C(0x28, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 + v, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 + v, D_8015EC2C);
        D_8015EDB8 += 0xA;
        if (D_8015EDB8 >= 0x1F5) {
            D_8015EDB8 += 0xA;
        }
        if (D_8015EDB8 >= 0x3E9) {
            func_8014EBA8();
        }
        break;
    }
}
