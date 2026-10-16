s32 func_80134228(void) {
    s32 temp_v1;
    u8 var_v0;

    if (D_800E6280.unk_71E & 0x40) {
        func_80044750(0xCF);
        main_state_set(0x52);
        return 0;
    }
    var_v0 = D_800E6280.unk_110D;
    if (var_v0 == 0) {
        func_800674B0();
        var_v0 = (D_800E6280.unk_110D + 1) & 0xFF;
    }
    D_800E6280.unk_110D = var_v0;
    if (var_v0 == 1) {
        temp_v1 = D_800E6280.unk_1104.w;
        D_800E6280.unk_1104.w = temp_v1 + 1;
        if ((u32) temp_v1 >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            goto block_10;
        }
        return 0;
    }
block_10:
    return func_80072B5C(1);
}
