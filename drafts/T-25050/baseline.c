s32 func_80075C84(void) {
    s32 i;
    s32 j;
    u32 k;

    if ((((DisplayActor44 *)D_8011ECD0)->unk_26 < -0x40) && (((DisplayActor44 *)D_8011ECD0)->unk_26 >= -0x4F)
        && (((DisplayActor44 *)D_8011ECD0)->unk_2A < 0x18) && (((DisplayActor44 *)D_8011ECD0)->unk_2A >= -0xF)) {
        switch (D_800E6280.unk_F88 & 0x70) {
        case 0x20:
            D_800B6990[D_800B6998] = 0;
            D_800B6998++;
            func_80044750(0x505);
            break;
        case 0x40:
            D_800B6990[D_800B6998] = 1;
            D_800B6998++;
            func_80044750(0x506);
            break;
        case 0x10:
            D_800B6990[D_800B6998] = 2;
            D_800B6998++;
            func_80044750(0x507);
            break;
        }
        if (D_800B6998 >= 7) {
            D_800B6998 = 0;
        }
        if (D_800E6280.unk_F88 & 0x70) {
            for (i = 0; i < 13; i++) {
                k = D_800B6998 + 7;
                for (j = 0; j < 7; j++, k++) {
                    if (D_800B6990[k % 7] != D_800B3778[i][j]) {
                        break;
                    }
                    if ((j == 6) && ((u32)(D_800E6280.unk_1BC[i].unk_0C.w << 28) >> 31)) {
                        if (i != 11) {
                            if (i == 12) {
                                D_801230D0 = 11;
                                D_800E6280.unk_F5F = 11;
                                D_800E6280.unk_1093 = 12;
                            } else {
                                D_801230D0 = i;
                                D_800E6280.unk_F5F = i;
                                D_800E6280.unk_1093 = i + 1;
                            }
                        }
                        *(s32 *)&D_800E6280.unk_1BC[i].unk_00 += rand() & 0x7FFF;
                        if (D_800E6280.unk_0F6.h & 1) {
                            D_800E6280.unk_11C.unk_02 += 0x20;
                        }
                        func_80042908(0x10);
                        func_80042940(2);
                        D_800E6280.unk_110D = 11;
                        func_8004E58C();
                        icon_disp_switch(0);
                        func_80065F34(0);
                        telephone_class_init();
                        k_sub_reset_point_set();
                        k_sub_disp_start(0);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
