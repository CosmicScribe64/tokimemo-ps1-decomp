/* NON_MATCHING: T-24075 source-only diagnostic, 116 resolved rows. */
s32 func_8013C8EC(void) {
    u32 temp_hi;
    u8 *temp_v0;

    if ((D_800E6280.unk_F5F == 0xC) || (D_800E6280.unk_F5F == 0xB)) {
        func_8004284C();
    } else {
        switch (D_800E6280.unk_110D) {
        case 0:
            if (D_800E6280.unk_F5F == 0) {
                do {
loop_9:
                    temp_hi = func_800AE0D0() % (u8) D_8014E0A4[D_800E6280.unk_F5F];
                    D_801500C0 = (s32) temp_hi;
                    if (temp_hi == D_80150078) {
                        goto loop_9;
                    }
                    D_801500C0 = (s32) temp_hi;
                    if (temp_hi == 0x14) {
                        goto loop_9;
                    }
                    D_801500C0 = (s32) temp_hi;
                } while (temp_hi == 0);
            } else if (D_800E6280.unk_F5F == 2) {
                do {
loop_15:
                    temp_hi = func_800AE0D0() % (u8) D_8014E0A4[D_800E6280.unk_F5F];
                    D_801500C0 = (s32) temp_hi;
                    if (temp_hi == D_80150078) {
                        goto loop_15;
                    }
                    D_801500C0 = (s32) temp_hi;
                } while (temp_hi == 8);
            } else if (D_800E6280.unk_F5F == 3) {
                do {
loop_20:
                    temp_hi = func_800AE0D0() % (u8) D_8014E0A4[D_800E6280.unk_F5F];
                    D_801500C0 = (s32) temp_hi;
                    if (temp_hi == D_80150078) {
                        goto loop_20;
                    }
                    D_801500C0 = (s32) temp_hi;
                } while (temp_hi == 9);
            } else if (D_800E6280.unk_F5F == 4) {
                do {
loop_25:
                    temp_hi = func_800AE0D0() % (u8) D_8014E0A4[D_800E6280.unk_F5F];
                    D_801500C0 = (s32) temp_hi;
                    if (temp_hi == D_80150078) {
                        goto loop_25;
                    }
                    D_801500C0 = (s32) temp_hi;
                } while (temp_hi == 0xD);
            } else if (D_800E6280.unk_F5F == 9) {
                do {
loop_30:
                    temp_hi = func_800AE0D0() % (u8) D_8014E0A4[D_800E6280.unk_F5F];
                    D_801500C0 = (s32) temp_hi;
                    if (temp_hi == D_80150078) {
                        goto loop_30;
                    }
                    D_801500C0 = (s32) temp_hi;
                } while (temp_hi == 0);
            } else {
                do {
                    temp_hi = func_800AE0D0() % (u8) D_8014E0A4[D_800E6280.unk_F5F];
                    D_801500C0 = (s32) temp_hi;
                } while (temp_hi == D_80150078);
            }
            D_80150078 = D_801500C0;
            temp_v0 = (u8 *) (D_801500C4 + (D_801500C0 * 2));
            func_80046290((*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), (s32) D_800E6280.unk_F5F);
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
