/* Same steps as the matched func_8013C330 with the background redraw first (T-17300). */
s32 func_8013C700(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8013D32C();
        func_80132C3C();
    default:
        break;
    case 1:
        func_8013D32C();
        func_8013D570(0, 0);
        func_8013E89C(5);
        if (D_80150070 >= 3) {
            D_800E6280.unk_110D += 1;
            D_80150070 = 0;
        } else if (D_80150070 == -1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_8013D32C();
        func_8013D570(0, 0);
        if (D_800E6280.unk_F5F == 0xC || D_800E6280.unk_F5F == 0xB) {
            func_8013F054();
        }
        if ((D_8015006C == 0 || D_8015006C >= 3) && ((D_800E6280.unk_F88 & 0x20) || D_801500D8 == 0 || D_801500D8 == 1)) {
            D_800E6280.unk_110D += 1;
            D_8015006C = 0;
        }
        break;
    case 3:
        func_8013D32C();
        func_8013D570(D_800E6280.unk_1104.u * 2, 3);
        if ((u32) D_800E6280.unk_1104.u < 0x28U) {
            D_800E6280.unk_1104.u += 1;
        } else {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        func_8013FC08();
        D_801500D4 = 0x800;
        func_8013FA7C();
        func_8004284C();
        break;
    }
}
