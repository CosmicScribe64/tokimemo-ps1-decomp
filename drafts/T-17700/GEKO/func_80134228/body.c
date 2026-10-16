s32 func_80134228(void) {
    u8 state;

    if (D_800E6280.unk_71E & 0x40) {
        func_80044750(0xCF);
        main_state_set(0x52);
        return 0;
    }
    state = D_800E6280.unk_110D;
    if (state == 0) {
        func_800674B0();
        state = D_800E6280.unk_110D + 1;
    }
    D_800E6280.unk_110D = state;
    if (state == 1) {
        if ((u32)D_800E6280.unk_1104.w++ >= 0x400) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    return func_80072B5C(1);
}
