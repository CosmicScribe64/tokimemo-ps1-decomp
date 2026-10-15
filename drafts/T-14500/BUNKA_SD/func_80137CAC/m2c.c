extern s32 D_800E7200;
extern u8 D_800E738A;

void func_80137CAC(void) {
    s32 var_at;
    u8 temp_v0;

    temp_v0 = D_800E738A;
    if ((temp_v0 >= 3U) && (temp_v0 < 0x2FU)) {
        var_at = temp_v0 < 3U;
        if (D_800E7200 & 0x20) {
            if (func_800460EC() != 2) {
                func_80044750(0x74);
            }
            func_80137D6C();
            goto block_6;
        }
    } else {
block_6:
        var_at = temp_v0 < 3U;
    }
    if ((var_at == 0) && (temp_v0 < 0x2DU) && (D_800E7200 & 0x40)) {
        func_80044750(0xC1);
        func_80044750(0xC4);
        func_80042940(0x2C);
    }
    func_80137D6C();
}
