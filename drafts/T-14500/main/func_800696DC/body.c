void func_800696DC(s32 arg0, s32 arg1) {
    s32 i;
    s32 var_s0;
    s32 spEC;
    u8 spC8[32];
    u8 spA8[32];
    u8 sp88[32];
    u8 sp68[32];
    u8 sp48[32];

    if (arg1 != 0) {
        spEC = -0x8C;
    } else {
        spEC = -8;
    }
    k_sub_reset();
    var_s0 = 0;
    if ((u8) D_800E8BEE != 0) {
        var_s0 = 0;
        for (i = 0; i < 13; i++) {
            if (((NameMetricWork *)&D_800E7D10)->flags[i] != 0) {
                var_s0 += 1;
            }
        }
        strcpy(spC8, D_800B3288[D_800E7D36]);
        strncat((s32) spC8, (s32) "番アルバム", 2);
        set_kanji_string((s16) (spEC + 0xC), -0x50, 0xFU, spC8, 0);
        if (D_800E7D36 == 0) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "システム", 0);
        } else {
            if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E7D36] == 0) {
                set_kanji_string((s16) spEC, -0x40, 0xFU, "未使用", 0);
            } else if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E7D36] == 0xFF) {
                set_kanji_string((s16) spEC, -0x40, 0xFU, "データ異常", 0);
            } else {
                strcpy(spC8, D_800B3288[(u8) ((NameMetricWork *)&D_800E7D10)->year[D_800E7D36] / 10U]);
                strncat((s32) spC8, (s32) D_800B3288[(u8) ((NameMetricWork *)&D_800E7D10)->year[D_800E7D36] % 10U], 2);
                strncat((s32) spC8, (s32) "年", 2);
                strncat((s32) spC8, (s32) D_800B3288[((NameMetricWork *)&D_800E7D10)->month[D_800E7D36]], 4);
                strncat((s32) spC8, (s32) "月", 2);
                if (((NameMetricWork *)&D_800E7D10)->day[D_800E7D36] >= 0xAU) {
                    strncat((s32) spC8, (s32) D_800B3288[((NameMetricWork *)&D_800E7D10)->day[D_800E7D36] / 10U], 2);
                }
                strncat((s32) spC8, (s32) D_800B3288[((NameMetricWork *)&D_800E7D10)->day[D_800E7D36] % 10U], 2);
                strncat((s32) spC8, (s32) "日", 2);
                set_kanji_string((s16) spEC, -0x40, 0xFU, spC8, 0);
                strcpy(spA8, "名前…");
                strncat((s32) spA8, (s32) ((NameMetricWork *)&D_800E7D10)->family_name[D_800E7D36], 6);
                strncat((s32) spA8, (s32) ((NameMetricWork *)&D_800E7D10)->given_name[D_800E7D36], 6);
                strcpy(sp88, "あだ名…");
                strncat((s32) sp88, (s32) ((NameMetricWork *)&D_800E7D10)->nickname[D_800E7D36], 0xD);
                set_kanji_string((s16) spEC, -0x30, 0xFU, spA8, 0);
                set_kanji_string((s16) spEC, -0x20, 0xFU, sp88, 0);
                strcpy(sp68, "所属部‥");
                strncat((s32) sp68, club_name_table[((NameMetricWork *)&D_800E7D10)->club[D_800E7D36]]);
                set_kanji_string((s16) spEC, -0x10, 0xFU, sp68, 0);
                strcpy(sp48, "クリア人数‥");
                if (var_s0 >= 0x64) {
                    var_s0 = 0x63;
                }
                if (var_s0 >= 0xA) {
                    strncat((s32) sp48, (s32) D_800B3288[var_s0 / 10], 2);
                }
                strncat((s32) sp48, (s32) D_800B3288[var_s0 % 10], 2);
                set_kanji_string((s16) spEC, 0, 0xFU, sp48, 0);
            }
        }
    } else {
        for (i = 0; i < 13; i++) {
            if (((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].flags[i] != 0) {
                var_s0 += 1;
            }
        }
        if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E6280.unk_1118] == 0) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "未使用", 0);
        } else if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E6280.unk_1118] == 0xFF) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "データ異常", 0);
        } else {
            strcpy(spC8, D_800B3288[D_800E6280.unk_1118]);
            strncat((s32) spC8, (s32) "番アルバム", 2);
            set_kanji_string((s16) (spEC + 0xC), -0x50, 0xFU, spC8, 0);
            strcpy(spC8, D_800B3288[(u8) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].year / 10U]);
            strncat((s32) spC8, (s32) D_800B3288[(u8) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].year % 10U], 2);
            strncat((s32) spC8, (s32) "年", 2);
            strncat((s32) spC8, (s32) D_800B3288[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].month], 4);
            strncat((s32) spC8, (s32) "月", 2);
            if (((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].day >= 0xAU) {
                strncat((s32) spC8, (s32) D_800B3288[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].day / 10U], 2);
            }
            strncat((s32) spC8, (s32) D_800B3288[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].day % 10U], 2);
            strncat((s32) spC8, (s32) "日", 2);
            set_kanji_string((s16) spEC, -0x40, 0xFU, spC8, 0);
            strcpy(spA8, "名前…");
            strncat((s32) spA8, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].family_name, 6);
            strncat((s32) spA8, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].given_name, 6);
            strcpy(sp88, "あだ名…");
            strncat((s32) sp88, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].nickname, 0xD);
            set_kanji_string((s16) spEC, -0x30, 0xFU, spA8, 0);
            set_kanji_string((s16) spEC, -0x20, 0xFU, sp88, 0);
            strcpy(sp68, "所属部‥");
            strncat((s32) sp68, club_name_table[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].club >> 12]);
            set_kanji_string((s16) spEC, -0x10, 0xFU, sp68, 0);
            strcpy(sp48, "クリア人数‥");
            if (var_s0 >= 0x64) {
                var_s0 = 0x63;
            }
            if (var_s0 >= 0xA) {
                strncat((s32) sp48, (s32) D_800B3288[var_s0 / 10], 2);
            }
            strncat((s32) sp48, (s32) D_800B3288[var_s0 % 10], 2);
            set_kanji_string((s16) spEC, 0, 0xFU, sp48, 0);
        }
    }
    k_sub_disp_start(6);
}
