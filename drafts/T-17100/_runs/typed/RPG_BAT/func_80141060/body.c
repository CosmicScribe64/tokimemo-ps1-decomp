void func_80141060(void) {
    DisplayActor44 *sp3C;
    DisplayActor44 *temp_v0;
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_3;
    DisplayActor44 *var_v0_4;
    DisplayActor44 *var_v0_5;
    DisplayActor44 *var_v0_6;
    DisplayActor44 *var_v0_7;
    DisplayActor44 *var_v0_8;
    DisplayActor44 *var_v0_9;
    s32 var_a0;
    s32 var_at;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;

    var_v0 = ((DisplayActor44 *)D_8011ECD0);
    var_s1 = 0;
    do {
        sp3C = var_v0;
        func_80048F64(var_s1 + 0x40);
        if (var_s1 < 0x3F) {
            var_v0[64].unk_01 = 3;
        } else if (var_s1 < 0x40) {
            var_v0[64].unk_01 = 9;
        } else if (var_s1 < 0x48) {
            var_v0[64].unk_01 = 4;
        } else {
            var_v0[64].unk_01 = 5;
        }
        var_s1 += 1;
        var_v0[64].unk_38 = 0x41000000;
        var_v0 += 1;
        var_v0[63].unk_03 = 0;
        var_v0[63].unk_02 = 1;
        var_v0[63].unk_06 = 1;
        var_v0[63].unk_07 = 0;
        var_v0[63].unk_26 = -0xA0;
        var_v0[63].unk_2A = -0x78;
        var_v0[63].unk_05 = 0x80;
        var_v0[63].unk_43 = 0;
        var_v0[63].unk_04 = 8;
        *(s32 *)&var_v0[63].unk_34 = (s32) D_8015EE8C;
        *(s32 *)&var_v0[63].unk_10 = (s32) D_8015EEBC;
        *(s32 *)&var_v0[63].unk_0C = (s32) D_8015EEA4;
    } while (var_s1 < 0x60);
    var_v0_2 = ((DisplayActor44 *)D_8011ECD0);
    var_s1_2 = 0;
    do {
        var_s1_2 += 1;
        var_v0_2 += 1;
        *(s32 *)&var_v0_2[63].unk_0C = (s32) D_8015EEA4;
        *(s32 *)&var_v0_2[63].unk_10 = (s32) D_8015EEBC;
        *(s32 *)&var_v0_2[63].unk_34 = (s32) D_8015EE8C;
    } while (var_s1_2 < 0x48);
    load_tpage_buf_lock(0x15, (*(s32 *)D_8015EE8C), 1);
    load_tpage_buf_lock(0x16, *(s32 *)((unsigned char *)D_8015EE8C + (4)), 1);
    load_tpage_buf_lock(0x17, *(s32 *)((unsigned char *)D_8015EE8C + (8)), 1);
    load_tpage_buf_lock(0x18, *(s32 *)((unsigned char *)D_8015EE8C + (12)), 1);
    load_tpage_buf_lock(0x19, *(s32 *)((unsigned char *)D_8015EE8C + (16)), 1);
    load_tpage_buf_lock(0x35, (*(s32 *)D_8015EE98), 1);
    load_tpage_buf_lock(0x39, *(s32 *)((unsigned char *)D_8015EE98 + (4)), 1);
    switch (D_8015EB98) {
    case 0x10000:
        load_tpage_buf_lock(0xB, *(s32 *)((unsigned char *)D_8015EE94 + (4)), 1);
        load_tpage_buf_lock(0xC, *(s32 *)((unsigned char *)D_8015EE94 + (8)), 1);
        /* fallthrough */
    case 0x20000:
    case 0x40000:
        load_tpage_buf_lock(0xD, (*(s32 *)D_8015EE94), 1);
        load_tpage_buf_lock(0x2B, (*(s32 *)D_8015EE90), 1);
        load_tpage_buf_lock(0x2C, *(s32 *)((unsigned char *)D_8015EE90 + (4)), 1);
        load_tpage_buf_lock(0x2D, *(s32 *)((unsigned char *)D_8015EE90 + (8)), 1);
        if (D_8015EB9C == 0xD) {
            load_tpage_buf_lock(5, (*(s32 *)D_8015EEA0), 1);
            load_tpage_buf_lock(6, *(s32 *)((unsigned char *)D_8015EEA0 + (4)), 1);
            load_tpage_buf_lock(7, *(s32 *)((unsigned char *)D_8015EEA0 + (8)), 1);
        }
        break;
    case 0x80000000:
        load_tpage_buf_lock(0xB, (*(s32 *)D_8015EEA0), 1);
        load_tpage_buf_lock(0xC, *(s32 *)((unsigned char *)D_8015EEA0 + (4)), 1);
        load_tpage_buf_lock(0xD, *(s32 *)((unsigned char *)D_8015EEA0 + (8)), 1);
        load_tpage_buf_lock(0xE, *(s32 *)((unsigned char *)D_8015EEA0 + (12)), 1);
        load_tpage_buf_lock(0xF, *(s32 *)((unsigned char *)D_8015EEA0 + (16)), 1);
        load_tpage_buf_lock(0x2B, *(s32 *)((unsigned char *)D_8015EEA0 + (20)), 1);
        load_tpage_buf_lock(0x2D, (*(s32 *)D_8015EE90), 1);
        load_tpage_buf_lock(0x2E, *(s32 *)((unsigned char *)D_8015EE90 + (4)), 1);
        load_tpage_buf_lock(0x2F, *(s32 *)((unsigned char *)D_8015EE90 + (8)), 1);
        break;
    case 0x100000:
        load_tpage_buf_lock(0xB, *(s32 *)((unsigned char *)D_8015EE90 + (24)), 1);
        switch (D_8015EB9C) {
        case 0:
            var_s1_2 = 1;
        default:
block_37:
            var_s0 = 0;
            if (var_s1_2 > 0) {
                var_v0_3 = NULL;
                do {
                    sp3C = var_v0_3;
                    load_tpage_buf_lock(var_s0 + 0x25, *(D_8015EEA0 + var_v0_3), 1);
                    var_s0 += 1;
                    var_v0_3 = (DisplayActor44 *) &var_v0_3->unk_04;
                } while (var_s0 != var_s1_2);
            }
            break;
        case 1:
            var_s1_2 = 2;
            goto block_37;
        case 2:
            var_s1_2 = 4;
            goto block_37;
        case 3:
        case 13:
block_36:
            var_s1_2 = 3;
            goto block_37;
        case 4:
            var_s1_2 = 2;
            goto block_37;
        case 5:
            var_s1_2 = 2;
            goto block_37;
        case 6:
            goto block_36;
        case 7:
            var_s1_2 = 2;
            goto block_37;
        case 8:
            var_s1_2 = 2;
            goto block_37;
        case 9:
            var_s1_2 = 1;
            goto block_37;
        case 10:
            var_s1_2 = 2;
            goto block_37;
        }
        /* fallthrough */
    case 0x8000000:
        if (D_8015EBA4 == 1) {
            var_s0_2 = 0;
            switch (D_8015EBA0) {
            case 0:
                var_s1_2 = 4;
                break;
            case 1:
                var_s1_2 = 4;
                break;
            case 2:
                var_s1_2 = 4;
                break;
            case 3:
                var_s1_2 = 5;
                break;
            case 4:
                var_s1_2 = 5;
                break;
            case 5:
                var_s1_2 = 4;
                break;
            case 6:
                var_s1_2 = 3;
                break;
            case 7:
                var_s1_2 = 3;
                break;
            case 8:
                var_s1_2 = 5;
                break;
            case 9:
                var_s1_2 = 5;
                break;
            case 10:
                var_s1_2 = 3;
                break;
            case 11:
                var_s1_2 = 6;
                break;
            }
            var_at = var_s1_2 > 0;
            do {
                if (var_at != 0) {
                    load_tpage_buf_lock(var_s0_2 + 5, *(s32 *)(D_8015EE9C + (var_s0_2 * 4)), 1);
                } else {
                    func_80041168(var_s0_2 + 5);
                }
                var_s0_2 += 1;
                var_at = var_s0_2 < var_s1_2;
            } while (var_s0_2 != 6);
        }
        load_tpage_buf_lock(0x2C, (*(s32 *)D_8015EE90), 1);
        load_tpage_buf_lock(0xC, *(s32 *)((unsigned char *)D_8015EE90 + (4)), 1);
        load_tpage_buf_lock(0xD, *(s32 *)((unsigned char *)D_8015EE90 + (8)), 1);
        load_tpage_buf_lock(0xE, *(s32 *)((unsigned char *)D_8015EE90 + (12)), 1);
        load_tpage_buf_lock(0xF, *(s32 *)((unsigned char *)D_8015EE90 + (16)), 1);
        load_tpage_buf_lock(0x2B, *(s32 *)((unsigned char *)D_8015EE90 + (20)), 1);
        break;
    }
    var_s1_3 = 0;
    var_a0 = 0;
    do {
        func_8013E8B8(var_a0, 0x27, 0);
        var_s1_3 += 1;
        var_a0 = var_s1_3;
    } while (var_s1_3 < 0x30);
    if (D_8015EB9C != 0xE) {
        var_v0_4 = &((DisplayActor44 *)D_8011ECD0)[29];
        do {
            var_v0_4 += 1;
            *(s32 *)&var_v0_4[95].unk_0C = (s32) D_8015EEB8;
            *(s32 *)&var_v0_4[95].unk_10 = (s32) D_8015EED0;
            *(s32 *)&var_v0_4[95].unk_34 = (s32) D_8015EEA0;
            var_v0_4[95].unk_43 = 0x1C;
            var_v0_4[95].unk_01 = 5;
        } while (var_v0_4 != (&((DisplayActor44 *)D_8011ECD0)[31]));
    }
    if (D_8015EBA4 == 1) {
        var_v0_5 = &((DisplayActor44 *)D_8011ECD0)[40];
        do {
            var_v0_5 += 4;
            *(s32 *)&var_v0_5[93].unk_0C = (s32) D_8015EEB4;
            *(s32 *)&var_v0_5[93].unk_10 = (s32) D_8015EECC;
            *(s32 *)&var_v0_5[93].unk_34 = (s32) D_8015EE9C;
            var_v0_5[93].unk_43 = 0x1D;
            *(s32 *)&var_v0_5[94].unk_0C = (s32) D_8015EEB4;
            *(s32 *)&var_v0_5[94].unk_10 = (s32) D_8015EECC;
            *(s32 *)&var_v0_5[94].unk_34 = (s32) D_8015EE9C;
            var_v0_5[94].unk_43 = 0x1D;
            *(s32 *)&var_v0_5[95].unk_0C = (s32) D_8015EEB4;
            *(s32 *)&var_v0_5[95].unk_10 = (s32) D_8015EECC;
            *(s32 *)&var_v0_5[95].unk_34 = (s32) D_8015EE9C;
            var_v0_5[95].unk_43 = 0x1D;
            *(s32 *)&var_v0_5[92].unk_0C = (s32) D_8015EEB4;
            *(s32 *)&var_v0_5[92].unk_10 = (s32) D_8015EECC;
            *(s32 *)&var_v0_5[92].unk_34 = (s32) D_8015EE9C;
            var_v0_5[92].unk_43 = 0x1D;
        } while (var_v0_5 != (&((DisplayActor44 *)D_8011ECD0)[48]));
    }
    if (D_8015EB98 & 0xF0000) {
        var_v0_6 = &((DisplayActor44 *)D_8011ECD0)[40];
        do {
            var_v0_6 += 4;
            *(s32 *)&var_v0_6[93].unk_0C = (s32) D_8015EEAC;
            *(s32 *)&var_v0_6[93].unk_10 = (s32) D_8015EEC4;
            *(s32 *)&var_v0_6[93].unk_34 = (s32) D_8015EE94;
            var_v0_6[93].unk_43 = 0x18;
            *(s32 *)&var_v0_6[94].unk_0C = (s32) D_8015EEAC;
            *(s32 *)&var_v0_6[94].unk_10 = (s32) D_8015EEC4;
            *(s32 *)&var_v0_6[94].unk_34 = (s32) D_8015EE94;
            var_v0_6[94].unk_43 = 0x18;
            *(s32 *)&var_v0_6[95].unk_0C = (s32) D_8015EEAC;
            *(s32 *)&var_v0_6[95].unk_10 = (s32) D_8015EEC4;
            *(s32 *)&var_v0_6[95].unk_34 = (s32) D_8015EE94;
            var_v0_6[95].unk_43 = 0x18;
            *(s32 *)&var_v0_6[92].unk_0C = (s32) D_8015EEAC;
            *(s32 *)&var_v0_6[92].unk_10 = (s32) D_8015EEC4;
            *(s32 *)&var_v0_6[92].unk_34 = (s32) D_8015EE94;
            var_v0_6[92].unk_43 = 0x18;
        } while (var_v0_6 != (&((DisplayActor44 *)D_8011ECD0)[48]));
    }
    if (D_8015EB98 & 0x80000000) {
        var_v0_7 = &((DisplayActor44 *)D_8011ECD0)[40];
        do {
            var_v0_7 += 4;
            *(s32 *)&var_v0_7[93].unk_0C = (s32) D_8015EEA8;
            *(s32 *)&var_v0_7[93].unk_10 = (s32) D_8015EEC0;
            *(s32 *)&var_v0_7[93].unk_34 = (s32) D_8015EE90;
            var_v0_7[93].unk_43 = 0x12;
            *(s32 *)&var_v0_7[94].unk_0C = (s32) D_8015EEA8;
            *(s32 *)&var_v0_7[94].unk_10 = (s32) D_8015EEC0;
            *(s32 *)&var_v0_7[94].unk_34 = (s32) D_8015EE90;
            var_v0_7[94].unk_43 = 0x12;
            *(s32 *)&var_v0_7[95].unk_0C = (s32) D_8015EEA8;
            *(s32 *)&var_v0_7[95].unk_10 = (s32) D_8015EEC0;
            *(s32 *)&var_v0_7[95].unk_34 = (s32) D_8015EE90;
            var_v0_7[95].unk_43 = 0x12;
            *(s32 *)&var_v0_7[92].unk_0C = (s32) D_8015EEA8;
            *(s32 *)&var_v0_7[92].unk_10 = (s32) D_8015EEC0;
            *(s32 *)&var_v0_7[92].unk_34 = (s32) D_8015EE90;
            var_v0_7[92].unk_43 = 0x12;
        } while (var_v0_7 != (&((DisplayActor44 *)D_8011ECD0)[48]));
    }
    var_v0_8 = &((DisplayActor44 *)D_8011ECD0)[48];
    do {
        var_v0_8 += 1;
        *(s32 *)&var_v0_8[95].unk_0C = (s32) D_8015EEB0;
        *(s32 *)&var_v0_8[95].unk_10 = (s32) D_8015EEC8;
        *(s32 *)&var_v0_8[95].unk_34 = (s32) D_8015EE98;
        var_v0_8[95].unk_43 = 0x1E;
    } while ((u32) var_v0_8 < (u32) ((DisplayActor44 *)D_8011ECD0));
    if (D_8015EBA4 == 1) {
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[146].unk_0C = (s32) D_8015EEB4;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[146].unk_10 = (s32) D_8015EECC;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[146].unk_34 = (s32) D_8015EE9C;
        ((DisplayActor44 *)D_8011ECD0)[146].unk_43 = 0x1D;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[147].unk_0C = (s32) D_8015EEB4;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[147].unk_10 = (s32) D_8015EECC;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[147].unk_34 = (s32) D_8015EE9C;
        ((DisplayActor44 *)D_8011ECD0)[147].unk_43 = 0x1D;
        temp_v0 = &((DisplayActor44 *)D_8011ECD0)[0x34];
        *(s32 *)&temp_v0[97].unk_0C = (s32) D_8015EEB4;
        *(s32 *)&temp_v0[97].unk_10 = (s32) D_8015EECC;
        *(s32 *)&temp_v0[97].unk_34 = (s32) D_8015EE9C;
        temp_v0[97].unk_43 = 0x1D;
        *(s32 *)&temp_v0[98].unk_0C = (s32) D_8015EEB4;
        *(s32 *)&temp_v0[98].unk_10 = (s32) D_8015EECC;
        *(s32 *)&temp_v0[98].unk_34 = (s32) D_8015EE9C;
        temp_v0[98].unk_43 = 0x1D;
        *(s32 *)&temp_v0[99].unk_0C = (s32) D_8015EEB4;
        *(s32 *)&temp_v0[99].unk_10 = (s32) D_8015EECC;
        *(s32 *)&temp_v0[99].unk_34 = (s32) D_8015EE9C;
        temp_v0[99].unk_43 = 0x1D;
        *(s32 *)&temp_v0[96].unk_0C = (s32) D_8015EEB4;
        *(s32 *)&temp_v0[96].unk_10 = (s32) D_8015EECC;
        *(s32 *)&temp_v0[96].unk_34 = (s32) D_8015EE9C;
        temp_v0[96].unk_43 = 0x1D;
    }
    var_v0_9 = &((DisplayActor44 *)D_8011ECD0)[56];
    do {
        *(s32 *)&var_v0_9[96].unk_0C = (s32) D_8015EEA8;
        *(s32 *)&var_v0_9[96].unk_10 = (s32) D_8015EEC0;
        *(s32 *)&var_v0_9[96].unk_34 = (s32) D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            var_v0_9[96].unk_43 = 0x13;
        } else {
            var_v0_9[96].unk_43 = 0x12;
        }
        *(s32 *)&var_v0_9[97].unk_0C = (s32) D_8015EEA8;
        *(s32 *)&var_v0_9[97].unk_10 = (s32) D_8015EEC0;
        *(s32 *)&var_v0_9[97].unk_34 = (s32) D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            var_v0_9[97].unk_43 = 0x13;
        } else {
            var_v0_9[97].unk_43 = 0x12;
        }
        *(s32 *)&var_v0_9[98].unk_0C = (s32) D_8015EEA8;
        *(s32 *)&var_v0_9[98].unk_10 = (s32) D_8015EEC0;
        *(s32 *)&var_v0_9[98].unk_34 = (s32) D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            var_v0_9[98].unk_43 = 0x13;
        } else {
            var_v0_9[98].unk_43 = 0x12;
        }
        *(s32 *)&var_v0_9[99].unk_0C = (s32) D_8015EEA8;
        *(s32 *)&var_v0_9[99].unk_10 = (s32) D_8015EEC0;
        *(s32 *)&var_v0_9[99].unk_34 = (s32) D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            var_v0_9[99].unk_43 = 0x13;
        } else {
            var_v0_9[99].unk_43 = 0x12;
        }
        var_v0_9 += 4;
    } while (var_v0_9 != (&((DisplayActor44 *)D_8011ECD0)[64]));
}
