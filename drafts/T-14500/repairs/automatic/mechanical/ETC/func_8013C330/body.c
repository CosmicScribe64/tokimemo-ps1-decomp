s32 func_8013C330(s32 arg0, s32 arg1) {
    switch (D_800E738D) {
    case 0:
        func_80132C3C();
        break;
    case 1:
        func_8013D570(0, 0);
        func_8013E89C(arg0);
        if (D_80150070 >= 3) {
            D_800E738D += 1;
            D_80150070 = 0;
        } else if (D_80150070 == -1) {
            D_800E738D += 1;
        }
        break;
    case 2:
        func_8013D570(0, 0);
        if ((D_800E71DF == 0xC) || (D_800E71DF == 0xB)) {
            func_8013F054();
        }
        if (((D_8015006C == 0) || (D_8015006C >= 3)) && ((D_800E7208 & 0x20) || (D_801500D8 == 0) || (D_801500D8 == 1))) {
            D_800E738D += 1;
            D_8015006C = 0;
        }
        break;
    case 3:
        func_8013D570(D_800E7384 * 2, arg1);
        if ((u32) D_800E7384 < 0x28U) {
            D_800E7384 += 1;
        } else {
            D_800E738D += 1;
        }
        break;
    case 4:
        func_8013FC08();
        if (D_80150070 == -1) {
            D_801500D4 = 0x800;
            func_8013FA7C();
            func_80042940(5);
        } else {
            D_800E738D += 1;
        }
        break;
    case 5:
        if (func_8013F3F8() == 1) {
            cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
            D_800E738D += 1;
        } else {
            D_800E738D += 1;
        }
        break;
    case 6:
        if (func_8013F3F8() == 0) {
            if (D_8014E0C0 == 1) {
                dec_bg_show_set(1, func_8005751C(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            func_8013EDF8();
            func_8013E364();
            D_800E738D += 1;
        }
        if ((func_8013F3F8() == 1) && (cd_read_sync_status() & 1)) {
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
            D_800E738D += 1;
        }
        break;
    case 7:
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
