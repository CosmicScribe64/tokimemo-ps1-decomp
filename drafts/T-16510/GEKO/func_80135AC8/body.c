s32 func_80135AC8(void) {
    u8 var_v0;

    if (D_800E699E & 0x20) {
        func_80044750(0xCF);
        main_state_set(0x52);
        return 0;
    }
    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        func_800674B0();
        var_v0 = (D_800E738D + 1) & 0xFF;
    }
    D_800E738D = var_v0;
    if (var_v0 == 1) {
        if ((u32) D_800E7384++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E738D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E738D += 1;
            goto block_10;
        }
        return 0;
    }
block_10:
    D_80122D4C = 0;
    return func_80072B5C(1);
}
