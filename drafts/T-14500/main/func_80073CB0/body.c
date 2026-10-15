void func_80073CB0(void) {
    s32 sp28;
    s32 var_v1;
    u32 temp_t6;
    u32 temp_t9;
    u8 temp_a0;
    u8 temp_t3;
    u8 temp_t5;
    u8 var_t0;

    D_801230D0 = 0xFF;
    temp_a0 = D_800E6280.unk_69C[D_800E699C].unk_03;
    if (temp_a0 != 0) {
        var_v1 = 1;
    } else {
        var_v1 = 0;
    }
    if ((D_800E6280.unk_69C[D_800E699C].unk_01 == 3) && (temp_a0 == 5)) {
        var_v1 = 2;
    }
    sp28 = var_v1;
    if (func_80066A84() == 1) {
        temp_t3 = D_800E6280.unk_69C[D_800E699C].unk_01;
        switch (temp_t3) {
        case 0:
            D_801230D0 = (s32) *(&D_800B68C0 + var_v1);
            break;
        case 1:
            D_801230D0 = (s32) *(&D_800B68C2 + var_v1);
            break;
        case 2:
            D_801230D0 = (s32) *(&D_800B68C4 + var_v1);
            break;
        case 3:
            D_801230D0 = (s32) *(&D_800B68C6 + var_v1);
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) *(&D_800B68C9 + var_v1);
            break;
        case 6:
            D_801230D0 = (s32) *(&D_800B68CB + var_v1);
            break;
        case 7:
            D_801230D0 = (s32) D_800B68CD;
            break;
        }
    } else {
        temp_t5 = D_800E6280.unk_69C[D_800E699C].unk_01;
        switch (temp_t5) {
        case 0:
            D_801230D0 = (s32) *(&D_800B68D0 + var_v1);
            break;
        case 1:
            D_801230D0 = (s32) *(&D_800B68D2 + var_v1);
            break;
        case 2:
            D_801230D0 = (s32) *(&D_800B68D4 + var_v1);
            break;
        case 3:
            D_801230D0 = (s32) *(&D_800B68D6 + var_v1);
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) *(&D_800B68D9 + var_v1);
            break;
        case 6:
            D_801230D0 = (s32) *(&D_800B68DB + var_v1);
            break;
        case 7:
            D_801230D0 = (s32) D_800B68DD;
            break;
        }
    }
    if (D_801230D0 == 0xFF) {
        sp28 = var_v1;
        if (func_80066A84() == 1) {
            temp_t6 = (u16) D_800E6374 >> 0xC;
            switch (temp_t6) {
            case 0:
                D_801230D0 = (s32) *(&D_800B6890 + var_v1);
                break;
            case 1:
                D_801230D0 = (s32) *(&D_800B6892 + var_v1);
                break;
            case 2:
                D_801230D0 = (s32) *(&D_800B6894 + var_v1);
                break;
            case 3:
                var_t0 = *(&D_800B6896 + var_v1);
                goto block_55;
            case 4:
                D_801230D0 = (s32) *(&D_800B6898 + var_v1);
                break;
            case 5:
                D_801230D0 = (s32) *(&D_800B689A + var_v1);
                break;
            case 6:
                D_801230D0 = (s32) *(&D_800B689C + var_v1);
                break;
            case 7:
                D_801230D0 = (s32) *(&D_800B689E + var_v1);
                break;
            case 8:
                D_801230D0 = (s32) *(&D_800B68A0 + var_v1);
                break;
            case 9:
                D_801230D0 = (s32) *(&D_800B68A2 + var_v1);
                break;
            case 10:
                D_801230D0 = (s32) *(&D_800B68A4 + var_v1);
                break;
            }
        } else {
            temp_t9 = (u16) D_800E6374 >> 0xC;
            switch (temp_t9) {
            case 0:
                var_t0 = *(&D_800B68A8 + var_v1);
block_55:
                D_801230D0 = (s32) var_t0;
                break;
            case 1:
                D_801230D0 = (s32) *(&D_800B68AA + var_v1);
                break;
            case 2:
                D_801230D0 = (s32) *(&D_800B68AC + var_v1);
                break;
            case 3:
                D_801230D0 = (s32) *(&D_800B68AE + var_v1);
                break;
            case 4:
                D_801230D0 = (s32) *(&D_800B68B0 + var_v1);
                break;
            case 5:
                D_801230D0 = (s32) *(&D_800B68B2 + var_v1);
                break;
            case 6:
                D_801230D0 = (s32) *(&D_800B68B4 + var_v1);
                break;
            case 7:
                D_801230D0 = (s32) *(&D_800B68B6 + var_v1);
                break;
            case 8:
                D_801230D0 = (s32) *(&D_800B68B8 + var_v1);
                break;
            case 9:
                D_801230D0 = (s32) *(&D_800B68BA + var_v1);
                break;
            case 10:
                var_t0 = *(&D_800B68BC + var_v1);
                goto block_55;
            }
        }
    }
}
