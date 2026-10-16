void func_8013ABAC(u8 arg0) {
    u8 var_v0;

    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        dec_bg_reset();
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
            goto block_8;
        }
        return;
    }
block_8:
    func_80072338();
    if (D_800CA2EC != 0) {
        D_800E6672 += 1;
    } else {
        D_800E6280.unk_1BC[D_800E71DF].unk_02 += 1;
        D_800E6280.unk_1BC[D_800E71DF].unk_06 += 1;
    }
    check_para_limit();
    func_80072B5C(1);
}
