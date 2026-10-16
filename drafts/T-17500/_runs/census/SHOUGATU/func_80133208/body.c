s32 func_80133208(void) {
    s16 temp_v0;
    s32 temp_a0;
    s32 var_v0;

    if ((D_800E6280.unk_F88 & 0x400040) && (D_80143B04 > 0) && (D_80143B04 < 5) && ((u32) D_800E6280.unk_1104.u >= 2U)) {
        func_80044750(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        D_800CA2AC = 0;
        D_80143B04 = 4;
        goto block_19;
    }
    if ((check_end_k() != 0) && (D_80143B04 < 5)) {
        if (D_80143B04 == 0) {
            temp_v0 = D_80143BC0 + 1;
            if (D_80143BC0 < 0x5A) {
                D_80143BC0 = temp_v0;
                if (temp_v0 == 1) {
                    func_80044750(0x60A);
                }
                if (D_80143BC0 == 0x1E) {
                    func_80044750(0x604);
                }
                return 0;
            }
            if (!(D_800E6280.unk_110D & 7)) {
                temp_a0 = (((*(&D_800B3778 + ((D_800E6280.unk_F5F * 7) + ((u8) D_800E6280.unk_110D >> 3))) + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                D_800E6280.unk_110D = D_800E6280.unk_110D;
                func_80044750(temp_a0);
            }
            goto block_16;
        }
block_16:
        if ((u8) D_800E6280.unk_110D < 0x28U) {
            D_800E6280.unk_110D = (D_800E6280.unk_110D + 1) & 0xFF;
            return 0;
        }
        D_800E6280.unk_110D = 0;
        k_reset(1);
        goto block_19;
    }
block_19:
    var_v0 = func_801323D4();
    if ((D_80143B04 < 5) && (D_800E6280.unk_1104.u == 1)) {
        var_v0 = func_80044750(0x608);
    }
    D_800E6280.unk_1104.u += 1;
    return var_v0;
}
