s32 func_80075C84(void) {
    GameState *g = &D_800E6280;
    s32 i;
    s32 j;
    u32 k;

    if ((((DisplayActor44 *)D_8011ECD0)->unk_26 < -0x40) && (((DisplayActor44 *)D_8011ECD0)->unk_26 >= -0x4F)
        && (((DisplayActor44 *)D_8011ECD0)->unk_2A < 0x18) && (((DisplayActor44 *)D_8011ECD0)->unk_2A >= -0xF)) {
        switch (g->unk_F88 & 0x70) {
        case 0x20:
            D_800B6990.keys[D_800B6990.pos] = 0;
            D_800B6990.pos++;
            func_80044750(0x505);
            break;
        case 0x40:
            D_800B6990.keys[D_800B6990.pos] = 1;
            D_800B6990.pos++;
            func_80044750(0x506);
            break;
        case 0x10:
            D_800B6990.keys[D_800B6990.pos] = 2;
            D_800B6990.pos++;
            func_80044750(0x507);
            break;
        }
        if (D_800B6990.pos >= 7) {
            D_800B6990.pos = 0;
        }
        if (g->unk_F88 & 0x70) {
            for (i = 0; i < 13; i++) {
                k = D_800B6990.pos + 7;
                for (j = 0; j < 7; j++, k++) {
                    if (D_800B6990.keys[k % 7] != D_800B3778[i][j]) {
                        break;
                    }
                    if ((j == 6) && ((u32)(g->unk_1BC[i].unk_0C.w << 28) >> 31)) {
                        if (i != 11) {
                            if (i == 12) {
                                D_801230D0 = 11;
                                g->unk_F5F = 11;
                                g->unk_1093 = 12;
                            } else {
                                D_801230D0 = i;
                                g->unk_F5F = i;
                                g->unk_1093 = i + 1;
                            }
                        }
                        *(s32 *)g.unk_1BC[i].unk_00 += rand() & 0x7FFF;
                        if (g->unk_0F6.h & 1) {
                            g->unk_11C.unk_02 += 0x20;
                        }
                        func_80042908(0x10);
                        func_80042940(2);
                        g->unk_110D = 11;
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
