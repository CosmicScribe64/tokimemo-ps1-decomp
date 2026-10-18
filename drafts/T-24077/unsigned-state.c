/* NON_MATCHING: T-24077 source-only diagnostic, 122 resolved rows. */
void func_8013CF2C(void) {
    /* Target reads the shared signed state field as an unsigned byte. T-24077. */
    s32 *temp_v0;

    if ((D_80150064 == 0) || (D_801500E0 == 1)) {
        (*(u8 *) &D_800E6280.unk_110C) = 5;
    }
    switch ((*(u8 *) &D_800E6280.unk_110C)) {
    case 0:
        if ((u32) D_800E6280.unk_1100 >= 0x79U) {
            if (D_800E6280.unk_F5F == 0) {
                func_8013F320(0x14);
                if (D_8015006C >= 3) {
                    (*(u8 *) &D_800E6280.unk_110C) += 1;
                    D_8015006C = 0;
                }
            } else if (D_800E6280.unk_F5F == 2) {
                func_8013F320(8);
                if (D_8015006C == 2) {
                    D_800E6280.unk_1100 = 0;
                }
                if (D_8015006C >= 3) {
                    if ((u32) D_800E6280.unk_1100 >= 0x3DU) {
                        (*(u8 *) &D_800E6280.unk_110C) += 1;
                        D_8015006C = 0;
                    } else {
                        D_800E6280.unk_1100 += 1;
                    }
                }
            } else if (D_800E6280.unk_F5F == 4) {
                func_8013F320(0xD);
                if (D_8015006C >= 3) {
                    (*(u8 *) &D_800E6280.unk_110C) += 1;
                    D_8015006C = 0;
                }
            } else if (D_800E6280.unk_F5F == 0xC) {
                func_8013F320(7);
                if (D_8015006C >= 3) {
                    (*(u8 *) &D_800E6280.unk_110C) += 1;
                    D_8015006C = 0;
                }
            } else {
                (*(u8 *) &D_800E6280.unk_110C) += 1;
            default: ;
            }
        }
        break;
    case 1:
        temp_v0 = D_8014DBF8[D_800E6280.unk_F5F];
        func_80046290(temp_v0[0], temp_v0[1], (s32) D_800E6280.unk_F5F);
        func_80044750(0x300);
        (*(u8 *) &D_800E6280.unk_110C) += 1;
        break;
    case 2:
        if (func_800460EC() & 4) {
            (*(u8 *) &D_800E6280.unk_110C) += 1;
        }
        break;
    case 3:
        if (func_800460EC() & 2) {
            D_800E6280.unk_1100 = 0;
            (*(u8 *) &D_800E6280.unk_110C) += 1;
        }
        break;
    case 4:
        if ((u32) D_800E6280.unk_1100 >= 0x79U) {
            D_800E6280.unk_1100 = 0;
            (*(u8 *) &D_800E6280.unk_110C) += 1;
        }
        break;
    case 5:
        func_8013E1DC((s32) D_800E6280.unk_1100);
        if ((u32) D_800E6280.unk_1100 >= 0x12DU) {
            D_801500E0 = 0;
            D_80150070 = 0;
            D_80150064 = 0;
            D_801500D8 = 0;
            D_801500DC = 0;
            D_801217D0[64].unk_00 = 0x80000000;
            D_801217D0[65].unk_00 = 0x80000000;
            D_801500D4 = 0xC00;
            func_8013FA7C();
            func_80042908(0xFF);
        }
        break;
    }
}
