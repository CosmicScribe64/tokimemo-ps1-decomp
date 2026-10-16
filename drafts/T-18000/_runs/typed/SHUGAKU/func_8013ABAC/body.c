void func_8013ABAC(u8 arg0) {
    s32 temp_v1;
    u8 temp_t1;
    u8 temp_t5;
    u8 var_v0;

    var_v0 = D_800E6280.unk_110D;
    if (var_v0 == 0) {
        dec_bg_reset();
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
            goto block_8;
        }
        return;
    }
block_8:
    func_80072338();
    if (D_800CA2EC != 0) {
        D_800E6280.unk_1BC[0xA].unk_06 = D_800E6280.unk_1BC[0xA].unk_06 + 1;
    } else {
        temp_t1 = D_800E6280.unk_F5F;
        D_800E6280.unk_1BC[temp_t1].unk_02 += 1;
        temp_t5 = D_800E6280.unk_F5F;
        D_800E6280.unk_1BC[temp_t5].unk_06 += 1;
    }
    check_para_limit();
    func_80072B5C(1);
}
