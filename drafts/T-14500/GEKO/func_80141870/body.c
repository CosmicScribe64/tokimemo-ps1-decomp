s32 func_80141870(void) {
    s32 temp_v1;
    s32 var_v0;
    s32 var_v0_2;

    if ((check_end_k() != 0) && ((u16) D_800CA154 == 1)) {
        if ((u8) D_800E738D < 0x40U) {
            D_800E738D = (D_800E738D + 1) & 0xFF;
            var_v0 = 0;
        } else {
            D_800E738D = 0;
            k_reset(1);
            goto block_5;
        }
    } else {
block_5:
        if (((u16) D_800CA154 == 0) || ((u16) D_800CA154 == 8)) {
            func_80138B74();
        } else {
            func_80138AF8();
        }
        temp_v1 = D_800E7384;
        if (temp_v1 == 1) {
            var_v0_2 = temp_v1 ^ 1;
            if ((u16) D_800CA154 == 1) {
                func_80044750(0x609);
                goto block_12;
            }
        } else {
block_12:
            var_v0_2 = temp_v1 ^ 1;
        }
        var_v0 = var_v0_2 == 0;
        D_800E7384 = temp_v1 + 1;
        if (var_v0 != 0) {
            if ((u16) D_800CA154 == 9) {
                var_v0 = func_80044750(0x60B);
            }
        }
    }
    return var_v0;
}
