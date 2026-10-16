void func_80132B04(s32 arg0, s32 arg1, s32 arg2) {
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_t0;
    s32 temp_v0;

    if (D_80122D00 != 0) {
        if (func_8004ECB4() != 0) {
            if (D_80122CF0 != 0) {
                if (D_80122CF0 == 1) {
                    if (func_80046094() == 9) {
                        goto block_5;
                    }
                }
            } else {
block_5:
                temp_v0 = D_800E6280.unk_1104.w + 1;
                D_800E6280.unk_1104.w = temp_v0;
                if ((u32) temp_v0 >= 0x3CU) {
                    D_80122D00 = 0;
                    func_8004E9F4(1);
                }
            }
        }
    } else if (D_80122D18 != 0) {
        if ((func_8004ECB4() != 0) || ((func_8004ECB4() == 0) && (D_800E6280.unk_F80 == 0))) {
            if (D_80122CF0 != 0) {
                if (D_80122CF0 == 1) {
                    if (func_80046094() == 9) {
                        goto block_14;
                    }
                }
            } else {
block_14:
                D_80122D18 = 0;
            }
        }
    } else if (D_80122CE4 != 0) {
        if ((func_8004ECB4() != 0) || ((func_8004ECB4() == 0) && (D_800E6280.unk_F80 == 0))) {
            if (D_80122CF0 != 0) {
                if (D_80122CF0 == 1) {
                    if (func_80046094() == 9) {
                        goto block_22;
                    }
                }
            } else {
block_22:
                D_80122CE4 = 0;
                func_8004E9F4(1);
            }
        }
    } else {
        D_800E6280.unk_1104.w = 0;
        temp_v1 = *D_800CA134;
        temp_t0 = temp_v1 * 4;
        temp_v0_2 = *D_800CA138;
        func_80132420(*(*(D_800CA13C + temp_t0) + (temp_v0_2 * 4)), (s32) *(temp_v0_2 + *(D_800CA140 + temp_t0)), (s32) (u8) arg0, D_800CA144, (s32) temp_v1, (s32) temp_v0_2, (s32) (u8) arg2);
        if ((u8) D_8013C364 == 0) {
            if (((u8) arg1 != 0) && (D_800CA2A8 == 0)) {
                temp_v1_2 = *D_800CA134;
                temp_v0_3 = *D_800CA138;
                if (*(temp_v0_3 + *(D_800CA140 + (temp_v1_2 * 4))) == 0xFF) {
                    *D_800CA134 = (s16) (temp_v1_2 + 1);
                    *D_800CA138 = 0;
                } else {
                    *D_800CA138 = (s16) (temp_v0_3 + 1);
                }
            }
        }
    }
}
