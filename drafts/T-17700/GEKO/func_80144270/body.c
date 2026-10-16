void func_80144270(void) {
    s32 sp33;
    s32 sp3A;
    s32 sp2F;
    s32 sp39;
    s32 sp2E;
    s32 sp37;
    s32 sp36;
    s32 sp31;
    s32 sp34;
    s32 sp38;
    s32 sp3B;
    GekoBytes21 sp2C;
    s32 var_at;
    u8 temp_a0;
    u8 temp_a1;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_F5F;
    sp2C = D_80147C6C;
    if ((temp_v1 == 1) && (D_800E6280.unk_03E == 0x60) && ((temp_v0 = D_800E6280.unk_03F, (temp_v0 == 0xC)) || (temp_v0 == 1) || ((temp_v0 == 2) && ((u8) D_800E6280.unk_040 < 0x15U))) && (D_800E6280.unk_56C[0xC] == 0)) {
        D_801476E0 = sp3A;
        D_800E6280.unk_120[0xE] = 1;
    }
    temp_a1 = D_800E6280.unk_040;
    temp_a0 = D_800E6280.unk_03E;
    temp_v0_2 = D_800E6280.unk_03F;
    if ((temp_v1 == 3) && (temp_a0 == 0x61) && ((temp_v0_2 == 9) || (temp_v0_2 == 0xA) || ((temp_v0_2 == 0xB) && (temp_a1 < 0x15U))) && (D_800E6280.unk_56C[0x18] == 0)) {
        D_801476E0 = sp33;
        D_800E6280.unk_120[7] = 1;
    }
    if ((temp_v1 == 4) && ((var_at = temp_a0 < 0x60U, (temp_v0_2 == 9)) || ((temp_v0_2 == 0xA) && (var_at = temp_a0 < 0x60U, ((temp_a1 < 0x15U) != 0)))) && (var_at == 0) && (D_800E6280.unk_56C[0x1E] == 0)) {
        D_801476E0 = sp39;
        D_800E6280.unk_120[0xD] = 1;
    }
    if ((temp_v1 == 7) && ((temp_v0_2 == 3) || (temp_v0_2 == 4) || ((temp_v0_2 == 5) && (temp_a1 < 0x15U))) && (D_800E6280.unk_56C[0x2F] == 0)) {
        D_801476E0 = sp2E;
        D_800E6280.unk_120[2] = 1;
    }
    if (temp_v1 == 0xA) {
        if (temp_a0 >= 0x60U) {
            if ((temp_v0_2 == 0xA) && (temp_a1 < 0x15U) && (D_800E6280.unk_56C[0x47] == 0)) {
                D_801476E0 = sp39;
                D_800E6280.unk_120[0xD] = 1;
            }
        }
    }
    if (temp_v1 == 8) {
        if ((((temp_a0 == 0x5F) && (temp_v0_2 == 0xC)) || ((temp_a0 == 0x60) && (temp_v0_2 == 1)) || ((temp_a0 == 0x60) && (temp_v0_2 == 2) && (temp_a1 < 0x15U))) && (D_800E6280.unk_1BC[8].unk_14[6] == 0)) {
            D_801476E0 = sp2F;
            D_800E6280.unk_120[3] = 1;
        }
        if ((temp_a0 == 0x60) && (D_800E6280.unk_1BC[8].unk_14[9] == 0)) {
            if (temp_v0_2 != 6) {
                if (temp_v0_2 == 7) {
                    goto block_52;
                }
            } else {
block_52:
                D_801476E0 = sp31;
                D_800E6280.unk_120[5] = 1;
            }
        }
        if ((temp_a0 == 0x60) && (D_800E6280.unk_1BC[8].unk_14[0x12] == 0)) {
            if (temp_v0_2 != 9) {
                if ((temp_v0_2 == 0xA) || ((temp_v0_2 == 0xB) && (temp_a1 < 0x15U))) {
                    goto block_60;
                }
            } else {
block_60:
                D_801476E0 = sp36;
                D_800E6280.unk_120[0xA] = 1;
            }
        }
        if ((temp_a0 == 0x61) && (D_800E6280.unk_1BC[8].unk_14[0x13] == 0)) {
            if (temp_v0_2 != 3) {
                if ((temp_v0_2 == 4) || ((temp_v0_2 == 5) && (temp_a1 < 0x15U))) {
                    goto block_68;
                }
            } else {
block_68:
                D_801476E0 = sp37;
                D_800E6280.unk_120[0xB] = 1;
            }
        }
        if ((temp_a0 == 0x61) && (D_800E6280.unk_56C[0x39] == 0)) {
            if (temp_v0_2 != 9) {
                if ((temp_v0_2 == 0xA) || ((temp_v0_2 == 0xB) && (temp_a1 < 0x15U))) {
                    goto block_76;
                }
            } else {
block_76:
                D_801476E0 = sp3B;
                D_800E6280.unk_120[0xF] = 1;
            }
        }
        if ((temp_a0 == 0x61) && ((temp_v0_2 == 0xC) || (temp_v0_2 == 1) || ((temp_v0_2 == 2) && (temp_a1 < 0xBU)))) {
            D_801476E0 = sp38;
            D_800E6280.unk_120[0xC] = 1;
        }
        if (((func_80144198(0) != 0x5F) || (func_80144198(1) != 0xC) || (func_80144198(2) != 0xA)) && ((func_80144198(0) != 0x60) || (func_80144198(1) != 0xC) || (func_80144198(2) != 8))) {
            if ((func_80144198(0) == 0x61) && (func_80144198(1) == 0xC) && (func_80144198(2) == 7)) {
                goto block_93;
            }
        } else {
block_93:
            D_801476E0 = sp34;
            D_800E6280.unk_120[8] = 1;
        }
    }
}
