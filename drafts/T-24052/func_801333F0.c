s32 func_801333F0(void) {
    s32 sound_id;

    if ((D_800E6280.unk_F88 & 0x400040) &&
        D_80143B04 > 0 && D_80143B04 < 3 &&
        (u32)D_800E6280.unk_1104.w >= 2U) {
        func_80044750(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        D_800CA2AC = 0;
        D_80143B04 = 2;
    } else {
        if ((check_end_k() != 0) && (D_80143B04 < 3)) {
            if (D_80143B04 == 0) {
                if (D_80143BC0 < 0x5A) {
                    D_80143BC0 += 1;
                    if (D_80143BC0 == 1) {
                        func_80044750(0x60A);
                    }
                    if (D_80143BC0 == 0x1E) {
                        func_80044750(0x604);
                    }
                    return 0;
                }
                if (!(D_800E6280.unk_110D & 7)) {
                    sound_id = (((D_800B3778[D_800E6280.unk_F5F][D_800E6280.unk_110D >> 3] + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                    /* FAKE: retain the original unchanged-byte store before the call.
                     * Direct self-assignment is removed; original spelling unknown. T-24052. */
                    D_800E6280.unk_110D |= 0;
                    func_80044750(sound_id);
                }
            }
            if (D_800E6280.unk_110D++ < 0x28U) {
                return 0;
            }
            D_800E6280.unk_110D = 0;
            k_reset(1);
        }
    }
    func_801323D4();
    if ((D_80143B04 < 3) && (D_800E6280.unk_1104.w == 1)) {
        /* FAKE: preserve the original unchanged halfword store. T-24052. */
        D_80143B04 = D_80143B04;
        func_80044750(0x608);
    } else {
        /* FAKE: preserve the original unchanged halfword store. T-24052. */
        D_80143B04 = D_80143B04;
        if ((D_80143B04 == 3) && (D_800E6280.unk_1104.w == 1)) {
            func_80044750(0x60B);
        }
    }
    D_800E6280.unk_1104.w += 1;
}