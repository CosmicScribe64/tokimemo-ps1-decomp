void func_80132B04(s32 arg0, s32 arg1, s32 arg2) {
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_t0;
    u32 temp_v0;

    if (D_80122D00 != 0) {
        if (check_end_k() != 0) {
            if (D_80122CF0 != 0) {
                if (D_80122CF0 == 1) {
                    if (func_80046094() == 9) {
                        goto block_5;
                    }
                }
            } else {
block_5:
                temp_v0 = D_800E7384 + 1;
                D_800E7384 = temp_v0;
                if (temp_v0 >= 0x3CU) {
                    D_80122D00 = 0;
                    k_reset(1);
                }
            }
        }
    } else if (D_80122D18 != 0) {
        if ((check_end_k() != 0) || ((check_end_k() == 0) && (D_800E7200 == 0))) {
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
        if ((check_end_k() != 0) || ((check_end_k() == 0) && (D_800E7200 == 0))) {
            if (D_80122CF0 != 0) {
                if (D_80122CF0 == 1) {
                    if (func_80046094() == 9) {
                        goto block_22;
                    }
                }
            } else {
block_22:
                D_80122CE4 = 0;
                k_reset(1);
            }
        }
    } else {
        D_800E7384 = 0;
        temp_v1 = *D_800CA134;
        temp_t0 = temp_v1 * 4;
        temp_v0_2 = *D_800CA138;
        func_80132420(*(s32 *)(*(s32 *)(D_800CA13C + temp_t0) + (temp_v0_2 * 4)), (s32) *(s32 *)(temp_v0_2 + *(s32 *)(D_800CA140 + temp_t0)), (s32) (u8) arg0, D_800CA144, (s32) temp_v1, (s32) temp_v0_2, (s32) (u8) arg2);
        if ((u8) D_8013C364 == 0) {
            if (((u8) arg1 != 0) && (D_800CA2A8 == 0)) {
                temp_v1_2 = *D_800CA134;
                temp_v0_3 = *D_800CA138;
                if (*(s32 *)(temp_v0_3 + *(s32 *)(D_800CA140 + (temp_v1_2 * 4))) == 0xFF) {
                    *D_800CA134 = (s16) (temp_v1_2 + 1);
                    *D_800CA138 = 0;
                } else {
                    *D_800CA138 = (s16) (temp_v0_3 + 1);
                }
            }
        }
    }
}
