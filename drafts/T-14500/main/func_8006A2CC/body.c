void func_8006A2CC(void) {
    s32 pad1;
    s32 var_v1;
    s32 var_v1_2;
    s32 pad2;
    s32 sp3C;

    var_v1 = D_800E6280.unk_03F;
    func_80048E78();
    func_80048F64(0x60);
    D_8011ECD0[96].unk_01 = 5;
    D_8011ECD0[96].unk_02 = 0x40;
    D_8011ECD0[96].unk_38 = 0x40000000;
    D_8011ECD0[96].unk_03 = 0x84;
    D_8011ECD0[96].unk_0C = sprite_frame_table;
    D_8011ECD0[96].unk_10 = D_800C9A14;
    D_8011ECD0[96].unk_34 = D_800C9730;
    D_8011ECD0[96].unk_14 = D_800C9A54;
    D_8011ECD0[96].unk_04 = 8;
    D_8011ECD0[96].unk_05 = 1;
    D_8011ECD0[96].unk_18 = 0x76;
    D_8011ECD0[96].unk_26 = 0;
    D_8011ECD0[96].unk_2A = -0x28;
    for (var_v1 = 0; var_v1 < 0x20; var_v1++) {
        func_80048F64(var_v1 + 0x61);
        D_8011ECD0[var_v1 + 0x61].unk_01 = 4;
        D_8011ECD0[var_v1 + 0x61].unk_02 = 0x40;
        D_8011ECD0[var_v1 + 0x61].unk_38 = 0;
        D_8011ECD0[var_v1 + 0x61].unk_03 = 0x84;
        D_8011ECD0[var_v1 + 0x61].unk_0C = sprite_frame_table;
        D_8011ECD0[var_v1 + 0x61].unk_10 = D_800C9A14;
        D_8011ECD0[var_v1 + 0x61].unk_34 = D_800C9730;
        D_8011ECD0[var_v1 + 0x61].unk_14 = D_800C9A54;
        D_8011ECD0[var_v1 + 0x61].unk_04 = 8;
        D_8011ECD0[var_v1 + 0x61].unk_05 = 1;
        D_8011ECD0[var_v1 + 0x61].unk_06 = 8;
        D_8011ECD0[var_v1 + 0x61].unk_18 = 0;
        D_8011ECD0[var_v1 + 0x61].unk_26 = 0x100;
        D_8011ECD0[var_v1 + 0x61].unk_2A = 0x100;
        D_8011ECD0[var_v1 + 0x61].unk_07 = 0x80;
    }
    var_v1_2 = 0x61;
    switch (D_800E6280.unk_03F) {
    case 2:
    case 3:
    case 4:
        if (D_800E6280.unk_03E == 0x5F) {
            D_8011ECD0[0x61].unk_18 = 0x79;
            D_8011ECD0[0x61].unk_26 = 0;
            sp3C = 0;
            D_8011ECD0[0x61].unk_2A = -0x28;
            var_v1_2++;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E0C, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E25, 0);
        }
        if (D_800E6280.unk_03E == 0x60) {
            sp3C = 4;
            if (D_800E6280.unk_03F != 2) {
                D_800E6280.unk_120[13] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x7A;
            D_8011ECD0[var_v1_2].unk_26 = 0;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
            D_8011ECD0[var_v1_2].unk_18 = 0x81;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            sp3C = 8;
            if (D_800E6280.unk_03F != 2) {
                D_800E6280.unk_120[11] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x7A;
            D_8011ECD0[var_v1_2].unk_26 = 0;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
            D_8011ECD0[var_v1_2].unk_18 = 0x84;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x62) {
            sp3C = 0xC;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        D_8011ECD0[0x61].unk_18 = 0x7B;
        D_8011ECD0[0x61].unk_26 = 0;
        D_8011ECD0[0x61].unk_2A = -0x28;
        var_v1_2++;
        if (D_800E6280.unk_03E == 0x5F) {
            sp3C = 1;
            if (D_800E6280.unk_03F != 5) {
                D_800E6280.unk_120[1] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x7E;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x60) {
            sp3C = 5;
            if (D_800E6280.unk_03F != 5) {
                D_800E6280.unk_120[5] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x82;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            sp3C = 9;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E3E, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E57, 0);
        }
        break;
    case 8:
    case 9:
    case 10:
        D_8011ECD0[0x61].unk_18 = 0x7C;
        D_8011ECD0[0x61].unk_26 = 0;
        D_8011ECD0[0x61].unk_2A = -0x28;
        var_v1_2++;
        if (D_800E6280.unk_03E == 0x5F) {
            sp3C = 2;
            if (D_800E6280.unk_03F != 8) {
                D_800E6280.unk_120[7] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x7F;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x60) {
            sp3C = 6;
            if (D_800E6280.unk_03F != 8) {
                D_800E6280.unk_120[10] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x83;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            sp3C = 0xA;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E3E, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E57, 0);
        }
        break;
    case 11:
    case 12:
        D_8011ECD0[0x61].unk_18 = 0x7D;
        D_8011ECD0[0x61].unk_26 = 0;
        D_8011ECD0[0x61].unk_2A = -0x28;
        var_v1_2++;
        if (D_800E6280.unk_03E == 0x5F) {
            sp3C = 3;
            if (D_800E6280.unk_03F == 0xC) {
                D_800E6280.unk_120[3] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x80;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x60) {
            sp3C = 7;
            if (D_800E6280.unk_03F == 0xC) {
                D_800E6280.unk_120[6] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x85;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            sp3C = 0xB;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    case 1:
        D_8011ECD0[0x61].unk_18 = 0x7D;
        D_8011ECD0[0x61].unk_26 = 0;
        D_8011ECD0[0x61].unk_2A = -0x28;
        var_v1_2++;
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_120[3] = 1;
            sp3C = 3;
            D_8011ECD0[var_v1_2].unk_18 = 0x80;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_120[6] = 1;
            sp3C = 7;
            D_8011ECD0[var_v1_2].unk_18 = 0x85;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x62) {
            sp3C = 0xB;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    }
    if ((D_800E6280.unk_03E != 0x5F) || (D_800E6280.unk_03F >= 6U)) {
        D_8011ECD0[var_v1_2].unk_18 = 0x77;
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    if (D_800E6280.unk_03E == 0x5F) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) (sp3C + 0x97);
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    } else if ((D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_03F < 2U)) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) (sp3C + 0x97);
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    } else {
        D_8011ECD0[var_v1_2].unk_01++;
        D_8011ECD0[var_v1_2].unk_18 = 0x78;
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
        D_8011ECD0[var_v1_2].unk_18 = (s16) (D_800B3750[sp3C][0] + 0x90);
        D_8011ECD0[var_v1_2].unk_26 = 0x110;
        D_8011ECD0[var_v1_2].unk_2A = 0x18;
        var_v1_2++;
        D_8011ECD0[var_v1_2].unk_18 = (s16) (D_800B3750[sp3C][1] + 0x90);
        D_8011ECD0[var_v1_2].unk_26 = 0x110;
        D_8011ECD0[var_v1_2].unk_2A = 0x28;
        var_v1_2++;
        D_8011ECD0[var_v1_2].unk_18 = (s16) (D_800B3750[sp3C][2] + 0x90);
        D_8011ECD0[var_v1_2].unk_26 = 0x110;
        D_8011ECD0[var_v1_2].unk_2A = 0x38;
        var_v1_2++;
        set_kanji_string(0x100, 0x10, 6U, D_800B32CC[(sp3C * 3 + 2) % 12], 0);
        set_kanji_string(0x100, 0x20, 6U, D_800B32CC[(sp3C * 3 + 3) % 12], 0);
        set_kanji_string(0x100, 0x30, 6U, D_800B32CC[(sp3C * 3 + 4) % 12], 0);
    }
    if (D_800B5DFC[sp3C] != 0xFF) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) D_800B5DFC[sp3C];
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    set_kanji_string(0x20, -0x5E, 7U, D_800B5D1C[sp3C], 0);
    if (D_800B5D0C[sp3C] != 0xFF) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) D_800B5D0C[sp3C];
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    set_kanji_string(0x20, -0x3E, 8U, D_800B5C2C[sp3C], 0);
    if (sp3C < 4) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) ((sp3C & 3) + 0x97);
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    if (sp3C >= 4) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) ((sp3C & 3) + 0x93);
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    set_kanji_string(-0x8F, -0xE, 6U, D_800B5EA4[sp3C][0], 0);
    set_kanji_string(-0x8F, 2, 6U, D_800B5EA4[sp3C][1], 0);
    set_kanji_string(-0x8F, 0x12, 6U, D_800B5EA4[sp3C][2], 0);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    gnsx(&D_800E6280.unk_0D4[0]);
    sndisp(D_800B66A4[sp3C], 0, 0x1F);
}
