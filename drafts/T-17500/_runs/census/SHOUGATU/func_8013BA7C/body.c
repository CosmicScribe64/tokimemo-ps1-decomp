s32 func_8013BA7C(void) {
    s16 var_v1;
    u8 temp_a0;
    u8 var_v0;

    var_v0 = D_800E6280.unk_110D;
    if (var_v0 == 0) {
        func_800674B0();
        var_v0 = (D_800E6280.unk_110D + 1) & 0xFF;
    }
    D_800E6280.unk_110D = var_v0;
    if (var_v0 == 1) {
        if ((u32) D_800E7384++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
            goto block_8;
        }
        return 0;
    }
block_8:
    var_v1 = 0;
    do {
        temp_a0 = D_800E6280.unk_1BC[var_v1].unk_0C.b[1];
        if (temp_a0 & 1) {
            D_800E6280.unk_1BC[var_v1].unk_0C.b[1] = temp_a0 & 0xFFFE;
            if (var_v1 != D_800E6280.unk_F5F) {
                D_800E6280.unk_1BC[var_v1].unk_06 += 1;
            }
        }
        var_v1 += 1;
    } while (var_v1 < 0xB);
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += 8;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 8;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A -= 0xA;
    check_para_limit(temp_a0, &D_800E6280, 0x38);
    return func_80072B5C(1);
}
