s32 func_8013A3C4(void) {
    s32 temp_t4;
    s32 temp_t8;
    s32 var_v0;
    u8 temp_v1;

    if ((D_800E6280.unk_F88 & 0x400040) && ((u16) D_800CA154 != 0) && ((u16) D_800CA154 < 4U) && ((u32) D_800E6280.unk_1104.w >= 2U)) {
        func_80044750(0x1508);
        D_80122D00 = 0;
        func_8004E9F4(1);
        D_800CA2AC = 0;
        D_800CA154 = 3;
        goto block_10;
    }
    if ((func_8004ECB4() != 0) && ((u16) D_800CA154 < 4U)) {
        temp_v1 = D_800E6280.unk_110D;
        if (temp_v1 < 0x40U) {
            D_800E6280.unk_110D = (temp_v1 + 1) & 0xFF;
            var_v0 = 0;
        } else {
            D_800E6280.unk_110D = 0;
            func_8004E9F4(1);
            goto block_10;
        }
    } else {
block_10:
        if ((u16) D_800CA154 < 3U) {
            func_80138B74();
        } else {
            func_80138AF8();
        }
        temp_t4 = D_800E6280.unk_1104.w;
        D_800E6280.unk_1104.w = temp_t4 + 1;
        if ((temp_t4 == 0) && ((u16) D_800CA154 < 4U) && ((u16) D_800CA150 == 0)) {
            func_80044750(0x609);
        }
        temp_t8 = D_800E6280.unk_1104.w;
        var_v0 = temp_t8 == 1;
        D_800E6280.unk_1104.w = temp_t8 + 1;
        if (var_v0 != 0) {
            if ((u16) D_800CA154 == 5) {
                var_v0 = func_80044750(0x60A);
            }
        }
    }
    return var_v0;
}
