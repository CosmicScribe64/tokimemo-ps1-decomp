void func_80143F50(void) {
    u8 temp_a0;

    temp_a0 = D_8014A13C->unk_00;
    if ((temp_a0 >= 2U) && (temp_a0 < 6U)) {
        if (D_800E6280.unk_F88 & 5) {
            D_8014A13C->unk_20 = 1;
        }
        if ((D_800E6280.unk_F88 & 0xA) && (D_8014A13C->unk_1C != 0)) {
            D_8014A13C->unk_1C = 0;
            D_8014A13C->unk_1A = 0;
            D_8014A13C->unk_02 = 0;
            D_8014A13C->unk_0C = 0;
            if ((u8) D_8014A110 != 0) {
                if (D_8014A13C->unk_20 != 0) {
                    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 0x11;
                } else {
                    func_80044750(0x1805);
                    func_80044750(0x1804);
                    func_80044750(0x806);
                    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 0x10;
                }
            } else if (D_8014A13C->unk_20 != 0) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 8;
            } else {
                func_80044750(0x1802);
                func_80044750(0x1801);
                func_80044750(0x803);
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 7;
            }
            D_8014A13C->unk_20 = 0;
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6803)) = 0;
            func_80044750(0x50A);
        }
    }
    if ((temp_a0 >= 6U) && (temp_a0 < 0xAU)) {
        if (D_800E6280.unk_F88 & 0xA) {
            D_8014A13C->unk_20 = 1;
        }
        if ((D_800E6280.unk_F88 & 5) && (D_8014A13C->unk_1C != 0)) {
            D_8014A13C->unk_1C = 0;
            D_8014A13C->unk_1A = 0;
            D_8014A13C->unk_02 = 0;
            D_8014A13C->unk_0C = 0;
            if ((u8) D_8014A110 != 0) {
                if (D_8014A13C->unk_20 != 0) {
                    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 0x11;
                } else {
                    func_80044750(0x1805);
                    func_80044750(0x1804);
                    func_80044750(0x806);
                    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 0x10;
                }
            } else if (D_8014A13C->unk_20 != 0) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 8;
            } else {
                func_80044750(0x1802);
                func_80044750(0x1801);
                func_80044750(0x803);
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 7;
            }
            D_8014A13C->unk_20 = 0;
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6803)) = 0;
            func_80044750(0x50A);
        }
    }
    if ((D_8014A13C->unk_04 == 0) && ((temp_a0 == 5) || (temp_a0 == 9)) && (D_8014A13C->unk_00 == 1)) {
        if (D_8014A13C->unk_20 != 0) {
            D_8014A13C->unk_20 = 0;
        } else if ((s32) D_8014A13C->unk0 != 0) {
            D_8014A13C->unk_20 = 0;
            D_8014A13C->unk_1C = 0;
            D_8014A13C->unk_1A = 0;
            D_8014A13C->unk_02 = 0;
            D_8014A13C->unk_0C = 0;
            if ((u8) D_8014A110 != 0) {
                func_80044750(0x1805);
                func_80044750(0x1804);
                func_80044750(0x806);
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 0xF;
            } else {
                func_80044750(0x1802);
                func_80044750(0x1801);
                func_80044750(0x803);
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 6;
            }
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6803)) = 0;
            func_80044750(0x50A);
        }
    }
    if (D_8014A13C->unk_1C == 0) {
        if ((u16) D_8014A13C->unk_1A >= 0x1EU) {
            D_8014A13C->unk_20 = 0;
            D_8014A13C->unk_1C = 1;
            D_8014A13C->unk_18 = 0;
            D_8014A13C->unk_00 = 0;
            if ((u8) D_8014A110 != 0) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 0xE;
            } else {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 5;
            }
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6803)) = 0xA4;
            if ((u8) D_8014A110 == 0) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6824)) = 0x2D;
                func_80044750(0x800);
            } else {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6824)) = 0x39;
            }
        }
    }
}
