/* T-24069: retain the earlier caller's K&R implicit-int view. */
s32 func_8013BFFC(void) {
    GameState *g = &D_800E6280;
    /* FAKE: join step/status loads through one word temporary to preserve
     * the original v1 allocation; original local declarations unknown. T-24069. */
    s32 value;
    switch (g->unk_110D) {
    case 0:
        func_8013FC08();
        D_801220D0 = 0x80000000;
        if (D_80150068 > 0) {
            D_80150070 = 0;
        }
        if ((value = D_80150070) == -1) {
            D_801500D4 = 0x800;
            func_8013FA7C();
            func_80042808();
        } else {
            g->unk_110D = (value = g->unk_110D) + 1;
        }
        break;
    case 1:
        if (D_80150068 > 0) {
            func_8013E89C(1);
            if ((value = D_80150070) >= 3) {
                if (D_80150068 == 1) {
                    D_80150070 = 0;
                } else {
                    D_80150068 = 0;
                    D_80150070 = 0;
                }
                g->unk_110D = (value = g->unk_110D) + 1;
            } else if ((value = D_80150070) == -1) {
                D_801500D4 = 0x800;
                func_8013FA7C();
                func_80042940(5);
            }
        } else {
            g->unk_110D = (value = g->unk_110D) + 1;
        }
        break;
    case 2:
        if (func_8013F3F8() == 1) {
            cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
            g->unk_110D = (value = g->unk_110D) + 1;
        } else {
            g->unk_110D = (value = g->unk_110D) + 1;
        }
        break;
    case 3:
        if (func_8013F3F8() == 0) {
            if (D_8014E0C0 == 1) {
                dec_bg_show_set(1, func_8005751C(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            func_8013EDF8();
            func_8013E364();
            g->unk_110D = (value = g->unk_110D) + 1;
        } else if ((func_8013F3F8() == 1) && (cd_read_sync_status() & 1)) {
            if (D_8014E0C0 == 1) {
                dec_bg_show_set(1, func_8005751C(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            if (func_8013F3F8() == 1) {
                func_8013F498();
            }
            func_8013EDF8();
            func_8013E364();
            g->unk_110D = (value = g->unk_110D) + 1;
        }
        break;
    case 4:
        func_8013D264();
        func_8013D2C0();
        func_8013EE28();
        func_801328A4(0x28, 0x3C);
        if (D_80150064 < 0xFF) {
            D_80150080 = D_80150064;
            D_80150064 += 1;
        }
        if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
            func_8013EEFC();
        }
        func_8004284C();
        break;
    }
    func_8013D32C();
}
