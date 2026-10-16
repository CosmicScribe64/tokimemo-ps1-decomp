s32 func_80141870(void) {
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;
    u8 temp_v1;

    if ((check_end_k() != 0) && ((u16) D_800CA154 == 1)) {
        temp_v1 = D_800E6280.unk_110D;
        if (temp_v1 < 0x40U) {
            D_800E6280.unk_110D = (temp_v1 + 1) & 0xFF;
            var_v0 = 0;
        } else {
            D_800E6280.unk_110D = 0;
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
        var_v1 = D_800E6280.unk_1104.w;
        if (var_v1 == 1) {
            var_v0_2 = var_v1 ^ 1;
            if ((u16) D_800CA154 == 1) {
                func_80044750(0x609);
                var_v1 = D_800E6280.unk_1104.w;
                goto block_12;
            }
        } else {
block_12:
            var_v0_2 = var_v1 ^ 1;
        }
        var_v0 = var_v0_2 == 0;
        D_800E6280.unk_1104.w = var_v1 + 1;
        if (var_v0 != 0) {
            if ((u16) D_800CA154 == 9) {
                var_v0 = func_80044750(0x60B);
            }
        }
    }
    return var_v0;
}
