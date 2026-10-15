extern s32 D_800E7208;
extern u32 D_800E7384;
extern u8 D_800E738D;

s32 func_8013A3C4(void) {
    s32 var_v0;

    if ((D_800E7208 & 0x400040) && ((u16) D_800CA154 != 0) && ((u16) D_800CA154 < 4U) && ((u32) D_800E7384 >= 2U)) {
        func_80044750(0x1508);
        D_80122D00 = 0;
        func_8004E9F4(1);
        D_800CA2AC = 0;
        D_800CA154 = 3;
        goto block_10;
    }
    if ((func_8004ECB4() != 0) && ((u16) D_800CA154 < 4U)) {
        if ((u8) D_800E738D < 0x40U) {
            D_800E738D = (D_800E738D + 1) & 0xFF;
            var_v0 = 0;
        } else {
            D_800E738D = 0;
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
        if ((D_800E7384++ == 0) && ((u16) D_800CA154 < 4U) && ((u16) D_800CA150 == 0)) {
            func_80044750(0x609);
        }
        var_v0 = D_800E7384 == 1;
        D_800E7384 += 1;
        if (var_v0 != 0) {
            if ((u16) D_800CA154 == 5) {
                var_v0 = func_80044750(0x60A);
            }
        }
    }
    return var_v0;
}
