void func_800696DC(s32 arg0, s32 arg1) {
    s32 spEC;
    u8 spC8;
    u8 spA8;
    u8 sp88;
    u8 sp68;
    u8 sp48;
    s32 sp40;
    s32 *var_v0;
    s32 var_at;
    s32 var_at_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_v1;
    u8 temp_t9;
    u8 temp_v0_2;
    u8 temp_v1;
    u8 var_v0_2;
    u8 var_v0_4;
    u8 *temp_v0;
    u8 *var_v0_3;

    if (arg1 != 0) {
        spEC = -0x8C;
    } else {
        spEC = -8;
    }
    k_sub_reset();
    var_s0 = 0;
    if ((u8) D_800E8BEE != 0) {
        var_s0_2 = 0;
        var_v0 = &D_800E7D10;
        do {
            temp_t9 = *(u8 *)((unsigned char *)var_v0 + (20));
            var_v0 += 1;
            if (temp_t9 != 0) {
                var_s0_2 += 1;
            }
        } while (var_v0 != &D_800E7D1D);
        strcpy(&spC8, D_800B3288[D_800E7D36]);
        strncat(&spC8, "番アルバム", 2);
        set_kanji_string((s16) (spEC + 0xC), -0x50, 0xFU, &spC8, 0);
        if (D_800E7D36 == 0) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "システム", 0);
        } else {
            temp_v0 = D_800E7D36 + &D_800E7D10;
            temp_v1 = *(u8 *)((unsigned char *)temp_v0 + (4));
            if (temp_v1 == 0) {
                set_kanji_string((s16) spEC, -0x40, 0xFU, "未使用", 0);
            } else if (temp_v1 == 0xFF) {
                set_kanji_string((s16) spEC, -0x40, 0xFU, "データ異常", 0);
            } else {
                strcpy(&spC8, D_800B3288[(u8) *(u8 *)((unsigned char *)temp_v0 + (2136)) / 10U]);
                strncat(&spC8, D_800B3288[(u8) *(&D_800E8568 + D_800E7D36) % 10U], 2);
                strncat(&spC8, "年", 2);
                strncat(&spC8, D_800B3288[*(&D_800E8578 + D_800E7D36)], 4);
                strncat(&spC8, "月", 2);
                var_v0_2 = *(&D_800E8588 + D_800E7D36);
                if (var_v0_2 >= 0xAU) {
                    strncat(&spC8, D_800B3288[var_v0_2 / 10U], 2);
                    var_v0_2 = *(&D_800E8588 + D_800E7D36);
                }
                strncat(&spC8, D_800B3288[var_v0_2 % 10U], 2);
                strncat(&spC8, "日", 2);
                sp40 = (s32) (s16) spEC;
                set_kanji_string((s16) spEC, -0x40, 0xFU, &spC8, 0);
                strcpy(&spA8, "名前…");
                strncat(&spA8, (D_800E7D36 * 8) + 0x888 + &D_800E7D10, 6);
                strncat(&spA8, (D_800E7D36 * 8) + 0x908 + &D_800E7D10, 6);
                strcpy(&sp88, "あだ名…");
                strncat(&sp88, (D_800E7D36 * 0x10) + 0x988 + &D_800E7D10, 0xD);
                set_kanji_string((s16) sp40, -0x30, 0xFU, &spA8, 0);
                set_kanji_string((s16) sp40, -0x20, 0xFU, &sp88, 0);
                strcpy(&sp68, "所属部‥");
                strncat(&sp68, club_name_table[*(&D_800E8BD8 + D_800E7D36)]);
                set_kanji_string((s16) sp40, -0x10, 0xFU, &sp68, 0);
                strcpy(&sp48, "クリア人数‥");
                var_at = var_s0_2 < 0xA;
                if (var_s0_2 >= 0x64) {
                    var_s0_2 = 0x63;
                    var_at = 0x63 < 0xA;
                }
                if (var_at == 0) {
                    strncat(&sp48, D_800B3288[var_s0_2 / 10], 2);
                }
                strncat(&sp48, D_800B3288[var_s0_2 % 10], 2);
                set_kanji_string((s16) sp40, 0, 0xFU, &sp48, 0);
            }
        }
    } else {
        var_v0_3 = (D_800E7397 * 0xF00) + D_80123120;
        var_v1 = 0;
        do {
            var_v1 += 1;
            if (*(u8 *)((unsigned char *)var_v0_3 + (1672)) != 0) {
                var_s0 += 1;
            }
            var_v0_3 += 1;
        } while (var_v1 != 0xD);
        temp_v0_2 = D_800E7D14[D_800E7398];
        if (temp_v0_2 == 0) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "未使用", 0);
        } else if (temp_v0_2 == 0xFF) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "データ異常", 0);
        } else {
            strcpy(&spC8, D_800B3288[D_800E7398]);
            strncat(&spC8, "番アルバム", 2);
            set_kanji_string((s16) (spEC + 0xC), -0x50, 0xFU, &spC8, 0);
            strcpy(&spC8, D_800B3288[(u8) *(&D_80123325 + (D_800E7397 * 0xF00)) / 10U]);
            strncat(&spC8, D_800B3288[(u8) *(&D_80123325 + (D_800E7397 * 0xF00)) % 10U], 2);
            strncat(&spC8, "年", 2);
            strncat(&spC8, D_800B3288[*(&D_80123326 + (D_800E7397 * 0xF00))], 4);
            strncat(&spC8, "月", 2);
            var_v0_4 = *(&D_80123327 + (D_800E7397 * 0xF00));
            if (var_v0_4 >= 0xAU) {
                strncat(&spC8, D_800B3288[var_v0_4 / 10U], 2);
                var_v0_4 = *(&D_80123327 + (D_800E7397 * 0xF00));
            }
            strncat(&spC8, D_800B3288[var_v0_4 % 10U], 2);
            strncat(&spC8, "日", 2);
            sp40 = (s32) (s16) spEC;
            set_kanji_string((s16) spEC, -0x40, 0xFU, &spC8, 0);
            strcpy(&spA8, "名前…");
            strncat(&spA8, (D_800E7397 * 0xF00) + 0x29C + D_80123120, 6);
            strncat(&spA8, (D_800E7397 * 0xF00) + 0x2A4 + D_80123120, 6);
            strcpy(&sp88, "あだ名…");
            strncat(&sp88, (D_800E7397 * 0xF00) + 0x2AC + D_80123120, 0xD);
            set_kanji_string((s16) sp40, -0x30, 0xFU, &spA8, 0);
            set_kanji_string((s16) sp40, -0x20, 0xFU, &sp88, 0);
            strcpy(&sp68, "所属部‥");
            strncat(&sp68, club_name_table[(u16) *(&D_801233DC + (D_800E7397 * 0xF00)) >> 0xC]);
            set_kanji_string((s16) sp40, -0x10, 0xFU, &sp68, 0);
            strcpy(&sp48, "クリア人数‥");
            var_at_2 = var_s0 < 0xA;
            if (var_s0 >= 0x64) {
                var_s0 = 0x63;
                var_at_2 = 0x63 < 0xA;
            }
            if (var_at_2 == 0) {
                strncat(&sp48, D_800B3288[var_s0 / 10], 2);
            }
            strncat(&sp48, D_800B3288[var_s0 % 10], 2);
            set_kanji_string((s16) sp40, 0, 0xFU, &sp48, 0);
        }
    }
    k_sub_disp_start(6);
}
