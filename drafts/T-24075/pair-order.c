/* NON_MATCHING: T-24075 source-only diagnostic, 12 resolved rows. */
s32 func_8013C8EC(void) {
    s32 *temp_v0;

    if ((D_800E6280.unk_F5F == 0xC) || (D_800E6280.unk_F5F == 0xB)) {
        func_8004284C();
    } else {
        switch (D_800E6280.unk_110D) {
        case 0:
            if (D_800E6280.unk_F5F == 0) {
                do {
                    D_801500C0 = func_800AE0D0() % D_8014E0A4[D_800E6280.unk_F5F];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 20) || (D_801500C0 == 0));
            } else if (D_800E6280.unk_F5F == 2) {
                do {
                    D_801500C0 = func_800AE0D0() % D_8014E0A4[D_800E6280.unk_F5F];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 8));
            } else if (D_800E6280.unk_F5F == 3) {
                do {
                    D_801500C0 = func_800AE0D0() % D_8014E0A4[D_800E6280.unk_F5F];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 9));
            } else if (D_800E6280.unk_F5F == 4) {
                do {
                    D_801500C0 = func_800AE0D0() % D_8014E0A4[D_800E6280.unk_F5F];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 13));
            } else if (D_800E6280.unk_F5F == 9) {
                do {
                    D_801500C0 = func_800AE0D0() % D_8014E0A4[D_800E6280.unk_F5F];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 0));
            } else {
                do {
                    D_801500C0 = func_800AE0D0() % D_8014E0A4[D_800E6280.unk_F5F];
                } while ((D_801500C0 == D_80150078));
            }
            D_80150078 = D_801500C0;
            temp_v0 = (D_801500C0 * 2) + D_801500C4;
            func_80046290(temp_v0[0], temp_v0[1], (s32) D_800E6280.unk_F5F);
            func_80044750(0x300);
            D_800E6280.unk_110D += 1;
            break;
        case 1:
            if (func_800460EC() & 4) {
                D_800E6280.unk_110D += 1;
            }
            break;
        case 2:
            if (func_800460EC() & 2) {
                func_8004284C();
            }
            break;
        }
    }
}
