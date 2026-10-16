u8 func_8014790C(void) {
    s32 sp7C[1];
    s32 *var_s0;
    s16 *var_s2;
    s16 *var_s3;
    s16 temp_a1;
    s16 temp_v1;
    s32 *var_s4;
    s32 temp_v0;
    s32 var_s1;
    u8 var_v0;
    u8 *var_t1;

    var_v0 = *(u8 *)((unsigned char *)(&D_800E6280) + (4365));
    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) {
    case 0:
        func_8004E58C();
        func_8006612C("結果発\x95\\ ");
        gnsx(&D_800E6280 + 0xD4);
        sndisp("今日は、試験結果の 発\x95\\の日だ。）", 0, 0x1F);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        func_8004500C(1, 0x202);
        hizuke_init();
        func_80048E78();
        *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        var_v0 = cd_read_sectors(0xD, 0x801E0000, 0x9CA8);
        break;
    case 1:
        if ((u32) D_800E6280.unk0 >= 0x101U) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 2:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            func_8004E58C();
            icon_disp_switch(0);
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            func_80048F64(0x60);
            *(s8 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6529)) = 4;
            *(s8 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6530)) = 0x40;
            *(s32 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6584)) = 0;
            *(s8 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6531)) = 0xA4;
            *(s32 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6540)) = 0x801E64A0;
            *(s32 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6544)) = 0x801E6520;
            *(s32 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6580)) = 0x801E6494;
            *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6552)) = 0;
            *(s8 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6533)) = 1;
            *(s8 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6532)) = 8;
            *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6566)) = 0;
            *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6570)) = -0x28;
            *(s8 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (6535)) = 0x80;
            func_80148E98();
            func_80148EE8();
            func_801480D0();
            var_s4 = &D_80150668 + 0x1C;
            var_s3 = &D_8015064C;
            var_s2 = &D_80150630;
            var_s0 = &((DisplayActor44 *)D_8011ECD0);
            var_s1 = 0;
            do {
                temp_v1 = *var_s2;
                temp_a1 = *var_s3;
                temp_v0 = *var_s4;
                *(s8 *)((unsigned char *)var_s0 + (6597)) = 3;
                *(s8 *)((unsigned char *)var_s0 + (6598)) = 0x40;
                *(s32 *)((unsigned char *)var_s0 + (6652)) = 0;
                *(s8 *)((unsigned char *)var_s0 + (6599)) = 0x24;
                *(s32 *)((unsigned char *)var_s0 + (6608)) = 0x801E64A0;
                *(s32 *)((unsigned char *)var_s0 + (6612)) = 0x801E6520;
                *(s32 *)((unsigned char *)var_s0 + (6648)) = 0x801E6494;
                *(s16 *)((unsigned char *)var_s0 + (6620)) = 0x1F;
                *(s8 *)((unsigned char *)var_s0 + (6601)) = 1;
                *(s8 *)((unsigned char *)var_s0 + (6600)) = 8;
                *(s8 *)((unsigned char *)var_s0 + (6603)) = 0x80;
                *(s16 *)((unsigned char *)var_s0 + (6634)) = temp_v1;
                *(s16 *)((unsigned char *)var_s0 + (6638)) = temp_a1;
                if (temp_v0 == 0xD) {
                    func_800AE090(&sp7C, &D_800E6280 + 0xD4, 8);
                    func_800AE110(&sp7C, &D_800E6280 + 0xDC, 8);
                    set_kanji_string((s16) (*var_s2 + 0x18), *var_s3, 3, &sp7C, 0);
                    func_800AE0B0("%s\n", &D_800E6280 + 0xC4);
                } else if (temp_v0 == 0xA) {
                    set_kanji_string((s16) (temp_v1 + 0x18), temp_a1, 3, D_800B356C, 0);
                } else if (temp_v0 == 0xB) {
                    if (((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (1072)) << 0x14) >> 0x1D) != 0) {
                        set_kanji_string((s16) (temp_v1 + 0x18), temp_a1, 3, D_800B3564, 0);
                    } else {
                        var_t1 = &((DisplayActor44 *)D_8011ECD0) + ((var_s1 + 0x61) * 0x44);
                        goto block_20;
                    }
                } else if (*(&D_800E6280 + (temp_v0 * 0x38)) & 2) {
                    set_kanji_string((s16) (temp_v1 + 0x18), temp_a1, 3, *(&D_800B3538 + (temp_v0 * 4)), 0);
                } else {
                    var_t1 = &((DisplayActor44 *)D_8011ECD0) + ((var_s1 + 0x61) * 0x44);
block_20:
                    *(s8 *)((unsigned char *)var_t1 + (3)) = 0xA4;
                }
                var_s1 += 1;
                var_s0 += 17;
                var_s2 += 1;
                var_s3 += 1;
                var_s4 += 1;
            } while (var_s1 != 0xE);
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            func_80148F34();
            var_v0 = func_8004284C();
        }
        break;
    }
    return var_v0;
}
