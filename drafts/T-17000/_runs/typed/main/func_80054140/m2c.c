s32 func_80054140(void) {
    u8 temp_a0;
    u8 temp_t0;
    u8 temp_v0;

    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        temp_v0 = D_800E6280.unk_111C;
        if (temp_v0 == 0) {
            D_800E6280.unk_1115 = 0;
            D_800E6280.unk_111C = temp_v0;
            D_800E6280.unk_1118 = temp_v0;
            if (func_80054AF4((s32) temp_v0) == 1) {
                D_800E8BEE = 1;
                func_80053CC0();
            } else {
                temp_t0 = (D_800E6280.unk_111D + 1) & 0xFF;
                D_800E6280.unk_111D = temp_t0;
                if (temp_t0 >= 3U) {
                    D_800E8BEE = 0;
                    func_80053CC0();
                }
            }
        } else if (temp_v0 < 0xFU) {
            D_800E6280.unk_111C = temp_v0;
            temp_a0 = temp_v0 & 0xFF;
            D_800E6280.unk_1118 = temp_a0;
            if (func_80054AF4((s32) temp_a0) == 1) {
                D_800E7D14[D_800E6280.unk_1118] = 1;
            } else {
                D_800E7D14[D_800E6280.unk0] = 0;
            }
            D_800E6280.unk_111C += 1;
        } else {
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 2;
        }
    }
    return (s32) D_800E6280.unk_1115;
}
